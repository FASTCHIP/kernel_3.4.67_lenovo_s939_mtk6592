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

#define   LCM_DSI_CMD_MODE							0

/*Lenovo-sw yexm1 modify 20140221 */
//#define GPIO_LCD_ENP_PIN GPIO118 //modify by lenovo.sw2 houdz1
//#define GPIO_LCD_ENN_PIN GPIO117



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
//lenovo.sw wangsx 20130815 update lcm parameters(hx8389b 720hd)
{0x39, 0xB9,  3 ,{0xFF, 0x83, 0x94}},

{0x39, 0xBA,  16 ,{0x13, 0x82, 0x00, 0x16, 
				0xc5, 0x10, 0x10, 0xff, //lenovo.sw wangsx3 20130829 fix "white circle" issue
				0x0f, 0x24, 0x03, 0x21, 
				0x24, 0x25, 0x20, 0x08}},

{0x39, 0xC1,  127 ,{0x01, 0x00, 0x04, 0x0B, 
				0x16, 0x20, 0x28, 0x2f, 
				0x36, 0x3F, 0x47, 0x4F, 
				0x57, 0x5F, 0x67, 0x6F, 
				0x76, 0x7E, 0x86, 0x8E, 
				0x96, 0x9E, 0xA6, 0xAE, 
				0xB6, 0xBE, 0xC6, 0xCE, 
				0xD6, 0xDE, 0xE6, 0xEE, 
				0xF8, 0xFF, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 
				0x08, 0x10, 0x17, 0x21, 
				0x29, 0x30, 0x37, 0x40, 
				0x48, 0x51, 0x58, 0x60, 
				0x67, 0x70, 0x76, 0x7E, 
				0x86, 0x8E, 0x96, 0x9E, 
				0xA6, 0xAE, 0xB6, 0xBE, 
				0xC6, 0xCE, 0xD6, 0xDE, 
				0xE6, 0xEE, 0xF8, 0xFF, 
				0x00, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x08, 0x13, 
				0x18, 0x21, 0x29, 0x30, 
				0x37, 0x40, 0x48, 0x51, 
				0x59, 0x61, 0x69, 0x72, 
				0x79, 0x81, 0x89, 0x92, 
				0x9A, 0xA2, 0xAB, 0xB3, 
				0xBB, 0xC3, 0xCC, 0xD3, 
				0xDB, 0xE3, 0xEB, 0xF1, 
				0xF8, 0xFF, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x00}},
{REGFLAG_ESCAPE_ID,REGFLAG_DELAY_MS_V3,5,{}},


{0x39, 0xB1,  16 ,{0x01, 0x00, 0x37, 0x87, 
				0x01, 0x11, 0x11, 0x22, 
				0x2A, 0x3f, 0x3F, 0x57, 
				0x02, 0x00, 0xE6, 0xE2}},
{REGFLAG_ESCAPE_ID,REGFLAG_DELAY_MS_V3,10,{}},

{0x39, 0xB2,  12 ,{0x00, 0xC8, 0x16, 0x06, 
				0x00, 0x88, 0x00, 0xFF, 
				0x00, 0x00, 0x05, 0x11}},//lenovo.sw wangsx3 20130829 fix "white circle" issue
 {0x15, 0xC6,  1 ,{0x08}}, 
{0x39, 0xBF,  4 ,{0x06, 0x00, 0x10, 0x04}},

{0x39, 0xB4,  22 ,{0x80, 0x08, 0x32, 0x10, 
				0x0d, 0x32, 0x10, 0x08, 
				0x22, 0x10, 0x08, 0x37, 
				0x04, 0x4a, 0x11, 0x37, 
				0x04, 0x44, 0x06, 0x5A, 
				0x5a, 0x06}},




{0x39, 0xD5,  52 ,{0x00, 0x00, 0x00, 0x10, 
				0x0A, 0x00, 0x01, 0x33, 
				0x00, 0x00, 0x33, 0x00, 
				0x23, 0x01, 0x67, 0x45, 
				0x01, 0x23, 0x88, 0x88, 
				0x88, 0x88, 0x88, 0x88, 
				0x88, 0x88, 0x88, 0x88, 
				0x88, 0x45, 0x88, 0x99, 
				0x54, 0x76, 0x10, 0x32, 
				0x32, 0x10, 0x88, 0x88, 
				0x88, 0x88, 0x88, 0x88, 
				0x88, 0x88, 0x88, 0x88, 
				0x88, 0x54, 0x99, 0x88}},


{0x39, 0xC7,  4 ,{0x00, 0x10, 0x00, 0x10}},


{0x39, 0xE0,  42 ,{0x00, 0x0a, 0x14, 0x39, 
				0x3E, 0x3f, 0x24, 0x44, 
				0x04, 0x0B, 0x0f, 0x13, 
				0x14, 0x14, 0x15, 0x18, 
				0x1C, 0x00, 0x0a, 0x14, 
				0x39, 0x3E, 0x3f, 0x24, 
				0x44, 0x04, 0x0B, 0x0f, 
				0x13, 0x16, 0x14, 0x15, 
				0x18, 0x1C, 0x08, 0x17, 
				0x0A, 0x1A, 0x08, 0x17, 
				0x0A, 0x1A}},


{0x39, 0xC0,  2 ,{0x0c, 0x17}},

{0x15, 0xCC,  1 ,{0x09}},

{0x15, 0xD4,  1 ,{0x32}},

{0x15, 0xB6,  1 ,{0x20}},

{0x15, 0x35,  1 ,{0x00}},//lenovo.sw 20130719 wangsx3 add for te pin

{0x15,0xE6,1,{0x01}},//lenovo.sw 20130730 wangsx3 enable CE  
{0x15,0xE4,1,{0x11}},//lenovo.sw 20130730 wangsx3 enable CE

{0x15, 0x55,   1 ,{0x00}},//lenovo.sw wangsx3 20130730 disable cabc
//{0x05, 0x11,0,{}},
//{REGFLAG_ESCAPE_ID,REGFLAG_DELAY_MS_V3,120,{}},

//{0x05, 0x29,0,{}},
//{REGFLAG_ESCAPE_ID,REGFLAG_DELAY_MS_V3,20,{}},	  

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

/***lenovo.sw2 houdz 20131210 add :support "lcm inverse" and "lcm cabc" **start***/

static void lcm_setInverse(unsigned int on)
{
	unsigned int data_array[16];
	#if BUILD_LK
	printf("%s on=%d\n",__func__,on);
	#else
	printk("%s on=%d\n",__func__,on);
	#endif

	switch(on){
		case 0:
			data_array[0]=0x00200500; 
			dsi_set_cmdq(data_array, 1, 1);
			break;
		case 1:
			data_array[0]=0x00210500; 
			dsi_set_cmdq(data_array, 1, 1);
			break;
		default:
			break;
		}

	 MDELAY(10);
}



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

        #if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
        #else
		params->dsi.mode   = SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE; 
        #endif
	
		// DSI
		/* Command mode setting */
		//1 Three lane or Four lane
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Video mode setting		
		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		/*lenovo.sw 20130719 wangsx3 change mipi clock*/
		params->dsi.vertical_sync_active				= 5;// 3    2
		params->dsi.vertical_backporch					= 19;//19;//lenovo wangsx3 20130806 fixed issue:the window show incompletely
		params->dsi.vertical_frontporch					= 8; // 1  12
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 15;//18;// 50  2
		params->dsi.horizontal_backporch				= 75;//95;
		params->dsi.horizontal_frontporch				= 75;//95;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
		params->dsi.PLL_CLOCK=236;//240;    /*lenovo.sw 20130719 wangsx3 change mipi clock*/
		params->dsi.DA_HS_EXIT=21;    /*lenovo.sw 20130804 wangsx3 change mipi clock data_HS_exit*/
	    //params->dsi.LPX=8; 

/* Lenovo-sw2 houdz1 add, 20140321 begin */
		{
		unsigned int ui;
		unsigned int cycle_time;
		unsigned int hs_trail_m, hs_trail_n;
		ui= 1000/(params->dsi.PLL_CLOCK*2)+0x01;
		cycle_time=8000/(params->dsi.PLL_CLOCK*2)+0x01;

   		hs_trail_m=1;
   		hs_trail_n= (((hs_trail_m * 0x4) + 0x60-60)/cycle_time);
 		params->dsi.HS_TRAIL = ((hs_trail_m > hs_trail_n) ? hs_trail_m : hs_trail_n) ;
		}
/* Lenovo-sw2 houdz1 add, 20140321 end */
/* Lenovo-sw2 houdz1 add, 20140124 begin */
#ifdef LENOVO_BACKLIGHT_LIMIT
 		params->bl_app.min =1;
  		params->bl_app.def =102;
  		params->bl_app.max =255;
  		params->bl_bsp.min =7;
  		params->bl_bsp.def =102;
  		params->bl_bsp.max =255;
#endif
/* Lenovo-sw2 houdz1 add, 20140124 end */
#if 0
		// Bit rate calculation
		//1 Every lane speed
		params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
		params->dsi.fbk_div =13;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	
#endif
}
static DISABLE_LCM_ID=TRUE;
static void lcm_init(void)
{
    unsigned int data_array[16];
    SET_RESET_PIN(0);
    MDELAY(20); 
    SET_RESET_PIN(1);
    //MDELAY(20); 
    mt_set_gpio_out(GPIO_LCD_ENP_PIN,1);
    MDELAY(10);
    mt_set_gpio_out(GPIO_LCD_ENN_PIN,1);
    MDELAY(10);
    mt_set_gpio_out(GPIO_LCDBL_EN_PIN,1);
    dsi_set_cmdq_V3(lcm_initialization_setting,sizeof(lcm_initialization_setting)/sizeof(lcm_initialization_setting[0]),1);
    data_array[0]=0x00110500; // sleep out
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(150);
    data_array[0] = 0x00290500; // display on
    dsi_set_cmdq(data_array, 1, 1);
    if(DISABLE_LCM_ID)
    {
      mt_set_gpio_mode(GPIO92,GPIO_MODE_GPIO);
      mt_set_gpio_dir(GPIO92,GPIO_DIR_IN);
      mt_set_gpio_pull_enable(GPIO92,GPIO_PULL_DISABLE);
      DISABLE_LCM_ID=FALSE;
    }
}



static void lcm_suspend(void)
{
	unsigned int data_array[16];
#if 0 //lenovo.sw wangsx3 20130829 fix "white circle" issue
	data_array[0]=0x00280500; // Display Off
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);
#endif
	data_array[0] = 0x00100500; // Sleep In
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(150);

#if 0  //lenovo.sw wangsx3 20130826 speed up lcm_resume
	SET_RESET_PIN(1);	
	SET_RESET_PIN(0);
	MDELAY(1); // 1ms
	
	SET_RESET_PIN(1);
	MDELAY(120);      
#endif
}
static void lcm_suspend_power(void)
{
  //lenovo.sw wangsx3 20130918 pull down ENN/ENP after ULPS
#ifndef BUILD_LK
  mt_set_gpio_out(GPIO_LCD_ENN_PIN,0);
  MDELAY(15);
  mt_set_gpio_out(GPIO_LCD_ENP_PIN,0);
  mt_set_gpio_out(GPIO_LCDBL_EN_PIN,0);//lenovo.sw wangsx3 20130904 pull down EN_LCD_BL
#endif
}


static void lcm_resume(void)
{
    //lenovo.sw wangsx3 20130826 speed up lcm_resume
    //lcm_init();
#ifndef BUILD_LK
	mt_set_gpio_out(GPIO_LCD_ENP_PIN,1);
	MDELAY(15);
	mt_set_gpio_out(GPIO_LCD_ENN_PIN,1);
	mt_set_gpio_out(GPIO_LCDBL_EN_PIN,1);//lenovo.sw wangsx3 20130904 pull down EN_LCD_BL
#endif
    unsigned int data_array[16];
    data_array[0]=0x00110500; // sleep out
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(150);
    data_array[0] = 0x00290500; // display on
    dsi_set_cmdq(data_array, 1, 1);
    if(lcm_esd_check())
      lcm_init();
    #ifdef BUILD_LK
	  printf("[LK]---cmd---hx8394a_hd720_dsi_vdo_tianma----%s------\n",__func__);
    #else
	  printk("[KERNEL]---cmd---hx8394a_hd720_dsi_vdo_tianma----%s------\n",__func__);
    #endif	
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
#if 0  //"read lcm id "instead of "read GPIO"
  	unsigned int ret = 0;

	ret = mt_get_gpio_in(GPIO92);
#if defined(BUILD_LK)
	printf("%s, [jx]hx8394a GPIO92 = %d \n", __func__, ret);
#endif	

	return (ret == 0)?1:0; 
#else
	unsigned int ret = 0;
  	unsigned int id=0;
  	unsigned char buffer[4];
  	unsigned int array[16]; 

  	SET_RESET_PIN(1);
  	SET_RESET_PIN(0);
  	MDELAY(20);
  	SET_RESET_PIN(1);
  	MDELAY(20);

  	array[0]=0x00043902;
  	array[1]=0x8983FFB9;
  	dsi_set_cmdq(&array, 2, 1);

  	array[0]=0x00083902;
  	array[1]=0x009341BA;
  	array[2]=0x1800A416;
  	dsi_set_cmdq(&array, 3, 1);

  	array[0] = 0x00013700;
  	dsi_set_cmdq(array, 1, 1);
  
  	read_reg_v2(0xDB, buffer, 1);
  	id = buffer[0]; //we only need ID
  #ifdef BUILD_LK
    	printf("%s, LCM LK hx8394a debug: hx8394a id = 0x%x\n", __func__, id);
  #else
    	printk("%s, LCM kernel hx8394a  debug: hx8394a id = 0x%x\n", __func__, id);
  #endif
	return (0x94 == id)?1:0;
#endif

}


static unsigned int lcm_esd_check(void)
{
  #ifndef BUILD_LK
	char  buffer[3];
	int   array[4];

	if(lcm_esd_test)
	{
		lcm_esd_test = FALSE;
		return TRUE;
	}

	array[0] = 0x00013700;
	dsi_set_cmdq(array, 1, 1);
	//lenovo:wangsx3 20130725 checkin esd code for stella
	read_reg_v2(0x0A, buffer, 1);
	//printk(" hx8394a esd 0x0A,buffer0 =%x \n",buffer[0]);
	if(buffer[0]==0x1C)
	{
		return FALSE;
	}
	else
	{			 
		return TRUE;
	}
 #endif

}

static unsigned int lcm_esd_recover(void)
{
    lcm_init();
	//lcm_resume();

	return TRUE;
}

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

LCM_DRIVER hx8394a_hd720_dsi_vdo_tianma_lcm_drv = 
{
    .name			= "hx8394a_hd720_dsi_vdo_tianma",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.suspend_power  = lcm_suspend_power,//lenovo.sw wangsx3 20310918 add suspend_power API in LCM driver
	.compare_id     = lcm_compare_id,
	.esd_check = lcm_esd_check,//lenovo:wangsx3 20130725 checkin esd code for stella
	.esd_recover = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
.set_inversemode = lcm_set_inverse,
    };
