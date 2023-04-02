#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>

class RemoteDevice {
private:
    char* device_name;
    int name_size;
    //struct sockaddr_in DeviceAddress;
public:
    RemoteDevice(const char* name, const int size) {
        device_name = new char[size];
        for (int i = 0; i < size; i++) {
            device_name[i] = name[i];
        }
        name_size = size;
        std::cout << "Remote Device CONSTRUCTED! ";
        std::cout << device_name << std::endl;
    }
    ~RemoteDevice() {
        std::cout << "Remote Device DESTROYED! ";
        std::cout << device_name << std::endl;
        delete[] device_name;
    }
    char* getDeviceName() const {
        return device_name;
    }
    int getNameSize() const {
        return name_size;
    }
};

class DevicesMap {
private:
    static const int block_size = 5;
    RemoteDevice** remoteDevices;
    int map_size;
public:
    DevicesMap() {

        remoteDevices = new RemoteDevice*[block_size];
        map_size = block_size;

        for (int i = 0; i < map_size; i++) {
            remoteDevices[i] = nullptr;
        }
        std::cout << "Constructor Called" << std::endl;
    }

    ~DevicesMap() {

        for (int i = 0; i < map_size; i++) {
            if (remoteDevices[i] != nullptr) {
                delete remoteDevices[i];
            }
        }
        delete[] remoteDevices;
        std::cout << "Destructor Called" << std::endl;
    }

    bool AddDevice(RemoteDevice* remoteDevice) {
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
        RemoteDevice** _remoteDevices = new RemoteDevice*[map_size + block_size];
        _remoteDevices[0] = remoteDevice;
        for (int i = 0; i < map_size + block_size - 1; i++) {
            _remoteDevices[i + 1] = i < map_size ? remoteDevices[i] : nullptr;
        }
        map_size += block_size;
        delete[] remoteDevices;
        remoteDevices = _remoteDevices;
        return true;
    }

    bool RemoveDevice(const char* device_name, const int &name_size) {

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

            RemoteDevice** _remoteDevices = new RemoteDevice*[new_size];

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

    RemoteDevice* GetDevice(const char* device_name, const int &name_size) {

        return nullptr;
    }

};

int main(int argc, char* argv[]) {

    DevicesMap* devices_map = new DevicesMap();

    char firstName[] = "Player01"; // size 9
    int name_size = sizeof firstName;
    int totalDevices = 35;

    for (int i = 0; i < totalDevices; i++) {
        firstName[6] = 48 + ((i+1)/10); // ascii code, 48 is equal to 0
        firstName[7] = 48 + ((i + 1) %10);

        RemoteDevice* remoteDevice = new RemoteDevice(firstName, sizeof firstName);

        devices_map->AddDevice(remoteDevice);
    }

    delete devices_map;
    return 0;
}