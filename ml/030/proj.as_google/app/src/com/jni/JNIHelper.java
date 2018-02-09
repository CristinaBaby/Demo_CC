package com.jni;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;

/**
 * Created by wuguiling on 17/3/8.
 */
public class JNIHelper {
    static  int androidPlatform;
    private static Activity sActivity = null;
    public static void init(final Activity activity) {
        sActivity = activity;
    }
    static public void setAndroidPlatformCode(int index){
        androidPlatform = index;
    }
    static public int getAndroidPlatformCode(){
        return androidPlatform;
    }
    static public void openUrl(String url){
        if(sActivity==null){
            return;
        }
        try {
            Intent i = new Intent(Intent.ACTION_VIEW);
            i.setData(Uri.parse(url));
            sActivity.startActivity(i);
        } catch (Exception e) {
        }
    }
}
