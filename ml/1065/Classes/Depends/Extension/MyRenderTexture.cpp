
#include "MyRenderTexture.h"

MyRenderTexture * MyRenderTexture::create(int w, int h)
{
    MyRenderTexture *pRet = new MyRenderTexture();
    if(pRet && pRet->initWithWidthAndHeight(w, h, cocos2d::Texture2D::PixelFormat::RGBA8888, 0))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

float MyRenderTexture::getPercentageTransparent()
{
    CCAssert(_pixelFormat == cocos2d::Texture2D::PixelFormat::RGBA8888, "only RGBA8888 can be saved as image");
    Size size = _texture->getContentSizeInPixels();
    int tx = size.width;
    int ty = size.height;
    int bitsPerPixel = 4*8;
    int bytesPerPixel = bitsPerPixel/8;
    int bytesPerRow = bytesPerPixel*tx;
    int myDataLength = 4*tx*ty;
    int numbetOfPiexelss = tx*ty;
    float numberOfTransparent = 0;
    
    GLubyte *buffer	= (GLubyte*)malloc(sizeof(GLubyte)*myDataLength);
    if (!(buffer)) {
        free(buffer);
        return -1.0f;
    }
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_oldFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
    
    if (Configuration::getInstance()->checkForGLExtension("GL_QCOM"))
    {
        // -- bind a temporary texture so we can clear the render buffer without losing our texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureCopy->getName(), 0);
        CHECK_GL_ERROR_DEBUG();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->getName(), 0);
    }
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, tx, ty, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _oldFBO);

//    this->begin();
//    glReadPixels(0, 0, tx, ty, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
//    this->end();
    
    for (int y =0; y<ty;y++) {
        for (int x = 0; x<tx; x++) {
            GLubyte alpha = buffer[(y * 4 * tx + ((x * 4)+3))];
//            log("===%d",alpha);
            if(alpha == 0) {
                numberOfTransparent++;
            }
        }
    }
    free(buffer);
    return numberOfTransparent/numbetOfPiexelss*100;
}


float MyRenderTexture::getAlphaWithLocation(Point point)
{
    CCAssert(_pixelFormat==cocos2d::Texture2D::PixelFormat::RGBA8888, "only RGBA8888 can be saved as image");
    Size size = _texture->getContentSizeInPixels();
    int tx = size.width;
    int ty = size.height;
    
    //图片高度为tx，ty，但是在数组中从0开始，因此上限为tx-1,ty-1;
    if (point.x < 0 || point.x > tx - 1 || point.y < 0 || point.y >ty - 1) {
        return 0;
    }
	
    int bitsPerPixel = 4*8;
    int bytesPerPixel = bitsPerPixel/8;
    int bytesPerRow = bytesPerPixel*tx;
    int myDataLength = bytesPerRow*ty;
    
	GLubyte *buffer	= (GLubyte*)malloc(sizeof(GLubyte)*myDataLength);
    if (!(buffer)) {
        free(buffer);
        return -1.0f;
    }
    
    this->begin();
    glReadPixels(0, 0, tx, ty, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    this->end();
    int x = point.x;
    int y = point.y;
    float alpha = buffer[(y * 4 * tx + ((x * 4)+3))];
    free(buffer);
    return alpha;
}