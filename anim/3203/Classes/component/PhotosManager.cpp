#include "PhotosManager.h"

#define SAVE_IMAGE_INDEX    "SAVE_IMAGE_INDEX"

static PhotosManager *_sharedPhotos = nullptr;

PhotosManager* PhotosManager::getInstance()
{
    if (! _sharedPhotos)
    {
        _sharedPhotos = new PhotosManager();
    }
    
    return _sharedPhotos;
}

void PhotosManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedPhotos);
}

PhotosManager::PhotosManager()
{
    _userDefault = UserDefault::getInstance();
    _increment = _userDefault->getIntegerForKey(SAVE_IMAGE_INDEX, 0);
    _writablePath = FileUtils::getInstance()->getWritablePath();
    loadPhotoNames();
    
    log("_writablePath %s", _writablePath.c_str());
}


void PhotosManager::saveToDocument(RenderTexture * rt)
{
    string filename = "photo" + std::to_string(_increment) + ".jpg";
    rt->saveToFile(filename);
    _increment++;                   //数据自动增加1. 避免重名
    
    //将filename 放如deque里面
    _photos.push_back(filename);
    //检查是否超过上线, 超过后将文件删除.
    if (_photos.size() > 10) {
        filename = _photos.front();
        removeFromDocument(filename);
    }
    
    _userDefault->setIntegerForKey(SAVE_IMAGE_INDEX, _increment);
    
    storePhotoNames();
}


Sprite* PhotosManager::getScreenSprite()
{
    GLView* glview = Director::getInstance()->getOpenGLView();
    Size frameSize = glview->getFrameSize();
    
    const int dataLength = frameSize.width * frameSize.height * 4;
    char* pixelData = new char[dataLength];
    glReadPixels(0, 0, frameSize.width, frameSize.height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    
    Texture2D* texture = new Texture2D();
    texture->initWithData(pixelData, dataLength, Texture2D::PixelFormat::RGBA8888, frameSize.width, frameSize.height, frameSize);
    
    Sprite* sprScreenshot = Sprite::createWithTexture(texture);
    sprScreenshot->setScaleX(1 / glview->getScaleX());
    sprScreenshot->setScaleY(1 / glview->getScaleY());
    sprScreenshot->setFlippedY(true);
    
    CC_SAFE_RELEASE(texture);
    delete[] pixelData;
    
    return sprScreenshot;
}


void PhotosManager::removeFromDocument(const string& filename)
{
    string path = _writablePath + filename;
    remove(path.c_str());
    
    for(deque<string>::iterator it = _photos.begin(); it!=_photos.end(); )
    {
        if((*it).compare(filename) == 0)
        {
            it = _photos.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
    
    storePhotoNames();
}

void PhotosManager::removeFromDocumentByFullPath(const string &fullpath)
{
    std::string::size_type pos = fullpath.find_last_of("/") + 1;
    string filename = fullpath.substr(pos, fullpath.length() - pos);
    removeFromDocument(filename);
}

void PhotosManager::storePhotoNames()
{
    ValueMap vm;
    
    //将deque 转换成 unordered_map;
    for (int i = 0; i < _photos.size(); i++) {
        
        Value value(_photos.at(i));
        string key = std::to_string(i);
        
        vm[key] = value;
    }
    
    string path = _writablePath + "photos.plist";
    
    if(! vm.empty())
    {
        FileUtils::getInstance()->writeToFile(vm, path);
    }
    else
    {
        remove(path.c_str());
    }
    
    _userDefault->flush();
}


void PhotosManager::loadPhotoNames()
{
    string path = _writablePath + "photos.plist";
    
    if (FileUtils::getInstance()->isFileExist(path)) {
        
        ValueMap vm = FileUtils::getInstance()->getValueMapFromFile(path);
        
        //将unordered_map 转换成 deque
        for (int i = 0; i < vm.size(); i++) {
            Value name = vm[std::to_string(i)];
            _photos.push_back(name.asString());
        }
    }
}

string PhotosManager::getPhotosName(int index)
{
    if (index >= _photos.size()) {
        string empty;
        return std::move(empty);
    }
    
    return _writablePath + _photos.at(_photos.size() - index - 1);
}

int PhotosManager::getPhotosCount()
{
    return (int)_photos.size();
}