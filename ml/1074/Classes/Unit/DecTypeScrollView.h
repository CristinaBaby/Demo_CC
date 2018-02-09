
#ifndef __DecTypeScrollView__
#define __DecTypeScrollView__


#include "cocos2d.h"
#include "Heads.h"

USING_NS_CC;

class DecTypeScrollView : public Layer
{
public:
    enum class Direction{
        eDirectionV,
        eDirectionH,
    };
    
    DecTypeScrollView();
    ~DecTypeScrollView();
    
    static DecTypeScrollView* createWithSize(Size size);
    virtual bool init(Size size);
    
    void setDirection(Direction dir){m_eDirection = dir;};
    
    void loadType(std::vector<DecorateTypeConfigData>);
    
    void hideAll();
    void showAll();
    
    std::function<void(int index,int type,DecorateTypeConfigData data,bool selected)> onItemCellSelected;
    
    void scrollToNext();
    
    void scrollToPre();
    DecorateTypeConfigData decTypeData;
    
    void selectBtn(int index);
    
    std::string btnPathEX;
    std::string boxPathEX;
    int getPercent();
    void setNormal(){        
        if (m_pCurTypeBtn) {
            int tag = m_pCurTypeBtn->getTag();
            DecorateTypeConfigData data = m_DecTypeVector.at(tag);
            if (m_bSingleAsset) {
                m_pCurTypeBtn->loadTextureNormal(btnPathEX+data.decIconName+".png");
            }else{
                m_pCurTypeBtn->loadTextureNormal(btnPathEX+"box_1_1.png");
            }
        }
        m_pCurTypeBtn = nullptr;
    }
    void unlcok(int index){
        
        Node* pChild = m_pScrollView->getInnerContainer()->getChildByTag(index);
        if(!pChild)
            return;
        Node* pLock = pChild->getChildByName(LOCK_NAME_REWARD);
        if (pLock) {
            pLock->setVisible(false);
        }
    }
    Vec2 getWorldItemPos(){
        if(m_pCurTypeBtn){
            log("=====%f,%f",m_pCurTypeBtn->getPositionX(),m_pCurTypeBtn->getPositionY());
            Vec2 pos = m_pCurTypeBtn->getParent()->convertToWorldSpace(m_pCurTypeBtn->getPosition());
            log("=====%f,%f",pos.x,pos.y);
            return pos;
        }
        return Vec2::ZERO;
    }
    
    //ture 表示按钮 normal和heigh light图片的背景和类别图片在一张图片资源上
//    反之  则表示分开2张资源的
    void setSingleAsset(bool isSingle = true){
        m_bSingleAsset = isSingle;
    }
    
    void setMargin(float margin){
        m_nMargin = margin;
    }
    void showSelectedAnimation(){
        if(!m_bSelectedAnimate){
            return;
        }
        if(m_pCurTypeBtn){
//            m_pCurTypeBtn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, 1.2),
//                                                                            ScaleTo::create(1, 1), NULL)));
            m_pCurTypeBtn->runAction(MoveBy::create(0.5, Vec2(0, 20)));
        }
    }
    void setSelectedAnimate(bool animate = true) {
        m_bSelectedAnimate = animate;
    }
    void unlock();
protected:
    void _itemSelected(Ref*,Widget::TouchEventType type);
    void _onButtonCallback(Ref *ref,Widget::TouchEventType);
protected:
    Sprite* m_pBoard;
    int m_nPercent;
    Button* m_pCurTypeBtn;
    std::vector<DecorateTypeConfigData> m_DecTypeVector;
    ExpandScrollView* m_pScrollView;
    Direction m_eDirection;
    bool m_bSingleAsset;
    float m_nMargin;
    bool m_bSelectedAnimate;
};

#endif