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
#include "Global.h"
#include "ActionUtils.h"

static vector<string> IAPTypes={"shop_btn_christmas pack@2x.png","shop_btn_decoration@2x.png","shop_btn_cone@2x.png","shop_btn_fruit@2x.png",
    "shop_btn_restore@2x.png"};
ShopLayer::ShopLayer()
{
    
}
ShopLayer::~ShopLayer()
{
    
}

void ShopLayer::onEnter()
{
    BaseLayer::onEnter();
    
    if (kIAPManager->isShowAds()) {
        
//        AdsManager::getInstance()->setVisiable(ADS_TYPE::kTypeBannerAds, false);
    }
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
    
    Sprite* m_pBgSprite=Sprite::create("images/shop_fav/shop.jpg");
    kAdapterScreen->setExactPosition(m_pBgSprite, 480, 320);
    this->addToBackGroundLayer(m_pBgSprite, 3);

    auto backBtn = ui::Button::create("images/button/btn_back@2x.png");
    backBtn -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    backBtn->setTag(30);
    kAdapterScreen->setExactPosition(backBtn, 10, 10,Vec2(0, 1),kBorderLeft,kBorderTop);
    this->addToUILayer(backBtn, 20);
   
    auto buyAll = ui::Button::create("images/shop_fav/shop_btn_buy@2x.png");
    buyAll -> addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchEvent, this));
    buyAll->setTag(40);
    buyAll->setPosition(Vec2(555,445));
    m_pBgSprite->addChild(buyAll);
    
    

    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getWinSize().width, 300));
    
    m_pGirdView->m_fHSpace = 0;
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft  = 100;
    m_pGirdView->m_bNeedAdjust  = false;
    
    m_pGirdView->ignoreAnchorPointForPosition(false);
    m_pGirdView->setEnable(false);
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 0-20,Vec2(0,0),kBorderLeft,kBorderBottom);
    m_pGirdView->setScrollCallback(this);
    m_pAdapter = KShopAdapter::create(m_vTypes);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(ShopLayer::onBuyCallback,this));
    m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-2391, 0), 0.1);
    
    this->addToContentLayer(m_pGirdView, 1);
    
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

//        kIAPManager->restore();
        //购买所有
        if(!kIAPManager->isPackagePurchased(4))
        {
            
            kIAPManager->purchase(4);
            
        }
        else
        {
            
            Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            
           
            dialog->setContentText("You have already purchased this package!");
          
            
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
        
    }
}

void ShopLayer::onScrolling(float offsetX,float offsetY)
{

    log("%f %f",offsetX,offsetY);
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

    if(index == 4){
        
        kIAPManager->restore();
        return;
    }
    
    if(!kIAPManager->isPackagePurchased(index))
    {

        kIAPManager->purchase(index);
        
    }
    else
    {
        
        Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
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
    
    Dialog* dialog= Dialog::create(Size(440,327), (void*)kDialogReset, Dialog::DIALOG_TYPE_SINGLE);
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
    log("Purchase is successful");
}

