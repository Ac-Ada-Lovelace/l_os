
#include "driver.h"

Driver::Driver()
{
    // Initialize the driver
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

void Driver::Deactive()
{
    // Deactivate the driver
}

Driver::DriverManager::DriverManager()
{
    printf("DriverManager constructor\n");
    numDrivers = 0;
}

void Driver::DriverManager::AddDriver(Driver* drv)
{
    drivers[numDrivers] = drv;
    numDrivers++;
}

void Driver::DriverManager::ActivateAll()
{
    for (int i = 0; i < numDrivers; i++)
    {
        drivers[i]->Activate();
    }
}
