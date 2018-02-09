//
//  PackLayer.cpp
//  BabyPlay
//
//  Created by luoxp on 3/18/15.
//
//

#include "PackLayer.h"
#include "AppConfigs.h"
#include "STVisibleRect.h"
#include "UICpp.h"
#include "HomeScene.h"
#include "PictureLayer.h"
#include "CBAppManager.h"
#include "CBPackManager.h"
#include "PictureManager.h"
#include "UITools.h"
#include "AsyncManager.h"
#include "IAPManager.h"

USING_NS_ST;

#define BOOK_1 "book-1.png"
#define BOOK_2 "book-2.png"
#define BOOK_3 "book-3.png"
#define BOOK_4 "book-4.png"
#define BOOK_5 "book-5.png"
#define CHOOSE_SCENE_NUM "123456"
#define PICTURE_WIDTH       934
#define PICTURE_HEIGHT      562
#define BOOK_WIDTH          255
#define BOOK_COUNT          3


bool PackLayer::init(){
    
    if(Layer::init())
    {
        _eventDispatcher->addCustomEventListener(EVENT_IAP, CC_CALLBACK_1(PackLayer::onEventIAP, this));
    
        _root = (Layout*)(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("choose-1.json"));
        _root->cocos2d::Node::visit();
        addChild(_root);

        //quickSetVisible("page number", _root, false);
        _back = quickButton("back", _root, CC_CALLBACK_2(PackLayer::onButton, this));
        _restore = quickButton("restore", _root, CC_CALLBACK_2(PackLayer::onButton, this));
        
        _list = (ListView*)Helper::seekWidgetByName(_root, "List");
        
//        auto table = (ImageView*)Helper::seekWidgetByName(_root, "table");
//        table->setLocalZOrder(2);
        
        auto layout = Layout::create();
        layout->setContentSize(Size(47,50));
        _list->pushBackCustomItem(layout);
        _list->pushBackCustomItem(quickBtn(BOOK_1, CC_CALLBACK_2(PackLayer::onButton, this)));
        _list->setItemsMargin(15);
        auto book2 = quickBtn(BOOK_2, CC_CALLBACK_2(PackLayer::onButton, this));
        _list->pushBackCustomItem(book2);
        auto book3 = quickBtn(BOOK_3, CC_CALLBACK_2(PackLayer::onButton, this));
        _list->pushBackCustomItem(book3);
        
        _list->pushBackCustomItem(quickBtn(BOOK_4, CC_CALLBACK_2(PackLayer::onButton, this)));
        
        _list->pushBackCustomItem(quickBtn(BOOK_5, CC_CALLBACK_2(PackLayer::onButton, this)));
        
        _list->setPositionY(_list->getPositionY());
        _list->setClippingEnabled(true);
        _list->setBounceEnabled(true);
        
        float width = Director::getInstance()->getWinSize().width - _list->getWorldPosition().x;
        _list->setContentSize(Size(width, _list->getContentSize().height));
        
        initAniamtion();
        
        schedule(schedule_selector(PackLayer::addParticle), 4.0f);
        
        _tagVisleble = 1;
        
        auto pos = Vec2(Director::getInstance()->getWinSize()/2) + Vec2(0, -52);
        ArmatureDataManager::getInstance()->addArmatureFileInfo("Choose-1.ExportJson");
        
        auto sun = quickGetAnimation(0, "Choose-1", pos);
        _root->addChild(sun);
        
        auto night = (ImageView*)(Helper::seekWidgetByName(_root, "Image_9"));
        auto posBook3 =  Vec2(150, 10);;  //-270 150
        auto butterfly1 = quickGetAnimation(1, "Choose-1", posBook3); //fly
        night->addChild(butterfly1);

        
        //snow
        
        auto snow = ParticleSystemQuad::create("Snow.plist");
        auto snow_bg = (ImageView*)(Helper::seekWidgetByName(_root, "Image_9"));
        snow_bg->addChild(snow);
        //snow->setPosVar(snow_bg->getContentSize()/2);
        snow->setPosition(Vec2(snow_bg->getPosition().x + 150,Director::getInstance()->getWinSize().height - 180));
        
        //////////////////////////////////////////////////////////// add next scene resource ////////////////////////////////////////////////////////////
        xAsync->add("choose-2.json");
        ArmatureDataManager::getInstance()->removeArmatureFileInfo("Home.ExportJson");
        xAsync->add("Choose-2.ExportJson");
        //////////
        reload();
        
//        auto listener = EventListenerKeyboard::create();
//        listener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event)
//        {
//            if(code == EventKeyboard::KeyCode::KEY_BACK)
//            {
//                auto node = Node::create();
//                node->setName("back-1 ");
//                onButton(node, Widget::TouchEventType::ENDED);
//            }
//        };
//        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        return true;
    }
    
    
    return false;
}

void PackLayer::reload(){
    if(!xIAP->isPurchased(iap_1)){
        auto purchase = Sprite::create("lock.png");
        purchase->setPosition(Vec2(_list->getItem(4)->getContentSize().width/2 - 1,
                                   purchase->getContentSize().height/1.5));
        purchase->setName("PurchasePack1");
        _list->getItem(4 )->addChild(purchase);
    }
    
    if(!xIAP->isPurchased(iap_2)){
        auto purchase = Sprite::create("lock.png");
        purchase->setPosition(Vec2(_list->getItem(5)->getContentSize().width/2 - 1,
                                   purchase->getContentSize().height/1.5));
        purchase->setName("PurchasePack2");
        _list->getItem(5 )->addChild(purchase);
    }
}


void PackLayer::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    string name = ((Node*)pSender)->getName();
    
    if (name == "back" || name == "restore") {
        xAudio->playDefaultSound();
    }
    else
    {
        auto it = name.find_first_of("12345");
        int i = stod(name.substr(it,1));
        if (i == 4 || i == 5) {
            if (i == 4) {
                if(!xIAP->isPurchased(iap_1)){
                    xAudio->playDefaultSound();
                }
                else {
                    xAudio->playSound("sfx/sfx_openbook.mp3");
                }
            }
            else if (i == 5)
            {
                if(!xIAP->isPurchased(iap_2)){
                    xAudio->playDefaultSound();
                }
                else {
                    xAudio->playSound("sfx/sfx_openbook.mp3");
                }
            }
        }
        else
        {
            xAudio->playSound("sfx/sfx_openbook.mp3");
        }
        
    }
    
    if(name.compare("back") == 0){
        _back->setTouchEnabled(false);
        _restore->setTouchEnabled(false);
        auto scene = HomeScene::create();
        scene->beBack();
        Director::getInstance()->replaceScene(scene);

    }
    else if(name.compare("restore") == 0){
        xIAP->restore();
    }
    else
    {
        auto it = name.find_first_of("12345");
        int i = stod(name.substr(it,1));
        if (i == 4 || i == 5) {
            if (i == 4) {
                if(!xIAP->isPurchased(iap_1)){
                    xIAP->purchase(iap_1);
                    return;
                }
            }
            else
            {
                if(!xIAP->isPurchased(iap_2)){
                    xIAP->purchase(iap_2);
                    return;
                }
            }
        }
        
        playAnimation(name);
        
        _list->setTouchEnabled(false);
        _back->setTouchEnabled(false);
        _restore->setTouchEnabled(false);
        for (int i = 1; i < 6; i++)
        {
            ((Button*)_list->getChildren().at(i))->setTouchEnabled(false);
        }
    }
}


void PackLayer::onEventIAP(EventCustom *){
    
    if(xIAP->isPurchased(iap_1)){
        auto sp = _list->getItem(4)->getChildByName("PurchasePack1");
        if(sp){
            sp->removeFromParent();
        }
    }
    
    if(xIAP->isPurchased(iap_2)){
        auto sp = _list->getItem(5)->getChildByName("PurchasePack2");
        if(sp){
            sp->removeFromParent();
        }
    }
    
    _list->requestRefreshView();
}
void PackLayer::playAnimation(const string &name){
    auto it = name.find_first_of("12345");
    //"Book" +name.substr(it,1) +".ExportJson";
    
    auto widget = (Button*)Helper::seekWidgetByName(_root, name);
    widget->setVisible(false);
    
    Vec2 pos = Director::getInstance()->getWinSize() / 2;
    
    auto size = widget->getContentSize();
    float scale = PICTURE_HEIGHT/size.height < PICTURE_WIDTH/size.width ? PICTURE_HEIGHT/size.height : PICTURE_WIDTH/size.width;
    scale *= 1.02;
    
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Book" +name.substr(it,1) +".ExportJson");
    _armatureBook = Armature::create("Book" +name.substr(it,1));
    _armatureBook->setName(name);
    _armatureBook->getAnimation()->playWithIndex(0);
    _armatureBook->setPosition(convertToNodeSpace(widget->getWorldPosition()) + Vec2(-111, -5));
    _armatureBook->setAnchorPoint(Vec2(0.4923, 0.31));   //4s 0.4928   pad2 0.4922   5s 0.4921
    _armatureBook->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(PackLayer::onEndTurning, this, std::placeholders::_1, std::placeholders::_2));
    _armatureBook->runAction(Spawn::create(MoveTo::create(1.6f, pos), ScaleTo::create(1.6f, scale), NULL));
    
    addChild(_armatureBook, 2);
    
    EventCustom event("event_start_turning");
    event.setUserData((void*)name.c_str());
    _eventDispatcher->dispatchEvent(&event);
}

void PackLayer::onEndTurning(cocostudio::Armature *armature, cocostudio::MovementEventType movementType)
{
    if (MovementEventType::COMPLETE == movementType || MovementEventType::LOOP_COMPLETE == movementType) {
        string name = armature->getName();
        armature->getAnimation()->stop();
        _packIndex = name.find_first_of(CHOOSE_SCENE_NUM);
        if(_packIndex != string::npos)
        {
            auto packIndex = std::stod(&name[_packIndex]);
            auto packPath = PackManager::getInstance()->getPackPath(packIndex, false);
            packPath.erase(packPath.end() - 1);
            auto num = PictureManager::getImgFileCount(packPath);
            
            EventCustom event("event_end_turning");
            int data[3]={(int)packIndex,num};
            event.setUserData(&data);
            _eventDispatcher->dispatchEvent(&event);
        }
    }
}

Button* PackLayer::quickBtn(const string &name, const Widget::ccWidgetTouchCallback &callback){
    auto button = Button::create(name, name);
    button->addTouchEventListener(callback);
    button->setTouchEnabled(true);
    button->setPressedActionEnabled(false);
    button->setName(name);
    if (button !=nullptr) {
        return button;
    }
    log("new button failed");
    return nullptr;
}

void PackLayer::initAniamtion()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Model01.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Model02.plist");
    
    _model = make_shared<ArmatureManager>();
    
    _model->setScale(0.7f);
    
    Vec2 pos = Vec2(_list->getWorldPosition().x /2 + 20, _list->getWorldPosition().y + 45);
    _model->setPosition(pos);
    _model->setLocalZOrder(1);
    
    _model->add("ModelWait1.ExportJson", this);
    auto armature = _model->add("ModelWait2.ExportJson", this, Vec2(-2, 49));
    armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(PackLayer::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    _model->add("ModelWait3.ExportJson", this);
    
    _model->switchArmature();
    
    auto armatures = _model->getArmatures();
    for (auto armature : armatures) {
        armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(PackLayer::onMovementEvent, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void PackLayer::onMovementEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType){
    if (MovementEventType::LOOP_COMPLETE == movementType) {
        _model->switchArmature();
    }
}

void PackLayer::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
    if (evt == "sing_start") {
        xAudio->playSound("interactive/sound_humming.mp3");
    }
}

void PackLayer::addParticle(float dt){
    auto star = ParticleSystemQuad::create("particle/star.plist");
    star->setPosition(getPriclePos());
    star->setName("star");
    runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=](){
        star->removeFromParentAndCleanup(true);
    }), NULL));
    getBook()->addChild(star);
}

Vec2 PackLayer::getPriclePos(){
    Size size = _list->getChildren().at(1)->getContentSize();
    return Vec2(size.width*quickRandom(2, 7) / 10,size.height*quickRandom(3, 7) / 10);
}

Button* PackLayer::getBook(){
    return (Button*)_list->getChildren().at(quickRandom(_tagVisleble,BOOK_COUNT));
}

void PackLayer::removeAarticleFromBook(Button *target){
    if (NULL != target->getChildByName("star")) {
        target->getChildByName("star")->setVisible(false);
    }
}


void PackLayer::onExit(){
    log("onExit");
    Layer::onExit();
    _eventDispatcher->removeCustomEventListeners(EVENT_IAP);
}
