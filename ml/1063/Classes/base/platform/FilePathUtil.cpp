//
//  FilePathUtil.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/24.
//
//

#include "FilePathUtil.h"

#include "STSystemFunction.h"
#include "cocos2d.h"
USING_NS_CC;
int FilePathUtil::getFileNum(string path){
 
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto serachPaht = FileUtils::getInstance()->getSearchPaths();
    STSystemFunction _cf;
    int num  =  _cf.listAssetFiles(path);
    for(auto rootPath:serachPaht){
        std::string aessets = "assets/";
        auto index = rootPath.find(aessets);
        if(index != -1)
            rootPath.replace(index, aessets.length(), "");
        if(num == 0)
            num =  _cf.listAssetFiles(rootPath+path);
        else
            break;
    }
    return num;
#endif
    path = FileUtils::getInstance()->fullPathForFilename(path);
    int _num = 0;
    DIR *dir;
    dirent *dir_info;
    char file_path[PATH_MAX];
    if(FileUtility::isDir(path.c_str()))
    {
        if((dir = opendir(path.c_str())) == NULL)
        {
           // log("invalid path");
        }else
        {
            while((dir_info = readdir(dir)) != NULL)
            {
                FileUtility::getFilePath(path.c_str(), dir_info->d_name, file_path);
                if(strcmp(dir_info->d_name, ".") == 0 || strcmp(dir_info->d_name, "..") == 0)
                    continue;
                if(FileUtility::isDir(file_path))
                {
                    
                }
                else if(FileUtility::isFile(file_path))
                {
                    string filePath(file_path);
                    if (filePath.substr(filePath.size() - 4) == ".png" ||
                        filePath.substr(filePath.size() - 4) == ".jpg")
                    {
                        _num++;
                    }
                }
            }
        }
    }
    return _num;
}


void FilePathUtil::saveToFavorite(RenderTexture* render, function<void(bool)> function, string projDir)
{
    Director::getInstance()->getRenderer()->render();
    FileUtility::createDirectory((FileUtility::getStoragePath() +"/"+ projDir).c_str());
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long time =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
    
    string rootPath;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    rootPath = FileUtility::getStoragePath()+"/"+projDir + "/";
#else
    rootPath = projDir + "/";
#endif
    
    string normalImgPath = rootPath + StringUtils::format("%ld.png",time);
    
    bool bigSave;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Image* normalImg = render->newImage();
    bigSave = normalImg->saveToFile(normalImgPath,false);
    CC_SAFE_RELEASE_NULL(normalImg);
    STSystemFunction _cf;
    _cf.refreshDCIM(normalImgPath);
#else
    bigSave = render->saveToFile(normalImgPath);
#endif

    if(function){
        function(bigSave);
    }
}

void FilePathUtil::getFilesPathByDir(const string& temp,vector<string>& v)
{
    auto path = FileUtility::getStoragePath() + "/" +temp;
    DIR *dir;
    dirent *dir_info;
    char file_path[PATH_MAX];
    if(FileUtility::isDir(path.c_str()))
    {
        if((dir = opendir(path.c_str())) == NULL)
        {
            log("invalid path");
        }else
        {
            while((dir_info = readdir(dir)) != NULL)
            {
                FileUtility::getFilePath(path.c_str(), dir_info->d_name, file_path);
                if(strcmp(dir_info->d_name, ".") == 0 || strcmp(dir_info->d_name, "..") == 0)
                    continue;
                if(FileUtility::isDir(file_path))
                {
                    
                }
                else if(FileUtility::isFile(file_path))
                {
                    string filePath(file_path);
                    if (filePath.substr(filePath.size() - 4) == ".png" ||
                        filePath.substr(filePath.size() - 4) == ".jpg")
                    {
                        v.push_back(file_path);
                    }
                }
            }
        }
    }
}

std::string FilePathUtil::newSharePath() {
    string name = "shareCache";
    
    FileUtility::createDirectory((FileUtility::getStoragePath()+ "/"+name).c_str());
    FileUtility::createDirectory((FileUtility::getStoragePath()+ "/"+name +"/"+ "share").c_str());
    string rootPath;
    rootPath = FileUtility::getStoragePath()+"/"+name + "/" + "share" + "/";
    
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long nowtime =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
    
    auto sharePath = StringUtils::format("%s%ld.png",rootPath.c_str(),nowtime);
    return sharePath;
}