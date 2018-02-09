#include "KSVisibleRect.h"

USING_NS_CC;

static Size s_FrameSize;
static Size s_DesignSize;
static Size s_RealDesignSize;
//static CCPoint s_TopLeftOffset;
static Point s_DesignOffset;
static Size m_VisibleSize;
static Point m_VisibleOrigin;
static float s_RealDesignScale = 1.0f;

static int s_RetainOffsetLeft = 0;
static int s_RetainOffsetTop = 0;
static int s_RetainOffsetRight = 0;
static int s_RetainOffsetBottom = 0;

bool KSVisibleRect::isPortrait(const Size& frameSize)
{
    return frameSize.width < frameSize.height;
}

void KSVisibleRect::setupVisibleRect(const Size& frameSize,const Size& designSize)
{
    s_DesignSize = designSize;
	s_FrameSize = frameSize;
    float scaleX = frameSize.width/designSize.width;
    float scaleY = frameSize.height/designSize.height;
    float scale = MIN(scaleX, scaleY);
    s_RealDesignSize = Size(frameSize.width/scale,frameSize.height/scale);
//    if (KSVisibleRect::isPortrait(frameSize))
//    {
//        KSVisibleRect::setPortraitSize(frameSize, designSize);
//    }else
//    {
//        KSVisibleRect::setLandSpaceSize(frameSize, designSize);
//    }
    log("scale%f", scale);
    log("s_RealDesignSize.w %f , s_RealDesignSize.h %f",s_RealDesignSize.width,s_RealDesignSize.height);
    KSVisibleRect::calculateOffset();
}

void KSVisibleRect::setPortraitSize(const Size& frameSize , Size designSize)
{
//    switch (CCApplication::sharedApplication()->getTargetPlatform())
//	{
//        case kTargetIphone:
//		{
//            CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
//            if(screenSize.width == 640 || screenSize.height == 1136)
//            {
//                CCLOG("This is a iPhone5.");
//                Util::_KSDesignResolution = KS_DESIGN_RESOLUTION_1136X640;
//                s_RealDesignSize = CCSize(frameSize.width,frameSize.height);
//            }else if(screenSize.width == 640 && screenSize.height == 960)
//            {
//                CCLOG("This is a iPhone.");
//                Util::_KSDesignResolution = KS_DESIGN_RESOLUTION_960X640;
//                s_RealDesignSize = CCSize(frameSize.width,frameSize.height);
//            }else
//            {
//                CCLOG("This is a other iPhone.");
//                Util::_KSDesignResolution = KS_DESIGN_RESOLUTION_1024X768;
//                KSVisibleRect::setupDesignRect(frameSize, designSize);
//            }
//		}
//            break;
//        case kTargetIpad:
//		{
//			CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
//			if(screenSize.width == 1536|| screenSize.height == 2048)
//			{
//				CCLOG("This is a retina iPad.");
//				Util::_KSDesignResolution = KS_DESIGN_RESOLUTION_1024X768;
//                s_RealDesignSize = CCSize(frameSize.width/2,frameSize.height/2);
//                s_RealDesignScale = 0.5f;
//			}
//			else
//			{
//				CCLOG("This is a normal iPad.");
//				Util::_KSDesignResolution = KS_DESIGN_RESOLUTION_1024X768;
//                s_RealDesignSize = CCSize(frameSize.width,frameSize.height);
//			}
//		}
//            break;
//        case kTargetAndroid:
//            CCLOG("This is an android device.");
//            Util::_KSDesignResolution = KS_DESIGN_RESOLUTION_960X640;
//            KSVisibleRect::setupDesignRect(frameSize, designSize);
//            break;
//        default:
//            Util::_KSDesignResolution = KS_DESIGN_RESOLUTION_960X640;
//            KSVisibleRect::setupDesignRect(frameSize, designSize);
//            break;
//	}
}

void KSVisibleRect::setLandSpaceSize(const Size& frameSize , Size designSize)
{
}

void KSVisibleRect::setupDesignRect(const Size& frameSize,const Size& designSize)
{
	float scaleX = (float) frameSize.width / designSize.width;
	float scaleY = (float) frameSize.height / designSize.height;
    
	float scale = 0.0f;
	if (scaleX > scaleY)
	{
		scale = scaleX / (frameSize.height / designSize.height);
	}
	else
	{
		scale = scaleY / (frameSize.width / designSize.width);
	}
    
	s_RealDesignScale = scale;
	s_RealDesignSize = Size(designSize.width * scale, designSize.height * scale);
    

}

void KSVisibleRect::calculateOffset()
{
    
    m_VisibleSize = Director::getInstance()->getVisibleSize();
    
//    s_TopLeftOffset = ccp((m_VisibleSize.width - s_DesignSize.width)/2.0f,
//                          (m_VisibleSize.height - s_DesignSize.height)/2.0f);
    
    s_DesignOffset = Vec2((s_RealDesignSize.width - s_DesignSize.width) / 2.0f,
                         (s_RealDesignSize.height - s_DesignSize.height) / 2.0f);
    
    log("s_DesignOffset.x %f,s_DesignOffset.y %f",s_DesignOffset.x,s_DesignOffset.y);
}

void KSVisibleRect::initRetainOffset(int left, int top, int right, int bottom)
{
	s_RetainOffsetLeft = left * s_RealDesignSize.width / s_FrameSize.width;
	s_RetainOffsetTop = top * s_RealDesignSize.height / s_FrameSize.height;
	s_RetainOffsetRight = right * s_RealDesignSize.width / s_FrameSize.width;
	s_RetainOffsetBottom = bottom * s_RealDesignSize.height / s_FrameSize.height;
}

Point KSVisibleRect::getPosition(float x, float y, BorderType xBorder,BorderType yBorder) {
    m_VisibleSize = Director::getInstance()->getVisibleSize();
    
    float posX = 0;
	float posY = 0;
    
    if (xBorder & kBorderLeft)
    {
        posX = m_VisibleOrigin.x + x;
    }else if(xBorder & kBorderRight)
    {
        posX = m_VisibleOrigin.x + m_VisibleSize.width - x;
    }else if(xBorder & kNoOffsetX)
    {
        posX = x ;
    }else
    {
        posX = x + s_DesignOffset.x;
    }
    
    if (yBorder & kBorderTop)
    {
        posY = m_VisibleOrigin.y + m_VisibleSize.height - y;
    }else if(yBorder & kBorderBottom)
    {
        posY = m_VisibleOrigin.y + y;
    }else if(yBorder & kNoOffsetY)
    {
        posY = y;
    }else
    {
        posY = y + s_DesignOffset.y;
    }
    return Vec2(posX, posY);
}

void KSVisibleRect::setPosition(Node* node, float x, float y,
                                BorderType xBorder,BorderType yBorder)
{
    m_VisibleSize = Director::getInstance()->getVisibleSize();
    
    float posX = 0;
	float posY = 0;
    
    if (xBorder & kBorderLeft)
    {
        posX = m_VisibleOrigin.x + x;
    }else if(xBorder & kBorderRight)
    {
        posX = m_VisibleOrigin.x + m_VisibleSize.width - x;
    }else if(xBorder & kNoOffsetX)
    {
        posX = x ;
    }else
    {
        posX = x + s_DesignOffset.x;
    }
    
    if (yBorder & kBorderTop)
    {
        posY = m_VisibleOrigin.y + m_VisibleSize.height - y;
    }else if(yBorder & kBorderBottom)
    {
        posY = m_VisibleOrigin.y + y;
    }else if(yBorder & kNoOffsetY)
    {
        posY = y;
    }else
    {
        posY = y + s_DesignOffset.y;
    }
    
	node->setPosition(Vec2(posX, posY));
}

void KSVisibleRect::setPositionWithRetainOffset(Node* node, float x, float y, bool bTop, bool bBottom)
{
	float posX = x + s_DesignOffset.x;
	float posY = y + s_DesignOffset.y;

//	if(bTop)
//	{
//		posY += (s_TopLeftOffset.y - s_RetainOffsetTop);
//	}
//
//	if(bBottom)
//	{
//		posY -= (s_TopLeftOffset.y - s_RetainOffsetBottom);
//	}

	node->setPosition(Vec2(posX, posY));
}

Size& KSVisibleRect::getRealDesignSize()
{
	return s_RealDesignSize;
}

Size& KSVisibleRect::getDesignSize()
{
	return s_DesignSize;
}

Size& KSVisibleRect::getFrameSize()
{
	return s_FrameSize;
}

float KSVisibleRect::getRealDesignScale()
{
	return s_RealDesignScale;
}

//CCPoint& KSVisibleRect::getTopLeftOffset()
//{
//	return s_TopLeftOffset;
//}

Point& KSVisibleRect::getDesignOffset()
{
	return s_DesignOffset;
}