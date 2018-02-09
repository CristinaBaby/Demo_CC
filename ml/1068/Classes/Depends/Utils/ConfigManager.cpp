
#include "ConfigManager.h"
#include "tinyxml2/tinyxml2.h"
#include "StringHelp.h"
#include "IAPManager.h"

using namespace tinyxml2;

ConfigManager::ConfigManager()
{
    
}

ConfigManager::~ConfigManager()
{
    
}

void ConfigManager::loadFile(std::string path)
{
    FileUtils *fu= FileUtils::getInstance();
    ValueMap vm;
    
    vm = fu->getValueMapFromFile(path);//dictionary,如果是vector,使用fu->getValueVectorFromFile(const std::string &filename)
    for (auto it = vm.begin(); it != vm.end();++it)
    {
        log("===%s",typeid(it->second).name());
        m_Map.insert(ValueMap::value_type(it->first,Value(it->second)));
    }
}

IcingHSB ConfigManager::getIcingHSB(const int index)
{
    ValueVector vct = _getIcingHSBMap();
    IcingHSB data;
    data.init();
    
    if (index>=vct.size()) {
        return data;
    }
    auto it = vct.at(index);
    
    ValueMap map = it.asValueMap();
    
    auto itHSBDark  = map.find("dark");
    if (itHSBDark !=map.end()) {
        string str = itHSBDark->second.asString();
        std::vector<std::string> vct = StringHelp::split(str,",");
        data.hsbDark   = Vec3(atof(vct.at(0).c_str()),atof(vct.at(1).c_str()),atof(vct.at(2).c_str()));
    }
    auto itHSBBright = map.find("bright");
    if (itHSBBright!=map.end()) {
        string str = itHSBDark->second.asString();
        std::vector<std::string> vct = StringHelp::split(str,",");
        data.hsbBright   = Vec3(atof(vct.at(0).c_str()),atof(vct.at(1).c_str()),atof(vct.at(2).c_str()));
        if (data.hsbBright.isZero()) {
            data.hsbBright = data.hsbDark;
        }
    }
    data.display();
    return data;
}
 DecorateConfigData ConfigManager::getDecorateFile(const std::string key)
{
    ValueMap map = _getDecorationMap(key);
    DecorateConfigData data;
    data.init();
    for (auto it = map.begin(); it != map.end();++it)
    {
        if (std::strcmp(it->first.c_str(), "freecount")==0) {
            data.freeCount = it->second.asInt();
        }else if (std::strcmp(it->first.c_str(), "totalcount")==0) {
            data.totalCount = it->second.asInt();
        }else if (std::strcmp(it->first.c_str(), "rotatable")==0) {
            data.rotatable = it->second.asBool();
        }else if (std::strcmp(it->first.c_str(), "zoomable")==0) {
            data.zoomable = it->second.asBool();
        }else if (std::strcmp(it->first.c_str(), "eatable")==0) {
            data.eatable = it->second.asBool();
        }else if (std::strcmp(it->first.c_str(), "beginIndex")==0) {
            data.beginIndex = it->second.asInt();
        }else if (std::strcmp(it->first.c_str(), "holidayIndex")==0) {
            data.holidayIndex = it->second.asInt();
        }else if (std::strcmp(it->first.c_str(), "holidayCount")==0) {
            data.holidayCount = it->second.asInt();
        }else if (std::strcmp(it->first.c_str(), "single")==0) {
            data.single = it->second.asBool();
        }else if (std::strcmp(it->first.c_str(), "small")==0) {
            data.small = it->second.asBool();
        }else if (std::strcmp(it->first.c_str(), "typename")==0) {
            data.pathName = it->second.asString();
        }else{
        }
    }
    if (data.pathName=="") {
        data.pathName = string(key);
    }
    data.decTypeName = string(key);
    auto it = m_UnlockTypeMap.find("holiday");
    if (it!=m_UnlockTypeMap.end()) {
        data.holidayCount = 0;
        
    }
    {
        auto it2 = m_UnlockTypeMap.find(key);
        if (it2!=m_UnlockTypeMap.end()) {
            data.freeCount = data.totalCount;
        }
        
    }
//    for (int i = 0; i<m_UnlockTypeStr.size(); i++) {
//        std::string item = m_UnlockTypeStr.at(i);
//        if (strcmp(item.c_str(), key.c_str())==0) {
//            data.freeCount = data.totalCount;
//        }else if (strcmp(item.c_str(), "holiday")==0){
//            data.holidayCount = 0;
//        }
//    }
    data.display();
    return data;
}

std::vector<DecorateTypeConfigData> ConfigManager::getDecorateType(const std::string key)
{
    ValueMap map = _getDecorateTypeMap();
    auto it = map.find(key);
    std::vector<DecorateTypeConfigData> vec;
    if (it==map.end()) {
        return vec;
    }
    ValueVector vct = it->second.asValueVector();
    for (auto it = vct.begin(); it!=vct.end(); ++it) {
        DecorateTypeConfigData data;
        data.init();
        ValueMap map = it->asValueMap();
        auto itTypeName = map.find("TypeName");
        if (itTypeName!=map.end()) {
            data.decTypeName = itTypeName->second.asString();
        }
        auto itIconImageName = map.find("IconImageName");
        if (itIconImageName!=map.end()) {
            data.decIconName = itIconImageName->second.asString();
        }
        auto itPathName = map.find("path");
        if (itPathName!=map.end()) {
            data.decPath = itPathName->second.asString();
        }
        if (data.decPath=="") {
            data.decPath = data.decTypeName;
        }
        data.display();
        vec.push_back(data);
    }
    return vec;
}

int ConfigManager::getPrice(int index)
{
    ValueVector vct = getPriceDataVector();
    if (index>=vct.size()) {
        log("index is out of the price array count");
        return 0;
    }
    return vct.at(index).asInt();    
}

IAPConfigData ConfigManager::getIapData(int index)
{
    ValueVector vct = getIapDataVector();
    IAPConfigData data;
    data.init();

    if (index>=vct.size()) {
        return data;
    }
    auto it = vct.at(index);
    
    ValueMap map = it.asValueMap();
    
    std::string str = "iap_ios";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    str = "iap_android";
#endif
    auto itIAPId = map.find(str);
    if (itIAPId!=map.end()) {
        data.iapId   = itIAPId->second.asString();
    }
    auto itIconName  = map.find("iconname");
    if (itIconName !=map.end()) {
        data.iconName   = itIconName ->second.asString();
    }
    auto itUnlockItem = map.find("unlock");
    if (itUnlockItem!=map.end()) {
        data.unlockItem = StringHelp::split(itUnlockItem->second.asString(), ",");
    }
    data.display();
    
    return data;
}

IAPConfigData ConfigManager::getIapData(const std::string key)
{
    int count = ConfigManager::getInstance()->getIapDataVector().size();
    for (int i = 0; i<count; i++) {
        IAPConfigData data = ConfigManager::getInstance()->getIapData(i);
        if (strcmp(key.c_str(), data.iapId.c_str())==0) {
            return data;
        }
    }
    IAPConfigData data;
    data.init();
    return data;
}



void ConfigManager::unlock(int index){
    int count = ConfigManager::getInstance()->getIapDataVector().size();
    if (index>=count) {
        log("===ind out of the range===");
        return;
    }
    IAPConfigData data = ConfigManager::getInstance()->getIapData(index);
    for_each(data.unlockItem.begin(), data.unlockItem.end(), [=](std::string item){
        ConfigManager::getInstance()->unlockDecorateConfigData(item);
    });
}


void ConfigManager::unlock(const std::string key){
    IAPConfigData tempData = ConfigManager::getInstance()->getIapData(1);
//    if (strcmp(tempData.iapId.c_str(), key.c_str())==0) {
//        unlockDecorateConfigData("holiday");
//        return;
//    }
    IAPConfigData data = ConfigManager::getInstance()->getIapData(key);
    for_each(data.unlockItem.begin(), data.unlockItem.end(), [=](std::string item){
        ConfigManager::getInstance()->unlockDecorateConfigData(item);
    });
}
void ConfigManager::restoreDecorateConfigData()
{
    for (int i = 0; i<ConfigManager::getInstance()->getIapDataVector().size(); i++) {
        if (IAPManager::getInstance()->getItemIsBought(i)) {
//            if (i!=1) {
//                unlock(i);
//            }else{
//                unlockDecorateConfigData("holiday");
//            }
            unlock(i);
        }
    }
}
void ConfigManager::unlockDecorateConfigData(const std::string typeName)
{
    m_UnlockTypeMap.insert(ValueMap::value_type(typeName, Value(true)));
}