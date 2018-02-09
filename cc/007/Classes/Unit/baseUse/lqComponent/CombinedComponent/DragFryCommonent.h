//
//  DragFryCommonent.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/14.
//
//

#ifndef DragFryCommonent_hpp
#define DragFryCommonent_hpp

#include "LQComponent.h"
#include "TouchMoveComponent.h"
#include "ui/CocosGUI.h"
class DragFryCommonent : public LQComponent {
    DECLARE_COMPONENT_DYNCLASS(DragFryCommonent);
public:
    DragFryCommonent();
    ~DragFryCommonent();
    virtual bool init();
    virtual void onAdd();
    virtual void onRemove();
public:
    void setFryLimit(LQPolygon *);
    void setEndlimit(LQPolygon *);
    void setFryPath(const vector<string>& fryPath);
    
    void setLoadingBar( ui::LoadingBar* _progress);
    void setFryTime(float);
    void setBubblePath(const vector<string>& Path);
    
    void startFry();
    void stopFry();
protected:
    void fryUpdate(float);
    void touchOperate(LQComponent* _move,OperateListner* _lis);
private:
    LQPolygon *fryLimit,*endLimit;
    Vector<Sprite*> fryNodes;
    vector<string> bubblePath;
    float time,currentTime;
    size_t count;
    bool isOk;
    Sprite* bubble;
    ui::LoadingBar* _progress;
};

#endif /* DragFryCommonent_hpp */
