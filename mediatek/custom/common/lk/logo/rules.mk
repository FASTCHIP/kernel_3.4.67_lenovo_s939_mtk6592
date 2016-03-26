LOCAL_DIR := $(GET_LOCAL_DIR)
BOOT_LOGO_DIR := $(LOCAL_DIR)
#$(info lk/logo/dir=$(LOCAL_DIR),builddir=$(BUILDDIR))

BMP_TO_RAW := $(BOOT_LOGO_DIR)/tool/bmp_to_raw
ZPIPE := $(BOOT_LOGO_DIR)/tool/zpipe


BOOT_LOGO_RESOURCE := $(BUILDDIR)/$(BOOT_LOGO_DIR)/$(BOOT_LOGO).raw
LOGO_IMAGE := $(BUILDDIR)/logo.bin
RESOURCE_OBJ_LIST :=   \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_uboot.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_battery.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_low_battery.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_charger_ov.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_0.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_1.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_2.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_3.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_4.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_5.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_6.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_7.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_8.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_9.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_num_percent.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_01.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_02.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_03.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_04.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_05.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_06.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_07.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_08.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_09.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_animation_10.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_01.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_02.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_03.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_04.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_05.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_06.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_07.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_08.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_09.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_10_10.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_bg.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_img.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_100.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_kernel.raw

#lenovo-sw jixj 2013.5.23 add begin
# 39
RESOURCE_OBJ_LIST += \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_factory.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_meta.raw 
#lenovo-sw jixj 2013.5.23 add end

#lenovo-sw yexh1 2013.6.17 add begin
# 41
RESOURCE_OBJ_LIST += \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/high_temp.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/low_temp.raw 
#lenovo-sw yexh1 2013.6.17  add end

#lenovo-sw liaohj 2013.10.31 add for lenovo poweroff charging ui begin
# 43
ifeq ($(LENOVO_POWEROFF_CHARGING_UI), yes)
RESOURCE_OBJ_LIST += \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_lenovo_charingtime.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_time_colon.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_horiz.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_0.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_1.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_2.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_3.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_4.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_5.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_6.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_7.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_8.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_date_9.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_lenovo_ani_0.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_lenovo_ani_1.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_lenovo_ani_2.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_lenovo_ani_3.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_capacity_logo.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_lenovo_ani_low_0.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_lenovo_ani_low_1.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_lenovo_ani_low_2.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_lenovo_ani_low_3.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_91.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_92.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_93.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_94.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_95.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_96.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_97.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_98.raw \
            $(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_bat_99.raw
endif 
#lenovo-sw liaohj 2013.10.31 add for lenovo poweroff charging ui end

GENERATED += \
            $(BOOT_LOGO_RESOURCE) \
            $(LOGO_IMAGE) \
            $(addprefix $(BUILDDIR)/,$(RESOURCE_OBJ_LIST))


all:: $(LOGO_IMAGE) 

$(LOGO_IMAGE):$(MKIMG) $(BOOT_LOGO_RESOURCE)
	$(NOECHO) if [ ! -x $(MKIMG) ]; then chmod a+x $(MKIMG); fi
	@echo "MKING $(LOGO_IMAGE)"
	$(MKIMG) $(BOOT_LOGO_RESOURCE) LOGO > $(LOGO_IMAGE)

$(BOOT_LOGO_RESOURCE): $(addprefix $(BUILDDIR)/,$(RESOURCE_OBJ_LIST)) $(ZPIPE)
	@$(MKDIR)
	$(NOECHO) if [ ! -x $(ZPIPE) ]; then chmod a+x $(ZPIPE); fi
	@echo "zpiping "
	$(ZPIPE) -l 9 $@ $(addprefix $(BUILDDIR)/,$(RESOURCE_OBJ_LIST))


$(BUILDDIR)/%.raw: %.bmp $(BMP_TO_RAW)
	@$(MKDIR)
	$(NOECHO) if [ ! -x $(BMP_TO_RAW) ]; then chmod a+x $(BMP_TO_RAW); fi
	@echo "Compiling_BMP_TO_RAW $<"
	$(BMP_TO_RAW) $@ $<

