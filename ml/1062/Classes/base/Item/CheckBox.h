//
//  CheckBox.h
//  MermaidSeaKingdom
//
//  Created by luotianqiang1 on 15/8/19.
//
//

#ifndef __MermaidSeaKingdom__CheckBox__
#define __MermaidSeaKingdom__CheckBox__

#include "EventSprite.h"
#include "LQPolygon.h"
NS_KD_BEGIN

enum class  CHECK_TYPE{
    CHECK_TYPE_SELECT,
    CHECK_TYPE_UNSELECTED
};

class CheckBox : public EventSprite {
public:
    CREATE_FUNC(CheckBox);
    static CheckBox* create(std::string _nomalPath, std::string  _selectPath);
    virtual ~CheckBox();
    void loadTexture(std::string _nomalPath, std::string  _selectPath);
    void setNomalTex(Texture2D*);
    void setSelectTex(Texture2D*);
    void setCheckType(CHECK_TYPE _type);
    void changeState();
    CC_SYNTHESIZE_RETAIN(LQPolygon*, _polygon, Polygon);
protected:
    CheckBox();
    bool init();
    virtual bool isTouchTex(Touch *pTouch);
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    Texture2D *nomalTex,*selectTex;
    CHECK_TYPE _check_type;
public:
    std::function<void(CheckBox*,CHECK_TYPE)> selectCallBack;
};
NS_KD_END

#endif /* defined(__MermaidSeaKingdom__CheckBox__) */
