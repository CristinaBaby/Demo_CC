
#ifndef __ConfigManager__
#define __ConfigManager__


#include "SingleTon.h"
#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class DecorateConfigData {
    
public:
    bool small;
    bool rotatable;
    bool zoomable;
    bool eatable;
    bool single;
    int beginIndex;
    int freeCount;
    int freeCountOrigin;
    int totalCount;
    int holidayIndex;
    int holidayCount;
    int videoCount;
    std::string decTypeName;
    std::string pathName;
    void init(){
        beginIndex = 0;
        freeCount = 0;
        freeCountOrigin = 0;
        totalCount = 0;
        holidayIndex = -1;
        holidayCount = 0;
        videoCount = 0;
        rotatable = true;
        zoomable = true;
        eatable = true;
        single = true;
        small = false;
        decTypeName = "";
    }
    void display(){
        std::cout<<"rotatable:"<<rotatable<<std::endl<<"zoomable:"<<zoomable<<std::endl<<"beginIndex:"<<beginIndex<<std::endl<<"freeCount:"<<freeCount<<std::endl<<"totalCount:"<<totalCount<<std::endl<<"videoCount:"<<videoCount<<std::endl<<"holidayIndex:"<<holidayIndex<<std::endl<<"holidayCount:"<<holidayCount<<std::endl<<"decTypeName:"<<decTypeName<<std::endl<<"pathName:"<<pathName<<std::endl;
    }
    void copyData(DecorateConfigData data){
        beginIndex = data.beginIndex;
        freeCount = data.freeCount;
        freeCountOrigin = data.freeCountOrigin;
        totalCount = data.totalCount;
        holidayIndex = data.holidayIndex;
        holidayCount = data.holidayCount;
        videoCount = data.videoCount;
        rotatable = data.rotatable;
        zoomable = data.zoomable;
        eatable = data.eatable;
        single = data.single;
        small = data.small;
        pathName = data.pathName;
        decTypeName = data.decTypeName;
    }
    
    static DecorateConfigData copy(DecorateConfigData data){
        DecorateConfigData copyData;
        copyData.beginIndex = data.beginIndex;
        copyData.freeCount = data.freeCount;
        copyData.freeCountOrigin = data.freeCountOrigin;
        copyData.totalCount = data.totalCount;
        copyData.holidayIndex = data.holidayIndex;
        copyData.holidayCount = data.holidayCount;
        copyData.videoCount = data.videoCount;
        copyData.rotatable = data.rotatable;
        copyData.zoomable = data.zoomable;
        copyData.eatable = data.eatable;
        copyData.single = data.single;
        copyData.small = data.small;
        copyData.pathName = data.pathName;
        copyData.decTypeName = data.decTypeName;
        return copyData;
    }
};

class DecorateTypeConfigData {
    
public:
    std::string decTypeName;
    std::string decPath;
    std::string decIconName;
    void init(){
        decTypeName = "";
        decIconName = "";
        decPath = "";
    }
    void display(){
        std::cout<<"decTypeName:"<<decTypeName<<std::endl<<"decPath:"<<decPath<<std::endl<<"decIconName:"<<decIconName<<std::endl;
    }
    void copyData(DecorateTypeConfigData data){
        decTypeName = data.decTypeName;
        decIconName = data.decIconName;
        decPath = data.decPath;
    }
    
    static DecorateTypeConfigData copy(DecorateTypeConfigData data){
        DecorateTypeConfigData copyData;
        copyData.decTypeName = data.decTypeName;
        copyData.decIconName = data.decIconName;
        copyData.decPath = data.decPath;
        return copyData;
    }
};

class IAPConfigData {
    
public:
    std::string iapId;
    std::string iconName;
    std::vector<std::string> unlockItem;
    void init(){
        iapId = "";
        iconName = "";
    }
    void display(){
        std::cout<<"iapId:"<<iapId<<std::endl<<"iconName:"<<iconName<<std::endl<<"unlockItem:"<<std::endl;
        for_each(unlockItem.begin(), unlockItem.end(), [=](std::string item){
            std::cout<<item<<std::endl;
        });
    }
    void copyData(IAPConfigData data){
        iapId = data.iapId;
        iconName = data.iconName;
        unlockItem = data.unlockItem;
    }
    
    static IAPConfigData copy(IAPConfigData data){
        IAPConfigData copyData;
        copyData.iapId = data.iapId;
        copyData.iconName = data.iconName;
        copyData.unlockItem = data.unlockItem;
        return copyData;
    }
};

class DressUpConfigData {
    
public:
    std::vector<int> levelVector;
    int freeCount;
    int levelCount;
    int lockCount;
    int totalCount;
    void init(){
        freeCount = 0;
        levelCount = 0;
        lockCount = 0;
        totalCount = 0;
    }
};

class ConfigManager : public SingleTon<ConfigManager>
{
public:
    ConfigManager();
    ~ConfigManager();
    
    
    void loadFile(std::string path);
    
    std::vector<Color4B> getColor();
    bool getUnLocked(std::string type);
    bool getVideoUnLocked(std::string type,int index);
    void setVidioUnlocked(std::string type,int index);
    
    DecorateConfigData getDecorateFile(const std::string key);
    
    
    std::vector<DecorateTypeConfigData> getDecorateType(const std::string key,const std::string key2 = "",const std::string key3 = "");
    
    IAPConfigData getIapData(int index);
    IAPConfigData getIapData(const std::string key);
    
    void unlock(int index);
    void unlock(const std::string key);
    
    void restoreDecorateConfigData();
    void unlockDecorateConfigData(const std::string typeName);
    
    inline ValueVector getIapDataVector()
    {
        auto it = m_Map.find("iap");
        ValueVector tempMap;
        if (it==m_Map.end()) {
            return tempMap;
        }
        return it->second.asValueVector();
    }
    
    inline ValueVector getPriceDataVector()
    {
        auto it = m_Map.find("price");
        ValueVector tempMap;
        if (it==m_Map.end()) {
            return tempMap;
        }
        return it->second.asValueVector();
    }
    int getPrice(int index);
private:
    inline ValueVector _getIcingHSBMap()
    {
        auto it = m_Map.find("icingHSB");
        ValueVector tempMap;
        if (it==m_Map.end()) {
            return tempMap;
        }
        return it->second.asValueVector();
    }
    
    inline ValueMap _getDecorateTypeMap()
    {
        auto it = m_Map.find("dectype");
        ValueMap tempMap;
        if (it==m_Map.end()) {
            return tempMap;
        }
        return it->second.asValueMap();
    }
    inline ValueMap _getDressUpMap(const std::string key)
    {
        auto it = m_Map.find("dressup");
        ValueMap tempMap;
        if (it==m_Map.end()) {
            return tempMap;
        }
        ValueMap map = it->second.asValueMap();
        it = map.find(key);
        if (it==map.end()) {
            return tempMap;
        }
        return it->second.asValueMap();
    }
    inline ValueMap _getDecorationMap(const std::string key)
    {
        auto it = m_Map.find("decoration");
        ValueMap tempMap;
        if (it==m_Map.end()) {
            return tempMap;
        }
        ValueMap map = it->second.asValueMap();
        it = map.find(key);
        if (it==map.end()) {
            return tempMap;
        }
        return it->second.asValueMap();
    }
    //    void _loadFile(std::string type,const std::string filePath,ValueMap* map);
protected:
    
    ValueMap m_Map;
    //    std::vector<std::string> m_UnlockTypeStr;
    ValueMap m_UnlockTypeMap;
    std::vector<Color4B> m_ColorArray;
};

#endif