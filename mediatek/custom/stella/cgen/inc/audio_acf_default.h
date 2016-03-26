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

/*******************************************************************************
 *
 * Filename:
 * ---------
 * audio_acf_default.h
 *
 * Project:
 * --------
 *   ALPS
 *
 * Description:
 * ------------
 * This file is the header of audio customization related parameters or definition.
 *
 * Author:
 * -------
 * Tina Tsai
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:$
 * $Modtime:$
 * $Log:$
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef AUDIO_ACF_DEFAULT_H
#define AUDIO_ACF_DEFAULT_H

   /* Compensation Filter HSF coeffs: default all pass filter       */
    /* BesLoudness also uses this coeffs    */ 
#define BES_LOUDNESS_HSF_COEFF \
0x7db51a8,   0xf0495caf,   0x7db51a8,   0x7db2c248,   0x0,     \
0x7d81ea6,   0xf04fc2b4,   0x7d81ea6,   0x7d7ec27a,   0x0,     \
0x7c9530f,   0xf06d59e1,   0x7c9530f,   0x7c8ec364,   0x0,     \
0x7b78e02,   0xf090e3fd,   0x7b78e02,   0x7b6dc47b,   0x0,     \
0x7b15264,   0xf09d5b38,   0x7b15264,   0x7b07c4dc,   0x0,     \
0x794ac35,   0xf0d6a796,   0x794ac35,   0x7930c69a,   0x0,     \
0x772a3e5,   0xf11ab836,   0x772a3e5,   0x76fbc8a7,   0x0,     \
0x766cc90,   0xf13266e0,   0x766cc90,   0x7635c95c,   0x0,     \
0x7310072,   0xf19dff1b,   0x7310072,   0x72aacc8a,   0x0,     \
    \
0x7f087fe,   0xf01ef003,   0x7f087fe,   0x7f05c0f4,   0x0,     \
0x7ef27db,   0xf021b049,   0x7ef27db,   0x7eefc10a,   0x0,     \
0x7e8bd10,   0xf02e85e1,   0x7e8bd10,   0x7e85c16d,   0x0,     \
0x7e0e896,   0xf03e2ed4,   0x7e0e896,   0x7e02c1e5,   0x0,     \
0x7de214b,   0xf043bd6b,   0x7de214b,   0x7dd3c20f,   0x0,     \
0x7d1243a,   0xf05db78c,   0x7d1243a,   0x7cf6c2d2,   0x0,     \
0x7c13d5c,   0xf07d8548,   0x7c13d5c,   0x7be3c3bb,   0x0,     \
0x7bb9584,   0xf088d4f7,   0x7bb9584,   0x7b7fc40d,   0x0,     \
0x7a111c7,   0xf0bddc72,   0x7a111c7,   0x79a5c582,   0x0

#define BES_LOUDNESS_BPF_COEFF \
0x3fa082bb,   0x3de77d44,   0xc2770000,     \
0x3f9882ff,   0x3db97d00,   0xc2ad0000,     \
0x3f728443,   0x3ce37bbc,   0xc3a90000,     \
0x3f4585e4,   0x3be37a1b,   0xc4d60000,     \
0x3f35867e,   0x3b8a7981,   0xc53f0000,     \
0x3eed8973,   0x39f1768c,   0xc7200000,     \
    \
0x3f838421,   0x3d377bde,   0xc3440000,     \
0x3f798492,   0x3cfa7b6d,   0xc38c0000,     \
0x3f4886c2,   0x3be0793d,   0xc4d60000,     \
0x3f0d89bd,   0x3a927642,   0xc65f0000,     \
0x3ef98ae0,   0x3a1d751f,   0xc6e80000,     \
0x3e9d90b0,   0x380b6f4f,   0xc9560000,     \
    \
0x3f678623,   0x3c8979dc,   0xc40e0000,     \
0x3f5a86dc,   0x3c3d7923,   0xc4670000,     \
0x3f1f8a96,   0x3ae27569,   0xc5fe0000,     \
0x3ed88fdf,   0x39477020,   0xc7df0000,     \
0x3ec091ee,   0x38b96e11,   0xc8860000,     \
0x3e519cb1,   0x3634634e,   0xcb790000,     \
    \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
    \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
    \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
    \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
    \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0

#define BES_LOUDNESS_LPF_COEFF \
0x1bc91bc9,   0x86c,   0x0,     \
0x1da81da8,   0x4ae,   0x0,     \
0x265d265d,   0xf345,   0x0,     \
0x32793279,   0xdb0c,   0x0,     \
0x37cb37cb,   0xd069,   0x0,     \
0x0,   0x0,   0x0
#define BES_LOUDNESS_WS_GAIN_MAX  0x0
#define BES_LOUDNESS_WS_GAIN_MIN  0x0
#define BES_LOUDNESS_FILTER_FIRST  0x0
#define BES_LOUDNESS_ATT_TIME  0x0
#define BES_LOUDNESS_REL_TIME  0x0
#define BES_LOUDNESS_GAIN_MAP_IN \
0xd5, 0xda, 0xe8, 0xfb, 0x6
#define BES_LOUDNESS_GAIN_MAP_OUT \
0x2, 0x3, 0x3, 0x2, 0xfffffffd
#endif
