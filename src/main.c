/*------------------------------------------------------------------------------------*/
/*!
 * \file  main.c 
 * \brief main component
 */
/*------------------------------------------------------------------------------------*/

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Includes                                                                           */
#include "core.h"
#include "systick.h"
#include "scheduler.h"
#include "queue.h"

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Defines                                                                            */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Type definitions                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global const                                                                */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static global variables                                                            */
static queue_t shared_queue;

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global variables                                                                   */

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions declarations                                                      */
static void uart_init(void);
static void uart_print(const char *msg);
static void uart_print_uint(uint32_t n);
static void producer(void);
static void consumer(void);
static void clock_init(void);

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Global functions                                                                   */
void main(void)
{
    clock_init();

    RCC_APB2ENR |= (1 << 2);
    
    uart_init();
    GPIOA_CRL   &= ~(0xF << 20);
    GPIOA_CRL   |=  (0x2 << 20);

    task_create(producer, 1u, 128u);
    task_create(consumer, 1u, 128u);

    queue_init(&shared_queue, sizeof(uint32_t));

    uart_print("Program started\n");

    systick_init(1000);  // Initialize SysTick with 1 kHz

    /*! NOTE: Must be called after creating at least one task */
    scheduler_init();

    while (1)
    {
        // do nothing, everything is handled by tasks and interrupts
    }
}

/*————————————————————————————————————————————————————————————————————————————————————*/
/* Static functions definitions                                                       */
static void uart_init(void)
{
    RCC_APB1ENR |= (1 << 17);  // USART2EN

    GPIOA_CRL &= ~(0xF << 8);
    GPIOA_CRL |=  (0xA << 8);

    USART2_BRR = 313;  // approximation, 36MHz / 115200 ≈ 313.0

    USART2_CR1 |= (1 << 13) | (1 << 3);
}

static void uart_print(const char *msg)
{
    while (*msg)
    {
        while (!(USART2_SR & (1 << 7)));
        USART2_DR = *msg++;
    }
}

static void uart_print_uint(uint32_t n)
{
    char buf[12];
    int i = 0;

    if (n == 0) { uart_print("0"); return; }

    while (n > 0)
    {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    /* flip */
    for (int j = i - 1; j >= 0; j--)
    {
        while (!(USART2_SR & (1 << 7)));
        USART2_DR = buf[j];
    }
}


static void producer(void)
{
    uint32_t last_wake = systick_get_tick();
    uint32_t shared_counter = 0;
    while (1)
    {
        shared_counter++;
        queue_push(&shared_queue, &shared_counter);

        uart_print("produced: ");
        uart_print_uint(shared_counter);
        uart_print("\r\n");

        task_delay_until(&last_wake, 500);
    }
}

static void consumer(void)
{
    uint32_t shared_counter = 0;
    while (1)
    {
        queue_pop(&shared_queue, &shared_counter);
        
        uart_print("consumed: ");
        uart_print_uint(shared_counter);
        uart_print("\r\n");
    }
}

static void clock_init(void)
{
    FLASH_ACR |= (2 << 0);  // 2 wait states for 48MHz < SYSCLK <= 72MHz

    RCC_CR |= (1 << 16);           // HSEON
    while (!(RCC_CR & (1 << 17))); // wait for HSERDY

    RCC_CFGR |= (1 << 16);   // PLLSRC = HSE
    RCC_CFGR |= (7 << 18);   // PLLMUL = x9 (value 7 in 4-bits field = x9)

    RCC_CFGR |= (4 << 8);    // APB1 prescaler = /2 (bits PPRE1)
    // APB2 zostaje /1 (domyślnie)

    RCC_CR |= (1 << 24);           // PLLON
    while (!(RCC_CR & (1 << 25))); // wait for PLLRDY

    RCC_CFGR &= ~(3 << 0);
    RCC_CFGR |= (2 << 0);          // SW = PLL
    while (((RCC_CFGR >> 2) & 3) != 2); // wait for SWS to show PLL as active source
}
