//
//  ComponentEventType.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/4.
//
//

#ifndef ComponentEventType_hpp
#define ComponentEventType_hpp

#include <iostream>
using namespace std;

extern const string  ComponentTouchEnd;
extern const string  ComponentTouchBegan;
extern const string  ComponentTouchMove;
extern const string  ComponentTouchMoveALL;
extern const string  ComponentTouchCancle;


extern const string  ACCLE_ROTE_END;//rote end valuse
extern const string  ACCLE_ROTE_BACK; //rote back
extern const string  ACCLE_ROTE_NULL;


extern const string  FallThingMoveEnd;//移动到碗里
extern const string  FallThingstartFall;//开始倒
extern const string  FallThingfallEnd;//倒完了
extern const string  FallThingResume;
extern const string  FallThingPause;

extern const string  MoveForeverCallBack;

extern const string  MoveDistancePercent;

extern const string  FrameChangeeEnd;
extern const string  FrameChange;

extern const string cutDountFailed;
extern const string cutDountSuccess;
extern const string cutDountEnd;

extern const string  FryOperateStart;
extern const string  FryOperatePause;
extern const string  fryOperateing;
extern const string  fryOperateOk;
extern const string  fryOperateFailed;
extern const string  FryOperateEnd;

extern const string  removeNode;

extern const string  InertiaInvoking;
extern const string  InertiaStop;
extern const string  Inertiaing;
#endif /* ComponentEventType_hpp */
