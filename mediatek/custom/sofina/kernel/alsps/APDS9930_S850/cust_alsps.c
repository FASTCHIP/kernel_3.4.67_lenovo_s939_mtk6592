#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>
//#include <mach/mt6577_pm_ldo.h>

static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 2,
	.polling_mode_ps =0,
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    .i2c_addr   = {0x72, 0x48, 0x78, 0x00},
    /*Lenovo-sw chenlj2 add 2011-06-03,modify parameter below two lines*/
    /*lenovo-sw jixj 2011.12.22 modified being*/
    /*
    .als_level  = { 4, 40,  80,   120,   160, 250,  400, 800, 1200,  1600, 2000, 3000, 5000, 10000, 65535},
    .als_value  = {10, 20,20,  120, 120, 280,  280,  280, 1600,  1600,  1600,  6000,  6000, 9000,  10240, 10240},
    */
    /*Lenovo chenlj2 20130725 modify for new Light sensor param begin*/
   .als_level	= {2,10, 110,200, 792,1669, 2578,4635,8110, 13100, 15610},
    .als_value	= {5, 20, 90, 200, 350, 720, 1100, 2000, 3500, 5600, 6700, 8700},
    /*Lenovo chenlj2 20130725 modify for new Light sensor param end*/
    /*lenovo-sw jixj 2011.12.22 modified end*/
    /*Lenovo-sw chenlj2 modified 2011-08-09*/
    .ps_threshold_high = 900,
    .ps_threshold_low = 750,
    .ps_threshold = 900,
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}
/* lenovo scofield board sensors spec, plz don't change it!!! ---liaoxl.lenovo 4.19.2012 start */
int APDS9930_CMM_PPCOUNT_VALUE = 0x0F;
int APDS9930_CMM_CONTROL_VALUE = 0x20;//0x68;
int ZOOM_TIME = 4;
