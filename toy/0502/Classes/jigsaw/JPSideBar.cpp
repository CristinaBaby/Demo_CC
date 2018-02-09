#include "../utilities/UICpp.h"
#include "JPSideBar.h"
#include "../widgets/TileSprite.h"
#include "../utilities/STVisibleRect.h"


#define MIN_MOVE            (10)

JPSideBar* JPSideBar::create(float scale , float separationSpace, Layout *root)
{
    JPSideBar* widget = new JPSideBar();
    widget->zoomScale = scale;
    widget->separationSpace = separationSpace;
    widget->_root = root;
    
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}


bool JPSideBar::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    //判断它是否在点击区域外(复制 Widget::onTouchBegan 的代码)
    _hitted = false;
    if (isVisible() && isEnabled() && isAncestorsEnabled() && isAncestorsVisible(this) )
    {
        _touchBeganPosition = touch->getLocation();
        if(hitTest(_touchBeganPosition) && isClippingParentContainsPoint(_touchBeganPosition))
        {
            _hitted = true;
        }
    }
    if (!_hitted)
    {
        return false;
    }
    
    //判断点击到哪一个Tile上面了.
    Vector<Widget*> pChildren = getItems();
    
    for (auto layout : pChildren) {
        Sprite* tile = (Sprite*)(layout->getChildren().at(0));
        
        Vec2 nsp = tile->convertToNodeSpace(_touchBeganPosition);
        Rect bb;
        bb.size = tile->getContentSize();
        if (bb.containsPoint(nsp))
        {
            curSelectNode = tile;
            beginScale = curSelectNode->getScale();
            beginPoint = curSelectNode->getPosition();
            
            log("touch %d", curSelectNode->getTag());
            
            break;
        }
    }
    
    return ScrollView::onTouchBegan(touch, unusedEvent);
}


void JPSideBar::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    if (isDragContent && curSelectNode !=NULL) {
        
        Vec2 touchPoint = touch->getLocation();
        Vec2 preTouchPoint = touch->getPreviousLocation();
        
        if(abs(touch->getStartLocation().x - touchPoint.x) > MIN_MOVE || abs(touch->getStartLocation().y - touchPoint.y) > MIN_MOVE)
        {
            Vec2 point =  ccpSub(touchPoint,preTouchPoint);
            
            curSelectNode->setPosition(ccpAdd(curSelectNode->getPosition(),point));
        }
        
        //由当前的x位置与原始x的位置之间的差距得出scale
        float moveSelectNodeX = abs(curSelectNode->getPositionX() - beginPoint.x);

        float scale;
        scale = moveSelectNodeX * (endScale - beginScale) / 300 + beginScale;
        
        if (scale < beginScale) {
            scale = beginScale;
        }
        
        if (scale > endScale)
        {
            scale = endScale;
        }
        
        curSelectNode->setScale(scale);
        
        return;
    }
    
    //如果横向移动距离大于纵向移动距离, 就判断为拖动块, 否则判断为拖动列表
    float moveX = abs(touch->getStartLocation().x - touch->getLocation().x) ;
    float moveY = abs(touch->getStartLocation().y - touch->getLocation().y) ;
    
    if (moveX > MIN_MOVE && moveX > moveY && moveY < 150) {
        isDragContent = true;
        if(curSelectNode != NULL){
            //parent 都没有变, setPosition 是做什么
//            Vec2 point = getInnerContainer()->convertTouchToNodeSpace(touch);
//            curSelectNode->setPosition(point.x, point.y);
            
        }else
            ScrollView::onTouchMoved(touch, unusedEvent);
    }else{
        ScrollView::onTouchMoved(touch, unusedEvent);
    }
}


void JPSideBar::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    if (isDragContent) {
        isDragContent = false;
        
        if (curSelectNode!=NULL) {
            
            separationSpace = ((Sprite*)curSelectNode)->getContentSize().width*beginScale+50;
            
            //如果落点在ListView外面(离ListView一段距离内都算在ListView里面), 就把它放到root上面, 否则就弹回原来的地方.
            log("curSelectNode->getPositionY()=%f" , curSelectNode->getPositionY());
            
            float moveX = abs(touch->getStartLocation().x - touch->getLocation().x);
            if (moveX > 250)
            {
                curSelectNode->setScale(endScale);
                
                log("%f", curSelectNode->getPosition().x);
                Size size = curSelectNode->getContentSize();
                
                auto world = curSelectNode->convertToWorldSpace(Vec2(size.width/2, size.height/2));
                Vec2 pos = _root->convertToNodeSpace(world);
                
                curSelectNode->setPosition(pos);
                //V::setPosition(curSelectNode, pos);
                
                //删除current select tile
                curSelectNode->retain();
                removeChild(curSelectNode->getParent());
                requestRefreshView();                       //需要手动刷新
                curSelectNode->removeFromParent();
                curSelectNode->setLocalZOrder(110);          //比SideBar 大就行
                
                _root->addChild(curSelectNode);
                curSelectNode->release();
                ((TileSprite*)curSelectNode)->openTouchEvent(0);
            }else{
                curSelectNode->runAction(CCEaseIn::create(CCMoveTo::create(0.3f, beginPoint), 0.6f));
                curSelectNode->setScale(beginScale);
            }
            
            curSelectNode = nullptr;
        }
    }
    
    ScrollView::onTouchEnded(touch, unusedEvent);
}


void JPSideBar::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    JPSideBar::onTouchEnded(touch, unusedEvent);
}


void JPSideBar::setEndScale(float scale)
{
    endScale = scale;
}

