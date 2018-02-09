//
//  PhotoPage.h
//  PaintDress
//
//  Created by maxiang on 6/30/15.
//
//

#ifndef __PaintDress__PhotoPage__
#define __PaintDress__PhotoPage__

#include "cocos2d.h"
#include "BasePage.h"

class PhotoPage : public BasePage
{
public:
    CREATE_FUNC(PhotoPage);
    virtual bool init() override;
    PhotoPage();

    void setImage(const std::string& fileName);
    virtual void touchEnd(cocos2d::Touch *touch) override;
    
protected:
    
    
protected:
    cocos2d::Sprite *_image;
    cocos2d::extension::Scale9Sprite *_frame;
};

#endif /* defined(__PaintDress__PhotoPage__) */
