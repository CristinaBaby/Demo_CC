//
//  MyGridView.h
//  MyCppGame
//
//  Created by luotianqiang1 on 9/15/14.
//
//

#ifndef __MyCppGame__MyGridView__
#define __MyCppGame__MyGridView__

#include <iostream>
#include "MyScollView.h"
NS_KD_BEGIN
class MyGridView : public MyScollView {
public:
    static MyGridView * create(float w, float h);
    static MyGridView * create();
    MyGridView();
     virtual void setAdapter(MyBaseAdapter* base);
    void setCol(size_t col);
    void setRow(size_t row);
    void setItemSize(Size itemSize);
    void setSize(float w,float h);
protected:
    size_t col,row;
    Size itemSize;
    void onTouchMoved(Touch* touch, Event* event);
};
NS_KD_END
#endif /* defined(__MyCppGame__MyGridView__) */
