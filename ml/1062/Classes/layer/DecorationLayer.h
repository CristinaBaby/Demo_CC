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
#include "ShadeSprite.h"
#include "TouchNodeComponent.h"
class DecorationLayer: public BaseDecorationLayer{
    DECLARE_DYNCLASS(DecorationLayer);
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
    virtual void freeClick(KDS::EventSprite*,int,KDS::MyScollView*);
    void reset();
    Map<string,Node*> decoration;
private:
    static const string _eventName;
    CheckBoxListView _listView;
};

#endif /* DecorationLayer_hpp */
