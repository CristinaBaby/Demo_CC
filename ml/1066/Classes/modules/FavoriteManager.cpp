//
//  FavoriteManager.cpp
//  PuzzleTest
//
//  Created by zhangguangzong1 on 10/27/14.
//
//

#include "FavoriteManager.h"

static FavoriteManager* instance = nullptr;

FavoriteManager* FavoriteManager::getInstance()
{
    if (!instance)
    {
        instance = new FavoriteManager();
    }
    return instance;
}

FavoriteManager::FavoriteManager():m_fThumbScale(0.65f)
{
}

FavoriteManager::~FavoriteManager(){}

void FavoriteManager::saveToFavorite(RenderTexture* render, function<void(bool)> function, string projDir)
{
    Director::getInstance()->getRenderer()->render();
    
    FileUtility::createDirectory((FileUtility::getStoragePath() +"/"+ projDir).c_str());
    
    FileUtility::createDirectory((FileUtility::getStoragePath() +"/"+ projDir +"/"+ FAVORITE).c_str());
    
    FileUtility::createDirectory((FileUtility::getStoragePath() +"/"+ projDir +"/"+ FAVORITE + "/" + THUMBNAIL).c_str());
    
    struct timeval tv;
    
    gettimeofday(&tv,NULL);
    
    long time =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
    
    string rootPath;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	rootPath = FileUtility::getStoragePath()+"/"+projDir + "/" + FAVORITE + "/";
#else
	rootPath = projDir + "/" + FAVORITE + "/";
#endif
    
	string normalImgPath = rootPath + StringUtils::format("%ld.png",time);
	string thumbnailPath = rootPath + THUMBNAIL + "/" +StringUtils::format("%ld.png",time);
    
    bool bigSave;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Image* normalImg = render->newImage();
    log("==========================>big normalImgPath:%s", normalImgPath.c_str());
    bigSave = normalImg->saveToFile(normalImgPath);
    CC_SAFE_RELEASE_NULL(normalImg);
#else
    bigSave = render->saveToFile(normalImgPath);
#endif
    
    Sprite* s = render->getSprite();
    s->setAnchorPoint(Vec2::ZERO);
    s->setPosition(Vec2::ZERO);
    RenderTexture* smallRender = RenderTexture::create(s->getContentSize().width * m_fThumbScale,
                                                       s->getContentSize().height * m_fThumbScale);
    s->setScale(m_fThumbScale);
    smallRender->begin();
    s->visit();
    smallRender->end();
    s->setScale(1.0f);
    Director::getInstance()->getRenderer()->render();
    
    bool smallSave;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Image* smallImg = smallRender->newImage();
    log("==========================>small thumbnailPath:%s", thumbnailPath.c_str());
    smallSave = smallImg->saveToFile(thumbnailPath);
    CC_SAFE_RELEASE_NULL(smallImg);
#else
    smallSave = smallRender->saveToFile(thumbnailPath);
#endif
    if(bigSave)
    {
        log("big success");
    }
    else
    {
        log("big failed");

    }
    if(smallSave)
    {
        log("small success");
    }
    else
    {
        log("small failed");
    }
    
    function(bigSave && smallSave);
}

void FavoriteManager::obtainFavoritePaths(vector<string>& favVec, vector<string>& thumbVec, string projDir)
{
    string favPath = FileUtility::getStoragePath() +"/"+ projDir +"/"+FAVORITE;
    
    this->getFilesPathByDir(favPath, favVec);
    
    string thumbPath = FileUtility::getStoragePath() +"/"+ projDir +"/"+FAVORITE + "/" + THUMBNAIL;
    
    this->getFilesPathByDir(thumbPath, thumbVec);
}

void FavoriteManager::getFilesPathByDir(const string& path,vector<string>& v)
{
    DIR *dir;
    dirent *dir_info;
    char file_path[PATH_MAX];
    if(FileUtility::is_dir(path.c_str()))
    {
        if((dir = opendir(path.c_str())) == NULL)
        {
            log("invalid path");
        }else
        {
            while((dir_info = readdir(dir)) != NULL)
            {
                FileUtility::get_file_path(path.c_str(), dir_info->d_name, file_path);
                
                if(strcmp(dir_info->d_name, ".") == 0 || strcmp(dir_info->d_name, "..") == 0)
                    continue;
                
                if(FileUtility::is_dir(file_path))
                {
                    
                }
                else if(FileUtility::is_file(file_path))
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

void FavoriteManager::deleteFile(const string& path)
{
    FileUtility::removeDir(path);
}
