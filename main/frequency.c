/**
 * @file frequency.c
 * @brief Demonstration of changing ESP32 CPU frequency via esp_pm and reading it.
 *
 * Shows how to:
 *  - Query the default power-management configuration;
 *  - Reconfigure DFS limits (min/max MHz and light-sleep flag);
 *  - Measure the time spent reconfiguring;
 *  - Inspect active power-management locks;
 *  - Read back the CPU clock using the Clock Tree API.
 */

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "esp_pm.h"
#include "esp_timer.h"
#include "esp_clk_tree.h"

#include "frequency.h"

/**
 * @brief Print current CPU frequency using Clock Tree API with a label.
 *
 * @param label Text prefix printed before the frequency value.
 */
static void print_cpu_hz(const char *label){
    uint32_t hz = 0;
    ESP_ERROR_CHECK(esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_CPU, ESP_CLK_TREE_SRC_FREQ_PRECISION_EXACT, &hz));
    printf("%s: " "%" PRIu32 "MHz\n", label, hz / 1000000);
}

void compare_clock_speeds_power_consumption(void)
{
    // 1) Show default PM configuration
    esp_pm_config_t default_cfg;
    esp_pm_get_configuration(&default_cfg);

    printf("\nDefault cfg:\nmin_freq_mhz = %d\nmax_freq_mhz = %d\nlight_sleep_enable = %s\n\n",
        default_cfg.min_freq_mhz,
        default_cfg.max_freq_mhz,
        default_cfg.light_sleep_enable ? "TRUE" : "FALSE");
    
    // 2) Apply initial wide range and measure reconfiguration time
    uint64_t frequency_change_t0 = esp_timer_get_time();   // µs
    esp_pm_config_t cfg = {
        .min_freq_mhz = 80,
        .max_freq_mhz = 240,
        .light_sleep_enable = false // if configUSE_TICKLESS_IDLE is on and light_sleep_enable is true, esp32 goes to sleep if all user tasks are blocked
    };
    ESP_ERROR_CHECK(esp_pm_configure(&cfg));
    uint64_t frequency_change_dt_us = esp_timer_get_time() - frequency_change_t0;
    printf("After first cfg:\nmin_freq_mhz = %d\nmax_freq_mhz = %d\nlight_sleep_enable = %s\n",
    cfg.min_freq_mhz,
    cfg.max_freq_mhz,
    cfg.light_sleep_enable ? "TRUE" : "FALSE");
    printf("This reconfiguration took %" PRIu64 "us, %.2fms.\n", frequency_change_dt_us, (float)frequency_change_dt_us / 1000);

    // 3) Show active PM locks (DFS can pin frequency at max)
    printf("\n");
    esp_pm_dump_locks(stdout);
    printf("Frequency is always set to max_freq_mhz due to the active rtos0 power management lock via Dynamic Frequency Scaling\n\n");

    // 4) Sweep fixed frequencies: 80, 160, 240 MHz
    while (1){
        cfg.min_freq_mhz = cfg.max_freq_mhz = 80;
        ESP_ERROR_CHECK(esp_pm_configure(&cfg));
        print_cpu_hz("Frequency after cfg [80-80]"); 
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("\n");
    
        cfg.min_freq_mhz = cfg.max_freq_mhz = 160;
        ESP_ERROR_CHECK(esp_pm_configure(&cfg));
        print_cpu_hz("Frequency after cfg [160-160]");
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("\n");
    
        cfg.min_freq_mhz = cfg.max_freq_mhz = 240;
        ESP_ERROR_CHECK(esp_pm_configure(&cfg));
        print_cpu_hz("Frequency after cfg [240-240]");
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("\n");
    }

}