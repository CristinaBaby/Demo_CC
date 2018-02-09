#include "ServeTransitionScene.h"
#include "CMVisibleRect.h"
#include "SceneManager.h"

USING_NS_CC;
using namespace cocostudio;

bool ServeTransitionScene::init(){
    if (!ExtensionScene::init()) {
        return false;
    }
    
    Sprite* bg = Sprite::create("content/make/bg/bg_excessive.jpg");
    bg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
    this->addChildToBGLayer(bg);
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_friends_coming.mp3");
    }),DelayTime::create(3),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<ChoosePackageScene>();
    }), NULL));
    return true;
}
