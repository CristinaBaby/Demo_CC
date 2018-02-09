//
//  PreviewPage.h
//  PaintDress
//
//  Created by maxiang on 6/30/15.
//
//

#ifndef __PaintDress__PreviewPage__
#define __PaintDress__PreviewPage__

#include "cocos2d.h"
#include "BasePage.h"

class PreviewPage : public BasePage
{
public:
    CREATE_FUNC(PreviewPage);
    virtual bool init() override;
    void setBackground(const std::string& fileName);
    
    virtual void touchEnd(cocos2d::Touch *touch) override;

protected:
    cocos2d::Sprite *_background;
    std::string _fileName;
};


#endif /* defined(__PaintDress__PreviewPage__) */
