//
//  FavoriteManager.h
//  PuzzleTest
//
//  Created by zhangguangzong1 on 10/27/14.
//
//

#ifndef __PuzzleTest__FavoriteManager__
#define __PuzzleTest__FavoriteManager__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "../utils/FileUtility.h"

#define FAVORITE        "fav"
#define THUMBNAIL       "thumbnail"


using namespace std;
USING_NS_CC;


class FavoriteManager {
    
protected:
    FavoriteManager();
    ~FavoriteManager();
public:
    static FavoriteManager* getInstance();
public:
    void saveToFavorite(RenderTexture* render, function<void(bool)> function, string projDir = "default");
    
    void obtainFavoritePaths(vector<string>&,vector<string>&,string projDir = "default");
    
    void deleteFile(const string& path);
    
    void setThumbScale(float f)
    { m_fThumbScale = f;}
protected:
    void getFilesPathByDir(const string& path,vector<string>& v);
protected:
    float m_fThumbScale;
};


#endif /* defined(__PuzzleTest__FavoriteManager__) */
