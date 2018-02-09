
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
    radom = radom%3;
//    radom = 0;
    std::stringstream ostr;
    ostr<<"guest"<<radom;
    if (radom==1) {
        lRole->setRole("res/"+ostr.str()+"/", ostr.str(), 4);
    }else{
        lRole->setRole("res/"+ostr.str()+"/", ostr.str(), 5);
    }
    m_pRole = lRole;
    m_nIndex = radom;
    return lRole;
}
void SaleManager::guestCome()
{
    log("----SaleManager-----%s",__func__);
    m_pRole->playArmation("walk", true);
    Armature* pArmature = m_pRole->getArmature();
    Bone* pBone = pArmature->getBone("eye");
    if(m_nIndex==1){
        pBone->changeDisplayWithIndex(5, true);
    }else{
        pBone->changeDisplayWithIndex(4, true);
    }
}

void SaleManager::guestStandby(){
    log("----SaleManager-----%s",__func__);
    m_pRole->gotoAndPause(40);
}
void SaleManager::guestStandby2(){
    log("----SaleManager-----%s",__func__);
    m_pRole->gotoAndPause(200);
}

void SaleManager::guestStandbyPay(){
    m_pRole->gotoAndPause(65);
    
}
void SaleManager::guestBack()
{
    log("----SaleManager-----%s",__func__);
    Armature* pArmature = m_pRole->getArmature();
    Bone* pBone = pArmature->getBone("donut");
    
    Bone* pDonutBone = CocoStudioHelper::getBone(pArmature, "donutBone", "root");
    Node* pNode = Node::create();
    pDonutBone->addDisplay(pNode,0);
    pDonutBone->changeDisplayWithIndex(0, true);
    
    m_pRole->playArmation("walk", true);
    
//    if(m_nIndex==0){
////        pBone->setLocalZOrder(11);
//    }else if(m_nIndex==1){
//        pBone->setLocalZOrder(11);
//    }
}
//void changeSpriteFrame(cocostudio::Armature* _armature,const string& searchPath){
//    auto _boneDic = _armature->getBoneDic();
//    for(auto _child:_boneDic.keys()) {
//        auto _list = _boneDic.at(_child)->getDisplayManager()->getDecorativeDisplayList();
//        for(auto _node:_list){
//            auto  _name = string(_node->getDisplayData()->displayName.c_str());
//            const char* temp = strtok((char*)_name.c_str(), ".");
////            auto _tempSpriteFrame = getSpriteFrame(temp,searchPath);
////            if(_tempSpriteFrame != nullptr){
//                auto _skin = dynamic_cast<Skin*>(_node->getDisplay());
//                if(nullptr != _skin){
////                    _skin->setSpriteFrame(_tempSpriteFrame);
//                }
////            } else {
////                CCLOG((searchPath+"/"+temp+".png not found").c_str());
////                auto _skin = dynamic_cast<Skin*>(_node->getDisplay());
////                if(nullptr != _skin){
////                    _skin->setSpriteFrame(Sprite::create()->getSpriteFrame());
////                }
////            }
//        }
//    }
//    
//}
void SaleManager::guestGetPizza(Node* pizza,float scale,int type)
{
    log("----SaleManager-----%s",__func__);
    m_pRole->playArmation("eat", false);
    Armature* pArmature = m_pRole->getArmature();
    Bone* pBone = pArmature->getBone("donut");
    pBone->setIgnoreMovementBoneData(true);
    Node* pNode = Node::create();
    pBone->addDisplay(pNode,0);
    pBone->changeDisplayWithIndex(0, true);
    
    Bone* pDonutBone = CocoStudioHelper::getBone(pArmature, "donutBone", "root");
    if (type==0) {
        pizza->setPosition(Vec2(-pizza->getContentSize().width/2, 10));
    }else{
        pizza->setRotation(-90);
        pizza->setPosition(Vec2(-pizza->getContentSize().width+20, pizza->getContentSize().height));
    }
    if(m_nIndex==2){
        pDonutBone->setLocalZOrder(14);
    }
    pDonutBone->setVisible(false);
    pDonutBone->runAction(Sequence::create(DelayTime::create(0.1),
                                           CallFunc::create([=](){
        pDonutBone->setVisible(true);
        pDonutBone->addDisplay(pizza,0);
        pDonutBone->setIgnoreMovementBoneData(true);
        pDonutBone->setScale(scale);
        pDonutBone->changeDisplayWithIndex(0, true);
        pDonutBone->addChild(pizza);
    }), NULL));
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
    m_pRole->addChild(pTip);
    pTip->setPosition(Vec2(300, 500));
//    m_pDialog = Sprite::create("content/sale/dialog.png");
//    m_pRole->addChild(m_pDialog);
//    m_pDialog->setPosition(Vec2(300, 500));
//    m_pDialog->setScale(2.3);
//    m_pDialog->addChild(pTip);
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