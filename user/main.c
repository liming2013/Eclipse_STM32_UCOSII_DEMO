#include <includes.h>
#include "led.h"

unsigned int watch_cnt=0;

static  void  task_start (void *p_arg);

#define START_TASK_PRIO		6												//¿ªÊ¼ÈÎÎñ
#define START_TASK_STK_SIZE	128										//¿ªÊ¼ÈÎÎñ¶ÑÕ»´óÐ¡
OS_STK START_TASK_STK[START_TASK_STK_SIZE];				//¿ªÊ¼ÈÎÎñ¶ÑÕ»

int  main (void)
{
    CPU_INT08U  err;


    //BSP_IntDisAll();                                            /* Disable all interrupts until we are ready to accept them */

    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"              */

    OSTaskCreate(task_start,
			(void *)0,
			(OS_STK*)&START_TASK_STK[START_TASK_STK_SIZE-1],
			START_TASK_PRIO
			);

#if (OS_TASK_NAME_EN > 0)
    //OSTaskNameSet(APP_TASK_START_PRIO, "Start Task", &err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}

static  void  task_start (void *p_arg)
{
    CPU_INT32U  hclk_freq;
    CPU_INT32U  cnts;

    RCC_ClocksTypeDef  rcc_clocks;
    uint8_t LEDVal=0;

   (void)p_arg;

    //BSP_Init();                                                 /* Init BSP fncts.                                          */
   LED_Init();
   CPU_Init();                                                 /* Init CPU name & int. dis. time measuring fncts.          */

   RCC_GetClocksFreq(&rcc_clocks);
   hclk_freq = (CPU_INT32U)rcc_clocks.HCLK_Frequency;          /* Determine SysTick reference freq.                        */
   cnts  = hclk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;           /* Determine nbr SysTick increments in OS_TICKS_PER_SEC.    */
   OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).                  */

   Mem_Init();                                                 /* Init Memory Management Module.                           */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif


    while (1) {                                          /* Task body, always written as an infinite loop.           */
        LED_RED(LEDVal);
        LEDVal=!LEDVal;
        OSTimeDlyHMSM(0, 0, 0, 500);
        watch_cnt++;
    }
}
