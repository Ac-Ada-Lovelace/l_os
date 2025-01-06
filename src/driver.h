#ifndef __DRIVER_H
#define __DRIVER_H

void printf(const char*);
class Driver
{
  public:
    Driver();
    ~Driver();
    void Activate();
    int Reset();
    void Deactive();

    class DriverManager
    {
      private:
        Driver* drivers[255];
        int numDrivers;

      public:
        DriverManager();
        void AddDriver(Driver* drv);

        void ActivateAll();
    };
};

#endif  // !__DRIVER_H
#define __DRIVER_H
