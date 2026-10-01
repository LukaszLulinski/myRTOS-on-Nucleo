
CC      = arm-none-eabi-gcc

CFLAGS  = -mcpu=cortex-m3 -mthumb -nostdlib -ffreestanding -O0 -g \
		  -I./hal \
		  -I./kernel

LDFLAGS = -T linker.ld -nostdlib -Wl,-Map=out/myRTOS.map

TARGET  = out/myRTOS.elf

SRCS    = src/main.c 		\
	      src/startup.c 	\
		  kernel/systick.c

all: $(TARGET)

$(TARGET): $(SRCS) | out
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET) out/myRTOS.map

out:
	mkdir -p out
