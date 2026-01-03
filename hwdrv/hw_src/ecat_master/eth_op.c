/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    eth_op.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2013.09.22

Description:    Ethernet Operation functions from Linux	

Others:

Function List:  
                    ether_setup                          :  Ethernet parameters initialization
                    netdev_init_one_queue         :add one devcie to queue
                    netdev_init_queues              : init net device queue
                    __hw_addr_init                    : hardware address list init
                    dev_mc_init                         : MultiCast init
                    dev_uc_init                          : UniCast init
                    __hw_addr_add_ex              : Hardware address added
                    __hw_addr_add                   : Hardware address added
                    dev_addr_init                      : device address init
                    alloc_netdev_mq                 : net device allocation
                    alloc_etherdev_mq              : Ethernet device allocation
                    ethtool_op_set_tx_csum      :
                    ethtool_op_set_tx_hw_csum:
                    ethtool_op_get_rx_csum      :
                    ethtool_op_get_tx_csum      :
                    mii_link_ok                         : MII Link status check
                    netif_carrier_off                  : carrier Off
                    netif_carrier_on                  : carrier On
                    mii_check_media                : mii status check
                    netif_device_attach            : Net Interface and device attach
                    netif_device_dettach          : Net Interface and device netif_device_dettach
                    skb_push                           : skip head space in SKB
                    eth_type_trans                  : Ethernet type
                    skb_put                             : reserve head space in SKB
                    __alloc_skb                       : allocate one SKB memory
                    dev_kfree_skb                   : free SKB allocated wth dev_alloc_skb
                    dev_alloc_skb                    : allocate one SKB
                    free_netdev                       : Free net device memory
                    
History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/
#include <string.h>
#include "types.h"
#include "config.h"
#include "debug.h"
#include "ec_mm.h"

#include "base_data_op.h"
#include "netdevice.h"
#include "skbuff.h"
#include "list.h"
#include "errno.h"


void ether_setup(struct net_device *dev)
{
	dev->type				= ARPHRD_ETHER;
	dev->hard_header_len	= ETH_HLEN;
	dev->mtu				= ETH_DATA_LEN;
	dev->addr_len			= ETH_ALEN;
	dev->tx_queue_len		= 1000;	/* Ethernet wants good queues */
	dev->flags				= IFF_BROADCAST|IFF_MULTICAST;
#ifdef CONFIG_USE_LIB_MEMCPY
	memset(dev->broadcast, 0xFF, ETH_ALEN);
#else
	ec_memset(dev->broadcast, 0xFF, ETH_ALEN);
#endif
}

/*static void netdev_init_queues(struct net_device *dev)
{
	netdev_init_one_queue(dev, &dev->rx_queue, NULL);
	netdev_for_each_tx_queue(dev, netdev_init_one_queue, NULL);
}*/

void __hw_addr_init(struct netdev_hw_addr_list *list)
{
	INIT_LIST_HEAD(&list->list);
	list->count = 0;
}

void dev_mc_init(struct net_device *dev)
{
	__hw_addr_init(&dev->mc);
}
void dev_uc_init(struct net_device *dev)
{
	__hw_addr_init(&dev->uc);
}

/*
 * General list handling functions
 */
#if 0
static int __hw_addr_add_ex(struct netdev_hw_addr_list *list,
			    unsigned char *addr, int addr_len,
			    unsigned char addr_type, bool global)
{
	struct netdev_hw_addr *ha;
	int alloc_size;

	if (addr_len > MAX_ADDR_LEN)
		return -EINVAL;

	list_for_each_entry(ha, netdev_hw_addr,&list->list, list) {
		if (!memcmp(ha->addr, addr, addr_len) &&
		    ha->type == addr_type) {
			return 0;
		}
	}


	alloc_size = sizeof(*ha);
	if (alloc_size < L1_CACHE_BYTES)
		alloc_size = L1_CACHE_BYTES;
#ifdef EC_DYN_MEM_PRT
	_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, alloc_size);
#endif
	ha = ec_malloc(alloc_size, 0);
	if (!ha)
		return -ENOMEM;
#ifdef CONFIG_USE_LIB_MEMCPY
	memcpy(ha->addr, addr, addr_len);
#else
	ec_memcpy(ha->addr, addr, addr_len);
#endif
	ha->type = addr_type;
	list->count++;
	return 0;
}
#endif
#if 0
static int __hw_addr_add(struct netdev_hw_addr_list *list, unsigned char *addr,
			 int addr_len, unsigned char addr_type)
{
	return __hw_addr_add_ex(list, addr, addr_len, addr_type, false);
}
#endif
/**
 *	dev_addr_init - Init device address list
 *	@dev: device
 *
 *	Init device address list and create the first element,
 *	used by ->dev_addr.
 *
 *	The caller must hold the rtnl_mutex.
 */
 #if 0
int dev_addr_init(struct net_device *dev)
{
	unsigned char addr[MAX_ADDR_LEN];
	struct netdev_hw_addr *ha;
	int err;

	/* rtnl_mutex must be held here */

	__hw_addr_init(&dev->dev_addrs);
#ifdef CONFIG_USE_LIB_MEMCPY
	memset(addr, 0, sizeof(addr));
#else
	ec_memset(addr, 0, sizeof(addr));
#endif
	err = __hw_addr_add(&dev->dev_addrs, addr, sizeof(addr),
			    NETDEV_HW_ADDR_T_LAN);
	if (!err) {
		/*
		 * Get the first (previously created) address from the list
		 * and set dev_addr pointer to this location.
		 */
		ha = list_first_entry(&dev->dev_addrs.list,
				      struct netdev_hw_addr, list);
		dev->dev_addr = ha->addr;
	}
	return err;
}
#endif
#ifndef DYN_MEM_ALLOC
#define NETDEV_BUF_LEN	448
uint8_t netdev_buf[NETDEV_BUF_LEN] __attribute((aligned(32)));
uint8_t *netdev_ptr;
uint8_t net_tx_queue[48] __attribute((aligned(4)));
#endif
struct net_device *alloc_netdev_mq(int sizeof_priv, const char *name,
		void (*setup)(struct net_device *), unsigned int queue_count)
{
	struct netdev_queue *tx;
	struct net_device *dev;
	struct net_device *p;

#ifdef DYN_MEM_ALLOC
	alloc_size = sizeof(struct net_device);
	if (sizeof_priv) {
		/* ensure 32-byte alignment of private area */
		alloc_size = ALIGN(alloc_size, NETDEV_ALIGN);
		alloc_size += sizeof_priv;
	}
	/* ensure 32-byte alignment of whole construct */
	alloc_size += NETDEV_ALIGN - 1;
#ifdef EC_DYN_MEM_PRT
	_DBG_PRINTF_("%d in %s, ec_zalloc size=%d\n", __LINE__, __FILE__, alloc_size);
#endif
	p = (struct net_device *)ec_zalloc(alloc_size, 0);
	if (!p) {
		_DBG_PRINTF_("alloc_netdev: Unable to allocate device.\n");
		return NULL;
	}
#else
	netdev_ptr = netdev_buf;
	memset(netdev_ptr, (uint8_t)0, NETDEV_BUF_LEN);
	p = (struct net_device *)netdev_ptr;
#endif
	//dev = PTR_ALIGN(p, NETDEV_ALIGN);	
	dev = (struct net_device *)PTR_ALIGN(p, NETDEV_ALIGN);
	
	dev->padded = (char *)dev - (char *)p;
#if 0
	if (dev_addr_init(dev))
		goto free_rx;
#endif
	dev_mc_init(dev);
	dev_uc_init(dev);
	dev->num_tx_queues = queue_count;
	dev->real_num_tx_queues = queue_count;
#ifdef DYN_MEM_ALLOC
    if (dev->num_tx_queues)
    {
#ifdef EC_DYN_MEM_PRT
		_DBG_PRINTF_("%d in %s, ec_calloc size=%d\n", __LINE__, __FILE__, dev->num_tx_queues * (sizeof(struct netdev_queue)));
#endif
        tx = (struct netdev_queue *)ec_calloc(dev->num_tx_queues, sizeof(struct netdev_queue), 0);
    }
    else
        tx = (struct netdev_queue *)NULL;
#else
		if (dev->num_tx_queues)
		{
			memset(net_tx_queue, (uint8_t)0, 48);
			tx = (struct netdev_queue *)net_tx_queue;
		}
#endif
	dev->_tx = tx;

	setup(dev);
	strcpy(dev->name, name);
	return dev;

#ifdef DYN_MEM_ALLOC
	ec_free(tx);
#endif
		
#ifdef DYN_MEM_ALLOC
	ec_free(p);
#endif
//	return NULL;
}
struct net_device *alloc_etherdev_mq(int sizeof_priv, unsigned int queue_count)
{
	return alloc_netdev_mq(sizeof_priv, "ec_eth", ether_setup, queue_count);
}

int ethtool_op_set_tx_csum(struct net_device *dev, u32 data)
{
	if (data)
		dev->features |= NETIF_F_IP_CSUM;
	else
		dev->features &= ~NETIF_F_IP_CSUM;

	return 0;
}
int ethtool_op_set_tx_hw_csum(struct net_device *dev, u32 data)
{
	if (data)
		dev->features |= NETIF_F_HW_CSUM;
	else
		dev->features &= ~NETIF_F_HW_CSUM;

	return 0;
}

u32 ethtool_op_get_rx_csum(struct net_device *dev)
{
	return (dev->features & NETIF_F_ALL_CSUM) != 0;
}


u32 ethtool_op_get_tx_csum(struct net_device *dev)
{
	return (dev->features & NETIF_F_ALL_CSUM) != 0;
}

int mii_link_ok (struct mii_if_info *mii)
{
	/* first, a dummy read, needed to latch some MII phys */
	mii->mdio_read(mii->dev, mii->phy_id, MII_BMSR);
	if (mii->mdio_read(mii->dev, mii->phy_id, MII_BMSR) & BMSR_LSTATUS)
		return 1;
	return 0;
}
void netif_carrier_off(struct net_device *dev)
{
	if (!test_and_set_bit(__LINK_STATE_NOCARRIER, &dev->state)) {
		if (dev->reg_state == NETREG_UNINITIALIZED)
			return;
	}
}

void netif_carrier_on(struct net_device *dev)
{
	if (test_and_clear_bit(__LINK_STATE_NOCARRIER, &dev->state)) {
		if (dev->reg_state == NETREG_UNINITIALIZED)
			return;
	}
}

/**
 * mii_check_media - check the MII interface for a duplex change
 * @mii: the MII interface
 * @ok_to_print: OK to print link up/down messages
 * @init_media: OK to save duplex mode in @mii
 *
 * Returns 1 if the duplex mode changed, 0 if not.
 * If the media type is forced, always returns 0.
 */
unsigned int mii_check_media (struct mii_if_info *mii,
			      unsigned int ok_to_print,
			      unsigned int init_media)
{
	unsigned int old_carrier, new_carrier;
	int advertise, lpa, media, duplex;
	u32	jiffies_start = jiffies;
	/* if forced media, go no further */
	if (mii->force_media)
		return 0; /* duplex did not change */

	/* check current and old link status */
	//do
	//{
		old_carrier = netif_carrier_ok(mii->dev) ? 1 : 0;
		new_carrier = (unsigned int) mii_link_ok(mii);
	//}while(!new_carrier || ((jiffies - jiffies_start) < 3000000));
	/* if carrier state did not change, this is a "bounce",
	 * just exit as everything is already set correctly
	 */
	if ((!init_media) && (old_carrier == new_carrier))
		return 0; /* duplex did not change */

	/* no carrier, nothing much to do */
	if (!new_carrier) {
		netif_carrier_off(mii->dev);
		if (ok_to_print)
			_DBG_PRINTF_("%s: link down\n", mii->dev->name);
		return 0; /* duplex did not change */
	}

	/*
	 * we have carrier, see who's on the other end
	 */
	netif_carrier_on(mii->dev);

	/* get MII advertise and LPA values */
	if ((!init_media) && (mii->advertising))
		advertise = mii->advertising;
	else {
		advertise = mii->mdio_read(mii->dev, mii->phy_id, MII_ADVERTISE);
		mii->advertising = advertise;
	}
	lpa = mii->mdio_read(mii->dev, mii->phy_id, MII_LPA);
//	if (mii->supports_gmii)
//		lpa2 = mii->mdio_read(mii->dev, mii->phy_id, MII_STAT1000);

	/* figure out media and duplex from advertise and LPA values */
	media = mii_nway_result(lpa & advertise);
	duplex = (media & ADVERTISE_FULL) ? 1 : 0;
//	if (lpa2 & LPA_1000FULL)
//		duplex = 1;

	if (ok_to_print && ((old_carrier != new_carrier) || init_media))
		_DBG_PRINTF_("%s: link up, %sMbps, %s-duplex, lpa 0x%04X\n",
		       mii->dev->name,
//		       lpa2 & (LPA_1000FULL | LPA_1000HALF) ? "1000" :
		       media & (ADVERTISE_100FULL | ADVERTISE_100HALF) ? "100" : "10",
		       duplex ? "full" : "half",
		       lpa);

	if ((init_media) || (mii->full_duplex != duplex)) {
		mii->full_duplex = duplex;
        //gonggw, 2014.04.09, for returning function immediately when normal work 
        mii->force_media = 1;
		return 1; /* duplex changed */
	}

	return 0; /* duplex did not change */
}

void netif_device_attach(struct net_device *dev)
{
	if (!test_and_set_bit(__LINK_STATE_PRESENT, &dev->state) &&
	    netif_running(dev)) {
	}
}


void netif_device_detach(struct net_device *dev)
{
	if (test_and_clear_bit(__LINK_STATE_PRESENT, &dev->state) &&
	    netif_running(dev)) {
	}
}

//SK buffer operation
unsigned char *skb_push(struct sk_buff *skb, unsigned int len)
{
	skb->data -= len;
	skb->len  += len;
	return skb->data;
}
#if 0
__be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev)
#else
__u16 eth_type_trans(struct sk_buff *skb, struct net_device *dev)
#endif
{
	struct ethhdr *eth;
	unsigned char *rawp;

	skb->dev = dev;
	skb_reset_mac_header(skb);
	skb_pull_inline(skb, ETH_HLEN);
	eth = eth_hdr(skb);

	if (is_multicast_ether_addr(eth->h_dest)) {
		if (!compare_ether_addr_64bits(eth->h_dest, dev->broadcast))
			skb->pkt_type = PACKET_BROADCAST;
		else
			skb->pkt_type = PACKET_MULTICAST;
	}

	/*
	 *      This ALLMULTI check should be redundant by 1.4
	 *      so don't forget to remove it.
	 *
	 *      Seems, you forgot to remove it. All silly devices
	 *      seems to set IFF_PROMISC.
	 */

	else if (1 /*dev->flags&IFF_PROMISC */ ) {
		if (compare_ether_addr_64bits(eth->h_dest, dev->dev_addr))
			skb->pkt_type = PACKET_OTHERHOST;
	}

	/*
	 * Some variants of DSA tagging don't have an ethertype field
	 * at all, so we check here whether one of those tagging
	 * variants has been configured on the receiving interface,
	 * and if so, set skb->protocol without looking at the packet.
	 */
	if (ntohs(eth->h_proto) >= 1536)
		return eth->h_proto;

	rawp = skb->data;

	/*
	 *      This is a magic hack to spot IPX packets. Older Novell breaks
	 *      the protocol design and runs IPX over 802.3 without an 802.2 LLC
	 *      layer. We look for FFFF which isn't a used 802.2 SSAP/DSAP. This
	 *      won't work for fault tolerant netware but does for the rest.
	 */
	if (*(unsigned short *)rawp == 0xFFFF)
		return htons(ETH_P_802_3);

	/*
	 *      Real 802.2 LLC
	 */
	return htons(ETH_P_802_2);
}

/**
 *	netif_rx	-	post buffer to the network code
 *	@skb: buffer to post
 *
 *	This function receives a packet from a device driver and queues it for
 *	the upper (protocol) levels to process.  It always succeeds. The buffer
 *	may be dropped during processing for congestion control or by the
 *	protocol layers.
 *
 *	return values:
 *	NET_RX_SUCCESS	(no congestion)
 *	NET_RX_DROP     (packet was dropped)
 *
 */

//the function of netif_rx in Linux is not used in EC_MASTER, so just create a blank function
int netif_rx(struct sk_buff *skb)
{
	return 0;
}

unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp = skb->tail;
	skb->tail += len;
	skb->len  += len;
	return tmp;
}
#if 0
struct sk_buff *__alloc_skb(unsigned int size)
{
	struct sk_buff *skb;
	u8 *data;


	/* Get the HEAD */
#ifdef EC_DYN_MEM_PRT
	_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, sizeof(struct sk_buff));
#endif
	skb = (struct sk_buff *)ec_malloc(sizeof(struct sk_buff), 0);
	if (!skb)
	{
		skb = NULL;
		return skb;
	}
#if 0
	prefetchw(skb);
#endif
	size = SKB_DATA_ALIGN(size);
#ifdef EC_DYN_MEM_PRT
	_DBG_PRINTF_("%d in %s, ec_malloc size=%d\n", __LINE__, __FILE__, size);
#endif
	data = (u8 *)ec_malloc(size, 0);
	if (!data)
	{
		ec_free(skb);
		skb = NULL;
		return skb;
	}
#if 0
	prefetchw(data + size);
#endif
	/*
	 * Only clear those fields we need to clear, not those that we will
	 * actually initialise below. Hence, don't put any more fields after
	 * the tail pointer in struct sk_buff!
	 */
#ifdef CONFIG_USE_LIB_MEMCPY
	memset(skb, 0, offsetof(struct sk_buff, tail));
#else
	ec_memset(skb, 0, offsetof(struct sk_buff, tail));
#endif
	skb->head = data;
	skb->data = data;
	skb->tail = data;
	skb->end = skb->tail + size;
	return skb;
}

#endif
void dev_kfree_skb(struct sk_buff *skb)
{

	if (!skb)
		return;
#ifdef DYN_MEM_ALLOC
	ec_free(skb->head);
	ec_free(skb);
#endif
}
#if 0
struct sk_buff *dev_alloc_skb(unsigned int length)
{
#if 0 //gonggw, 2013.10.12
	return __dev_alloc_skb(length);
#else
	struct sk_buff *skb = __alloc_skb(length + NET_SKB_PAD);
	if (skb)
		skb_reserve(skb, NET_SKB_PAD);
	return skb;
#endif
}
#endif
void free_netdev(struct net_device *dev)
{
#ifdef DYN_MEM_ALLOC
    if (dev->_tx)
        ec_free(dev->_tx);
	ec_free(dev);
#endif
}

