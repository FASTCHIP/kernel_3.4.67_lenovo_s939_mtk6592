/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
   BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"
#include <cust_gpio_usage.h>

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif
/* Lenovo-sw yexm1 add, 20130819 begin */
#if defined(LENOVO_ID_READ)
#ifdef BUILD_LK
#else
	#include <linux/proc_fs.h>
#endif
#endif
/* Lenovo-sw yexm1 add, 20130819 end */


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  				(720)
#define FRAME_HEIGHT 				(1280)

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//#define LCM_DSI_CMD_MODE			0

#define LCM_ID_OTM1282A     0x1A

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
static unsigned int lcm_esd_test = FALSE; ///only for ESD test

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    	(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 		(lcm_util.udelay(n))
#define MDELAY(n) 		(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)						lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)			lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd) 						lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   		lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    
/* Lenovo-sw yexm1 add, 20130819 begin */
#if defined(LENOVO_ID_READ)
#ifdef BUILD_LK
#else
#define OTM1283A_CONFIG_PROC_FILE     "lcm_config"
static struct proc_dir_entry *otm1283a_config_proc = NULL;
#endif
#endif
/* Lenovo-sw yexm1 add, 20130819 end */ 
       
static unsigned int lcm_compare_id(void);
static unsigned int lcm_check_status(void);
static unsigned int lcm_esd_check(void);
static unsigned int lcm_esd_recover(void);
static unsigned int lcm_resume_esd_check(void);
/* Lenovo-sw yexm1 modify for esd_check, 20131127 */
#if defined(LENOVO_LCM_ESD_HS_READ)
extern void Lenovo_set_HS_read();
extern void Lenovo_restore_HS_read();
#endif
// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

		// enable tearing-free
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = BURST_VDO_MODE;
#endif
	
		// DSI
		/* Command mode setting */
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		params->dsi.packet_size=256;
        /* Lenovo-sw yexm1 add, fix the rebooting while boot bug, 20130428 */
        params->dsi.compatibility_for_nvk=0;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 0;

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

		params->dsi.vertical_sync_active				= 2;
		params->dsi.vertical_backporch					= 14;
		params->dsi.vertical_frontporch					= 16;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 2;
		params->dsi.horizontal_backporch				= 34;
		params->dsi.horizontal_frontporch				= 24;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

		// Bit rate calculation
		params->dsi.pll_div1=0;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)
		params->dsi.pll_div2=1; 		// div2=0~15: fout=fvo/(2*div2)
		params->dsi.fbk_div =15;    //fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)


}

/* Lenovo-sw yexm1 modify, 20130717 */
static void lcm_init_register(void)
{
	unsigned int data_array[16];

    	data_array[0] = 0x00023902;
    	data_array[1] = 0x00000000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00043902; //EXTC=1
    	data_array[1] = 0x018312FF;                 
    	dsi_set_cmdq(&data_array, 2, 1);    

    	data_array[0] = 0x00023902; //ORISE mode enable
    	data_array[1] = 0x00008000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x008312FF;    
    	dsi_set_cmdq(&data_array, 2, 1);    



		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000a3902; //10
    	data_array[1] = 0x005A00C0;//0x006400C0;
		data_array[2] = 0x6400110F;
		data_array[3] = 0x0000110F;
    	dsi_set_cmdq(&data_array, 4, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00073902; //07
    	data_array[1] = 0x005C00C0;
		data_array[2] = 0x00040001;
    	dsi_set_cmdq(&data_array, 3, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000A400;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000000C0; 
    	dsi_set_cmdq(&data_array, 2, 1); 

		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008700;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000018C4; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		/* Lenovo-sw yexm1 modify 20131017 begin */
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000030C4; 
    	dsi_set_cmdq(&data_array, 2, 1);

		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008B00;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000000C4; 
    	dsi_set_cmdq(&data_array, 2, 1);
	/* Lenovo-sw yexm1 modify 20131017 end */
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B300;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x005000c0; //0x005000c0
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008100;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000066C1;//55=60Hz ;66=65Hz
    	dsi_set_cmdq(&data_array, 2, 1); 

		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008100;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000082C4; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008200;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000002C4; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000049C4; 
    	dsi_set_cmdq(&data_array, 2, 1);

		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000C600;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000003B0; 
    	dsi_set_cmdq(&data_array, 2, 1); 		
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00053902; //5
    	data_array[1] = 0x021102F5;
		data_array[2] = 0x00000011;
	   	dsi_set_cmdq(&data_array, 3, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000050C5; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009400;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000066C5; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009400;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000002F5; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000BA00;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000003F5; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B200;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x000000F5; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B400;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x000000F5; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B600;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x000000F5; 
    	dsi_set_cmdq(&data_array, 2, 1);

		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B800;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x000000F5; 
    	dsi_set_cmdq(&data_array, 2, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B400;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x0000C0C5; 
    	dsi_set_cmdq(&data_array, 2, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B200;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000040C5; 
    	dsi_set_cmdq(&data_array, 2, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000A000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x061005C4;
		data_array[2] = 0x10150502;
		data_array[3] = 0x02071005;
		data_array[4] = 0x00101505;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x000000C4; 
    	dsi_set_cmdq(&data_array, 2, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009100;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x005007C5; 
    	dsi_set_cmdq(&data_array, 2, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00000000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x00BCBCD8; 
    	dsi_set_cmdq(&data_array, 2, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x00B804C5; 
    	dsi_set_cmdq(&data_array, 2, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000BB00;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000080C5; 
    	dsi_set_cmdq(&data_array, 2, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00000000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x000040D0; 
    	dsi_set_cmdq(&data_array, 2, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00000000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00033902;
    	data_array[1] = 0x000000D1; 
    	dsi_set_cmdq(&data_array, 2, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000C3902; //12
    	data_array[1] = 0x000000CB;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		dsi_set_cmdq(&data_array, 4, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00103902; //16
    	data_array[1] = 0x000000CB;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000A000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00103902; //16
    	data_array[1] = 0x000000CB;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00103902; //16
    	data_array[1] = 0x000000CB;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000C000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00103902; //16
    	data_array[1] = 0x050505CB;
		data_array[2] = 0x00050505;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000D000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00103902; //16
    	data_array[1] = 0x000000CB;
		data_array[2] = 0x05050000;
		data_array[3] = 0x05050505;
		data_array[4] = 0x00000505;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000E000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x000000CB;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00050500;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000F000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000C3902; //12
    	data_array[1] = 0xFFFFFFCB;
		data_array[2] = 0xFFFFFFFF;
		data_array[3] = 0xFFFFFFFF;
	   	dsi_set_cmdq(&data_array, 4, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00103902; //16
    	data_array[1] = 0x0D0B09CC;
		data_array[2] = 0x0003010F;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00103902; //16
    	data_array[1] = 0x000000CC;
		data_array[2] = 0x2D2E0000;
		data_array[3] = 0x100E0C0A;
		data_array[4] = 0x00000402;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000A000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x000000CC;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x002D2E00;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00103902; //16
    	data_array[1] = 0x0C0E10CC;
		data_array[2] = 0x0002040A;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000C000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00103902; //16
    	data_array[1] = 0x000000CC;
		data_array[2] = 0x2E2D0000;
		data_array[3] = 0x090B0D0F;
		data_array[4] = 0x00000103;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000D000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x000000CC;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x002E2D00;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000D3902; //13
    	data_array[1] = 0x00038FCE;
		data_array[2] = 0x8D00038E;
		data_array[3] = 0x038C0003;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1);
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x000000CE;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		/*Lenovo-sw yexm1 modify, 20131224 */
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000A000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x050B38CE;
		data_array[2] = 0x040A0000;
		data_array[3] = 0x01050A38;
		data_array[4] = 0x00040A00;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x050938CE;
		data_array[2] = 0x040A0002;
		data_array[3] = 0x03050838;
		data_array[4] = 0x00040A00;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000C000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x050738CE;
		data_array[2] = 0x040A0004;
		data_array[3] = 0x05050638;
		data_array[4] = 0x00040A00;
    	dsi_set_cmdq(&data_array, 5, 1); 
			
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000D000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x050538CE;
		data_array[2] = 0x040A0006;
		data_array[3] = 0x07050438;
		data_array[4] = 0x00040A00;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00008000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x000000CF;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00009000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x000000CF;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000A000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x000000CF;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000F3902; //15
    	data_array[1] = 0x000000CF;
		data_array[2] = 0x00000000;
		data_array[3] = 0x00000000;
		data_array[4] = 0x00000000;
    	dsi_set_cmdq(&data_array, 5, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000C000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x000C3902; //12
    	data_array[1] = 0x200101CF;
		data_array[2] = 0x01000020;
		data_array[3] = 0x08000000;//yexm1 modify 0x08000002
	   	dsi_set_cmdq(&data_array, 4, 1); 
		
		data_array[0] = 0x00023902;
    	data_array[1] = 0x0000B500;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00073902; //7
    	data_array[1] = 0xFFF133C5;
		data_array[2] = 0x00FFF133;
    	dsi_set_cmdq(&data_array, 3, 1); 
		
		
		
		
//--Gamma-------------------------
//E1
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x00000000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00113902; //17
    	data_array[1] = 0x0b0700E1;    
    	data_array[2] = 0x0c0e060c;    
    	data_array[3] = 0x0a06020b;    
    	data_array[4] = 0x0c120f06;    
    	data_array[5] = 0x00000000;     
    	dsi_set_cmdq(&data_array, 6, 1);     

 
//E2
    	data_array[0] = 0x00023902;
    	data_array[1] = 0x00000000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00113902; //25
    	data_array[1] = 0x0c0700E2;    
    	data_array[2] = 0x0c0e060c;    
    	data_array[3] = 0x0a05020b;    
    	data_array[4] = 0x0c120f06;    
    	data_array[5] = 0x00000000;      
    	dsi_set_cmdq(&data_array, 6, 1);      

    	data_array[0] = 0x00023902;
    	data_array[1] = 0x00000000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
		data_array[0] = 0x00023902;
    	data_array[1] = 0x000078D9;    
    	dsi_set_cmdq(&data_array, 2, 1); 

			
///////////////////////////////////////////////////// update by xuanquan.wang 2013/04/09
#if 0
data_array[0] = 0x00023902;
data_array[1] = 0x0000A000;
dsi_set_cmdq(&data_array, 2, 1);

data_array[0] = 0x00023902;
data_array[1] = 0x000002C1;
dsi_set_cmdq(&data_array, 2, 1);
MDELAY(1);

//data_array[0] = 0x00023902;
//data_array[1] = 0x0000A200;
//dsi_set_cmdq(&data_array, 2, 1);
//data_array[0] = 0x00023902;
//data_array[1] = 0x000008C1;
//dsi_set_cmdq(&data_array, 2, 1);
//data_array[0] = 0x00023902;
//data_array[1] = 0x0000A400;
//dsi_set_cmdq(&data_array, 2, 1);
//data_array[0] = 0x00023902;
//data_array[1] = 0x0000F0C1;
//dsi_set_cmdq(&data_array, 2, 1);
data_array[0] = 0x00023902;
data_array[1] = 0x00008000;
dsi_set_cmdq(&data_array, 2, 1);
data_array[0] = 0x00023902;
data_array[1] = 0x000030C4;
dsi_set_cmdq(&data_array, 2, 1);
MDELAY(10);
data_array[0] = 0x00023902;
data_array[1] = 0x00008A00;
dsi_set_cmdq(&data_array, 2, 1);
data_array[0] = 0x00023902;
data_array[1] = 0x000040C4;
dsi_set_cmdq(&data_array, 2, 1);
MDELAY(10);
#else
/* Lenovo-sw yexm1 modify, 20130718 */
data_array[0]=0x00023902;
data_array[1]=0x0000A000;
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00023902;
data_array[1]=0x000000C1;//yexm1 modify 0x000002C1
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00023902;
data_array[1]=0x0000A200;
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00023902;
data_array[1]=0x0000FFC1;//0x000008C1
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00023902;
data_array[1]=0x0000A400;
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00023902;
data_array[1]=0x0000FFC1;//0x0000F0C1
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00023902;
data_array[1]=0x00009000;
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00103902;
data_array[1]=0xC0C0C0CB;
data_array[2]=0x00C0C0C0;
data_array[3]=0x00000000;
data_array[4]=0x00000000;
dsi_set_cmdq(&data_array, 5, 1);

data_array[0]=0x00023902;
data_array[1]=0x0000A000;
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00103902;
data_array[1]=0x000000CB;
data_array[2]=0xC0C00000;
data_array[3]=0xC0C0C0C0;
data_array[4]=0x0000C0C0;
dsi_set_cmdq(&data_array, 5, 1);

data_array[0]=0x00023902;
data_array[1]=0x0000B000;
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00103902;
data_array[1]=0x000000CB;
data_array[2]=0x00000000;
data_array[3]=0x00000000;
data_array[4]=0xC0C00000;
dsi_set_cmdq(&data_array, 5, 1);

data_array[0]=0x00023902;
data_array[1]=0x0000B400;
dsi_set_cmdq(&data_array, 2, 1);

data_array[0]=0x00023902;
data_array[1]=0x000050C0;
dsi_set_cmdq(&data_array, 2, 1);
#endif
///////////////////////////////////////////////////////		
  
//orise mode disable
		data_array[0] = 0x00023902;
    	data_array[1] = 0x00000000;    
    	dsi_set_cmdq(&data_array, 2, 1);     
    	data_array[0] = 0x00043902; //EXTC=1
    	data_array[1] = 0x0FFFFFFF;                 
    	dsi_set_cmdq(&data_array, 2, 1);  

	data_array[0] = 0x00351500;//te on
    	dsi_set_cmdq(&data_array, 1, 1);    

     	data_array[0] = 0x00110500;                
    	dsi_set_cmdq(&data_array, 1, 1); 
    	MDELAY(120); 
    	
    	data_array[0] = 0x00290500;                
    	dsi_set_cmdq(&data_array, 1, 1);    
    	MDELAY(20); 

//    	data_array[0] = 0x002C0500;                
//    	dsi_set_cmdq(&data_array, 1, 1);    
//    	MDELAY(20); 

} 

static void lcm_init(void)
{	
	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	/*Lenovo-sw yexm1 modify 20130911 */
	MDELAY(10);//10ms
	SET_RESET_PIN(1);
	MDELAY(50);//50ms

#ifdef BUILD_LK
    printf("[wj]otm1283a init code.\n");
#endif
    lcm_init_register();
}

/* Lenovo-sw yexm1 modify, 20130924 begin */
static void lcm_suspend(void)
{
	unsigned int data_array[16];

#if defined(BUILD_LK) || defined(BUILD_UBOOT)
	printf("%s, lcm_init \n", __func__);
#else
	printk("%s, lcm_init \n", __func__);
#endif	
	data_array[0] = 0x00280500;                
	dsi_set_cmdq(&data_array, 1, 1);    

	data_array[0] = 0x00100500;                
	dsi_set_cmdq(&data_array, 1, 1);   
	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(50);
#if 1		
//begin lenovo jixu add for power consume 20130206
	SET_RESET_PIN(0);
//end lenovo jixu add for power consume 20130206
#endif
}


static void lcm_resume(void)
{
	int i;
	unsigned int data_array[16];

#if defined(BUILD_LK) || defined(BUILD_UBOOT)
	printf("%s, lcm_resume \n", __func__);
#else
	printk("%s, otm1283a lcm_resume \n", __func__);
#endif	
	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(50);

	lcm_init_register();
}
/* Lenovo-sw yexm1 modify, 20130924 end */
static unsigned int lcm_check_status(void)
{
	unsigned char buffer[2];
	int   array[4];
//	unsigned int i = 0;
	
	array[0] = 0x00013700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);
#if 0
	while(i < 10){
		read_reg_v2(0x0A, buffer, 1);
#ifdef BUILD_LK
		printf("Check LCM Status: 0x%08x\n", buffer[0]);
#else
		printk("Check LCM Status: 0x%08x\n", buffer[0]);
#endif
		if(buffer[0] != 0x9C)
			init_lcm_registers();
		else
			break;
		i++;
	}
#else
	read_reg_v2(0x0A, buffer, 1);
#ifdef BUILD_LK
	printf("Check LCM Status: 0x%08x\n", buffer[0]);
#else
	printk("Check LCM Status: 0x%08x\n", buffer[0]);
#endif
#endif
	return 0;
}

static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	data_array[3]= 0x00053902;
	data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[5]= (y1_LSB);
	data_array[6]= 0x002c3909;

	dsi_set_cmdq(&data_array, 7, 0);

}

static void lcm_setbacklight(unsigned int level)
{
	unsigned int data_array[16];


	if(level > 255) 
	level = 255;

	data_array[0] = 0x00023902;      
	data_array[1] = (0x51|(level<<8)); 
	dsi_set_cmdq(&data_array, 2, 1); 

}

static unsigned int lcm_esd_check(void)
{
unsigned char buffer[2],ret;
unsigned int array[16];

#ifndef BUILD_LK
	if(lcm_esd_test)
	{
	lcm_esd_test = FALSE;
	return TRUE;
	}

	/// please notice: the max return packet size is 1
	/// if you want to change it, you can refer to the following marked code
	/// but read_reg currently only support read no more than 4 bytes....
	/// if you need to read more, please let BinHan knows.

	/* Lenovo-sw yexm1 modify for esd_check, 20131127 */
	#if defined(LENOVO_LCM_ESD_HS_READ)
	Lenovo_set_HS_read();
	#endif
	array[0] = 0x00013708;
	dsi_set_cmdq(array, 1, 1);
	read_reg_v2(0x0A, buffer,2);
	#if defined(LENOVO_LCM_ESD_HS_READ)
	Lenovo_restore_HS_read();
	#endif
	#ifndef BUILD_LK
	printk("[JX] %s 0x0A 0=0x%x 1=0x%x \n",__func__,buffer[0],buffer[1]);
	#endif
	ret = buffer[0]==0x9C?0:1;
	#ifndef BUILD_LK
	printk("[JX] %s ret=%d \n",__func__,ret);
	#endif
	if(ret) return TRUE;
#if 0
	read_reg_v2(0x0D, buffer,2);
	#ifndef BUILD_LK
	printk("[JX] %s 0x0D 0=0x%x 1=0x%x \n",__func__,buffer[0],buffer[1]);
	#endif
	ret = buffer[0]==0x00?0:1;
	#ifndef BUILD_LK
	printk("[JX] %s ret=%d \n",__func__,ret);
	#endif
	if(ret) return TRUE;

	read_reg_v2(0x0E, buffer,2);
	#ifndef BUILD_LK
	printk("[JX] %s 0x0E 0=0x%x 1=0x%x \n",__func__,buffer[0],buffer[1]);
	#endif
	ret = buffer[0]==0x81?0:1;
	#ifndef BUILD_LK
	printk("[JX] %s ret=%d \n",__func__,ret);
	#endif
	if(ret) return TRUE;
#endif
	else return FALSE;
#endif
}

static unsigned int lcm_resume_esd_check(void)
{
	unsigned char buffer[2],ret;

	read_reg_v2(0x0A, buffer,2);
	ret = buffer[0]==0x08?0:1;
	if(ret) return TRUE;

	read_reg_v2(0x0D, buffer,2);
	ret = buffer[0]==0x00?0:1;
	if(ret) return TRUE;

	read_reg_v2(0x0E, buffer,2);
	ret = ((buffer[0])&(0xf0))==0x80?0:1;
	if(ret) return TRUE;
	else return FALSE;
}

static unsigned int lcm_esd_recover(void)
{
#if 1
	unsigned char para = 0;

	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(50);

	lcm_init_register();

	return TRUE;
#else
lcm_suspend();
MDELAY(50);
lcm_resume();
#endif
}

static unsigned int lcm_compare_id(void)
{
	unsigned int ret = 0;
	mt_set_gpio_mode(GPIO92|0x80000000,GPIO_MODE_GPIO);
	mt_set_gpio_dir(GPIO92|0x80000000,GPIO_DIR_IN);
	mt_set_gpio_pull_enable(GPIO92|0x80000000,GPIO_PULL_DISABLE);
	ret = mt_get_gpio_in(GPIO92|0x80000000);
#if defined(BUILD_LK)
	printf("%s, [yexm1]otm1283a GPIO92 = %d \n", __func__, ret);
#endif	

	return (ret == 1)?1:0;
}

/* Lenovo-sw yexm1 add, 20130819 begin */
#if defined(LENOVO_ID_READ)
#ifdef BUILD_LK
#else
extern LCM_DRIVER otm1283a_hd720_dsi_vdo_lcm_drv;
static int otm1283a_config_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    char *ptr = page;

    ptr += sprintf(ptr, "%s\n", otm1283a_hd720_dsi_vdo_lcm_drv.name);
	ptr += sprintf(ptr, "%s\n", otm1283a_hd720_dsi_vdo_lcm_drv.module);

    *eof = 1;
    return (ptr - page);
}

static void lcm_read_id(void)
{
	// Create proc file system
	otm1283a_config_proc = create_proc_entry(OTM1283A_CONFIG_PROC_FILE, 0664, NULL);

	if (otm1283a_config_proc == NULL)
	{
	    return;
	}
	else
	{
	    otm1283a_config_proc->read_proc = otm1283a_config_read_proc;
	}
}
#endif
#endif
/* Lenovo-sw yexm1 add, 20130819 end */

LCM_DRIVER otm1283a_hd720_dsi_vdo_lcm_drv = 
{
	.name = "otm1283a_hd720_dsi_vdo",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,
	.esd_check   = lcm_esd_check,
    .esd_recover   = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
	.set_backlight	= lcm_setbacklight,
    .update         = lcm_update,
#endif
	/* Lenovo-sw yexm1 add, 20130819 begin */
	#if defined(LENOVO_ID_READ)
	#ifdef BUILD_LK
	#else
	.module			= "boe",
	.read_id   = lcm_read_id,
	#endif
	#endif
	/* Lenovo-sw yexm1 add, 20130819 end */
};
