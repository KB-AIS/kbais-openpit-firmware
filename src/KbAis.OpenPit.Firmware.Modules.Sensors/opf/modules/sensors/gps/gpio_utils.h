#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

class gpio_utils {

public:   
    gpio_utils() = delete;

    enum class PinDirection { In, Out };

    static void export_pin(int pin);

    static void setup_dir(int pin, PinDirection dir);

    static void setup_val(int pin, bool val);

};

#endif // GPIO_UTILS_H
