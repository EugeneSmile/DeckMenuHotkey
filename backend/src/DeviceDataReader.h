#ifndef DEVICEDATAREADER_H_GUARD
#define DEVICEDATAREADER_H_GUARD

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstdint>

class DeviceData
{
private:
    std::string name;
    uint16_t bus;
    uint16_t vendor;
    uint16_t product;
    uint16_t version;
    std::string phys;
    std::string sysfs;
    std::string unique;
    std::vector<std::string> handlers;
    std::vector<std::pair<std::string, std::string>> bitmaps;
    int event_number{-1};
    bool is_keyboard{false};
    bool is_mouse{false};

public:
    void fillData(const std::string &line);
    int getEventNumber();
    const std::string &getName();
    bool isKeyboard();
    bool isMouse();
};

class DeviceDataReader
{
private:
    const std::string devices_path = "/proc/bus/input/devices";
    std::unordered_map<std::string, DeviceData> devices;
    std::vector<std::string> device_names;

public:
    int getEventNumber(const std::string &device_name);
    std::vector<std::string> &getDeviceNames();
    void reloadDevices();
};

#endif