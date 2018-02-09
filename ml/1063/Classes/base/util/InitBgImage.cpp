#include"initBgImage.h"

Texture2D* InitBgImage::crateBgTexNoCache(const char* fileName) {
   // return Director::getInstance()->getTextureCache()->addImage(fileName);
    auto image = new Image();
    Texture2D *tex =nullptr;
    if(image->initWithImageFile(fileName)) {
         tex = new Texture2D();
        if(!tex->initWithImage(image, Texture2D::PixelFormat::RGB565))
            CC_SAFE_RELEASE(tex);
        else
            tex->autorelease();
    }
    CC_SAFE_RELEASE(image);
    return tex;
}

Texture2D* InitBgImage::addBgImage(const char * path){
  
    return Director::getInstance()->getTextureCache()->addImage(path);
    
//    Texture2D * texture = nullptr;
//    Image* image = nullptr;
//    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(path);
//    if (fullpath.size() == 0)
//    {
//        return nullptr;
//    }
//    auto it = _textures.find(fullpath);
//    if( it != _textures.end() )
//        texture = it->second;
//    
//    if (! texture)
//    {
//        // all images are handled by UIImage except PVR extension that is handled by our own handler
//        do
//        {
//            image = new Image();
//            CC_BREAK_IF(nullptr == image);
//            
//            bool bRet = image->initWithImageFile(fullpath);
//            CC_BREAK_IF(!bRet);
//            
//            texture = new Texture2D();
//            
//            if( texture && texture->initWithImage(image, Texture2D::PixelFormat::RGB565) )
//            {
//
//            
//                // texture already retained, no need to re-retain it
//                _textures.insert( std::make_pair(fullpath, texture) );
//            }
//            else
//            {
//                
//            }
//        } while (0);
//    }
//    
//    CC_SAFE_RELEASE(image);
//    
//    return texture;
}