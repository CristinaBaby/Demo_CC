package com.make.base;

import com.common.android.LaunchActivity;
import com.common.android.utils.Utils;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.provider.Settings;
import android.util.Log;

public class MyJinHelp {
	public static String getMoreGameUrl() {
	    LaunchActivity context = (LaunchActivity) LaunchActivity.getInstance();
		 StringBuilder Cache_StringBuilder = new StringBuilder();
		Cache_StringBuilder.delete(0, Cache_StringBuilder.length());
		PackageInfo info = null;
		PackageManager pm = context.getApplicationContext().getPackageManager();
		try {
			info = pm.getPackageInfo(context.getPackageName(), 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		if (info == null)
			return null;
		Cache_StringBuilder.append(info.packageName);
		int last = Cache_StringBuilder.lastIndexOf(".");

		String appName = Cache_StringBuilder.substring(last + 1);
		String model = Build.MODEL.replace(" ", "_");

		Cache_StringBuilder.delete(0, Cache_StringBuilder.length());

		Cache_StringBuilder.append(Utils.getMetaData(context, "studioService")).append("/more/").append(appName)
				.append("/").append(context.getResources().getConfiguration().locale.getLanguage()).append("/")
				.append(Settings.System.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID)).append("/?model=")
				.append(model).append("&sysVer=").append(Build.VERSION.RELEASE).append("&bundleVer=")
				.append(info.versionName).append("&bundleID=").append(info.packageName).append("&platform=")
				.append(context.getPlatformCode()).append("&format=json");

		if (context.getDebugMode())
			Log.i("", "index url: " + Cache_StringBuilder.toString());


			Log.i("", "index url: " + Cache_StringBuilder.toString());
		return Cache_StringBuilder.toString();

	}
}


