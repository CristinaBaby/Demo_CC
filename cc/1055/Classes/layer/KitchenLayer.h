//
//  KitchenLayer.hpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/30.
//
//

#ifndef KitchenLayer_hpp
#define KitchenLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Configure.h"
#include "MainScene.h"
#include "PopAcgLayer.h"
USING_NS_CC;

enum KitchenType{
    kitchen_material = 0,
    kitchen_noodles ,
    kitchen_oven ,
    kitchen_dec ,
    kitchen_choose ,
    kitchen_door
    
};




class KitchenLayer :public  Layer{
    KitchenLayer();
    ~KitchenLayer();
    
    
    
public:
    bool init(KitchenType type);
    static KitchenLayer* create(KitchenType type);
    static Scene* createScene(KitchenType type);
    
    static void loading(KitchenType type = KitchenType::kitchen_material,std::function<void()> end = nullptr);
    
//    CREATE_FUNC(KitchenLayer);
//    CREATE_SCENE(KitchenLayer);
private:
    Sprite* m_pBg;
    
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
private:
    void bgAction();
    
    std::function<void()> endFunc;
    
    KitchenType _type;
    Size _visibleSize;
    Sprite* m_pDoor;
};


#endif /* KitchenLayer_hpp */
