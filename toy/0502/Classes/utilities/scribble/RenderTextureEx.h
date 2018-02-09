#pragma once

#include <stdio.h>
#include <cocos2d.h>

using namespace cocos2d;

#define EVENT_SAVE_IMAGE            ("event_save_image")

class RenderTextureEx : public RenderTexture {
    
public:
    static RenderTextureEx * create(int w ,int h, Texture2D::PixelFormat format, GLuint depthStencilFormat);
    
    /** creates a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
    static RenderTextureEx * create(int w, int h, Texture2D::PixelFormat format);
    
    /** creates a RenderTexture object with width and height in Points, pixel format is RGBA8888 */
    static RenderTextureEx * create(int w, int h);
    
    bool saveToFile(const std::string& filename, bool isRGBA = true);
    
    bool saveToFile(const std::string& filename, Image::Format format, bool isRGBA = true);
    
protected:
    void onSaveToFile(const std::string& fileName, bool isRGBA = true);
};