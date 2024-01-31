#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"
#include "freertos/FreeRTOSConfig.h"

#define led1 5
uint8_t led_level = 0;
static const char* TAG = "Main";
TimerHandle_t xTimers;
int interval = 1000; 
int timerId = 1;

esp_err_t init_led(void);
esp_err_t blink_led(void);
esp_err_t set_timer(void);

void vTimerCallback(TimerHandle_t pxTimer)
{
    ESP_LOGI(TAG, "Event was called from timer");
    blink_led();
}

void app_main(void)
{
    init_led();
    set_timer();
}

esp_err_t init_led(void)
{
    gpio_reset_pin(led1);
    gpio_set_direction(led1, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t blink_led(void)
{
    led_level = !led_level;
    gpio_set_level(led1, led_level);
    return ESP_OK;
}

esp_err_t set_timer(void)
{
    ESP_LOGI(TAG, "Timer init configuration");
    xTimers = xTimerCreate("Timer",
                            (pdMS_TO_TICKS(interval)),
                            pdTRUE, 
                            (void *)timerId, 
                            vTimerCallback
    );

    if (xTimers == NULL)
    {
        ESP_LOGE(TAG, "The timer was not created");
    }
    else
    {
        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            ESP_LOGE(TAG, "The timer could not be set into the active state");
        }
    }
    return ESP_OK;
}