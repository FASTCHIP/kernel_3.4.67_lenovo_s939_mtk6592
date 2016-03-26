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
0x7ac72b3,   0xf0a71a9a,   0x7ac72b3,   0x7aabc51d,   0x0,     \
0x7a5399a,   0xf0b58ccc,   0x7a5399a,   0x7a33c58c,   0x0,     \
0x783f4ea,   0xf0f8162b,   0x783f4ea,   0x7803c784,   0x0,     \
0x75c4ba5,   0xf14768b6,   0x75c4ba5,   0x755bc9d2,   0x0,     \
0x74e75ee,   0xf1631424,   0x74e75ee,   0x746cca9d,   0x0,     \
0x70f6207,   0xf1e13bf2,   0x70f6207,   0x7013ce26,   0x0,     \
0x6c58c6d,   0xf274e725,   0x6c58c6d,   0x6ad4d222,   0x0,     \
0x6ac2311,   0xf2a7b9dd,   0x6ac2311,   0x68fbd377,   0x0,     \
0x63a7944,   0xf38b0d78,   0x63a7944,   0x607ad92a,   0x0,     \
\
0x0,   0x0,   0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,   0x0,   0x0,     \
0x0,   0x0,   0x0,   0x0,   0x0

#define BES_LOUDNESS_BPF_COEFF \
0x3f96825b,   0x3ec27da4,   0xc1a70000,     \
0x3f8d82a1,   0x3ea67d5e,   0xc1cc0000,     \
0x3f628408,   0x3e267bf7,   0xc2770000,     \
0x3f2f8608,   0x3d8c79f7,   0xc3440000,     \
0x3f1d86d0,   0x3d56792f,   0xc38c0000,     \
0x3ecb8af4,   0x3c5e750b,   0xc4d60000,     \
\
0x423f8ee8,   0x32d77117,   0xcae80000,     \
0x426e907e,   0x31c56f81,   0xcbcb0000,     \
0x4341985c,   0x2cf367a3,   0xcfca0000,     \
0x4435a2db,   0x27645d24,   0xd4660000,     \
0x4488a6c7,   0x257d5938,   0xd5f90000,     \
0x45fbba16,   0x1d0145e9,   0xdd020000,     \
\
0x423f9ea4,   0x32d7615b,   0xcae80000,     \
0x426ea2df,   0x31c55d20,   0xcbcb0000,     \
0x4341b8d1,   0x2cf3472e,   0xcfca0000,     \
0x4435d6d2,   0x2764292d,   0xd4660000,     \
0x4488e1d5,   0x257d1e2a,   0xd5f90000,     \
0x45fb134f,   0x1d01ecb0,   0xdd020000,     \
\    
0x423fbef2,   0x32d7410d,   0xcae80000,     \
0x426ec80f,   0x31c537f0,   0xcbcb0000,     \
0x4341f500,   0x2cf30aff,   0xcfca0000,     \
0x4435292d,   0x2764d6d2,   0xd4660000,     \
0x448838e3,   0x257dc71c,   0xd5f90000,     \
0x0,   0x0,   0x0,     \
\    
0x3fc081ee,   0x3e977e11,   0xc1a70000,     \
0x3fbb8220,   0x3e787ddf,   0xc1cc0000,     \
0x3fa28315,   0x3de67cea,   0xc2770000,     \
0x3f83845b,   0x3d387ba4,   0xc3440000,     \
0x3f7884d6,   0x3cfb7b29,   0xc38c0000,     \
0x3f478743,   0x3be178bc,   0xc4d60000,     \
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
0xd3, 0xda, 0xed, 0xee, 0x0
#define BES_LOUDNESS_GAIN_MAP_OUT \
0xc, 0xc, 0xc, 0xc, 0x0
#endif
