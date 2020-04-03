#pragma once


class GpioHandler
{
    public:
        GpioHandler();
    private:
        static void isr();
};

void wakeUpBlink();
