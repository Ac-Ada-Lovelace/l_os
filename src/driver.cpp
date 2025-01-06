
#include "driver.h"

Driver::Driver()
{
    // Initialize the driverdirpp
}

Driver::~Driver()
{
    // Deinitialize the driver
}

void Driver::Activate()
{
    // Activate the driver
}

int Driver::Reset()
{
    // Reset the driver
    return 0;
}

void Driver::Deactivate()
{
    // Deactivate the driver
}

DriverManager::DriverManager()
{
    printf("DriverManager constructor\n");
    numDrivers = 0;
}

void DriverManager::AddDriver(Driver* drv)
{
    drivers[numDrivers] = drv;
    numDrivers++;
}

void DriverManager::ActivateAll()
{
    for (int i = 0; i < numDrivers; i++)
    {
        drivers[i]->Activate();
    }
}
