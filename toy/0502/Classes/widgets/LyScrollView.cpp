//
//  LyScrollView.cpp
//  my_demo
//
//  Created by liyang1 on 9/28/14.
//
//

#include "LyScrollView.h"
#include <math.h>
#include "TileSprite.h"
#include "../AppGlobal.h"
#include "../AppConfig.h"


LyScrollView::LyScrollView(){
    isSurrportZoom = false;
    isDragContent = false;
    zoomScale = 1.0f;
    
    Size size  = Director::getInstance()->getVisibleSize();
    float scaleX = size.width/768.f;
    float scaleY = size.height/1024.f;
    topDistrict = size.height - 123*MAX(scaleX, scaleY);
}

LyScrollView::~LyScrollView()
{

}


LyScrollView* LyScrollView::create(Size size, Node* container , float scale , float distance){
    LyScrollView* pRet = new LyScrollView();
    pRet->zoomScale = scale;
    pRet->separationSpace = distance;
    
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

LyScrollView* LyScrollView::create()
{
    LyScrollView* pRet = new LyScrollView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

Node *curSelectNode = NULL;
Vec2 beginPoint;
float beginScale = 0;
//float nodeScale = 0;

bool LyScrollView::onTouchBegan(Touch *pTouch, Event *pEvent){

    Vec2 nodePoint =   getParent()->convertToWorldSpace(pTouch->getLocation());
    //容器区域外的点击屏蔽
    
//    CCLOG("x left =%f, x right=%f" ,getContainer()->boundingBox()->getMinX() , getContainer->boundingBox()->getMaxX() );
//        CCLOG("y left =%f, y right=%f" ,getContainer()->boundingBox()->getMinY() , getContainer->boundingBox()->getMaxY() );
    if(nodePoint.y > topDistrict){
        return false;
    }
    Vector<Node*> pChildren = getContainer()->getChildren();
    
    
    for (auto pObj : pChildren) {
        Vec2 point = getContainer()->convertTouchToNodeSpace(pTouch);
        Sprite* pChild = (Sprite*)pObj;
        cocos2d::Rect rect = pChild->boundingBox();
        if (rect.containsPoint(point)) {
            //
            curSelectNode = pChild;
            beginScale = curSelectNode->getScale();
            
            beginPoint = curSelectNode->getPosition();
            break;
        }
    }
    
    return ScrollView::onTouchBegan(pTouch, pEvent);
};




void LyScrollView::onTouchMoved(Touch *pTouch, Event *pEvent){
//先判断鼠标的移动距离，若符合要求，则移动内容，若不符合要求，则继续传递给CCScrollView
     
     if (isDragContent && curSelectNode !=NULL) {
         
         Vec2 touchPoint = pTouch->getLocation();
         Vec2 preTouchPoint = pTouch->getPreviousLocation();
         
         if(abs(pTouch->getStartLocation().x - touchPoint.x) > 10 || abs(pTouch->getStartLocation().y - touchPoint.y) > 10)
         {
             Vec2 point =  ccpSub(touchPoint,preTouchPoint);
             
            curSelectNode->setPosition(ccpAdd(curSelectNode->getPosition(),point));
         }
         
        float delaySelectNodeY = curSelectNode->getPositionY() - beginPoint.y;
//         log("delaySelectNodeY=%f  getscale=%f" , delaySelectNodeY , curSelectNode->getScale());
         if (delaySelectNodeY <  -10*UI_SCALE && delaySelectNodeY > -210*UI_SCALE) {
             float piece = (-delaySelectNodeY - 50*UI_SCALE)/((210-10)*UI_SCALE);
//             log("piece=%f", piece);
             if(piece > beginScale){
               curSelectNode->setScale(piece);
             }
             
         }else if(delaySelectNodeY < -210*UI_SCALE){
             
             if(curSelectNode->getScale()!=1.0f){
                 curSelectNode->setScale(1.0f);
             }
         
         }
         
         return;
     }
     
     float delayX = abs(pTouch->getStartLocation().x - pTouch->getLocation().x) ;
     float delayY = abs(pTouch->getStartLocation().y - pTouch->getLocation().y) ;

     float delayPercentage = delayY / delayX;
     if (delayY > 10 && delayPercentage > 2.0) {
         isDragContent = true;
         if(curSelectNode != NULL){
             Vec2 point = getContainer()->convertTouchToNodeSpace(pTouch);
             curSelectNode->setPosition(point.x, point.y);
             
         }else
             ScrollView::onTouchMoved(pTouch, pEvent);
     }else{
         ScrollView::onTouchMoved(pTouch, pEvent);
     }
 };

 void LyScrollView::onTouchEnded(Touch *pTouch, Event *pEvent){
     isDragContent = false;
     if (curSelectNode!=NULL) {
         
         separationSpace = ((Sprite*)curSelectNode)->getContentSize().width*beginScale+50*UI_SCALE;

         log("curSelectNode->getPositionY()=%f" , curSelectNode->getPositionY());
         if (curSelectNode->getPositionY() < -(curSelectNode->getContentSize().height/2*curSelectNode->getScale()) ) {
             curSelectNode->setScale(1.0);
             
             
             Vector<Node*> children = getContainer()->getChildren();
             for (auto pObj : children) {
                 Sprite* pChild = (Sprite*)pObj;
                 
                 if (pChild->getPositionX() > beginPoint.x) {
                     pChild->runAction(CCEaseIn::create(CCMoveBy::create(0.2f, Vec2(-separationSpace, 0)), 0.6f));
                 }
             }
 
             getContainer()->setContentSize(Size(getContainer()->getContentSize().width - separationSpace, getContainer()->getContentSize().height));
             Vec2 worldPoint = getContainer()->convertToWorldSpace(curSelectNode->getPosition());
             
             curSelectNode->retain();
             getContainer()->removeChild(curSelectNode, true);
             
             getContainer()->getParent()->getParent()->addChild(curSelectNode);
             curSelectNode->setPosition(worldPoint);
             ((TileSprite*)curSelectNode)->openTouchEvent(0);
         }else{
             curSelectNode->runAction(CCEaseIn::create(CCMoveTo::create(0.3f, beginPoint), 0.6f));
             curSelectNode->setScale(beginScale);
         }
         
         curSelectNode = NULL;
     }
     ScrollView::onTouchEnded(pTouch, pEvent);
     
 }

 void LyScrollView::onTouchCancelled(Touch *pTouch, Event *pEvent){
    
         LyScrollView::onTouchEnded(pTouch, pEvent);
 }












