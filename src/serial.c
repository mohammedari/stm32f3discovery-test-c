#include "serial.h"
#include "stm32f30x.h"

//buffer size must be a power of 2
#define SERIAL_RBUF_SIZE 256
size_t serial_get_rbuf_size() { return SERIAL_RBUF_SIZE; }
#define SERIAL_WBUF_SIZE 256
size_t serial_get_wbuf_size() { return SERIAL_WBUF_SIZE; }

static uint32_t _serial_baudrate = 115200;
uint32_t serial_get_baudrate() { return _serial_baudrate; }
void serial_set_baudrate(uint32_t baudrate) { _serial_baudrate = baudrate; }

static char _serial_rbuf[SERIAL_RBUF_SIZE];
static char _serial_wbuf[SERIAL_WBUF_SIZE];
static size_t _serial_rbuf_head = 0;
static size_t _serial_rbuf_tail = 0;
static size_t _serial_wbuf_head = 0;
static size_t _serial_wbuf_tail = 0;

void serial_init()
{
  //Interrupt Configuration
  NVIC_InitTypeDef nvic;
  nvic.NVIC_IRQChannel = USART1_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 1;
  nvic.NVIC_IRQChannelSubPriority = 0;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

  //Clock Configuration
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  //USART GPIO Connection
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_7);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);

  //GPIO Configuration
  GPIO_InitTypeDef gpio;
  gpio.GPIO_Mode = GPIO_Mode_AF;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_PuPd = GPIO_PuPd_UP;
  gpio.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOA, &gpio);
  gpio.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &gpio);

  //USART Configuration
  USART_InitTypeDef usart;
  usart.USART_BaudRate = _serial_baudrate;
  usart.USART_WordLength = USART_WordLength_8b;
  usart.USART_StopBits = USART_StopBits_1;
  usart.USART_Parity = USART_Parity_No;
  usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &usart);

  //enable USART
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);
}

size_t serial_read(char* ptr, size_t len)
{
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
  size_t received = 0;
  while (_serial_rbuf_head != _serial_rbuf_tail && received < len)
  {
    *ptr++ = _serial_rbuf[_serial_rbuf_tail++ % SERIAL_RBUF_SIZE];
    ++received;
  }
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  return received;
} 

size_t serial_write(char* ptr, size_t len)
{
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  size_t sent = 0;
  while (sent < len)
  {
    _serial_wbuf[_serial_wbuf_head++ % SERIAL_WBUF_SIZE] = *ptr++;
    ++sent;
  }
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

  return sent;
}

void serial_interrupt_handler()
{
  //Receive
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    _serial_rbuf[_serial_rbuf_head++ % SERIAL_RBUF_SIZE] = (char)USART_ReceiveData(USART1);
    return;
  }

  //Send
  if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
    if (_serial_wbuf_head != _serial_wbuf_tail)
      USART_SendData(USART1, _serial_wbuf[_serial_wbuf_tail++ % SERIAL_WBUF_SIZE]);
    else
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

    return;
  }

  //Interrupts which is not permitted raised!
  //This happens when so many packetes are received.
  //Something is wrong... reset USART module.
  //USART_Cmd(USART1, DISABLE);
  //USART_Cmd(USART1, ENABLE);
}
