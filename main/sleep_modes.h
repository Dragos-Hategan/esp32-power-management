/**
 * @file sleep_modes.h
 * @brief Public API for demonstrating ESP32 sleep modes and power behavior.
 *
 * This header defines the power mode selector and timing configuration used by
 * the demo in @ref sleep_modes.c. You can choose the power mode at compile time
 * by defining @ref POWER_MODE and the wake timer via @ref WAKE_TIMER_US.
 */

#ifndef SLEEP_MODES_H
#define SLEEP_MODES_H

/**
 * @enum power_modes_t
 * @brief Compile-time selectable operating mode for the demo.
 *
 * - @ref ACTIVE            : Remain fully active; periodically blink the LED.
 * - @ref LIGHT_SLEEP_TIMER : Enter light sleep and wake up using a timer.
 * - @ref DEEP_SLEEP_TIMER  : Enter deep sleep and wake up using a timer
 *                            (the MCU resets on wake).
 */
typedef enum{
    ACTIVE,                 /**< Stay active; no sleep. */
    LIGHT_SLEEP_TIMER,      /**< Light-sleep; wake on timer. */
    DEEP_SLEEP_TIMER        /**< Deep-sleep; wake on timer (causes reset). */
} power_modes_t;

/**
 * @def POWER_MODE
 * @brief Selected demo power mode at compile time.
 *
 * Define this macro (e.g., in CMake with `-DPOWER_MODE=LIGHT_SLEEP_TIMER`)
 * to pick which mode @ref compare_sleep_modes_power_consumption executes.
 */
#ifndef POWER_MODE
#define POWER_MODE DEEP_SLEEP_TIMER
#endif

/**
 * @def WAKE_TIMER_US
 * @brief Wake-up interval in microseconds used for timer-based sleep modes.
 */
#ifndef WAKE_TIMER_US
#define WAKE_TIMER_US (5ULL * 1000000ULL)   // 5s
#endif

/**
 * @brief Run the selected demo and compare power behavior across modes.
 *
 * The executed path depends on the compile-time @ref POWER_MODE:
 * - @ref ACTIVE: stays active and blinks forever;
 * - @ref LIGHT_SLEEP_TIMER: blinks then enters light sleep repeatedly;
 * - @ref DEEP_SLEEP_TIMER: configures timer and enters deep sleep (no return).
 */
void compare_sleep_modes_power_consumption (void);

#endif