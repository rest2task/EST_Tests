/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    eth_op.h

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2015.01.14

Description:    function prototype declaration in eth_op.c

Others:


History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/
#ifndef _ETH_OP_H_
#define _ETH_OP_H_
//#ifndef _EC_MM_H_
//#define _EC_MM_H_

#include "skbuff.h"


struct sk_buff *dev_alloc_skb(unsigned int length);
unsigned char *skb_push(struct sk_buff *skb, unsigned int len);
void dev_kfree_skb(struct sk_buff *skb);
struct net_device *alloc_netdev_mq(int sizeof_priv, const char *name,
		void (*setup)(struct net_device *), unsigned int queue_count);
void free_netdev(struct net_device *dev);
__u16 eth_type_trans(struct sk_buff *skb, struct net_device *dev);
int netif_rx(struct sk_buff *skb);
unsigned char *skb_put(struct sk_buff *skb, unsigned int len);
void netif_carrier_off(struct net_device *dev);

void ether_setup(struct net_device *dev);
unsigned int mii_check_media (struct mii_if_info *mii,unsigned int ok_to_print,unsigned int init_media);
//struct net_device *alloc_etherdev_mq(int sizeof_priv, unsigned int queue_count);


#endif
