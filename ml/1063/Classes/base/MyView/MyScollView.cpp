//
//  ScollView.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 9/4/14.
//
//

#include "MyScollView.h"
#include "math.h"
USING_NS_KD;
USING_NS_CC_EXT;

static float convertDistanceFromPointToInch(float pointDis)
{
    auto glview = Director::getInstance()->getOpenGLView();
    float factor = ( glview->getScaleX() + glview->getScaleY() ) / 2;
    return pointDis * factor / Device::getDPI();
}


MyScollView::MyScollView(){
    isMove = false;
    space =0;
    marginRight = 0;
    marginLeft = 0;
    marginTop = 0;
    marginBottom = 0;
    ignoreAnchorPointForPosition(false);
    currAdpter = nullptr;
    curentItem = nullptr;
    _unClipp = nullptr;
    
}

void MyScollView::addChild(Node * child, int zOrder, const std::string &name)
{
    if (_container != child && _unClipp!=child)
    {
        _container->addChild(child, zOrder, name);
    }
    else
    {
        Layer::addChild(child, zOrder, name);
    }
}

void MyScollView::setSpace(float space){
    this->space = space;
}
MyScollView * MyScollView::create(float w, float h) {
    
    auto scol = new MyScollView();
    if(scol&& scol->init()) {
        scol->setViewSize(Size(w,h));
        scol->setContainer(Layer::create());
        scol->_unClipp = Layer::create();
        scol->addChild(scol->_unClipp,20,"");
        scol->autorelease();
        return scol;
    }
    delete scol;
    scol = nullptr;
    return scol;
}

void MyScollView::updateButton(){
    changeAdapter(currAdpter);
}

void MyScollView::changeAdapter(MyBaseAdapter* base){
    unscheduleAllSelectors();
    _dragging = false;
    _container->removeAllChildren();
    _container->stopAllActions();
    _container->setPosition(Vec2::ZERO);
    if(_unClipp != nullptr){
        _unClipp->removeAllChildren();
        _unClipp->setPosition(Vec2::ZERO);
    }
    //setContainer(Layer::create());
    setAdapter(base);
}

MyBaseAdapter *MyScollView::getAdapter(){
    return currAdpter;
}

void MyScollView::setAdapter(MyBaseAdapter *base){
    CC_SAFE_RETAIN(base);
    CC_SAFE_RELEASE_NULL(currAdpter);
    currAdpter = base;
    if(base == nullptr)
        return;
    base->setMyScollView(this);
    nodeCache.clear();
    float curentPositionX = marginLeft;
    float curentPositionY = marginBottom;
    float maxW = 0;
    float maxH =0;
    for(size_t i=0;i<base->getCount();i++) {
        auto btn = base->getItem(i);
        nodeCache.pushBack(btn);
        auto size = btn->getContentSize();
        if(maxW < size.width)
            maxW = size.width;
        if(maxH < size.height)
            maxH = size.height;
    }
    switch (getDirection()) {
        case Direction::VERTICAL:
            curentPositionX += std::max(maxW,_viewSize.width)*.5;
            for(int i = nodeCache.size()-1 ;i>=0;i--){
                auto btn =nodeCache.at(i);
                btn->touchSpriteBegan  = CC_CALLBACK_3(MyScollView::touchDown,this);
                btn->touchSpriteEnded  = CC_CALLBACK_3(MyScollView::touchEnd,this);
                curentPositionY+=btn->getContentSize().height*.5;
                btn->setPosition(Vec2(curentPositionX, curentPositionY));
                curentPositionY +=btn->getContentSize().height*.5+space;
                ScrollView::addChild(btn);
                btn->savePosition();
            }
            setContentSize(Size(curentPositionX+maxW*.5+marginRight,curentPositionY+marginTop-space));
            setContentOffset(Vec2(0,getViewSize().height-getContentSize().height));
            break;
        default:
            curentPositionY += std::max(maxH,_viewSize.height)*.5;
            for(size_t i = 0;i<nodeCache.size();i++){
                auto btn = nodeCache.at(i);
               	btn->touchSpriteBegan  = CC_CALLBACK_3(MyScollView::touchDown,this);
                btn->touchSpriteEnded  = CC_CALLBACK_3(MyScollView::touchEnd,this);
                curentPositionX+=btn->getContentSize().width*.5;
                btn->setPosition(Vec2(curentPositionX, curentPositionY));
                curentPositionX +=space + btn->getContentSize().width*.5+space;
                ScrollView::addChild(btn);
                btn->savePosition();
            }
            setContentSize(Size(curentPositionX+marginRight-space, curentPositionY+maxH*.5+marginTop));
            setContentOffset(Vec2(0,0));
    }
}

void MyScollView::setMarginLeft(float left){
    this->marginLeft = left;
}

void MyScollView::setMarginRight(float right){
    this->marginRight = right;
}

void MyScollView::setMarginTop(float top){
    this->marginTop = top;
}

void MyScollView::setMarginBottom(float bottom){
    this->marginBottom = bottom;
}

void MyScollView::touchEnd(EventSprite* btn,int tag, Touch *pTouch){
    if(!isMove||(btn->isRequsetMove())){
        do {
            CC_BREAK_IF(!currAdpter);
            auto cuCLick = currAdpter->getItemClick();
            CC_BREAK_IF(!cuCLick);
            cuCLick(btn,nodeCache.getIndex(btn) ,this);
        }while (false);
    } else
        btn->onTouchCancle(pTouch, nullptr);
    
    if(curentItem != nullptr){
        curentItem->setLockMove(false);
        curentItem = nullptr;
    }
}
bool MyScollView::touchDown(EventSprite* btn,int tag, Touch *pTouch){
    if(_unClipp != nullptr){
        if( _unClipp->getChildren().size() !=0 ){
            if(btn->getParent() == _unClipp)
                return true;
            else
                return false;
        }
    }
    
    if(!isVisible())
        return false;
    if(_clippingToBounds&& !Rect(0, 0, _viewSize.width, _viewSize.height).containsPoint(this->convertToNodeSpace(pTouch->getLocation())))
        return false;
    curentItem = btn;
    return true;
}

void MyScollView::onTouchMoved(Touch* touch, Event* event) {
    if(_unClipp != nullptr)
    _unClipp->setPosition(_container->getPosition());
    if(curentItem != nullptr && curentItem->isRequsetMove()) {
        unscheduleAllSelectors();
        getContainer()->stopAllActions();
        return;
    }
    
    while (_touches.size()>1)
        _touches.erase(_touches.begin());
    
    ScrollView::onTouchMoved(touch, event);
        if(_unClipp != nullptr)
    _unClipp->setPosition(_container->getPosition());
    
    Vec2 dis = touch->getLocation() -touch->getStartLocation();
    float distance = touch->getLocation().distance(touch->getStartLocation());
    float degree = dis.getAngle()/M_PI*180;
    switch (_direction) {
        case ScrollView::Direction::VERTICAL:
            if(std::abs(dis.y)>discardMove)
                isMove = true;
            if(distance>8&&curentItem != nullptr) {
                if(std::abs(degree)<60||std::abs(degree)>150)
                    curentItem->setLockMove(false);
                else
                    curentItem = nullptr;
            }
            break;
        default:
            if(std::abs(dis.x)>discardMove)
                isMove = true;
            if(distance>8&&curentItem != nullptr) {
                if(std::abs(degree)>30&&std::abs(degree)<150)
                    curentItem->setLockMove(false);
                else
                    curentItem = nullptr;
                
            }
            break;
    }
    if(_unClipp!= nullptr&& curentItem != nullptr && curentItem->isRequsetMove()) {
        
        
        auto p = curentItem->getParent()->convertToWorldSpace(curentItem->getPosition());
        curentItem->removeFromParentAndCleanup(false);
        _unClipp->removeAllChildren();
        curentItem->setPosition(_unClipp->convertToNodeSpace(p));
        _unClipp->addChild(curentItem);
        curentItem->resume();
        curentItem->backEndToOrn = [&](EventSprite* _t){
            auto p = _t->getParent()->convertToWorldSpace( _t->getPosition());
            _t->removeFromParentAndCleanup(false);
            _t->setPosition(_container->convertToNodeSpace(p));
            _container->addChild(_t);
            _t->resume();
        };
        
    }
}
bool MyScollView::onTouchBegan(Touch *touch, Event *event) {
    if(_unClipp != nullptr&& _unClipp->getChildren().size()!=0)
        return false;
    isMove = false;
    _touchMoved = false;
    
    _touches.clear();
    bool flag = ScrollView::onTouchBegan(touch, event);
    if(nullptr != curentItem)
        curentItem->setLockMove(flag);
    if(_unClipp != nullptr)
        _unClipp->setPosition(_container->getPosition());
    return flag&&isVisible();
}

void MyScollView::onTouchEnded(Touch *touch, Event *event) {
    if(nullptr != curentItem) {
        curentItem->setLockMove(false);
        curentItem = nullptr;
    }
    ScrollView::onTouchEnded(touch, event);
}
void MyScollView::onTouchCancelled(Touch *touch, Event *event) {
    if(nullptr != curentItem) {
        curentItem->setLockMove(false);
        curentItem = nullptr;
    }
    ScrollView::onTouchCancelled(touch, event);
}

void MyScollView::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    if(_unClipp  == nullptr){
        extension::ScrollView::visit(renderer, parentTransform, parentFlags);
        return;
    }
    // quick return if not visible
    if (!isVisible())
    {
        return;
    }
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    
    this->beforeDraw();
    if (!_children.empty())
    {
        int i=0;
        
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at(i);
            if ( child->getLocalZOrder() < 0 )
            {
                if(_unClipp != nullptr )
                    child->visit(renderer, _modelViewTransform, flags);
            }
            else
            {
                break;
            }
        }
        
        // this draw
        this->draw(renderer, _modelViewTransform, flags);
        
        // draw children zOrder >= 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at(i);
            if(child != _unClipp)
                child->visit(renderer, _modelViewTransform, flags);
        }
        
    }
    else
    {
        this->draw(renderer, _modelViewTransform, flags);
    }
    
    this->afterDraw();
    if(_unClipp != nullptr )
        _unClipp->visit(renderer, _modelViewTransform, flags);
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

