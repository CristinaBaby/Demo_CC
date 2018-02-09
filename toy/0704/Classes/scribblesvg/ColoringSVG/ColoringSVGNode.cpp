//
//  ColoringSVGNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/10/10.
//
//

#include "ColoringSVGNode.h"
#include "ScribbleUtil.h"
#include "SVGPattern.h"
#define kShaderName "ColoringSVG.frag"
//#define kSVGPatternShaderName "SVGPattern.frag"
#define STRINGIFY(A)  #A
#include kShaderName
//#include kSVGPatternShaderName

#pragma mark - class ColoringSVGNode
ColoringSVGNode *ColoringSVGNode::create(string pLineArtFile){
    ColoringSVGNode *ret = new ColoringSVGNode();
    if(ret && ret->init(pLineArtFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ColoringSVGNode::ColoringSVGNode(){
    _debugPattern = false;
    _lineFrameNode = nullptr;
}

ColoringSVGNode::~ColoringSVGNode(){
    
}

bool ColoringSVGNode::init(string pLineArtFile){
    if (!Node::init()) {
        return false;
    }
    //renhong mark: 请尝试优化void SVGSource::optimize()，进一步加快加载SVG速度。
    SVGSprite *lLineArt = SVGSprite::create(pLineArtFile, SVGFill::FillState::eAuto);
    
    Size lSize = lLineArt->getContentSize();
    this->setContentSize(lSize);
    //设置SVG画布大小为Scribble画布大小，以免SVG离线绘制变形
    vgResizeSurfaceMNK(lSize.width, lSize.height);
    RenderTexture *lRT = RenderTexture::create(lSize.width, lSize.height);
    lRT->getSprite()->getTexture()->setAliasTexParameters();
//    lRT->clear(1.0, 0.0, 0.0, 1.0);
    lRT->begin();
    lLineArt->visit();
    lRT->end();
    Director::getInstance()->getRenderer()->render();
    
    //创建Pattern像素图
    Image *lPatternImage = lRT->newImage();
    CC_SAFE_RELEASE(_templateImage);
    _templateImage = lPatternImage;
    
    //创建Pattern精灵及纹理
    Sprite *lTemplate = Sprite::createWithTexture(lRT->getSprite()->getTexture());//lRT->getSprite();
    this->setTemplate(lTemplate);
    Size drawSize = lTemplate->getContentSize();
    lTemplate->setPosition(Vec2(drawSize.width/2, drawSize.height/2));
    
    lSize = Director::getInstance()->getWinSize();
    //重置SVG画布大小
    vgResizeSurfaceMNK(lSize.width, lSize.height);
    //重置cocos2d视口大小
    Director::getInstance()->setViewport();
    
    //构建并初始化画布
    this->initCanvas(drawSize);
    this->initRenderPara();
    
    return true;
}

void ColoringSVGNode::setBrushShader(){
    string lGLProgramKey = ScribbleUtil::getInstance()->createUniqueGLProgramName(kShaderName, this);
    GLProgram *lProgram = ScribbleUtil::getInstance()->loadGLProgram(ccPositionTextureColor_noMVP_vert,ColoringSVG_frag, lGLProgramKey);
    if (NULL != lProgram) {
        if (this->getTemplate()->getGLProgram() != lProgram) {
            ScribbleUtil::getInstance()->setGLProgram(this->getTemplate(), lProgram);
        }
    }
}

void ColoringSVGNode::initRenderPara(){
    //构造着色参数
    setBrushShader();
}

void ColoringSVGNode::initPattern(string pPatternImage){
    //overwrite父类方法，以此避免在init方法中被调用执行ColoringNode::initPattern
}

bool ColoringSVGNode::coloring(const Point &pWorldPosition){
    if (nullptr == _templateImage) {
        return false;
    }
    Point pos = this->convertToNodeSpace(pWorldPosition);
//    if (!this->getBoundingBox().containsPoint(pos)) {
//        return false;
//    }
    Color4B lTouchedColor = ScribbleUtil::getInstance()->getColor(_templateImage, pos);
    if (_prePatternColor == lTouchedColor) {
        return false;
    }
    _prePatternColor = lTouchedColor;
    
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getTemplate()->getGLProgram());
    Color4F lTouchedColor4F(lTouchedColor);
    lState->setUniformVec4("v_touched_color", Vec4(lTouchedColor4F.r, lTouchedColor4F.g, lTouchedColor4F.b, lTouchedColor4F.a));
    //        _canvas->clear(1, 1, 1, 1);
    _canvas->begin();
    Texture2D::TexParams tp;
    tp.magFilter = GL_NEAREST;
    tp.minFilter = GL_NEAREST;
    tp.wrapS = GL_CLAMP_TO_EDGE;
    tp.wrapT = GL_CLAMP_TO_EDGE;
    this->getTemplate()->getTexture()->setTexParameters(tp);
    this->getTemplate()->visit();
    _canvas->end();
    Director::getInstance()->getRenderer()->render();
    return true;
}

bool ColoringSVGNode::coloring(const Point &pStartWorldPosition, const Point &pEndWorldPosition, float pInterpolateDelta/* = 30.0f*/){
    vector<Point> lPositions = ScribbleUtil::getInstance()->interpolate(pStartWorldPosition, pEndWorldPosition, pInterpolateDelta);
    for (int i = 0; i < lPositions.size(); ++i) {
        Point lPoint = lPositions.at(i);
        this->coloring(lPoint);
    }
    return true;
}

void ColoringSVGNode::updatePattern(){
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getTemplate()->getGLProgram());
    lState->setUniformTexture("s_texture_shape", this->getTemplate()->getTexture());
    lState->setUniformVec2("v_texture_shape_size", this->getTemplate()->getTexture()->getContentSize());
    
    CC_SAFE_RELEASE(_shapeBoundTexture);
    _shapeBoundTexture = this->getShapeBounds(_templateImage);
    _shapeBoundTexture->retain();
    lState->setUniformTexture("s_texture_shape_bounds", _shapeBoundTexture);
}

bool ColoringSVGNode::setPattern(string pSVGFile, SVGFill::FillState pState/* = SVGFill::FillState::eAuto*/){
    /*优化Pattern图片_templete的锯齿，以便线框图在多重采样抗锯齿之后，完全遮住Pattern图*/
    //renhong mark: 请进一步优化SVG绘制，以在GPU完成第一次绘制即完成本工作。
    Sprite *lTemplate = Sprite::createWithTexture(this->getTemplate()->getTexture());
    RenderTexture *lRT = SVGPattern::getInstance()->getPattern(lTemplate);
    this->getTemplate()->setTexture(lRT->getSprite()->getTexture());
 
    //debug
    if(_debugPattern){
        string lPath = "coloring_svg_pattern_debug.png";
        if(lRT->saveToFile(lPath)){
            log("save pattern at %s", (FileUtils::getInstance()->getWritablePath() + "/" + lPath).c_str());
        }
        Director::getInstance()->getRenderer()->render();
    }
    return true;
}

void ColoringSVGNode::debugPattern(bool pDebug/* = true*/){
    if (!pDebug) {
        return;
    }
    _debugPattern = true;
    if(nullptr != _lineArt){
        _lineArt->setVisible(false);
    }
    if(nullptr != this->getTemplate()){
        this->getTemplate()->setVisible(true);
    }
    if(nullptr != _canvas){
        _canvas->clear(0.0, 0.0, 0.0, 0.0);
    }
}

void ColoringSVGNode::setLineFrame(Node *pLineFrame, Vec2 pPosition/* = Vec2::ZERO*/){
    if (nullptr != _lineFrameNode) {
        _lineFrameNode->removeFromParent();
    }
    _lineFrameNode = pLineFrame;
    if (nullptr != pLineFrame) {
        this->addChild(pLineFrame, 10);
        pLineFrame->setPosition(pPosition);
        pLineFrame->ignoreAnchorPointForPosition(true);
    }
}

#pragma mark - class ColoringSVGTouchNode
float ColoringSVGTouchNode::interpolateDelta = 30.0f;

ColoringSVGTouchNode *ColoringSVGTouchNode::create(string pLineArtFile){
    ColoringSVGTouchNode *ret = new ColoringSVGTouchNode();
    if(ret && ret->init(pLineArtFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ColoringSVGTouchNode::init(string pLineArtFile){
    if (!ColoringSVGNode::init(pLineArtFile)) {
        return false;
    }
    //响应点击事件：根据点击位置，够造相应着色参数给着色器。
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->bool{
        _prePoint = pTouch->getLocation();
        this->coloring(pTouch->getLocation());
        return true;
    };
    listener->onTouchMoved = [this](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{
        this->coloring(pTouch->getLocation(), _prePoint, ColoringSVGTouchNode::interpolateDelta);
        _prePoint = pTouch->getLocation();
    };
    listener->onTouchEnded = [this](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{
        this->resetPatternColor();
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}