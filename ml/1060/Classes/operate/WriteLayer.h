//
//  WriteLayer.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/4.
//
//

#ifndef WriteLayer_hpp
#define WriteLayer_hpp

#include "BaseDecorationLayer.h"
//#include "CaculateScribbleNode.h"
#include "CheckBoxListView.h"
#include "EventLayer.h"
#include "ScribbleSauce.h"
class WriteLayer : public BaseDecorationLayer{
    DECLARE_DYNCLASS(WriteLayer);
public:
    CREATE_FUNC(WriteLayer);
    WriteLayer();
public:
   void showOperate(Vec2 startPos);
protected:
    bool init();
    virtual void touchEnd(ui::Widget* widget);
    virtual void onEnterTransitionDidFinish();
    void choose(ui::CheckBox*,int index);
    void hide(ui::CheckBox*,int index);
    virtual void freeClick(KDS::EventSprite*,int,KDS::MyScollView*);
    ScribbleSauce* _drawNode;
    Node* _rendreNode;
    Sprite* _bgSprite;
    EventLayer* _paper;
    CheckBoxListView  _checkList;
    virtual void reset();
    bool isDecs;
    void showFinger();
    void hideFinger();
    void hideUi();
    void endbleUi();
    virtual bool onBack() {return true;}
private:
    void changeDec(KDS::EventSprite* s,string _name,const int index);
    Vec2 _startPos;
    Vec2 _endPos;
    string bgPath;
    Sprite *_pen,*_brush;
public:
    std::function<void(Node*,bool,string)> writeEnd;
};

#endif /* WriteLayer_hpp */
