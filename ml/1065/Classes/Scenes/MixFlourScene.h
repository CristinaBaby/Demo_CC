
#ifndef __MixFlourScene__
#define __MixFlourScene__


#include "ExtensionScene.h"

class MixFlourScene : public ExtensionScene
{
public:
    MixFlourScene();
    ~MixFlourScene();
    
    MY_SCENE(MixFlourScene);
    CREATE_FUNC(MixFlourScene);
    
    virtual bool init();
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    
    DragNode* _createDrageNode(std::string path,bool enable = true);
    void _showShadow(DragNode* pDragNode);
    void _hideShadow(DragNode* pDragNode);
    void _showIngrident(int index);
    void _showIngrident(Node*);
    void _addFinished();
    
    void _resumeFry();
    void _pauseFry();
    void _resetBar();
    void _showBar();
    void _hideBar();
    ProgressBar* m_pBar;
    
    Sprite* m_pFlour;
    DragNode* m_pFlourSpoon;
    Sprite* m_pEggBox;
    DragNode* m_pEgg;
    DragNode* m_pEgg2;
    DragNode* m_pSugar;
    DragNode* m_pMilk;
    Sprite* m_pBufferBox;
    DragNode* m_pBufferSpoon;
    DragNode* m_pWhrisk;
    
    Sprite* m_pBowl;
    Sprite* m_pBowlFront;
    Node* m_pMixture;
    DragNode* m_pBowlMixture;
    
    Sprite* m_pPan;
    
    GuideLayer *m_pGuideLayer2;
    Vector<Node*> m_VectorIngredient;
    
    int m_nAddCount;
    int m_nMixCount;
    int m_nIngridentCount;
    int m_nLeftShow;
    int m_nRightShow;
    bool m_bWhriskIsReady;
    
    int m_nAddStep[7]; //加了进度条  搅拌失败会重新搅拌  需纪录添加材料的层次 ［baking,egg,egg,flour,milk,salt,sugar］
};

#endif