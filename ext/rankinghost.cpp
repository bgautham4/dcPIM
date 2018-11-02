#include "assert.h"

#include "../coresim/event.h"
#include "../coresim/topology.h"
#include "../coresim/debug.h"

#include "factory.h"
#include "rankingflow.h"
#include "rankinghost.h"
#include "rankingTopology.h"

#include "../run/params.h"

extern uint32_t total_finished_flows;
extern double get_current_time();
extern void add_to_event_queue(Event*);
extern DCExpParams params;
extern Topology *topology;


TokenProcessingEvent::TokenProcessingEvent(double time, RankingHost *h, bool is_timeout)
    : Event(TOKEN_PROCESSING, time) {
        this->host = h;
        this->is_timeout_evt = is_timeout;
    }

TokenProcessingEvent::~TokenProcessingEvent() {
    if (host->token_send_evt == this) {
        host->token_send_evt = NULL;
    }
}

void TokenProcessingEvent::process_event() {
    this->host->token_send_evt = NULL;
    this->host->send_token();
}


RankingHostWakeupProcessingEvent::RankingHostWakeupProcessingEvent(double time, RankingHost *h)
    : Event(RANKINGHOST_WAKEUP_PROCESSING, time) {
        this->host = h;
    }

RankingHostWakeupProcessingEvent::~RankingHostWakeupProcessingEvent() {
    if (host->wakeup_evt == this) {
        host->wakeup_evt = NULL;
    }
}

void RankingHostWakeupProcessingEvent::process_event() {
    this->host->wakeup_evt = NULL;
    this->host->wakeup();
}



ListRTSComparator::ListRTSComparator() {
    for (uint i = 0; i < params.num_hosts; i++){
        this->ranking.push_back(rand());
    }
}
bool ListRTSComparator::operator() (ListRTS* a, ListRTS* b) {
    return this->ranking[a->dst->id] > this->ranking[b->dst->id];
}

RankingHost::RankingHost(uint32_t id, double rate, uint32_t queue_type) : SchedulingHost(id, rate, queue_type) {
    this->active_receiving_flow = NULL;
    this->active_sending_flow = NULL;
    this->host_proc_event = NULL;
    this->host_type = RANKING_HOST;
    this->total_token_schd_evt_count = 0;
}

// ---- Sender -------
void RankingHost::receive_token(RankingToken* pkt) {
    assert(this->active_sending_flow == NULL || this->active_sending_flow == (RankingFlow*)pkt->flow);
    this->active_sending_flow = dynamic_cast<RankingFlow*>(pkt->flow);
    Token* t = new Token();
    t->timeout = get_current_time() + pkt->ttl;
    t->seq_num = pkt->token_seq_num;
    t->data_seq_num = pkt->data_seq_num;
    this->active_sending_flow->tokens.push_back(t);
    this->active_sending_flow->remaining_pkts_at_sender = pkt->remaining_sz;
    if(this->host_proc_event == NULL) {
        this->schedule_host_proc_evt();
    }
            // if(CAPABILITY_MEASURE_WASTE)
        // {
        //     if(this->has_sibling_idle_source())
        //         c->has_idle_sibling_sender = true;
        //     else
        //         c->has_idle_sibling_sender = false;
        // }
}

void RankingHost::schedule_host_proc_evt(){
    assert(this->host_proc_event == NULL);

    double qpe_time = 0;
    double td_time = 0;
    if(this->queue->busy){
        qpe_time = this->queue->queue_proc_event->time;
    }
    else{
        qpe_time = get_current_time();
    }

    uint32_t queue_size = this->queue->bytes_in_queue;
    td_time = this->queue->get_transmission_delay(queue_size);

    this->host_proc_event = new HostProcessingEvent(qpe_time + td_time + INFINITESIMAL_TIME, this);
    add_to_event_queue(this->host_proc_event);
}

void RankingHost::send(){
    assert(this->host_proc_event == NULL);
    if(this->queue->busy)
    {
        schedule_host_proc_evt();
    }
    else
    {
        assert(this->active_sending_flow != NULL);
        if (this->active_sending_flow->has_token()) {
            this->active_sending_flow->send_pending_data();
        }
    }
}

// ---- Receiver -----
void RankingHost::schedule_token_proc_evt(double time, bool is_timeout)
{
    assert(this->token_send_evt == NULL);
    this->token_send_evt = new TokenProcessingEvent(get_current_time() + time + INFINITESIMAL_TIME, this, is_timeout);
    add_to_event_queue(this->token_send_evt);
}

void RankingHost::receive_rts(RankingRTS* pkt) {
    this->pending_flows.push_back(pkt->flow);
    if(this->active_receiving_flow == NULL) {
        // send list RTS
        send_listRTS();
        if(this->wakeup_evt == NULL) {
            schedule_wakeup_event();
        }
    }
}

void RankingHost::receive_nrts(RankingNRTS* pkt) {
    this->active_receiving_flow->sending_nrts_to_arbiter();
    this->active_receiving_flow = NULL;
    if(!this->pending_flows.empty()) {
        this->send_listRTS();
        if(this->wakeup_evt == NULL) {
            schedule_wakeup_event();
        }
    }
}
void RankingHost::send_listRTS() {
    if(debug_host(this->id))
        std::cout << get_current_time() << " send listRTS of dst:" << this->id << "\n";
    RankingListRTS* listRTS = new RankingListRTS(dynamic_cast<RankingTopology*>(topology)->arbiter->fake_flow,
     this, dynamic_cast<RankingTopology*>(topology)->arbiter , this, this->pending_flows);
    add_to_event_queue(new PacketQueuingEvent(get_current_time(), listRTS, this->queue));
} 

void RankingHost::schedule_wakeup_event() {
    assert(this->wakeup_evt == NULL);
    this->wakeup_evt = new RankingHostWakeupProcessingEvent(get_current_time() + params.rankinghost_idle_timeout, this);
    add_to_event_queue(this->wakeup_evt);
}

void RankingHost::wakeup() {
    assert(this->wakeup_evt == NULL);
    if(!this->pending_flows.empty()) {
        this->send_listRTS();
        this->schedule_wakeup_event();
    }
}
void RankingHost::send_token() {
    assert(this->active_receiving_flow != NULL);
    assert(this->token_send_evt == NULL);
    bool token_sent = false;
    this->total_token_schd_evt_count++;
    double closet_timeout = 999999;

    // if(CAPABILITY_HOLD && this->hold_on > 0){
    //     hold_on--;
    //     capability_sent = true;
    // }

    RankingFlow* f = this->active_receiving_flow;
    // probably can do better here
    if(f->finished_at_receiver)
    {
        return;
    }

    //not yet timed out, shouldn't send
    if(f->redundancy_ctrl_timeout > get_current_time()){
        if(f->redundancy_ctrl_timeout < closet_timeout)
        {
            closet_timeout = f->redundancy_ctrl_timeout;
        }
    }
    //ok to send
    else
    {
        //just timeout, reset timeout state
        if(f->redundancy_ctrl_timeout > 0)
        {
            f->redundancy_ctrl_timeout = -1;
            f->token_goal += f->remaining_pkts();
        }

        if(f->token_gap() > params.token_window)
        {
            if(get_current_time() >= f->latest_token_sent_time + params.token_window_timeout * params.get_full_pkt_tran_delay())
                f->relax_token_gap();
            else{
                if(f->latest_token_sent_time + params.token_window_timeout * params.get_full_pkt_tran_delay() < closet_timeout)
                {
                    closet_timeout = f->latest_token_sent_time + params.token_window_timeout * params.get_full_pkt_tran_delay();
                }
            }

        }


        if(f->token_gap() <= params.token_window)
        {
            f->send_token_pkt();
            token_sent = true;
            // this->token_hist.push_back(this->recv_flow->id);
            if(f->token_count == f->token_goal){
                f->redundancy_ctrl_timeout = get_current_time() + params.token_resend_timeout * params.get_full_pkt_tran_delay();
            }
        }
    }

    if(token_sent)// pkt sent
    {
        this->schedule_token_proc_evt(params.get_full_pkt_tran_delay(1500/* + 40*/), false);
    }
    else if(closet_timeout < 999999) //has unsend flow, but its within timeout
    {
        assert(closet_timeout > get_current_time());
        this->schedule_token_proc_evt(closet_timeout - get_current_time(), true);
    }
    else{
        //do nothing, no unfinished flow
    }


}

void RankingHost::receive_gosrc(RankingGoSrc* pkt) {
    assert(this->active_receiving_flow == NULL);
    this->active_receiving_flow = dynamic_cast<RankingFlow*>(pkt->flow);
    if(this->wakeup_evt != NULL) {
        this->wakeup_evt->cancelled = true;
    }
    // schedule sending token event;
    if(this->token_send_evt == NULL){
        this->schedule_token_proc_evt(0, false);
    }
}

// ---- Ranking Arbiter

RankingArbiter::RankingArbiter(uint32_t id, double rate, uint32_t queue_type) : Host(id, rate, queue_type, RANKING_ARBITER) {
    this->src_state = std::vector<bool>(params.num_hosts, true);
    // fake flow for receiving listRTS;
    this->fake_flow = new RankingFlow(-1, -1, -1, NULL, this);
}

void RankingArbiter::start_arbiter() {
    this->schedule_proc_evt(1.0);
}


void RankingArbiter::schedule_proc_evt(double time) {
    if (this->arbiter_proc_evt != NULL) {
        this->arbiter_proc_evt->cancelled = true;
        this->arbiter_proc_evt = NULL;
    }
    this->arbiter_proc_evt = new RankingArbiterProcessingEvent(time, this);
    add_to_event_queue(this->arbiter_proc_evt);
}

void RankingArbiter::schedule_epoch() {
    if (total_finished_flows >= params.num_flows_to_run)
        return;
    while(!this->pending_q.empty()) {
        auto listRTS = this->pending_q.top();
        this->pending_q.pop();
        for(auto i = listRTS->listFlows.begin(); i != listRTS->listFlows.end(); i++) {
            if(this->src_state[(*i)->src->id]) {
                this->src_state[(*i)->src->id] = false;
                // send GoSRC packet
                dynamic_cast<RankingFlow*>(*i)->sending_gosrc();
                break;
            }
        }
        delete listRTS;
    }
    //schedule next arbiter proc evt
    this->schedule_proc_evt(get_current_time() + params.ranking_epoch_time);
}
void RankingArbiter::receive_listrts(RankingListRTS* pkt) {
    auto listRTS = new ListRTS();
    listRTS->dst = pkt->rts_dst;
    listRTS->listFlows = pkt->listFlows;
    this->pending_q.push(listRTS);
    // TODO: schedule event 
}

void RankingArbiter::receive_nrts(RankingNRTS* pkt) {
    assert(this->src_state[pkt->flow->src->id] == false);
    this->src_state[pkt->flow->src->id] = true;
}

// void RankingArbiter::receive_rts(RankingRTS* rts)
// {
//     if(!((RankingFlow*)rts->flow)->arbiter_received_rts)
//     {
//         ((RankingFlow*) rts->flow)->arbiter_received_rts = true;
//         dynamic_cast<RankingTopology*>(topology)->arbiter->sending_flows.push((RankingFlow*)rts->flow);
//     }

//     if(rts->remaining_num_pkts < 0){
//         ((RankingFlow*)rts->flow)->arbiter_remaining_num_pkts = 0;
//         ((RankingFlow*)rts->flow)->arbiter_finished = true;
//     }
//     else
//         ((RankingFlow*)rts->flow)->arbiter_remaining_num_pkts = rts->remaining_num_pkts;
// }
