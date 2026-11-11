#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include "Inventory.h"
#include <QCoreApplication>

using namespace std;

// DATA FILE PATH
static string stockPath()
{
    if(QCoreApplication::instance())
        return QCoreApplication::applicationDirPath().toStdString()+"/blood_stock.txt";

    return "blood_stock.txt";
}

// VALID BLOOD GROUP
static bool validGroup(string g)
{
    string groups[]={"A+","A-","B+","B-","AB+","AB-","O+","O-"};

    for(string s:groups)
        if(g==s) return true;

    return false;
}

// READ STOCK
static vector<BloodStock> readStock()
{
    vector<BloodStock> stocks;
    ifstream file(stockPath());

    string group,unitsText;

    while(getline(file,group,'|') && getline(file,unitsText))
    {
        int units=atoi(unitsText.c_str());
        if(!validGroup(group) || units<0) continue;

        bool found=false;

        for(auto &s:stocks)
        {
            if(group==s.bloodGroup)
            {
                s.units+=units;
                found=true;
                break;
            }
        }

        if(!found)
        {
            BloodStock s{};
            strncpy(s.bloodGroup,group.c_str(),sizeof(s.bloodGroup)-1);
            s.units=units;
            stocks.push_back(s);
        }
    }

    return stocks;
}

// SAVE STOCK
static bool saveStock(vector<BloodStock> stocks)
{
    string path=stockPath();
    string temp=path+".tmp";
    string backup=path+".bak";

    ofstream file(temp);

    if(!file) return false;

    for(const auto &s:stocks)
        file<<s.bloodGroup<<"|"<<s.units<<"\n";

    file.close();

    if(!file) return false;

    remove(backup.c_str());

    if(rename(path.c_str(),backup.c_str())==0)
    {
        if(rename(temp.c_str(),path.c_str())!=0)
        {
            rename(backup.c_str(),path.c_str());
            remove(temp.c_str());
            return false;
        }

        remove(backup.c_str());
    }
    else if(rename(temp.c_str(),path.c_str())!=0)
        return false;

    return true;
}

// ADD STOCK
void InventoryManager::addStock()
{
    string group;
    int units;

    cout<<"\n===== ADD BLOOD STOCK =====\n";
    cout<<"Enter Blood Group: ";
    cin>>group;

    cout<<"Enter Number of Units: ";
    cin>>units;

    if(!addStockRecord(group.c_str(),units))
        cout<<"\nInvalid details or unable to save stock!\n";
    else
        cout<<"\nBlood Stock Added Successfully!\n";
}

// DISPLAY STOCK
void InventoryManager::displayStock()
{
    vector<BloodStock> stocks=getAllStock();

    cout<<"\n===== BLOOD INVENTORY =====\n";
    cout<<"\nBlood Group\tAvailable Units\n";

    if(stocks.empty())
        cout<<"No stock available!\n";

    for(const auto &s:stocks)
        cout<<s.bloodGroup<<"\t\t"<<s.units<<endl;
}

// SEARCH STOCK
void InventoryManager::searchStock()
{
    string group;
    cout<<"\nEnter Blood Group: ";
    cin>>group;

    if(!validGroup(group))
    {
        cout<<"\nInvalid Blood Group!\n";
        return;
    }

    for(const auto &s:getAllStock())
    {
        if(group==s.bloodGroup)
        {
            cout<<"\nBlood Group: "<<s.bloodGroup;
            cout<<"\nAvailable Units: "<<s.units<<endl;
            return;
        }
    }

    cout<<"\nBlood Group Not Found!\n";
}

// GET ALL STOCK
vector<BloodStock> InventoryManager::getAllStock()
{
    return readStock();
}

// ADD STOCK THROUGH GUI
bool InventoryManager::addStockRecord(const char* group,int units)
{
    if(!group || units<=0 || !validGroup(group))
        return false;

    vector<BloodStock> stocks=getAllStock();

    for(auto &s:stocks)
    {
        if(strcmp(s.bloodGroup,group)==0)
        {
            s.units+=units;
            return saveStock(stocks);
        }
    }

    BloodStock s{};
    strncpy(s.bloodGroup,group,sizeof(s.bloodGroup)-1);
    s.units=units;
    stocks.push_back(s);

    return saveStock(stocks);
}