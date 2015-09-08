#include "accel.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"
//#include "nrf_drv_twi.h"
//#include "app_util_platform.h"
#include "twi_master.h"
#include "board.h"
#include "mlcd.h"
#include "ble/ble_peripheral.h"

#define DEFAULT_I2C_ADDR 0x1D

static bool has_event = false;

//static const nrf_drv_twi_t twi = NRF_DRV_TWI_INSTANCE(0);
/*static const nrf_drv_twi_t twi = {                                                                             
         .p_reg       = NRF_TWI0,                                                  
         .irq         = NULL,//GPIOTE_IRQn,//SPI0_TWI0_IRQn,                                     
         .instance_id = TWI0_INSTANCE_INDEX                                      
        };*/

//static const nrf_drv_twi_config_t twi_config = NRF_DRV_TWI_DEFAULT_CONFIG(0);
	
static void accel_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
		has_event = true;
}

void accel_process_events() {
		if (!has_event) {
				return;
		}
		has_event = false;
		
		uint8_t int_src;
		accel_read_register(0x0C, &int_src);
/*		switch(pin) {
			case ACCEL_INT1:
					mlcd_backlight_toggle();
					break;
			case ACCEL_INT2:
					mlcd_colors_toggle();
					break;
		}*/
	
		if (int_src & 0x08) {
				
				uint8_t pulse_src;
				accel_read_register(0x22, &pulse_src);
			
				if (pulse_src & 0x80) {
						/*if (pulse_src & 0x8) {
								//double
								mlcd_colors_toggle();
						} else {
								mlcd_backlight_toggle();
						}*/
							
						uint8_t data[] = {int_src, pulse_src};
						ble_peripheral_invoke_notification_function_with_data(0xE0, data, sizeof(data));
				}
		}
}
	
static uint32_t accel_int_init(uint8_t pin_no)
{
    uint32_t err_code;

    if (!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);	
    config.pull = NRF_GPIO_PIN_PULLDOWN;
        
    err_code = nrf_drv_gpiote_in_init(pin_no, &config, accel_event_handler);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    nrf_drv_gpiote_in_event_enable(pin_no, true);
		return NRF_SUCCESS;
}

void accel_init(void) {
		twi_master_init();
	
    uint32_t err_code;
		err_code = accel_int_init(ACCEL_INT1);
    APP_ERROR_CHECK(err_code);
		err_code = accel_int_init(ACCEL_INT2);
    APP_ERROR_CHECK(err_code);
	
		//ret_code_t err_code = nrf_drv_twi_init(&twi, &twi_config, NULL);
		//APP_ERROR_CHECK(err_code);
		//nrf_drv_twi_enable(&twi);
}

void accel_write_register(uint8_t reg, uint8_t value) {
		uint8_t data[] = {reg, value};
		//nrf_drv_twi_tx(&twi, DEFAULT_I2C_ADDR, data, 2, false);
		twi_master_transfer	(	DEFAULT_I2C_ADDR<<1, data, 2, true);
}

void accel_read_register(uint8_t reg, uint8_t* value) {
		//nrf_drv_twi_tx(&twi, DEFAULT_I2C_ADDR, &reg, 1, true);
		//nrf_drv_twi_rx(&twi, DEFAULT_I2C_ADDR, value, 1, false);
		twi_master_transfer	(	DEFAULT_I2C_ADDR<<1, &reg, 1, false);
		twi_master_transfer	(	(DEFAULT_I2C_ADDR<<1)|TWI_READ_BIT, value, 1, true);
}
