
#include "ChooseIceShapeScene.h"
#include "SceneManager.h"
#include "BlenderCup.h"
#include "SceneManager.h"

ChooseIceShapeScene::ChooseIceShapeScene()
{
}

ChooseIceShapeScene::~ChooseIceShapeScene()
{
    
}
bool ChooseIceShapeScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    //    bg
    Sprite* pBg = Sprite::create(localPath("bg/bg_1.png"));
    pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(pBg);
    
    _showModule();
    
    AudioHelp::getInstance()->playBackGroundMusic("make.mp3");
    m_pGameUI->showNormalLayout();
    
    return true;
}

void ChooseIceShapeScene::onEnter()
{
    ExtensionScene::onEnter();
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_MAKE);
    
}

void ChooseIceShapeScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

#pragma mark - initData
void ChooseIceShapeScene::_initData()
{
    setExPath("content/make/");
    m_nNextSceneTag = GameUIEvent::eSceneTagDecorate;
    m_bItemTouchEnable = false;
}


void ChooseIceShapeScene::_showModule()
{
    int shapeCount = 6;
    AudioHelp::getInstance()->playEffect("slide_in_out.mp3");
    for (int i = 0; i<shapeCount; i++) {
        LockItem* pItem = LockItem::create();
        
        bool locked = false;
        std::string lockName = "";
        if (!IAPManager::getInstance()->getItemIsBought(1)){
            if (i==1 || i==2 || i==5){
                if (ConfigManager::getInstance()->getVideoUnLocked("shape", i)){
                    lockName = "";
                }else{
                    lockName = "content/common/video.png";
                    locked = true;
                }
            }
        }
        
        pItem->initWithName("content/make/freeze/choose_mould/"+std::to_string(i+1)+".png", lockName,locked);
        pItem->setTag(i);
        pItem->onItemClicked = CC_CALLBACK_1(ChooseIceShapeScene::_onItemCallback, this);
        this->addChildToContentLayer(pItem);
        CMVisibleRect::setPositionAdapted(pItem, 167+280*(i%2), 765-(230+10*i/2)*(i/2)+visibleSize.height);
        
        if (locked) {
            Sprite* pLock = pItem->getLockSprite();
            if (pLock) {
                pLock->setTag(TAG_REWARD_LOCK);
                LockUIManager::getInstance()->registerLock("shape", i, pLock->getParent());
            }
        }
        
        m_modules.push_back(pItem);
        pItem->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(1, Vec2(0, -visibleSize.height))),
                                          CallFunc::create([=](){
            m_bItemTouchEnable = true;
            if(shapeCount-1==i){
                AudioHelp::getInstance()->playEffect("vo_make_ice.mp3");
            }
        }), NULL));
    }
}


void ChooseIceShapeScene::_onItemCallback(LockItem* item)
{
    if (!m_bItemTouchEnable) {
        return;
    }
    int tag = item->getTag();
    if (item->getChildByTag(TAG_REWARD_LOCK)) {
        RewardManager::getInstance()->showRewardAds("shape", tag);
        return;
    }
    AudioHelp::getInstance()->playSelectedEffect();
    GameDataManager::getInstance()->m_nShapeIndex = tag;
    m_bItemTouchEnable = false;
    for_each(m_modules.begin(), m_modules.end(), [=](LockItem* pCurItem){
        if (pCurItem!=item) {
            if (pCurItem->getPositionX()<visibleSize.width/2) {
                pCurItem->runAction(MoveBy::create(0.5, Vec2(-visibleSize.width*0.8, 0)));
            }else{
                pCurItem->runAction(MoveBy::create(0.5, Vec2(visibleSize.width, 0)));
            }
        }
    });
    item->runAction(Sequence::create(DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("slide_in_out.mp3");
    }),
                                     MoveTo::create(0.5, Vec2(visibleSize.width*0.5, visibleSize.height/2)),
                                     ScaleTo::create(0.5, 1.2),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        SceneManager::replaceTheScene<PourWaterScene>();
    }), NULL));
}