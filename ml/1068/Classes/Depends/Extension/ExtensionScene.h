
#ifndef __ExtensionScene__
#define __ExtensionScene__

#include "cocos2d.h"
#include "Heads.h"
#include "GameUILayoutLayer.h"
#include "GuideLayer.h"

USING_NS_CC;


template <class T>
class TemplateScene : public Layer{
    
public:
    static Scene* scene()
    {
        Scene *scene = Scene::create();
        T *layer = T::create();
        scene->addChild(layer);
        return scene;
    }
    static T* create()
    {
        T *pRet = new T();
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }
};



class NotifRef:public Ref
{
public:
    static NotifRef* create(std::string str)
    {
        NotifRef *ref = new NotifRef();
        if (ref) {
            ref->noticyString = str;
            ref->autorelease();
            return ref;
        }
        return NULL;
    }
    std::string noticyString;
};

class ExtensionScene : public Layer,public KeypadDelegate
{
public:
    ExtensionScene();
    ~ExtensionScene();
    
public:
    
    MY_SCENE(ExtensionScene);
    CREATE_FUNC(ExtensionScene);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void onKeyBackClicked();
    virtual void onNotificationCallback(Ref* ref){};
    virtual void onButtonCallback(Button* btn);
    virtual void onShopItemBuy(cocos2d::Ref *pRef){}
    
    virtual void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint){}
    virtual void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint){}
    virtual void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint){}
    virtual void dragNodeClicked(DragNode* pDragNode,Point worldPoint){}
    
    // 添加一个Node到背景层
    void addChildToBGLayer(Node* child);
    
    // 添加一个Node到UI层
    void addChildToUILayer(Node* child);
    
    // 添加一个Node到内容层
    void addChildToContentLayer(Node* child);
    
    void setExPath(const std::string&  path){m_sExPath = std::string(path);}
    std::string localPath(const std::string&  name){
        return m_sExPath+std::string(name);
    }
    
    void setExCocosPath(const std::string&  path){m_sExCocosPath = std::string(path);}
    std::string localCocosPath(const std::string&  name){
        return m_sExCocosPath+std::string(name);
    }
    
protected:
    
    DragNode* _createDrageNode(std::string path,bool enable = true);
    
protected:
    Layer* m_pBGLayer;
    Layer* m_pUILayer;
    Layer* m_pContentLayer;
    
    GameUILayoutLayer* m_pGameUI;
    GuideLayer* m_pGuideLayer;
    TipLayer* m_pTip;
    
    std::string m_sExPath;
    std::string m_sExCocosPath;
    
    int m_nNextSceneTag;
    bool m_bShowAds;
};

#endif /* defined(__ExtensionScene__) */
