//
//  StringHelper.h
//  CodeBase
//
//  Created by zhangguangzong1 on 15-4-19.
//
//

#ifndef CODEBASE_CORE_UTILS_STRINGHELPER_H_
#define CODEBASE_CORE_UTILS_STRINGHELPER_H_
#include <iostream>
#include <string>
#include <vector>
#include "cocos2d.h"

using namespace std;

typedef std::vector<std::string> strArray;

class StringHelper
{
public:
    static void split(const std::string& src, const std::string& token, strArray* strs);
    
    static bool splitWithForm(const std::string& content, strArray* strs);
};

#endif /* defined(CODEBASE_CORE_UTILS_STRINGHELPER_H_) */
