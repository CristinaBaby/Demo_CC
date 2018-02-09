//
//  ColoringLineFrameNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/17/15.
//
//

#ifndef __ScribbleDemo_3_2__ColoringLineFrameNode__
#define __ScribbleDemo_3_2__ColoringLineFrameNode__

#include <stdio.h>
#include "ColoringNode.h"
#include "Pattern.h"

class ColoringLineFrameNode;
class ColoringLineFrameLoadDelegate {
public:
    virtual void loadPercent(Node *pNode, float pPercent) = 0;//Pattern加载进度0.0~1.0
    virtual void loadEnded(Node *pNode) = 0;
};

class ColoringLineFrameNode : public ColoringNode, public PatternLoadDelegate {
public:
    ColoringLineFrameNode();
    ~ColoringLineFrameNode();
    bool init(string pLineArtFile);
    virtual void initPattern(string pPatternImage);
    void updatePattern();
    bool setPattern(string pLineFrameFile,int pTolerance = 0);
    Pattern *getPattern(){return _patternNode;}
    
    void setDelegate(ColoringLineFrameLoadDelegate *pDelegate){_delegate = pDelegate;}
    ColoringLineFrameLoadDelegate *getDelegate(){return _delegate;}
    void debugPattern(bool pDebug = true);
    
protected:
    virtual void loadPercent(Pattern *pNode, float pPercent);
    virtual void loadEnded(Pattern *pNode);
    
private:
    Pattern *_patternNode;
    ColoringLineFrameLoadDelegate *_delegate;
    bool _debugPattern;
};
#endif /* defined(__ScribbleDemo_3_2__ColoringLineFrameNode__) */
