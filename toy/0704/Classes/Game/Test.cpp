//
//  Test.cpp
//  ColorBook
//
//  Created by maxiang on 4/24/15.
//
//

#include "Test.h"
#include "GameScene.h"

Test::Test()
{
    for(int i = 0; i < 10; i++)
    {
        _collect.push_back(0);
    }
    
    _collect.at(9) += 1;
    
//    std::default_random_engine engine(std::random_device{}());
//    std::uniform_int_distribution <> distribution(0, 9);
//    _random = std::bind(distribution, engine);
}

Test::~Test()
{
}

void Test::run()
{
    time_t  now;
    time(&now);
    
    long old = now;
    
    
    for (int i = 0; i < 1000000; i ++) {
        //_collect.at(_random()) += 1;
        random();
    }
    
    for(int i = 0; i < 10; i++)
    {
        log("%d", _collect.at(i));
    }
    
    
    time(&now);
    old = now - old;
    
    log("time %ld", old);
    //gen = ;
    
    //u(0, 2);
}

void Test::random()
{
    std::default_random_engine engine(std::random_device{}());
    std::uniform_int_distribution <> distribution(0, 9);
    //_random = std::bind(distribution, engine);
    
    int i = distribution(engine);
    
    
    _collect.at(i) += 1;
}
