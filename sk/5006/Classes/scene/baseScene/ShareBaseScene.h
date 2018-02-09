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
#include "Charactor.h"
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
    void registEat(Sprite *sp);
    void registServe(Node* foodNode, Rect r,function<void()> callfunc);
    void resetServe(Node* food);
protected:
    Button *_nextBtn;
    Button *_preBtn;
    Button *_shareBtn;
    Button *_homeBtn;
    
    bool _isClicked;
    int _eatCount;
    int _coinCount;
    vector<ComponentEat*> _eatComVec;
    Charactor* _npc;

    int _foodCount;
    function<void()> _allCoinCollceted = nullptr;
    function<void()> _foodServedOver = nullptr;
    function<void()> _preBtnClicked = nullptr;
    
    
    
    vector<Sprite*> _scoreNumberVec;
    int _tempScore;
    Sprite* _coinBg;
    bool _isFirstInit;
    bool _allCoinGet;
    bool _walkOut;
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
    
    void showCoin();
    void showCoinScore(int number,bool animated = false);
    void addCoinScore();
    
    void showServeAgain(function<void()> call);
    void checkReserve();
    
    void shareImage();
    void saveImage();
    void onPermissionGrantedResult(int requestCode,bool bGranted);

};

#endif /* ShareBaseScene_h */
