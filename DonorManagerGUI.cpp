#include <fstream>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include "Donormanager.h"

using namespace std;

bool DonorManager::addDonorRecord(const Donor& d)
{
    char id[15] = {};

    strncpy(id, d.donorID, sizeof(id) - 1);

    if (strlen(id) == 0 || checkID(id))
        return false;

    ofstream file("donors.txt", ios::app);

    if (!file)
        return false;

    file << d.donorID << "|"
         << d.name << "|"
         << d.age << "|"
         << d.bloodGroup << "|"
         << d.phone << "|"
         << d.city << "|"
         << d.lastDonation << "|"
         << d.availability << "\n";

    return file.good();
}

vector<Donor> DonorManager::getAllDonors()
{
    vector<Donor> donors;

    ifstream file("donors.txt");

    if (!file)
        return donors;

    string line;

    while (getline(file, line))
    {
        string fields[8];
        stringstream ss(line);

        bool valid = true;

        for (int i = 0; i < 7; i++)
        {
            if (!getline(ss, fields[i], '|'))
            {
                valid = false;
                break;
            }
        }

        if (!valid || !getline(ss, fields[7]))
            continue;

        Donor d{};

        strncpy(
            d.donorID,
            fields[0].c_str(),
            sizeof(d.donorID) - 1
        );

        strncpy(
            d.name,
            fields[1].c_str(),
            sizeof(d.name) - 1
        );

        try
        {
            d.age = stoi(fields[2]);
        }
        catch (...)
        {
            continue;
        }

        strncpy(
            d.bloodGroup,
            fields[3].c_str(),
            sizeof(d.bloodGroup) - 1
        );

        strncpy(
            d.phone,
            fields[4].c_str(),
            sizeof(d.phone) - 1
        );

        strncpy(
            d.city,
            fields[5].c_str(),
            sizeof(d.city) - 1
        );

        strncpy(
            d.lastDonation,
            fields[6].c_str(),
            sizeof(d.lastDonation) - 1
        );

        strncpy(
            d.availability,
            fields[7].c_str(),
            sizeof(d.availability) - 1
        );

        donors.push_back(d);
    }

    return donors;
}

bool DonorManager::findDonorByID(
    const char* id,
    Donor& result)
{
    vector<Donor> donors = getAllDonors();

    for (const Donor& d : donors)
    {
        if (strcmp(d.donorID, id) == 0)
        {
            result = d;
            return true;
        }
    }

    return false;
}
