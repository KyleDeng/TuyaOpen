/*
 * MicroPython Configuration for T5AI (BK7258)
 * Minimal configuration for initial build
 */

#ifndef MICROPYTHON_MPCONFIGPORT_H
#define MICROPYTHON_MPCONFIGPORT_H

/* MicroPython version and build information */
#define MICROPY_HW_BOARD_NAME       "TuyaOpen-T5AI"
#define MICROPY_HW_MCU_NAME         "BK7258"
#define MICROPY_PY_SYS_PLATFORM     "T5AI"

/* Python language features - minimal set */
#define MICROPY_ENABLE_COMPILER     (0)  /* Start without compiler */
#define MICROPY_ENABLE_GC           (0)  /* Start without GC */
#define MICROPY_HELPER_REPL         (0)  /* Start without REPL */
#define MICROPY_MODULE_FROZEN_MPY   (0)
#define MICROPY_QSTR_BYTES_IN_HASH  (1)

/* Core Python features - disabled for minimal build */
#define MICROPY_PY_BUILTINS_FLOAT   (0)
#define MICROPY_PY_BUILTINS_COMPLEX (0)
#define MICROPY_PY_BUILTINS_DICT    (0)
#define MICROPY_PY_BUILTINS_SET     (0)
#define MICROPY_PY_BUILTINS_FROZENSET (0)
#define MICROPY_PY_BUILTINS_MEMORYVIEW (0)
#define MICROPY_PY_BUILTINS_SLICE   (0)
#define MICROPY_PY_BUILTINS_PROPERTY (0)

/* Memory allocation - will be replaced with real implementation */
#define MICROPY_ALLOC_PATH_MAX      (128)

/* Error reporting */
#define MICROPY_ERROR_REPORTING     (MICROPY_ERROR_REPORTING_TERSE)

/* Optimizations */
#define MICROPY_OPT_COMPUTED_GOTO   (0)
#define MICROPY_OPT_MPZ_BITWISE     (0)

/* Type definitions for T5AI */
typedef intptr_t mp_int_t;
typedef uintptr_t mp_uint_t;
typedef long mp_off_t;

/* We need to provide a type for the GC heap */
#define MICROPY_GCREGS_SETJMP       (0)

/* Minimal heap size for testing */
#define MICROPY_HEAP_SIZE           (8 * 1024)

/* Include HAL header */
#include "mphalport.h"

/* Dummy macros for now */
#define MP_PLAT_PRINT_STRN(str, len) mp_hal_stdout_tx_strn_cooked(str, len)

/* Define some basic functions as macros for minimal build */
#define mp_type_print(x, y, z)
#define mp_type_make_new(x, y, z, w)

/* Root pointers for GC (when enabled) */
#define MICROPY_PORT_ROOT_POINTERS

/* Miscellaneous settings */
#define MICROPY_KBD_EXCEPTION       (0)
#define MICROPY_HELPER_LEXER_UNIX   (0)
#define MICROPY_ENABLE_SOURCE_LINE  (0)
#define MICROPY_STREAMS_NON_BLOCK   (0)
#define MICROPY_MODULE_WEAK_LINKS   (0)
#define MICROPY_CAN_OVERRIDE_BUILTINS (0)
#define MICROPY_USE_INTERNAL_ERRNO  (0)
#define MICROPY_ENABLE_SCHEDULER    (0)

/* Module configuration */
#define MICROPY_PY___FILE__         (0)
#define MICROPY_PY_GC               (0)
#define MICROPY_PY_ARRAY            (0)
#define MICROPY_PY_COLLECTIONS      (0)
#define MICROPY_PY_MATH             (0)
#define MICROPY_PY_IO               (0)
#define MICROPY_PY_STRUCT           (0)
#define MICROPY_PY_SYS              (0)
#define MICROPY_PY_UTIME            (0)

/* Machine module configuration (disabled for now) */
#define MICROPY_PY_MACHINE          (0)

/* Network module configuration (disabled for now) */
#define MICROPY_PY_NETWORK          (0)

/* Tuya module configuration (disabled for now) */
#define MICROPY_PY_TUYA             (0)

#endif /* MICROPYTHON_MPCONFIGPORT_H */