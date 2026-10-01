# myRTOS

A lightweight preemptive RTOS kernel for ARM Cortex-M, written from scratch in C and ARM assembly. Built as an educational project to understand the internals of real-time operating systems — no HAL, no vendor libraries, no external dependencies.

---

## Features

- Priority-based preemptive scheduling. The scheduler selects the highest-priority task in `READY` state.
- PendSV context switching, with task register state saved on each task's stack.
- SysTick-driven timekeeping and task delays. `systick_init(1000u)` configures a 1 ms tick for the 25 MHz board clock.
- Tasks with relative (`task_delay`) and periodic (`task_delay_until`) delays.
- Mutexes with a FIFO list of blocked tasks.
- Counting semaphores. Blocked tasks are stored LIFO.
- Fixed-size queues built from semaphores, with up to 10 items and 8 bytes per item.
- Software timers with one-shot and cyclic modes.
- An idle task that waits using `WFI`.
- Hand-written memcpy — no libc, fully freestanding (-nostdlib -ffreestanding)

---

## Project Structure

```
myRTOS/
├── hal/
│   ├── core.h              Cortex-M registers and MPS2-AN386 clock definition
│   ├── clock.c/h             — HSE + PLL config, 72 MHz SYSCLK
│   ├── systick.c/h         SysTick setup and tick counter
│   └── uart.c/h              — USART2 (PA2/PA3, ST-Link virtual COM)
├── kernel/
│   ├── context.s           SVC startup and PendSV context switching
│   ├── mutex.c/h           Mutexes
│   ├── queue.c/h           Fixed-size queues
│   ├── scheduler.c/h       Priority scheduler and idle task
│   ├── semaphore.c/h       Counting semaphores
│   ├── task.c/h            Task control blocks, creation, and delays
│   └── timer.c/h           Software timers
├── src/
│   ├── main.c              Producer/consumer demo using a queue and UART
│   └── startup.c           Vector table and reset/fault handlers
│
├── linker.ld               stm32f103 memory layout
└── Makefile                ARM cross-compilation build
```

---

## Building and Running

**Prerequisites (Ubuntu):**

```bash
sudo apt install gcc-arm-none-eabi openocd picocom
sudo usermod -aG dialout $USER   # then log out/in
```

**Build & flash:**

```bash
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg
# separate terminal:
gdb-multiarch out/myRTOS.elf
(gdb) target remote :3333
(gdb) monitor reset halt
(gdb) load
(gdb) monitor reset init
(gdb) continue
```

**Serial output** (USART2, ST-Link virtual COM port, 115200 baud):

```bash
picocom -b 115200 /dev/ttyACM0
```

---

## Clock Configuration (Nucleo-F103RB)

Runs on HSE (8 MHz, from ST-Link) → PLL x9 → **72 MHz SYSCLK**, the F103's maximum.

```
HSE (8 MHz) → PLL ×9 → SYSCLK (72 MHz) → AHB (72 MHz)
                                            ├── APB1 /2 → 36 MHz (USART2, TIM2-7)
                                            └── APB2 /1 → 72 MHz (GPIO, USART1, TIM1)
```

Flash latency is set to 2 wait states (required above 48 MHz). See `hal/stm32f103/clock.c`.

---

## Current limitations

The current implementation has fixed task, stack, queue, and timer capacities. A task has only one `next` link for blocking lists, so it cannot safely wait on multiple synchronization objects at once. Synchronization operations do not yet provide full interrupt-safe atomicity, and semaphore wakeup currently assumes the released count is handed to the woken task.
