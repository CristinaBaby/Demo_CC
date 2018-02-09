
#include "AnimationScene.h"
#include "SceneManager.h"

class Phone:public Node {
    
    
public:
    CREATE_FUNC(Phone);
    virtual bool init();
    
    Phone(){
        m_pBody = nullptr;
    }
    
    void sendMessage(std::string head,std::string message,float dt = 0.0f,std::function<void()> onCallback = nullptr);
    void receiveMessage(std::string head,std::string message,float dt = 0.0f,std::function<void()> onCallback = nullptr);
    void updatePosition();
protected:
    void _addMessage(int type,std::string head,std::string message,std::function<void()> onCallback,float dt = 0.0f);
    Sprite* m_pBody;
    ExpandScrollView* m_pScrollView;
    Vector<Sprite*> m_MessageVector;
};

bool Phone::init(){
    if (!Node::init()) {
        return false;
    }
    m_pBody = Sprite::create("content/animation/phone_frame.png");
    this->addChild(m_pBody);
    
    m_pScrollView = ExpandScrollView::create();
    m_pScrollView->setContentSize(Size(340, 423));
    m_pBody->addChild(m_pScrollView);
    m_pScrollView->setPosition(Vec2(110, 183));
    m_pScrollView->getInnerContainer()->setContentSize(m_pScrollView->getContentSize());
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
//    m_pScrollView->setLayoutType(cocos2d::ui::Layout::Type::VERTICAL);
    
    return true;
}

void Phone::sendMessage(std::string head, std::string message,float dt,std::function<void()> onCallback)
{
    _addMessage(0,head,message,onCallback,dt);
}

void Phone::receiveMessage(std::string head, std::string message,float dt,std::function<void()> onCallback)
{
    m_pBody->runAction(Sequence::create(DelayTime::create(dt),
                                        ActionHelper::createShakeAction(), NULL));
    _addMessage(1,head,message,onCallback,dt);
}

void Phone::updatePosition()
{
    if (m_MessageVector.size() >= 0) {
        Sprite* pNode = m_MessageVector.at(m_MessageVector.size()-1);
        Vec2 pos = pNode->getPosition();
        float dt = pos.y-pNode->getContentSize().height-10;
        if (dt<0) {
            for(auto pChild:m_pScrollView->getInnerContainer()->getChildren()){
                pChild->runAction(MoveBy::create(0.3, Vec2(0, -dt+20)));
            }
        }
    }
    
}

void Phone::_addMessage(int type,std::string head,std::string message,std::function<void()> onCallback,float dt){
    
    this->runAction(Sequence::create(DelayTime::create(dt),
                                     CallFunc::create([=](){
        Vec2 pos = m_pScrollView->getInnerContainer()->getContentSize();
        if (m_MessageVector.size() > 0) {
            Sprite* pNode = m_MessageVector.at(m_MessageVector.size()-1);
            pos.y = pNode->getPositionY()-pNode->getContentSize().height;
        }
        Sprite* pHead = Sprite::create(head);
        m_pScrollView->getInnerContainer()->addChild(pHead);
        
        Sprite* pMessage = Sprite::create(message);
        m_pScrollView->getInnerContainer()->addChild(pMessage);
        m_MessageVector.pushBack(pMessage);
        
        pMessage->setScale(0.1);
        pMessage->runAction(ScaleTo::create(0.5, 1));
        if(type == 0) {
            pMessage->setAnchorPoint(Vec2(1, 1));
            pMessage->setPosition(pos+Vec2(-pHead->getContentSize().width-10, -20));
            pHead->setPosition(pos+Vec2(-pHead->getContentSize().width/2, -50));
        }else{
            pMessage->setAnchorPoint(Vec2(0, 1));
            pMessage->setPosition(Vec2(pHead->getContentSize().width+10, -20+pos.y));
            pHead->setPosition(Vec2(pHead->getContentSize().width/2, -50+pos.y));
        }
        if (onCallback) {
            onCallback();
        }
        updatePosition();
        
    }), NULL));
}

class DialogCloud:public Node {
    
    
public:
    CREATE_FUNC(DialogCloud);
    virtual bool init();
    
    DialogCloud(){
        m_pCloud = nullptr;
        m_pCircle0 = nullptr;
        m_pCircle1 = nullptr;
//        m_pText = nullptr;
    }
    
    void showDialog(int type,bool left = true);
    void showText(float dt,std::string text,...);
protected:
    Sprite* m_pCloud;
    Sprite* m_pCircle0;
    Sprite* m_pCircle1;
//    Sprite* m_pText;
    Vector<Sprite*> m_Text;
};


bool DialogCloud::init(){
    if (!Node::init()) {
        return false;
    }
    return true;
}
void DialogCloud::showDialog(int type,bool left)
{
    m_pCircle0 = Sprite::create("content/animation/dialogue"+std::to_string(type+1)+"_0.png");
    this->addChild(m_pCircle0);
    
    m_pCircle1 = Sprite::create("content/animation/dialogue"+std::to_string(type+1)+"_0.png");
    this->addChild(m_pCircle1);
    
    m_pCloud = Sprite::create("content/animation/dialogue"+std::to_string(type+1)+"_1.png");
    this->addChild(m_pCloud);
    
    if(left){
        m_pCircle1->setPosition(Vec2(-50, 40));
        m_pCloud->setPosition(Vec2(-80, 200));
    }else{
        m_pCircle1->setPosition(Vec2(15, 50));
        m_pCloud->setPosition(Vec2(-46, 200));
    }
    m_pCircle0->setScale(0.2);
    m_pCircle1->setScale(0.2);
    m_pCloud->setScale(0.2);
    m_pCircle0->setOpacity(0);
    m_pCircle1->setOpacity(0);
    m_pCloud->setOpacity(0);
    m_pCircle0->runAction(Sequence::create(FadeIn::create(0.1),
                                           ActionHelper::getJellyAction(), NULL));
    m_pCircle1->runAction(Sequence::create(DelayTime::create(0.2),
                                           FadeIn::create(0.1),
                                           ActionHelper::getJellyAction(), NULL));
    m_pCloud->runAction(Sequence::create(DelayTime::create(0.4),
                                         FadeIn::create(0.1),
                                         ActionHelper::getJellyAction(), NULL));
}

void DialogCloud::showText(float dt,std::string text,...)
{
    for(const auto &child : m_Text) {
        child->runAction(Sequence::create(DelayTime::create(dt),
                                          CallFunc::create([=](){
            child->removeFromParent();
        }), NULL));;
    }
    m_Text.clear();
    
    Sprite* pText = Sprite::create(text);
    this->addChild(pText,10);
    
    m_Text.pushBack(pText);
    {
        va_list ap;
        va_start(ap, text);
        while (1) {
            const char* path = va_arg(ap, char*);
            if(!path) {
                break;
            }
            Sprite* pText = Sprite::create(std::string(path));
            this->addChild(pText,10);
            m_Text.pushBack(pText);
        }
        va_end(ap);
        
    }
    float padding = 10;
    float height = padding;
    for(const auto &child : m_Text)
        height = child->getContentSize().height * child->getScaleY() + padding;
    
    float y = height / 2.0f;
    if (m_Text.size()<2) {
        y = 0;
    }
    int i = 0;
    Vec2 orgPos = m_pCloud->getPosition();
    for(const auto &child : m_Text) {
        child->setVisible(false);
        child->setPosition(orgPos+Vec2(0,y ));
        y -= child->getContentSize().height * child->getScaleY() + padding;
        child->runAction(Sequence::create(DelayTime::create(dt+i*0.5),
                                            Show::create(), NULL));
    }
    
//    for(int i = 0;i<m_Text.size();i++){
//        Sprite* pText = m_Text.at(i);
//        pText->setPosition(<#const cocos2d::Vec2 &pos#>)
//        pText->runAction(<#cocos2d::Action *action#>)
//    }
    
//    if (m_pText) {
//        m_pText->runAction(Sequence::create(DelayTime::create(dt),
//                                            Hide::create(),
//                                            CallFunc::create([=](){
//            m_pText->setTexture(text);
//        }),
//                                            Show::create(), NULL));
//    }else{
//        m_pText = Sprite::create(text);
//        this->addChild(m_pText,10);
//        m_pText->setOpacity(0);
//        m_pText->runAction(Sequence::create(DelayTime::create(dt),
//                                            Show::create(), NULL));
//        m_pText->setPosition(m_pCloud->getPosition());
//    }
}

AnimationScene::AnimationScene()
{
    
}

AnimationScene::~AnimationScene()
{
    MemoryManager::getInstance()->clean(ClassString(AnimationScene));
    
}
bool AnimationScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    
    //    bg
    m_pBg = Sprite::create(localPath("animation1_bg.jpg"));
    m_pBg->setPosition(visibleSize*0.5);
    this->addChildToBGLayer(m_pBg);
    
    m_pRole = Armature::create("model1");
    this->addChildToContentLayer(m_pRole);
    CMVisibleRect::setPositionAdapted(m_pRole, 320, 240,kBorderNone,kBorderBottom);
    ArmatureAnimation* lArmation = m_pRole->getAnimation();
    lArmation->play("2",-1,1);
    m_pRole->setScale(1.5);
    
    Bone* pBone = m_pRole->getBone("table");
    pBone->setScale(0.66);
    pBone->changeDisplayWithIndex(0, true);
    
    DialogCloud* pDialogLeft = DialogCloud::create();
    this->addChildToUILayer(pDialogLeft);
    CMVisibleRect::setPositionAdapted(pDialogLeft, 240, 540);
    
    DialogCloud* pDialogRight = DialogCloud::create();
    this->addChildToUILayer(pDialogRight);
    CMVisibleRect::setPositionAdapted(pDialogRight, 500, 520);
    
    Phone* pPhoneChat = Phone::create();
    this->addChildToUILayer(pPhoneChat);
    pPhoneChat->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    pPhoneChat->setVisible(false);
    
    m_pGameUI->showNormalLayout();
    
    if(UserDefault::getInstance()->getBoolForKey("animation_play",false)){
        m_pGameUI->showNextLayout();
    }
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=](){
        pDialogLeft->showDialog(0,true);
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_animation0.mp3");
        pDialogLeft->showText(0.1,"content/animation/sentence0.png","content/animation/sentence1.png","content/animation/sentence2.png",nullptr);
//        pDialogLeft->showText("content/animation/sentence1.png",1);
//        pDialogLeft->showText("content/animation/sentence2.png",2);
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        pDialogLeft->runAction(MoveBy::create(1, Vec2(0, -100)));
        ArmatureAnimation* lArmation = m_pRole->getAnimation();
        lArmation->stop();
        lArmation->play("3",-1,0);
        
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_animation1.mp3");
    }),
                                     DelayTime::create(4),
                                     CallFunc::create([=](){
        ArmatureAnimation* lArmation = m_pRole->getAnimation();
        lArmation->stop();
        lArmation->play("4",-1,0);
        
        pDialogLeft->removeFromParent();
        
        Sprite* pBingo = Sprite::create(localPath("image1.png"));
        this->addChildToUILayer(pBingo);
        CMVisibleRect::setPositionAdapted(pBingo, 520, 780);
        pBingo->setScale(0.2);
        pBingo->setOpacity(0);
        pBingo->runAction(Sequence::create(FadeIn::create(0.3),
                                           EaseElasticOut::create(ScaleTo::create(0.5, 1), 1.0f),
                                           DelayTime::create(0.5),
                                           CallFunc::create([=](){
            pBingo->removeFromParent();
        }), NULL));
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        pDialogRight->showDialog(1,false);
        pDialogRight->showText(0.6,"content/animation/sentence3.png",nullptr);
        pDialogRight->showText(1.5,"content/animation/sentence4.png",nullptr);
    }),
                                     DelayTime::create(0.5),
                                     CallFunc::create([=](){
        AudioHelp::getInstance()->playEffect("vo_animation2_invite_friends.mp3");
    }),
                                     DelayTime::create(1.5),
                                     CallFunc::create([=](){
        lArmation->play("5");
        lArmation->gotoAndPause(176);
    }),
                                     DelayTime::create(1),
                                     CallFunc::create([=](){
        Sprite* pPhone = Sprite::create(localPath("image2.png"));
        pPhone->setPosition(Vec2(30,220));
        pPhone->runAction(Sequence::create(RotateBy::create(0.5,-30),
                                           RotateBy::create(1,60),
                                           RotateBy::create(1,-60),
                                           RotateBy::create(0.5,30),
                                           NULL));
    }),
                                     DelayTime::create(2),
                                     CallFunc::create([=](){
        pDialogRight->removeFromParent();
        
        Sprite* pPhone = Sprite::create(localPath("sellphone.png"));
        this->addChildToUILayer(pPhone);
        CMVisibleRect::setPositionAdapted(pPhone, 340, 170);
        ActionHelper::showBezier(pPhone, pPhone->getPosition(), ActionHelper::ShowDirection::show_from_right,1,[=](){
            pPhone->runAction(Sequence::create(DelayTime::create(0.5),
                                               Spawn::create(RotateBy::create(0.5, 360*2),
                                                             ScaleTo::create(0.5, 2), NULL),
                                               CallFunc::create([=](){
                pPhoneChat->cocos2d::Node::setScale(0.3);
                pPhoneChat->setPosition(pPhone->getPosition());
                pPhoneChat->setVisible(true);
                pPhoneChat->runAction(Sequence::create(Spawn::create(RotateBy::create(0.5, 360*1),
                                                                     ScaleTo::create(0.5, 1),
                                                                     MoveTo::create(0.5,visibleSize*0.5), NULL),
                                                       DelayTime::create(1),
                                                       CallFunc::create([=](){
//                    pPhoneChat->sendMessage(localPath("1.png"), localPath("chat1.png"));
                    
                    int k = arc4random()%3;
                    for (int i = 0; i<6; i++) {
                        if(i%6==0){
                            pPhoneChat->sendMessage(localPath("1.png"), localPath("chat1.png"),i*2,[=](){
                                AudioHelp::getInstance()->playEffect("vo_animation_text1.mp3");
                            });
                            
                        }else if (i%6 == 5){
                            pPhoneChat->sendMessage(localPath("1.png"), localPath("chat6.png"),i*2+0.5,[=](){
                                AudioHelp::getInstance()->playEffect("vo_animation_text6_1.mp3");
                                this->runAction(Sequence::create(DelayTime::create(2),
                                                                 CallFunc::create([=](){
                                    AudioHelp::getInstance()->playEffect("vo_animation_text6_2.mp3");
                                }), NULL));
                            });
                            
                        }else{
                            k++;
                            pPhoneChat->receiveMessage(localPath(std::to_string(k%3+2)+".png"), localPath("chat"+std::to_string(i%6+1)+".png"),i*2+0.3,[=](){
                                AudioHelp::getInstance()->playEffect("vo_animation_text"+std::to_string(i%6+1)+".mp3");
                            });
                        }
                    }
                }),
                                                       DelayTime::create(15),
                                                       Spawn::create(RotateBy::create(0.5, 360*1),
                                                                     ScaleTo::create(0.5, 0.2),
                                                                     MoveBy::create(1,Vec2(visibleSize.width, 0)), NULL),
                                                       CallFunc::create([=](){
                    UserDefault::getInstance()->setBoolForKey("animation_play", true);
                    lArmation->play("5");
                    
                    m_pRole->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 1.1),
                                                                      MoveBy::create(1, Vec2(0, 200)), NULL) ,
                                                        DelayTime::create(2),
                                                        CallFunc::create([=](){
                        AudioHelp::getInstance()->playEffect("all_star.mp3");
                        ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/finish.plist");
                        this->addChildToUILayer(pParticle);
                        pParticle->setPosition(visibleSize*0.5);
                    }),
                                                        DelayTime::create(1),
                                                        CallFunc::create([=](){
                        
                        ChoosePackageLayer* pLayer = ChoosePackageLayer::create();
                        this->addChildToUILayer(pLayer);
                        pLayer->setLocalZOrder(1000);
                        pLayer->onChooseCallback = [=](int index){
                            GameDataManager::getInstance()->m_nPackage = index;
                            if (index == 0) {
                                SceneManager::replaceTheScene<ClassicChooseScene>();
                            }else{
                                SceneManager::replaceTheScene<CaramelMeltSugarScene>();
                            }
                        };
                    }), NULL));
                    AudioHelp::getInstance()->playEffect("vo_animation_finish.mp3");
                }),  NULL));
                pPhone->removeFromParent();
            }), NULL));
        });
    }),NULL));
    AudioHelp::getInstance()->playBackGroundMusic("music_home.mp3");
    
//    lArmation->setMovementEventCallFunc([=](Armature *arm, MovementEventType type, const std::string &id){
//        
//    });
    return true;
}

void AnimationScene::onEnter()
{
    ExtensionScene::onEnter();
}

void AnimationScene::onButtonCallback(Button* btn)
{
    int tag = btn->getTag();
    
    if (GameUILayoutLayer::eUIButtonTagNext==tag){
        btn->setEnabled(false);
        this->stopAllActions();
        for(auto pChild:m_pUILayer->getChildren()){
            pChild->stopAllActions();
        }
        AudioHelp::getInstance()->stopAlleffect();
        ChoosePackageLayer* pLayer = ChoosePackageLayer::create();
        this->addChildToUILayer(pLayer);
        pLayer->setLocalZOrder(1000);
        pLayer->onChooseCallback = [=](int index){
            if (index == 0) {
                SceneManager::replaceTheScene<ClassicChooseScene>();
            }else{
                SceneManager::replaceTheScene<CaramelMeltSugarScene>();
            }
        };
        return;
    }else if (GameUILayoutLayer::eUIButtonTagHome==tag){
        this->stopAllActions();
        for(auto pChild:m_pUILayer->getChildren()){
            pChild->stopAllActions();
        }
        AudioHelp::getInstance()->stopAlleffect();
        btn->setEnabled(false);
        Dialog* dialog=Dialog::create(Size(504,360), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_NEGATIVE);
        dialog->setContentText("Do you want to start over with a new food?");
        Director::getInstance()->getRunningScene()->addChild(dialog, 9999, 9999);
        dialog->setPosition(Director::getInstance()->getVisibleOrigin());
        dialog->onPositiveClick = [=](void*){
            KeypadDispatchCenter::getInstance()->removeDelegate(this);
            btn->setEnabled(true);
            DecorateManager::getInstance()->reset();
            
            AudioHelp::getInstance()->resetEffect();
            SceneManager::popToRootScene();
            SceneManager::replaceTheScene<HomeScene>();
            
        };
        btn->setEnabled(true);
        dialog->onNegativeClick = [=](void*){
            btn->setEnabled(true);
            
        };
        return;
    }
    ExtensionScene::onButtonCallback(btn);
}
void AnimationScene::onShopItemBuy(cocos2d::Ref *pRef)
{
}

void AnimationScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
}

void AnimationScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
}

void AnimationScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){
    
}

void AnimationScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
}

#pragma mark - initData
void AnimationScene::_initData()
{
    setExPath("content/animation/");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("model1/model10.png", "model1/model10.plist", "model1/model1.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("model1/model11.png", "model1/model11.plist", "model1/model1.ExportJson");
    MemoryManager::getInstance()->addArmaturePath("model1/model1.ExportJson", ClassString(AnimationScene));
    
    MemoryManager::getInstance()->addTexturePath("model1/model10.png", ClassString(AnimationScene));
    MemoryManager::getInstance()->addTexturePath("model1/model11.png", ClassString(AnimationScene));
}