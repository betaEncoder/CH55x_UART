# CH55x_UART
A tiny UART library for CH55x series MCU
Currently, only UART0 can be work.

## Resource
UART peripheral require Timer1 or Timer2 for generating any baudrate at MODE1 and MODE3.
In this situation, SMOD and/or bTMR_CLK are controlled by divider configration.
These bits are effect all timer clock.
When fixed baudrate it's not required.