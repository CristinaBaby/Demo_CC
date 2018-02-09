//
//  AnimalPos.h
//  ANIM3203
//
//  Created by dengcheng on 15/9/24.
//
//

#ifndef ANIM3203_AnimalPos_h
#define ANIM3203_AnimalPos_h

#include "cocos2d.h"

static const Vec2 g_sAnimalDecorationPos[3][5] =
{
    //cat dog
    {Vec2::ZERO, Vec2(153, 281), Vec2(229, 302), Vec2(153, 281), Vec2(217,308)},//hat
    {Vec2::ZERO, Vec2(170,179), Vec2(233, 214), Vec2(170,179), Vec2(233, 214)},//glass
    {Vec2::ZERO, Vec2(247, 238), Vec2(234, 296), Vec2(237, 240), Vec2(234, 296)}//necklace
};

static const Vec2 g_sAnimalClothPos[2][5] =
{
    {Vec2::ZERO, Vec2(238, 192), Vec2(234, 226), Vec2(237, 208), Vec2(228, 235)},//cloth
    {Vec2::ZERO, Vec2(245, 54), Vec2(235, 56), Vec2(239, 73), Vec2(222, 64)}//shoe
};

static const  Vec2 g_sAnimalHeadPos[] = {
    Vec2::ZERO,
    Vec2(243, 233),
    Vec2(234, 282),
    Vec2(241, 246),
    Vec2(227, 281)
};

static const Vec2 g_sAnimalEyesPos[] = {
    Vec2::ZERO,
    Vec2(176, 176),
    Vec2(233, 182),
    Vec2(173, 175),
    Vec2(224, 175)
};

//234, 176
//225 176
static const Vec2 g_sAnimalMousePos[] = {
    Vec2::ZERO,
    Vec2(176, 48),
    Vec2(236, 60),
    Vec2(174, 89),
    Vec2(220, 72)
};

static const Vec2 g_sAnimalTailPos[] = {
    Vec2::ZERO,
    Vec2(-46, -143),
    Vec2(122, -48),
    Vec2(-57, -134),
    Vec2(104, -85)
};

static const Vec2 g_sAnimalFingernailPos[5][13] = {
    {Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO},
    {Vec2::ZERO, Vec2(382, 46), Vec2(380, 33), Vec2(362, 27), Vec2(346, 9), Vec2(317, 8), Vec2(284, 7), Vec2(217, 5), Vec2(176, 4), Vec2(146, 6), Vec2(131, 30), Vec2(115, 31), Vec2(109, 44)},
    {Vec2::ZERO, Vec2(378, 70), Vec2(361, 63), Vec2(337, 59), Vec2(334, 17), Vec2(303, 9), Vec2(267, 7), Vec2(204, 4), Vec2(170, 4), Vec2(138, 9), Vec2(136, 59), Vec2(116, 59), Vec2(98, 68)},
    {Vec2::ZERO, Vec2(371, 64), Vec2(361.5, 51), Vec2(340, 43), Vec2(334, 15), Vec2(305, 7), Vec2(271, 8), Vec2(211, 6), Vec2(181, 4), Vec2(153, 7), Vec2(142, 42), Vec2(123, 47), Vec2(112, 65)},
    {Vec2::ZERO, Vec2(364, 69), Vec2(347, 64), Vec2(327, 60), Vec2(322, 16), Vec2(292, 7), Vec2(255, 7), Vec2(193, 6), Vec2(156, 4), Vec2(124, 9), Vec2(120, 59), Vec2(102, 60), Vec2(84, 68)}
};

/*
 1:2(4)---1(4)---1(2)--2(2)
 2:4(2)---3(2)---2(2)--1(2)---6(2)--5(2)
 3:1-8
 4:2(4)--1(4)--3(2)--4(2)
 */
static const Vec2 g_sAnimalFurPos[5][13] = {
    {Vec2::ZERO, Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO,Vec2::ZERO},
    {Vec2::ZERO, Vec2(140, 96), Vec2(119, 157), Vec2(180, 115), Vec2(167, 178), Vec2(378, 159), Vec2(353, 108), Vec2(296, 103), Vec2(320, 164), Vec2(310, 149), Vec2(241, 257), Vec2(107, 257), Vec2(57, 115)},
    {Vec2::ZERO, Vec2(287, 266), Vec2(308, 109), Vec2(192, 237), Vec2(164, 140), Vec2(341, 282), Vec2(332, 210), Vec2(143, 291), Vec2(130, 240), Vec2(429, 181), Vec2(400, 103), Vec2(42, 163), Vec2(75, 96)},
    {Vec2::ZERO, Vec2(121, 167), Vec2(368, 189), Vec2(148, 214), Vec2(349, 234), Vec2(351, 139), Vec2(212, 142), Vec2(158, 285), Vec2(298, 185), Vec2(301, 122), Vec2(276, 85), Vec2(79, 77), Vec2(58, 108)},
    {Vec2::ZERO, Vec2(347, 176), Vec2(318, 141), Vec2(276, 264), Vec2(314, 273), Vec2(193, 167), Vec2(174, 262), Vec2(128, 165), Vec2(124, 265), Vec2(109, 85), Vec2(111, 145), Vec2(327, 148), Vec2(333, 94)}
};

static const Vec2 g_sAnimalOrdure[5][3] = {
    {Vec2::ZERO, Vec2::ZERO, Vec2::ZERO},
    {Vec2::ZERO, Vec2(249, 150), Vec2(171, 183)},
    {Vec2::ZERO, Vec2(232, 188), Vec2(233, 171)},
    {Vec2::ZERO, Vec2(244, 163), Vec2(169, 162)},
    {Vec2::ZERO, Vec2(220, 201), Vec2(223, 176)}
};

#endif
