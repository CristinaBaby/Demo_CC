//
//  DecorationLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef DecorationLayer_hpp
#define DecorationLayer_hpp

#include "BaseDecorationLayer.h"
#include "CheckBoxListView.h"
#include "TouchNodeComponent.h"
#include "ScribbleWithCallBack.h"
class DecorationLayer: public BaseDecorationLayer{
public:
    CREATE_SECENE(DecorationLayer);
    DecorationLayer();
    ~DecorationLayer();
    bool init();
    virtual void onEnterTransitionDidFinish();
      virtual void onExitTransitionDidStart();
protected:
    void showEnd();
    void touchListView(ui::CheckBox*,int index);
    void unSelect(ui::CheckBox*,int index);
    void touchEnd(ui::Widget* widge);
    virtual void freeClick(KDS::EventSprite*,size_t,KDS::MyScollView*);
    void reset();
    void creatIcon(size_t _index);
private:
    static const string _eventName;
    CheckBoxListView _listView;
    int chooseIndex;
    ScribbleWithCallBack* _icing;
    ClippingNode* clipping,*clippingFruit;

};

#endif /* DecorationLayer_hpp */
