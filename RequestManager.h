#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include "Request.h"

class RequestManager
{
public:

    void addRequest();

    void displayRequests();

    void searchRequest();

    void updateRequestStatus();

    void deleteRequest();

private:

    bool checkRequestID(char id[]);
};

#endif