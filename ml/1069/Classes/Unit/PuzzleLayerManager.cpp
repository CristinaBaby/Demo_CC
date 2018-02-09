
#include "PuzzleLayerManager.h"

void PuzzleLayerManager::init(Node* pNode,int index)
{
    onFinishedCallback = nullptr;
    std::stringstream ostrUnit;
    ostrUnit<<"puzzle"<<index;
    Node* pPuzzleUnit = pNode->getChildByName(ostrUnit.str());
    
    std::stringstream ostrBg0;
    ostrBg0<<"bg"<<index+1;
    m_pBg0 = dynamic_cast<Sprite*>(pPuzzleUnit->getChildByName(ostrBg0.str()));
    
    
    std::stringstream ostrBg1;
    ostrBg1<<"bg"<<index+1<<"_1";
    m_pBg1 = dynamic_cast<Sprite*>(pPuzzleUnit->getChildByName(ostrBg1.str()));
    
    std::stringstream ostrBg2;
    ostrBg2<<"bg"<<index+1<<"_2";
    m_pBg2 = dynamic_cast<Sprite*>(pPuzzleUnit->getChildByName(ostrBg2.str()));
    
    m_pShape = dynamic_cast<Sprite*>(pPuzzleUnit->getChildByName("shape"));
    
    for (int i=0; i<4; i++) {
        std::stringstream ostrUnit;
        ostrUnit<<"puzzle"<<index+1<<"_"<<i+1;
        Sprite* pUnit = dynamic_cast<Sprite*>(pPuzzleUnit->getChildByName(ostrUnit.str()));
        pUnit->setTag(i);
        m_PuzzleUnits.pushBack(pUnit);        
    }
    m_PuzzleUnitsCopy.operator=(m_PuzzleUnits);
}
void PuzzleLayerManager::beginPuzzle()
{
    m_pBg0->setVisible(false);
    m_pBg1->setVisible(false);
    m_pBg2->setVisible(true);
    m_pShape->setVisible(false);
    for_each(m_PuzzleUnits.begin(),m_PuzzleUnits.end(),[=](Sprite* pUnit){
        pUnit->setVisible(false);
    });
}

Node* PuzzleLayerManager::addPuzzleUnit(cocos2d::Rect wordRect,int index)
{
    int size = m_PuzzleUnitsCopy.size();
    for (int i = 0; i<size; i++) {
        Sprite* pUnit = m_PuzzleUnitsCopy.at(i);
        Vec2 point = pUnit->getPosition();
        point = pUnit->getParent()->convertToWorldSpace(point);
        int tag = pUnit->getTag();
        if (index==tag && wordRect.containsPoint(point)) {
            pUnit->setVisible(true);
            m_PuzzleUnitsCopy.eraseObject(pUnit);
            int count = m_PuzzleUnitsCopy.size();
            if (count==0) {
                finish();
                if (onFinishedCallback) {
                    onFinishedCallback();
                }
            }
            return pUnit;
        }
    }
    return nullptr;
}

void PuzzleLayerManager::finish()
{
    m_pBg0->setVisible(false);
    m_pBg1->setVisible(true);
    m_pBg2->setVisible(false);
//    m_pShape->setVisible(true);
    for_each(m_PuzzleUnits.begin(),m_PuzzleUnits.end(),[=](Sprite* pUnit){
        pUnit->setVisible(false);
    });    
}

Sprite* PuzzleLayerManager::copeShape()
{
    Sprite* pSprite = Sprite::createWithTexture(m_pShape->getTexture());
    pSprite->setPosition(m_pShape->getParent()->convertToWorldSpace(m_pShape->getPosition()));
    return pSprite;
}