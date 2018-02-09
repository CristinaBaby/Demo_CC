//
//  DragBtnAdapter.h
//  JuiceMaker
//
//  Created by luotianqiang1 on 3/27/15.
//
//

#ifndef __JuiceMaker__DragBtnAdapter__
#define __JuiceMaker__DragBtnAdapter__

#include <iostream>
#include"MyBaseAdapter.h"
#include "LQMath.h"
NS_KD_BEGIN
class DragBtnAdapter : public MyBaseAdapter {
public:
    CREATE_ADAPTER(DragBtnAdapter);
    virtual EventSprite * getItem(size_t position);
    void setRects(Vector<LQPolygon*>  limitRects);
    DragBtnAdapter();
    ~DragBtnAdapter();
protected:
    Vector<LQPolygon*> limitRects;
};
NS_KD_END
#endif /* defined(__JuiceMaker__DragBtnAdapter__) */
