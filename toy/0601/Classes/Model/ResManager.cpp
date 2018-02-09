//
//  ResManager.cpp
//  PaintDress
//
//  Created by maxiang on 6/23/15.
//
//

#include "ResManager.h"
#include "CSVParse.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

static ResManager *_singletonInstance = nullptr;

ResManager::ResManager()
{}

ResManager* ResManager::getInstance()
{
    if (!_singletonInstance) {
        _singletonInstance = new ResManager();
        
        std::string rootPath = FileUtils::getInstance()->getWritablePath() + "res/";
        if (!FileUtils::getInstance()->isFileExist(rootPath)) {
            _singletonInstance->createDirectory(rootPath.c_str());
        }
        
        std::string clothesPath = rootPath + "clothes/";
        if (!FileUtils::getInstance()->isFileExist(clothesPath)) {
            _singletonInstance->createDirectory(clothesPath.c_str());
        }
        
        std::string topClothesPath = clothesPath + "top/";
        if (!FileUtils::getInstance()->isFileExist(topClothesPath)) {
            _singletonInstance->createDirectory(topClothesPath.c_str());
        }
        
        std::string bottomClothesPath = clothesPath + "bottom/";
        if (!FileUtils::getInstance()->isFileExist(bottomClothesPath)) {
            _singletonInstance->createDirectory(bottomClothesPath.c_str());
        }
        
        std::string dressClothesPath = clothesPath + "dress/";
        if (!FileUtils::getInstance()->isFileExist(dressClothesPath)) {
            _singletonInstance->createDirectory(dressClothesPath.c_str());
        }
        
        std::string shoesClothesPath = clothesPath + "shoes/";
        if (!FileUtils::getInstance()->isFileExist(shoesClothesPath)) {
            _singletonInstance->createDirectory(shoesClothesPath.c_str());
        }
    }
    
    return _singletonInstance;
}

#pragma mark- Setup res data

bool ResManager::setupResData(const std::string& resFileName)
{
    auto parser = CSVParse::create(resFileName.c_str());
    int rows = parser->getRows();
    
    for (int row = 1; row < rows; ++row)
    {
        Res res;
        std::string type = parser->getData(row, 1);

        res.resName = parser->getData(row, 5);
        res.resOriginalName = res.resName;
        res.resTypeName = parser->getData(row, 2);
        res.price = atoi(parser->getData(row, 7));
        res.positionX = atof(parser->getData(row, 9));
        res.positionY = atof(parser->getData(row, 10));
        res.isPaintRes = false;
        
        //purchase new res
        std::string key = UserDefaultKey_IsNewPurchaseRes + res.resName;
        res.isNewPurchasedRes = UserDefault::getInstance()->getBoolForKey(key.c_str(), false);

        auto addPath = [=](Res& res, bool withExtension){
          
            std::string pictureExtension = "";
            std::string maskExtension = "";
            if (withExtension)
            {
                pictureExtension = "picture.png";
                maskExtension = "mask.png";
            }
            res.resPicturePath = getResClothesPath(res) + res.resName + pictureExtension;
            res.resMaskPath = getResClothesPath(res) + res.resName + maskExtension;
            res.resIconPath = getResIconPath(res) + parser->getData(row, 4) + ".png";
        };
        
        if (type == "Hair")
        {
            res.type = ResType::ResType_Hair;
            addPath(res, false);
            _hairRes.push_back(res);
        }
        else if (type == "Bottom")
        {
            res.type = ResType::ResType_Bottom;
            addPath(res, true);
            _bottomRes.push_back(res);
        }
        else if (type == "Top")
        {
            res.type = ResType::ResType_Top;
            addPath(res, true);
            _topRes.push_back(res);
        }
        else if(type == "Dress")
        {
            res.type = ResType::ResType_Dress;
            addPath(res, true);
            _dressRes.push_back(res);
        }
        else if(type == "Shoes")
        {
            res.type = ResType::ResType_Shoes;
            addPath(res, true);
            _shoesRes.push_back(res);
        }
        else if(type == "Accessory")
        {
            res.type = ResType::ResType_Accessory;
            addPath(res, false);
            _accessoryRes.push_back(res);
        }
        
    }
    
    return true;
}

std::string ResManager::getResIconPath(const Res& res)
{
    if (res.type == ResType::ResType_Hair)
    {
        return std::string(PATH_ICON) + "icon_hair/";
    }
    else if (res.type == ResType::ResType_Bottom)
    {
        return std::string(PATH_ICON) + "icon_bottom/";
    }
    else if (res.type == ResType::ResType_Top)
    {
        return std::string(PATH_ICON) + "icon_top/";
    }
    else if(res.type == ResType::ResType_Dress)
    {
        return std::string(PATH_ICON) + "icon_dress/";
    }
    else if(res.type == ResType::ResType_Shoes)
    {
        return std::string(PATH_ICON) + "icon_shoes/";
    }
    else
    {
        return std::string(PATH_ICON) + "icon_accessory/";
    }
}

std::string ResManager::getResClothesPath(const Res& res)
{
    if (res.type == ResType::ResType_Hair)
    {
        return std::string(PATH_CLOTHES) + "hair/";
    }
    else if (res.type == ResType::ResType_Bottom)
    {
        return std::string(PATH_CLOTHES) + "bottom/";
    }
    else if (res.type == ResType::ResType_Top)
    {
        return std::string(PATH_CLOTHES) + "top/";
    }
    else if(res.type == ResType::ResType_Dress)
    {
        return std::string(PATH_CLOTHES) + "dress/";
    }
    else if(res.type == ResType::ResType_Shoes)
    {
        return std::string(PATH_CLOTHES) + "shoes/";
    }
    else
    {
        return std::string(PATH_CLOTHES) + "accessory/";
    }
}

bool ResManager::setupPaintResData(const std::string& paintResFileName)
{
    _paintResDic = FileUtils::getInstance()->getValueMapFromFile(paintResFileName);
    if (_paintResDic.size() == 0)
    {
        _paintResDic = ValueMap();
        _paintResDic[KEY_PAINT_TOP] = ValueVector();
        _paintResDic[KEY_PAINT_BOTTOM] = ValueVector();
        _paintResDic[KEY_PAINT_DRESS] = ValueVector();
        _paintResDic[KEY_PAINT_SHOES] = ValueVector();
        
        FileUtils::getInstance()->writeToFile(_paintResDic, paintResFileName);
        return true;
    }

    ValueVector paintTopRess = _paintResDic[KEY_PAINT_TOP].asValueVector();
    getPaint(_paintTopRes, paintTopRess);

    ValueVector paintBottomRess = _paintResDic[KEY_PAINT_BOTTOM].asValueVector();
    getPaint(_paintBottomRes, paintBottomRess);
    
    ValueVector paintDressRess = _paintResDic[KEY_PAINT_DRESS].asValueVector();
    getPaint(_paintDressRes, paintDressRess);
    
    ValueVector paintShoesRess = _paintResDic[KEY_PAINT_SHOES].asValueVector();
    getPaint(_paintShoesRes, paintShoesRess);
    
    return true;
}

void ResManager::getPaint(std::vector<Res>& resVector, const cocos2d::ValueVector& valueVector)
{
    std::string writeablePath = FileUtils::getInstance()->getWritablePath();
    
    for (int i = 0; i < valueVector.size(); ++i)
    {
        ValueMap item = valueVector.at(i).asValueMap();
        std::string resName = item["resName"].asString();
        std::string resOriginalName = item["resOriginalName"].asString();
        float positionX = atof(item["positionX"].asString().c_str());
        float positionY = atof(item["positionY"].asString().c_str());
        ResType type = ResType(item["type"].asInt());
        
        Res res;
        res.type = type;
        res.resHistoryPath = writeablePath + getResClothesPath(res) + resName + "_history.png";
        res.resPicturePath = writeablePath + getResClothesPath(res) + resName + "_picture.png";
        res.positionX = positionX;
        res.positionY = positionY;
        res.resName = resName;
        res.isPaintRes = true;
        res.resOriginalName = resOriginalName;
        
        //is new paint res?
        std::string key = UserDefaultKey_IsNewPaintRes + res.resOriginalName;
        res.isNewPaintRes = UserDefault::getInstance()->getBoolForKey(key.c_str(), false);
        
        resVector.push_back(res);
    }
}

#pragma mark- Saved photos

bool ResManager::setupSavedPhotos(const std::string& savedPhotoFileName)
{
    //setup saved photos
    std::string savedPhotoPath = FileUtils::getInstance()->getWritablePath() + DATA_PATH_SAVED_PHOTOS;
    if (FileUtils::getInstance()->isFileExist(savedPhotoPath))
    {
        ValueMap map = FileUtils::getInstance()->getValueMapFromFile(savedPhotoPath);
        _singletonInstance->_savedPhotos = map[KEY_SAVED_PHOTOS].asValueVector();
    }
    
    return true;
}

void ResManager::savePhoto(const std::string& photo)
{
    _savedPhotos.push_back(Value(photo));
    
    ValueMap map;
    map[KEY_SAVED_PHOTOS] = Value(_savedPhotos);
    std::string path = FileUtils::getInstance()->getWritablePath() + DATA_PATH_SAVED_PHOTOS;
    FileUtils::getInstance()->writeToFile(map, path);
}

void ResManager::deletePhoto(const std::string& photo)
{
    for (auto iterator = _savedPhotos.begin(); iterator != _savedPhotos.end(); ++iterator)
    {
        if (iterator->asString() == photo)
        {
            _savedPhotos.erase(iterator);
            
            ValueMap map;
            map[KEY_SAVED_PHOTOS] = Value(_savedPhotos);
            std::string path = FileUtils::getInstance()->getWritablePath() + DATA_PATH_SAVED_PHOTOS;
            FileUtils::getInstance()->writeToFile(map, path);
            return;
        }
    }
}

#pragma mark- Manage res

void ResManager::addPaint(const int index, const Res& res, std::vector<Res>& resVector, const char* key)
{
    if (resVector.size() >= MAX_PICTURE_SAVE_COUTN)
    {
        resVector.erase(resVector.begin());
    }
    char resName[10];
    sprintf(resName, "%d",index);
    Res paintRes = res;
    paintRes.resName = resName;
    paintRes.resOriginalName = res.resOriginalName;
    paintRes.isPaintRes = true;
    paintRes.resPicturePath = FileUtils::getInstance()->getWritablePath() + getResClothesPath(res) + resName + "_picture.png";
    paintRes.resHistoryPath = FileUtils::getInstance()->getWritablePath() + getResClothesPath(res) + resName + "_history.png";
    resVector.push_back(paintRes);
    
    ValueVector vector;
    for (auto res : resVector)
    {
        ValueMap map;
        map["resName"] = res.resName;
        map["resOriginalName"] = res.resOriginalName;
        map["positionX"] = res.positionX;
        map["positionY"] = res.positionY;
        map["type"] = (int)res.type;

        vector.push_back(Value(map));
    }
    
    _paintResDic[key] = vector;
    
    
    std::string paintResPath = FileUtils::getInstance()->getWritablePath() + "paintResData.plist";
    FileUtils::getInstance()->writeToFile(_paintResDic, paintResPath);
}

void ResManager::addPaintBottomRes(const Res& res)
{
    int index = getSaveIndex(_paintBottomRes);
    addPaint(index, res, _paintBottomRes, KEY_PAINT_BOTTOM);
}

void ResManager::addPaintDressRes(const Res& res)
{
    int index = getSaveIndex(_paintDressRes);
    addPaint(index, res, _paintDressRes, KEY_PAINT_DRESS);
}

void ResManager::addPaintShoesRes(const Res& res)
{
    int index = getSaveIndex(_paintShoesRes);
    addPaint(index, res, _paintShoesRes, KEY_PAINT_SHOES);
}

void ResManager::addPaintTopRes(const Res& res)
{
    int index = getSaveIndex(_paintTopRes);
    addPaint(index, res, _paintTopRes, KEY_PAINT_TOP);
}

int ResManager::getSaveIndex(const std::vector<Res>& resVector)
{
    int currentIndex = resVector.size();
    if (currentIndex + 1 > MAX_PICTURE_SAVE_COUTN)
    {
        auto begin = resVector.begin();
        return atoi(begin->resName.c_str());
    }
    else
    {
        return currentIndex + 1;
    }
}

#pragma mark- others

bool ResManager::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#endif
}


