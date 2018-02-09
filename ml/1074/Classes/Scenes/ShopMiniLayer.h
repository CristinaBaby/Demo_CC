
#ifndef __ShopMiniLayer__
#define __ShopMiniLayer__


#include "cocos2d.h"
#include "IAPItem.h"
#include "Heads.h"

USING_NS_CC;


class ShopMiniLayer : public Layer
{
public:
    ShopMiniLayer();
    ~ShopMiniLayer();
    
    static ShopMiniLayer* create(bool dec = true);
    
    virtual bool init(bool dec = true);
    virtual void onEnter();
    virtual void onExit();
    
    void onItemClicked(int);
    
    void setExPath(const std::string&  path){m_sExPath = std::string(path);}
    std::string localPath(const std::string&  name){
        return m_sExPath+std::string(name);
    }
    
    void showBannerDismiss(bool show = true){m_bShowBanner = show;}
    
    std::function<void()>shopDismissed;
    std::function<void()>shopGetFree;
    int getPercent();
protected:
    virtual void _initData();
    
    void _dismiss();
    
    Vector<IAPItem*> m_ItemVector;
    Point m_InnerPosition;
    bool m_bIsScrolling;
    int m_nPercent;
    
    std::string m_sExPath;
    bool m_bShowBanner;
};

#endif