//
//  ColoringSVGNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/10/10.
//
//

#ifndef __ScribbleDemo_3_2__ColoringSVGNode__
#define __ScribbleDemo_3_2__ColoringSVGNode__

#include <stdio.h>
#include "ColoringNode.h"
#include "SVGSprite.h"
#include "SVGFill.h"

class ColoringSVGNode : public ColoringNode {
public:
    static ColoringSVGNode *create(string pLineArtFile);
    
    ColoringSVGNode();
    ~ColoringSVGNode();
    bool init(string pLineArtFile);
    virtual void setBrushShader();
    void initRenderPara();
    virtual void initPattern(string pPatternImage);
    inline bool coloring(const Point &pWorldPosition);
    bool coloring(const Point &pStartWorldPosition, const Point &pEndWorldPosition, float pInterpolateDelta = 30.0f);
    void updatePattern();
    bool setPattern(string pSVGFile, SVGFill::FillState pState = SVGFill::FillState::eAuto);
    Sprite *getPattern(){return nullptr;}
    inline void setCanvas(RenderTexture *pCanvas){this->_canvas = pCanvas;}
    inline void setPatternImage(Image *pPatternImage){
        if (pPatternImage == this->_templateImage) {
            return;
        }
        CC_SAFE_RELEASE(this->_templateImage);
        this->_templateImage = pPatternImage;
        this->_templateImage->retain();
    }
    inline void setPattern(Sprite *pPattern){
        if (nullptr == this->getTemplate()) {
            Sprite *lTemplate = Sprite::createWithTexture(pPattern->getTexture());
            this->setTemplate(lTemplate);
        }
        else{
            this->getTemplate()->setTexture(pPattern->getTexture());
        }
        this->getTemplate()->setPosition(this->getTemplate()->getContentSize() / 2);
    }
    
    void debugPattern(bool pDebug = true);
    void setLineFrame(Node *pLineFrame, Vec2 pPosition = Vec2::ZERO);
public:
    virtual inline void setColoringMode(ColoringMode pMode){
        ColoringNode::setColoringMode(pMode);
        GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getTemplate()->getGLProgram());
        lState->setUniformInt("i_color_model", static_cast<int>(_coloringMode));
        
    }
    virtual inline void setStartColor(const Color4B &pColor){
        ColoringNode::setStartColor(pColor);
        Color4F lStartColor4F(_startColor);
        GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getTemplate()->getGLProgram());
        lState->setUniformVec4("v_start_color", Vec4(lStartColor4F.r, lStartColor4F.g, lStartColor4F.b, lStartColor4F.a));
    }
    virtual inline void setEndColor(const Color4B &pColor){
        ColoringNode::setEndColor(pColor);
        Color4F lEndColor4F(_endColor);
        GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getTemplate()->getGLProgram());
        lState->setUniformVec4("v_end_color", Vec4(lEndColor4F.r, lEndColor4F.g, lEndColor4F.b, lEndColor4F.a));
    }
private:
    bool _debugPattern;
    Node *_lineFrameNode;
//    Sprite *_tem2plate2;
};

class ColoringSVGTouchNode : public ColoringSVGNode{
public:
    static ColoringSVGTouchNode *create(string pLineArtFile);
    bool init(string pLineArtFile);
    
public:
    static float interpolateDelta/* = 30.0f*/;//touch move 速度过快，有效touch点个数极少，采用插值算法平滑插入touch点，插值Deleta值为interpolateDelta， 可根据项目需要调整。
protected:
    Point _prePoint;
};
#endif /* defined(__ScribbleDemo_3_2__ColoringSVGNode__) */
