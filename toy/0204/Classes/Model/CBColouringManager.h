//
//  CBColouringManager.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__CBColouringManager__
#define __ColorBook__CBColouringManager__

#include "cocos2d.h"
#include "CBAppGlobal.h"

CB_BEGIN_NAMESPACE

struct Pen
{
    std::string penLogo;
    GLuint R;
    GLuint G;
    GLuint B;
    GLuint A;
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
    PensType_Pen1,
    PensType_Pen2,
    PensType_Patter,
};

class ColouringManager
{
public:
    static ColouringManager* getInstance();
    
    ColouringManager();
    virtual ~ColouringManager();
    
    void setupPens1(const std::string& path);
    void setupPens2(const std::string& path);
    void setupPatterns(const std::string& path);
    
    void setCurrentPensType(PensType type){_pensType = type;};
    PensType getCurrentPensType(){return _pensType;};
    
    //is locked pattern pen?
    bool hasUnlockedPatternPen(){return _hasUnlockedPatternPen;};

    std::vector<Pen> *getPens1(){return &_pens1;};
    std::vector<Pen> *getPens2(){return &_pens2;};
    std::vector<Pattern> *getPatterns(){return &_patterns;};
    
    //if user finish purchase, unlock pattern pen
    void unlockPatternPen();
    
protected:
    void setupPens(const std::string& path, std::vector<Pen> *pens);

protected:
    std::vector<Pen> _pens1;
    std::vector<Pen> _pens2;
    std::vector<Pattern> _patterns;
    

    bool _hasUnlockedPatternPen;

    PensType _pensType;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBColouringManager__) */
