//
//  ResManager.h
//  PaintDress
//
//  Created by maxiang on 6/23/15.
//
//

#ifndef __PaintDress__ResManager__
#define __PaintDress__ResManager__

#include "cocos2d.h"
#include "AppConfigs.h"

enum class ResType
{
    ResType_None = 0,
    ResType_Hair,
    ResType_Bottom,
    ResType_Dress,
    ResType_Top,
    ResType_Shoes,
    ResType_Accessory
};

struct Res
{
    ResType type;
    
    std::string resName;
    std::string resOriginalName;

    std::string resPicturePath;
    std::string resIconPath;
    std::string resMaskPath;
    std::string resHistoryPath;
    std::string resTypeName;

    int price;
    
    float positionX;
    float positionY;
    bool isPaintRes;
    bool isNewPurchasedRes;
    bool isNewPaintRes;
};

struct Model
{
    Res *hairRes;
    Res *topRes;
    Res *dressRes;
    Res *bottomRes;
    Res *shoesRes;
    Res *accessoryRes;
};

class ResManager
{
public:
    static ResManager* getInstance();
    ResManager();
    
    bool setupResData(const std::string& resFileName);
    bool setupPaintResData(const std::string& paintResFileName);
    bool setupSavedPhotos(const std::string& savedPhotoFileName);

    cocos2d::ValueVector& getSavedPhotos(){return _savedPhotos;};
    void savePhoto(const std::string& photo);
    void deletePhoto(const std::string& photo);

    std::vector<Res>& getHairRes(){return _hairRes;};
    std::vector<Res>& getTopRes(){return _topRes;};
    std::vector<Res>& getBottomRes(){return _bottomRes;};
    std::vector<Res>& getDressRes(){return _dressRes;};
    std::vector<Res>& getShoesRes(){return _shoesRes;};
    std::vector<Res>& getAccessory(){return _accessoryRes;};

    void addPaintBottomRes(const Res& res);
    void addPaintDressRes(const Res& res);
    void addPaintShoesRes(const Res& res);
    void addPaintTopRes(const Res& res);

    std::vector<Res>& getPaintBottomRes(){return _paintBottomRes;};
    std::vector<Res>& getPaintDressRes(){return _paintDressRes;};
    std::vector<Res>& getPaintShoesRes(){return _paintShoesRes;};
    std::vector<Res>& getPaintTopRes(){return _paintTopRes;};
    
    int getSaveIndex(const std::vector<Res>& resVector);
    std::string getResClothesPath(const Res& res);
    std::string getResIconPath(const Res& res);

protected:
    bool createDirectory(const char *path);
    void addPaint(const int index, const Res& res, std::vector<Res>& resVector, const char* key);
    void getPaint(std::vector<Res>& resVector, const cocos2d::ValueVector& valueVector);

protected:
    std::vector<Res> _hairRes;
    std::vector<Res> _topRes;
    std::vector<Res> _bottomRes;
    std::vector<Res> _dressRes;
    std::vector<Res> _shoesRes;
    std::vector<Res> _accessoryRes;

    std::vector<Res> _paintBottomRes;
    std::vector<Res> _paintDressRes;
    std::vector<Res> _paintShoesRes;
    std::vector<Res> _paintTopRes;
    
    cocos2d::ValueVector _savedPhotos;

    typedef std::unordered_map<std::string, std::vector<cocos2d::ValueMap>> VectorMap;
    cocos2d::ValueMap _paintResDic;
};

#endif /* defined(__PaintDress__ResManager__) */
