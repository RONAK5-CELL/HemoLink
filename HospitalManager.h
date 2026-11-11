#ifndef HOSPITALMANAGER_H
#define HOSPITALMANAGER_H

#include "Hospital.h"

class HospitalManager
{
public:
    void addHospital();
    void displayHospitals();
    void searchHospital();
    void updateHospital();
    void deleteHospital();

private:
    bool checkHospitalID(char id[]);
};

#endif