//
//  Test.h
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#ifndef __ColorBook__Test__
#define __ColorBook__Test__

#include "cocos2d.h"
#include <random>
#include "UIHeader.h"

typedef std::function<int()> randomFunction;

class Test
{
public:
    Test();
    virtual ~Test();
    
    void run();
    void random();
    
protected:
    bool    _canChange;
    
    vector<int> _collect;
    
    std::random_device rd;                      //随机设备
    std::mt19937 gen;                           //随机种子
    std::uniform_int_distribution <size_t> u;   //随机分布
    
    randomFunction _random;
    
    //std::default_random_engine generator(time(NULL));
    //std::uniform_int_distribution<int> dis(0,100);
    //auto dice= std::bind(dis,generator);
    
//    std::mt19937 eng;
//    std::uniform_int_distribution<size_t> axis_dist;
//    std::uniform_real_distribution<float> fraction_dist;
};

#endif /* defined(__ColorBook__Test__) */
