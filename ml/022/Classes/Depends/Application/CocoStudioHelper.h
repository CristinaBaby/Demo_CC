
#ifndef __CocoStudioHelper__
#define __CocoStudioHelper__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CCArmature.h"



using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;

class CocoStudioHelper{
public:
    static BaseData* getBoneWorldInfo(Armature* aArmature,const std::string boneName);
    static Point getBonePoint(Armature* aArmature,const std::string boneName);
    static Point getBoneWorlPoint(Armature* aArmature,const std::string boneName);
    static std::string getBoneName(Armature* aArmature,const std::string boneName);
    static Sprite* getBoneSprite(Armature* aArmature,const std::string boneName);
    static  Bone* getBone(Armature* aArmature,const std::string childBoneName,const std::string parentBoneName);
};
#endif /* defined(__CocoStudioHelper__) */
