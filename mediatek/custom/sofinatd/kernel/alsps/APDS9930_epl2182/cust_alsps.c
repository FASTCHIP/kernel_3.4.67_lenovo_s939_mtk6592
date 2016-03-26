#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>
#include <mach/upmu_common.h>

static struct alsps_hw cust_alsps_hw_apds9930 = {
    .i2c_num    = 2,
	.polling_mode_ps =0,
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    .i2c_addr   = {0x72, 0x48, 0x78, 0x00},
    /*Lenovo-sw chenlj2 add 2011-06-03,modify parameter below two lines*/
    //.als_level  = { 4, 40,  80,   120,   160, 250,  400, 800, 1200,  1600, 2000, 3000, 5000, 10000, 65535},
    //.als_value  = {10, 20,20,  120, 120, 280,  280,  280, 1600,  1600,  1600,  6000,  6000, 9000,  10240, 10240},
    //.als_level  = { 5, 10,  25,   50,  100, 150,  200, 400, 1000,  1500, 2000, 3000, 5000, 8000, 10000},
    //.als_value  = {10, 50,  100,  150, 200, 250,  280,  280, 1600,  1600,  1600,  6000,  6000, 9000,  10240, 10240},
    /* MTK: modified to support AAL */
    //.als_level  = { 6, 9, 17, 38, 56, 74, 116, 342, 778, 1082,  1386, 1914, 3000, 5000, 8000 },
    //.als_value  = {136, 218, 312, 730, 1065, 1400, 2250, 4286, 5745, 7390, 9034, 11000, 12250, 12250, 12250, 12250},
/*Lenovo houdz1 20130820 modify for new Light sensor param begin*/
    //.als_level	= {10, 110, 792, 2578,8110, 13100, 15610},
    //.als_value	= {20, 90, 350, 1100, 3500, 5600, 6700, 8700},
/*Lenovo houdz1 20130820 modify for new Light sensor param end*/
/*Lenovo houdz1 20130822 modify for new Light sensor param begin*/
   .als_level	= {2,10, 110,200, 792,1669, 2578,4635,8110, 13100, 15610},
   .als_value	= {5,20, 90,200, 350,720, 1100, 2000,3500, 5600, 6700, 8700},
/*Lenovo houdz1 20130822 modify for new Light sensor param end*/

    .ps_threshold_high = 900,
    .ps_threshold_low = 750,
    .ps_threshold = 900,
};
static struct alsps_hw cust_alsps_hw_epl2182 = {
	.i2c_num    = 2,
	.polling_mode_ps =0,
        .polling_mode_als =1,
	.power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
	.power_vol  = VOL_DEFAULT,          /*LDO is not used*/
	.i2c_addr   = {0x0C, 0x48, 0x78, 0x00},
/*Lenovo houdz1 20130725 modify for new Light sensor param begin*/
	//.als_level	= {200, 1000, 6000, 18928,60832, 65535, 65535},
	//.als_value	= {20, 90, 350, 1100, 3500, 5600, 6700, 8700},
/*Lenovo houdz1 20130725 modify for new Light sensor param end*/
/*Lenovo houdz1 20130822 modify for new Light sensor param begin*/
	.als_level	= {10, 300, 650, 1000, 1600, 2100, 6900, 8325, 10175, 14300, 17575, 18928, 27750, 37000, 60832, 65536, 65536},
	.als_value	= {5, 30, 60, 90, 150, 200, 350, 450, 550, 720, 950, 1100, 1500, 2000,3500, 5600, 6700, 8700},
/*Lenovo houdz1 20130822 modify for new Light sensor param end*/
	.ps_threshold_low = 2000,
	.ps_threshold_high = 2500,
};
int alsps_device_index = 0;// 0 unknow 1 avago 2 rohm
struct alsps_hw *get_cust_alsps_hw(void) {
    if(alsps_device_index ==1)
    return &cust_alsps_hw_apds9930;
    else if(alsps_device_index ==2)
    return &cust_alsps_hw_epl2182;
    else
    return &cust_alsps_hw_apds9930;
}
int APDS9930_CMM_PPCOUNT_VALUE = 0x06;
int APDS9930_CMM_CONTROL_VALUE = 0x28;//0x68;
int ZOOM_TIME = 4;


