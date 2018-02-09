//
//  MyZipUtils.h
//  puzzle
//
//  Created by liyang1 on 5/15/14.
//
//

#ifndef __puzzle__MyZipUtils__
#define __puzzle__MyZipUtils__

#include <iostream>
#include "cocos2d.h"

#include <dirent.h>
#include <sys/stat.h>
using std::string;

#define BUFFER_SIZE   4096
#define MAX_FILENAME   512

class MyZipUtils {
public:
MyZipUtils();
~MyZipUtils();
public:
    bool uncompress(string _storagePath , string TEMP_PACKAGE_FILE_NAME , string unCompressDirect) {
        CCASSERT(false, "you should add next code later");
        return true;
    };

    bool createDirectory(const char *path);
};
#endif /* defined(__puzzle__MyZipUtils__) */
