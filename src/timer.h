#pragma once
#include <stdint.h>

void timer_init();
void timer_wait_ms(uint32_t delay);
void timer_tick();
uint32_t timer_now();
