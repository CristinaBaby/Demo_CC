
#include "MemoryManager.h"
#include "cocostudio/CocoLoader.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CCArmature.h"



using namespace cocos2d::ui;
using namespace cocostudio;

MemoryManager::MemoryManager()
{
    
}

MemoryManager::~MemoryManager()
{
}



void MemoryManager::addArmaturePath(const std::string path,const std::string classStr)
{
    log("----%s",classStr.c_str());
    auto it = m_pArmatureMap.find(classStr);
    ValueVector lVector;
    if (it != m_pArmatureMap.end()){
        lVector = it->second.asValueVector();
    }
    lVector.push_back(Value(path));
    m_pArmatureMap.insert(ValueMap::value_type(classStr,Value(lVector)));
}

void MemoryManager::cleanArmature(const std::string classStr)
{
    log("----%s",classStr.c_str());
    auto it = m_pArmatureMap.find(classStr);
    ValueVector lVector;
    if (it != m_pArmatureMap.end()){
        lVector = it->second.asValueVector();
        for_each(lVector.begin(),lVector.end(),[&](Value data){
            log("====remove Armature Data %s",data.asString().c_str());
            ArmatureDataManager::getInstance()->removeArmatureFileInfo(data.asString());
        });
        lVector.clear();
    }
    
}

void MemoryManager::addTexturePath(const std::string path, const std::string classStr)
{
    auto it = m_pTextureMap.find(classStr);
    ValueVector lVector;
    if (it != m_pTextureMap.end()){
        lVector = it->second.asValueVector();
    }
    lVector.push_back(Value(path));
    m_pTextureMap.insert(ValueMap::value_type(classStr,Value(lVector)));
}


void MemoryManager::cleanTexture(const std::string classStr)
{
    auto it = m_pTextureMap.find(classStr);
    ValueVector lVector;
    if (it != m_pTextureMap.end()){
        lVector = it->second.asValueVector();
        for_each(lVector.begin(),lVector.end(),[&](Value data){
            log("====remove Texture Cache data %s",data.asString().c_str());
            Director::getInstance()->getTextureCache()->removeTextureForKey(data.asString());
        });
        lVector.clear();
    };
    
}

void MemoryManager::clean(const std::string classStr)
{
    cleanArmature(classStr);
    cleanTexture(classStr);
}



