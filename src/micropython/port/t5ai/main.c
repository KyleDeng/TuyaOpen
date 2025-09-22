/*
 * MicroPython main entry for T5AI (BK7258)
 * Minimal implementation for compilation test
 */

#include <stdio.h>
#include <stdint.h>
#include "tal_log.h"
#include "tal_system.h"
#include "tal_thread.h"

#ifdef CONFIG_ENABLE_MICROPYTHON

/* MicroPython task priority and stack size */
#define MP_TASK_PRIORITY    5
#define MP_TASK_STACK_SIZE  (8 * 1024)

/* MicroPython main task handle */
static THREAD_HANDLE sg_mp_thread = NULL;

/**
 * @brief MicroPython main task
 */
static void micropython_task(void *arg)
{
    PR_NOTICE("MicroPython task started");

    /* TODO: Initialize MicroPython core */
    PR_DEBUG("MicroPython version: minimal build for T5AI");

    /* TODO: Initialize heap */

    /* TODO: Initialize modules */

    /* TODO: Start REPL if enabled */

    /* Main loop */
    while (1) {
        /* TODO: Run MicroPython main loop */
        tal_system_sleep(1000); /* Sleep 1 second for now */
    }
}

/**
 * @brief Initialize MicroPython component
 * @return 0 on success, negative on error
 */
int micropython_init(void)
{
    OPERATE_RET rt = OPRT_OK;

    PR_NOTICE("Initializing MicroPython for T5AI...");

    /* Create MicroPython main thread */
    THREAD_CFG_T thread_cfg = {
        .priority = MP_TASK_PRIORITY,
        .stackDepth = MP_TASK_STACK_SIZE,
        .thrdname = "micropython"
    };

    rt = tal_thread_create_and_start(&sg_mp_thread,
                                      NULL,
                                      NULL,
                                      micropython_task,
                                      NULL,
                                      &thread_cfg);

    if (rt != OPRT_OK) {
        PR_ERR("Failed to create MicroPython thread: %d", rt);
        return -1;
    }

    PR_NOTICE("MicroPython initialized successfully");
    return 0;
}

/**
 * @brief Deinitialize MicroPython component
 */
void micropython_deinit(void)
{
    PR_NOTICE("Deinitializing MicroPython...");

    if (sg_mp_thread) {
        tal_thread_delete(sg_mp_thread);
        sg_mp_thread = NULL;
    }

    /* TODO: Clean up MicroPython resources */

    PR_NOTICE("MicroPython deinitialized");
}

#else /* CONFIG_ENABLE_MICROPYTHON */

int micropython_init(void)
{
    PR_NOTICE("MicroPython is disabled in configuration");
    return 0;
}

void micropython_deinit(void)
{
    /* Nothing to do */
}

#endif /* CONFIG_ENABLE_MICROPYTHON */