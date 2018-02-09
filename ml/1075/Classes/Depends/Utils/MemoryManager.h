
#ifndef _____MemoryManager__
#define _____MemoryManager__


#include "SingleTon.h"
#include "cocos2d.h"

USING_NS_CC;


#define ClassString(T) std::string(typeid(T).name())
#define ClassAddressString(T) std::string(&T)

#define cleanCache(T) MemoryManager::getInstance()->clean(ClassString(T))

class MemoryManager : public SingleTon<MemoryManager>
{
public:
    MemoryManager();
    ~MemoryManager();
    
    void addArmaturePath(const std::string path,const std::string classStr);
    void cleanArmature(const std::string classStr);
    
    void addTexturePath(const std::string path,const std::string classStr);
    void cleanTexture(const std::string classStr);
    
    void clean(const std::string classStr);
protected:

    ValueMap m_pArmatureMap;
    ValueMap m_pTextureMap;
};

#endif