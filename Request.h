#ifndef REQUEST_H
#define REQUEST_H

struct BloodRequest
{
    char requestID[15];
    char hospitalID[15];
    char bloodGroup[5];
    int units;
    char requestDate[15];
    char priority[15];
    char status[20];
};

#endif