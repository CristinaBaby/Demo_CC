

#include "BaseScene.h"

BaseScene::BaseScene()
:m_bBackKeyEvent(false),
m_bTouchEvent(false)
{
    
}

bool BaseScene::init(const std::string &aFilePath)
{
    if ( !Scene::init() ) {
        return false;
    }
    
    m_pUILayer = Layer::create();
    m_pUILayer->setPosition(Vec2::ZERO);
    addChild(m_pUILayer, kUIZorder);
    
    m_pGameLayer = Layer::create();
    m_pGameLayer->setPosition(Vec2::ZERO);
    addChild(m_pGameLayer, kGameZorder);
    
    m_pPopLayer = Layer::create();
    m_pPopLayer->setPosition(Vec2::ZERO);
    addChild(m_pPopLayer, kPopZorder);
    
    setBackgroundImage(aFilePath);
    
    return true;
}

void BaseScene::openBackKeyEvent(std::function<void()> aFunc)
{
    m_fBackKeyCallback = aFunc;
    EventListenerKeyboard *pEvent = EventListenerKeyboard::create();
    pEvent->onKeyReleased = CC_CALLBACK_2(BaseScene::onKeyCallback, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pEvent, this);
}

void BaseScene::setBackgroundImage(std::string aFile)
{
    if (aFile.empty()) {
        return;
    }
    
    Sprite *pBg = Sprite::create(aFile);
    pBg->setPosition(Director::getInstance()->getWinSize() / 2);
    m_pGameLayer->addChild(pBg, kBackgroundZorder);
}

void BaseScene::openTouchEvent()
{
    Layer *pTouchLayer = Layer::create();
    addChild(pTouchLayer, 99999);
    EventListenerTouchOneByOne *pEvent = EventListenerTouchOneByOne::create();
    pEvent->onTouchBegan = CC_CALLBACK_2(BaseScene::onTouchBegan, this);
    pEvent->onTouchMoved = CC_CALLBACK_2(BaseScene::onTouchMoved, this);
    pEvent->onTouchEnded = CC_CALLBACK_2(BaseScene::onTouchEnded, this);
    pEvent->onTouchCancelled = CC_CALLBACK_2(BaseScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pEvent, pTouchLayer);
}

void BaseScene::onKeyCallback(EventKeyboard::KeyCode code, cocos2d::Event *pEvent)
{
    if (code == EventKeyboard::KeyCode::KEY_BACK && m_bBackKeyEvent == false) {
        m_fBackKeyCallback();
    }
}

void BaseScene::onExit()
{
    Scene::onExit();
}

BaseScene::~BaseScene()
{
    
}