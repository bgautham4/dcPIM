#include "tracing.h"
#include <stdio.h>

CtrlPacketCounter ctrl_pkt_cntr = {{0}};

void incr_ctrl_packet_count(CtrlPacketCounter *cntr, ctrlpacket_t type) {
	if (type >= NUM_CTRL_PACKET_TYPES) {
		return;
	}	
	cntr->packet_counts[type] += 1;
}

// Dump ctrl packet counts onto stdout
void dump_ctrl_packet_counts(const CtrlPacketCounter *cntr) {
	const char *packet_names[NUM_CTRL_PACKET_TYPES] = {
		"SYNC", "SYNC_ACK", "PERMIT", "RTS", "GRANT", "ACCEPT", "GRANTR"
	};

	printf("Control Packet Counts:\n");
	for (int i = 0; i < NUM_CTRL_PACKET_TYPES; i++) {
		printf("%s: %lu\n", packet_names[i], cntr->packet_counts[i]);
	}
}

MatchCounter match_cntr = {0ul,0ul};

void incr_match_count(MatchCounter *cntr, unsigned long epoch) {
	if (epoch > cntr->last_epoch) {
		cntr->last_epoch = epoch;
	}
	cntr->num_matches += 1;
}

void dump_match_count(const MatchCounter *cntr) {
	printf("TOT_EPOCHS_SEEN: %lu\nTOT_MATCHES: %lu\n", cntr->last_epoch, cntr->num_matches);
}
