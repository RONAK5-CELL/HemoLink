#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iomanip>

#include "RequestManager.h"

using namespace std;


// Check whether request ID already exists
bool RequestManager::checkRequestID(char id[])
{
    ifstream file("requests.txt");

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

        for (int i = 0; i < 6; i++)
        {
            if (!file.getline(temp, 100, (i == 5) ? '\n' : '|'))
                break;
        }
    }

    file.close();
    return false;
}


// Check whether hospital is registered
bool hospitalExists(char id[])
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

        for (int i = 0; i < 4; i++)
        {
            if (!file.getline(temp, 100, (i == 3) ? '\n' : '|'))
                break;
        }
    }

    file.close();
    return false;
}


// Add new blood request
void RequestManager::addRequest()
{
    BloodRequest r;

    cout << "\n===== GENERATE BLOOD REQUEST =====\n";

    cout << "Enter Request ID: ";
    cin >> setw(15) >> r.requestID;

    if (checkRequestID(r.requestID))
    {
        cout << "\nRequest ID already exists!\n";
        return;
    }

    cout << "Enter Hospital ID: ";
    cin >> setw(15) >> r.hospitalID;

    if (!hospitalExists(r.hospitalID))
    {
        cout << "\nHospital is not registered!\n";
        return;
    }

    cout << "Enter Blood Group: ";
    cin >> setw(5) >> r.bloodGroup;

    cout << "Enter Required Units: ";
    cin >> r.units;

    if (r.units <= 0)
    {
        cout << "\nInvalid number of units!\n";
        return;
    }

    cout << "Enter Request Date: ";
    cin >> setw(15) >> r.requestDate;

    cout << "Enter Priority (Normal/Urgent/Emergency): ";
    cin >> setw(15) >> r.priority;

    strcpy(r.status, "Pending");

    ofstream file("requests.txt", ios::app);

    if (!file)
    {
        cout << "\nUnable to open request file!\n";
        return;
    }

    file << r.requestID << "|"
         << r.hospitalID << "|"
         << r.bloodGroup << "|"
         << r.units << "|"
         << r.requestDate << "|"
         << r.priority << "|"
         << r.status << "\n";

    file.close();

    cout << "\nBlood Request Generated Successfully!\n";
}


// Display all requests
void RequestManager::displayRequests()
{
    ifstream file("requests.txt");

    if (!file)
    {
        cout << "\nNo Blood Requests Found!\n";
        return;
    }

    BloodRequest r;
    char units[10];
    int count = 0;

    cout << "\n========== BLOOD REQUESTS ==========\n";

    while (file.getline(r.requestID, 15, '|'))
    {
        if (!file.getline(r.hospitalID, 15, '|'))
            break;

        if (!file.getline(r.bloodGroup, 5, '|'))
            break;

        if (!file.getline(units, 10, '|'))
            break;

        if (!file.getline(r.requestDate, 15, '|'))
            break;

        if (!file.getline(r.priority, 15, '|'))
            break;

        if (!file.getline(r.status, 20))
            break;

        r.units = atoi(units);

        cout << "\nRequest ID: " << r.requestID;
        cout << "\nHospital ID: " << r.hospitalID;
        cout << "\nBlood Group: " << r.bloodGroup;
        cout << "\nRequired Units: " << r.units;
        cout << "\nRequest Date: " << r.requestDate;
        cout << "\nPriority: " << r.priority;
        cout << "\nStatus: " << r.status;
        cout << "\n-----------------------------------";

        count++;
    }

    if (count == 0)
        cout << "\nNo Requests Available!\n";

    file.close();
}


// Search request using ID
void RequestManager::searchRequest()
{
    ifstream file("requests.txt");

    if (!file)
    {
        cout << "\nNo Blood Requests Found!\n";
        return;
    }

    char id[15];
    char units[10];

    BloodRequest r;
    bool found = false;

    cout << "\nEnter Request ID: ";
    cin >> setw(15) >> id;

    while (file.getline(r.requestID, 15, '|'))
    {
        if (!file.getline(r.hospitalID, 15, '|'))
            break;

        if (!file.getline(r.bloodGroup, 5, '|'))
            break;

        if (!file.getline(units, 10, '|'))
            break;

        if (!file.getline(r.requestDate, 15, '|'))
            break;

        if (!file.getline(r.priority, 15, '|'))
            break;

        if (!file.getline(r.status, 20))
            break;

        r.units = atoi(units);

        if (strcmp(r.requestID, id) == 0)
        {
            cout << "\n========== REQUEST FOUND ==========\n";

            cout << "Request ID: " << r.requestID << endl;
            cout << "Hospital ID: " << r.hospitalID << endl;
            cout << "Blood Group: " << r.bloodGroup << endl;
            cout << "Required Units: " << r.units << endl;
            cout << "Request Date: " << r.requestDate << endl;
            cout << "Priority: " << r.priority << endl;
            cout << "Status: " << r.status << endl;

            found = true;
            break;
        }
    }

    if (!found)
        cout << "\nRequest Not Found!\n";

    file.close();
}


// Update request status



// Delete request
