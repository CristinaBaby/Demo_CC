//
//  SVGSource.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/21/15.
//
//

#include "SVGSource.h"
#include "SVGFill.h"
//CC_SYNTHESIZE(Rect, _contentRect, ContentRect);
//CC_SYNTHESIZE(Size, _contentSize, ContentSize);

void SVGSource::setTessellationIterations(unsigned int pNumberOfTesselationIterations){
    vgSeti(VG_TESSELLATION_ITERATIONS_MNK, pNumberOfTesselationIterations);
}

bool SVGSource::init(){
    isOptimized_ = false;
    svg_ = std::static_pointer_cast<MonkSVG::OpenVG_SVGHandler>(MonkSVG::OpenVG_SVGHandler::create());
    return true;
}

SVGSource *SVGSource::initWithData(Data *pData, SVGFill::FillState pState){
    if (!init()) {
        return nullptr;
    }
//    MonkSVG::SVG parser;
    SVGFill parser(pState);
    parser.initialize(svg_);
    parser.read((char *)pData->getBytes());
    
    contentRect_ = Rect(svg_->minX(), svg_->minY(), svg_->width(), svg_->height());
    contentSize_ = Size(svg_->width(), svg_->height());
    return this;
}

SVGSource *SVGSource::initWithFile(string pFile, SVGFill::FillState pState){
    Data lData = FileUtils::getInstance()->getDataFromFile(pFile);
    return this->initWithData(&lData, pState);
}

void SVGSource::draw(){
    this->optimize();
    svg_->draw();
}

void SVGSource::optimize(){
    if (!isOptimized_) {
        
        VGfloat matrix[9];
        vgGetMatrix(matrix);
        
        vgLoadIdentity();
        svg_->optimize();
        vgLoadMatrix(matrix);
        
        isOptimized_ = true;
        
    }
}