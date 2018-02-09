//
//  FryDount.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/14.
//
//

#ifndef FryDount_hpp
#define FryDount_hpp

#include "BaseStepLayer.h"
#include "DragFryCommonent.h"
class FryDount: public BaseStepLayer{
      DECLARE_DYNCLASS(FryDount);
public:
    CREATE_FUNC(FryDount);
    FryDount();
protected:
    bool init();
    void addFryOperate(Node* _dount);
    virtual void onEnterTransitionDidFinish();
    Vector<Sprite*> dounts;
    vector<Vec2> _startPos;
    void fryEnd(Node*,Component*,OperateListner*);
    void fryFailed(Node*,Component*,OperateListner*);
    int fryCount;
    bool isFirst;
};


#endif /* FryDount_hpp */
