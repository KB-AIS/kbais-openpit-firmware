#ifndef GPIOUTILS_H
#define GPIOUTILS_H

class GpioUtils {

public:   
    GpioUtils() = delete;

    enum class PinDirection { In, Out };

    static void export_pin(int pin);

    static void setup_dir(int pin, PinDirection dir);

    static void setup_val(int pin, bool val);

};

#endif // GPIOUTILS_H
