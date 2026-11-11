#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "Donormanager.h"

using namespace std;


// CHECK DONOR ID
bool DonorManager::checkID(char id[])
{
    ifstream file("donors.txt");

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

        for (int i = 0; i < 7; i++)
        {
            file.getline(
                temp,
                100,
                (i == 6) ? '\n' : '|'
            );
        }
    }

    file.close();
    return false;
}


// ADD DONOR - CONSOLE
void DonorManager::addDonor()
{
    Donor d{};

    cout << "\n===== REGISTER NEW DONOR =====\n";

    cout << "Enter Donor ID: ";
    cin >> setw(15) >> d.donorID;

    if (checkID(d.donorID))
    {
        cout << "\nDonor ID already exists!\n";
        return;
    }

    cin.ignore(1000, '\n');

    cout << "Enter Full Name: ";
    cin.getline(d.name, 50);

    cout << "Enter Age: ";
    cin >> d.age;

    cout << "Enter Blood Group: ";
    cin >> setw(5) >> d.bloodGroup;

    cout << "Enter Phone Number: ";
    cin >> setw(15) >> d.phone;

    cin.ignore(1000, '\n');

    cout << "Enter City: ";
    cin.getline(d.city, 30);

    cout << "Enter Last Donation Date: ";
    cin >> setw(15) >> d.lastDonation;

    cout << "Enter Availability (Yes/No): ";
    cin >> setw(10) >> d.availability;

    ofstream file("donors.txt", ios::app);

    if (!file)
    {
        cout << "\nUnable to open donor file!\n";
        return;
    }

    file << d.donorID << "|"
         << d.name << "|"
         << d.age << "|"
         << d.bloodGroup << "|"
         << d.phone << "|"
         << d.city << "|"
         << d.lastDonation << "|"
         << d.availability << "\n";

    file.close();

    cout << "\nDonor Registered Successfully!\n";
}


// DISPLAY ALL DONORS
void DonorManager::displayDonors()
{
    ifstream file("donors.txt");

    if (!file)
    {
        cout << "\nNo donor records found!\n";
        return;
    }

    cout << "\n========== DONOR RECORDS ==========\n";

    Donor d{};
    char age[10];
    int count = 0;

    while (file.getline(d.donorID, 15, '|'))
    {
        if (!file.getline(d.name, 50, '|'))
            break;

        if (!file.getline(age, 10, '|'))
            break;

        if (!file.getline(d.bloodGroup, 5, '|'))
            break;

        if (!file.getline(d.phone, 15, '|'))
            break;

        if (!file.getline(d.city, 30, '|'))
            break;

        if (!file.getline(d.lastDonation, 15, '|'))
            break;

        if (!file.getline(d.availability, 10))
            break;

        d.age = atoi(age);

        cout << "\nDonor ID: " << d.donorID;
        cout << "\nName: " << d.name;
        cout << "\nAge: " << d.age;
        cout << "\nBlood Group: " << d.bloodGroup;
        cout << "\nPhone: " << d.phone;
        cout << "\nCity: " << d.city;
        cout << "\nLast Donation: " << d.lastDonation;
        cout << "\nAvailability: " << d.availability;
        cout << "\n-----------------------------------";

        count++;
    }

    if (count == 0)
        cout << "\nNo donor records available!\n";

    file.close();
}


// SEARCH DONOR - CONSOLE
void DonorManager::searchDonor()
{
    ifstream file("donors.txt");

    if (!file)
    {
        cout << "\nNo donor records found!\n";
        return;
    }

    char id[15];
    char age[10];

    Donor d{};
    bool found = false;

    cout << "\nEnter Donor ID to Search: ";
    cin >> setw(15) >> id;

    while (file.getline(d.donorID, 15, '|'))
    {
        if (!file.getline(d.name, 50, '|'))
            break;

        if (!file.getline(age, 10, '|'))
            break;

        if (!file.getline(d.bloodGroup, 5, '|'))
            break;

        if (!file.getline(d.phone, 15, '|'))
            break;

        if (!file.getline(d.city, 30, '|'))
            break;

        if (!file.getline(d.lastDonation, 15, '|'))
            break;

        if (!file.getline(d.availability, 10))
            break;

        d.age = atoi(age);

        if (strcmp(d.donorID, id) == 0)
        {
            cout << "\n========== DONOR FOUND ==========\n";

            cout << "Donor ID: " << d.donorID << endl;
            cout << "Name: " << d.name << endl;
            cout << "Age: " << d.age << endl;
            cout << "Blood Group: " << d.bloodGroup << endl;
            cout << "Phone: " << d.phone << endl;
            cout << "City: " << d.city << endl;
            cout << "Last Donation: " << d.lastDonation << endl;
            cout << "Availability: " << d.availability << endl;

            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nDonor Not Found!\n";

    file.close();
}


// UPDATE DONOR



// DELETE DONOR



// ======================================
// QT GUI FUNCTIONS
// ======================================


// ADD DONOR FROM GUI



// GET ALL DONORS FOR GUI



// FIND DONOR BY ID FOR GUI
