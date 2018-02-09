
#ifndef __ChooseBgScene__
#define __ChooseBgScene__


#include "ExtensionScene.h"
#include "CandlePosition.h"
#include "CandleNode.h"

class ChooseBgScene : public ExtensionScene
{
public:
    ChooseBgScene();
    ~ChooseBgScene();
    
    MY_SCENE(ChooseBgScene);
    CREATE_FUNC(ChooseBgScene);
    
    virtual bool init();
    
    void onBgCallback(int index);
protected:
    virtual void _initData();
    
    void _showChooseLayer();
    
    Layer* m_pCandleLayer;
    Sprite* m_pDecoratedCake;
    Vector<CandleNode*> m_VectorCandle;
    Sprite* m_pBg;
    Sprite* m_pTable;
};

#endif