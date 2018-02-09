//
//  FruitSprite.cpp
//  test-cocos2dx
//
//  Created by wusonglin on 15/12/12.
//
//

#include "FruitSprite.h"
enum color{
    GREEN = 0,
    RED,
    YELLOW,
    PURPLE,
    BLUE
};
FruitSprite::FruitSprite()
{
    //每个水果的状态 0表示静止 1表示运动 2表示需要播放弹动动画
    state = 0;
    
    m_bAdjustX = false;
    m_bAdjustY = false;
}
FruitSprite::~FruitSprite()
{
    
}
FruitSprite* FruitSprite::create(int color)
{
    FruitSprite* ret = new FruitSprite();
    if(ret && ret->init(color)){
        ret->color = color;
        ret->selected = false;
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
    

}
bool FruitSprite::init(int color)
{
    if (!initWithFile(this->getImage(color)->getCString())) {
        return false;
    }
    return true;
}
__String* FruitSprite::getImage(int color)
{
    switch(color){
        case color::BLUE:
            return __String::create("images/pop_fruit/bottom_blueberry.png");
        case color::GREEN:
            return  __String::create("images/pop_fruit/bottom_kiwi.png");
        case color::YELLOW:
            return  __String::create("images/pop_fruit/bottom_lemon.png");
        case color::RED:
            return  __String::create("images/pop_fruit/bottom_strawberry.png");
        case color::PURPLE:
            return  __String::create("images/pop_fruit/bottom_orange.png");
    }
    return nullptr;
}

bool FruitSprite::isSelected(){
    return selected;
}

int FruitSprite::getColor(){
    return color;
}
void FruitSprite::setDesPosition(const Vec2 p){
    desPosition = p;
}
//根据目标点设置方块的位置
void FruitSprite::updatePosition()
{
   if(desPosition.y != getPositionY()){
       
        setPositionY(getPositionY() - MOVE_SPEED);
      
       m_bAdjustY = true;
        if(getPositionY() < desPosition.y){
            setPositionY(desPosition.y);
        }
       
        //运动状态
        state = 1;
       
    }
    if(desPosition.x != getPositionX()){
        setPositionX(getPositionX() - MOVE_SPEED);
        
        m_bAdjustX = true;
        
        if(getPositionX() < desPosition.x){
            setPositionX(desPosition.x);
        }
        state = 1;
    }
    if(desPosition == this->getPosition()){
    
        if (state == 1) {
            
            state = 0;
            
            if (m_bAdjustY) {
                m_bAdjustY = false;
                float timeScale = 0.9;
                ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.1, 0.80);
                ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.1);
                ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
                ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
                this->runAction(Sequence::create(lScale1, lScale2, lScale3,EaseElasticOut::create(lScale6,1.0), NULL));
                
            }else
            if (m_bAdjustX) {
                
                m_bAdjustX = false;
                
                float timeScale = 0.9;
                ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 0.8, 1.1);
                ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 1.1, 0.8);
                ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
                ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
                this->runAction(Sequence::create(lScale1, lScale2, lScale3,EaseElasticOut::create(lScale6,1.0), NULL));
            }
//            ActionInterval *ret = NULL;
//            float timeScale = 1.2;
//            ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
//            ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
//            ScaleTo *lScale3 = ScaleTo::create(0.10*timeScale, 1.0, 0.86);
//            ScaleTo *lScale4 = ScaleTo::create(0.09*timeScale, 0.88, 1.0);
//            ScaleTo *lScale5 = ScaleTo::create(0.08*timeScale, 1.0, 0.89);
//            ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
//
//            this->runAction(Sequence::create(lScale1, lScale2, lScale3, lScale4, lScale5,EaseElasticOut::create(lScale6,1.0), NULL));
        }
    }

}
bool FruitSprite::isCheckPosAndDes()
{

    if(desPosition == this->getPosition())
        return true;
    
    return false;

}

