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
#include "StudioLayer.h"
#include "CheckBoxListView.h"
#include "ShadeSprite.h"
#include "ScribbleWithCallBack.h"
#include "TouchNodeComponent.h"
#include "ScribbleTouchNode.h"
#include "ScribbleSauce.h"
class DecorationLayer: public BaseDecorationLayer{
    DECLARE_DYNCLASS(DecorationLayer);
public:
    DecorationLayer();
    ~DecorationLayer();
    CREATE_FUNC(DecorationLayer);
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
    void creatIcon(int _index);
private:
    static const string _eventName;
    CheckBoxListView _listView;
    Node* _eatContiner;
    TouchNodeComponent* _thingOperate;
    ScribbleWithCallBack* _icing;

    ScribbleSauce*_sauce;
    string _decPath;
    void showFinger();
    void hideFinger();
    ClippingNode* _clipp;
    vector<string> _suaceNames;
    vector<string> _suaceDecos;
    vector<string> _chooseTyps;
    void addTypes();
    bool _suaceIs;
};

#endif /* DecorationLayer_hpp */
