#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include "timer.h"
#include "roulette.h"
#include "serial.h"

#define BUF_SIZE 256

int main()
{
  timer_init();

  roulette_init();
  roulette_set_delay(100);

  serial_set_baudrate(115200);
  serial_init();

  char buf[BUF_SIZE];
  while (true)
  {
    size_t n = read(STDIN_FILENO, buf, BUF_SIZE);
    write(STDOUT_FILENO, buf, n);

    roulette_update();
    timer_wait_ms(10);
  }

  return 0;
}
