#include "DeviceDataReader.h"

#include <iostream>
#include <algorithm>
#include <sstream>

#include "StringHelper.h"

DeviceData::DeviceData(/* args */)
{
}

DeviceData::~DeviceData()
{
}

void DeviceData::fillData(const std::string &line)
{
    if (!line.empty())
    {
        std::string line_data = line.substr(3);
        switch (line.front())
        {
        case 'I':
        {
            auto result = getPairs(line_data);
            bus = std::stoi(result["Bus"], 0, 16);
            vendor = std::stoi(result["Vendor"], 0, 16);
            product = std::stoi(result["Product"], 0, 16);
            version = std::stoi(result["Version"], 0, 16);
            break;
        }
        case 'N':
        {
            auto name_pair = getPair(line_data);
            if (name_pair.has_value())
                name = name_pair.value().second.substr(1, name_pair.value().second.size() - 2);
            break;
        }

        case 'P':
        {
            auto phys_pair = getPair(line_data);
            if (phys_pair.has_value())
                phys = phys_pair.value().second;
            break;
        }

        case 'S':
        {
            auto sysfs_pair = getPair(line_data);
            if (sysfs_pair.has_value())
                sysfs = sysfs_pair.value().second;
            break;
        }

        case 'U':
        {
            auto unique_pair = getPair(line_data);
            if (unique_pair.has_value())
                unique = unique_pair.value().second;
            break;
        }

        case 'H':
        {
            auto handlers_pair = getPair(line_data);
            if (handlers_pair.has_value())
            {
                std::stringstream ss{handlers_pair.value().second};
                std::string substring;
                while (std::getline(ss, substring, ' '))
                {
                    handlers.push_back(substring);
                    auto it = substring.find("event");
                    if (it != std::string::npos)
                        event_number = std::stoi(substring.substr(it + 5)); // 5 = "event".size()
                    if (substring.find("kbd") != std::string::npos)
                        is_keyboard = true;
                    if (substring.find("mouse") != std::string::npos)
                        is_mouse = true;
                }
            }
            break;
        }

        case 'B':
        {
            auto bitmap_pair = getPair(line_data);
            if (bitmap_pair.has_value())
                bitmaps.push_back(bitmap_pair.value());
            break;
        }

        default:
            break;
        }
    }
}

int DeviceData::getEventNumber()
{
    return event_number;
}

const std::string &DeviceData::getName()
{
    return name;
}

bool DeviceData::isKeyboard()
{
    return is_keyboard;
}

bool DeviceData::isMouse()
{
    return is_mouse;
}

DeviceDataReader::DeviceDataReader(/* args */)
{
    std::ifstream fs = std::ifstream(devices_path.c_str(), std::ios::in);
    if (fs.is_open())
    {
        std::string line;
        DeviceData data;
        while (getline(fs, line))
        {
            if (!line.empty())
            {
                data.fillData(line);
            }
            else
            {
                devices.emplace(data.getName(), data);
                if (data.isKeyboard())
                    keyboard_names.push_back(data.getName());
                data = DeviceData();
            }
        }
    }
}

DeviceDataReader::~DeviceDataReader()
{
}

int DeviceDataReader::getEventNumber(const std::string &device_name)
{
    return devices[device_name].getEventNumber();
}

std::vector<std::string> &DeviceDataReader::getKeyboards()
{
    return keyboard_names;
}