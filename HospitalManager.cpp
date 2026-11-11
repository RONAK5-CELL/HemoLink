#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <iomanip>

#include "HospitalManager.h"

using namespace std;


// Check whether hospital ID already exists
bool HospitalManager::checkHospitalID(char id[])
{
    ifstream file("hospitals.txt");

    if (!file)
        return false;

    char storedID[15];
    char temp[100];

    while (file.getline(storedID, 15, '|'))
    {
        if (strcmp(storedID, id) == 0)
        {
            file.close();
            return true;
        }

        // Skip remaining four fields
        for (int i = 0; i < 4; i++)
        {
            if (!file.getline(temp, 100, (i == 3) ? '\n' : '|'))
                break;
        }
    }

    file.close();
    return false;
}


// Register a new hospital
void HospitalManager::addHospital()
{
    Hospital h;

    cout << "\n===== REGISTER HOSPITAL =====\n";

    cout << "Enter Hospital ID: ";
    cin >> setw(15) >> h.hospitalID;

    if (checkHospitalID(h.hospitalID))
    {
        cout << "\nHospital ID already exists!\n";
        return;
    }

    cin.ignore(1000, '\n');

    cout << "Enter Hospital Name: ";
    cin.getline(h.name, 50);

    cout << "Enter City: ";
    cin.getline(h.city, 30);

    cout << "Enter Phone Number: ";
    cin >> setw(15) >> h.phone;

    cin.ignore(1000, '\n');

    cout << "Enter Address: ";
    cin.getline(h.address, 100);

    ofstream file("hospitals.txt", ios::app);

    if (!file)
    {
        cout << "\nUnable to open hospital file!\n";
        return;
    }

    file << h.hospitalID << "|"
         << h.name << "|"
         << h.city << "|"
         << h.phone << "|"
         << h.address << "\n";

    file.close();

    cout << "\nHospital Registered Successfully!\n";
}


// Display all hospitals
void HospitalManager::displayHospitals()
{
    ifstream file("hospitals.txt");

    if (!file)
    {
        cout << "\nNo Hospital Records Found!\n";
        return;
    }

    Hospital h;
    int count = 0;

    cout << "\n========== HOSPITAL RECORDS ==========\n";

    while (file.getline(h.hospitalID, 15, '|'))
    {
        if (!file.getline(h.name, 50, '|'))
            break;

        if (!file.getline(h.city, 30, '|'))
            break;

        if (!file.getline(h.phone, 15, '|'))
            break;

        if (!file.getline(h.address, 100))
            break;

        cout << "\nHospital ID: " << h.hospitalID;
        cout << "\nName: " << h.name;
        cout << "\nCity: " << h.city;
        cout << "\nPhone: " << h.phone;
        cout << "\nAddress: " << h.address;
        cout << "\n--------------------------------------";

        count++;
    }

    if (count == 0)
        cout << "\nNo Hospital Records Found!\n";

    file.close();
}


// Search hospital using ID
void HospitalManager::searchHospital()
{
    ifstream file("hospitals.txt");

    if (!file)
    {
        cout << "\nNo Hospital Records Found!\n";
        return;
    }

    char id[15];
    Hospital h;
    bool found = false;

    cout << "\nEnter Hospital ID to Search: ";
    cin >> setw(15) >> id;

    while (file.getline(h.hospitalID, 15, '|'))
    {
        if (!file.getline(h.name, 50, '|'))
            break;

        if (!file.getline(h.city, 30, '|'))
            break;

        if (!file.getline(h.phone, 15, '|'))
            break;

        if (!file.getline(h.address, 100))
            break;

        if (strcmp(h.hospitalID, id) == 0)
        {
            cout << "\n========== HOSPITAL FOUND ==========\n";

            cout << "Hospital ID: " << h.hospitalID << endl;
            cout << "Name: " << h.name << endl;
            cout << "City: " << h.city << endl;
            cout << "Phone: " << h.phone << endl;
            cout << "Address: " << h.address << endl;

            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nHospital Not Found!\n";

    file.close();
}


// Update hospital details



// Delete hospital record
