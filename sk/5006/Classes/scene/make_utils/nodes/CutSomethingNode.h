//
//  CutSomethingNode.h
//  BHUG1064
//
//  Created by maxiang on 5/31/16.
//
//

#ifndef CutSomethingNode_h
#define CutSomethingNode_h

#include "MakeBaseNode.h"

class CutSomethingNode : public MakeBaseNode
{
public:
    static CutSomethingNode* create(std::vector<Vec2> cutterOffsetVector,
                                    const std::string& fileFormat,
                                    int startIndex,
                                    int endIndex,
                                    bool LtoR);
    bool init(std::vector<Vec2> cutterOffsetVector,
              const std::string& fileFormat,
              int startIndex,
              int endIndex,
              bool LtoR);
    
    //设置刀子
    void setKnife(const std::string& file);
    
    //开始切
    void beginAutoCut();
    void beginDragCut();
    
    //进一步设置刀子的回调，有时刀子的zorder需要微调
    std::function<void(int partIndex, Sprite *knife)> setupKnifeCallback = nullptr;
    
    //切完回调
    std::function<void()> finishCutCallback = nullptr;
    std::function<void()> closeBtnClicked = nullptr;
    
    //默认tag为索引值
    Sprite* getContent(int tag);

    void setKnifeZorder(int zorder);
    
    void initBackground();
public:
    CutSomethingNode():
    _partIndex(-1),
    _knife(nullptr),
    _closeBtn(nullptr)
    {};
    virtual ~CutSomethingNode();
    
protected:
    void setupCutter();
    void showGuideLine(const Vec2& position);
    void removeGuideLine();
    void finishCutOnePart();
protected:
    int _startIndex;
    int _endIndex;
    int _partIndex;
    int _partZorder;
    int _count;
    Sprite *_knife;
    std::vector<Vec2> _cutterOffsetVector;
    Button* _closeBtn;
};

#endif /* CutSomethingNode_h */
