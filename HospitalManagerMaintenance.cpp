#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <limits>

#include "HospitalManager.h"

using namespace std;

// READ HOSPITAL RECORD
static bool readHospital(string line,string &id,string &name,
                         string &city,string &phone,string &address)
{
    stringstream ss(line);

    return getline(ss,id,'|') &&
           getline(ss,name,'|') &&
           getline(ss,city,'|') &&
           getline(ss,phone,'|') &&
           getline(ss,address);
}

// REPLACE MAIN FILE SAFELY
static bool replaceHospitalFile()
{
    remove("backup.txt");

    if(rename("hospitals.txt","backup.txt")!=0)
        return false;

    if(rename("temp.txt","hospitals.txt")!=0)
    {
        rename("backup.txt","hospitals.txt");
        return false;
    }

    remove("backup.txt");
    return true;
}

// REMOVE PIPE SYMBOL FROM INPUT
static void cleanHospitalField(string &text)
{
    for(char &c:text)
        if(c=='|') c=' ';
}

// UPDATE HOSPITAL
void HospitalManager::updateHospital()
{
    string id,name,city,phone,address,line;
    bool found=false;

    cout<<"\nEnter Hospital ID to Update: ";
    cin>>id;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    ifstream file("hospitals.txt");
    ofstream temp("temp.txt");

    if(!file || !temp)
    {
        cout<<"\nUnable to open files!\n";
        return;
    }

    while(getline(file,line))
    {
        string hid,hname,hcity,hphone,haddress;

        if(!readHospital(line,hid,hname,hcity,hphone,haddress))
        {
            temp<<line<<"\n";
            continue;
        }

        if(hid==id)
        {
            found=true;

            cout<<"\nEnter Updated Details:\n";

            cout<<"Hospital Name: ";
            getline(cin,name);

            cout<<"City: ";
            getline(cin,city);

            cout<<"Phone Number: ";
            getline(cin,phone);

            cout<<"Address: ";
            getline(cin,address);

            cleanHospitalField(name);
            cleanHospitalField(city);
            cleanHospitalField(phone);
            cleanHospitalField(address);

            hname=name;
            hcity=city;
            hphone=phone;
            haddress=address;
        }

        temp<<hid<<"|"<<hname<<"|"<<hcity<<"|"
            <<hphone<<"|"<<haddress<<"\n";
    }

    file.close();
    temp.close();

    if(!found)
    {
        remove("temp.txt");
        cout<<"\nHospital Not Found!\n";
        return;
    }

    if(replaceHospitalFile())
        cout<<"\nHospital Updated Successfully!\n";
    else
    {
        remove("temp.txt");
        cout<<"\nUnable to update hospital file!\n";
    }
}

// DELETE HOSPITAL
void HospitalManager::deleteHospital()
{
    string id,line;
    bool found=false;

    cout<<"\nEnter Hospital ID to Delete: ";
    cin>>id;

    ifstream file("hospitals.txt");
    ofstream temp("temp.txt");

    if(!file || !temp)
    {
        cout<<"\nUnable to open files!\n";
        return;
    }

    while(getline(file,line))
    {
        string hid,name,city,phone,address;

        if(!readHospital(line,hid,name,city,phone,address))
        {
            temp<<line<<"\n";
            continue;
        }

        if(hid==id)
        {
            found=true;
            continue;
        }

        temp<<line<<"\n";
    }

    file.close();
    temp.close();

    if(!found)
    {
        remove("temp.txt");
        cout<<"\nHospital Not Found!\n";
        return;
    }

    if(replaceHospitalFile())
        cout<<"\nHospital Deleted Successfully!\n";
    else
    {
        remove("temp.txt");
        cout<<"\nUnable to delete hospital record!\n";
    }
}