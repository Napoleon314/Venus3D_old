package com.VenusIE.PowerTest;

import android.os.Bundle;

public class MainActivity extends android.app.NativeActivity
{
    static
    {
        System.loadLibrary("PowerTest");
    }

    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }
}
