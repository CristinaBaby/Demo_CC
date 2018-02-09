
#ifndef __CrushIceScene__
#define __CrushIceScene__


#include "ExtensionScene.h"
#include "MachineNode.h"
class CrushIceScene : public ExtensionScene
{
public:
    CrushIceScene();
    ~CrushIceScene();
    
    MY_SCENE(CrushIceScene);
    CREATE_FUNC(CrushIceScene);
    
    virtual bool init();
    virtual void onEnter();
    void update( float delta);
protected:
    virtual void _initData();
    
    void _showMachineUnit();
    void _addMachineUnit(int index = 0);
    void _addIce();
    void _produceIce(float dt = 0);
    void _finish();
    
    Sprite *m_pTable;
    
    Vector<Sprite*> m_IceVector;
    MachineNode* m_pMachine;
    int m_nIceShape;
    int updateCount;
};

#endif