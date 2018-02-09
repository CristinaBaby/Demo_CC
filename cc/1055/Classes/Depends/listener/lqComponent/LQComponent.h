//
//  AccleComplent.hpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/2.
//
//

#ifndef LQComponent_h
#define LQComponent_h

#include "cocos2d.h"
#include "DynBase.h"
#include "DynAny.h"
#include "ComponentEventType.h"
#include "cocostudio/cocostudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
USING_NS_CC;
class OperateListner;
class LQComponent;
typedef  std::function<void(LQComponent*, OperateListner* _lis)> ComponentCallFunc;

class OperateListner: public Ref {
    CC_SYNTHESIZE(ComponentCallFunc, _callBack, CallBack);
    CC_SYNTHESIZE(string,_key,Key);
    CC_SYNTHESIZE(bool,_enable,Enable);
public:
    CREATE_FUNC(OperateListner)
    virtual bool init(){return true;}
    OperateListner():
    _key("")
    ,_callBack(nullptr)
    ,_enable(true){
   
        
    }
public:
    void setData(const DynAny& _any){
        _data = _any;
    }
    const DynAny& getData() {
        return _data;
    }
private:
    DynAny _data;
};

#define DECLARE_COMPONENT_DYNCLASS(name)\
public:\
CREATE_FUNC(name)\
DECLARE_DYNCLASS(name)\

class LQComponent: public Component,public DynObject{
    DECLARE_COMPONENT_DYNCLASS(LQComponent);
public:
    LQComponent();
    virtual ~LQComponent();
public:
    virtual bool init();
    virtual void onResume();
    virtual void onPause();
    virtual void onAdd();
    virtual void onRemove();
    virtual void setEnabled(bool b);
	virtual void setTouchPorit(int _por);
    void setDispatche(EventDispatcher*);
    void addComponent(Component*);
    LQComponent* getComponent(string _name);
    virtual void addListners(string name,ComponentCallFunc _callBack);
    virtual void addListners(OperateListner*);
    virtual void removeListner(OperateListner* ,bool isRemoveAll = false);
    virtual void removeListner(string _eventName,bool isRemoveAll = false);
    Node* getLisNode(){return _lisNode;}
    void playSound();
    void pauseSound();
    void stopSound();
    void changeSound(const string& _path);
    virtual void dispatcherEvent(string _eventName);
    virtual void dispatcherEventWithData(string _eventName,const DynAny& _any);
protected:
	virtual EventListener* createLis();
    void scheduleOnce(SEL_SCHEDULE , float );
    void schedule(SEL_SCHEDULE , float , unsigned int , float );
    void schedule(SEL_SCHEDULE , float );
    void unSchedule(SEL_SCHEDULE);
    bool isSchedule(SEL_SCHEDULE);
protected:
     EventListener * _componentLis;
	 EventDispatcher* _dispatcher;
     Scheduler* _scheduler;
     Node* _lisNode;
     Map<string,LQComponent*>* _childCompoent;
    Vector<OperateListner*>* _listners;
    bool isRunning;
    bool isSoundPlaying;
private:
    ComAudio* _sound;
    int _soundId;
};

#endif /* AccleComplent_h */
