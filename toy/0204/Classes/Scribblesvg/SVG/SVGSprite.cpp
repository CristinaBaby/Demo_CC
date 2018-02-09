//
//  SVGSprite.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/24/15.
//
//

#include "SVGSprite.h"
#include "SVGCache.h"

#if defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#include "glu.h"
#else
#include <OpenGL/glu.h>
#define glOrthof glOrtho
#endif
#elif ANDROID
#include "glu.h"
#elif WIN32
#define WIN32_LEAN_AND_MEAN
#include <GL/glew.h>
#define glOrthof glOrthofOES
//#include <gl\gl.h>		// Header File For The OpenGL32 Library
//#include <gl\glu.h>		// Header File For The GLu32 Library
#endif // #if defined(__APPLE__)

SVGSprite *SVGSprite::create(string pFile, SVGFill::FillState pState){
    SVGSprite *lSprite = new SVGSprite();
    lSprite->init(pFile, pState);
    lSprite->autorelease();
    return lSprite;
}

SVGSprite *SVGSprite::create(SVGSource *pSource){
    SVGSprite *lSprite = new SVGSprite();
    lSprite->init(pSource);
    lSprite->autorelease();
    return lSprite;
}

bool SVGSprite::init(string pFile, SVGFill::FillState pState){
    SVGSource *lSource = SVGCache::getInstance()->addFile(pFile, pState);
    if (nullptr == lSource) {
        return nullptr;
    }
    return this->init(lSource);
}

bool SVGSprite::init(SVGSource *pSource){
    if (!Node::init()) {
        return false;
    }
//    vgCreateContextMNK( 768*2, 1024*2, VG_RENDERING_BACKEND_TYPE_OPENGLES20 );

    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));//SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP
    Rect lRect = pSource->getContentRect();
    Vec2 lAnchorPoint = Vec2(-lRect.origin.x / lRect.size.width,-lRect.origin.y / lRect.size.height);
    this->setAnchorPoint(lAnchorPoint);
    this->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
    this->setContentSize(pSource->getContentSize());
    this->setSource(pSource);
    return true;
}

SVGSprite::SVGSprite(){
    source_ = nullptr;
}

SVGSprite::~SVGSprite(){
    CC_SAFE_RELEASE(source_);
}

#pragma mark - Draw

void SVGSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(SVGSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void SVGSprite::onDraw(const Mat4 &transform, uint32_t flags){
    auto glProgram = getGLProgram();
    glProgram->use();
//    glProgram->setUniformsForBuiltins(transform);
//
    
//    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_COLOR );

//    float position[9]={
//        10, 10, 0.0f,
//        310, 10, 0.0f,
//        160, 300, 0.0f};
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, position);
//    glLineWidth(5);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    
//    return;
    if (nullptr == this->getSource()) {
        return;
    }
    
    bool doBlend = this->getSource()->getHasTransparentColors();
    bool doBlendFunc = (blendFunc_.src != CC_BLEND_SRC || blendFunc_.dst != CC_BLEND_DST);
    
    if (!doBlend) {
        glDisable(GL_BLEND);
    } else if (doBlendFunc) {
//        glBlendFunc(blendFunc_.src, blendFunc_.dst);
        GL::blendFunc(blendFunc_.src, blendFunc_.dst);
    }
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );

    // transform
    AffineTransform lTransform = AffineTransformMakeIdentity();
    
    lTransform = AffineTransformConcat(lTransform, this->getNodeToWorldAffineTransform());
//
    lTransform = AffineTransformScale(lTransform, CC_CONTENT_SCALE_FACTOR(), CC_CONTENT_SCALE_FACTOR());
//
    lTransform = AffineTransformTranslate(lTransform, 0.0f, this->getContentSize().height);
    lTransform = AffineTransformScale(lTransform, 1.0f, -1.0);
    
    // matrix
    VGfloat matrix[9] = {
        lTransform.a, lTransform.c, lTransform.tx,  // a, c, tx
        lTransform.b, lTransform.d, lTransform.ty,  // b, d, ty
        0, 0, 1,                                    // 0, 0, 1
    };
    vgLoadMatrix(matrix);
    this->getSource()->draw();

//    glLoadIdentity();
//    this->transformAncestors();
    // enable blending
    if (!doBlend) {
        glEnable(GL_BLEND);
    } else if (doBlendFunc) {
        glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    }
}