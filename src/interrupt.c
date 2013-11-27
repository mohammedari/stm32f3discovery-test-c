#include <stdbool.h>
#include "timer.h"
#include "serial.h"

void NMI_Handler()
{
}

void HardFault_Handler()
{
  while(true);
}

void MemManage_Handler()
{
  while(true);
}

void BusFault_Handler()
{
  while(true);
}

void UsageFault_Handler()
{
  while(true);
}

void SVC_Handler()
{
}

void DebugMon_Handler()
{
}

void PendSV_Handler()
{
}

void SysTick_Handler()
{
  timer_tick();
}

void USART1_IRQHandler()
{
  serial_interrupt_handler();
}
