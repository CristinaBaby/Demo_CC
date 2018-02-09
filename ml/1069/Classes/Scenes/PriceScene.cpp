
#include "PriceScene.h"
#include "SceneManager.h"
#include "SaleManager.h"

static Vec2 gPizzaPos[] = {
    Vec2(192-20, 430-70),
    Vec2(425-20, 460-70),
    Vec2(635-20, 460-70),
    Vec2(835-20, 460-70),
    
    Vec2(182-20, 250-70),
    Vec2(436-10, 230-70),
    Vec2(640-20, 230-70),
    Vec2(835-20, 230-70),
};

PriceScene::PriceScene()
{
    m_pPizza = nullptr;
    m_bPriceTagEnable = false;
    m_bReadySale = false;
    m_nSaleCount = 0;
    m_bGirlShown = false;
    m_bExpensive = false;
    m_bFinish =false;
}

PriceScene::~PriceScene()
{
    
}
bool PriceScene::init()
{
    if ( !ExtensionScene::init() )
    {
        return false;
    }
    _initData();
    int pizzaCount = 8;
    int index = 0;
    int minIndex = 0;
    int maxIndex = 1;
    bool bIsFull = true;
    for (int i = 0; i<11; i++) {
        if(GameDataManager::getInstance()->getFirstTimePrice()){
            m_PizzaPathIndex.push_back(i);
        }else{
            m_PizzaPathIndex.push_back(-2);
        }
    }
    if(GameDataManager::getInstance()->getFirstTimePrice()){
        std::random_shuffle(m_PizzaPathIndex.begin(), m_PizzaPathIndex.end());
    }
    std::memset(m_bOnSale, 1, 8);
    for(int i = 0 ;i<pizzaCount  ;i++){
        FoodData data = GameDataManager::getInstance()->getPriceData(i);
        data.display();
        if(data.empty){
            m_bOnSale[i] = false;
            bIsFull = false;
            if (i==2 && GameDataManager::getInstance()->getFirstTimePrice()) {
                data.index = 1;
                m_nEmptyIndex = i;
            }else{
                data.init();
                data.empty = false;
                data.price = 30;
                data.selfPrice = 0;
                if(GameDataManager::getInstance()->getFirstTimePrice()){
                    if (i==0 || i==1 ||i ==4) {
                        data.selfPrice = arc4random()%2==0?9.99:12.99;
                    }
                    data.type = m_PizzaPathIndex[i];
                    data.index = 0;
                }else{
                    data.type = -2;
                    data.selfPrice = 0;
                }
                data.packed = false;
                GameDataManager::getInstance()->setPriceData(i, data);
            }
        }else{
            
        }
        if (i==0) {
            minIndex = data.index;
        }else{
            if (data.index<minIndex) {
                minIndex = data.index;
                index = i;
            }
        }
        if (data.index>maxIndex) {
            maxIndex = data.index;
        }
        m_PizzaPathIndex[i] = data.type;
        m_FoodDataVector.push_back(data);
    }
    
    for (int i = 0; i<pizzaCount; i++) {
        FoodData data = m_FoodDataVector.at(i);
        if(!GameDataManager::getInstance()->getFirstTimePrice()){
            int dataType = m_PizzaPathIndex[i];
            
            if (dataType==-2) {
                int radomType = arc4random()%11;
                auto iter = std::find(m_PizzaPathIndex.begin(), m_PizzaPathIndex.end(), radomType);
                while (iter != m_PizzaPathIndex.end()) {
                    radomType = arc4random()%11;
                    iter = std::find(m_PizzaPathIndex.begin(), m_PizzaPathIndex.end(), radomType);
                }
                m_PizzaPathIndex.at(i) = radomType;
            }
        }
    }
    
    for (int i = 0; i<pizzaCount; i++) {
        FoodData data = m_FoodDataVector.at(i);
        if (data.type!=-1 && !data.empty) {
            int radom = m_PizzaPathIndex[i]+1;
            std::stringstream ostr;
            ostr<<"content/showcase/pizza"<<radom<<".png";
            data.path = ostr.str();
            data.type = m_PizzaPathIndex.at(i);
            m_FoodDataVector.at(i) = data;
            GameDataManager::getInstance()->setPriceData(i, data);
        }
    }
    for (int i = 0; i<pizzaCount; i++) {
        FoodData data = m_FoodDataVector.at(i);
        if (data.index==-1) {
            data.index = maxIndex;
            m_FoodDataVector.at(i) = data;
        }
    }
    for (int i = 0; i<pizzaCount; i++) {
        FoodData data = m_FoodDataVector.at(i);
        if (i==0) {
            minIndex = data.index;
        }else{
            if (data.index<minIndex) {
                minIndex = data.index;
                index = i;
            }
        }
    }
    if (!GameDataManager::getInstance()->getFirstTimePrice()){
        FoodData data = m_FoodDataVector.at(index);
        data.init();
        data.index = maxIndex+1;
        data.empty = true;
        m_nEmptyIndex = index;
        
        m_FoodDataVector.at(index) = data;
    }
//    if (bIsFull) {
//        FoodData data = m_FoodDataVector.at(index);
//        data.init();
//        data.index = maxIndex+1;
//        data.empty = true;
//        m_nEmptyIndex = index;
//        
//        m_FoodDataVector.at(index) = data;
//    }

    m_pShowCase = Layer::create();
    this->addChildToContentLayer(m_pShowCase);
    
    
    Sprite* pBg = Sprite::create(localPath("bg.png"));
    this->addChildToBGLayer(pBg);
    CMVisibleRect::setPositionAdapted(pBg, 480, 320+20);
    
    Sprite* pBgBack = Sprite::create(localPath("bg_2.png"));
    m_pShowCase->addChild(pBgBack);
    CMVisibleRect::setPositionAdapted(pBgBack, 480, 320);
    
    Sprite* pBgFront = Sprite::create(localPath("bg_1.png"));
    m_pShowCase->addChild(pBgFront,5);
    CMVisibleRect::setPositionAdapted(pBgFront, 480, 320);
    
    bool bPacked = GameDataManager::getInstance()->m_bPacked;
    std::string name = GameDataManager::getInstance()->getBoxName();
    m_sPath = name;
    m_pPizza = _createDrageNode(getStoragePath(name));
    m_pShowCase->addChild(m_pPizza,20);
    CMVisibleRect::setPositionAdapted(m_pPizza, 100-visibleSize.width/2, 100);
    m_pPizza->setTouchEnabled(false);
    m_pPizza->setScale(0.7);
    m_pPizza->runAction(Sequence::create(DelayTime::create(1.5),
                                         MoveBy::create(0.5, Vec2(visibleSize.width/2, 0)),
                                         CallFunc::create([=](){
        m_pPizza->setOrgPositionDefault();
        m_pPizza->setTouchEnabled(true);
        m_pGuideLayer->showGuideMove(m_pPizza->getPosition(), gPizzaPos[m_nEmptyIndex]);
    }), NULL));
    m_pPizza->setTag(m_nEmptyIndex);
    
    
    for (int i = 0; i<pizzaCount; i++) {
        FoodData data = m_FoodDataVector.at(i);
        DragNode* pNode = _addPizza(i);
        if (!m_bOnSale[i] && pNode) {
            CMVisibleRect::setPosition(pNode, 400, -200);
            pNode->runAction(Sequence::create(DelayTime::create(0.5+i*0.2),
                                              EaseBackOut::create(MoveTo::create(0.8, pNode->getOrgPosition())), NULL));
        }
    }
    _showPriceTag();
    
    m_pGuideLayer = GuideLayer::create();
    this->addChildToUILayer(m_pGuideLayer);
    m_pGuideLayer->setLocalZOrder(10);
    
//    m_pGameUI->showNextLayout();
    
    m_pGirl = RoleModel::create();
    m_pGirl->setRole(localCocosPath("cooker1/"), "cooker1", 4);
    m_pShowCase->addChild(m_pGirl,10);
    m_pGirl->setScale(0.5);
    CMVisibleRect::setPositionAdapted(m_pGirl, 100-visibleSize.width, -100-120);
    
    m_pGirl->onArmationCallback = [=](Armature *armature, MovementEventType movementType, const std::string& movementID){
        if (MovementEventType::COMPLETE==movementType) {
            if (0==std::strcmp(movementID.c_str(), "Animation1") ){
                m_pGirl->playArmation(1, true);
            }else if (0==std::strcmp(movementID.c_str(), "Animation2") ){
                m_pGirl->playArmation(2, true);
            }
        }
    };
    return true;
}

void PriceScene::onEnter()
{
    ExtensionScene::onEnter();
//    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent(Flurry_EVENT_SALE);
    Analytics::getInstance()->sendScreenEvent(Flurry_EVENT_SALE);
}

void PriceScene::onExit()
{
//    FlurryEventManager::getInstance()->logCurrentModuleExitEvent(Flurry_EVENT_SALE);
    ExtensionScene::onExit();
}

void PriceScene::onButtonCallback(Button* btn)
{
    if (btn->getTag()==GameUILayoutLayer::eUIButtonTagNext) {
        AudioHelp::getInstance()->playEffect("vo_shop_owner.mp3");
        btn->setTouchEnabled(false);
        m_bFinish =true;
        m_bPriceTagEnable = false;
        m_bReadySale = false;
        m_pGuideLayer->removeGuide();
        
        this->runAction(Repeat::create(Sequence::create(DelayTime::create(0.5),
                                                        CallFunc::create([=](){
            _showParticle();
        }), NULL), 6));
        this->runAction(Sequence::create(DelayTime::create(4),
                                         CallFunc::create([=](){
            ExtensionScene::onButtonCallback(btn);
        }), NULL));
    }
}

void PriceScene::onPriceSetCallback(float price)
{
    m_pGuideLayer->removeGuide();
    
    Button* pButton = (Button*)m_PizzaPriceVector.at(m_nCurPriceSetIndex);
    pButton->setVisible(true);
    
    _updatePrice(m_nCurPriceSetIndex,price);
    FoodData data = m_FoodDataVector.at(m_nCurPriceSetIndex);
    data.selfPrice = price;
    m_FoodDataVector.at(m_nCurPriceSetIndex) = data;
    _saveData();
    
    bool finish = true;
    for (int i = 0; i<m_FoodDataVector.size(); i++) {
        FoodData data = m_FoodDataVector.at(i);
        if (0==data.selfPrice && !data.empty) {
            Button* pButton = m_PizzaPriceVector.at(i);
            pButton->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.1),
                                                                      ScaleTo::create(1, 1),
                                                                      DelayTime::create(1), NULL)));
            m_pGuideLayer->showGuideTap(pButton->getParent()->convertToWorldSpace(pButton->getPosition()));
            finish= false;
            break;
        }
    }
    if (finish && !m_bGirlShown) {
        m_bGirlShown = true;
        _readyToSale();
    }
}

void PriceScene::dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint)
{
    pDragNode->setLocalZOrder(20);
}
void PriceScene::dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint)
{
    
}
void PriceScene::dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint)
{
    if (pDragNode==m_pPizza) {
        Vec2 pos = gPizzaPos[m_nEmptyIndex];
        Rect rect = Rect(pos.x-60, pos.y-60, 120, 120);
        rect = pDragNode->getDragSprite()->getBoundingBox();
        rect.origin = pDragNode->convertToWorldSpace(rect.origin);
        pos = m_pShowCase->convertToWorldSpace(pos);
        
        if (rect.containsPoint(pos)) {
            m_pGuideLayer->removeGuide();
            int tag = pDragNode->getTag();
            CMVisibleRect::setPosition(pDragNode, gPizzaPos[tag].x, gPizzaPos[tag].y);
            pDragNode->setOrgPositionDefault();
            m_nCurPriceSetIndex = tag;
            m_bPriceTagEnable = true;
            FoodData data = m_FoodDataVector.at(tag);
            data.empty = false;
            data.packed = true;
            data.path = m_sPath;
            data.type = -1;
            m_FoodDataVector.at(tag) = data;
            m_pPizza->removeFromParent();
            m_pPizza = nullptr;
            _addPizza(m_nCurPriceSetIndex);
            
            Button* pButton = (Button*)m_PizzaPriceVector.at(tag);
            m_PriceTagPos = pButton->getParent()->convertToWorldSpace(pButton->getPosition());
            pButton->setVisible(false);
            _showSetPriceLayer();
            GameDataManager::getInstance()->setFirstTimePrice(false);
            AudioHelp::getInstance()->playEffect("vo_enter_price.mp3");
        }else{
            
            pDragNode->back(20);
        }
    }else{
        m_pGuideLayer->removeGuide();
        if (!m_bReadySale) {
            pDragNode->back();
        }else{
            RoleModel* pGuest = SaleManager::getInstance()->getRole();
            Rect rect = Rect(pGuest->getPositionX()-150, pGuest->getPositionY()-200-100, 300, 400);
            rect.origin = pGuest->getParent()->convertToWorldSpace(rect.origin);
            rect = Rect(460,70,240,260);
            
            rect.origin = CMVisibleRect::getPositionAdapted(rect.origin);
            if (rect.containsPoint(worldPoint)) {
                int tag  = pDragNode->getTag();
                FoodData data = m_FoodDataVector.at(tag);
                
                if(data.selfPrice>50) {
                    AudioHelp::getInstance()->stopEffect("vo_too_expensive.mp3");
                    AudioHelp::getInstance()->playEffect("vo_too_expensive.mp3");
                    m_bExpensive = true;
                    SaleManager::getInstance()->guestSad();
                    pDragNode->back();
                }else if (m_nSaleIndex == tag) {
                    _pickPizza();
                    pDragNode->setVisible(false);
                    pDragNode->setTouchEnabled(false);
                    pDragNode->back();
                    
                }else if(m_bExpensive){
                    m_nSaleIndex = tag;
                    _pickPizza();
                    pDragNode->setVisible(false);
                    pDragNode->setTouchEnabled(false);
                    pDragNode->back();
                }else{
                    pDragNode->back();
                }
            }else{
                pDragNode->back();
            }
        }
    }
}
void PriceScene::dragNodeClicked(DragNode* pDragNode,Point worldPoint)
{
    
    if (pDragNode==m_pPizza){
        
    }else{
        pDragNode->back();
    }
}

void PriceScene::onPiceCallback(Ref* ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        if (!m_bPriceTagEnable) {
            return;
        }
        AudioHelp::getInstance()->playSelectedEffect();
        Button* pButton = (Button*)ref;
        pButton->stopAllActions();
        pButton->setScale(1);
        int tag = pButton->getTag();
        m_nCurPriceSetIndex = tag;
        m_PriceTagPos = pButton->getParent()->convertToWorldSpace(pButton->getPosition());
        pButton->setVisible(false);
        _showSetPriceLayer();
    }
}
#pragma mark - initData
void PriceScene::_initData()
{
    
    setExPath("content/showcase/");
    setExCocosPath("res/");
    m_nNextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    GameUIEvent::getInstance()->nextSceneTag = GameUIEvent::eSceneTagChoosePackage;
    ArmatureDataManager::getInstance()->addArmatureFileInfo(localCocosPath("guest0/guest00.png"), localCocosPath("guest0/guest00.plist"), localCocosPath("guest0/guest0.ExportJson"));
    ArmatureDataManager::getInstance()->addArmatureFileInfo(localCocosPath("guest1/guest10.png"), localCocosPath("guest1/guest10.plist"), localCocosPath("guest1/guest1.ExportJson"));
}

DragNode* PriceScene::_addPizza(int index)
{
    FoodData data = m_FoodDataVector.at(index);
    if (data.empty) {
        return nullptr;
    }
    std::string path = data.packed?getStoragePath(data.path):data.path;
    DragNode* pPizza = _createDrageNode(path);
    m_pShowCase->addChild(pPizza);
    
    if (data.packed) {
        pPizza->setScale(0.4);
    }
    
    CMVisibleRect::setPosition(pPizza, gPizzaPos[index].x, gPizzaPos[index].y);
    pPizza->setTag(index);
    pPizza->setTouchEnabled(true);
    pPizza->setOrgPositionDefault();
    m_PizzaVector.pushBack(pPizza);
    return pPizza;
}

void PriceScene::_showPriceTag()
{
    for (int i = 0; i<8; i++) {
        Button* pPriceTag = Button::create(localPath("price0.png"));
        m_pShowCase->addChild(pPriceTag);
        CMVisibleRect::setPosition(pPriceTag, gPizzaPos[i].x+50, gPizzaPos[i].y-20);
        pPriceTag->setTag(i);
        pPriceTag->setLocalZOrder(10);
        pPriceTag->addTouchEventListener(CC_CALLBACK_2(PriceScene::onPiceCallback, this));
        
        FoodData data = m_FoodDataVector.at(i);
        float price = data.selfPrice;
        m_FoodDataVector.at(i) = data;
        if (price>0) {
            std::stringstream ostr;
            ostr<<"$"<<price;
            Label* pLable = Label::createWithTTF(ostr.str(), font_felt, 20,Size(60,35));
            pPriceTag->addChild(pLable);
            pLable->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
            pLable->setPosition(Vec2(87, 30));
            pLable->setColor(Color3B::BLACK);
            pLable->setName("priceLabel");
        }
        m_PizzaPriceVector.pushBack(pPriceTag);
    }
    
}

void PriceScene::_updatePrice(int index,float price)
{
    Button* button = m_PizzaPriceVector.at(index);
    Label *pLabel = dynamic_cast<Label*>(button->getChildByName("priceLabel"));
    std::stringstream ostr;
    if (price!=0) {
        ostr<<"$"<<price;
    }
    if (pLabel) {
        pLabel->setString(ostr.str());
    }else{
        Label* pLable = Label::createWithTTF(ostr.str(), font_felt, 20);
        button->addChild(pLable);
        pLable->setPosition(Vec2(87, 30));
        pLable->setColor(Color3B::BLACK);
        pLable->setName("priceLabel");
    }
}
void PriceScene::_showSetPriceLayer()
{
    SetPriceLayer* pLayer = SetPriceLayer::create();
    this->addChildToUILayer(pLayer);
    pLayer->setLocalZOrder(100);
    pLayer->setTagWorldPosition(m_PriceTagPos);
    pLayer->showPriceTag();
    pLayer->onPriceSetCallbak = CC_CALLBACK_1(PriceScene::onPriceSetCallback, this);
}

void PriceScene::_saveData()
{
    int i = 0;
    for_each(m_FoodDataVector.begin(), m_FoodDataVector.end(), [=,&i](FoodData data){
        log("===i %d",i);
        data.display();
        GameDataManager::getInstance()->setPriceData(i, data);
        i++;
    });
}

void PriceScene::_readyToSale()
{
    AudioHelp::getInstance()->playEffect("vo_price_done.mp3");
    m_pGameUI->showCointLayout();
//    m_bPriceTagEnable = false;
    m_pShowCase->runAction(MoveBy::create(0.5, Vec2(0, 100+60)));
    m_pShowCase->runAction(ScaleTo::create(0.5, 0.8));
    
    m_pGirl->runAction(Sequence::create(MoveBy::create(1.5, Vec2(visibleSize.width, 0)),
                                        CallFunc::create([=](){
        _produceGuese();
        
    }),NULL));
    m_pGirl->playArmation(2, true);
}

void PriceScene::_beginiveCoint()
{
    Node* pCointNode = Node::create();
    FoodData data = m_FoodDataVector.at(m_nSaleIndex);
    int count = data.selfPrice/10+1;
    
    for (int i = 0; i<count; i++) {
        Sprite* pCoint = Sprite::create(localPath("coin.png"));
        pCoint->setPosition(Vec2(-3+6*i/2, 2*i));
        pCointNode->addChild(pCoint);
    }
    SaleManager::getInstance()->guestGetCoint(pCointNode);

}
void PriceScene::_giveCoint()
{
    if(!m_bExpensive) {
        AudioHelp::getInstance()->playEffect("vo_customer_likes_it.mp3");
    }
    RoleModel* pGuest = SaleManager::getInstance()->getRole();
    
    FoodData data = m_FoodDataVector.at(m_nSaleIndex);
    
    int count = data.selfPrice/4+1;
    count = count>15?10:count;
    ccBezierConfig cfg;
    cfg.controlPoint_1 = Vec2(500, 600);
    cfg.controlPoint_2 = Vec2(200, 300);
    cfg.endPosition = CMVisibleRect::getPosition(100, 20,kBorderLeft,kBorderTop);
    for (int i = 0; i<count; i++) {
        Sprite* pCoint = Sprite::create(localPath("coin.png"));
        this->addChildToUILayer(pCoint);
        pCoint->setLocalZOrder(100);
        SaleManager::getInstance()->guestPay(pCoint);
        pCoint->setPosition(pCoint->getPosition());
        pCoint->setScale(0.5);
        pCoint->setLocalZOrder(count-i);
        pCoint->runAction(Sequence::create(DelayTime::create(0.2*i+0.1),
                                           BezierTo::create(1, cfg),
                                               CallFunc::create([=](){
            if (i == count-1) {
                m_pGameUI->updateCoint();
                pGuest->setScaleX(-pGuest->getScale());
                SaleManager::getInstance()->guestBack();
                
                if (m_nSaleCount<8) {
                    _produceGuese();
                }
                m_bReadySale = false;
                pGuest->runAction(Sequence::create(MoveBy::create(3.5, Vec2(visibleSize.width, 0)),
                                                   CallFunc::create([=](){
                    pGuest->removeFromParent();
                }),
                                                   NULL));
            }
            pCoint->removeFromParent();
        }), NULL));
    }
    float coint = GameDataManager::getInstance()->getCoint();
    GameDataManager::getInstance()->setCoint(coint+data.selfPrice);
    
    
    data.init();
    m_FoodDataVector.at(m_nSaleIndex) = data;
    GameDataManager::getInstance()->setPriceData(m_nSaleIndex, data);
    
}
void PriceScene::_produceGuese()
{
    if (m_bFinish) {
        return;
    }
    RoleModel* pGuest = SaleManager::getInstance()->produceGuest();
    m_pShowCase->addChild(pGuest,10);
    pGuest->onArmationCallback = [=](Armature *armature, MovementEventType movementType, const std::string& movementID){
        if (MovementEventType::COMPLETE==movementType) {
            if (0==std::strcmp(movementID.c_str(), "buy") ){
                _giveCoint();
            }else if (0==std::strcmp(movementID.c_str(), "sad")) {
                this->runAction(Sequence::create(DelayTime::create(3),
                                                 CallFunc::create([=](){
                    log("======PriceScene  sad===");
                    if(m_bReadySale){
                        SaleManager::getInstance()->guestSad();
                        SaleManager::getInstance()->guestStandby2();
                    }
                }), NULL));
                }
        }else if (MovementEventType::START==movementType) {
            if (0==std::strcmp(movementID.c_str(), "buy")) {
                _beginiveCoint();
            }
        }
    };
    
    int radom = arc4random()%10*0.1;
    pGuest->setScale(0.5+radom);
    CMVisibleRect::setPosition(pGuest, 600+visibleSize.width, -100-130);
    SaleManager::getInstance()->guestCome();
    m_bExpensive = false;
    pGuest->runAction(Sequence::create(MoveBy::create(3.5, Vec2(-visibleSize.width, 0)),
                                       CallFunc::create([=](){
        if (m_nSaleCount==0) {
            m_pGuideLayer->showGuideMove(Vec2(visibleSize.width/2, visibleSize.height*0.65), pGuest->getPosition());
        }
        SaleManager::getInstance()->guestSad();
        SaleManager::getInstance()->guestStandby2();
        m_nSaleIndex = arc4random()%8;
        while (m_FoodDataVector.at(m_nSaleIndex).empty) {
            m_nSaleIndex = arc4random()%8;
        }
        
        FoodData data = m_FoodDataVector.at(m_nSaleIndex);
        std::string path = data.packed?getStoragePath(data.path):data.path;
        Sprite* pTip = Sprite::create(path);
        
        if (data.packed) {
            pTip->setScale(0.3);
        }else{
            pTip->setScale(0.8);
        }
        SaleManager::getInstance()->showDialog(pTip);
        m_bReadySale = true;
        AudioHelp::getInstance()->playEffect("vo_customer_wants_pizza.mp3");
    }),
                                       NULL));
}
void PriceScene::_pickPizza()
{
    AudioHelp::getInstance()->stopEffect("vo_too_expensive.mp3");
    if(!m_bExpensive) {
        AudioHelp::getInstance()->playEffect("vo_nice_work_sold.mp3");
    }
    m_bReadySale = false;
    FoodData data = m_FoodDataVector.at(m_nSaleIndex);
    std::string path = data.packed?getStoragePath(data.path):data.path;
    
    Skin* skin = Skin::create(path);
    if (data.packed) {
        //                        skin->setScale(0.1);
        SaleManager::getInstance()->guestGetPizza(skin,1);
    }else{
        SaleManager::getInstance()->guestGetPizza(skin,2.2);
    }
    m_nSaleCount++;
    if (m_nSaleCount==1) {
        m_pGameUI->showNextLayout();
    }
    SaleManager::getInstance()->removeDialog();
    Node* pNode = m_PizzaPriceVector.at(m_nSaleIndex);
    Label* pLabel = dynamic_cast<Label*>(pNode->getChildByName("priceLabel"));
    if (pLabel) {
        pLabel->setString("");
    }
}
void PriceScene::_showParticle()
{
    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/powerStar.plist");
    this->addChildToContentLayer(pParticle);
    pParticle->setLocalZOrder(5);
    float poY = 20.0f+30*(arc4random()%20);
    float poX = 100.0f+50*(arc4random()%20);
    pParticle->setPosition(CMVisibleRect::getPosition(poX, poY));
}
