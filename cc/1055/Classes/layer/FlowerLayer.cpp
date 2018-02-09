//
//  FlowerLayer.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin on 16/4/24.
//
//

#include "FlowerLayer.h"
#include "AudioHelp.h"

string fontPath[3]={"wonderful.png","welldone.png","great.png"};
string songPath[3]={"wonderful.mp3","well done.mp3","Great.mp3"};
FlowerLayer::FlowerLayer()
{


}
FlowerLayer::~FlowerLayer()
{


}

void FlowerLayer::Loading(Node* node,std::function<void()> endFunc)
{
    FlowerLayer* layer = FlowerLayer::create();
    layer->_endfunc = endFunc;
    node->addChild(layer,10000);
}

bool FlowerLayer::init()
{
    if(!Layer::init())
        return false;
    
    
    m_pAnumation = Sprite::create("cocostudio/UI/rewards/animation/donhua_1.png");
    m_pAnumation->setPosition(this->getContentSize()/2);
    this->addChild(m_pAnumation);
    m_pAnumation->setPositionY(m_pAnumation->getPositionY()-1000);
    
    return true;
    
}
void FlowerLayer::onEnter()
{
    Layer::onEnter();

    int num = rand()%3;
    auto font = Sprite::create(__String::createWithFormat("cocostudio/UI/tips/%s",fontPath[num].c_str())->getCString());
    font->setPosition(Director::getInstance()->getVisibleSize()/2);
    this->addChild(font);
    font->setScale(0);

    AudioHelp::getInstance()->playEffect(__String::createWithFormat("VoiceOver/%s",songPath[num].c_str())->getCString());
    
    font->runAction(Sequence::create(ScaleTo::create(0.05,3.0),ScaleTo::create(0.5,1.0),ScaleTo::create(0.25,1.5),ScaleTo::create(0.25,1.0),DelayTime::create(0.225),CallFunc::create([=](){
        
        auto pOvenAnimation = Animation::create();
        
        for(int i = 1 ; i< 9 ; i++){
            
            pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("cocostudio/UI/rewards/animation/donhua_%d.png",i)->getCString());
            
        }
        pOvenAnimation->setDelayPerUnit(0.2);
        pOvenAnimation->setLoops(1);
        
        auto pOvenAnimat = Animate::create(pOvenAnimation);
        m_pAnumation->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(0.5,Vec2(0,1000)),CallFunc::create([=](){
            
            AudioHelp::getInstance()->playEffect("success.mp3");
            
        }),pOvenAnimat,FadeOut::create(0.25),CallFunc::create([=](){
            
            if (_endfunc) {
                _endfunc();
            }
            
        }), NULL));
    }),ScaleTo::create(0.25,1.5),ScaleTo::create(0.25,1.0),FadeOut::create(0.25), NULL));
}
void FlowerLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void FlowerLayer::onExit()
{
    Layer::onExit();
}