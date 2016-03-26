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

#define LCM_DSI_CMD_MODE			0

#define LCM_ID       (0x80)

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
#define hx8392b_CONFIG_PROC_FILE     "lcm_config"
static struct proc_dir_entry *hx8392b_config_proc = NULL;
#endif
#endif
/* Lenovo-sw yexm1 add, 20130819 end */ 
       
static unsigned int lcm_compare_id(void);
static unsigned int lcm_check_status(void);
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
		// enable tearing-free
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
#endif

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
		//params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 0;

		//params->dsi.word_count=720*3;	//DSI CMD mode need set these two bellow params, different to 6577
		//params->dsi.vertical_active_line=1280;

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		/* Lenovo-sw yexm1 modify, disable the LVDS_PLL lane, 20130922 */
		params->dsi.pll_select=0;	//0: MIPI_PLL; 1: LVDS_PLL
#if 1
		params->dsi.vertical_sync_active				= 2;//2
		params->dsi.vertical_backporch					= 18;//14
		params->dsi.vertical_frontporch					= 20;//16
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 76;//2
		params->dsi.horizontal_backporch				= 77;//34
		params->dsi.horizontal_frontporch				= 76;//24
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
#endif

		params->dsi.PLL_CLOCK=245;

}


static void lcm_init_register(void)
{
	unsigned int data_array[16];

data_array[0] = 0x00043902;
data_array[1] = 0x9283FFB9;//B9
dsi_set_cmdq(&data_array, 2, 1);
	

data_array[0] = 0x00033902;
data_array[1] = 0x000343BA;//BA
//data_array[2] = 0x1040DA16;
//data_array[3] = 0x00380FFF;
dsi_set_cmdq(&data_array, 2, 1);


data_array[0] = 0x00163902;
data_array[1] = 0x525245B1;//B1
data_array[2] = 0x55313B14;
data_array[3] = 0x27FBED11;
data_array[4] = 0xF0430458;
data_array[5] = 0xC0C0C0C0;
data_array[6] = 0x00000200;
dsi_set_cmdq(&data_array, 7, 1);
	
data_array[0] = 0x00133902;
data_array[1] = 0xC80000B2;//B2
data_array[2] = 0x1A721212;
data_array[3] = 0x1A1A1212;
data_array[4] = 0x04200102;
data_array[5] = 0x00489513;
dsi_set_cmdq(&data_array, 6, 1);

	
data_array[0] = 0x00293902;
data_array[1] = 0x00FF00B4;//B4
data_array[2] = 0x043B043F;
data_array[3] = 0x0200023B;
data_array[4] = 0x020E0404;
data_array[5] = 0x00010402;
data_array[6] = 0X3F003A04;
data_array[7] = 0x3B043B04;
data_array[8] = 0x04020002;
data_array[9] = 0x02020E00;
data_array[10] = 0x04000104;
data_array[11] = 0x0000003A;
dsi_set_cmdq(&data_array, 12, 1);	
	

data_array[0] = 0x08CC1500;	//CC
dsi_set_cmdq(&data_array, 1, 1);	
	
data_array[0] = 0x00283902;
data_array[1] = 0x010000D3;//D3
data_array[2] = 0x28000000;
data_array[3] = 0x12103208;
data_array[4] = 0x13321200;
data_array[5] = 0x320000C0;
data_array[6] = 0X00000810;
data_array[7] = 0x03121230;
data_array[8] = 0x00270004;
data_array[9] = 0x008A0000;
data_array[10] = 0x15051201;
//data_array[11] = 0x00000000;
dsi_set_cmdq(&data_array, 11, 1);
	
data_array[0] = 0x00383902;
data_array[1] = 0x010018D5;//D5
data_array[2] = 0x03021818;
data_array[3] = 0x18181818;
data_array[4] = 0x18181818;
data_array[5] = 0x18181818;
data_array[6] = 0X18181818;
data_array[7] = 0x18181818;
data_array[8] = 0x18181818;
data_array[9] = 0x18181818;
data_array[10] = 0x201E1E18;
data_array[11] = 0x31181820;
data_array[12] = 0X2F303031;
data_array[13] = 0x3332322F;
data_array[14] = 0x00343433;
dsi_set_cmdq(&data_array, 15, 1);

data_array[0] = 0x00383902;
data_array[1] = 0x020318D6;//D6
data_array[2] = 0x00011818;
data_array[3] = 0x18181818;
data_array[4] = 0x18181818;
data_array[5] = 0x18181818;
data_array[6] = 0X18181818;
data_array[7] = 0x18181818;
data_array[8] = 0x18181818;
data_array[9] = 0x18181818;
data_array[10] = 0x20181818;
data_array[11] = 0x31181820;
data_array[12] = 0X2F303031;
data_array[13] = 0x3332322F;
data_array[14] = 0x00343433;
dsi_set_cmdq(&data_array, 15, 1);



data_array[0] = 0x00BD1500;	
dsi_set_cmdq(&data_array, 1, 1);
	
data_array[0] = 0x001D3902;
data_array[1] = 0x008822D8;//D8
data_array[2] = 0xFCEA0200;
data_array[3] = 0x00008888;
data_array[4] = 0x22FCEA02;
data_array[5] = 0x02000088;
data_array[6] = 0X8888FCAA;
data_array[7] = 0xAA020000;
data_array[8] = 0x000000FC;
dsi_set_cmdq(&data_array, 9, 1);

data_array[0] = 0x01BD1500;	
dsi_set_cmdq(&data_array, 1, 1);
	
data_array[0] = 0x002B3902;
data_array[1] = 0x00CC22D8;//D8
data_array[2] = 0xFCAA0200;
data_array[3] = 0x0000CC88;
data_array[4] = 0x22FCAA02;
data_array[5] = 0x020000CC;
data_array[6] = 0XCC88FCAA;
data_array[7] = 0xAA020000;
data_array[8] = 0x00CC22FC;
data_array[9] = 0xFCAA0200;
data_array[10] = 0x0000CC88;
data_array[11] = 0x00FCAA02;
dsi_set_cmdq(&data_array, 12, 1);

data_array[0] = 0x02BD1500;	
dsi_set_cmdq(&data_array, 1, 1);

data_array[0] = 0x000A3902;
data_array[1] = 0x00CC00D8;//D8
data_array[2] = 0x00000000;
data_array[3] = 0x0000CC00;
dsi_set_cmdq(&data_array, 4, 1);

data_array[0] = 0x00BD1500;	
dsi_set_cmdq(&data_array, 1, 1);

//data_array[0] = 0x00033902;
//data_array[1] = 0x000610C7;//C7
//dsi_set_cmdq(&data_array, 2, 1);

data_array[0] = 0x002B3902;
data_array[1] = 0x160E00E0;//E0
data_array[2] = 0x283F2828;
data_array[3] = 0x0F0D0841;
data_array[4] = 0x15141613;
data_array[5] = 0x0E001A13;
data_array[6] = 0X3F282816;
data_array[7] = 0x0D084128;
data_array[8] = 0x1416130F;
data_array[9] = 0x0D1A1315;
data_array[10] = 0x0D140918;
data_array[11] = 0x00140918;
dsi_set_cmdq(&data_array, 12, 1);

data_array[0] = 0x00043902;
data_array[1] = 0x006E6EB6;//B6
dsi_set_cmdq(&data_array, 2, 1);


data_array[0] = 0x00110500;
dsi_set_cmdq(&data_array, 1, 1);
MDELAY(200);
data_array[0] = 0x00290500;
dsi_set_cmdq(&data_array, 1, 1);               
//not open CABC                
//data_array[0] = 0x00023902;
//data_array[1] = 0x0000FF51;
//dsi_set_cmdq(&data_array, 2, 1);
//data_array[0] = 0x00023902;
//data_array[1] = 0x00002C53;
//dsi_set_cmdq(&data_array, 2, 1);
//data_array[0] = 0x00023902;
//data_array[1] = 0x00000055;
//dsi_set_cmdq(&data_array, 2, 1);

} 

static void lcm_init(void)
{	
#if defined(BUILD_LK) || defined(BUILD_UBOOT)
	printf("%s, hx8392b yassy lcm_resume \n", __func__);
#else
	printk("%s, hx8392b yassy lcm_resume \n", __func__);
#endif	

	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	/*Lenovo-sw yexm1 modify 20130923 */
	MDELAY(120);

	lcm_init_register();
}


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
//begin lenovo jixu add for power consume 20130206
		SET_RESET_PIN(0);
//end lenovo jixu add for power consume 20130206
}


static void lcm_resume(void)
{
	int i;
#if defined(BUILD_LK) || defined(BUILD_UBOOT)
	printf("%s, hx8392b yassy lcm_resume \n", __func__);
#else
	printk("%s, hx8392b yassy lcm_resume \n", __func__);
#endif	
#if 1
	SET_RESET_PIN(1);
	MDELAY(5);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	/*Lenovo-sw yexm1 modify 20130923 */
	MDELAY(120);

	lcm_init_register();

#else
	data_array[0] = 0x00110500; 			   
	dsi_set_cmdq(&data_array, 1, 1); 
	MDELAY(150); 
	
	data_array[0] = 0x00290500; 			   
	dsi_set_cmdq(&data_array, 1, 1);	
	MDELAY(20); 
#endif
}

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
	read_reg_v2(0x0A, buffer,2);
	//#ifndef BUILD_LK
	//printk("[JX] %s 0x0A 0=0x%x 1=0x%x \n",__func__,buffer[0],buffer[1]);
	//#endif
	ret = buffer[0]==0x9C?0:1;
	//#ifndef BUILD_LK
	//printk("[JX] %s ret=%d \n",__func__,ret);
	//#endif
	if(ret) return TRUE;
	else
	{
		#ifndef BUILD_LK
		printk("[JX] %s 0x0A 0=0x%x 1=0x%x \n",__func__,buffer[0],buffer[1]);
		#endif
		return FALSE;
	}
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
	else return FALSE;
#endif
#endif
}

static unsigned int lcm_esd_recover(void)
{
#if 0
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
  	unsigned int id=0;
  	unsigned char buffer[4];
  	unsigned int array[16];

  	mt_set_gpio_mode(GPIO92|0x80000000,GPIO_MODE_GPIO);
 	mt_set_gpio_dir(GPIO92|0x80000000,GPIO_DIR_IN);
  	mt_set_gpio_pull_enable(GPIO92|0x80000000,GPIO_PULL_DISABLE);
	MDELAY(1);
#if 0
	ret = mt_get_gpio_in(GPIO92|0x80000000);
#if defined(BUILD_LK)
	printf("%s, [yexm1]hx8392b GPIO92 = %d \n", __func__, ret);
#endif	

	return (ret == 0)?1:0;
#endif
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
    	printf("%s, LCM LK hx8392b debug: hx8392b id = 0x%x\n", __func__, id);
  #else
    	printk("%s, LCM kernel hx8392b  debug: hx8392b id = 0x%x\n", __func__, id);
  #endif
	return (0x92 == id)?1:0;
}

/* Lenovo-sw yexm1 add, 20130819 begin */
#if defined(LENOVO_ID_READ)
#ifdef BUILD_LK
#else
extern LCM_DRIVER hx8392b_hd720_dsi_vdo_drv;
static int hx8392b_config_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    char *ptr = page;

    ptr += sprintf(ptr, "%s\n", hx8392b_hd720_dsi_vdo_drv.name);
	ptr += sprintf(ptr, "%s\n", hx8392b_hd720_dsi_vdo_drv.module);

    *eof = 1;
    return (ptr - page);
}

static void lcm_read_id(void)
{
	// Create proc file system
	hx8392b_config_proc = create_proc_entry(hx8392b_CONFIG_PROC_FILE, 0664, NULL);

	if (hx8392b_config_proc == NULL)
	{
	    return;
	}
	else
	{
	    hx8392b_config_proc->read_proc = hx8392b_config_read_proc;
	}
}
#endif
#endif
/* Lenovo-sw yexm1 add, 20130819 end */

LCM_DRIVER hx8392b_hd720_dsi_vdo_drv = 
{
	.name = "hx8392b",
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
	.module			= "yassy",
	.read_id   		= lcm_read_id,
	#endif
	#endif
	/* Lenovo-sw yexm1 add, 20130819 end */
};
