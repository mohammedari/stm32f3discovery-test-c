#include "timer.h"
#include "stm32f30x.h"

static uint32_t timer_count;
static uint32_t timer_time;

void timer_init()
{
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);
  NVIC_SetPriority(SysTick_IRQn, 0);
}

void timer_wait_ms(uint32_t delay)
{
  timer_count = delay;
  while(timer_count > 0);
}

void timer_tick()
{
  if (timer_count > 0)
    --timer_count;

  ++timer_time;
}

uint32_t timer_now()
{
  return timer_time;
}
