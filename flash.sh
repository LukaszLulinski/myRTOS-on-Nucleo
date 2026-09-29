#!/bin/bash
gdb-multiarch -batch \
  -ex "target remote :3333" \
  -ex "monitor reset halt" \
  -ex "load" \
  -ex "monitor reset init" \
  -ex "continue" \
  out/myRTOS.elf
  