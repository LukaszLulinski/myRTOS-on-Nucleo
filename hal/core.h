/*------------------------------------------------------------------------------------*/
/*!
 * \file  core.h 
 * \brief Core definitions
 */
/*------------------------------------------------------------------------------------*/

#ifndef CORE_H
#define CORE_H

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */
/* Interrupt control and state register */
#define ICSR (*(volatile uint32_t*)0xE000ED04)

/* Registers SysTick */
#define SYST_CSR  (*(volatile uint32_t *)0xE000E010) // control and status
#define SYST_RVR  (*(volatile uint32_t *)0xE000E014) // reload value
#define SYST_CVR  (*(volatile uint32_t *)0xE000E018) // current value

/* Frequency for f103 */
#define SYSTEM_CLOCK 72000000  // 72 MHz

#define RCC_CR     (*(volatile uint32_t *)0x40021000)
#define RCC_CFGR   (*(volatile uint32_t *)0x40021004)
#define FLASH_ACR  (*(volatile uint32_t *)0x40022000)

/* Peripherials relevant addresses */
#define RCC_APB2ENR  (*(volatile uint32_t *)0x40021018)
#define RCC_APB1ENR  (*(volatile uint32_t *)0x4002101C)
#define GPIOA_CRL    (*(volatile uint32_t *)0x40010800)
#define GPIOA_ODR    (*(volatile uint32_t *)0x4001080C)

#define USART2_SR    (*(volatile uint32_t *)0x40004400)
#define USART2_DR    (*(volatile uint32_t *)0x40004404)
#define USART2_BRR   (*(volatile uint32_t *)0x40004408)
#define USART2_CR1   (*(volatile uint32_t *)0x4000440C)

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */

#endif /* CORE_H */
