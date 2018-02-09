//
//  ScribbleSVGUtil.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/11/26.
//
//

#include "ScribbleSVGUtil.h"
#include "CCRenderTexture_Multisampling.h"
#include "TextureLinearFilter.h"

Image *ScribbleSVGUtil::capture(ScribbleSVGLineFrameNode *pNode, uint pSampleNumber/* = 4*/){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    Size lSize = pNode->getContentSize();
    vgResizeSurfaceMNK(lSize.width, lSize.height);
    RenderTexture_Multisampling
    *lRT = RenderTexture_Multisampling::create(lSize.width, lSize.height, pSampleNumber);
    lRT->begin();
    pNode->visit();
    lRT->end();
    lRT->getSprite()->getTexture()->setAntiAliasTexParameters();
    Director::getInstance()->getRenderer()->render();
    
    lSize = Director::getInstance()->getWinSize();
    vgResizeSurfaceMNK(lSize.width, lSize.height);
    Director::getInstance()->setViewport();
    
    return lRT->newImage();
#elif  CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    float lScale = pSampleNumber;
    Size lSize = pNode->getContentSize() * lScale;
    vgResizeSurfaceMNK(lSize.width, lSize.height);
    pNode->setScale(lScale);
    Node *lNode = dynamic_cast<Node *>(pNode);
    RenderTexture *lRT = ScribbleSVGUtil::capture(lNode);
    lSize = Director::getInstance()->getWinSize();
    vgResizeSurfaceMNK(lSize.width, lSize.height);
    Director::getInstance()->setViewport();
    pNode->setScale(1.0);
    lRT = TextureLinearFilter::getInstance()->linearTexture(lRT->getSprite());
    
    Sprite *lTmp = lRT->getSprite();
    lTmp->setScale(1.0 / lScale);
    lTmp->setPosition(lTmp->getContentSize() / lScale / 2);
    lRT = ScribbleSVGUtil::capture(lTmp);
    return lRT->newImage();
#endif
}

RenderTexture *ScribbleSVGUtil::capture(Node *pNode){
    Size lSize = pNode->getContentSize() * pNode->getScale();
    RenderTexture
    *lRT = RenderTexture::create(lSize.width, lSize.height);
    lRT->begin();
    pNode->visit();
    lRT->end();
    lRT->getSprite()->getTexture()->setAntiAliasTexParameters();
    Director::getInstance()->getRenderer()->render();
    return lRT;
}