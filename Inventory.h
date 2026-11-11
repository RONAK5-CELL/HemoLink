#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>

struct BloodStock
{
    char bloodGroup[5];
    int units;
};

class InventoryManager
{
public:
    void addStock();
    void displayStock();
    void searchStock();

    std::vector<BloodStock> getAllStock();
    bool addStockRecord(const char* group, int units);
};

#endif