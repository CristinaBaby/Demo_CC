//
// AnimalModel.h
// ANIM3203
//
// Created by dengcheng on 15/9/14.
//
//

#ifndef __ANIM3203__AnimalModel__
#define __ANIM3203__AnimalModel__

#include "cocos2d.h"
#include <cocostudio/CocoStudio.h>
#include <ui/CocosGUI.h>
#include "ScribbleTouchNode.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class AnimalModel : public Node
{
public:
    
    typedef enum {
        kNone,
        kTabby,
        kBeagle,
        kRagDoll,
        kSchnauzer
    }Animal;
    
    typedef enum {
        kClothing,
        kGlasses,
        kHeadWear,
        kNecklace,
        kShoe
    }ClothType;
    
public:
    AnimalModel();
    ~AnimalModel();

    static AnimalModel* create(Animal aAnimal);
    virtual bool init(Animal aAnimal);
    
    void addBubble(Sprite *aBubble, const Vec2 &aWorldPos);
    
    void completeAddBubble();
    
    void removeBubble(float aRange, const Vec2 &aWorldPos);
    
    void completeRemoveBubble();
    
    void blowAnimalFurs(const Vec2 &aWorldPos);
    
    void endBlowAnimalFurs();
    
    void clearBubble();
    
    void cutFurs(float aRange, const Vec2 &aWorldPos);
    
    bool cutFingernail(const Vec2 &aWorldPos);
    
    inline Vec2 getAnimalFootPos() {
        return m_pBody->convertToWorldSpace(Vec2(m_pBody->getContentSize().width * 0.5, 70));
    }
    
    const Animal &getAnimalType()
    {
        return m_eAnimalType;
    }
    
    void changeClothing(ClothType aType, Sprite *aClothing);
    
    void clearCloth();
    
protected://method

	void onEnter() override;
    
	void onExit() override;

private://Variables
    
    static AnimalModel *s_Instance;
    
    Animal m_eAnimalType;
    
    bool m_bFurAnimate;
    
    bool m_bCloseEyesAnimate;
    
    bool m_bUseTool;
    
    Image *m_pAnimalHeadImage;
    
    Image *m_pAnimalBodyImage;
    
    Sprite *m_pBody;
    
    Sprite *m_pHead;
    
    Sprite *m_pTail;
    
    Sprite *m_pEyes;
    
    Sprite *m_pMouse;
    
    Sprite *m_pNose;
    
    ScribbleTouchNode *m_pHeadOrdure;
    
    ScribbleTouchNode *m_pBodyOrdure;
    
    std::vector<Sprite *> m_vBubbles;
    
    std::vector<Sprite *> m_vFurs;
    
    std::vector<Sprite *> m_vFingernails;
    
private://method
    
    void playAnimation();
    
    void closeEyes();
    
    void openEyes();
    
    void shakeFurs();
    
    void closeShakeFurs();
    
    void loadAnimal();
    
    void clearAnimal();
    
    void visitAnimalImage();
    
    void loadAnimalDynamicObject();
    
    bool touchAtImageIsAlpha(Image *aImage, const Vec2 &aPos, float aAlpha);
};

#endif /* defined(__ANIM3203__AnimalModel__) */