
#ifndef __MakeGummyPizzaScene__
#define __MakeGummyPizzaScene__


#include "ExtensionScene.h"
#include "OvenNode.h"
#include "FreezerNode.h"
#include "RuntimePermission.h"
#include "RuntimePermissionManager.h"
class MakeGummyPizzaScene : public ExtensionScene
{
public:
    MakeGummyPizzaScene();
    ~MakeGummyPizzaScene();
    
    MY_SCENE(MakeGummyPizzaScene);
    CREATE_FUNC(MakeGummyPizzaScene);
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    enum{
        eStepMakeCandy,
        eStepCutCandy,
        eStepCutJellew,
        eStepMakeVanlia,
        eStepAddRedCandy,
        eStepAddJellew,
        eStepMakeJellew,
        eStepAddJellewPiece,
    };
    void registerEffectScene(){AudioHelp::getInstance()->registerEffectScene(ClassString(MakeGummyPizzaScene));}
    void removeEffectScene(){AudioHelp::getInstance()->removeEffectScene(ClassString(MakeGummyPizzaScene));}
    
    virtual void onButtonCallback(Button* btn);
    
    void onOvenDoorCallback(OvenNode* pOven);
    void onOvenBakeCallback(OvenNode* pOven,int state);
    void onFreezerDoorCallback(FreezerNode* pFreezer);
    
    void dragNodeTouchBegin(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchMoved(DragNode* pDragNode,Point worldPoint);
    void dragNodeTouchEnded(DragNode* pDragNode,Point worldPoint);
    void dragNodeClicked(DragNode* pDragNode,Point worldPoint);
protected:
    virtual void _initData();
    
    //   *******Make Candy UI
    void _createMakeCandyUI();
    void _showCandyIngredient(int group); //4 ingredient
    
    //    *******Cut Candy UI
    void _showRing();
    
    //    *******Cut Jellew UI
    void _showJellewIngredient(int group); //2 ingredient
    void _showBoard();
    void _removeBoard();
    void _showKnife();
    void _showJellewBoard(int type);
    
    //    *******Make Vanilla Piece UI
    void _createMakeVanillaUI();
    void _showVanillaIngredient(int group); //5 ingredient
    
    //    add candy jellew
    void _showAddCandyIngredient(int group); //2 ingredient
    
    //    ********Make Jellew Piece UI
    void _createMakeJellewPieceUI();
    void _showJellewPieceIngredient(int group); //5 ingredient
    
    //    ******* add piece
    void _showPeiceWithCandy();
    
    //    共同
    void _showShadow(DragNode* pIngredient);
    void _hideShadow(DragNode* pIngredient);
    
    void _showTray();
    void _hideTray();
    void _showSpoon();
    void _hideSpoon();
    
    void _showTableMat();
    void _hideTableMat();
    void _showBowl();
    void _showBowlFrontView();
    void _showPan();
    void _showPanFrontView();
    void _resetBowl();
    void _resetPan();
    
    void _showOilBottle();
    
    void _moveBowlLeft();
    void _moveBowlMiddle();
    
    void _movePanMiddle();
    void _movePanRight();
    //    deel
    void _addCandyIngredient(DragNode* pIngredient);
    void _mix(int step);
    void _putinOven();
    void _putinFreezer();
    void _putoutOven();
    void _putoutFreezer();
    
    void _freeze();
    
    void _addOil();
    void _addBatterPan();
    
    void _cutJellew(int type);
    void _putJellew(int type);
    
    void _addVanlliaIngredient(DragNode* pIngredient);
    void _saveMixture();
    void _addJewllewPieceIngredient(DragNode* pIngredient);
    
    void _addGummyPiece();
    
    //    finish
    void _finishMakeCandyAdd(int step);
    void _finishMix(int step);
    void _finishBake();
    void _finishFreeze();
    
    void _finishAddOil();
    void _finishAddBatter();
    void _finishCutCandy();
    
    void _finishCutJellew(int type);
    void _finishPutJellew(int type);
    
    void _finishMakeVanillaAdd(int step);
    void _finishAddCandy(int type);
    void _finishFreezeGummyPiece(int type);
    
    void _finishMakeJellewPieceAdd(int step);
    
    void _finishAddGummyPiece();
    //    Make Candy Unit
    DragNode* m_pWater;
    DragNode* m_pGelatin;
    DragNode* m_pPowderJellew;
    DragNode* m_pSyrupCorn;
    int m_nCandyAddCount;
    std::vector<Sprite*> m_ringVector;
    
    //    cut candy
    DragNode* m_pRing;
    
    //    cut Jewllew
    Sprite* m_pJellewPlate;
    Sprite* m_pJellewPlateEmpty;
    std::vector<DragNode*> m_JellewGreens;
    std::vector<DragNode*> m_JellewYellows;
    Sprite* m_pBoard;
    DragNode* m_pKnife;
    DragNode* m_pJellewBoard;
    int m_nJellewCutCount;
    int m_nJellewType;
    bool m_bIsJellewOnBoard;
    
    Node* m_pJellewGreenBlock;
    Node* m_pJellewYellowBlock;
    
    //    Make Vanilla
    DragNode* m_pMilk;
    DragNode* m_pSugar;
    DragNode* m_pVanilla;
    
    //    add candy jellew
    Sprite* m_pCandyPlate;
    DragNode* m_pRedCandy;
    DragNode* m_pGreenCandy;
    DragNode* m_pYellowCandy;
    std::vector<Sprite*> m_candyVector;
    
    //    add piece
    DragNode* m_pGummyPiece;
    Sprite* m_pGummyPan;
    
    //    make Jellew yellow piece
    DragNode* m_pPowderJellew2;
    
    //    共同工具
    Sprite* m_pBg;
    OvenNode* m_pOven;
    FreezerNode* m_pFreezer;
    DragNode* m_pSpoon;
    Sprite* m_pTray;
    Sprite* m_pTableMat;
    
    DragNode* m_pBowl; //俯视的碗
    DragNode* m_pBowlFrontView;
    Node* m_pMixture;
    Node* m_pMixtureFrontView;
    
    DragNode* m_pPan; //俯视的盘子
    DragNode* m_pPanFrontView;
    Node* m_pMixturePan;
    
    Sprite* m_pPanFrontViewFront;
    Sprite* m_pMixturePanFrontView;
    
    DragNode* m_pOilBottle;
    
    bool m_bSpoonReady;
    int m_nMixCount;
    int m_nStep; //0 make candy  1 cut candy ...
    //    bool m_bIsBake;
    
    
private:
    void saveImage1();
    void saveImage2();
    void onPermissionGrantedResult(int requestCode,bool bGranted);
};

#endif