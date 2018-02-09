//
//  FrameWorkHelper.h
//  FashionDoll
//
//  Created by zhangguangzong1 on 15-4-19.
//
//

#ifndef __FashionDoll__FrameWorkHelper__
#define __FashionDoll__FrameWorkHelper__

#include <iostream>
#include <string>
#include <vector>
#include "cocos2d.h"

using namespace std;

typedef std::vector<std::string> strArray;

class FrameWorkHelper
{
public:
    static inline void split(const std::string& src, const std::string& token, strArray& vect)
    {
        size_t nend = 0;
        size_t nbegin = 0;
        size_t tokenSize = token.size();
        while(nend != std::string::npos)
        {
            nend = src.find(token, nbegin);
            if(nend == std::string::npos)
                vect.push_back(src.substr(nbegin, src.length()-nbegin));
            else
                vect.push_back(src.substr(nbegin, nend-nbegin));
            nbegin = nend + tokenSize;
        }
    }
    
    static bool splitWithForm(const std::string& content, strArray& strs);
};

#endif /* defined(__FashionDoll__FrameWorkHelper__) */
