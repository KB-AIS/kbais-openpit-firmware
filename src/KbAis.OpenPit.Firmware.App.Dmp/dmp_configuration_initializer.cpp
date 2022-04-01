#include "dmp_configuration_initializer.h"

const auto default_configuration_base =
    R"(
        {
            "enable_manual_begin_shift": false,
            "lcd_day_light": 15,
            "offset_local_time": 0,
            "shift_times": [
                { "shift_hours_begin": 8, "shift_hours_end": 16, "shift_minutes_begin": 0, "shift_minutes_end": 59 },
                { "shift_hours_begin": 17, "shift_hours_end": 2, "shift_minutes_begin": 0, "shift_minutes_end": 59 },
                { "shift_hours_begin": 8, "shift_hours_end": 16, "shift_minutes_begin": 0, "shift_minutes_end": 59 },
                { "shift_hours_begin": 17, "shift_hours_end": 2, "shift_minutes_begin": 0, "shift_minutes_end": 59 }
            ],
            "sound_volume_ch_1": 12,
            "sound_volume_ch_2": 12,
            "sound_volume_system": 12,
            "vehihle_id": "100",
            "version": "1.0"
        }
    )"_json;

const auto default_configuration_ethernet =
    R"(
        {
            "dns": "",
            "gateway": "10.214.1.1",
            "ip": "10.214.1.205",
            "manual_enable": true,
            "mask": "255.255.255.0",
            "version":"1.0"
        }
    )"_json;

const auto default_configuration_networking =
    R"(
        {
            "primary_message_server": -1,
            "servers": [
                {
                    "enabled": true,
                    "protocol": 0,
                    "reserve_enabled": false,
                    "reserve_server_name": "",
                    "reserve_server_port": 0,
                    "send_interval": 10,
                    "server_name": "10.73.212.191",
                    "server_port": 47653
                }
            ],
            "version": "1.0"
        }
    )"_json;

const auto default_configuration_scale =
    R"(
        {
            "Scales":[
                {"Divisor":1,"MaxScale":1000,"Percent":20,"Sens":[
                    {"Koeff":1,"Parametrs":0,"Removal":0,"Tar":[
                        {"ADC":0,"Litrs":0},{"ADC":243,"Litrs":150},
                        {"ADC":587,"Litrs":300},{"ADC":1485,"Litrs":700},
                        {"ADC":2272,"Litrs":1050},{"ADC":3274,"Litrs":1500},
                        {"ADC":3968,"Litrs":1800}],"TypeCounter":0,"TypeSens":1,"U_ADC":0},
                    {"Koeff":0,"Parametrs":0,"Removal":0,"Tar":[],"TypeCounter":0,"TypeSens":0,"U_ADC":0},
                    {"Koeff":0,"Parametrs":0,"Removal":0,"Tar":[],"TypeCounter":0,"TypeSens":0,"U_ADC":0},
                    {"Koeff":0,"Parametrs":0,"Removal":0,"Tar":[],"TypeCounter":0,"TypeSens":0,"U_ADC":0}]}],
            "version":"1.0"
        }
    )"_json;

DmpConfigurationInitializer::DmpConfigurationInitializer(
    ConfigurationManager& configuraiton_manager
) {
    configuraiton_manager.registerConfiguration(QStringLiteral("base"), default_configuration_base);
    configuraiton_manager.registerConfiguration(QStringLiteral("ethernet"), default_configuration_ethernet);
    configuraiton_manager.registerConfiguration(QStringLiteral("networking"), default_configuration_networking);
    configuraiton_manager.registerConfiguration(QStringLiteral("scale"), default_configuration_scale);
}
