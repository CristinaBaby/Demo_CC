//
//  WheelView.h
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/8/18.
//
//

#ifndef __MermaidSeaKingdom__WheelView__
#define __MermaidSeaKingdom__WheelView__

#include "CheckBox.h"
using namespace std;
NS_KD_BEGIN
class WheelView : public EventSprite {
public:
    WheelView();
     static WheelView* create(const std::string& filename);
    void randRote();
    void randIndex();
    void addItems(Vector<KDS::CheckBox*>);
    void addItems(vector<string> _nomalPath, vector<string> _selectPath);
    void setPositionR(float _r);
    std::function<void(CheckBox*,size_t _index,CHECK_TYPE)> _itemClick;
     std::function<void(CheckBox*,size_t _index,CHECK_TYPE)> _randitem;
    Vector<CheckBox*>& getItems(){return _items;}
protected:
    enum class  WheelEventType {
        type_move,
        type_click,
        type_null
    };
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancle(Touch *pTouch, Event *pEvent);
    void click(CheckBox*,CHECK_TYPE);
    
    void setRote(float _degree,float _time);
    void Roteing(float);
    void deaccelerateRoteing(float);
    void stopRoteAnimal();
    float _roteDistance;
    bool isDrag;
    WheelEventType _evnetTyp;
    
    float _positionR;//item的位置到中心点的距离
    Vector<CheckBox*> _items;
    
    int curentIndex;
    int changeTime;
    size_t endIndex;
    void changeTex();
    bool isChange;
    float touchRote;
    int musicJudge;
};
NS_KD_END
#endif /* defined(__MermaidSeaKingdom__WheelView__) */
