/**
 * @file frequency.h
 * @brief Helpers for comparing CPU clock configurations and power behavior.
 *
 * This header exposes a single demo routine that reconfigures CPU frequency
 * at runtime using ESP-IDF Power Management (DFS) and prints the observed
 * frequency. It also defines a convenience delay constant.
 */

#ifndef FREQUENCY_H
#define FREQUENCY_H

#define WAIT_TIME_BETWEEN_CLOCK_FREQUENCY_CHANGES_MS 3000

/**
 * @brief Reconfigure CPU frequency across 80/160/240 MHz and print results.
 *
 * Procedure:
 *  1. Print the default PM config (min/max and light-sleep flag).
 *  2. Apply a first config (80–240 MHz, light sleep disabled) and print how
 *     long the call to esp_pm_configure() took.
 *  3. In a loop, set fixed frequencies [80-80], [160-160], [240-240], and
 *     print the measured CPU rate using the clock tree API.
 *
 * @note If the `rtos0` DFS lock is active (common with default FreeRTOS/IDF),
 *       the CPU may remain at the maximum configured frequency regardless of
 *       the min value you set. The demo prints existing PM locks to stdout.
 */
void compare_clock_speeds_power_consumption(void);

#endif