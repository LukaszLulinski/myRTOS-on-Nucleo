/*------------------------------------------------------------------------------------*/
/*!
 * \file  startup.c 
 * \brief Initializing processor
 */
/*------------------------------------------------------------------------------------*/

#include <stdint.h>

#define STACK_TOP 0x20005000

extern void main(void);
extern void systick_handler(void);
extern void PendSV_Handler(void);
extern void SVC_Handler(void);

extern uint32_t _sdata, _edata, _sidata;
extern uint32_t _sbss, _ebss;

void reset_handler(void)
{
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    dst = &_sbss;
    while (dst < &_ebss)
    {
        *dst++ = 0;
    }

    main();
    while (1);
}

void default_handler(void)
{
    while (1);
}

__attribute__((section(".vectors")))
void (*vectors[])(void) =
{
    (void (*)(void))STACK_TOP,  // 0  - stack pointer
    reset_handler,              // 1  - reset
    default_handler,            // 2  - NMI
    default_handler,            // 3  - HardFault
    default_handler,            // 4  - MemManage
    default_handler,            // 5  - BusFault
    default_handler,            // 6  - UsageFault
    0, 0, 0, 0,                 // 7-10 - reserved
    SVC_Handler,                // 11 - SVCall
    default_handler,            // 12 - DebugMon
    0,                          // 13 - reserved
    PendSV_Handler,             // 14 - PendSV
    systick_handler,            // 15 - SysTick
};
