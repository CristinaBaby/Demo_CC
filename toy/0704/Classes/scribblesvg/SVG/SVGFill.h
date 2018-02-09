//
//  SVGFill.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/9/2.
//
//

#ifndef __ScribbleDemo_3_2__SVGFill__
#define __ScribbleDemo_3_2__SVGFill__

#include <stdio.h>
#include <vector>
#include "mkSVG.h"

using namespace std;

class SVGFill : public MonkSVG::SVG {
public:
    enum class FillState{
        eNone,
        eAuto,
        eSVGFile
    };
public:
    SVGFill(FillState pState/* = FillState::eAuto*/);
    ~SVGFill();
    static inline void setPatternColorStep(int pStep = 5){_patternColorStep = pStep;}
protected:
    void fill(string pColor);
    virtual void handle_path( TiXmlElement* pathElement );
    void parse_path_style( string& ps ) override;
    void handle_general_parameter( TiXmlElement* pathElement ) override;
private:
    static int _patternColorStep;
    int _colorIndex;
    vector<unsigned int> _colors;
    FillState _fillState;
};
#endif /* defined(__ScribbleDemo_3_2__SVGFill__) */
