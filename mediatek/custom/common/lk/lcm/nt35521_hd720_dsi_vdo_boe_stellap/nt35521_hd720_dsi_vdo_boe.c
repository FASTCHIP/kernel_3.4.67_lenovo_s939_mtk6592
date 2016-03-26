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


                                                           //Page 0              
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x00  }},
{0x39, 0xFF, 4, {0xAA,0x55,0xA5,0x80}},

{0x15, 0x6F, 1, {0x07}},
{0x15, 0xF7, 1, {0x50}},
                             
{0x39, 0x6F, 2, {0x11,0x00}},
{0x39, 0xF7, 2, {0x20,0x00}},
{0x15, 0x6F, 1, {0x06}},
{0x15, 0xF7, 1, {0xA0}},
{0x15, 0x6F, 1, {0x19}},
{0x15, 0xF7, 1, {0x12}},
{0x15, 0x6F, 1, {0x02}},
{0x15, 0xF7, 1, {0x47}},

{0x39, 0xBD, 5, {0x01,0xA0,0x0C,0x08,0x01}},
{0x15, 0x6F, 1, {0x02 }},                       
{0x15, 0xB8, 1, {0x08 }},          
{0x39, 0xBB, 2, {0x74,0x44}},
{0x39, 0xBC, 2, {0x00,0x00}},
{0x15, 0xB6, 1, {0x01}},
 //                                        //Page 1
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x01}},     
                           
{0x39, 0xB0, 2, {0x09,0x09}},
{0x39, 0xB1, 2, {0x09,0x09}},	
{0x39, 0xBC, 2, {0x98,0x00}},
{0x39, 0xBD, 2, {0x98,0x00}},	        
{0x15, 0xCA, 1, {0x00     }},
{0x15, 0xC0, 1, {0x0C     }},       
{0x39, 0xB5, 2, {0x03,0x03}},
//{0x15, 0xBE, 1, {0x38     }},     //50->38 /8.6 //lenovo.sw wangsx3 20130816 update parameters
{0x39, 0xB3, 2, {0x19,0x19}}, 
{0x39, 0xB4, 2, {0x19,0x19}},       
{0x39, 0xB6, 2, {0x04,0x04}},//lenovo.sw wangsx3 20130903 fixed high temperature issue
                                    //ADD 8.29  
{0x39, 0xB7, 2, {0x04,0x04}},       
                                    //ADD 8.29
{0x39, 0xB9, 2, {0x26,0x26}},      
{0x39, 0xBA, 2, {0x24,0x24}},     
//                                        //Page 2
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x02  }},  
                                        //                      1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20                                        
{0x15, 0xEE, 1, {0x01}},
{0x39, 0xB0, 16, {0x00,0x24,0x00,0x3F,0x00,0x5A,0x00,0x6C,0x00,0x81,0x00,0xA5,0x00,0xC4,0x00,0xF9}},	
{0x39, 0xB1, 16, {0x01,0x24,0x01,0x6F,0x01,0xAB,0x02,0x0A,0x02,0x59,0x02,0x5C,0x02,0xA5,0x02,0xEE}},
{0x39, 0xB2, 16, {0x03,0x5F,0x03,0x78,0x03,0x7F,0x03,0xD0,0x03,0xE9,0x03,0xF8,0x03,0xFC,0x03,0xFE}},	        
{0x39, 0xB3, 4, {0x03,0xFF,0x03,0xFF}},
{0x15, 0x6F, 1, {0x17}},          
{0x15, 0xF4, 1, {0x70}},
{0x15, 0x6F, 1, {0x11}},           
{0x15, 0xF3, 1, {0x01}},
{0x15, 0x6F, 1, {0x01}},
{0x15, 0xF9, 1, {0x46}},          
//                                       //Page 6
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x06}},
                                           
{0x39, 0xB0, 2, {0x10,0x12}},
{0x39, 0xB1, 2, {0x14,0x16}},		
{0x39, 0xB2, 2, {0x00,0x02}},
{0x39, 0xB3, 2, {0x31,0x31}},	        
{0x39, 0xB4, 2, {0x31,0x34}},
{0x39, 0xB5, 2, {0x34,0x34}},         
{0x39, 0xB6, 2, {0x34,0x31}},
{0x39, 0xB7, 2, {0x31,0x31}},              
{0x39, 0xB8, 2, {0x31,0x31}}, 
{0x39, 0xB9, 2, {0x2D,0x2E}}, 
{0x39, 0xBA, 2, {0x2E,0x2D}},
{0x39, 0xBB, 2, {0x31,0x31}},
{0x39, 0xBC, 2, {0x31,0x31}},
{0x39, 0xBD, 2, {0x31,0x34}},
{0x39, 0xBE, 2, {0x34,0x34}},
{0x39, 0xBF, 2, {0x34,0x31}},   
{0x39, 0xC0, 2, {0x31,0x31}},
{0x39, 0xC1, 2, {0x03,0x01}},
{0x39, 0xC2, 2, {0x17,0x15}},
{0x39, 0xC3, 2, {0x13,0x11}},
{0x39, 0xE5, 2, {0x31,0x31}},
{0x39, 0xC4, 2, {0x17,0x15}},
{0x39, 0xC5, 2, {0x13,0x11}},
{0x39, 0xC6, 2, {0x03,0x01}},
{0x39, 0xC7, 2, {0x31,0x31}},
{0x39, 0xC8, 2, {0x31,0x34}},
{0x39, 0xC9, 2, {0x34,0x34}},
{0x39, 0xCA, 2, {0x34,0x31}},
{0x39, 0xCB, 2, {0x31,0x31}},
{0x39, 0xCC, 2, {0x31,0x31}},
{0x39, 0xCD, 2, {0x2E,0x2D}},
{0x39, 0xCE, 2, {0x2D,0x2E}},
{0x39, 0xCF, 2, {0x31,0x31}},
{0x39, 0xD0, 2, {0x31,0x31}},
{0x39, 0xD1, 2, {0x31,0x34}}, 
{0x39, 0xD2, 2, {0x34,0x34}},
{0x39, 0xD3, 2, {0x34,0x31}}, 
{0x39, 0xD4, 2, {0x31,0x31}},
{0x39, 0xD5, 2, {0x00,0x02}},
{0x39, 0xD6, 2, {0x10,0x12}},
{0x39, 0xD7, 2, {0x14,0x16}},
{0x39, 0xE6, 2, {0x32,0x32}},
{0x39, 0xD8, 5, {0x00,0x00,0x00,0x00,0x00}},
{0x39, 0xD9, 5, {0x00,0x00,0x00,0x00,0x00}},
{0x15, 0xE7, 1, {0x00}},
//                                      //Page5
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x05}},

{0x15, 0xED, 1, {0x30}},
//                                      //Page3
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x03}},

{0x39, 0xB0, 2, {0x20,0x00 }},
{0x39, 0xB1, 2, {0x20,0x00 }},
//                                      //Page5_1
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x05 }},

{0x39, 0xB0, 2, {0x17,0x06}},
{0x15, 0xB8, 1, {0x00}},
{0x39, 0xBD, 5, {0x03,0x03,0x00,0x00,0x03}},
{0x39, 0xB1, 2, {0x17,0x06}},
{0x39, 0xB9, 2, {0x00,0x03}},
{0x39, 0xB2, 2, {0x17,0x06}},   
{0x39, 0xBA, 2, {0x00,0x00}},
{0x39, 0xB3, 2, {0x17,0x06}},
{0x39, 0xBB, 2, {0x00,0x00}},
{0x39, 0xB4, 2, {0x17,0x06}},
{0x39, 0xB5, 2, {0x17,0x06}},
{0x39, 0xB6, 2, {0x17,0x06}},
{0x39, 0xB7, 2, {0x17,0x06}},
{0x39, 0xBC, 2, {0x00,0x03}},
{0x15, 0xE5, 1, {0x06 }},
{0x15, 0xE6, 1, {0x06 }}, 
{0x15, 0xE7, 1, {0x06 }},
{0x15, 0xE8, 1, {0x06 }},
{0x15, 0xE9, 1, {0x06 }},
{0x15, 0xEA, 1, {0x06 }},
{0x15, 0xEB, 1, {0x06 }}, 
{0x15, 0xEC, 1, {0x06 }},
                                        //Page5_2               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x05 }},

{0x15, 0xC0, 1, {0x0D }},
{0x15, 0xC1, 1, {0x0B }},
{0x15, 0xC2, 1, {0x23 }},
{0x15, 0xC3, 1, {0x40 }},
                                        //Page3_1               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x03}},
        
{0x39, 0xB2, 5, {0x05,0x00,0x0A,0x00,0x00}},
{0x39, 0xB3, 5, {0x05,0x00,0x0A,0x00,0x00}},              
{0x39, 0xB4, 5, {0x05,0x00,0x0A,0x00,0x00}},  
{0x39, 0xB5, 5, {0x05,0x00,0x0A,0x00,0x00}},  
                                        //Page5_3               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x05}},

{0x15, 0xC4, 1, {0x84}},
{0x15, 0xC5, 1, {0x82}},
{0x15, 0xC6, 1, {0x82}},
{0x15, 0xC7, 1, {0x80}},  
                                        //Page3_2               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x03}},
     
{0x39, 0xB6, 5, {0x02,0x00,0x0A,0x00,0x00}},
{0x39, 0xB7, 5, {0x02,0x00,0x0A,0x00,0x00}},          
{0x39, 0xB8, 5, {0x02,0x00,0x0A,0x00,0x00}},
{0x39, 0xB9, 5, {0x02,0x00,0x0A,0x00,0x00}},
										//Page5_4               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x05}},

{0x39, 0xC8, 2, {0x0B,0x30}},
{0x39, 0xC9, 2, {0x05,0x10}},
{0x39, 0xCA, 2, {0x01,0x10}},
{0x39, 0xCB, 2, {0x01,0x10}},           
                                        //Page3_3               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x03}},	
{0x39, 0xBA, 5, {0x53,0x00,0x0A,0x00,0x00}},
{0x39, 0xBB, 5, {0x53,0x00,0x0A,0x00,0x00}},             
{0x39, 0xBC, 5, {0x53,0x00,0x0A,0x00,0x00}}, 
{0x39, 0xBD, 5, {0x53,0x00,0x0A,0x00,0x00}}, 
											//Page5_5               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x05}},
{0x15, 0xC0, 1, {0x0D}},//lenovo.sw wangsx3 20130903 fixed high temperature issue
                          //ADD 8.29
{0x15, 0xC1, 1, {0x0B}},
                          //ADD 8.29
{0x39, 0xD1, 5, {0x03,0x05,0x05,0x07,0x00}},
{0x39, 0xD2, 5, {0x03,0x05,0x09,0x03,0x00}},
{0x39, 0xD3, 5, {0x00,0x00,0x6A,0x07,0x10}},
{0x39, 0xD4, 5, {0x30,0x00,0x6A,0x07,0x10}},  
											//Page3_4               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x03}},

{0x15, 0xC4, 1, {0x60}},
{0x15, 0xC5, 1, {0x40}},                
{0x15, 0xC6, 1, {0x64}}, 
{0x15, 0xC7, 1, {0x44}},                           
                                        //Page0_1               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x00}},
{0x39, 0xBD, 5, {0x01,0xA0,0x10,0x10,0x01}},
											//Page0_2               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x00}},

{0x15, 0xB6, 1, {0x08 }},//lenovo.sw wangsx3 20130903 fixed high temperature issue
                         //8.29 UPDATE 03->08,source hold time
//{0x39, 0xB8, 5, {0x00,0x00,0x00,0x00}},
                                        // B8h disable 8.29
                                        //Page1_1              
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x01}},
//59->38 /8.6  //lenovo.sw wangsx3 20130816 update parameters
//{0x15, 0xBE, 1, {0x38 }},
                                        //Page3_5               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x03}},

{0x39, 0xB0, 2, {0x00,0x00 }},
{0x39, 0xB1, 2, {0x00,0x00 }},
                                        //Page0_3               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x00}},
                                         
{0x39, 0xBB, 2, {0x33,0x33}},//lenovo.sw wangsx3 20130903 fixed high temperature issue
                                       //11,11 -> 33,33 /8.29
{0x39, 0xB8, 4, {0x01,0x00,0x00,0x00}},
                                       //0x02,0x0C,0x02->0x00,0x00,0x00 /0829
{0x39, 0xBC, 2, {0x00,0x00}},
                                       //ADD BCh /0829    
                                        //Page2_1               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x02}},
//UP DATE 8.6 //lenovo.sw wangsx3 20130812 update parameters
{0x15, 0xEE, 1, {0x01}},
{0x39, 0xB0, 16, {0x00,0x20,0x00,0x39,0x00,0x65,0x00,0x84,0x00,0x9E,0x00,0xC7,0x00,0xE7,0x01,0x19 }},
{0x39, 0xB1, 16, {0x01,0x43,0x01,0x81,0x01,0xB3,0x01,0xFD,0x02,0x38,0x02,0x3A,0x02,0x6D,0x02,0xA4 }},
{0x39, 0xB2, 16, {0x02,0xC7,0x02,0xFA,0x03,0x1D,0x03,0x4D,0x03,0x6D,0x03,0x93,0x03,0xB1,0x03,0xD9 }},
{0x39, 0xB3, 4, {0x03,0xFB,0x03,0xFF }},      
                                        //Page1_2              
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x01}},

{0x15, 0xC0, 1, {0x04}},
                                        //Page0_4               
{0x39, 0xF0, 5, {0x55,0xAA,0x52,0x08,0x00}},

{0x15, 0xC8, 1, {0x00}}, //lenovo.sw wangsx3 20130823 donot clear lcm without H-sync
{0x15, 0x6F, 1, {0x06}},                                          
{0x15, 0xF7, 1, {0xA0}},
{0x15, 0x6F, 1, {0x19}},                                         
{0x15, 0xF7, 1, {0x12}},
{0x15, 0x51, 1, {0xFF}},
{0x15, 0x53, 1, {0x2C}},
{0x15, 0x55, 1, {0x00}}, //lenovo.sw wangsx3 20130816 update params,disable CABC

 
//{0x39, 0xf0, 5, {0x55,0xaa,0x52,0x08,0x00}},
//{0x39, 0xee, 4, {0x87, 0x78, 0x02, 0x40}},
//{0x39, 0xef, 2, {0x07, 0xff}},
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
		params->dsi.LANE_NUM				= LCM_THREE_LANE; //LCM_FOUR_LANE;  /*lenovo.sw2 20131230 houdz1 :fix bug "radio frequency interference(RFI)"*/  
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Video mode setting		
		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		
		params->dsi.vertical_sync_active				= 0x05;// 3    2
		params->dsi.vertical_backporch					= 0x0d;// 20   1
		params->dsi.vertical_frontporch					= 0x08; // 1  12
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 0x12;// 50  2
		params->dsi.horizontal_backporch				= 130;//120;//0x5f;/*lenovo.sw2 20131230 houdz1 :fix bug "radio frequency interference(RFI)"*/  
		params->dsi.horizontal_frontporch				= 130;//120;//0x5f;/*lenovo.sw2 20131230 houdz1 :fix bug "radio frequency interference(RFI)"*/  
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
		params->dsi.PLL_CLOCK=334;//230;  /*lenovo.sw2 20131230 houdz1 :fix bug "radio frequency interference(RFI)"*/     
		params->dsi.DA_HS_EXIT=21;    /*lenovo.sw 20130804 wangsx3 change mipi clock data_HS_exit*/
		params->dsi.ssc_disable=1;
	    //params->dsi.LPX=8; 
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

static int inverse_mode =0;
static void lcm_init(void)
{
    unsigned int data_array[16];
	//lenovo.sw wangsx3 20130903 power on lcm
    mt_set_gpio_out(GPIO_LCD_ENP_PIN,1);
    MDELAY(15);
    mt_set_gpio_out(GPIO_LCD_ENN_PIN,1);
    mt_set_gpio_out(GPIO_LCDBL_EN_PIN,1);//lenovo.sw wangsx3 20130904 pull down EN_LCD_BL
    SET_RESET_PIN(0);
    MDELAY(20); 
    SET_RESET_PIN(1);
    MDELAY(20); 
    dsi_set_cmdq_V3(lcm_initialization_setting,sizeof(lcm_initialization_setting)/sizeof(lcm_initialization_setting[0]),1);
    if(inverse_mode)
    {
	data_array[0]=0x00210500; 
	dsi_set_cmdq(data_array, 1, 1);
    }
#ifdef LENOVO_LCD_BACKLIGHT_CONTROL_BY_LCM
    data_array[0]= 0x00023902;
    data_array[1] =(0x51|(0xff<<8)); //brightness
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0]= 0x00023902;
    data_array[1] =(0x53|(0x24<<8)); 
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0]= 0x00033902;
    data_array[1] =(0xc9|(0x0f<<8)|(0x00<<16)); //freq = 9M/255/PWM_PERIOD
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0]= 0x00023902;
    data_array[1] =(0x5E|(0x0A<<8));  //mini duty
    dsi_set_cmdq(data_array, 2, 1);
    data_array[0]= 0x00023902;
    data_array[1] =(0x55|(0x01<<8));   //ui mode
    dsi_set_cmdq(data_array, 2, 1);
    MDELAY(5);
#endif
    #ifdef BUILD_LK
	  printf("[LK]---LCM---nt35521----%s------\n",__func__);
    #else
	  printk("[KERNEL]---LCM---nt35521----%s------\n",__func__);
    #endif
}



static void lcm_suspend(void)
{
	unsigned int data_array[16];
	//lenovo.sw wangs3 20130823 clear lcm before suspend
	data_array[0]=0x00063902;//PAGE0
	data_array[1]=0x52AA55F0;
	data_array[2]=0x00000008;
	dsi_set_cmdq(&data_array,3,1);

	data_array[0]=0x80C81500;
	dsi_set_cmdq(&data_array,1,1);

       MDELAY(200);

	data_array[0]=0x00280500; // Display Off
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);
	data_array[0] = 0x00100500; // Sleep In
	dsi_set_cmdq(data_array, 1, 1);


#if 0 //lenovo.sw wangsx3 20130826 speed up lcm_resume
       SET_RESET_PIN(1);
       MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(120);      
#endif
}



static void lcm_suspend_power(void)
{
  //lenovo.sw wangsx3 20130918 pull down ENN/ENP after ULPS
#ifndef BUILD_LK
 printk("[KERNEL]---enter %s------\n",__func__);
  MDELAY(15);
  mt_set_gpio_out(GPIO_LCD_ENN_PIN,0);
  MDELAY(15);
  mt_set_gpio_out(GPIO_LCD_ENP_PIN,0);
  mt_set_gpio_out(GPIO_LCDBL_EN_PIN,0);//lenovo.sw wangsx3 20130904 pull down EN_LCD_BL
#endif
}


static void lcm_resume(void)
{
#if 0
	//lcm_init();
	unsigned int data_array[16];
	//lenovo.sw wangsx3 20130826 speed up lcm_resume exit sleep mode,remove lcm_init in lcm_resume
	data_array[0]=0x00063902;//PAGE0
	data_array[1]=0x52AA55F0;
	data_array[2]=0x00000008;
	dsi_set_cmdq(&data_array,3,1);

        data_array[0]=0x00C81500;//compare with lcm_suspend
	dsi_set_cmdq(&data_array,1,1);

	data_array[0]=0x00110500; // sleep out
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120);
	data_array[0] = 0x00290500; // display on
	dsi_set_cmdq(data_array, 1, 1);
#endif
	//if(lcm_esd_check())
	  lcm_init();
    #ifdef BUILD_LK
	  printf("[LK]---LCM---nt35521----%s------\n",__func__);
    #else
	  printk("[KERNEL]---LCM---nt35521----%s------\n",__func__);
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
#if 0
  	unsigned int ret = 0;
      mt_set_gpio_mode(GPIO92|0x80000000,GPIO_MODE_GPIO);
      mt_set_gpio_dir(GPIO92|0x80000000,GPIO_DIR_IN);
      mt_set_gpio_pull_enable(GPIO92|0x80000000,GPIO_PULL_DISABLE);
	MDELAY(1);
	ret = mt_get_gpio_in(GPIO92|0x80000000);
#if defined(BUILD_LK)
	printf("%s, [jx]nt35521 GPIO92 = %d \n", __func__, ret);
#endif	

	return (ret == 1)?1:0; 
#else
  	unsigned int id=0;
  	unsigned char buffer[4];
  	unsigned int array[16];  

	mt_set_gpio_out(GPIO_LCD_ENP_PIN,1);
	MDELAY(15);
	mt_set_gpio_out(GPIO_LCD_ENN_PIN,1);
	

  	SET_RESET_PIN(1);
  	SET_RESET_PIN(0);
  	MDELAY(20);
  	SET_RESET_PIN(1);
  	MDELAY(20);

	
  	array[0]=0x00063902;
  	array[1]=0x52aa55f0;
 	 array[2]=0x00000108;
  	dsi_set_cmdq(array, 3, 1);
  	MDELAY(10);

  	array[0] = 0x00083700;
  	dsi_set_cmdq(array, 1, 1);
  
  	read_reg_v2(0xc5, buffer,2);
  	id = buffer[0]<<8 |buffer[1]; 

 
  	#ifdef BUILD_LK
   	 printf("[LK]LCM %s nt35512 id = %x \n",__func__,id);
  	#else
    	 printk("[KERNEL] LCM %s nt35512 id = %x\n",__func__,id);
  	#endif
  	return (0x5521 == id)?1:0;

#endif 


}

static unsigned int lcm_esd_check(void)
{
  //lenovo.sw wangsx3 20130816 checkin lcm esd code
  #ifndef BUILD_LK
  char  buffer[4]={0};
  int   array[4];

  if(lcm_esd_test)
  {
    lcm_esd_test = FALSE;
    return TRUE;
  }

  array[0] = 0x00013700;
  dsi_set_cmdq(array, 1, 1);

  read_reg_v2(0x0A, buffer, 1);
  printk("nt35521 boe esd 0x0A,buffer0=%x\n",buffer[0]);
  if(buffer[0]==0x9C)
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


/***lenovo.sw2 houdz 20140124  add :support "lcm inverse" and "lcm cabc" **start***/

static void lcm_setInverse(unsigned int on)
{
	unsigned int data_array[16];
#if 0
	#if BUILD_LK
	printf("%s on=%d\n",__func__,on);
	#else
	printk("%s on=%d\n",__func__,on);
	#endif
#endif

	switch(on){
		case 0:
			data_array[0]=0x00200500; 
			dsi_set_cmdq(data_array, 1, 1);
			inverse_mode =0;
			break;
		case 1:
			data_array[0]=0x00210500; 
			dsi_set_cmdq(data_array, 1, 1);
			inverse_mode =1;
			break;
		default:
			break;
		}

	 MDELAY(10);
}

static unsigned int lcm_cabcmode_index = 3;
static void lcm_setcabcmode(unsigned int mode)
{
	unsigned int data_array[16];
	#if BUILD_LK
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
			lcm_cabcmode_index=3;
			break;
		default:
			break;
	}
	data_array[0]= 0x00023902;
    	data_array[1] =(0x55|(lcm_cabcmode_index<<8));
   	dsi_set_cmdq(data_array, 2, 1);

	 MDELAY(10);
}
static void lcm_getcabcstate(unsigned int * state)
{
	if(lcm_cabcmode_index == 0){
		*state = 0;
	}else if(lcm_cabcmode_index == 1){
		*state = 1;
	}else{
		*state = 2;
	}
	return;
}

/***lenovo.sw2 houdz 20140124 add :support "lcm inverse" and "lcm cabc" **end***/





static void lcm_setbacklight(unsigned int level)
{
	unsigned int data_array[16];
#if defined(BUILD_LK)
	printf("%s, %d\n", __func__, level);
#elif defined(BUILD_UBOOT)
    printf("%s, %d\n", __func__, level);
#else
    printk("lcm_setbacklight = %d\n", level);
#endif

	if(level > 255) 
	    level = 255;

	data_array[0]= 0x00023902;
	data_array[1] =(0x51|(level<<8));
	dsi_set_cmdq(data_array, 2, 1);

}

LCM_DRIVER nt35521_hd720_dsi_vdo_boe_lcm_drv_stellap = 
{
    .name			= "nt35521_hd720_dsi_vdo_boe",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.suspend_power  = lcm_suspend_power,//lenovo.sw wangsx3 20310918 add suspend_power API in LCM driver
	.compare_id     = lcm_compare_id,
	.esd_check = lcm_esd_check,
	.esd_recover = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif

	.set_inversemode = lcm_setInverse, //lenovo.sw2 houdz 20131210 add :support lcm inverse

#ifdef  LENOVO_LCD_BACKLIGHT_CONTROL_BY_LCM
	.set_backlight	= lcm_setbacklight,
	.set_cabcmode = lcm_setcabcmode,//lenovo.sw2 houdz 20131210 add :support lcm cabc
	.get_cabcmode = lcm_getcabcstate,//lenovo.sw2 houdz 20131210 add :support lcm cabc
#endif
    };
