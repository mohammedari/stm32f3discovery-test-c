#pragma once

#include <stdint.h>
#include <stddef.h>

size_t serial_get_rbuf_size();
size_t serial_get_wbuf_size();
uint32_t serial_get_baudrate();
void serial_set_baudrate(uint32_t baudrate);

void serial_init();
size_t serial_read(char* ptr, size_t len);
size_t serial_write(char* ptr, size_t len);
void serial_interrupt_handler();
