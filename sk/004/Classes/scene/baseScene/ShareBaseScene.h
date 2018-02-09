//
//  ShareBaseScene.h
//  ccats1061
//
//  Created by liji on 16/8/25.
//
//

#ifndef ShareBaseScene_h
#define ShareBaseScene_h

#include <stdio.h>
#include "ESMakeBaseScene.h"
#include "ComponentEat.h"
#include "RuntimePermission.h"
#include "RuntimePermissionManager.h"
class ShareBaseScene : public ESMakeBaseScene
{
public:
    
    ShareBaseScene();
    ~ShareBaseScene();
    
    MY_SCENE(ShareBaseScene);
    CREATE_FUNC(ShareBaseScene);
    bool init();
    virtual void initFood();
    void registEat(Sprite *sp);
    void registServe(Node* foodNode, function<void()> callfunc);
    
protected:
    Button *_nextBtn;
    Button *_preBtn;
    Button *_shareBtn;
    Button *_homeBtn;
    
    bool _isClicked;
    int _eatCount;
    int _coinCount;
    vector<ComponentEat*> _eatComVec;
    Sprite* _girl;
    
    int _foodCount;
    int _girlIndex;
    function<void()> _girlWalkedIn = nullptr;
    function<void()> _foodServed = nullptr;
    function<void()> _foodServedOver = nullptr;
    
    vector<Sprite*> _scoreNumberVec;
    int _tempScore;
    Sprite* _coinBg;
protected:
    void initUI();
    void initEatHint(bool isEat = true);
    void saveForShare(const std::string& name, const std::function<void(RenderTexture*, const std::string&)>& callback);
    void shareBtnClicked();
    void onEnter();
    void onExit();
    void resetEat();
    void onEnterTransitionDidFinish();
    
    void chooseServe();
    void initGirl();
    void girlSmile();
    void girlEat();
    void girlWalk(Vec2 endPos,float duration,function<void()> call);
    void girlStand();
    
    void showCoin();
    void showCoinScore(int number,bool animated = false);
    void addCoinScore();
    
    void showServeAgain(function<void()> call);
private:
    void shareImage();
    void saveImage();
    void onPermissionGrantedResult(int requestCode,bool bGranted);
};

#endif /* ShareBaseScene_h */
