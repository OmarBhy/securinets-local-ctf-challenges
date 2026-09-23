#include "uart.h"
#include "systick.h"
#include "cli.h"

int main(void) {
    uart_init();

    // starting point; tune if needed
    systick_init(12000);

    cli_run();
    return 0;
}
