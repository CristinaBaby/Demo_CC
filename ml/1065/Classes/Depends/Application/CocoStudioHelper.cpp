
#include "CocoStudioHelper.h"

BaseData* CocoStudioHelper::getBoneWorldInfo(cocostudio::Armature *aArmature, const std::string boneName)
{
    Bone*lBone = aArmature->getBone(boneName);
    return lBone->getWorldInfo();
}

Point CocoStudioHelper::getBonePoint(Armature* aArmature, const std::string boneName)
{
    BaseData* lWorldInfo = getBoneWorldInfo(aArmature,boneName);
    Point point = Vec2::ZERO;
    point.x = lWorldInfo->x;
    point.y = lWorldInfo->y;
    return point;
}

Point CocoStudioHelper::getBoneWorlPoint(Armature* aArmature, const std::string boneName)
{
    Point point = getBonePoint(aArmature, boneName);
    point = aArmature->convertToWorldSpace(point);
    return point;
}

std::string CocoStudioHelper::getBoneName(Armature *aArmature, const std::string boneName)
{
    Bone*lBone = aArmature->getBone(boneName);
    BoneData* lBoneData = lBone->getBoneData();
    DisplayData* lDisplayData = lBoneData->getDisplayData(0);
    return lDisplayData->displayName;
}
Sprite* CocoStudioHelper::getBoneSprite(Armature *aArmature, const std::string boneName)
{
    Bone*lBone = aArmature->getBone(boneName);
    BoneData* lBoneData = lBone->getBoneData();
    DisplayData* lDisplayData = lBoneData->getDisplayData(0);
    Sprite* sprite = CCSprite::createWithSpriteFrameName(lDisplayData->displayName.c_str());
    sprite->setPosition(getBoneWorlPoint(aArmature, boneName));
    return sprite;
}

 Bone* CocoStudioHelper::getBone(cocostudio::Armature *aArmature, const std::string childBoneName, const std::string parentBoneName)
{
    
    Bone* pBoneChild = aArmature->getBone(childBoneName);
    if (!pBoneChild) {
        pBoneChild = Bone::create(childBoneName);
        aArmature->addBone(pBoneChild, parentBoneName);
        if (aArmature->getBone(parentBoneName)) {
            pBoneChild->setZOrder(aArmature->getBone(parentBoneName)->getZOrder());
        }
    }
    return pBoneChild;
}