//
//  FilePathUtil.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/24.
//
//

#ifndef FilePathUtil_hpp
#define FilePathUtil_hpp
#include "FileUtility.h"
#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class FilePathUtil {
public:
    static int getFileNum(string _parentPath);
    static void saveToFavorite(RenderTexture* render, function<void(bool)> function, string projDir);
    static void getFilesPathByDir(const string& path,vector<string>& v);
    //创建一个缓存图片的路径，传入文件夹名字
    static std::string newCachePath(const string& path);
};

#endif /* FilePathUtil_hpp */
