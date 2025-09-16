/**
 * @file sleep_modes.c
 * @brief Implementation of an ESP32 power management demo using sleep modes.
 *
 * The demo showcases three behaviors:
 *  - Staying fully active and blinking an LED;
 *  - Entering light sleep and waking up via a timer;
 *  - Entering deep sleep and waking up via a timer (which resets the MCU).
 *
 * Select the behavior at compile time via @ref POWER_MODE.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "esp_sleep.h"

#include "sleep_modes.h"

static const char *TAG = "PM_DEMO";

/**
 * @brief Toggle the on-board LED once with a short ON pulse.
 *
 * Uses GPIO 2 by default (typical for many ESP32 devkits).
 * If your board wires the LED to a different pin, adjust this function.
 */
static void blink_once(void) {
    const gpio_num_t led = GPIO_NUM_2;
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    gpio_set_level(led, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(led, 0);
}

/**
 * @brief Remain active and blink periodically.
 *
 * Logs the mode and then blinks the LED once per second in an infinite loop.
 */
static void stay_active_loop(void) {
    ESP_LOGI(TAG, "ACTIVE");
    while (1) { blink_once(); vTaskDelay(pdMS_TO_TICKS(900)); }
}

/**
 * @brief Enter light sleep and wake up after @ref WAKE_TIMER_US microseconds.
 *
 * The function configures a timer wake-up source, starts light sleep, and
 * resumes when the timer fires. It logs before entering and after waking.
 */
static void enter_light_sleep_timer(void) {
    ESP_LOGI(TAG, "LIGHT_SLEEP: %llus", WAKE_TIMER_US / 1000000ULL);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(WAKE_TIMER_US));
    ESP_ERROR_CHECK(esp_light_sleep_start());
    ESP_LOGI(TAG, "Woke up from light sleep.");
}

/**
 * @brief Enter deep sleep and wake up after @ref WAKE_TIMER_US microseconds.
 *
 * This call never returns. On wake-up the MCU fully resets and starts from
 * bootloader, then app entry, as usual for deep-sleep wake events.
 */
static void enter_deep_sleep_timer(void) {
    ESP_LOGI(TAG, "DEEP_SLEEP: %llus.", WAKE_TIMER_US / 1000000ULL);
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(WAKE_TIMER_US));
    esp_deep_sleep_start();
}

void compare_sleep_modes_power_consumption(void)
{    
    switch(POWER_MODE){
        case ACTIVE:            stay_active_loop(); break;
        case LIGHT_SLEEP_TIMER: while (1) { blink_once(); enter_light_sleep_timer(); } break;
        case DEEP_SLEEP_TIMER:  enter_deep_sleep_timer(); break;
        default:                printf("POWER_MODE unknown. Use: ACTIVE, LIGHT_SLEEP_TIMER, LIGHT_SLEEP_GPIO, DEEP_SLEEP_TIMER, DEEP_SLEEP_GPIO"); break;
    }
}