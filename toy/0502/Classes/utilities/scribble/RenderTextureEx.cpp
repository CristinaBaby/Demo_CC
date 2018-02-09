#include "RenderTextureEx.h"


RenderTextureEx * RenderTextureEx::create(int w, int h, Texture2D::PixelFormat eFormat)
{
    RenderTextureEx *ret = new RenderTextureEx();
    
    if(ret && ret->initWithWidthAndHeight(w, h, eFormat))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

RenderTextureEx * RenderTextureEx::create(int w ,int h, Texture2D::PixelFormat eFormat, GLuint uDepthStencilFormat)
{
    RenderTextureEx *ret = new RenderTextureEx();
    
    if(ret && ret->initWithWidthAndHeight(w, h, eFormat, uDepthStencilFormat))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

RenderTextureEx * RenderTextureEx::create(int w, int h)
{
    RenderTextureEx *ret = new RenderTextureEx();
    
    if(ret && ret->initWithWidthAndHeight(w, h, Texture2D::PixelFormat::RGBA8888, 0))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool RenderTextureEx::saveToFile(const std::string& filename, bool isRGBA)
{
    std::string basename(filename);
    std::transform(basename.begin(), basename.end(), basename.begin(), ::tolower);
    
    if (basename.find(".png") != std::string::npos)
    {
        return saveToFile(filename, Image::Format::PNG, isRGBA);
    }
    else if (basename.find(".jpg") != std::string::npos)
    {
        if (isRGBA) CCLOG("RGBA is not supported for JPG format.");
        return saveToFile(filename, Image::Format::JPG, false);
    }
    else
    {
        CCLOG("Only PNG and JPG format are supported now!");
    }
    
    return saveToFile(filename, Image::Format::JPG, false);
}

bool RenderTextureEx::saveToFile(const std::string& fileName, Image::Format format, bool isRGBA)
{
    CCASSERT(format == Image::Format::JPG || format == Image::Format::PNG,
             "the image can only be saved as JPG or PNG format");
    if (isRGBA && format == Image::Format::JPG) CCLOG("RGBA is not supported for JPG format");
    
    std::string fullpath = FileUtils::getInstance()->getWritablePath() + fileName;
    _saveToFileCommand.init(_globalZOrder);
    _saveToFileCommand.func = CC_CALLBACK_0(RenderTextureEx::onSaveToFile, this, fullpath, isRGBA);
    
    Director::getInstance()->getRenderer()->addCommand(&_saveToFileCommand);
    return true;
}

void RenderTextureEx::onSaveToFile(const std::string& filename, bool isRGBA)
{
    Image *image = newImage(true);
    if (image)
    {
        image->saveToFile(filename.c_str(), !isRGBA);
    }
    
    CC_SAFE_DELETE(image);
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_SAVE_IMAGE);
}