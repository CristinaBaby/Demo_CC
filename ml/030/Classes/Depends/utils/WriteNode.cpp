//
//  WriteNode.cpp
//  DrawLayer
//
//  Created by ren hong on 14-2-20.
//
//

#include "WriteNode.h"

enum {
    eCanvasZ = -10,
    eCanvasPreviewBackZ,
    eCanvasPreviewMiddleZ,
    eCanvasPreviewFrontZ
};

WriteNode *WriteNode::create(Size pCanvasSize){
    WriteNode *pRet = new WriteNode();
    if (pRet && pRet->init(pCanvasSize)){
        pRet->autorelease();
        return pRet;
    }
    else{
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool WriteNode::init(Size pCanvasSize){
    if (!CCNode::init()) {
        return false;
    }
    _size = pCanvasSize;
    this->addCanvas();
    return true;
}

WriteNode::WriteNode() {
    _penType = WriteNode::PenType::eUnknow;
    _brushFront = NULL;
    _brushMiddle = NULL;
    _brushBack = NULL;
    _earser = NULL;
    _canvasPreviewFront = NULL;
    _canvasPreviewMiddle = NULL;
    _canvasPreviewBack = NULL;
    _canvas = NULL;
    _drawing = false;
    
    _penColor = Color4B(0, 0, 255, 255);
    _outlineColor = Color4B(0, 0, 255, 255);
    _strokeColor = Color4B(0,0,255,5);
}

WriteNode::~WriteNode(){
    if (NULL != _brushFront) {
        _brushFront->release();
        _brushFront = NULL;
    }
    
    if (NULL != _brushMiddle) {
        _brushMiddle->release();
        _brushMiddle = NULL;
    }
    
    if (NULL != _brushBack) {
        _brushBack->release();
        _brushBack = NULL;
    }
    
    if (NULL != _earser) {
        _earser->release();
        _earser = NULL;
    }
    
    if (NULL != _magic) {
        _magic->release();
        _magic = NULL;
    }
    
}

void WriteNode::touchBegan(Point pPoint)

{
//    this->setDrawing(true);
    if (!this->getDrawing()) {
        return;
    }
    
    Point lStartPoint = this->convertToNodeSpace(pPoint);
    if (_brushBack) {
        _brushBack->setColor(Color3B(_strokeColor));
        _brushBack->setOpacity(_strokeColor.a);
        this->draw(_brushBack, _canvasPreviewBack, lStartPoint);
    }
    
    if (_brushMiddle) {
        _brushMiddle->setColor(Color3B(_outlineColor));
        _brushMiddle->setOpacity(_outlineColor.a);
        this->draw(_brushMiddle, _canvasPreviewMiddle, lStartPoint);
    }
    
    if (_brushFront) {
        _brushFront->setColor(Color3B(_penColor));
        _brushFront->setOpacity(_penColor.a);
        this->draw(_brushFront, _canvasPreviewFront, lStartPoint);
    }
    
    if (_earser) {
        this->draw(_earser, _canvas, lStartPoint);
    }
}

void WriteNode::touchMoved(Point pStartPoint, Point pEndPoint)
{
    if (this->getDrawing())
    {
        
        Point lStartPoint = this->convertToNodeSpace(pStartPoint);
        Point lEndPoint = this->convertToNodeSpace(pEndPoint);
        float lDistance = lStartPoint.getDistance(lEndPoint);
        
        if (_penType == WriteNode::PenType::eMagicPen && _magic)
        {
            static float totalDistance = 0;
            static Point startPoint = Point::ZERO;
            
            if (startPoint == Point::ZERO)
            {
                startPoint = pStartPoint;
            }
            
            totalDistance += lDistance;
            if (totalDistance < 50)
            {
                return;
            }
            
            Vec2 midPoint = (startPoint + pEndPoint)/2;
            startPoint = Point::ZERO;
            float scale = CCRANDOM_0_1()+0.2;
            _magic->setScale(scale);
            _magic->setPosition(midPoint);
            _canvasPreviewMiddle->begin();
            _magic->visit();
            _canvasPreviewMiddle->end();
            
            totalDistance = 0;
            
            return;
        }
        
        if (lDistance > 1)
        {
            if (_brushBack)
            {
                this->draw(_brushBack, _canvasPreviewBack, lStartPoint, lEndPoint);
            }

            if (_brushMiddle)
            {
                this->draw(_brushMiddle, _canvasPreviewMiddle, lStartPoint, lEndPoint);
            }

            if (_brushFront)
            {
                this->draw(_brushFront, _canvasPreviewFront, lStartPoint, lEndPoint);
            }

            if (_earser)
            {
                this->draw(_earser, _canvas, lStartPoint, lEndPoint);
            }
        }
    }
}

void WriteNode::touchEnded(Point pPoint){
    if (this->getDrawing())
    {
//        this->setDrawing(false);
        Size lSize = _size;
        _canvas->begin();
        Point lPoint(lSize.width / 2, lSize.height / 2);
        _canvasPreviewBack->getSprite()->setPosition(lPoint);
        _canvasPreviewMiddle->getSprite()->setPosition(lPoint);
        _canvasPreviewFront->getSprite()->setPosition(lPoint);
        _canvasPreviewBack->getSprite()->visit();
        _canvasPreviewMiddle->getSprite()->visit();
        _canvasPreviewFront->getSprite()->visit();
        _canvas->end();
        CCDirector::getInstance()->getRenderer()->render();
        
        _canvasPreviewBack->getSprite()->setPosition(Point(0,0));
        _canvasPreviewMiddle->getSprite()->setPosition(Point(0,0));
        _canvasPreviewFront->getSprite()->setPosition(Point(0,0));
        
        _canvasPreviewFront->clear(0, 0, 0, 0);
        _canvasPreviewMiddle->clear(0, 0, 0, 0);
        _canvasPreviewBack->clear(0, 0, 0, 0);
    }
    
}

void WriteNode::addCanvas(){
    Size lSize = _size;
    _canvas = RenderTexture::create(lSize.width, lSize.height,Texture2D::PixelFormat::RGBA8888);
    this->addChild(_canvas, eCanvasZ);
    _canvas->setPosition(Point(lSize.width / 2, lSize.height / 2));
    NotificationCenter::getInstance()->removeObserver(_canvas, EVENT_COME_TO_BACKGROUND);
    NotificationCenter::getInstance()->removeObserver(_canvas, EVENT_COME_TO_FOREGROUND);
    
    _canvasPreviewBack = RenderTexture::create(lSize.width, lSize.height, Texture2D::PixelFormat::RGBA8888);
    this->addChild(_canvasPreviewBack, eCanvasPreviewBackZ);
    _canvasPreviewBack->setPosition(Point(lSize.width / 2, lSize.height / 2));
    NotificationCenter::getInstance()->removeObserver(_canvasPreviewBack, EVENT_COME_TO_BACKGROUND);
    NotificationCenter::getInstance()->removeObserver(_canvasPreviewBack, EVENT_COME_TO_FOREGROUND);
    
    _canvasPreviewMiddle = RenderTexture::create(lSize.width, lSize.height, Texture2D::PixelFormat::RGBA8888);
    this->addChild(_canvasPreviewMiddle, eCanvasPreviewMiddleZ);
    _canvasPreviewMiddle->setPosition(Point(lSize.width / 2, lSize.height / 2));
    NotificationCenter::getInstance()->removeObserver(_canvasPreviewMiddle, EVENT_COME_TO_BACKGROUND);
    NotificationCenter::getInstance()->removeObserver(_canvasPreviewMiddle, EVENT_COME_TO_FOREGROUND);
    
    _canvasPreviewFront = RenderTexture::create(lSize.width, lSize.height, Texture2D::PixelFormat::RGBA8888);
    this->addChild(_canvasPreviewFront, eCanvasPreviewFrontZ);
    _canvasPreviewFront->setPosition(Point(lSize.width / 2, lSize.height / 2));
    NotificationCenter::getInstance()->removeObserver(_canvasPreviewFront, EVENT_COME_TO_BACKGROUND);
    NotificationCenter::getInstance()->removeObserver(_canvasPreviewFront, EVENT_COME_TO_FOREGROUND);
}

void WriteNode::setPenType(PenType pType){
    if (pType == _penType) {
        return;
    }
    _penType = pType;
    if (NULL != _brushFront) {
        _brushFront->release();
        _brushFront = NULL;
    }
    
    if (NULL != _brushMiddle) {
        _brushMiddle->release();
        _brushMiddle = NULL;
    }
    
    if (NULL != _brushBack) {
        _brushBack->release();
        _brushBack = NULL;
    }
    
    if (NULL != _earser) {
        _earser->release();
        _earser = NULL;
    }
    
    //创建新的画笔和画刷
    switch (_penType) {
            
        case WriteNode::PenType::eMagicPen:{
            _brushFront = CCSprite::create("pen/pen_brush_1.png");
            _brushFront->retain();
            break;
        }
        case WriteNode::PenType::ePencil:{
            _brushFront = CCSprite::create("pen/pen_brush_1.png");
            _brushFront->retain();
            break;
        }
        case WriteNode::PenType::eHighlighter:{
            _brushFront = CCSprite::create("pen/pen_brush_1.png");
            _brushFront->retain();
            
            _brushMiddle = CCSprite::create("pen/pen_brush_4.png");
            _brushMiddle->retain();
            
            _brushBack = CCSprite::create("pen/pen_brush_3.png");
            _brushBack->retain();
            break;
        }
        case WriteNode::PenType::eTwoColor:{
            _brushFront = CCSprite::create("pen/pen_brush_1.png");
            _brushFront->retain();
            
            _brushBack = CCSprite::create("pen/pen_brush_4.png");
            _brushBack->retain();
            break;
        }
        case WriteNode::PenType::eChalk:{
            _brushFront = CCSprite::create("pen/chalk.png");
            _brushFront->retain();
            break;
        }
        case WriteNode::PenType::eColorPen:{
            _brushFront = CCSprite::create("pen/pen_brush_1.png");
            _brushFront->retain();
            break;
        }
        case WriteNode::PenType::eEraser:{
            _earser = CCSprite::create("pen/earser_brush.png");
            _earser->retain();
            BlendFunc f = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA };
            _earser->setBlendFunc(f);
            break;
        }
        default:
            break;
    }
}

void WriteNode::draw(Sprite *pBrush, RenderTexture *pCanvas, vector<Point> pPositions){
    for (int i = 0; i < pPositions.size(); ++i) {
        pCanvas->begin();
        pBrush->setPosition(pPositions.at(i));
        pBrush->visit();
        pCanvas->end();
        CCDirector::getInstance()->getRenderer()->render();
    }
}

void WriteNode::draw(Sprite *pBrush, RenderTexture *pCanvas, Point pStartPosition, Point pEndPosition){
    float lDistance = pStartPosition.getDistance(pEndPosition);
    int lDis = (int)lDistance;
    int lDelta = (WriteNode::PenType::eChalk == _penType) ? 5.0f * _brushFront->getScale() : 1;
    if (WriteNode::PenType::eColorPen == _penType) {
        pBrush->setColor(Color3B(arc4random() % 255, arc4random() % 255, arc4random() % 255));
    }
    
    for (int i = 0; i < lDis; i += lDelta) {
        pCanvas->begin();
        float lDifX = pEndPosition.x - pStartPosition.x;
        float lDifY = pEndPosition.y - pStartPosition.y;
        float lDelta = (float)i / lDistance;
        pBrush->setPosition(Point(pStartPosition.x + (lDifX * lDelta), pStartPosition.y + (lDifY * lDelta)));
        pBrush->visit();
        pCanvas->end();
        CCDirector::getInstance()->getRenderer()->render();
    }
}

void WriteNode::draw(Sprite *pBrush, RenderTexture *pCanvas, Point pPosition){
    //CCLOG("pPosition %f %f", pPosition.x, pPosition.y);
    pCanvas->begin();
    pBrush->setPosition(pPosition);
    if (WriteNode::PenType::eColorPen == _penType) {
        pBrush->setColor(Color3B(arc4random() % 255, arc4random() % 255, arc4random() % 255));
    }
    pBrush->visit();
    pCanvas->end();
    CCDirector::getInstance()->getRenderer()->render();
}

void WriteNode::reset(){
    _canvas->clear(0, 0, 0, 0);
}

void WriteNode::setPenScale(float s)
{
    if(_brushFront)
    {
        _brushFront->setScale(s);
        
    }
    
    if (_brushBack)
    {
        _brushBack->setScale(s);
    }
    
    if (_brushMiddle)
    {
        _brushMiddle->setScale(s);
    }
}

 void WriteNode::setPenSprite(const string& path)
{
    _brushFront->setTexture(path);
}