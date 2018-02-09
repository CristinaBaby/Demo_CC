
#include "IAPUnit.h"
#include "StringHelp.h"

#define kIAPId "ID"
#define kIAPImageName "ImageName"
#define kIAPKeyString "KeyString"
#define kIAPTypeNames "TypeNames"

IAPUnit::IAPUnit()
{
}

IAPUnit::~IAPUnit()
{
}

IAPUnit* IAPUnit::createWithDic(ValueMap &map)
{
    IAPUnit* unit = new IAPUnit();
    if (unit && unit->initWithDic(map)) {
        unit->autorelease();
        return unit;
    }
    delete unit;
    unit = NULL;
    return NULL;
    
}

bool IAPUnit::initWithDic(ValueMap &map)
{    
    CCAssert(map.size()!=0, "iap unit init with a null dic");
    auto it = map.find(kIAPId);
    if (it != map.end()) {
        m_pID = it->second.asString();
    }
    
    it = map.find(kIAPImageName);
    if (it != map.end()) {
        m_pImageName = it->second.asString();
    }
    
    it = map.find(kIAPKeyString);
    if (it != map.end()) {
        m_pKeyString = it->second.asString();
    }
    
    it = map.find(kIAPTypeNames);
    std::string str  = "";
    if (it != map.end()) {
        str = it->second.asString();
    }
    m_pTypeNamesVector = StringHelp::split(str, ",");
    for_each(m_pTypeNamesVector.begin(), m_pTypeNamesVector.end(), [=](std::string typeName){
        log("====%s",typeName.c_str());
    });
    return true;
}

IAPUnit* IAPUnit::create(string sID,
                       string sKeyString,
                       string sImageName,
                         std::vector<std::string> vct)
{
    IAPUnit* unit = new IAPUnit();
    unit->autorelease();
    
    unit->m_pID = sID;
    unit->m_pKeyString = sKeyString;
    unit->m_pImageName = sImageName;
    unit->m_pTypeNamesVector.swap(vct);
    return unit;
    
    delete unit;
    unit = NULL;
    return NULL;

}

//bool IAPUnit::init(string sID,
//          string sKeyString,
//          string sImageName)
//{
//    
//}