#include <stdio.h>
#include <math.h>
#include "driver/ledc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    ledc_timer_config_t tconfig;

    tconfig.speed_mode = LEDC_LOW_SPEED_MODE;                /*!< LEDC speed speed_mode, high-speed mode or low-speed mode */
    tconfig.duty_resolution = LEDC_TIMER_13_BIT; //Means 2**13 number of LED intensities available /*!< LEDC channel duty resolution */
    tconfig.timer_num = LEDC_TIMER_1;               /*!< The timer source of channel (0 - 3) */
    tconfig.freq_hz = 5000;                      /*!< LEDC timer frequency (Hz) */
    tconfig.clk_cfg = LEDC_AUTO_CLK;              /*!< Configure LEDC source clock.
                                                For low speed channels and high speed channels, you can specify the source clock using LEDC_USE_REF_TICK, LEDC_USE_APB_CLK or LEDC_AUTO_CLK.
                                                For low speed channels, you can also specify the source clock using LEDC_USE_RTC8M_CLK, in this case, all low speed channel's source clock must be RTC8M_CLK*/
    ledc_timer_config(&tconfig);

    ledc_channel_config_t cconfig;

    cconfig.gpio_num = 16;                 /*!< the LEDC output gpio_num, if you want to use gpio16, gpio_num = 16 */
    cconfig.speed_mode = LEDC_LOW_SPEED_MODE;        /*!< LEDC speed speed_mode, high-speed mode or low-speed mode */
    cconfig.channel = LEDC_CHANNEL_0;         /*!< LEDC channel (0 - 7) */
    cconfig.timer_sel = LEDC_TIMER_1;        /*!< Select the timer source of channel (0 - 3) */
    cconfig.duty =  0;                /*!< LEDC channel duty, the range of duty setting is [0, (2**duty_resolution)] */
    cconfig.hpoint = 0;                    /*!< LEDC channel hpoint value, the max value is 0xfffff */

    ledc_channel_config(&cconfig);

    while(true)
    {
        for( int i = 0 ; i < pow(2,13); i++)
        {
            esp_err_t err = ledc_set_duty(cconfig.speed_mode, cconfig.channel, i);
            if(err == -1)
            {
                ESP_LOGE("LEDC", "Duty cycle set error");
            }
            ledc_update_duty(cconfig.speed_mode, cconfig.channel);
            printf("%d \n",i);
        }

        vTaskDelay(1000/portTICK_PERIOD_MS);

        for( int i = pow(2,13)-1 ; i > 0; i--)
        {
            esp_err_t err = ledc_set_duty(cconfig.speed_mode, cconfig.channel, i);
            if(err == -1)
            {
                ESP_LOGE("LEDC", "Duty cycle set error");
            }
            ledc_update_duty(cconfig.speed_mode, cconfig.channel);
            printf("%d \n",i);
        }

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}   
