#ifndef __PHOTOSMANAGER_H__
#define __PHOTOSMANAGER_H__

#include "cocos2d.h"

USING_NS_CC;

using std::deque;
using std::string;

#define xPhoto              (PhotosManager::getInstance())

class PhotosManager {
public:
    static PhotosManager* getInstance();
    void destroyInstance();
    void removeFromDocument(const string& filename);
    void removeFromDocumentByFullPath(const string &fullpath);
    void saveToDocument(RenderTexture * rt);
    void storePhotoNames();
    string getPhotosName(int index);
    int getPhotosCount();
    Sprite* getScreenSprite();
    
private:
    PhotosManager();
    
    UserDefault *   _userDefault;
    deque<string>   _photos;
    int             _increment;
    string          _writablePath;
    
    void loadPhotoNames();
};


#endif