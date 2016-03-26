#include <cust_vibrator.h>
#include <linux/types.h>

static struct vibrator_hw cust_vibrator_hw = {
	.vib_timer = 50,
  #ifdef CUST_VIBR_LIMIT
	.vib_limit = 9,
  #endif
  #ifdef CUST_VIBR_VOL
       #ifdef LENOVO_VIBR_VOL_MAX
	.vib_vol = 0x7,//3.3//0x5,//2.8V for vibr
	#else
	.vib_vol =0x05,
	#endif
  #endif
};

struct vibrator_hw *get_cust_vibrator_hw(void)
{
    return &cust_vibrator_hw;
}

