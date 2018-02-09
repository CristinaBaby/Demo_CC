/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package net.makerlabs.android_kidsicecreammaker;

import org.cocos2dx.lib.Cocos2dxActivity;

//import com.common.android.IapResult;
//import com.common.android.OnSetupListener;
//import com.common.android.PlatformCode;
//import com.common.android.jni.MoreGamesActivityForJNI;
//import com.common.android.analyticscenter.AnalyticsCenterJni;
//import com.common.android.analyticscenter.AnalyticsCenter;
//import com.common.android.utils.Utils;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;

import com.common.android.InternalInterfaceManager;
import com.common.android.ads.AdsManager;
import com.common.android.analyticscenter.AnalyticsCenter;
import com.common.android.analyticscenter.AnalyticsCenterJni;
import com.common.android.analyticscenter.utils.AppPlateform;
import com.common.android.iap.IapResult;
import com.common.android.iap.OnSetupListener;
import com.common.android.moregame.MoreGamesJni;
import com.common.android.utils.PlatformCode;
import com.jni.JNIHelper;

public class AppActivity extends Cocos2dxActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if (getPlatformCode() == PlatformCode.AMAZON)
		{
			setAmazonIAP();
		}else if (getPlatformCode() == PlatformCode.GOOGLEPLAY)
		{
			setGooglePlayIAP();
		}

/*==============最新广告加初始化方式，根据自己用到的广告选择加载。==============*/
		AppPlateform.setPlateform(AppPlateform.COCOS2DX);

		AdsManager.getInstance(this).setup(AdsManager.AD_BANNER
						// | AdsManager.AD_CORSSPROMO //不再支持CBCP
						| AdsManager.AD_INTERSTITIAL
						| AdsManager.AD_REWARDED,
				getDebugMode());

		MoreGamesJni.setup(this, getPlatformCode());

		AnalyticsCenterJni.getInstace().setDebugMode(getDebugMode());
		AnalyticsCenterJni.getInstace().setPlateform(getPlatformCode());
		//财务统计相关,参数:用到哪些统计就给哪些
		AnalyticsCenterJni.getInstace().startSession(this,
				//AnalyticsCenter.ANALYTICS    //not supported
				AnalyticsCenter.FLURRY
						| AnalyticsCenter.GOOGLE
						| AnalyticsCenter.FIREBASE);


		//init JNI enviroment
		setupNativeEnvironment();

		InternalInterfaceManager.getInstance(this).setNeedRateUsDlg(false);

		JNIHelper.init(this);
	}

	@Override
	public int getPlatformCode() {
		try{
			ApplicationInfo e = this.getPackageManager().getApplicationInfo(this.getPackageName(), 128);
			Bundle bundle = e.metaData;
			int code = bundle.getInt("PlatformCode");
			return code;

		} catch (PackageManager.NameNotFoundException e1) {
			e1.printStackTrace();
		}
		return PlatformCode.GOOGLEPLAY;
	}


	@Override
	public boolean getDebugMode() {
		return false;
	}

	@Override
	public boolean enableEvent() {
		return !getDebugMode();
	}

	public void setGooglePlayIAP()
	{
/*====================Google IAP setting begin====================*/
//Google版没有用到IAP就删除以下代码
//		if (getPlatformCode() == PlatformCode.GOOGLEPLAY) {
//			mIAPHelper.setPublicKey("");
//		}
//		mIAPHelper.setDebugMode(getDebugMode());
//		mIAPHelper.setSetupBillingListener(new OnSetupListener() {
//
//			@Override
//			public void onIabSetupFinished(IapResult result) {
//				Log.i("SSCIAPHelper", "----------------SSCIAPHelper" + result.toString());
//				if (result.isSuccess()) {
//					if (getDebugMode())
//						Log.i("", "=======Billing support======");
//				} else {
//					if (getDebugMode())
//						Log.i("SSCIAPHelper", "=======Billing unsupported======");
//				}
//			}
//		});
//
//		mIAPHelper.onStartSetupBilling();
//
//		//设置必需的环境，该方法必须在moki各sdk初始化后调用
//		setupNativeEnvironment();
/*====================Google IAP setting end====================*/
	}

	public void setAmazonIAP()
	{
/*====================Amazon IAP setting begin====================*/
		//IAP设置成功与否的回调监听，和IAP使用没关系
//		mIAPHelper.setSetupBillingListener(new OnSetupListener() {
//
//			@Override
//			public void onIabSetupFinished(IapResult result) {
//
//				if (result.isSuccess()) {
//					if (getDebugMode())
//						Log.i("", "=======Billing support======");
//				} else {
//					if (getDebugMode())
//						Log.i("", "=======Billing unsupported======");
//				}
//			}
//		});
//		mIAPHelper.setDebugMode(getDebugMode());
//		//有IAP必须调用
//		mIAPHelper.onStartSetupBilling();
///*====================Amazon IAP setting end====================*/
	}
}
