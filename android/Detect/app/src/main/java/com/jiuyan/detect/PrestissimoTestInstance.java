package com.jiuyan.detect;

import android.graphics.Bitmap;

/**
 * Created by jxt on 17-10-30.
 */

public class PrestissimoTestInstance {
    public static int CPU_INT_ST = 2;
    public static int CPU_FLOAT_ST = 0;
    public static int CPU_FLOAT_MT = 1;
    public static int CPU_INT_MT = 3;
    public static int GPU = 4;


    public native static String test(int type);
    public native static long createNetInstance(int type, String modelFileName);
    public native static void destroyNetInstance(long instance);
    public native static float[] predict(long instance, Bitmap inputImage);


    static {
        System.loadLibrary("Prestissimo");
        System.loadLibrary("PrestissimoDetectDemo");
    }
}
