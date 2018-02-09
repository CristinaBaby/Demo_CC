//
//  Charactor.h
//  BHUG1062
//
//  Created by liji on 16/4/27.
//
//

#ifndef Charactor_h
#define Charactor_h

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/cocostudio.h"
#include "ui/cocosGUI.h"
using namespace cocos2d;
using namespace std;
using namespace cocostudio;
using namespace ui;

#define NPC_WALK "walk"
#define NPC_EAT "eat"
#define NPC_JUMP "jump"
#define NPC_GOOD "good"
#define NPC_BAD "bad"
#define NPC_STAND "breath"
#define NPC_LOOK "look"


#define NPC_FOODREADY "good_food"
#define NPC_ANGRY "anger"
#define NPC_DISAPPOINT "disappointment"
#define NPC_HAPPY "happy"

#define NPC_CONGRANTS "love"
#define NPC_SAYHI "say_hi"
#define NPC_RUN "run"


class Charactor : public Armature
{
public:
    
    Charactor();
    ~Charactor(){};
    static Charactor* createModel(string name);
    bool init(string name);
    
    void initProgressTimer();
    void initProgressTimerHeartStyle();

     void walkTo(Vec2 endPos,float duration,function<void()> call);
//     void playAction(const string &name);
     void playAction(const string &name ,function<void()> callback = nullptr);
    void startWaiting();
    void pauseWaiting();
    void resumeWaiting();
    
    void eat(function<void()> eatCall = nullptr, function<void()> leaveCall = nullptr);
    void leave(function<void()> call = nullptr);
    void angry();

    function<void()> _walkedOutScreen = nullptr;
    function<void(int foodType)> _foodButtonClicked = nullptr;
    
    void sayHi(function<void()> call = nullptr);
    void happy(function<void()> call = nullptr);
    void foodReady(function<void()> call = nullptr);
    
     void playRandomAction();
     
private:
    void checkStatus(float dt);
    void playVoiceEat();
    void playVoiceStand();
    void onEnter();
    string getEmojiPath();
    void updateEmoji();
    void standBy();
    void laugh();

    
protected:
    Sprite* _progressBg = nullptr;
    Sprite* _emoji = nullptr;
    ProgressTimer* _progressTimer = nullptr;
    Vector<Sprite*> _heartsVec;
    vector<ParticleSystemQuad*> _fireVec;
    
    int _npcIndex = 0;
    int _heartIndex = 0;
    CC_SYNTHESIZE(int, _angryTime, AngryTime);
    CC_SYNTHESIZE(int, _requiredFoodType, RequiredFoodType);
    CC_SYNTHESIZE(string, _name, Name);
    const int ANGRY_TIME = 21;
    const int EMOTION_STEP = 3;


};
#endif /* Charactor_h */
