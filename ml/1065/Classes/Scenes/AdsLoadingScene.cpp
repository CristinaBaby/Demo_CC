//


#include "AdsLoadingScene.h"
#include "SceneManager.h"
#include "AnimationHelp.h"


static std::string gFontLetter[7] = {"l","o","a","d","i","n","g"};

bool AdsLoadingScene::init()
{
    if ( !AdLoadingLayerBase::init() ) {
        return false;
    }
    loading();
    
    return true;
}

void AdsLoadingScene::onEnter()
{
    AdLoadingLayerBase::onEnter();
    
    AudioHelp::getInstance()->pauseBackGroundMusic();
    AdsManager::getInstance()->setVisiable(kTypeBannerAds,false);
}
void AdsLoadingScene::onExit()
{
    AudioHelp::getInstance()->resumeBackGroundMusic();
    AdsManager::getInstance()->setVisiable(kTypeBannerAds,true);
    AdLoadingLayerBase::onExit();
}
void AdsLoadingScene::loading()
{
    Size size = Director::getInstance()->getVisibleSize();
    
    Sprite* bg = Sprite::create("adsloading/loading.jpg");
    bg->setPosition(Point(size.width/2, size.height/2));
    this->addChild(bg);
    
//    Sprite* pLoad = Sprite::create("adsloading/loading.png");
//    pLoad->setPosition(Point(size.width/2, 160));
//    this->addChild(pLoad);
    
//    for (int i = 0; i<3; i++) {
//        Sprite* pSprite = Sprite::create("adsloading/dot.png");
//        CMVisibleRect::setPositionAdapted(pSprite, 530+15*i, 160);
//        this->addChild(pSprite);
//        
//        pSprite->runAction(Sequence::create(DelayTime::create(i*0.2),
//                                            Repeat::create(Sequence::create(ScaleTo::create(0.2, 1.3),
//                                                                            DelayTime::create(0.1),
//                                                                            ScaleTo::create(0.1, 1),
//                                                                            DelayTime::create(2),
//                                                                            NULL), 3),
//                                            NULL));
//    }
    

    Sprite* pLight = Sprite::create("adsloading/light0.png");
    this->addChild(pLight);
    pLight->setPosition(Point(size.width/2, size.height/2));
    pLight->runAction(RepeatForever::create(Sequence::create(AnimationHelp::createAnimate("adsloading/light", 0, 1,false,true,0.5),
                                                             DelayTime::create(0.5), NULL)));

}
