//
//  AppConfigs.h
//  AdsPopupDemo
//
//  Created by tang.wei on 13-12-13.
//  Copyright (c) 2013年 tang.wei. All rights reserved.
//

/*************************** MoPub账号设置 **************************/

#define IPHONE_BANNER_ID         @"cc1a51b52bbf4356b93224124150aac9"
#define IPHONE_FULLSCREEN_ID     @"e234228414c0456985e4f305334b648c"

#define IPAD_BANNER_ID           @"c4c6c96b05d44e8fa2546d408798b8ef"
#define IPAD_FULLSCREEN_ID       @"2a86746506de40b0b2c3a7b056303d7b"

// 方块广告，如果不需要支持可以留空
#define IPHONE_MEDIUM_RECT_ID    @""
#define IPAD_MEDIUM_RECT_ID      @""

// Chartboost账号，如果不需要支持开启chartboost会话，可以留空
#define CHARTBOOST_APP_ID        @""
#define CHARTBOOST_APP_SIGNATURE @""

#define IPAD_FULLSCREEN_ID_CROSSPROMO   @"ff7a48b2a8604f3091b6860eaf6ce7fe"
#define IPHONE_FULLSCREEN_ID_CROSSPROMO @"bc6afd09678b4f7faadd50f14f36a084"

#define IPHONE_FULLSCREEN_ID_REWARD   @""
#define IPAD_FULLSCREEN_ID_REWARD @""


/*************************** Flurry账号 ****************************/
// 如果留空不设置，那么表示不支持Flurry
#define FLURRY_KEY               @"7VQQTZSPTQDVYHW6K2G8"

/*************************** Privacy Page设置 ****************************/
// 如果留空默认为http://www.xxx.com/privacy.html, 其中xxx为bundle id中间部分
#define PRIVACY_PAGE_URL         @"http://www.absoluteappsmedia.com/privacy.html"