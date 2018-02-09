//
//  PictureManager.cpp
//  ColorBook0818
//
//  Created by huangwen on 15/8/24.
//
//

#include "PictureManager.h"
#include "STSystemFunction.h"

void  PictureManager::get_file_path(const char *path, const char *file_name,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, file_name);
}

//
bool PictureManager::is_dir(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)//
    {
        return S_ISDIR(statbuf.st_mode) != 0;//
    }
    return false;
#else
    
    return false;
#endif
    
}

//
bool PictureManager::is_file(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)
        return S_ISREG(statbuf.st_mode) != 0;//
    return false;
#else
    
    return false;
#endif
}

int PictureManager::getImgFileCount(const string& path)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    DIR *dir;
    dirent *dir_info;
    char file_path[PATH_MAX];
    int fileCount = 0;
    string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(path);
    if(PictureManager::is_dir(fullPath.c_str()))
    {
        if((dir = opendir(fullPath.c_str())) == NULL)
        {
            log("invalid path");
            return 0;
        }else
        {
            while((dir_info = readdir(dir)) != NULL)
            {
                PictureManager::get_file_path(fullPath.c_str(), dir_info->d_name, file_path);
                if(strcmp(dir_info->d_name, ".") == 0 || strcmp(dir_info->d_name, "..") == 0)
                    continue;
                if(PictureManager::is_dir(file_path))
                {
                    //不做处理
                    continue;
                }else if(PictureManager::is_file(file_path))
                {
                    string filePath(file_path);
                    if (filePath.substr(filePath.size() - 4) == ".png" ||
                        filePath.substr(filePath.size() - 4) == ".jpg")
                    {
                        fileCount++;
                    }
                }
            }
        }
    }
    return fileCount;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //STSystemFunction sys;
    return 6;//sys.listAssetFiles(path);
#else
    return 0;
#endif
}

