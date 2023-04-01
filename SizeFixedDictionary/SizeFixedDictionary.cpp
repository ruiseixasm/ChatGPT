// SizeFixedDictionary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

class DeviceAddress
{
private:
    const char* deviceName;
    const int sizeName;
    char *networkAdrress; // 4 bytes

public:
    DeviceAddress(char* name, int size, char* address) : deviceName(name), sizeName(size) {
        networkAdrress = address;
    }

    ~DeviceAddress() {
        delete[] deviceName;
        deviceName = nullptr;
        delete[] networkAdrress;
        networkAdrress = nullptr;
    }

    bool sameName(char name[], int size)
    {
        if (size != sizeName) {
            return false;
        }
        for (int i = 0; i < sizeName; i++) {
            if (name[i] != deviceName[i]) {
                return false;
            }
        }
        return true;
    }
};

class DeviceDictionary5
{

};


int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
