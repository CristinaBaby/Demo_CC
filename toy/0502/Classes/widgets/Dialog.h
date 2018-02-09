#pragma once

#include "cocos2d.h"
#include "../utilities/UITools.h"


#define xDialog         (Dialog::getInstance())

const string dialogYes = "ui07_btn_yes";
const string dialogQuit = "ui07_btn_quit";
const string dialogReplay = "ui07_btn_replay";
const string dialogClose = "ui07_close";


class Dialog
{
public:
    typedef std::function<void(const string& name)> ccDialogButtonCallback;
    typedef std::function<void(void)> ccDialogProgressCallback;
    
    static Dialog * getInstance();
    bool init();
    
    void showQuit(const string& text, ccDialogButtonCallback callback);
    void show(const string& text);
    void showOver(const string& text, int starCount, ccDialogButtonCallback callback);
    void showContinue(const string& text, ccDialogButtonCallback callback, Layer *layer);
    void showProgress(float percent, ccDialogProgressCallback callback, Layer *layer);
    void showExit(ccDialogButtonCallback callback, Layer *layer);
    void onButtonQuit(Ref* pSender, Widget::TouchEventType type);
    void onButtonOver(Ref* pSender, Widget::TouchEventType type);
    void onButtonContinue(Ref* pSender, Widget::TouchEventType type);
    void hide();
    
    bool isVisible();
    
    float getPercent();
    
private:
    Dialog();
    
    Layout      * _rootQuit;
    Layout      * _rootOver;
    Layout      * _rootProgress;
    Layout      * _rootContinue;
    Layout      * _rootExit;
    ImageView   * _quitIcon;
    
    
    Text    * _labelQuit;
    Text    * _labelOver;
    Text    * _labelContinue;
    
    ccDialogButtonCallback _ccButtonCallback;
    ccDialogProgressCallback _ccProgressCallback;
    
    Vector<CheckBox*>   _stars;
    
    ProgressTimer * _progressTimer;
};
