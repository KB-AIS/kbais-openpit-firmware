from conans import ConanFile

class CarrierFirmwareConan(ConanFile):
    requires = \
        "di/1.2.0", \
        "fmt/8.1.1", \
        "nlohmann_json/3.10.4", \
        "plog/1.1.5", \
        "readerwriterqueue/1.0.5", \
        "rxcpp/4.1.0"

    generators = \
        "qmake"
