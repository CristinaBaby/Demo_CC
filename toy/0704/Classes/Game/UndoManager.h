//
//  UndoManager.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__UndoManager__
#define __ColorBook__UndoManager__

#include "UITools.h"

#define xUndo  (UndoManager::getInstance())

class FillNode;

class UndoManager : public Node
{
public:
    static UndoManager* getInstance();
    void destroyInstance();
    
    void init(FillNode * fillSVGNode);
    void undo();
    void painted();
    
public:
    void update(float dt);
    
protected:
    UndoManager();
    void record();
    string getSaveFileName();

protected:
    int              _recordIndex = -1;
    int              _recordCount = 0;
    FillNode *      _fillSVGNode;
    
    float            _acc = 0;
    bool             _painted = false;
};

#endif /* defined(__ColorBook__UndoManager__) */
