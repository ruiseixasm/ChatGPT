#include "NetworkCommunication.h"


RemoteDevice::RemoteDevice(const char* name, const int size, sockaddr_in address) {
    device_name = new char[size];
    for (int i = 0; i < size; i++) {
        device_name[i] = name[i];
    }
    name_size = size;
    device_address = address;
    std::cout << "Remote Device CONSTRUCTED! ";
    std::cout << device_name << std::endl;
}

RemoteDevice::~RemoteDevice() {
    std::cout << "Remote Device DESTROYED! ";
    std::cout << device_name << std::endl;
    delete[] device_name;
}

char* RemoteDevice::getDeviceName() const {
    return device_name;
}

int RemoteDevice::getNameSize() const {
    return name_size;
}

sockaddr_in RemoteDevice::getDeviceAddress() {
    return device_address;
}

void RemoteDevice::setDeviceAddress(sockaddr_in address) {
    device_address = address;
}


DevicesMap::DevicesMap() {

    remoteDevices = new RemoteDevice * [block_size];
    map_size = block_size;

    for (int i = 0; i < map_size; i++) {
        remoteDevices[i] = nullptr;
    }
    std::cout << "Constructor Called" << std::endl;
}

DevicesMap::~DevicesMap() {

    for (int i = 0; i < map_size; i++) {
        if (remoteDevices[i] != nullptr) {
            delete remoteDevices[i];
        }
    }
    delete[] remoteDevices;
    std::cout << "Destructor Called" << std::endl;
}

bool DevicesMap::AddDevice(RemoteDevice* remoteDevice) {
    char* device_name = remoteDevice->getDeviceName();
    int name_size = remoteDevice->getNameSize();
    if (GetDevice(device_name, name_size) != nullptr) {
        return false;
    }
    for (int i = 0; i < map_size; i++) {
        if (remoteDevices[i] == nullptr) { // found an empty space
            remoteDevices[i] = remoteDevice;
            return true;
        }
    }
    RemoteDevice** _remoteDevices = new RemoteDevice * [map_size + block_size];
    _remoteDevices[0] = remoteDevice;
    for (int i = 0; i < map_size + block_size - 1; i++) {
        _remoteDevices[i + 1] = i < map_size ? remoteDevices[i] : nullptr;
    }
    map_size += block_size;
    delete[] remoteDevices;
    remoteDevices = _remoteDevices;
    return true;
}

bool DevicesMap::RemoveDevice(const char* device_name, const int& name_size) {

    RemoteDevice* remoteDevice = GetDevice(device_name, name_size);
    if (remoteDevice == nullptr) {
        return false;
    }

    // count empty slots (remove times 5 if times 5 are empty)
    int total_empty_slots = 1; // counts with the one that will be removed
    for (int i = 0; i < map_size; i++) {
        if (remoteDevices[i] == nullptr) { // found an empty space
            total_empty_slots++;
        }
        else if (remoteDevices[i] == remoteDevice) { // compare pointers (same device equates to same pointer)
            delete remoteDevices[i];
            remoteDevices[i] = nullptr;
        }
    }
    int excess_blocks = total_empty_slots / block_size;
    int new_size = map_size - excess_blocks * block_size;
    if (excess_blocks > 0 && new_size > 0) {

        RemoteDevice** _remoteDevices = new RemoteDevice * [new_size];

        int j = 0;
        for (int i = 0; i < map_size; i++) {
            if (remoteDevices[i] != nullptr) {
                _remoteDevices[j] = remoteDevices[i];
                j++;
            }
        }
        map_size = new_size;
        delete[] remoteDevices;
        remoteDevices = _remoteDevices;
    }

    return true;
}

RemoteDevice* DevicesMap::GetDevice(const char* device_name, const int& name_size) {

    for (int i = 0; i < map_size; i++) {

        if (remoteDevices[i] != nullptr && remoteDevices[i]->getNameSize() == name_size) {
            char* _device_name = remoteDevices[i]->getDeviceName();
            for (int j = 0; j < name_size; j++) {
                if (device_name[j] != _device_name[j]) {
                    break;
                }
                if (j == name_size - 1) {
                    return remoteDevices[i];
                }
            }
        }
    }
    return nullptr;
}