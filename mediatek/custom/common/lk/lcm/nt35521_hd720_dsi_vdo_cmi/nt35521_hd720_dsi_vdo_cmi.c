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
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
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

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (720)
#define FRAME_HEIGHT (1280)


#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#define LCM_ID    0x21
#define LCM_BACKLIGHT_EN GPIO18
#define LCM_ID_PIN GPIO114
#define LCM_COMPARE_BY_SW 0

static unsigned int lcm_esd_test = FALSE;      ///only for ESD test
static unsigned int lcm_esd_check(void);
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V3(para_tbl,size,force_update)        lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define dsi_set_cmdq_V4(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq_V4(pdata, queue_size, force_update)

#define   LCM_DSI_CMD_MODE							0



static LCM_setting_table_V3 lcm_initialization_setting[] = {
	
	/*
	Note :

	Data ID will depends on the following rule.
	
		count of parameters > 1	=> Data ID = 0x39
		count of parameters = 1	=> Data ID = 0x15
		count of parameters = 0	=> Data ID = 0x05

	Structure Format :

	{DCS command, count of parameters, {parameter list}}
	{REGFLAG_DELAY, milliseconds of time, {}},

	...

	Setting ending by predefined flag
	
	{REGFLAG_END_OF_TABLE, 0x00, {}}
	*/


	
{0x39,0xFF,4,{0xAA,0x55,0xA5,0x80}},				
           
{0x39,0x6F,2,{0x11,0x00}},				
			
{0x39,0xF7,2,{0x20,0x00}},				
			
           
{0x39,0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},				
			
{0x39,0xB1,2,{0x60,0x21}},				
			
{0x39,0xBC,2,{0x00,0x00}},				

{0x39,0xd9,2,{0x02,0x04}},//pwm frequence 58K
			
{0x39,0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},				
			
				
{0x39,0xB0,2,{0x0F,0x0F}},				
			
{0x39,0xB1,2,{0x0F,0x0F}},				
           
{0x39,0xBC,2,{0xA0,0x00}},				
{0x39,0xBD,2,{0xA0,0x00}},				
           
{0x15,0xCA,1,{0x01}},				
{0x15,0xC0,1,{0x04}},				
           
//{0x15,0xBE,1,{0x77}},	//VCOM
	     
{0x39,0xB3,2,{0x38,0x38}},
{0x39,0xB4,2,{0x11,0x11}},
           
{0x39,0xB9,2,{0x44,0x44}},				
{0x39,0xBA,2,{0x13,0x13}},
{0x39,0xC4,2,{0x11,0x11}},
{0x15,0xCE,1,{0x66}},
           
{0x39,0xF0,5,{0x55,0xAA,0x52,0x08,0x02}},
//gamma code start 20131114 by suk.lu			
{0x15,0xEE,1,{0x03}},
  			{0x39,0xB0,16,{0x01,0x5A,0x01,0x5E,0x01,0x65,0x01,0x6D,0x01,0x76,0x01,0x83,0x01,0x90,0x01,0xA6}},   //R+
  			{0x39,0xB1,16,{0x01,0xBC,0x01,0xE2,0x02,0x02,0x02,0x39,0x02,0x67,0x02,0x68,0x02,0x93,0x02,0xC5}},
  			{0x39,0xB2,16,{0x02,0xE6,0x03,0x0F,0x03,0x2F,0x03,0x56,0x03,0x6F,0x03,0x82,0x03,0x9F,0x03,0xB3}},
{0x39,0xB3,4,{0x03,0xE0,0x03,0xFF}},
  			{0x39,0xB4,16,{0x01,0x5A,0x01,0x5E,0x01,0x65,0x01,0x6D,0x01,0x76,0x01,0x83,0x01,0x90,0x01,0xA6}},   //G+
  			{0x39,0xB5,16,{0x01,0xBC,0x01,0xE2,0x02,0x02,0x02,0x39,0x02,0x67,0x02,0x68,0x02,0x93,0x02,0xC5}},
  			{0x39,0xB6,16,{0x02,0xE6,0x03,0x0F,0x03,0x2F,0x03,0x56,0x03,0x6F,0x03,0x82,0x03,0x9F,0x03,0xB3}},
{0x39,0xB7,4,{0x03,0xE0,0x03,0xFF}},
{0x39,0xB8,16,{0x00,0x0A,0x00,0x4F,0x00,0x83,0x00,0xA6,0x00,0xB8,0x00,0xEB,0x01,0x0C,0x01,0x42}},
{0x39,0xB9,16,{0x01,0x68,0x01,0xA2,0x01,0xD3,0x02,0x18,0x02,0x53,0x02,0x54,0x02,0x83,0x02,0xBA}},
{0x39,0xBA,16,{0x02,0xDE,0x03,0x12,0x03,0x35,0x03,0x63,0x03,0x82,0x03,0xA9,0x03,0xBF,0x03,0xD0}},
{0x39,0xBB,4,{0x03,0xE0,0x03,0xFF}},		
//gamma code end20131114
						
{0x39,0xF0,5,{0x55,0xAA,0x52,0x08,0x06}},				
{0x39,0xB0,2,{0x00,0x10}},				
{0x39,0xB1,2,{0x12,0x14}},				
{0x39,0xB2,2,{0x16,0x18}},				
{0x39,0xB3,2,{0x1A,0x2A}},				
{0x39,0xB4,2,{0x29,0x02}},				
{0x39,0xB5,2,{0x34,0x31}},				
{0x39,0xB6,2,{0x34,0x31}},				
{0x39,0xB7,2,{0x31,0x34}},				
{0x39,0xB8,2,{0x34,0x04}},				
{0x39,0xB9,2,{0x34,0x34}},				
{0x39,0xBA,2,{0x34,0x34}},				
{0x39,0xBB,2,{0x05,0x34}},				
{0x39,0xBC,2,{0x31,0x31}},				
{0x39,0xBD,2,{0x31,0x34}},
{0x39,0xBE,2,{0x31,0x34}},
{0x39,0xBF,2,{0x03,0x29}},
{0x39,0xC0,2,{0x2A,0x1B}},
{0x39,0xC1,2,{0x19,0x17}},
{0x39,0xC2,2,{0x15,0x13}},
{0x39,0xC3,2,{0x11,0x01}},
{0x39,0xE5,2,{0x31,0x31}},
{0x39,0xC4,2,{0x03,0x1B}},
{0x39,0xC5,2,{0x19,0x17}},
           
{0x15,0xC4,1,{0x82}},
{0x15,0xC5,1,{0xA2}},
           
{0x39,0xC6,2,{0x15,0x13}},
{0x39,0xC7,2,{0x11,0x2A}},
{0x39,0xC8,2,{0x29,0x01}},
{0x39,0xC9,2,{0x34,0x31}},
{0x39,0xCA,2,{0x34,0x31}},
{0x39,0xCB,2,{0x31,0x34}},
{0x39,0xCC,2,{0x34,0x05}},
{0x39,0xCD,2,{0x34,0x34}},
{0x39,0xCE,2,{0x34,0x34}},
{0x39,0xCF,2,{0x04,0x34}},
{0x39,0xD0,2,{0x31,0x31}},
{0x39,0xD1,2,{0x31,0x34}},
{0x39,0xD2,2,{0x31,0x34}},
{0x39,0xD3,2,{0x00,0x29}},
{0x39,0xD4,2,{0x2A,0x10}},
{0x39,0xD5,2,{0x12,0x14}},
{0x39,0xD6,2,{0x16,0x18}},
{0x39,0xD7,2,{0x1A,0x02}},
{0x39,0xE5,2,{0x31,0x31}},				
{0x39,0xE6,2,{0x31,0x31}},			
{0x39,0xD8,5,{0x00,0x00,0x00,0x14,0x00}},				
{0x39,0xD9,5,{0x00,0x15,0x00,0x00,0x00}},				
{0x15,0xE7,1,{0x00}},				
           
{0x39,0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},				
{0x39,0xB1,2,{0x00,0x00}},				
{0x39,0xB0,2,{0x00,0x00}},				
{0x39,0xB2,5,{0x05,0x00,0x00,0x00,0x00}},				
{0x39,0xB3,5,{0x05,0x00,0x00,0x00,0x00}},				
{0x39,0xB4,5,{0x05,0x00,0x00,0x00,0x00}},				
{0x39,0xB5,5,{0x05,0x00,0x17,0x00,0x00}},
{0x39,0xB6,5,{0x12,0x00,0x19,0x00,0x00}},				
{0x39,0xB7,5,{0x12,0x00,0x19,0x00,0x00}},				
{0x39,0xB8,5,{0x12,0x00,0x19,0x00,0x00}},				
{0x39,0xB9,5,{0x12,0x00,0x19,0x00,0x00}},				
{0x39,0xBA,5,{0x57,0x00,0x00,0x00,0x00}},				
{0x39,0xBB,5,{0x57,0x00,0x00,0x00,0x00}},				
{0x39,0xBC,5,{0x75,0x00,0x1A,0x00,0x00}},				
{0x39,0xBD,5,{0x53,0x00,0x1A,0x00,0x00}},				
           
{0x39,0xC0,4,{0x00,0x34,0x00,0x00}},			
{0x39,0xC1,4,{0x00,0x34,0x00,0x00}},			
{0x39,0xC2,4,{0x00,0x34,0x00,0x00}},			
{0x39,0xC3,4,{0x00,0x34,0x00,0x00}},			
{0x15,0xC4,1,{0x20}},				
{0x15,0xC5,1,{0x80}},				
{0x15,0xC6,1,{0x00}},				
{0x15,0xC7,1,{0x00}},				
           
{0x39,0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},				
{0x15,0xED,1,{0x30}},
{0x15,0xE5,1,{0x00}},
{0x39,0xB0,2,{0x17,0x06}},
{0x15,0xB8,1,{0x08}},
{0x39,0xBD,5,{0x03,0x07,0x00,0x03,0x00}},
           
{0x39,0xB1,2,{0x17,0x06}},
{0x39,0xB9,2,{0x00,0x07}},
{0x39,0xB2,2,{0x00,0x00}},
{0x39,0xBA,2,{0x00,0x00}},
{0x39,0xB3,2,{0x17,0x06}},
{0x39,0xBB,2,{0x0A,0x03}},
{0x39,0xB4,2,{0x17,0x06}},
{0x39,0xB5,2,{0x17,0x06}},
{0x39,0xB6,2,{0x14,0x03}},
{0x39,0xB7,2,{0x00,0x00}},
{0x39,0xBC,2,{0x02,0x00}},
           
{0x15,0xE5,1,{0x06}},
{0x15,0xE6,1,{0x06}},
{0x15,0xE7,1,{0x00}},
{0x15,0xE8,1,{0x06}},
{0x15,0xE9,1,{0x06}},
{0x15,0xEA,1,{0x06}},
{0x15,0xEB,1,{0x00}},
{0x15,0xEC,1,{0x00}},
       
{0x15,0xC0,1,{0x07}},
{0x15,0xC1,1,{0x80}},
{0x15,0xC2,1,{0xA4}},
{0x15,0xC3,1,{0x05}},
{0x15,0xC4,1,{0x00}},
{0x15,0xC5,1,{0x02}},
{0x15,0xC6,1,{0x22}},
{0x15,0xC7,1,{0x03}},
{0x39,0xC8,2,{0x05,0x30}},
{0x39,0xC9,2,{0x01,0x31}},
           
{0x39,0xCA,2,{0x03,0x21}},
{0x39,0xCB,2,{0x01,0x20}},
{0x39,0xD1,5,{0x00,0x04,0xFE,0x07,0x10}},
{0x39,0xD2,5,{0x10,0x05,0x04,0x03,0x10}},
{0x39,0xD3,5,{0x20,0x00,0x48,0x07,0x10}},
{0x39,0xD4,5,{0x30,0x00,0x43,0x07,0x10}},
{0x39,0xD0,7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x39,0xD5,7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x15,0x6F,1,{0x07}},
{0x39,0xD5,4,{0x00,0x00,0x00,0x00}},
{0x39,0xD6,7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00}},			
{0x15,0x6F,1,{0x07}},				
{0x39,0xD6,4,{0x00,0x00,0x00,0x00}},			
{0x39,0xD7,7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00}},				
{0x15,0x6F,1,{0x07}},				
{0x39,0xD7,4,{0x00,0x00,0x00,0x00}},			
{0x39,0xD8,5,{0x00,0x00,0x00,0x00,0x00}},			
{0x39,0xCC,3,{0x00,0x00,0x3E}},				
{0x39,0xCD,3,{0x00,0x00,0x3E}},				
{0x39,0xCE,3,{0x00,0x00,0x02}},				
{0x39,0xCF,3,{0x00,0x00,0x02}},				
           
{0x15,0x6F,1,{0x11}},			
{0x15,0xF3,1,{0x01}},

{0x15,0x35,1,{0x00}},
{0x15,0x53,1,{0x24}},
{0x15,0x55,1,{0x00}},//cabc 03:move 00:off

{0x05, 0x11,0,{}},//
{REGFLAG_ESCAPE_ID,REGFLAG_DELAY_MS_V3,120,{}},

{0x05, 0x29,0,{}},//
{REGFLAG_ESCAPE_ID,REGFLAG_DELAY_MS_V3,20,{}},				 

	/* FIXME */
	/*
		params->dsi.horizontal_sync_active				= 0x16;// 50  2
		params->dsi.horizontal_backporch				= 0x38;
		params->dsi.horizontal_frontporch				= 0x18;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
		params->dsi.horizontal_blanking_pixel =0;    //lenovo:fix flicker issue
	    //params->dsi.LPX=8; 
	*/

};


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
	unsigned int div1 = 0;
	unsigned int div2 = 0;
	unsigned int pre_div = 0;
	unsigned int post_div = 0;
	unsigned int fbk_sel = 0;
	unsigned int fbk_div = 0;
	unsigned int lane_no = 0;//lcm_params->dsi.LANE_NUM;

	unsigned int cycle_time;
	unsigned int ui;
	unsigned int hs_trail_m, hs_trail_n;

		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

        #if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
        #else
		params->dsi.mode   = BURST_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE; 
        #endif
	
		// DSI
		/* Command mode setting */
		//1 Three lane or Four lane
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Video mode setting		
		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		

		params->dsi.vertical_sync_active				= 2;
		params->dsi.vertical_backporch					= 14;
		params->dsi.vertical_frontporch					= 16;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 2;
		params->dsi.horizontal_backporch				= 34;
		params->dsi.horizontal_frontporch				= 34;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

		// Bit rate calculation
		params->dsi.pll_div1=0;		// fref=26MHz, fvco=fref*(div1+1)	(div1=0~63, fvco=500MHZ~1GHz)
		params->dsi.pll_div2=1; 		// div2=0~15: fout=fvo/(2*div2)
		params->dsi.fbk_div =16;    //fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)
#if 1
				params->bl_app.min =1;
				params->bl_app.def =102;
				params->bl_app.max =255;
		
				params->bl_bsp.min =6;//min > 500ns
				params->bl_bsp.def =102;
				params->bl_bsp.max =255;
#endif

#if 1
        div1 = params->dsi.pll_div1;
        div2 = params->dsi.pll_div2;
        fbk_div = params->dsi.fbk_div;
		switch(div1)
   		{
	  		case 0:
		 	div1 = 1;
		 	break;

			case 1:
		 	div1 = 2;
		 	break;

			case 2:
			case 3:
			 div1 = 4;
			 break;

			default:
				div1 = 4;
				break;
		}

		switch(div2)
		{
			case 0:
				div2 = 1;
				break;
			case 1:
				div2 = 2;
				break;
			case 2:
			case 3:
				div2 = 4;
				break;
			default:
				div2 = 4;
				break;
		}

		switch(pre_div)
		{
		  case 0:
			 pre_div = 1;
			 break;

		  case 1:
			 pre_div = 2;
			 break;

		  case 2:
		  case 3:
			 pre_div = 4;
			 break;

		  default:
			 pre_div = 4;
			 break;
		}

		switch(post_div)
		{
		  case 0:
			 post_div = 1;
			 break;

		  case 1:
			 post_div = 2;
			 break;

		  case 2:
		  case 3:
			 post_div = 4;
			 break;

		  default:
			 post_div = 4;
			 break;
		}

		switch(fbk_sel)
		{
		  case 0:
			 fbk_sel = 1;
			 break;

		  case 1:
			 fbk_sel = 2;
			 break;

		  case 2:
		  case 3:
			 fbk_sel = 4;
			 break;

		  default:
			 fbk_sel = 4;
			 break;
		}
  		cycle_time=(1000*4*div2*div1)/(fbk_div*26)+0x01;

		ui=(1000*div2*div1)/(fbk_div*26*0x2)+0x01;
#define NS_TO_CYCLE(n, c)	((n) / (c))

   hs_trail_m=1;
   hs_trail_n= NS_TO_CYCLE(((hs_trail_m * 0x4) + 0x60 - 75), cycle_time);
   // +3 is recommended from designer becauase of HW latency
   params->dsi.HS_TRAIL = ((hs_trail_m > hs_trail_n) ? hs_trail_m : hs_trail_n);

   params->dsi.HS_PRPR	=  NS_TO_CYCLE((0x40 + 0x5 * ui + 25), cycle_time);
   // HS_PRPR can't be 1.
   if (params->dsi.HS_PRPR == 0)
	  params->dsi.HS_PRPR = 1;

#endif

}

static void lcm_init(void)
{
	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(5);
	SET_RESET_PIN(1);
	MDELAY(10);

    dsi_set_cmdq_V3(lcm_initialization_setting,sizeof(lcm_initialization_setting)/sizeof(lcm_initialization_setting[0]),1);
    #ifdef BUILD_LK
	  printf("[LK]---LCM---nt35521----%s------\n",__func__);
    #else
	  printk("[KERNEL]---LCM---nt35521----%s------\n",__func__);
    #endif
mt_set_gpio_out(LCM_BACKLIGHT_EN, 1);
}



static void lcm_suspend(void)
{
	unsigned int data_array[16];
mt_set_gpio_out(LCM_BACKLIGHT_EN, 0);

	data_array[0]=0x00280500; // Display Off
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);
	data_array[0] = 0x00100500; // Sleep In
	dsi_set_cmdq(data_array, 1, 1);



}


static void lcm_resume(void)
{
	unsigned int data_array[16];
    #ifdef BUILD_LK
	  printf("[LK]---LCM---nt35521----%s------\n",__func__);
    #else
	  printk("[KERNEL]---LCM---nt35521----%s------\n",__func__);
    #endif	

	data_array[0] = 0x00110500;
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(150);

	data_array[0] = 0x00290500;
	dsi_set_cmdq(&data_array, 1, 1);
	MDELAY(5);
mt_set_gpio_out(LCM_BACKLIGHT_EN, 1);
}
         
#if (LCM_DSI_CMD_MODE)
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
	dsi_set_cmdq(data_array, 3, 1);
	
	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0]= 0x002c3909;
	dsi_set_cmdq(data_array, 1, 0);

}
#endif

static unsigned int lcm_compare_id(void)
{
#if LCM_COMPARE_BY_SW
	unsigned int id=0;
	unsigned char buffer[2];
	unsigned int data_array[16];  

	SET_RESET_PIN(1);  //NOTE:should reset LCM firstly
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(10);	

/*	
	data_array[0] = 0x00110500;		// Sleep Out
	dsi_set_cmdq_V4(data_array, 1, 1);
	MDELAY(120);
*/
		
//*************Enable CMD2 Page1  *******************//
	data_array[0]=0x00063902;
	data_array[1]=0x52AA55F0;
	data_array[2]=0x00000108;
	dsi_set_cmdq_V4(data_array, 3, 1);
	MDELAY(10); 

	data_array[0] = 0x00023700;// read id return two byte,version and id
	dsi_set_cmdq_V4(data_array, 1, 1);
	MDELAY(10); 
	
	read_reg_v2(0xC5, buffer, 2);

	id = buffer[1];
#if defined(BUILD_LK) || defined(BUILD_UBOOT)
	printf("%s, nt35521 id = 0x%x \n", __func__, id);
#else
       printk("%s, nt35521 id = 0x%x \n", __func__, id);
#endif

	return (LCM_ID == id)?1:0;
//return 1;
#else

	unsigned int ret = 0;
	ret = mt_get_gpio_in(LCM_ID_PIN);
#if defined(BUILD_LK)
	printf("%s, [jx]nt35521 LCM_ID_PIN = %d \n", __func__, ret);
#endif	

	return (ret == 0)?1:0;

#endif
}


static unsigned int lcm_esd_check(void)
{
unsigned char buffer[2],ret;

#ifndef BUILD_UBOOT
	if(lcm_esd_test)
	{
	lcm_esd_test = FALSE;
	return TRUE;
	}

	/// please notice: the max return packet size is 1
	/// if you want to change it, you can refer to the following marked code
	/// but read_reg currently only support read no more than 4 bytes....
	/// if you need to read more, please let BinHan knows.
	/*
	unsigned int data_array[16];
	unsigned int max_return_size = 1;

	data_array[0]= 0x00003700 | (max_return_size << 16); 

	dsi_set_cmdq(&data_array, 1, 1);
	*/
	//return FALSE;//jixu temp
	read_reg_v2(0x0A, buffer,2);
#ifndef BUILD_LK
	//printk("[JX] %s 0x0A 0=0x%x 1=0x%x \n",__func__,buffer[0],buffer[1]);
#endif
	ret = buffer[0]==0x9C?0:1;
#ifndef BUILD_LK
	//printk("[JX] %s ret=%d \n",__func__,ret);
#endif
	if(ret) return TRUE;

	read_reg_v2(0x0E, buffer,2);
#ifndef BUILD_LK
	//printk("[JX] %s 0x0E 0=0x%x 1=0x%x \n",__func__,buffer[0],buffer[1]);
#endif
	ret = ((buffer[0])&(0xf0))==0x80?0:1;
#ifndef BUILD_LK
	//printk("[JX] %s ret=%d \n",__func__,ret);
#endif
	if(ret) return TRUE;
	else return FALSE;
#endif
}


static unsigned int lcm_esd_recover(void)
{
#ifndef BUILD_LK
	printk("[JX]+ %s \n",__func__);
#endif
	lcm_init();
#ifndef BUILD_LK
	printk("[JX]- %s \n",__func__);
#endif

}


static void lcm_setbacklight(unsigned int level)
{
	unsigned int data_array[16];


	if(level > 255) 
	level = 255;

	data_array[0] = 0x00023902;      
	data_array[1] = (0x51|(level<<8)); 
	#ifdef BUILD_LK
	dsi_set_cmdq_V4(&data_array, 2, 1);
	#else
	dsi_set_cmdq(&data_array, 2, 1); 
	#endif

}
static unsigned int lcm_cabcmode_index = 0;

static void lcm_setcabcmode(unsigned int mode)
{
	unsigned int data_array[16];

#ifdef BUILD_LK
	printf("%s mode=%d\n",__func__,mode);
#else
	printk("%s mode=%d\n",__func__,mode);
#endif

	switch(mode){
		case 0:
			lcm_cabcmode_index=0;
			break;
		case 1:
			lcm_cabcmode_index=1;
			break;
		case 2:
			lcm_cabcmode_index=2;
			break;
		case 3:
			lcm_cabcmode_index=3;
			break;
		default:
			break;
	}
	data_array[0] = 0x00023902; 	 
	data_array[1] = (0x55|(lcm_cabcmode_index<<8)); 
	dsi_set_cmdq(&data_array, 2, 1); 

	 MDELAY(10);
}


static void lcm_getcabcstate(unsigned int * state)
{

	*state = lcm_cabcmode_index;
	return ;
}
#ifdef LENOVO_LCD_BACKLIGHT_COMPATIBLE_SUPPORT
static int lcm_detect_backlight(void)
{
//return 1; that is lcm control backlight
//for scofield GPIO127=1 and GPIO128=1 is BB PWM control;
		unsigned int ret_gpio127 = 0;
		unsigned int ret_gpio128 = 0;
		ret_gpio127 = mt_get_gpio_in(GPIO127);
		ret_gpio128 = mt_get_gpio_in(GPIO128);
#if defined(BUILD_LK)
		printf("[JX] %s,  gpio127=%d gpio128=%d \n", __func__, ret_gpio127,ret_gpio128);
#endif	
	return !(ret_gpio127 & ret_gpio128);

}
#endif

static void lcm_set_inverse(unsigned int mode)
{
	unsigned int data_array[16];

#ifdef BUILD_LK
	printf("%s mode=%d\n",__func__,mode);
#else
	printk("%s mode=%d\n",__func__,mode);
#endif

	if(mode){
		data_array[0] = 0x00210500;
		dsi_set_cmdq(data_array, 1, 1);
	}else
	{
		data_array[0] = 0x00200500;
		dsi_set_cmdq(data_array, 1, 1);
		
	}
	
	 MDELAY(10);
}

LCM_DRIVER nt35521_hd720_dsi_vdo_cmi_lcm_drv = 
{
    .name			= "nt35521_hd720_dsi_vdo_cmi",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
//	.suspend_power  = lcm_suspend_power,//lenovo.sw wangsx3 20310918 add suspend_power API in LCM driver
	.compare_id     = lcm_compare_id,
	.esd_check = lcm_esd_check,
	.esd_recover = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
	.set_backlight	= lcm_setbacklight,
#if defined(LENOVO_LCD_BACKLIGHT_CONTROL_BY_LCM) || defined(LENOVO_LCD_BACKLIGHT_COMPATIBLE_SUPPORT)
		.set_cabcmode = lcm_setcabcmode,
		.get_cabcmode = lcm_getcabcstate,
#endif
#ifdef LENOVO_LCD_BACKLIGHT_COMPATIBLE_SUPPORT
	.detect_backlight = lcm_detect_backlight,
#endif
	.set_inversemode = lcm_set_inverse,

    };
