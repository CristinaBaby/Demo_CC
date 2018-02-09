
#include "DecTypeScrollView.h"



DecTypeScrollView::DecTypeScrollView()
{
    m_pCurTypeBtn = nullptr;
    onItemCellSelected = nullptr;
    m_bSingleAsset = true;
    m_pBoard = nullptr;
    m_nMargin = 0;
}

DecTypeScrollView::~DecTypeScrollView()
{
    
}
DecTypeScrollView* DecTypeScrollView::createWithSize(cocos2d::Size size)
{
    auto pRet = new DecTypeScrollView();
    if (pRet && pRet->init(size))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool DecTypeScrollView::init(Size size)
{
    if ( !Layer::init() )
    {
        return false;
    };
    m_eDirection = Direction::eDirectionH;
    decTypeData.init();
    
    onItemCellSelected = nullptr;
    m_bSelectedAnimate = false;
    
    m_nPercent = 0;
//    if (boxPathEX=="") {
//        m_pBoard = Sprite::create("content/category/icon/box_1.png");
//    }else{
//        m_pBoard = Sprite::create(boxPathEX);
//    }
//    this->addChild(m_pBoard);
//    m_pBoard->setPosition(Vec2(m_pBoard->getContentSize().width/2, m_pBoard->getContentSize().height*0.5));
  
    m_pScrollView = ExpandScrollView::create();
    this->addChild(m_pScrollView);
    m_pScrollView->setPosition(Vec2(0, 0));
    m_pScrollView->setContentSize(size);
    m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setScrollBarEnabled(false);
    
    this->setContentSize(size);
//    Button* pUp = Button::create("content/category/icon/arrow_up.png");
//    this->addChild(pUp);
//    pUp->setPosition(Vec2(size.width/2, size.height+20));
    
//    pUp->setName("up");
//    pUp->addTouchEventListener(CC_CALLBACK_2(DecTypeScrollView::_onButtonCallback, this));
//    
//    Button* pDown = Button::create("content/category/icon/arrow_down.png");
//    this->addChild(pDown);
//    pDown->setPosition(Vec2(size.width/2, -30));
//    pDown->setName("down");
//    pDown->addTouchEventListener(CC_CALLBACK_2(DecTypeScrollView::_onButtonCallback, this));

    return true;
}

void DecTypeScrollView::loadType(std::vector<DecorateTypeConfigData> vec)
{
    m_DecTypeVector.operator=(vec);
    m_pScrollView->getInnerContainer()->removeAllChildren();
    m_pCurTypeBtn = nullptr;
    int i = 0;
    float deltX = 115;
    float deltY = 115;
    float scale = 1.1f;
    float margin = m_nMargin;
//    if (vec.size()==4) {
//        margin = 0;
//    }else{
//        margin = 10;
//    }
    Vec2 prePos = Vec2::ZERO;
    if (vec.size()>3){
        prePos = Vec2(0, 0);
    }else{
        prePos = Vec2(40, 0);
    }
    if(!m_pBoard){
        if (boxPathEX=="") {
            m_pBoard = Sprite::create();
        }else{
            m_pBoard = Sprite::create(boxPathEX);
        }
        this->addChild(m_pBoard,-1);
    }
    int count = vec.size();
    int lockCount = 0;
    if (count>2) {
        lockCount = 1;
    }else if (count>5) {
        lockCount = 2;
    }else if (count >10) {
        lockCount = 3;
    }
    for (auto it = vec.begin(); it!=vec.end(); ++it) {
        Button* pBtn;
        if (m_bSingleAsset) {
            pBtn = Button::create(btnPathEX+it->decIconName+".png",btnPathEX+it->decIconName+".png");
        }else{
            pBtn = Button::create(btnPathEX+"box_1_1.png",btnPathEX+"box_1_1.png");
            Sprite* pContent = Sprite::create(btnPathEX+it->decIconName+".png");
            pContent->setName("content");
            pContent->setPosition(pBtn->getContentSize()*0.5);
            pBtn->addChild(pContent);
        }
        if (i == count-lockCount && !IAPManager::getInstance()->getItemIsBought(1)) {
            if (!ConfigManager::getInstance()->getVideoUnLocked(it->decTypeName+"_type", 0)) {
                Sprite* pLock = Sprite::create("content/common/video.png");
                pBtn->addChild(pLock,10);
                pLock->setTag(TAG_REWARD_LOCK);
                pLock->setPosition(Vec2(130, 30));
                
                pBtn->setUserData(pLock);
                LockUIManager::getInstance()->registerLock(it->decTypeName+"_type", 0, pBtn);
                LockUIManager::getInstance()->setLockOriginPos(Vec2(130, 50));
            }
        }
        m_pScrollView->getInnerContainer()->addChild(pBtn);
        
        deltX = pBtn->getContentSize().width;
        deltY = pBtn->getContentSize().height;
        
        if (m_eDirection==Direction::eDirectionH) {
//            pBtn->setPosition(Vec2(margin+deltX*0.5+(deltX+10+margin)*i, getContentSize().height/2));
            pBtn->setPosition(Vec2(prePos.x+ margin+deltX*0.5, getContentSize().height/2));
        }else{
            pBtn->setPosition(Vec2(getContentSize().width/2,deltY*0.5+20+(deltY+margin)*(vec.size()-i-1)));
        }
        pBtn->setTag(i);
        pBtn->addTouchEventListener(CC_CALLBACK_2(DecTypeScrollView::_itemSelected, this));
        Rect rect = pBtn->getBoundingBox();
        prePos = Vec2(rect.getMaxX(), rect.getMinY());
        i++;
    }
    if (m_eDirection==Direction::eDirectionH) {
        m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
        if (vec.size()>3) {
            m_pScrollView->setInnerContainerSize(Size(deltX*0.5+(deltX+margin)*m_DecTypeVector.size(), deltY));
        }else{
            m_pScrollView->setInnerContainerSize(m_pScrollView->getContentSize());
        }
        CMVisibleRect::setPositionAdapted(m_pBoard, m_pBoard->getContentSize().width*0.4, visibleSize.height*0.5);
        m_pBoard->setPosition(Vec2(m_pScrollView->getContentSize().width/2, m_pBoard->getContentSize().height*0.5));
    }else{
        m_pScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
        if (vec.size()>4) {
            m_pScrollView->setInnerContainerSize(Size(deltX,20+(deltY+margin)*m_DecTypeVector.size()));
        }else{
            m_pScrollView->setInnerContainerSize(m_pScrollView->getContentSize());
        }
        CMVisibleRect::setPositionAdapted(m_pBoard, m_pBoard->getContentSize().width*0.2,m_pScrollView->getContentSize().height/2);
        m_pBoard->setPosition(Vec2( m_pBoard->getContentSize().width*0.5, m_pScrollView->getContentSize().height/2));
    }
    if (m_eDirection==Direction::eDirectionH){
        m_pScrollView->scrollToRight(2, true);
        this->runAction(Sequence::create(DelayTime::create(2.5),
                                         CallFunc::create([=]()
                                                          {
                                                              m_pScrollView->scrollToLeft(2, true);
                                                          }), NULL));
        
    }else{
        m_pScrollView->scrollToBottom(2, true);
        this->runAction(Sequence::create(DelayTime::create(2.5),
                                         CallFunc::create([=]()
                                                          {
                                                              m_pScrollView->scrollToTop(2, true);
                                                          }), NULL));
        
    }
}

void DecTypeScrollView::showAll()
{
    for (int i = 0; i<m_DecTypeVector.size(); i++) {
        Button* btn = (Button*)m_pScrollView->getInnerContainer()->getChildByTag(i);
        btn->setEnabled(false);
        btn->runAction(Sequence::create(EaseBounceOut::create(MoveBy::create(3, Vec2(0, -visibleSize.height*2))),
                                        CallFunc::create([=]()
                                                         {
                                                             
                                                             btn->setEnabled(true);
                                                         }), NULL));
    }
}

void DecTypeScrollView::hideAll()
{
    for (int i = 0; i<m_DecTypeVector.size(); i++) {
        Button* btn = (Button*)m_pScrollView->getInnerContainer()->getChildByTag(i);
        btn->setPosition(Vec2(btn->getPositionX(), btn->getPositionY()+visibleSize.height*2));
    }
    
}

void DecTypeScrollView::scrollToNext()
{
    int percent = getPercent();
    if (m_pScrollView->getDirection()== cocos2d::ui::ScrollView::Direction::HORIZONTAL){
        //            往左滑动
        m_pScrollView->scrollToPercentHorizontal(percent, 1, true);
    }else{
        //            往上滑动
        percent+= (100.0f*m_pScrollView->getContentSize().height/m_pScrollView->getInnerContainerSize().height);
        if (percent>100) {
            percent = 100;
        }
        
        m_pScrollView->scrollToPercentVertical(percent, 1, true);
    }
    m_nPercent = percent;
}

void DecTypeScrollView::scrollToPre()
{
    if (m_pScrollView->getDirection()== cocos2d::ui::ScrollView::Direction::HORIZONTAL){
        //            往右滑动
        float percent = getPercent();
        percent-=10;
        if (percent<0) {
            percent = 0;
        }
        m_pScrollView->scrollToPercentHorizontal(percent, 1, true);
        m_nPercent = percent;
    }else{
        //            往下滑动
        float percent = getPercent();
        percent-=(100.0f*m_pScrollView->getContentSize().height/m_pScrollView->getInnerContainerSize().height);
        if (percent<0) {
            percent = 0;
        }
        m_pScrollView->scrollToPercentVertical(percent, 1, true);
        m_nPercent = percent;
    }
    
}

int DecTypeScrollView::getPercent()
{
    if (m_pScrollView->getDirection()== cocos2d::ui::ScrollView::Direction::HORIZONTAL) {
        float offsetX = -m_pScrollView->getInnerContainer()->getPositionX();
        m_nPercent = (offsetX+m_pScrollView->getContentSize().width)*100/m_pScrollView->getInnerContainerSize().width;
        return m_nPercent;
    }else{
        float offsetY = -m_pScrollView->getInnerContainer()->getPositionY();
        m_nPercent = 100.0f-100.0f*offsetY/(m_pScrollView->getInnerContainerSize().height-m_pScrollView->getContentSize().height);
        return m_nPercent;
    }
}


void DecTypeScrollView::selectBtn(int index)
{
    Button* btn = (Button*)m_pScrollView->getInnerContainer()->getChildByTag(index);
    if (!btn) {
        return;
    }
    
    Rect rect = Rect::ZERO;
    Vec2 pointTop;
    Vec2 pointDown;
    
    rect = m_pScrollView->boundingBox();
    rect.origin = m_pScrollView->getParent()->convertToWorldSpace(rect.origin);
    pointTop = btn->convertToWorldSpace(Vec2(btn->getContentSize().width*0.5, btn->getContentSize().height*0.3));
    pointDown = btn->convertToWorldSpace(Vec2(btn->getContentSize().width*0.5, -btn->getContentSize().height*0.3));
    if (!rect.containsPoint(pointTop) && !rect.containsPoint(pointDown)) {
        log("touch no in scrollview");
        return;
    }
    if(m_pCurTypeBtn){
        m_pCurTypeBtn->stopAllActions();
    }
    
    m_pCurTypeBtn = btn;
    
    DecorateTypeConfigData data = m_DecTypeVector.at(index);
    
    if (m_bSingleAsset) {
        btn->loadTextureNormal(btnPathEX+data.decIconName+"_h.png");
    }else{
        btn->loadTextureNormal(btnPathEX+"dec_icon_h.png");
    }
    if(onItemCellSelected){
        onItemCellSelected(index,m_pCurTypeBtn->getUserData()?1:0,data,true);
    }
}
void DecTypeScrollView::_itemSelected(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        Button* btn = (Button*)ref;
        int tag = btn->getTag();
        DecorateTypeConfigData data = m_DecTypeVector.at(tag);
        if (btn==m_pCurTypeBtn) {
//            if (m_bSelectedAnimate) {
//                m_pCurTypeBtn->stopAllActions();
//                m_pCurTypeBtn->setScale(1);
//            }
            if(onItemCellSelected){
                onItemCellSelected(tag,m_pCurTypeBtn->getUserData()?1:0,data,true);
            }
//            if (m_bSingleAsset) {
//                btn->loadTextureNormal(btnPathEX+data.decIconName+".png");
//            }else{
//                btn->loadTextureNormal(btnPathEX+"box_1_1.png");
//            }
//            onItemCellSelected(tag,data,false);
//            m_pCurTypeBtn = nullptr;
        }else{
            if (m_pCurTypeBtn) {
                if (m_bSelectedAnimate) {
                    m_pCurTypeBtn->stopAllActions();
                    m_pCurTypeBtn->setScale(1);
//                    m_pCurTypeBtn->setPosition(Vec2(m_pCurTypeBtn->getPositionX(), btn->getPositionY()));
                }
                int tag = m_pCurTypeBtn->getTag();
                DecorateTypeConfigData data = m_DecTypeVector.at(tag);
                if (m_bSingleAsset) {
                    m_pCurTypeBtn->loadTextureNormal(btnPathEX+data.decIconName+".png");
                }else{
                    m_pCurTypeBtn->loadTextureNormal(btnPathEX+"box_1_1.png");
                }
                
                ParticleSystemQuad* pParticle = (ParticleSystemQuad*)m_pCurTypeBtn->getChildByName("decoration");
                if (pParticle) {
                    pParticle->removeFromParent();
                }
            }
            if (btn) {
                if (m_bSingleAsset) {
                    btn->loadTextureNormal(btnPathEX+data.decIconName+"_h.png");
                }else{
                    btn->loadTextureNormal(btnPathEX+"box_1_2.png");
                }
            }
            m_pCurTypeBtn = btn;
            if (m_bSelectedAnimate) {
                m_pCurTypeBtn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 0.9),
                                                                                ScaleTo::create(1, 1), NULL)));
//                if(m_pCurTypeBtn){
//                    m_pCurTypeBtn->runAction(MoveBy::create(0.5, Vec2(0, -40)));
//                }
//                btn->runAction(MoveBy::create(0.5, Vec2(0, 40)));
            }
            if(onItemCellSelected){
                onItemCellSelected(tag,m_pCurTypeBtn->getUserData()?1:0,data,true);
            }
//            ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/decoration_button.plist");
//            btn->addChild(pParticle);
//            pParticle->setPosition(btn->getContentSize()*0.5);
//            pParticle->setName("decoration");
        }
    }
}


void DecTypeScrollView::_onButtonCallback(cocos2d::Ref *ref, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        Button* pBtn = (Button*)ref;
        pBtn->setEnabled(false);
        pBtn->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=]()
                                                          {
                                                              pBtn->setEnabled(true);
                                                          }), NULL));
        std::string name = pBtn->getName();
        if (strcmp(name.c_str(), "up")==0) {
            scrollToPre();
        }else if (strcmp(name.c_str(), "down")==0) {
            scrollToNext();
        }
    }
}