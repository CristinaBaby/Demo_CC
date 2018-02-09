
#include "SaleManager.h"
#include "cocos2d.h"
#include "GameDataAdapter.h"
#include "IAPManager.h"
#include "StringHelp.h"

USING_NS_CC;


SaleManager::SaleManager()
{
    
}

SaleManager::~SaleManager()
{
    
}

void SaleManager::setup()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for (int i = 0; i<4; i++) {
        std::stringstream ostr;
        ostr<<"Shop"<<i;
        if (GameDataAdapter::getBoolValue(ostr.str())) {
            IAPManager::getInstance()->setItemBoughtWithIndex(i);
        }
    }
#endif
}

RoleModel* SaleManager::produceGuest()
{
    RoleModel* lRole = RoleModel::create();
    static int radom = 0;
    radom++;
    radom = radom%2;
//    radom = 1;
    std::stringstream ostr;
    ostr<<"guest"<<radom;
    if (radom==0) {
        lRole->setRole("res/"+ostr.str()+"/", ostr.str(), 5);
    }else{
        
        lRole->setRole("res/"+ostr.str()+"/", ostr.str(), 4);
    }
    m_pRole = lRole;
    m_nIndex = radom;
    return lRole;
}
void SaleManager::guestCome()
{
    m_pRole->playArmation("walk", true);
}

void SaleManager::guestStandby(){
    m_pRole->gotoAndPause(20);
}
void SaleManager::guestStandby2(){
    m_pRole->gotoAndPause(200);
}
void SaleManager::guestBack()
{
    m_pRole->playArmation("go", true);
    Armature* pArmature = m_pRole->getArmature();
    Bone* pBone = pArmature->getBone("pizza");
    if(m_nIndex==0){
//        pBone->setLocalZOrder(11);
    }else if(m_nIndex==1){
        pBone->setLocalZOrder(11);
    }
}
void SaleManager::guestGetPizza(Node* pizza,float scale)
{
    m_pRole->playArmation("buy", false);
    Armature* pArmature = m_pRole->getArmature();
    Bone* pBone = pArmature->getBone("pizza");
//    Bone* pBone = CocoStudioHelper::getBone(pArmature, "pz", "arm2");
    pBone->addDisplay(pizza,0);
//    pBone->setLocalZOrder(11);
    pBone->setIgnoreMovementBoneData(true);
    pBone->setScale(scale);
    pBone->changeDisplayWithIndex(0, true);
    pBone->addChild(pizza);
}

void SaleManager::guestGetCoint(Node* pCoint)
{
    Armature* pArmature = m_pRole->getArmature();
    Bone* pBone = pArmature->getBone("coin");
    pBone->setLocalZOrder(15);
    pBone->addDisplay(pCoint,0);
//    pBone->setIgnoreMovementBoneData(true);
    pBone->changeDisplayWithIndex(0, true);
}
void SaleManager::guestPay(Node* pCoint)
{
    Armature* pArmature = m_pRole->getArmature();
    Bone* pBone = pArmature->getBone("coin");
    Node* pSkin = Node::create();
    pBone->addDisplay(pSkin,0);
    pBone->changeDisplayWithIndex(0, true);
    pBone->setVisible(false);
    Vec2 pos = CocoStudioHelper::getBoneWorlPoint(pArmature, "coin");
    pCoint->setPosition(pos);
}

void SaleManager::showDialog(Node* pTip)
{
    m_pDialog = Sprite::create("content/showcase/dialog.png");
    m_pRole->addChild(m_pDialog);
    m_pDialog->setPosition(Vec2(300, 500));
    m_pDialog->setScale(1.2);
    m_pDialog->addChild(pTip);
    pTip->setPosition(Vec2(120, 100));
}
void SaleManager::removeDialog()
{
    if(m_pDialog){
        m_pDialog->removeFromParent();
    }
}

void SaleManager::guestSad()
{
    m_pRole->playArmation("sad", false);
}