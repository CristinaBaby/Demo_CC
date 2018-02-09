//
//  DragBtnAdapter.cpp
//  JuiceMaker
//
//  Created by luotianqiang1 on 3/27/15.
//
//

#include "DragBtnAdapter.h"
#include "MoveSprite.h"
USING_NS_KD;
EventSprite * DragBtnAdapter::getItem(size_t position) {
    auto sp = MoveSprite::create(file_path.at(position));
    sp->setMoveType(kMoveEndBackStart);
    if(position<limitRects.size())
        sp->setRect(limitRects.at(position));
    return sp;
}

void DragBtnAdapter::setRects(Vector<LQPolygon*> limitRects) {
    this->limitRects = limitRects;
}

DragBtnAdapter::DragBtnAdapter() {
    
}
DragBtnAdapter::~DragBtnAdapter() {
    
}