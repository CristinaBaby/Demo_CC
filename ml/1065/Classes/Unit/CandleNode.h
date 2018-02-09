
#ifndef CandleNode_h
#define CandleNode_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>

USING_NS_CC;
class CandleNode : public Node
{
public:
    CandleNode();
    ~CandleNode();
    
    CREATE_FUNC(CandleNode);
    virtual bool init();
    void initCandle(int type,int index);
    inline void addFire(Node* pNode){
        this->addChild(pNode);
        pNode->setPosition(this->convertToNodeSpace(m_pCandle->convertToWorldSpace(firePosition)));
        m_VectorFire.pushBack(pNode);
    }
    inline void removeFire(){
        for_each(m_VectorFire.begin(), m_VectorFire.end(), [=](Node* pNode){
            pNode->removeFromParent();
        });
        m_VectorFire.clear();
    }
    
    inline Vec2 getFireWorldPos(){
        return m_pCandle->convertToWorldSpace(firePosition);
    }
    
    inline  int getCandleType(){
        return m_nType;
    }
    inline  int getCandleIndex(){
        return m_nIndex;
    }
    CC_SYNTHESIZE(Vec2, firePosition, FirePosition);
protected:
    Sprite* m_pCandle;
    Vector<Node*> m_VectorFire;
    int m_nType;
    int m_nIndex;
};
#endif /* CandleNode_h */
