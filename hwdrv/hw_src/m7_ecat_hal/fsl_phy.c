/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_phy.h"
#include "machapi.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the timeout macro. */
#define PHY_TIMEOUT_COUNT 0x3FFFFFFU
#define READ_PHY_TIMEOUT_COUNT 0x3FFFFFFU//0x100U
#define WRITE_TIMEOUT_COUNT 0x3FFFFFFU


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the ENET instance from peripheral base address.
 *
 * @param base ENET peripheral base address.
 * @return ENET instance.
 */
extern uint32_t ENET_GetInstance(ENET_Type *base);
void FeedDog(void);


/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to enet clocks for each instance. */
extern clock_ip_name_t s_enetClock[FSL_FEATURE_SOC_ENET_COUNT];
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
#include <stdio.h>		
#include <string.h>	
#include <stdlib.h>
void delay_ms(uint16_t CN);
status_t PHY_Init(ENET_Type *base, uint32_t phyAddr, uint32_t srcClock_Hz)
{
    uint32_t bssReg;
    uint32_t counter = PHY_TIMEOUT_COUNT;
    uint32_t idReg = 1;
    status_t result = kStatus_Success;
    uint32_t instance = ENET_GetInstance(base);
    uint32_t timeDelay;
    uint32_t ctlReg = 0;
	uint32_t phycontrlid1;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Set SMI first. */
    CLOCK_EnableClock(s_enetClock[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
    ENET_SetSMI(base, srcClock_Hz, false);

    /* Initialization after PHY stars to work. */
	if(phyAddr == 1)							//dp83848
		phycontrlid1 = 0x2000;
	else
		phycontrlid1 = 0;						//yt8512
    while ((idReg != phycontrlid1) && (counter != 0))
    {
        PHY_Read(base, phyAddr, PHY_ID1_REG, &idReg);
        counter --;       
    }
    PHY_Read(base, phyAddr, PHY_ID2_REG, &idReg);
    printf("-PHY_ID2_REG=%x--\n\r",idReg);
    PHY_Read(base, phyAddr, PHY_ID1_REG, &idReg);
    printf("-PHY_ID1_REG--=%x--\n\r",idReg);
    PHY_Read(base, phyAddr, PHY_BASICCONTROL_REG, &idReg);
    printf("-PHY_BASICCONTROL_REG--=%x--\n\r",idReg);
	
    if (!counter)
    {
        return kStatus_Fail;
    }

    /* Reset PHY. */
    if(ChkSysCfg(CO3_EJTNEW))
        counter = 100000;//PHY_TIMEOUT_COUNT;
    else
        counter = 35000;
    result = PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG, PHY_BCTL_RESET_MASK);
    if (result == kStatus_Success)
    { 
        /* Set the negotiation. */
        result = PHY_Write(base, phyAddr, PHY_AUTONEG_ADVERTISE_REG,
                           (PHY_100BASETX_FULLDUPLEX_MASK | PHY_100BASETX_HALFDUPLEX_MASK |
                            PHY_10BASETX_FULLDUPLEX_MASK | PHY_10BASETX_HALFDUPLEX_MASK | 0x1U));
        if (result == kStatus_Success)
        {
            result = PHY_Write(base, phyAddr, PHY_BASICCONTROL_REG,
                               (PHY_BCTL_AUTONEG_MASK | PHY_BCTL_RESTART_AUTONEG_MASK));
            if (result == kStatus_Success)
            {
                /* Check auto negotiation complete. */
                while (counter --)
                {
                	FeedDog();
                    result = PHY_Read(base, phyAddr, PHY_BASICSTATUS_REG, &bssReg);
                    if ( result == kStatus_Success)
                    {
						if (phyAddr == 1)			//DP83848
						{
							PHY_Read(base, phyAddr, PHY_STATUS_REG, &ctlReg);
							if (((bssReg & PHY_BSTATUS_AUTONEGCOMP_MASK) != 0) && (ctlReg & PHY_STATUS_LINK_STATUS_MASK))
							{
								/* Wait a moment for Phy status stable. */
								for (timeDelay = 0; timeDelay < PHY_TIMEOUT_COUNT; timeDelay ++)
								{
									__ASM("nop");
								}
								break;
							}
						}
						else						//YT8512
						{
								if (((bssReg & PHY_BSTATUS_AUTONEGCOMP_MASK) != 0) && (bssReg & 0x4))

							{
								/* Wait a moment for Phy status stable. */
								for (timeDelay = 0; timeDelay < PHY_TIMEOUT_COUNT; timeDelay ++)
								{
									__ASM("nop");
								}
								break;
							}
						}
                    }

                    if (!counter)
                    {
                        return kStatus_PHY_AutoNegotiateFail;
                    }
                }
            }
        }
    }

    return result;
}

status_t PHY_Write(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t data)
{
    uint32_t counter;

    /* Clear the SMI interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI write command. */
    ENET_StartSMIWrite(base, phyAddr, phyReg, kENET_MiiWriteValidFrame, data);

    /* Wait for SMI complete. */
    for (counter = PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}

status_t PHY_Read(ENET_Type *base, uint32_t phyAddr, uint32_t phyReg, uint32_t *dataPtr)
{
    assert(dataPtr);

    uint32_t counter;

    /* Clear the MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    /* Starts a SMI read command operation. */
    ENET_StartSMIRead(base, phyAddr, phyReg, kENET_MiiReadValidFrame);

    /* Wait for MII complete. */
    for (counter = READ_PHY_TIMEOUT_COUNT; counter > 0; counter--)
    {
        if (ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK)
        {
            break;
        }
    }

    /* Check for timeout. */
    if (!counter)
    {
        return kStatus_PHY_SMIVisitTimeout;
    }

    /* Get data from MII register. */
    *dataPtr = ENET_ReadSMIData(base);

    /* Clear MII interrupt event. */
    ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);

    return kStatus_Success;
}


status_t PHY_GetLinkStatus(ENET_Type *base, uint32_t phyAddr, bool *status)
{
    assert(status);

    status_t result = kStatus_Success;
    uint32_t data;

    /* Read the basic status register. */
    result = PHY_Read(base, phyAddr, PHY_BASICSTATUS_REG, &data);
    if (result == kStatus_Success)
    {
        if (!(PHY_BSTATUS_LINKSTATUS_MASK & data))
        {
            /* link down. */
            *status = false;
        }
        else
        {
            /* link up. */
            *status = true;
        }
    }
    return result;
}

status_t PHY_GetLinkSpeedDuplex(ENET_Type *base, uint32_t phyAddr, phy_speed_t *speed, phy_duplex_t *duplex)
{
    assert(duplex);

    status_t result = kStatus_Success;
    uint32_t data;
   // uint32_t ctlReg;

    /* Read the control two register. */
    //result = PHY_Read(base, phyAddr, PHY_STATUS_REG, &ctlReg); 
	if (phyAddr == 1)					//dp83848
	{
		result = PHY_Read(base, phyAddr, 0x10, &data); 
	}
	else								//yt8512
	{
		result = PHY_Read(base, phyAddr, 0x11, &data); 
	}
    if (result == kStatus_Success)
    {
		if (phyAddr == 1)	
		{
			if ((data & PHY_STATUS_DUPLEX_STATUS_MASK) == PHY_STATUS_DUPLEX_IS_FULL)
			{
				/* Full duplex. */
				*duplex = kPHY_FullDuplex;
			}
			else
			{
				/* Half duplex. */
				*duplex = kPHY_HalfDuplex;
			}

			if ((data & PHY_STATUS_SPEED_STATUS_MASK) == PHY_STATUS_SPEED_IS_100M)
			{
				/* 100M speed. */
				*speed = kPHY_Speed100M;
			}
			else
			{ /* 10M speed. */
				*speed = kPHY_Speed10M;
			}
		}
		else
		{
			if ((data & 0x2000) == 0x2000)
			{
				/* Full duplex. */
				*duplex = kPHY_FullDuplex;
			}
			else
			{
				/* Half duplex. */
				*duplex = kPHY_HalfDuplex;
			}

			if ((data & 0xc000) == 0x4000)
			{
				/* 100M speed. */
				*speed = kPHY_Speed100M;
			}
			else
			{ /* 10M speed. */
				*speed = kPHY_Speed10M;
			}
		}			
    }

    return result;
}
