/*************************************************

Copyright (C), 2014-2015, EST Tech. Co., Ltd.

File name:    ec_init.c

Author:        Gong Guowang(gongguowang@esto.com)

Version:        1.0

Date:           2015.01.20

Description:     System initialization after Power On

Others:

Function List:  
                    ec_dev_init     : EtherCAT device init
                    ec_init            : system hardware init
                    
History:    

    1. Date:

     Author:

     Modification:

    2. ...

*************************************************/

#include "ecat_hal.h"


#include "types.h"
#include "debug.h"
#include "config.h"
#include "ec_mm.h"
#include "irqflags.h"
#include "netdevice.h"
#include "ecdev.h"
#include "ec_globals.h"
#include "base_data_op.h"
#include "master.h"
#include "ecrt.h"
#include "app_data.h"

#include "eth_op.h"




//defined in "driver/dm9000.c"
extern struct board_info *db;
extern struct net_device *ndev;

//defined in "master/master.c"
extern ec_master_t *masters;
extern unsigned int master_count;


int ec_dev_init()
{
    int ret;
    db->ecdev = ecdev_offer(ndev, ec_poll);
    if (!db->ecdev)
    {
        ret = -1;
        goto out;
    }
    if (db->ecdev && ecdev_open(db->ecdev)) {
        ecdev_withdraw(db->ecdev);
        ret = -2;
        goto out;
    }
    return 0;
out:
	dev_err(db->dev, "ECAT Device initialization failure (%d).\n", ret);        
    free_netdev(ndev);
    return ret;
}


