//
// AnimalModel.cpp
// ANIM3203
//
// Created by dengcheng on 15/9/14.
//
//

#include "AnimalModel.h"
#include "AnimalPos.h"
#include <time.h>

static const std::string s_AnimalName[] = {"", "tabby", "beagle", "ragdoll", "schnauzer"};

static const std::string s_AnimalFur[5][13] = {
    {"", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "model/tabby/fur_2.png",
        "model/tabby/fur_2.png",
        "model/tabby/fur_2.png",
        "model/tabby/fur_2.png",
        "model/tabby/fur_1.png",
        "model/tabby/fur_1.png",
        "model/tabby/fur_1.png",
        "model/tabby/fur_1.png",
        "model/tabby/fur_1.png",
        "model/tabby/fur_1.png",
        "model/tabby/fur_2.png",
        "model/tabby/fur_2.png"},
    {"", "model/beagle/fur_4.png",
        "model/beagle/fur_4.png",
        "model/beagle/fur_3.png",
        "model/beagle/fur_3.png",
        "model/beagle/fur_2.png",
        "model/beagle/fur_2.png",
        "model/beagle/fur_1.png",
        "model/beagle/fur_1.png",
        "model/beagle/fur_6.png",
        "model/beagle/fur_6.png",
        "model/beagle/fur_5.png",
        "model/beagle/fur_5.png",},
    {"", "model/ragdoll/fur_1.png",
        "model/ragdoll/fur_2.png",
        "model/ragdoll/fur_3.png",
        "model/ragdoll/fur_4.png",
        "model/ragdoll/fur_5.png",
        "model/ragdoll/fur_6.png",
        "model/ragdoll/fur_7.png",
        "model/ragdoll/fur_8.png",
        "model/ragdoll/fur_4.png",
        "model/ragdoll/fur_4.png",
        "model/ragdoll/fur_3.png",
        "model/ragdoll/fur_3.png"},
    {"", "model/schnauzer/fur_2.png",
        "model/schnauzer/fur_2.png",
        "model/schnauzer/fur_2.png",
        "model/schnauzer/fur_2.png",
        "model/schnauzer/fur_1.png",
        "model/schnauzer/fur_1.png",
        "model/schnauzer/fur_1.png",
        "model/schnauzer/fur_1.png",
        "model/schnauzer/fur_3.png",
        "model/schnauzer/fur_3.png",
        "model/schnauzer/fur_4.png",
        "model/schnauzer/fur_4.png"}
};

static const std::string sClothName[] = {"AnimalClothing", "AnimalGlasses", "AnimalHeadWear", "AnimalNecklace", "AnimalShoe" };

AnimalModel *AnimalModel::s_Instance = nullptr;

#define kArmatureManager ArmatureDataManager::getInstance()

AnimalModel::AnimalModel()
:m_eAnimalType(kRagDoll)
,m_pAnimalHeadImage(nullptr)
,m_pAnimalBodyImage(nullptr)
,m_vFurs(std::vector<Sprite *>())
,m_vFingernails(std::vector<Sprite *>())
,m_vBubbles(std::vector<Sprite *>())
,m_bFurAnimate(false)
,m_bCloseEyesAnimate(false)
,m_bUseTool(false)
,m_pHeadOrdure(nullptr)
,m_pBodyOrdure(nullptr)
,m_pNose(nullptr)
{
    
}

void AnimalModel::onEnter()
{
    Node::onEnter();
}

AnimalModel* AnimalModel::create(Animal aAnimal)
{
    if (!s_Instance) {
        s_Instance = new AnimalModel();
    }
    s_Instance->init(aAnimal);
    return s_Instance;
}

bool AnimalModel::init(Animal aAnimal)
{
    if ( !Node::init() )
    {
        return false;
    }
    
    if (aAnimal == kNone) {
        removeFromParent();
        playAnimation();
        return true;
    }
    
    clearAnimal();
    
    srand((int)time(0));
    
    m_eAnimalType = aAnimal;
    
    loadAnimal();
    
    visitAnimalImage();
    
    loadAnimalDynamicObject();
    
    playAnimation();
    
    return true;
}

void AnimalModel::addBubble(cocos2d::Sprite *aBubble, const cocos2d::Vec2 &aWorldPos)
{
    
    m_bUseTool = true;
    
    closeEyes();
    
    int parentTag = 0;
    
    do {
        
        parentTag = touchAtImageIsAlpha(m_pAnimalHeadImage, m_pHead->convertToNodeSpace(aWorldPos), 180) ? 1 : 0;
        
        if (parentTag != 0) {
            break;
        }
        
        parentTag = touchAtImageIsAlpha(m_pAnimalBodyImage, m_pBody->convertToNodeSpace(aWorldPos), 180) ? 2 : 0;
        
    } while (false);
    
    if (parentTag == 0) {
        return;
    }
    
    for (auto bubble : m_vBubbles) {
        Vec2 bubbleWorldPos = bubble->getParent()->convertToWorldSpace(bubble->getPosition());
        if (aWorldPos.distance(bubbleWorldPos) <= 15) {
            return;
        }
    }
    
    m_vBubbles.push_back(aBubble);
    aBubble->setScale(0.1f);
    aBubble->setOpacity(1);
    
    if (parentTag == 1) {
        m_pHeadOrdure->paint(aWorldPos);
        aBubble->setPosition(m_pHead->convertToNodeSpace(aWorldPos));
        m_pHead->addChild(aBubble,30);
    } else {
        m_pBodyOrdure->paint(aWorldPos);
        aBubble->setPosition(m_pBody->convertToNodeSpace(aWorldPos));
        m_pBody->addChild(aBubble,30);
    }
    
    float scale = ((2.0 + rand() % 5) / 10.0);
    aBubble->runAction(Sequence::create(EaseSineOut::create(Spawn::create(FadeTo::create(0.5, 255), ScaleTo::create(0.5, scale), nullptr)),nullptr));
    
}

void AnimalModel::completeAddBubble()
{
    m_bUseTool = false;
    openEyes();
}

void AnimalModel::removeBubble(float aRange, const cocos2d::Vec2 &aWorldPos)
{
    m_bUseTool = true;
    closeEyes();
    
    auto bubbleItr = m_vBubbles.begin();
    
    while (bubbleItr != m_vBubbles.end()) {
        
        Sprite *pBubble = *bubbleItr;
        
        Vec2 worldPos = pBubble->getParent()->convertToWorldSpace(pBubble->getPosition());
        
        if (aWorldPos.distance(worldPos) < aRange) {
            bubbleItr = m_vBubbles.erase(bubbleItr);
            pBubble->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.5, 0), FadeTo::create(0.5, 1), nullptr), RemoveSelf::create(), nullptr));
        } else {
            ++ bubbleItr;
        }
        
    }
}

void AnimalModel::completeRemoveBubble()
{
    m_bUseTool = false;
    openEyes();
}

void AnimalModel::blowAnimalFurs(const cocos2d::Vec2 &aWorldPos)
{
    shakeFurs();
    
    for (auto fur : m_vFurs) {
        Vec2 worldPos = fur->getParent()->convertToNodeSpace(aWorldPos);
        float angle = Vec2(0, 1000).getAngle(worldPos - fur->getPosition()) * 180.0 / M_PI;
        fur->setRotation(-angle);
    }
    
}

void AnimalModel::endBlowAnimalFurs()
{
    closeShakeFurs();
}


void AnimalModel::clearBubble()
{
    
    auto bubbleItr = m_vBubbles.begin();
    
    while (bubbleItr != m_vBubbles.end()) {
        Sprite *pBubble = *bubbleItr;
        bubbleItr = m_vBubbles.erase(bubbleItr);
        pBubble->removeFromParent();
    }
}

void AnimalModel::cutFurs(float aRange, const cocos2d::Vec2 &aWorldPos)
{
    auto furItr = m_vFurs.begin();
    
    while (furItr != m_vFurs.end()) {
        Sprite *pFur = *furItr;
        Vec2 worldPos = pFur->getParent()->convertToWorldSpace(pFur->getPosition());
        if (worldPos.distance(aWorldPos) <= aRange) {
            furItr = m_vFurs.erase(furItr);
            pFur->runAction(Sequence::create(Spawn::create(JumpBy::create(1, Vec2(0, -10), 20, 1), FadeTo::create(1, 0), nullptr), RemoveSelf::create(), nullptr));
        } else {
            ++ furItr;
        }
    }
}

bool AnimalModel::cutFingernail(const cocos2d::Vec2 &aWorldPos)
{
    auto fingernailItr = m_vFingernails.begin();
    
    while (fingernailItr != m_vFingernails.end()) {
        Sprite *pFingernail = *fingernailItr;
        if (pFingernail->getBoundingBox().containsPoint(m_pBody->convertToNodeSpace(aWorldPos))) {
            fingernailItr = m_vFingernails.erase(fingernailItr);
            pFingernail->runAction(Sequence::create(Spawn::create(JumpBy::create(1, Vec2(0, -10), 20, 1), FadeTo::create(1, 0), nullptr), RemoveSelf::create(), nullptr));
            return true;
        } else {
            ++ fingernailItr;
        }
    }
    return false;
}

void AnimalModel::changeClothing(ClothType aType, Sprite *aClothing)
{
    
    switch (aType)
    {
        case kClothing:
        {
            Node *pCloth = m_pBody->getChildByName(sClothName[aType]);
            aClothing->setName(sClothName[aType]);
            aClothing->setPosition(pCloth->getPosition());
            aClothing->setLocalZOrder(pCloth->getLocalZOrder());
            pCloth->removeFromParent();
            m_pBody->addChild(aClothing);
        }
            break;
        case kGlasses:
        {
            Node *pCloth = m_pHead->getChildByName(sClothName[aType]);
            aClothing->setName(sClothName[aType]);
            aClothing->setPosition(pCloth->getPosition());
            aClothing->setLocalZOrder(pCloth->getLocalZOrder());
            pCloth->removeFromParent();
            m_pHead->addChild(aClothing);
        }
            break;
        case kHeadWear:
        {
            Node *pCloth = m_pHead->getChildByName(sClothName[aType]);
            aClothing->setName(sClothName[aType]);
            aClothing->setPosition(pCloth->getPosition());
            aClothing->setLocalZOrder(pCloth->getLocalZOrder());
            pCloth->removeFromParent();
            m_pHead->addChild(aClothing);
        }
            break;
        case kNecklace:
        {
            Node *pCloth = m_pBody->getChildByName(sClothName[aType]);
            aClothing->setName(sClothName[aType]);
            aClothing->setPosition(pCloth->getPosition());
            aClothing->setLocalZOrder(pCloth->getLocalZOrder());
            pCloth->removeFromParent();
            m_pBody->addChild(aClothing);
        }
            break;
        case kShoe:
        {
            Node *pCloth = m_pBody->getChildByName(sClothName[aType]);
            aClothing->setName(sClothName[aType]);
            aClothing->setPosition(pCloth->getPosition());
            aClothing->setLocalZOrder(pCloth->getLocalZOrder());
            pCloth->removeFromParent();
            m_pBody->addChild(aClothing);
        }
            break;
    }
}

/*=================private method======================*/

void AnimalModel::playAnimation()
{
    openEyes();
    m_pHead->runAction(RepeatForever::create(Sequence::create(RotateBy::create(1.5, 2), RotateBy::create(1.5, -2), RotateBy::create(1.5, -2), RotateBy::create(1.5, 2), nullptr)));
    m_pTail->runAction(RepeatForever::create(Sequence::create(RotateBy::create(1, 12), RotateBy::create(1, -12), RotateBy::create(1, -12), RotateBy::create(1, 12), nullptr)));
    m_pMouse->runAction(RepeatForever::create(Sequence::create(DelayTime::create(3), CallFunc::create([this](){
        if (m_eAnimalType == kTabby || m_eAnimalType == kRagDoll) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("sound/Pets/cat_%d.mp3", (1 + rand() % 5)).c_str());
        } else {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("sound/Pets/dog_%d.mp3", (1 + rand() % 5)).c_str());
        }
        
        m_pMouse->setVisible(false);
        Sprite *pCloseMous = (Sprite *)m_pMouse->getUserData();
        pCloseMous->setVisible(true);
    }), DelayTime::create(0.3), CallFunc::create([this](){
        m_pMouse->setVisible(true);
        Sprite *pCloseMous = (Sprite *)m_pMouse->getUserData();
        pCloseMous->setVisible(false);
    }), nullptr)));
    m_pEyes->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2.5), CallFunc::create([this](){
        if (!m_bCloseEyesAnimate) {
            this->closeEyes();
        }
    }), DelayTime::create(0.5), CallFunc::create([this](){
        this->openEyes();
    }), nullptr)));
    if (m_pNose) {
        m_pNose->runAction(RepeatForever::create(Sequence::create(DelayTime::create(6),
                                                                  Spawn::create(ScaleTo::create(0.3, 1.1), MoveBy::create(0.3, Vec2(0, 10)),  nullptr),
                                                                  Spawn::create(ScaleTo::create(0.3, 1), MoveBy::create(0.3, Vec2(0, -10)),  nullptr),
                                                                  Spawn::create(ScaleTo::create(0.3, 1.1), MoveBy::create(0.3, Vec2(0, 10)),  nullptr),
                                                                  Spawn::create(ScaleTo::create(0.3, 1), MoveBy::create(0.3, Vec2(0, -10)),  nullptr),
                                                                  nullptr)));
    }
}

void AnimalModel::closeEyes()
{
    if (!m_bCloseEyesAnimate) {
        m_bCloseEyesAnimate =  true;
        m_pEyes->setVisible(false);
        Sprite *pClose = (Sprite *)m_pEyes->getUserData();
        pClose->setVisible(true);
    }
}

void AnimalModel::openEyes()
{
    if (m_bCloseEyesAnimate && !m_bUseTool) {
        m_bCloseEyesAnimate = false;
        m_pEyes->setVisible(true);
        Sprite *pClose = (Sprite *)m_pEyes->getUserData();
        pClose->setVisible(false);
    }
}

void AnimalModel::shakeFurs()
{
    if (!m_bFurAnimate) {
        m_bFurAnimate = true;
        for (auto fur : m_vFurs) {
            fur->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.1), FlipX::create(true), DelayTime::create(0.1), FlipX::create(false), nullptr)));
        }
    }
}

void AnimalModel::closeShakeFurs()
{
    if (m_bFurAnimate) {
        m_bFurAnimate = false;
        for (auto fur : m_vFurs) {
            fur->setRotation(0);
            fur->stopAllActions();
        }
    }
}

void AnimalModel::loadAnimal()
{
    m_pBody = Sprite::create(StringUtils::format("model/%s/body.png", s_AnimalName[m_eAnimalType].c_str()));
    m_pBody->setPosition(0, 65);
    addChild(m_pBody, 2);
    
    m_pHead = Sprite::create(StringUtils::format("model/%s/head.png", s_AnimalName[m_eAnimalType].c_str()));
    m_pHead->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    m_pHead->setPosition(g_sAnimalHeadPos[m_eAnimalType]);
    m_pBody->addChild(m_pHead, 70);
    
    m_pTail = Sprite::create(StringUtils::format("model/%s/tail.png", s_AnimalName[m_eAnimalType].c_str()));
    if (m_eAnimalType == Animal::kTabby || m_eAnimalType == Animal::kRagDoll) {
        m_pTail->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    } else {
        m_pTail->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    }
    m_pTail->setPosition(g_sAnimalTailPos[m_eAnimalType]);
    addChild(m_pTail, 1);
    
    m_pEyes = Sprite::create(StringUtils::format("model/%s/eye.png", s_AnimalName[m_eAnimalType].c_str()));
    m_pEyes->setPosition(g_sAnimalEyesPos[m_eAnimalType]);
    m_pHead->addChild(m_pEyes, 50);
    
    Sprite *pCloseEyes = Sprite::create(StringUtils::format("model/%s/close_eye.png", s_AnimalName[m_eAnimalType].c_str()));
    pCloseEyes->setPosition(m_pEyes->getPosition());
    pCloseEyes->setVisible(false);
    m_pHead->addChild(pCloseEyes, 30);
    m_pEyes->setUserData(pCloseEyes);
    
    m_pMouse = Sprite::create(StringUtils::format("model/%s/tongue_1.png", s_AnimalName[m_eAnimalType].c_str()));
    m_pMouse->setPosition(g_sAnimalMousePos[m_eAnimalType]);
    m_pHead->addChild(m_pMouse, 40);
    
    Sprite *pOpenMouse = Sprite::create(StringUtils::format("model/%s/tongue_2.png", s_AnimalName[m_eAnimalType].c_str()));
    pOpenMouse->setPosition(g_sAnimalMousePos[m_eAnimalType]);
    pOpenMouse->setVisible(false);
    m_pHead->addChild(pOpenMouse, 20);
    m_pMouse->setUserData(pOpenMouse);
    
    if (m_eAnimalType == Animal::kBeagle || m_eAnimalType == Animal::kSchnauzer) {
        m_pNose = Sprite::create(StringUtils::format("model/%s/nose.png", s_AnimalName[m_eAnimalType].c_str()));
        if (m_eAnimalType == Animal::kBeagle) {
            m_pNose->setPosition(234, 176);
        } else {
            m_pNose->setPosition(225, 176);
        }
        m_pHead->addChild(m_pNose,55);
    }
    
    //初始化衣服
    Sprite *pCloth = Sprite::create();
    pCloth->setName(sClothName[0]);
    pCloth->setPosition(g_sAnimalClothPos[0][m_eAnimalType]);
    m_pBody->addChild(pCloth, 50);
    
    Sprite *pGlasses = Sprite::create();
    pGlasses->setName(sClothName[1]);
    pGlasses->setPosition(g_sAnimalDecorationPos[1][m_eAnimalType]);
    m_pHead->addChild(pGlasses, 70);
    
    Sprite *pHeadwear = Sprite::create();
    pHeadwear->setName(sClothName[2]);
    pHeadwear->setPosition(g_sAnimalDecorationPos[0][m_eAnimalType]);
    m_pHead->addChild(pHeadwear, 60);
    
    Sprite *pNecklace = Sprite::create();
    pNecklace->setName(sClothName[3]);
    pNecklace->setPosition(g_sAnimalDecorationPos[2][m_eAnimalType]);
    m_pBody->addChild(pNecklace, 60);
    
    Sprite *pShoe = Sprite::create();
    pShoe->setName(sClothName[4]);
    pShoe->setPosition(g_sAnimalClothPos[1][m_eAnimalType]);
    m_pBody->addChild(pShoe, 40);
}

void AnimalModel::visitAnimalImage()
{
    m_pAnimalHeadImage = new Image();
    m_pAnimalHeadImage->initWithImageFile(StringUtils::format("model/%s/head_mask.png", s_AnimalName[m_eAnimalType].c_str()));
    
    m_pAnimalBodyImage = new Image();
    m_pAnimalBodyImage->initWithImageFile(StringUtils::format("model/%s/body_mask.png", s_AnimalName[m_eAnimalType].c_str()));
}

void AnimalModel::clearAnimal()
{
    removeAllChildren();
    
    m_pNose = nullptr;
    
    m_bFurAnimate = false;
    m_bCloseEyesAnimate = false;
    m_bUseTool = false;
    
    m_vBubbles.clear();
    m_vFingernails.clear();
    m_vFurs.clear();
    
    if (m_pAnimalBodyImage) {
        delete m_pAnimalBodyImage;
        m_pAnimalBodyImage = nullptr;
    }
    
    if (m_pAnimalHeadImage) {
        delete m_pAnimalHeadImage;
        m_pAnimalHeadImage = nullptr;
    }
}

void AnimalModel::clearCloth()
{
    for (int i=0; i<5; ++i) {
        Node *pCloth = m_pBody->getChildByName(sClothName[i]);
        if (!pCloth) {
            pCloth = m_pHead->getChildByName(sClothName[i]);
        }
        if (pCloth) {
            
            Node *pParent = pCloth->getParent();
            std::string name = pCloth->getName();
            Vec2 pos = pCloth->getPosition();
            int zorder = pCloth->getLocalZOrder();
            pCloth->removeFromParent();
            
            Sprite *pNewCloth = Sprite::create();
            pNewCloth->setName(name);
            pNewCloth->setPosition(pos);
            pNewCloth->setLocalZOrder(zorder);
            pParent->addChild(pNewCloth);
        }
    }
}

void AnimalModel::loadAnimalDynamicObject()
{
    
    m_vFingernails.clear();
    m_vFurs.clear();
    
    for (int i=1; i<=12; i++) {
        Sprite *pFingernail = Sprite::create(StringUtils::format("model/fingernail/fingernail_%d.png", i));
        pFingernail->setPosition(g_sAnimalFingernailPos[m_eAnimalType][i]);
        m_pBody->addChild(pFingernail);
        m_vFingernails.push_back(pFingernail);
    }
    
    for (int i=1; i<= 12; ++i) {
        Sprite *pFur = nullptr;
        pFur = Sprite::create(s_AnimalFur[m_eAnimalType][i]);
        if (i > 8) {
            m_pHead->addChild(pFur, 20);
        } else {
            m_pBody->addChild(pFur, 20);
        }
        pFur->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        pFur->setPosition(g_sAnimalFurPos[m_eAnimalType][i]);
        m_vFurs.push_back(pFur);
    }
    
    std::string headOrdurePath = StringUtils::format("model/%s/ordure_1.png", s_AnimalName[m_eAnimalType].c_str());
    Sprite *pHeadOrdure = Sprite::create(headOrdurePath);
    
    m_pHeadOrdure = ScribbleTouchNode::create(pHeadOrdure->getContentSize());
    m_pHeadOrdure->setPosition(g_sAnimalOrdure[m_eAnimalType][2]);
    m_pHead->addChild(m_pHeadOrdure, 10);
    
    m_pHeadOrdure->useBrush(headOrdurePath, Scribble::eBrush);
    m_pHeadOrdure->useTarget(headOrdurePath);
    m_pHeadOrdure->paint(m_pHead->convertToWorldSpace(m_pHeadOrdure->getPosition()));
    m_pHeadOrdure->useBrush("spa/brush1.png", Scribble::eEaser);
    m_pHeadOrdure->setBrushSolid(false, 1.0);
    
    std::string bodyOrdurePath = StringUtils::format("model/%s/ordure_2.png", s_AnimalName[m_eAnimalType].c_str());
    Sprite *pBodyOrdure = Sprite::create(bodyOrdurePath);
    
    m_pBodyOrdure = ScribbleTouchNode::create(pBodyOrdure->getContentSize());
    m_pBodyOrdure->setPosition(g_sAnimalOrdure[m_eAnimalType][1]);
    m_pBody->addChild(m_pBodyOrdure, 10);
    
    m_pBodyOrdure->useBrush(bodyOrdurePath, Scribble::eBrush);
    m_pBodyOrdure->useTarget(bodyOrdurePath);
    m_pBodyOrdure->paint(m_pBody->convertToWorldSpace(m_pBodyOrdure->getPosition()));
    m_pBodyOrdure->useBrush("spa/brush1.png", Scribble::eEaser);
    m_pBodyOrdure->setBrushSolid(false, 1.0);
    
    m_pHeadOrdure->disable();
    m_pBodyOrdure->disable();
}

bool AnimalModel::touchAtImageIsAlpha(Image *aImage, const Vec2 &aPos, float aAlpha)
{
    
    Image *pImage = aImage;
    
    unsigned char *data = pImage->getData();
    
    //only support rgba888
    const int bytesPerPixel = 4;
    
    int row = pImage->getHeight() - aPos.y;
    int col = aPos.x;
    
    if (row < 0 || row >= pImage->getHeight()) {
        return false;
    }
    
    if (col < 0 || col >= pImage->getWidth()) {
        return false;
    }
    
    int index = row * pImage->getWidth() + col;
    index *= bytesPerPixel;
    
    float alpha = (3 == bytesPerPixel) ? 255 : data[index + 3];
    
    if (alpha > aAlpha) {
        return true;
    }
    
    return false;
}

void AnimalModel::onExit()
{
    
    Node::onExit();
}
AnimalModel::~AnimalModel()
{
    delete m_pAnimalHeadImage;
    delete m_pAnimalBodyImage;
}