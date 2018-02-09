//
//  EatScribbleView.cpp
//  Make
//
//  Created by QAMAC01 on 15-4-7.
//
//

#include "EatScribbleView.h"
#include "KSVisibleRect.h"

#define visibleSize     Director::getInstance() ->getVisibleSize()

const std::string solidBrushPath = "button/eat_mask.png";

Sprite* test_;

//EatScribbleView::EatScribbleView(){
//    _scribble = new Scribble(Sprite::create(solidBrushPath));
//    _scribble->setBrushShader();
//    _scribble->setBrushType(Scribble::eBrush);
//    _scribble->setTargetSolid(true);
//    _scribble->setTargetAlphaTestValue(0.0f);
//}
//
//EatScribbleView::~EatScribbleView(){
//    _targetCandyCoating->release();
//    delete _scribble;
//}
bool EatScribbleView::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    restorePixels_ = 1;
    checkPercent_ = false;
    
    showLayer_ = Layer::create();
    this -> addChild(showLayer_);
    
    eatLayer_ = Layer::create();
    this -> addChild(eatLayer_);
    
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(EatScribbleView::touchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(EatScribbleView::touchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(EatScribbleView::touchEnded, this);
    listener -> retain();
    
    
    return true;
}

void EatScribbleView::allowEat()
{

    Point delta = KSVisibleRect::getDesignOffset();
    
    log("visibleSize.widthvisibleSize.width%f" ,showLayer_->getContentSize().width );
    log("visibleSize.widthvisibleSize.width%f" ,showLayer_->getContentSize().height );
    showLayer_ -> setPosition(delta.x,delta.y);
    
    _canvas = RenderTexture::create(visibleSize.width, visibleSize.height);
    _canvas -> setPosition(visibleSize.width/2-delta.x , visibleSize.height/2-delta.y);
    eatLayer_ ->addChild(_canvas, 10);
    
    _canvas->begin();
    showLayer_->visit();
    _canvas->end();
    Director::getInstance() -> getRenderer() -> render();
    
    restorePixels_ = calcute(_canvas -> newImage());
    showLayer_ -> setVisible(false);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener -> clone(), this);
   
    showLayer_ -> setPosition(Vec2::ZERO);

//    eatLayer_ -> setPositionY(-delta.y);
//    Image* img;
//    Sprite* sp = Sprite::createWithTexture(Director::getInstance() -> getTextureCache() -> addImage(img, "xxxx"));

}


void EatScribbleView::forbidEat()
{
    eatLayer_ -> removeAllChildren();
    this->getEventDispatcher() -> removeEventListenersForTarget(this);
    showLayer_ -> setVisible(true);
    tapCount_ = 0;
    _checkEatAgain = true;

}



long EatScribbleView::calcute(Image *pImage)
{
    long lCounter = 1;
    for (long i = 0; i < pImage->getHeight(); ++i) {
        for (long j = 0; j < pImage->getWidth(); ++j) {
            unsigned char *lData = pImage->getData();
            long lPixcelIndex = i * pImage->getWidth() + j;
            unsigned char lRed = lData[lPixcelIndex * 4];
            unsigned char lGreen = lData[lPixcelIndex * 4 + 1];
            unsigned char lBlue = lData[lPixcelIndex * 4 + 2];
            unsigned char lAlpha = lData[lPixcelIndex * 4 + 3];
            if (lAlpha > 10) {
                if (lRed > 0 || lGreen > 0 || lBlue > 0) {
                    ++lCounter;
                }
            }
        }
    }
    delete pImage;
    return lCounter;


}

float EatScribbleView::getPecent()
{
    return (float)calcute(_canvas -> newImage())/restorePixels_;
}


bool EatScribbleView::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());//获取的当前触摸的目标
    
    Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (!canEat )
    {
        return false;
    }
    
    if (rect.containsPoint(locationInNode))
    {
        
        Point delta = KSVisibleRect::getDesignOffset();

        
        Sprite *lBrush = Sprite::create(solidBrushPath);
        lBrush -> setPosition(locationInNode + delta);
        _canvas -> addChild(lBrush);
        CocosDenshion::SimpleAudioEngine::getInstance() -> playEffect("sound/eat.mp3");

        
        auto chosenP = ParticleSystemQuad::create("biting.plist");
        chosenP -> setPosition(locationInNode);
        chosenP -> setAutoRemoveOnFinish(true);
        this -> addChild(chosenP,333);

        
//        BlendFunc f = {GL_ONE, GL_ZERO };
        BlendFunc f = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA };

        lBrush->setBlendFunc(f);
        _canvas->begin();
        lBrush->visit();
        _canvas->end();
        Director::getInstance() -> getRenderer() -> render();
        
            return true;
    }
    
    
    return true;//????
}

void EatScribbleView::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

}

void EatScribbleView::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
        
    if (_checkEatAgain)
    {
        if(getPecent()<1.f)
        {
            __NotificationCenter::getInstance() -> postNotification("BeginEat");
            _checkEatAgain = false;
        }
    }

    
    if (checkPercent_ && tapCount_%9 == 0)
    {
        
        if (getPecent() <0.15)
        {
            if (delegate_)
            {
                delegate_ -> scribbleCleared();
            }
        }
    }
    tapCount_++;
}
