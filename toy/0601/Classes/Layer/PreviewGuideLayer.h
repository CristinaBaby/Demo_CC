//
//  PreviewGuideLayer.h
//  PaintDress
//
//  Created by maxiang on 7/7/15.
//
//

#ifndef __PaintDress__PreviewGuideLayer__
#define __PaintDress__PreviewGuideLayer__

#include "cocos2d.h"

class PreviewGuideLayer : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(PreviewGuideLayer);
    virtual bool init() override;
    virtual ~PreviewGuideLayer();

};


#endif /* defined(__PaintDress__PreviewGuideLayer__) */
