#ifndef DONORMANAGER_H
#define DONORMANAGER_H

#include "Donor.h"
#include <vector>

class DonorManager
{
public:
    // Existing console functions
    void addDonor();
    void displayDonors();
    void searchDonor();
    void updateDonor();
    void deleteDonor();

    // New GUI functions
    bool addDonorRecord(const Donor& d);
    std::vector<Donor> getAllDonors();
    bool findDonorByID(const char* id, Donor& result);

private:

    bool checkID(char id[]);

};

#endif