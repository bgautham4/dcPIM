#ifndef PARAMS_H
#define PARAMS_H

#include <string>
#include <fstream>
#include <cstdint>

class DCExpParams {
    public:
        std::string param_str;
        std::string util_file;

        std::string topology;
        // for fat tree topology;
        int k;

        uint32_t initial_cwnd;
        uint32_t max_cwnd;
        double retx_timeout_value;
        uint32_t mss;
        uint32_t hdr_size;
        int queue_size;
        uint32_t queue_type;
        uint32_t flow_type;
        uint32_t load_balancing; //0 per pkt, 1 per flow

        double propagation_delay;
        double bandwidth;
        // oversubscription ratio
        double os_ratio;
        
        uint32_t num_flows_to_run;
        double end_time;
        std::string cdf_or_flow_trace;
        std::string incast_trace;
        uint32_t bytes_mode;
        uint32_t cut_through;
        uint32_t mean_flow_size;


        uint32_t num_hosts;
        uint32_t num_agg_switches;
        uint32_t num_core_switches;
        uint32_t preemptive_queue;
        uint32_t big_switch;
        uint32_t host_type;
        double traffic_imbalance;
        double load;
        double local_load;
        double remote_load;
        double reauth_limit;

        double magic_trans_slack;
        uint32_t magic_delay_scheduling;
        uint32_t magic_inflate;

        uint32_t use_flow_trace;
        uint32_t smooth_cdf;
        uint32_t burst_at_beginning;
        double capability_timeout;
        double capability_resend_timeout;
        uint32_t capability_initial;
        uint32_t capability_window;
        uint32_t capability_prio_thresh;
        double capability_window_timeout;
        uint32_t capability_third_level;
        uint32_t capability_fourth_level;

        uint32_t ddc;
        double ddc_cpu_ratio;
        double ddc_mem_ratio;
        double ddc_disk_ratio;
        uint32_t ddc_normalize; //0: sender send, 1: receiver side, 2: both
        uint32_t ddc_type;

        uint32_t deadline;
        uint32_t schedule_by_deadline;
        double avg_deadline;
        std::string interarrival_cdf;
        uint32_t num_host_types;

        double fastpass_epoch_time;

        uint32_t permutation_tm;

        uint32_t incast_tm;

        uint32_t outcast_tm;

        uint32_t worstcase_tm;

        uint32_t dctcp_mark_thresh;
        //uint32_t dctcp_delayed_ack_freq;
        uint32_t fastpass_epoch_pkts;
        bool fastpass_localize;
        bool fastpass_limit_conns;
        // Ruf Algorithm
        double BDP;
        double token_timeout;
        double token_resend_timeout;
        uint32_t token_initial;
        uint32_t token_window;
        // uint32_t token_prio_thresh;
        double token_window_timeout;
        uint32_t token_third_level;
        uint32_t token_fourth_level;
        
        //double ruf_epoch_time;
        double rufhost_idle_timeout;
        double ruf_controller_epoch;
        // double ruf_reset_epoch;
        double ruf_max_tokens;
        double ruf_min_tokens;
        double rtt;
        // double ctrl_pkt_rtt;
        double ruf_max_src_num;
        bool debug_controller_queue;
        bool debug_queue;
        double debug_queue_interval;
        bool ruf_localize;
        bool ruf_limit_conns;
        // std::string policy;
        // debug for max-min fairness
        bool print_max_min_fairness;

        // Multi Round Distributed protoocl
        uint32_t pim_iter_limit;
        uint32_t pim_select_min_iters;
        uint32_t pim_k;
        double pim_alpha;
        double pim_beta;
        double pim_iter_epoch;
        double pim_epoch;
        int pim_window_size;
        int pim_small_flow;
        double pim_window_timeout;
        double pim_resend_timeout;
        int pim_low_priority;
        int pim_link_pkts;

        bool pim_modified; // G : Run modified pim?
        double alpha; // G : Value of alpha for modified pim
        double get_full_pkt_tran_delay(uint32_t size_in_byte = 1500)
        {
            return size_in_byte * 8 / this->bandwidth;
        }
        int packet_priority(int size_in_pkt, int base);

};


#define CAPABILITY_MEASURE_WASTE false
#define CAPABILITY_NOTIFY_BLOCKING false
#define CAPABILITY_HOLD true

//#define FASTPASS_EPOCH_TIME 0.000010
// #define FASTPASS_EPOCH_PKTS 8
// #define RANKING_EPOCH_PKTS 3

#define TOKEN_HOLD true

void read_experiment_parameters(std::string conf_filename, uint32_t exp_type); 

/* General main function */
#define DEFAULT_EXP 1
#define GEN_ONLY 2

#define INFINITESIMAL_TIME 0.000000000001

#endif
