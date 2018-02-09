//
//  STUtility.cpp
//  LVUP002
//
//  Created by Steven.Xc.Tian on 13-9-27.
//
//

#include "STUtility.h"

USING_NS_CC;
USING_NS_ST;

#define HOURS_TO_SECONDS    (60 * 60)

void STUtility::shuffleArray(Vector<Ref*> *src)
{
    const int size = src->size();
    if (size > 1) {
        // create a random squence
        for(int i = 0; i < size; i++)
        {
            src->swap(i, getRandom(size));
        }
    }
}


bool STUtility::parseBoolean(const std::string &value)
{
    if (value.length() == 0)
    {
        return false;
    }
    
    if (0 == strcmp(value.c_str(), "0") || 0 == strcmp(value.c_str(), "false") || 0 == strcmp(value.c_str(), "FALSE"))
    {
        return false;
    }
    return true;
}


long STUtility::getCurrentTimeSeconds()
{
    time_t  now;
    time(&now);
    return  now;
}

long STUtility::getHoursByTimeInterval(long before, long now)
{
    long interval = now - before;
    
    long during = interval / HOURS_TO_SECONDS;
    
    return during;
}

int STUtility::cycleNum(int iCount, bool bAdd, int *idx)
{
    if (bAdd == true)
    {
        if (*idx >= iCount - 1)
        {
            *idx = 0;
        }
        else
        {
            (*idx)++;
        }
    }
    else if (bAdd == false)
    {
        if ((*idx) <= 0)
        {
            (*idx) = iCount - 1;
        }
        else
        {
            (*idx)--;
        }
    }
    
    return (*idx);
}

