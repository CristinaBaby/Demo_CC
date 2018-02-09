//
//  PaintingManager.h
//  PaintDress
//
//  Created by maxiang on 6/25/15.
//
//

#ifndef __PaintDress__PaintingManager__
#define __PaintDress__PaintingManager__

#include "cocos2d.h"

struct Pen
{
    std::string penLogo;
    int R;
    int G;
    int B;
    int index;
};

struct Pattern
{
    std::string patternLogo;
    std::string pattern;
    int index;
};

enum class PensType
{
    PensType_None = 0,
    PensType_Pen,
    PensType_Patter,
};

class PaintingManager
{
public:
    static PaintingManager* getInstance();
    
    PaintingManager();
    virtual ~PaintingManager();
    
    void setupPens(const std::string& path);
    void setupPatterns(const std::string& path);
    
    void setCurrentPensType(PensType type){_pensType = type;};
    PensType getCurrentPensType(){return _pensType;};
    
    //is locked pattern pen?
    bool hasUnlockedPatternPen(){return _hasUnlockedPatternPen;};
    
    std::vector<Pen>& getPens(){return _pens;};
    std::vector<Pattern>& getPatterns(){return _patterns;};
    
    //if user finish purchase, unlock pattern pen
    void unlockPatternPen();
    
protected:
    void setupPens(const std::string& path, std::vector<Pen>& pens);
    
protected:
    std::vector<Pen> _pens;
    std::vector<Pattern> _patterns;
    
    bool _hasUnlockedPatternPen;
    
    PensType _pensType;
};


#endif /* defined(__PaintDress__PaintingManager__) */
