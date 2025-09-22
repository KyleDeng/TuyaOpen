/**
 * @file app_main.c
 * @brief MicroPython minimal application for T5AI
 */

#include "tuya_cloud_types.h"
#include "tal_log.h"
#include "tal_system.h"
#include "tal_thread.h"
#include "tkl_init.h"
#include "netmgr.h"

/* MicroPython header */
#include "micropython.h"

#define APP_TASK_PRIORITY       (THREAD_PRIO_2)
#define APP_TASK_STACK_SIZE     (4096)

static void app_task(void *arg)
{
    PR_NOTICE("MicroPython minimal app started");

    /* Wait a bit for system to stabilize */
    tal_system_sleep(2000);

#ifdef CONFIG_ENABLE_MICROPYTHON
    /* Initialize MicroPython */
    PR_NOTICE("Initializing MicroPython...");
    if (micropython_init() != 0) {
        PR_ERR("Failed to initialize MicroPython");
    } else {
        PR_NOTICE("MicroPython initialized successfully");
    }
#else
    PR_NOTICE("MicroPython is disabled in configuration");
#endif

    /* Main loop */
    while (1) {
        tal_system_sleep(10000); /* Sleep 10 seconds */
    }
}

/**
 * @brief Application main entry
 */
void tuya_app_main(void)
{
    OPERATE_RET rt = OPRT_OK;
    THREAD_HANDLE app_thread = NULL;

    PR_NOTICE("========================================");
    PR_NOTICE("   MicroPython Minimal for T5AI");
    PR_NOTICE("========================================");

    /* Create application task */
    THREAD_CFG_T thread_cfg = {
        .stackDepth = APP_TASK_STACK_SIZE,
        .priority = APP_TASK_PRIORITY,
        .thrdname = "mpy_main"
    };

    rt = tal_thread_create_and_start(&app_thread, NULL, NULL, app_task, NULL, &thread_cfg);
    if (rt != OPRT_OK) {
        PR_ERR("Failed to create app task: %d", rt);
        return;
    }

    PR_NOTICE("Application started successfully");
}