//
//  ShopLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#include "ShopLayer.h"
#include "SceneManager.h"
#include "IAPManager.h"
#include "AudioHelp.h"
#include "STAds.h"
#include "Global.h"
#include "ActionUtils.h"

static vector<string> IAPTypes={"shop_li_1.png","shop_li_2.png","shop_li_3.png",
    "shop_li_4.png","shop_li_5.png","shop_li_6.png"};
ShopLayer::ShopLayer()
{
    
}
ShopLayer::~ShopLayer()
{
    
}

void ShopLayer::onEnter()
{
    BaseLayer::onEnter();
    STAds ads;
    ads.removeAds();
    
    
    auto func = CallFunc::create([=](){
        
        
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(0, 0), 1);
        
        
    });
    auto func1 = CallFunc::create([=](){
        
        
        
        m_pGirdView->setEnable(true);
        
    });
    
    
    this->runAction(Sequence::create(DelayTime::create(1.0),func, NULL));
    
    ActionUtils::delayTimeAction(this,2.0,func1);
    
}
void ShopLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}

void ShopLayer::onExit()
{
    BaseLayer::onExit();
}
bool ShopLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        isFirstIn=true;
        
        
        initData();
        initUI();
        isInit=true;
        
    }while(0);
    
    return true;
    
}
void ShopLayer::initData()
{
  
    for(auto i = IAPTypes.begin();i<IAPTypes.end();i++)
    {
        string buypath = (*i).c_str();
        Sprite* data = Sprite::create("images/shop_fav/"+buypath);
        m_vTypes.pushBack(data);
    }
}
void ShopLayer::initUI()
{
    kIAPManager->m_fnAfterPurchase = CC_CALLBACK_1(ShopLayer::purchaseFinishCallback, this);
    kIAPManager->m_fnAfterRestore = CC_CALLBACK_2(ShopLayer::AfterRestoreCallBack, this);
    Sprite* m_pBgSprite=Sprite::create("images/shop_fav/shop_bg.png");
    kAdapterScreen->setExactPosition(m_pBgSprite, 320, 480);
    this->addToBackGroundLayer(m_pBgSprite, 3);
   
    Sprite* bg=Sprite::create("images/shop_fav/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addToBackGroundLayer(bg, 1);
    
    auto bg_logo =Sprite::create("images/shop_fav/shop_logo.png");
    kAdapterScreen->setExactPosition(bg_logo, 320, 10,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addToBackGroundLayer(bg_logo, 1);
    
    
    auto backBtn = ui::Button::create("images/shop_fav/shop_btn_close.png");
    backBtn -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    kAdapterScreen->setExactPosition(backBtn, 320 + 180, 20,Vec2(0.5,1),kBorderNone,kBorderTop);
    backBtn->setTag(30);
    this->addToBackGroundLayer(backBtn, 10);
   
    auto restore = ui::Button::create("images/shop_fav/shop_btn_Restore-Purchases.png");
    restore -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    restore->setTag(40);
    restore->setAnchorPoint(Vec2(0.5,0.5));
    kAdapterScreen->setExactPosition(restore, 320, 100,Vec2(0.5, 0.5),kBorderNone,
                                     kBorderBottom);
    this->addToUILayer(restore, 20);

    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getWinSize().width, 700));
    
    m_pGirdView->m_fHSpace = 10;
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fMarginRight = Director::getInstance()->getWinSize().width/2 -  513/2 ;
    m_pGirdView->m_fMarginLeft  =  Director::getInstance()->getWinSize().width/2 -  513/2;
    m_pGirdView->m_bNeedAdjust  = true;
    
    m_pGirdView->ignoreAnchorPointForPosition(false);
    m_pGirdView->setEnable(false);
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 150,Vec2(0,0),kBorderLeft,kBorderBottom);
    m_pGirdView->setScrollCallback(this);
    m_pAdapter = KShopAdapter::create(m_vTypes);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(ShopLayer::onBuyCallback,this));
    m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-2391, 0), 0.1);
    
    this->addToContentLayer(m_pGirdView, 1);
    
    if(m_pGirdView->m_pMenuItemsVector.size()>0){
        for(int i = 0; i < m_pGirdView->m_pMenuItemsVector.size(); i ++){
            Point pos = m_pGirdView->m_pMenuItemsVector.at(i)->getParent()->convertToWorldSpace(m_pGirdView->m_pMenuItemsVector.at(i)->getPosition());
            float dis = pos.distance(m_pGirdView->getParent()->convertToWorldSpace(m_pGirdView->getPosition()) + Point(m_pGirdView->getContentSize()/2));
            if(i == 0){
                log("dis = %f", dis);
            }
            float scale = 1 - dis / (m_pGirdView->getContentSize().width/2);
            if(scale < 0.5)
                scale = 0.5;
            (m_pGirdView->m_pMenuItemsVector).at(i)->setScale(scale);
        }
        m_pGirdView->m_pMenuItemsVector.at(0)->getParent()->reorderChild(m_pGirdView->m_pMenuItemsVector.at(0), 1);
    }

    m_pGirdView->m_pScrollView->setBounceable(true);
    
}

void ShopLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    if(type == ui::Widget::TouchEventType::ENDED&&btn->getTag() == 30)
    {
        SceneManager::getInstance()->popScene();
        AudioHelp::getInstance()->playDeleteEffect();
        
    }else if (type == ui::Widget::TouchEventType::ENDED&&btn->getTag() == 40)
    {

            kIAPManager->restore();
        
        
    }
}

void ShopLayer::onScrolling(float offsetX,float offsetY)
{
    if(m_pGirdView->m_pMenuItemsVector.size()>0){
        for(int i = 0; i < m_pGirdView->m_pMenuItemsVector.size(); i ++){
            Point pos = m_pGirdView->m_pMenuItemsVector.at(i)->getParent()->convertToWorldSpace(m_pGirdView->m_pMenuItemsVector.at(i)->getPosition());
            float dis = pos.distance(m_pGirdView->getParent()->convertToWorldSpace(m_pGirdView->getPosition()) + Point(m_pGirdView->getContentSize()/2));
            if(i == 0){
                log("dis = %f", dis);
            }
            float scale = 1 - dis / (m_pGirdView->getContentSize().width/2);
            if(scale < 0.5)
                scale = 0.5;
            (m_pGirdView->m_pMenuItemsVector).at(i)->setScale(scale);
            if(dis < 70){
                m_pGirdView->m_pMenuItemsVector.at(i)->getParent()->reorderChild(m_pGirdView->m_pMenuItemsVector.at(i), 1);
            }
        }
    }
    
}
void ShopLayer::onScrollEnd(float offsetX,float offsetY)
{

    
}
void ShopLayer::onScrollDidZoom(cocos2d::extension::ScrollView* view)
{
  

}
void ShopLayer::onBuyCallback(int tag,MenuItem* menuItem,int index)
{
    log("-----------IAP %d",index);

    if(!kIAPManager->isPackagePurchased(index))
    {
        
         kIAPManager->purchase(index);
        
    }
    else
    {
        Dialog* dialog= Dialog::create(Size(460,300), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
        kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
        dialog->setContentText("You have already purchased this package!");
        dialog->setPositiveBtnText("");
        dialog->setNegativeBtnText("");
        dialog->setCallback(this);
        this->addToUILayer(dialog, 9999);
    }
    
    
}
void ShopLayer::AfterRestoreCallBack(bool isRestore,bool isPurchase)
{
    
    if (kIAPManager->isPackagePurchased(2) && kIAPManager->isPackagePurchased(3) && kIAPManager->isPackagePurchased(4)) {
        
        //若三个包解锁,则解锁这个包
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.beachfoodpro");
        
        
    }else if(kIAPManager->isPackagePurchased(1)){
        
        //解锁其他两个包
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.icepop");
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.icecream");
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.snowcone");
        
    }else if(kIAPManager->isPackagePurchased(0)){
        
        
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.beachfoodpro");
        
    }else if(kIAPManager->isPackagePurchased(2) && kIAPManager->isPackagePurchased(3) && kIAPManager->isPackagePurchased(4)&&kIAPManager->isPackagePurchased(5)){
    
        
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.beachfood");
    
    }else if(kIAPManager->isPackagePurchased(5)&&kIAPManager->isPackagePurchased(1)){
    
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.beachfood");
    
    }
    
    
    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
    dialog->setPositiveBtnText("");
    dialog->setNegativeBtnText("");
    dialog->setCallback(this);
    
    if(isRestore)
    {
        if(isPurchase)
        {
            dialog->setContentText("Your content has been restored!");
        }
        else
        {
            dialog->setContentText("Sorry! It looks like you haven't purchased anything yet.");
            
        }
    }
    else
    {
        dialog->setContentText("Sorry, restore transaction failed !");
    }
    this->addToUILayer(dialog, 9999);


}
void ShopLayer::onNegativeClick(void* type)
{
}

void ShopLayer::onPositiveClick(void* type)
{
}

void ShopLayer::purchaseFinishCallback(bool isPurchase)
{
    cocos2d::CCNotificationCenter::getInstance()->postNotification("purchase_Finish");
    if (kIAPManager->isPackagePurchased(2) && kIAPManager->isPackagePurchased(3) && kIAPManager->isPackagePurchased(4)) {
        
        //若三个包解锁,则解锁这个包
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.beachfoodpro");
        
        
    }else if(kIAPManager->isPackagePurchased(1)){
        
        //解锁其他两个包
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.icepop");
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.icecream");
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.snowcone");
        
    }else if(kIAPManager->isPackagePurchased(0)){
        
        
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.beachfoodpro");
        
    }else if(kIAPManager->isPackagePurchased(2) && kIAPManager->isPackagePurchased(3) && kIAPManager->isPackagePurchased(4)&&kIAPManager->isPackagePurchased(5)){
        
        
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.beachfood");
        
    }else if(kIAPManager->isPackagePurchased(5)&&kIAPManager->isPackagePurchased(1)){
        
        kIAPManager->purchaseSuccessfulNocallback("com.crazycatsmedia.beachfoodparty.beachfood");
        
    }

    
//    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
//    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
//    if(!isPurchase)
//            dialog->setContentText("\n     Purchase failed.");
//    else
//            dialog->setContentText("Thank you for your \npurchase.");
//        
//    dialog->setPositiveBtnText("");
//    dialog->setNegativeBtnText("");
//    dialog->setCallback(this);
//    this->addToUILayer(dialog, 9999);
    
//    _eventDispatcher->dispatchCustomEvent("after_purchase");
    
    
    
}

