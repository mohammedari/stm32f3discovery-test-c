#include "timer.h"
#include "roulette.h"
#include "stm32f3_discovery.h"

static uint32_t _roulette_delay = 100;
const Led_TypeDef _roulette_sequence[] 
  = { LED3, LED5, LED7, LED9, LED10, LED8, LED6, LED4 };

void roulette_init()
{
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED7);
  STM_EVAL_LEDInit(LED8);
  STM_EVAL_LEDInit(LED9);
  STM_EVAL_LEDInit(LED10);
} 

void roulette_update()
{
  static uint32_t prev = 0;
  static uint32_t pos = 0;

  uint32_t now = timer_now();
  if (now - prev > _roulette_delay)
  {
    STM_EVAL_LEDToggle(_roulette_sequence[pos % 8]);
    ++pos;
    prev = now;
  }
}

void roulette_set_delay(uint32_t delay)
{
  _roulette_delay = delay;
}

uint32_t roulette_get_delay()
{
  return _roulette_delay;
}
