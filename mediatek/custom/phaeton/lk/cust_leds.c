//#include <platform/cust_leds.h>
#include <cust_leds.h>
#include <platform/mt_pwm.h>

//extern int DISP_SetBacklight(int level);

extern int disp_bls_set_backlight(unsigned int level);

// Only support 64 levels of backlight (when lcd-backlight = MT65XX_LED_MODE_PWM)
#define BACKLIGHT_LEVEL_PWM_64_FIFO_MODE_SUPPORT 64 
// Support 256 levels of backlight (when lcd-backlight = MT65XX_LED_MODE_PWM)
#define BACKLIGHT_LEVEL_PWM_256_SUPPORT 256 

// Custom can decide the support type "BACKLIGHT_LEVEL_PWM_256_SUPPORT" or "BACKLIGHT_LEVEL_PWM_64_FIFO_MODE_SUPPORT"
#define BACKLIGHT_LEVEL_PWM_MODE_CONFIG BACKLIGHT_LEVEL_PWM_256_SUPPORT
#ifdef LENOVO_LCD_BACKLIGHT_COMPATIBLE_SUPPORT
extern int DISP_SetBacklight(int level);
extern BOOL DISP_IsLcmBacklight(void);
unsigned int Cust_SetBacklight(int level, int div)
{
	if(DISP_IsLcmBacklight())
	    DISP_SetBacklight(level);
	else
		disp_bls_set_backlight(level);
		
    return 0;
}

#elif defined(LENOVO_LCD_BACKLIGHT_CONTROL_BY_LCM)//lenovo add by jixu@lenovo.com
extern int DISP_SetBacklight(int level);
unsigned int Cust_SetBacklight(int level, int div)
{
    DISP_SetBacklight(level);
    return 0;
}
#endif
unsigned int Cust_GetBacklightLevelSupport_byPWM(void)
{
	return BACKLIGHT_LEVEL_PWM_MODE_CONFIG;
}

static struct cust_mt65xx_led cust_led_list[MT65XX_LED_TYPE_TOTAL] = {
	{"red",               MT65XX_LED_MODE_PMIC, MT65XX_LED_PMIC_NLED_ISINK1,{0,0,0,0,0}},
	{"green",             MT65XX_LED_MODE_NONE, -1,{0,0,0,0,0}},
	{"blue",              MT65XX_LED_MODE_NONE, -1,{0,0,0,0,0}},
	{"jogball-backlight", MT65XX_LED_MODE_NONE, -1,{0,0,0,0,0}},
	{"keyboard-backlight",MT65XX_LED_MODE_NONE, -1,{0,0,0,0,0}},
	{"button-backlight",  MT65XX_LED_MODE_NONE, -1,{0,0,0,0,0}},
	#if defined(LENOVO_LCD_BACKLIGHT_CONTROL_BY_LCM) || defined(LENOVO_LCD_BACKLIGHT_COMPATIBLE_SUPPORT)//lenovo add by jixu@lenovo.com
	{"lcd-backlight",     MT65XX_LED_MODE_CUST_LCM, (int)Cust_SetBacklight,{0}},//control by cabc
	#else
	{"lcd-backlight",     MT65XX_LED_MODE_CUST_BLS_PWM, (int)disp_bls_set_backlight,{0,0,0,0,0}},
 #endif
};

struct cust_mt65xx_led *get_cust_led_list(void)
{
	return cust_led_list;
}

