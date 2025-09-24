/**
 * @file mpprint_port.c
 * @brief MicroPython print implementation for T5AI port
 * This file provides the platform-specific print functions
 */

#include "py/mpconfig.h"
#include "py/mpprint.h"
#include "mphalport.h"

#if MICROPY_PY_IO && MICROPY_PY_SYS_STDFILES
#include "py/mphal.h"
#endif

// Forward declaration (already in mphalport.h but including for clarity)
mp_uint_t mp_hal_stdout_tx_strn(const char *str, size_t len);

/**
 * @brief Platform-specific print function
 * This function is called by MicroPython's print system to output text
 *
 * @param data User data (unused)
 * @param str String to print
 * @param len Length of string
 */
static void mp_plat_print_strn(void *data, const char *str, size_t len) {
    (void)data;  // Unused parameter

    // Use the HAL function to output the string
    mp_hal_stdout_tx_strn(str, len);
}

/**
 * @brief Global print object for MicroPython
 * This structure is used throughout MicroPython for printing
 */
const mp_print_t mp_plat_print = {
    .data = NULL,
    .print_strn = mp_plat_print_strn,
};

#if MICROPY_PY_IO && MICROPY_PY_SYS_STDFILES

/**
 * @brief File print function for sys.stdout
 * Used when MICROPY_PY_SYS_STDFILES is enabled
 */
static void mp_stderr_print_strn(void *data, const char *str, size_t len) {
    (void)data;
    // For stderr, we also use the same output (could be differentiated later)
    mp_hal_stdout_tx_strn(str, len);
}

/**
 * @brief stderr print object
 */
const mp_print_t mp_stderr_print = {
    .data = NULL,
    .print_strn = mp_stderr_print_strn,
};

#endif // MICROPY_PY_IO && MICROPY_PY_SYS_STDFILES