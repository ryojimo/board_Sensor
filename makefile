#------------------------------------------------------------------------------
# Path Setting
#------------------------------------------------------------------------------
#--------------------------------------
# Source File Path
#--------------------------------------
PATH_SYS         = ./sys
PATH_HAL         = ./hal
PATH_HAL_TSL2561 = ./hal/tsl2561
PATH_APP_IF_LCD  = ./app/if_lcd
PATH_APP_IF_PC   = ./app/if_pc
PATH_APP_MENU    = ./app/menu

OBJS       = main.o \
             sys.o \
             hal_cmn.o \
             hal_cmn_gpio.o \
             hal_cmn_i2c.o \
             hal_cmn_spi.o \
             hal_cmn_spi_mcp3208.o \
             hal_drv_i2c_lcd.o \
             hal_drv_led.o \
             hal_drv_motorSV.o \
             hal_drv_pushsw.o \
             hal_drv_relay.o \
             hal_drv_sensor_adc_acc.o \
             hal_drv_sensor_adc_gyro.o \
             hal_drv_sensor_adc_pm.o \
             hal_drv_sensor_i2c_gp2y0e03.o \
             hal_drv_sensor_i2c_lps25h.o \
             hal_drv_sensor_i2c_tsl2561.o \
             hal_drv_time.o \
             TSL2561.o \
             if_lcd.o \
             if_pc.o \
             menu.o \
             menu_base.o \
             menu_cmd_help.o \
             menu_cmd_i2c_lcd.o \
             menu_cmd_led.o \
             menu_cmd_motorSV.o \
             menu_cmd_pushsw.o \
             menu_cmd_relay.o \
             menu_cmd_sensor_adc_acc.o \
             menu_cmd_sensor_adc_gyro.o \
             menu_cmd_sensor_adc_pm.o \
             menu_cmd_sensor_i2c_gp2y0e03.o \
             menu_cmd_sensor_i2c_lps25h.o \
             menu_cmd_sensor_i2c_tsl2561.o \
             menu_cmd_time.o \
             menu_input.o \
             menu_input_switch.o \
             menu_input_volume.o


TARGET     = rpx.out
TARGET_MAP = rpx.map

CFLAGS = -v -O2 -Wall -c

all                            : rpx.out
main.o                         : main.c
sys.o                          : $(PATH_SYS)/sys.c
hal_cmn.o                      : $(PATH_HAL)/hal_cmn.c
hal_cmn_gpio.o                 : $(PATH_HAL)/hal_cmn_gpio.c
hal_cmn_i2c.o                  : $(PATH_HAL)/hal_cmn_i2c.c
hal_cmn_spi.o                  : $(PATH_HAL)/hal_cmn_spi.c
hal_cmn_spi_mcp3208.o          : $(PATH_HAL)/hal_cmn_spi_mcp3208.c
hal_drv_i2c_lcd.o              : $(PATH_HAL)/hal_drv_i2c_lcd.c
hal_drv_led.o                  : $(PATH_HAL)/hal_drv_led.c
hal_drv_motorSV.o              : $(PATH_HAL)/hal_drv_motorSV.c
hal_drv_pushsw.o               : $(PATH_HAL)/hal_drv_pushsw.c
hal_drv_relay.o                : $(PATH_HAL)/hal_drv_relay.c
hal_drv_sensor_adc_acc.o       : $(PATH_HAL)/hal_drv_sensor_adc_acc.c
hal_drv_sensor_adc_gyro.o      : $(PATH_HAL)/hal_drv_sensor_adc_gyro.c
hal_drv_sensor_adc_pm.o        : $(PATH_HAL)/hal_drv_sensor_adc_pm.c
hal_drv_sensor_i2c_gp2y0e03.o  : $(PATH_HAL)/hal_drv_sensor_i2c_gp2y0e03.c
hal_drv_sensor_i2c_lps25h.o    : $(PATH_HAL)/hal_drv_sensor_i2c_lps25h.c
hal_drv_sensor_i2c_tsl2561.o   : $(PATH_HAL)/hal_drv_sensor_i2c_tsl2561.c
hal_drv_time.o                 : $(PATH_HAL)/hal_drv_time.c
TSL2561.o                      : $(PATH_HAL_TSL2561)/TSL2561.c
if_lcd.o                       : $(PATH_APP_IF_LCD)/if_lcd.c
if_pc.o                        : $(PATH_APP_IF_PC)/if_pc.c
menu.o                         : $(PATH_APP_MENU)/menu.c
menu_base.o                    : $(PATH_APP_MENU)/menu_base.c
menu_cmd_help.o                : $(PATH_APP_MENU)/menu_cmd_help.c
menu_cmd_i2c_lcd.o             : $(PATH_APP_MENU)/menu_cmd_i2c_lcd.c
menu_cmd_led.o                 : $(PATH_APP_MENU)/menu_cmd_led.c
menu_cmd_motorSV.o             : $(PATH_APP_MENU)/menu_cmd_motorSV.c
menu_cmd_pushsw.o              : $(PATH_APP_MENU)/menu_cmd_pushsw.c
menu_cmd_relay.o               : $(PATH_APP_MENU)/menu_cmd_relay.c
menu_cmd_sensor_adc_acc.o      : $(PATH_APP_MENU)/menu_cmd_sensor_adc_acc.c
menu_cmd_sensor_adc_gyro.o     : $(PATH_APP_MENU)/menu_cmd_sensor_adc_gyro.c
menu_cmd_sensor_adc_pm.o       : $(PATH_APP_MENU)/menu_cmd_sensor_adc_pm.c
menu_cmd_sensor_i2c_gp2y0e03.o : $(PATH_APP_MENU)/menu_cmd_sensor_i2c_gp2y0e03.c
menu_cmd_sensor_i2c_lps25h.o   : $(PATH_APP_MENU)/menu_cmd_sensor_i2c_lps25h.c
menu_cmd_sensor_i2c_tsl2561.o  : $(PATH_APP_MENU)/menu_cmd_sensor_i2c_tsl2561.c
menu_cmd_time.o                : $(PATH_APP_MENU)/menu_cmd_time.c
menu_input.o                   : $(PATH_APP_MENU)/menu_input.c
menu_input_switch.o            : $(PATH_APP_MENU)/menu_input_switch.c
menu_input_volume.o            : $(PATH_APP_MENU)/menu_input_volume.c


rpx.out: $(OBJS)
	gcc $(OBJS) -o $(TARGET) -I/usr/local/include -L/usr/local/lib -lrt -lwiringPi -Wl,-Map=$(TARGET_MAP)

%.o: $(PATH_APP_IF_LCD)/%.c
	gcc $(CFLAGS) $< -o $@

%.o: $(PATH_APP_IF_PC)/%.c
	gcc $(CFLAGS) $< -o $@

%.o: $(PATH_APP_MENU)/%.c
	gcc $(CFLAGS) $< -o $@

%.o: $(PATH_HAL)/%.c
	gcc $(CFLAGS) $< -o $@

%.o: $(PATH_HAL_TSL2561)/%.c
	gcc $(CFLAGS) $< -o $@

%.o: $(PATH_SYS)/%.c
	gcc $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET_MAP)

