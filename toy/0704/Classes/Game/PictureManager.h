//
//  PictureManager.h
//  ColorBook0818
//
//  Created by huangwen on 15/8/24.
//
//

#ifndef __ColorBook0818__PictureManager__
#define __ColorBook0818__PictureManager__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#endif
using namespace std;
using namespace cocos2d;

class PictureManager{
public:
    
    static void get_file_path(const char *path, const char *file_name,  char *file_path);
    //
    static bool is_dir(const char *path);
    //
    static bool is_file(const char *path);
    
    //获取指定路径文件夹下的图片文件数量
    static int getImgFileCount(const string& path);
};

#endif /* defined(__ColorBook0818__PictureManager__) */
