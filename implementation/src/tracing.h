#ifndef TRACING_H
#define TRACING_H


#define NUM_CTRL_PACKET_TYPES 7

typedef enum ctrlpacket_t {
	SYNC=0,
	SYNCS_ACK,
	PERMIT,
	RTS,
	GRANT,
	ACCEPT,
	GRANTR
} ctrlpacket_t;

/*
For tracking number of control packets transmitted by each host
*/
typedef struct CtrlPacketCounter {
	unsigned long packet_counts[NUM_CTRL_PACKET_TYPES];		 
} CtrlPacketCounter;

extern CtrlPacketCounter ctrl_pkt_cntr;

void incr_ctrl_packet_count(CtrlPacketCounter *cntr, ctrlpacket_t type);
void dump_ctrl_packet_counts(const CtrlPacketCounter *cntr);

/*
For tracking the number of matches
*/
typedef struct MatchCounter {
	unsigned long last_epoch;
	unsigned long num_matches;
} MatchCounter;

extern MatchCounter match_cntr;

void incr_match_count(MatchCounter *cntr, unsigned long epoch);
void dump_match_count(const MatchCounter *cntr);
#endif
