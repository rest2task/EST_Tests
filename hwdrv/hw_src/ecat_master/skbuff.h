/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    netdevice.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2013.09.22

Description:    SKB structures from Linux	 

Others:


History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#ifndef _LINUX_SKBUFF_H_
#define _LINUX_SKBUFF_H_
#include "types.h"
#if 0
#define kmemcheck_bitfield_begin(name)
#define kmemcheck_bitfield_end(name)
#endif
typedef unsigned char *sk_buff_data_t;

/* Don't change this without changing skb_csum_unnecessary! */
#define CHECKSUM_NONE 0
#define CHECKSUM_UNNECESSARY 1
#define CHECKSUM_COMPLETE 2
#define CHECKSUM_PARTIAL 3

#ifndef NET_SKB_PAD
#define NET_SKB_PAD	64
#endif
#define __aligned(x)			__attribute__((aligned (x)))

struct sk_buff {
	/* These two members must be first. */
	struct sk_buff		*next;
	struct sk_buff		*prev;
	struct net_device	*dev;

	unsigned int		len, data_len;
	u8			local_df:1,
				cloned:1,
				ip_summed:2,
				nohdr:1,
				nfctinfo:3;
	u8			pkt_type:3,
				fclone:2,
				ipvs_property:1,
				peeked:1,
				nf_trace:1;
	//be16			protocol; //gonggw, 2013.09.22, original code from Linux
	u16			protocol;
	
	u16			queue_mapping:16;
	u8			deliver_no_wcard:1;
	/* 0/14 bit hole */

	sk_buff_data_t		mac_header;
	/* These elements must be at the end, see alloc_skb() for details.  */
	sk_buff_data_t		tail;
	sk_buff_data_t		end;
	unsigned char		*head, *data;
};

enum {
	SKB_GSO_TCPV4 = 1 << 0,
	SKB_GSO_UDP = 1 << 1,

	/* This indicates the skb is from an untrusted source. */
	SKB_GSO_DODGY = 1 << 2,

	/* This indicates the tcp segment has CWR set. */
	SKB_GSO_TCP_ECN = 1 << 3,

	SKB_GSO_TCPV6 = 1 << 4,

	SKB_GSO_FCOE = 1 << 5,
};
static inline void skb_reserve(struct sk_buff *skb, int len)
{
	skb->data += len;
	skb->tail += len;
}
static inline unsigned char *__skb_pull(struct sk_buff *skb, unsigned int len)
{
	skb->len -= len;
	return skb->data += len;
}

static inline unsigned char *skb_pull_inline(struct sk_buff *skb, unsigned int len)
{
	return (len > skb->len) ? NULL : __skb_pull(skb, len);
}

static inline void skb_reset_mac_header(struct sk_buff *skb)
{
	skb->mac_header = skb->data;
}
static inline unsigned char *skb_mac_header(const struct sk_buff *skb)
{
	return skb->mac_header;
}
static inline struct ethhdr *eth_hdr(const struct sk_buff *skb)
{
	return (struct ethhdr *)skb_mac_header(skb);
}
#if 0
static inline struct sk_buff *alloc_skb(unsigned int size)
{
	return (struct sk_buff *)__alloc_skb(size);
}

static inline struct sk_buff *__dev_alloc_skb(unsigned int length)
{
	struct sk_buff *skb = alloc_skb(length + NET_SKB_PAD);
	if (skb)
		skb_reserve(skb, NET_SKB_PAD);
	return skb;
}
#endif

#endif

