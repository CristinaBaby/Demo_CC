//
//  SVGFill.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/9/2.
//
//

#include "SVGFill.h"

#include "tinyxml.h"
#include <map>
#include <iterator>
#include <regex>

using namespace std;
int SVGFill::_patternColorStep = 5;
SVGFill::SVGFill(FillState pState/*FillState::eAuto*/){
    _fillState = pState;
    _colorIndex = 0;
    if(pState == FillState::eAuto){
        const int a = 255;
        for (int b = 10; b <= 250; b += _patternColorStep) {
            for (int g = 10; g <= 250; g += _patternColorStep) {
                for (int r = 10; r <= 250; r += _patternColorStep) {
                    unsigned int color = 0;
                    color = (r << 24 | (g << 16) | (b << 8) | (a));
                    _colors.push_back(color);
                }
            }
        }
    }
}

SVGFill::~SVGFill(){
    
}

void SVGFill::fill(string pColor){
    switch (_fillState) {
        case FillState::eSVGFile:
            if( pColor != "none" )
                _handler->onPathFillColor( string_hex_color_to_uint( pColor ) );
            break;
        case FillState::eNone:{
//            string lBlackColor = "#000000";
//            if (pColor == lBlackColor) {
//                _handler->onPathFillColor( string_hex_color_to_uint( lBlackColor ) );
//            }
            break;
        }
        case FillState::eAuto:{
            if( pColor == "" || pColor == "#000000")
                _handler->onPathFillColor( 0 );
            else{
                _handler->onPathFillColor( _colors.at(_colorIndex++) );
            }
            
            break;
        }
        default:
            break;
    }
}

void SVGFill::handle_path( TiXmlElement* pathElement ) {
    
    //		string id_;
    //		if ( pathElement->QueryStringAttribute( "id", &id_) == TIXML_SUCCESS ) {
    //			//_handler->onId( id_ );
    //			cout << "path: " << id_ << endl;
    //		}
    
    if (_fillState == SVGFill::FillState::eAuto) {
        SVG::handle_path(pathElement);
    }
    else{
        _handler->onPathBegin();
        
        
        string fill;
        pathElement->QueryStringAttribute( "fill", &fill );
        if (fill.length() <= 0) {
            string d;
            if ( pathElement->QueryStringAttribute( "d", &d ) == TIXML_SUCCESS ) {
                parse_path_d( d );
            }
            
            handle_general_parameter( pathElement );
        }
        
        _handler->onPathEnd();
    }
}

//解析类似 "fill:#ffffff;stroke:#000000;stroke-width:0.17200001" 的字符串
map<string, string> parse_key_values(string& ps)
{
    map< string, string > key_values;
    size_t start = 0;
    size_t semicolon;
    
    while (true)
    {
        semicolon = ps.find(";", start);
        if(semicolon == string::npos)
        {
            if (start < ps.length())
                semicolon = ps.length();
            else
                break;
        }
        
        //将前面的部分取出来
        auto term = ps.substr(start, semicolon - start);
        auto colon = term.find(":");
        
        string first = term.substr(0, colon);
        string second = term.substr(colon + 1, semicolon - (colon + 1));
        
        key_values.insert(std::make_pair(first, second));
        start = semicolon + 1;
    }
    
    return std::move(key_values);
}

void SVGFill::parse_path_style( string& ps ){
    map< string, string > style_key_values = parse_key_values(ps);
    
    map<string, string>::iterator kv = style_key_values.find( string("fill") );
    if ( kv != style_key_values.end() ) {
        this->fill(kv->second);
    }
    
    kv = style_key_values.find( "stroke" );
    if ( kv != style_key_values.end() ) {
        if( kv->second != "none" )
            _handler->onPathStrokeColor( string_hex_color_to_uint( kv->second ) );
    }
    
    kv = style_key_values.find( "stroke-width" );
    if ( kv != style_key_values.end() ) {
        float width = FillState::eAuto == _fillState ? 0 : atof( kv->second.c_str() );
        _handler->onPathStrokeWidth( width );
    }
    
    kv = style_key_values.find( "fill-rule" );
    if ( kv != style_key_values.end() ) {
        _handler->onPathFillRule( kv->second );
    }
    
    kv = style_key_values.find( "fill-opacity" );
    if ( kv != style_key_values.end() ) {
        float o = atof( kv->second.c_str() );
        _handler->onPathFillOpacity( o );
    }
    
    kv = style_key_values.find( "opacity" );
    if ( kv != style_key_values.end() ) {
        float o = atof( kv->second.c_str() );
        _handler->onPathFillOpacity( o );
        // ?? TODO: stroke Opacity???
    }
    
    
    kv = style_key_values.find( "stroke-opacity" );
    if ( kv != style_key_values.end() ) {
        float o = atof( kv->second.c_str() );
        _handler->onPathStrokeOpacity( o );
    }
}


void SVGFill::handle_general_parameter( TiXmlElement* pathElement ) {
    string fill;
    if ( pathElement->QueryStringAttribute( "fill", &fill ) == TIXML_SUCCESS ) {
//        printf("fill %s\n", fill.c_str());
//        _handler->onPathFillColor( string_hex_color_to_uint( fill ) );
        this->fill(fill);
    }
    
    
    string stroke;
    if ( pathElement->QueryStringAttribute( "stroke", &stroke) == TIXML_SUCCESS ) {
        _handler->onPathStrokeColor( string_hex_color_to_uint( stroke ) );
    }
    
    string stroke_width;
    if ( pathElement->QueryStringAttribute( "stroke-width", &stroke_width) == TIXML_SUCCESS ) {
        float width = FillState::eAuto == _fillState ? 0 : atof( stroke_width.c_str() );
        _handler->onPathStrokeWidth( width );
    }
    
    string style;
    if ( pathElement->QueryStringAttribute( "style", &style) == TIXML_SUCCESS ) {
        parse_path_style( style );
    }
    
    string transform;
    if ( pathElement->QueryStringAttribute( "transform", &transform) == TIXML_SUCCESS ) {
        parse_path_transform( transform );
    }
    string id_;
    if ( pathElement->QueryStringAttribute( "id", &id_) == TIXML_SUCCESS ) {
        _handler->onId( id_ );
        //cout << id_ << endl;
    }
    
    string opacity;
    if ( pathElement->QueryStringAttribute( "opacity", &opacity) == TIXML_SUCCESS ) {
        float o = atof( opacity.c_str() );
        _handler->onPathFillOpacity( o );
        // TODO: ??? stroke opacity???
    }
    if ( pathElement->QueryStringAttribute( "fill-opacity", &opacity) == TIXML_SUCCESS ) {
        float o = atof( opacity.c_str() );
        _handler->onPathFillOpacity( o );
    }
    
    string fillrule;
    if ( pathElement->QueryStringAttribute( "fill-rule", &fillrule) == TIXML_SUCCESS ) {
        _handler->onPathFillRule( fillrule );		
    }
    
}