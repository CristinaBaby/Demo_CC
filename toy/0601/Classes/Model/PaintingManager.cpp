//
//  PaintingManager.cpp
//  PaintDress
//
//  Created by maxiang on 6/25/15.
//
//

#include "PaintingManager.h"
#include "CSVParse.h"

USING_NS_CC;

static PaintingManager *s_SingletonColouringManager = nullptr;

PaintingManager* PaintingManager::getInstance()
{
    if (!s_SingletonColouringManager)
    {
        s_SingletonColouringManager = new (std::nothrow)(PaintingManager);
        CCASSERT(s_SingletonColouringManager, "ABORT: create PackManager failed");
    }
    
    return s_SingletonColouringManager;
}

PaintingManager::PaintingManager():
_pensType(PensType::PensType_None)
{
    
}

PaintingManager::~PaintingManager()
{
    
}

void PaintingManager::setupPens(const std::string& path)
{
    _pens.clear();
    
    auto parser = CSVParse::create(path.c_str());
    int rows = parser->getRows();
    
    for (int row = 1; row < rows; ++row)
    {
        Pen pen;
        std::string logoName = parser->getData(row, 0);
        pen.penLogo = logoName;
        pen.R = atoi(parser->getData(row, 1));
        pen.G = atoi(parser->getData(row, 2));
        pen.B = atoi(parser->getData(row, 3));
        
        pen.index = row - 1;
        _pens.push_back(pen);
    }
}

void PaintingManager::setupPatterns(const std::string& path)
{
    _patterns.clear();
    
    auto parser = CSVParse::create(path.c_str());
    int rows = parser->getRows();
    
    for (int row = 1; row < rows; ++row)
    {
        Pattern pattern;
        std::string logoName = parser->getData(row, 0);
        std::string patternName = parser->getData(row, 1);

        pattern.patternLogo = logoName;
        pattern.pattern = patternName;
        pattern.index = row - 1;
        
        _patterns.push_back(pattern);
    }
}
