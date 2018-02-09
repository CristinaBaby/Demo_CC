
#include "SaleManager.h"
#include "cocos2d.h"
#include "GameDataAdapter.h"
#include "IAPManager.h"
#include "StringHelp.h"

USING_NS_CC;


RoleModel* SaleManager::produceGuest()
{
    log("----SaleManager-----%s",__func__);
    RoleModel* lRole = RoleModel::create();
    static int radom = 0;
    radom++;
    radom = radom%4;
//    radom = 0;
    std::stringstream ostr;
    ostr<<"boy"<<radom;
    if (radom>1) {
        lRole->setRole(ostr.str()+"/", ostr.str(), 1);
    }else{
        lRole->setRole(ostr.str()+"/", ostr.str(), 2);
    }
    m_pRole = lRole;
    m_nIndex = radom;
    return lRole;
}
void SaleManager::guestCome()
{
    log("----SaleManager-----%s",__func__);
    m_pRole->playArmation("run", true);
}

void SaleManager::guestStandby(){
    log("----SaleManager-----%s",__func__);
    m_pRole->gotoAndPause(135);
}
void SaleManager::guestStandby2(){
    log("----SaleManager-----%s",__func__);
    m_pRole->gotoAndPause(135);
}

void SaleManager::guestStandbyPay(){
    m_pRole->gotoAndPause(65);
    
}
void SaleManager::guestBack()
{
    log("----SaleManager-----%s",__func__);
    m_pRole->playArmation("food", true);
//    Armature* pArmature = m_pRole->getArmature();
//    Bone* pBone = pArmature->getBone("cookie");
//    if(m_nIndex==0){
////        pBone->setLocalZOrder(11);
//    }else if(m_nIndex==1){
//        pBone->setLocalZOrder(11);
//    }
}
void SaleManager::guestGetPizza(Node* pizza,float scale)
{
    log("----SaleManager-----%s",__func__);
    m_pRole->playArmation("na", false);
    Armature* pArmature = m_pRole->getArmature();
    Bone* pBone = pArmature->getBone("cookie");
    
    pBone->addDisplay(pizza, 1);
    pBone->setIgnoreMovementBoneData(true);
    pBone->setScale(scale);
    pBone->changeDisplayWithIndex(1, true);
//    pBone->addChild(pizza);
}

void SaleManager::guestGetCoint(Node* pCoint)
{
    log("----SaleManager-----%s",__func__);
    Armature* pArmature = m_pRole->getArmature();
    Bone* pBone = pArmature->getBone("coin");
    pBone->setLocalZOrder(15);
    pBone->addDisplay(pCoint,0);
    
    pBone->changeDisplayWithIndex(0, true);
}
void SaleManager::guestPay(Node* pCoint)
{
    log("----SaleManager-----%s",__func__);
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
    log("----SaleManager-----%s",__func__);
    m_pDialog = Sprite::create("content/sale/dialog.png");
    m_pRole->addChild(m_pDialog);
    m_pDialog->setPosition(Vec2(300, 500));
    m_pDialog->setScale(2.3);
    m_pDialog->addChild(pTip);
}
void SaleManager::removeDialog()
{
    log("----SaleManager-----%s",__func__);
    if(m_pDialog){
        m_pDialog->removeFromParent();
    }
}

void SaleManager::guestSad()
{
    log("----SaleManager-----%s",__func__);
    m_pRole->playArmation("sad", false);
}