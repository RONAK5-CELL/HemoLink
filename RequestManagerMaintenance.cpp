#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iomanip>

#include "RequestManager.h"

using namespace std;


// Check whether request ID already exists

void RequestManager::updateRequestStatus()
{
    char id[15];
    char units[10];

    BloodRequest r;
    bool found = false;

    cout << "\nEnter Request ID: ";
    cin >> setw(15) >> id;

    if (!checkRequestID(id))
    {
        cout << "\nRequest Not Found!\n";
        return;
    }

    ifstream file("requests.txt");
    ofstream temp("temp.txt");

    if (!file || !temp)
    {
        cout << "\nUnable to open files!\n";
        return;
    }

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

        if (strcmp(r.requestID, id) == 0)
        {
            found = true;

            cout << "\nCurrent Status: " << r.status;

            cout << "\nEnter New Status (Pending/Approved/Fulfilled/Rejected): ";
            cin >> setw(20) >> r.status;
        }

        temp << r.requestID << "|"
             << r.hospitalID << "|"
             << r.bloodGroup << "|"
             << units << "|"
             << r.requestDate << "|"
             << r.priority << "|"
             << r.status << "\n";
    }

    file.close();
    temp.close();

    if (found)
    {
        if (rename("requests.txt", "backup.txt") != 0)
        {
            cout << "\nUnable to update request!\n";
            remove("temp.txt");
            return;
        }

        if (rename("temp.txt", "requests.txt") != 0)
        {
            rename("backup.txt", "requests.txt");
            cout << "\nUnable to update request!\n";
            return;
        }

        remove("backup.txt");

        cout << "\nRequest Status Updated Successfully!\n";
    }
    else
    {
        remove("temp.txt");
        cout << "\nRequest Not Found!\n";
    }
}
void RequestManager::deleteRequest()
{
    char id[15];
    char units[10];

    BloodRequest r;
    bool found = false;

    cout << "\nEnter Request ID to Delete: ";
    cin >> setw(15) >> id;

    if (!checkRequestID(id))
    {
        cout << "\nRequest Not Found!\n";
        return;
    }

    ifstream file("requests.txt");
    ofstream temp("temp.txt");

    if (!file || !temp)
    {
        cout << "\nUnable to open files!\n";
        return;
    }

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

        if (strcmp(r.requestID, id) == 0)
        {
            found = true;
            continue;
        }

        temp << r.requestID << "|"
             << r.hospitalID << "|"
             << r.bloodGroup << "|"
             << units << "|"
             << r.requestDate << "|"
             << r.priority << "|"
             << r.status << "\n";
    }

    file.close();
    temp.close();

    if (found)
    {
        if (rename("requests.txt", "backup.txt") != 0)
        {
            cout << "\nUnable to delete request!\n";
            remove("temp.txt");
            return;
        }

        if (rename("temp.txt", "requests.txt") != 0)
        {
            rename("backup.txt", "requests.txt");
            cout << "\nUnable to delete request!\n";
            return;
        }

        remove("backup.txt");

        cout << "\nRequest Deleted Successfully!\n";
    }
    else
    {
        remove("temp.txt");
        cout << "\nRequest Not Found!\n";
    }
}
