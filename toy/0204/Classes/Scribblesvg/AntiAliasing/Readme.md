**当前版本：v1.0.0**

**作者:    		任鸿**

**最后维护更新:  	任鸿**

#简介：

anti_aliasing主要功能是对“矢量图/opengl画线”截图抗锯齿.
其中，CCRenderTexture_Multisampling类使用MSAA FBO实现截图抗锯齿；TextureLinearFilter使用Shader模拟SSAA抗锯齿过程。


#依赖外链：

无


**简单的使用语句如下：**

```

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
    lRT = TextureLinearFilter::getPattern(lRT->getSprite());
    
    Sprite *lTmp = lRT->getSprite();
    lTmp->setScale(1.0 / lScale);
    lTmp->setPosition(lTmp->getContentSize() / lScale / 2);
    lRT = ScribbleSVGUtil::capture(lTmp);
    return lRT->newImage();
#endif
}
		
```

**更多使用方式和效果详情参见代码库Demo！**

CodeBaseDemo：  [ssh://git@stash.stm.com:7999/cmr/scribbledemo.git](ssh://git@stash.stm.com:7999/cmr/scribbledemo.git)