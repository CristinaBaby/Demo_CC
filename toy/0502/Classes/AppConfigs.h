//
//  AppConfigs.h
//  AdsPopupDemo
//
//  Created by tang.wei on 13-12-13.
//  Copyright (c) 2013年 tang.wei. All rights reserved.
//

/*************************** MoPub账号设置 **************************/

#define IPHONE_BANNER_ID         @"b40e2935496340deab5bd7e47f6c2d0d"
#define IPHONE_FULLSCREEN_ID     @"7622ee96b577438682d581221a43aee5"

#define IPAD_BANNER_ID           @"732975cd17694d2eb0bc4be75c163a83"
#define IPAD_FULLSCREEN_ID       @"6dbcda3483734da8981d7574561be8d1"

// 方块广告，如果不需要支持可以留空
#define IPHONE_MEDIUM_RECT_ID    @""
#define IPAD_MEDIUM_RECT_ID      @""

// Chartboost账号，如果不需要支持开启chartboost会话，可以留空
#define CHARTBOOST_APP_ID        @"1"
#define CHARTBOOST_APP_SIGNATURE @"1"

// 交叉推广相关
#define IPHONE_FULLSCREEN_ID_CROSSPROMO   @"bd6ec7f2558947b592be367a9ed93006"
#define IPAD_FULLSCREEN_ID_CROSSPROMO     @"ab0b5357a03b462a84260d46db5689e4"

/*************************** Flurry账号 ****************************/
// 如果留空不设置，那么表示不支持Flurry
#define FLURRY_KEY               @"ZG6PZTDGJB69R2NGVDZD"

/*************************** Privacy Page设置 ****************************/
// 如果留空默认为http://www.xxx.com/privacy.html, 其中xxx为bundle id中间部分
#define PRIVACY_PAGE_URL         @""