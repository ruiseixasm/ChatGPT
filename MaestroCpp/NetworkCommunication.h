#ifndef NETWORK_COMMUNICATION_H
#define NETWORK_COMMUNICATION_H

#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>


class RemoteDevice {
private:
    char* device_name;
    int name_size;
    struct sockaddr_in device_address;
public:
    RemoteDevice(const char* name, const int size, sockaddr_in address);
    ~RemoteDevice();

    char* getDeviceName() const;
    int getNameSize() const;
    sockaddr_in getDeviceAddress();
    void setDeviceAddress(sockaddr_in address);
};

class DevicesMap {
private:
    static const int block_size = 5;
    RemoteDevice** remoteDevices;
    int map_size;
public:
    DevicesMap();
    ~DevicesMap();

    bool AddDevice(RemoteDevice* remoteDevice);
    bool RemoveDevice(const char* device_name, const int& name_size);
    RemoteDevice* GetDevice(const char* device_name, const int& name_size);
};

#endif /* NETWORK_COMMUNICATION_H */