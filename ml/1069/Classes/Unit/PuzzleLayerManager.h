
#ifndef PuzzleLayerManager_h
#define PuzzleLayerManager_h

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class PuzzleLayerManager
{
public:
    
    void init(Node* pNode,int index);
    void beginPuzzle();
    Node* addPuzzleUnit(Rect wordRect,int index);
    void finish();

    Sprite* copeShape();
    std::function<void()> onFinishedCallback;
private:
    Sprite* m_pBg0;
    Sprite* m_pBg1;
    Sprite* m_pBg2;
    
    Sprite* m_pShape;
    Vector<Sprite*> m_PuzzleUnits;
    Vector<Sprite*> m_PuzzleUnitsCopy;
};
#endif /* PuzzleLayerManager_h */
