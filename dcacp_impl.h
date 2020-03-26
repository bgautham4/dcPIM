/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _UDP4_IMPL_H
#define _UDP4_IMPL_H
#include <net/udp.h>
#include <net/udplite.h>
#include <net/protocol.h>
#include <net/inet_common.h>

#include "net_dcacp.h"
#include "net_dcacplite.h"


struct dcacp_message_in* dcacp_message_in_init(struct dcacp_peer *peer, 
	struct dcacp_sock *sock, __u64 message_id, int message_size, int sport);
void dcacp_message_in_finish(struct dcacp_message_in *msg);
void dcacp_message_in_destroy(struct dcacp_message_in *msg);

struct sk_buff *dcacp_fill_packets(struct dcacp_peer *peer,
		struct msghdr *msg, size_t len);
struct dcacp_message_out* dcacp_message_out_init(struct dcacp_peer *peer, 
	struct dcacp_sock *sock, struct sk_buff* skb, __u64 message_id, int message_size, int dport);
void dcacp_message_out_destroy(struct dcacp_message_out *msg);

int dcacp_peertab_init(struct dcacp_peertab *peertab);
void dcacp_peertab_destroy(struct dcacp_peertab *peertab);
struct dcacp_peer *dcacp_peer_find(struct dcacp_peertab *peertab, __be32 addr,
	struct inet_sock *inet);

struct dcacp_message_in *dcacp_wait_for_message(struct dcacp_sock *dsk, unsigned flags, int *err);
int dcacp_message_in_copy_data(struct dcacp_message_in *msg,
		struct iov_iter *iter, int max_bytes);
void dcacp_msg_ready(struct dcacp_message_in *msg);
void dcacp_add_packet(struct dcacp_message_in *msg, struct sk_buff *skb);
int dcacp_handle_data_pkt(struct sk_buff *skb);
int dcacp_handle_flow_sync_pkt(struct sk_buff *skb);
int dcacp_handle_token_pkt(struct sk_buff *skb);
int dcacp_handle_ack_pkt(struct sk_buff *skb);

struct sk_buff* construct_flow_sync_pkt(struct dcacp_sock* d_sk, __u64 message_id, 
	int message_size, __u64 start_time);
struct sk_buff* construct_token_pkt(struct dcacp_sock* d_sk, bool free_token, unsigned short priority,
	 __u64 message_id, __u32 seq_no, __u32 data_seq_no, __u32 remaining_size);
struct sk_buff* construct_ack_pkt(struct dcacp_sock* d_sk, __u64 message_id);
int dcacp_xmit_control(struct sk_buff* skb, struct dcacp_peer *peer, struct dcacp_sock *dcacp_sk, int dport);
void dcacp_xmit_data(struct dcacp_message_out* msg, bool force);
void __dcacp_xmit_data(struct sk_buff *skb,  struct dcacp_peer* peer, struct dcacp_sock* sock, int dport);

int __dcacp4_lib_rcv(struct sk_buff *, struct udp_table *, int);
int __dcacp4_lib_err(struct sk_buff *, u32, struct udp_table *);

int dcacp_v4_get_port(struct sock *sk, unsigned short snum);
void dcacp_v4_rehash(struct sock *sk);

int dcacp_setsockopt(struct sock *sk, int level, int optname,
		   char __user *optval, unsigned int optlen);
int dcacp_getsockopt(struct sock *sk, int level, int optname,
		   char __user *optval, int __user *optlen);

#ifdef CONFIG_COMPAT
int compat_dcacp_setsockopt(struct sock *sk, int level, int optname,
			  char __user *optval, unsigned int optlen);
int compat_dcacp_getsockopt(struct sock *sk, int level, int optname,
			  char __user *optval, int __user *optlen);
#endif
int dcacp_recvmsg(struct sock *sk, struct msghdr *msg, size_t len, int noblock,
		int flags, int *addr_len);
int dcacp_sendpage(struct sock *sk, struct page *page, int offset, size_t size,
		 int flags);
void dcacp_destroy_sock(struct sock *sk);

#ifdef CONFIG_PROC_FS
int udp4_seq_show(struct seq_file *seq, void *v);
#endif
#endif	/* _UDP4_IMPL_H */
