#ifndef DONOR_H
#define DONOR_H

struct Donor
{
    char donorID[15];
    char name[50];
    int age;
    char bloodGroup[5];
    char phone[15];
    char city[30];
    char lastDonation[15];
    char availability[10];
};

#endif