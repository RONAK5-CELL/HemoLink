#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <limits>
#include <cstring>
#include <QCoreApplication>

#include "DonorManager.h"

using namespace std;

// GET DONOR FILE PATH
static string donorPath()
{
    string path=QCoreApplication::applicationDirPath().toStdString()
                +"/donors.txt";

    ifstream f(path);
    if(f.good()) return path;

    return "donors.txt";
}

// READ DONOR RECORD
static bool readDonor(string line,Donor &d)
{
    string id,name,age,blood,phone,city,date,status;
    stringstream ss(line);

    if(!getline(ss,id,'|') ||
       !getline(ss,name,'|') ||
       !getline(ss,age,'|') ||
       !getline(ss,blood,'|') ||
       !getline(ss,phone,'|') ||
       !getline(ss,city,'|') ||
       !getline(ss,date,'|') ||
       !getline(ss,status))
        return false;

    stringstream ageInput(age);
    if(!(ageInput>>d.age)) return false;

    snprintf(d.donorID,sizeof(d.donorID),"%s",id.c_str());
    snprintf(d.name,sizeof(d.name),"%s",name.c_str());
    snprintf(d.bloodGroup,sizeof(d.bloodGroup),"%s",blood.c_str());
    snprintf(d.phone,sizeof(d.phone),"%s",phone.c_str());
    snprintf(d.city,sizeof(d.city),"%s",city.c_str());
    snprintf(d.lastDonation,sizeof(d.lastDonation),"%s",date.c_str());
    snprintf(d.availability,sizeof(d.availability),"%s",status.c_str());

    return true;
}

// REMOVE PIPE FROM INPUT
static void cleanInput(string &s)
{
    for(char &c:s)
        if(c=='|') c=' ';
}

// WRITE DONOR RECORD
static void writeDonor(ofstream &out,Donor &d)
{
    out<<d.donorID<<"|"<<d.name<<"|"<<d.age<<"|"
       <<d.bloodGroup<<"|"<<d.phone<<"|"<<d.city<<"|"
       <<d.lastDonation<<"|"<<d.availability<<"\n";
}

// SAFE FILE REPLACEMENT
static bool replaceDonorFile(string path)
{
    string backup=path+".bak";
    string temp=path+".tmp";

    remove(backup.c_str());

    if(rename(path.c_str(),backup.c_str())!=0)
        return false;

    if(rename(temp.c_str(),path.c_str())!=0)
    {
        rename(backup.c_str(),path.c_str());
        return false;
    }

    remove(backup.c_str());
    return true;
}

// UPDATE DONOR
void DonorManager::updateDonor()
{
    string path=donorPath();
    string temp=path+".tmp";
    string id,line;
    bool found=false;

    cout<<"\nEnter Donor ID to Update: ";
    cin>>id;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    ifstream file(path);
    ofstream output(temp);

    if(!file || !output)
    {
        cout<<"\nUnable to open files!\n";
        return;
    }

    while(getline(file,line))
    {
        Donor d;

        if(!readDonor(line,d))
        {
            output<<line<<"\n";
            continue;
        }

        if(id==d.donorID)
        {
            found=true;

            cout<<"\nEnter Updated Details:\n";

            cout<<"Name: ";
            string name;
            getline(cin,name);

            cout<<"Age: ";
            int age;
            if(!(cin>>age) || age<1)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                file.close();
                output.close();
                remove(temp.c_str());
                cout<<"\nInvalid Age!\n";
                return;
            }

            cin.ignore(numeric_limits<streamsize>::max(),'\n');

            cout<<"Blood Group: ";
            string blood;
            getline(cin,blood);

            cout<<"Phone: ";
            string phone;
            getline(cin,phone);

            cout<<"City: ";
            string city;
            getline(cin,city);

            cout<<"Last Donation Date: ";
            string date;
            getline(cin,date);

            cout<<"Availability (Yes/No): ";
            string status;
            getline(cin,status);

            cleanInput(name);
            cleanInput(blood);
            cleanInput(phone);
            cleanInput(city);
            cleanInput(date);
            cleanInput(status);

            snprintf(d.name,sizeof(d.name),"%s",name.c_str());
            snprintf(d.bloodGroup,sizeof(d.bloodGroup),"%s",blood.c_str());
            snprintf(d.phone,sizeof(d.phone),"%s",phone.c_str());
            snprintf(d.city,sizeof(d.city),"%s",city.c_str());
            snprintf(d.lastDonation,sizeof(d.lastDonation),"%s",date.c_str());
            snprintf(d.availability,sizeof(d.availability),"%s",status.c_str());

            d.age=age;
        }

        writeDonor(output,d);
    }

    file.close();
    output.close();

    if(!found)
    {
        remove(temp.c_str());
        cout<<"\nDonor Not Found!\n";
        return;
    }

    if(replaceDonorFile(path))
        cout<<"\nDonor Updated Successfully!\n";
    else
    {
        remove(temp.c_str());
        cout<<"\nUnable to update donor!\n";
    }
}

// DELETE DONOR
void DonorManager::deleteDonor()
{
    string path=donorPath();
    string temp=path+".tmp";
    string id,line;
    bool found=false;

    cout<<"\nEnter Donor ID to Delete: ";
    cin>>id;

    ifstream file(path);
    ofstream output(temp);

    if(!file || !output)
    {
        cout<<"\nUnable to open files!\n";
        return;
    }

    while(getline(file,line))
    {
        Donor d;

        if(!readDonor(line,d))
        {
            output<<line<<"\n";
            continue;
        }

        if(id==d.donorID)
        {
            found=true;
            continue;
        }

        writeDonor(output,d);
    }

    file.close();
    output.close();

    if(!found)
    {
        remove(temp.c_str());
        cout<<"\nDonor Not Found!\n";
        return;
    }

    if(replaceDonorFile(path))
        cout<<"\nDonor Deleted Successfully!\n";
    else
    {
        remove(temp.c_str());
        cout<<"\nUnable to delete donor!\n";
    }
}