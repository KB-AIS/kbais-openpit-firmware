from conans import ConanFile

class CarrierFirmwareConan(ConanFile):
    """ Core libraries """
    requires = \
        "di/1.2.0", \
        "plog/1.1.5", \
        "rxcpp/4.1.1"

    """ Support libraries """
    requires += \
        "readerwriterqueue/1.0.5", \
        "nlohmann_json/3.10.5"

    """ Missing C++20 libraries"""
    requires += \
        "expected-lite/0.5.0", \
        "fmt/8.1.1", \
        "range-v3/0.11.0"

    generators = \
        "qmake"
