package com.android.backlight;

import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.os.RemoteException;
import android.provider.Settings;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.util.Log;
import android.backlight.BacklightManager;

public class MainActivity extends Activity {
    private Context mContext;
    private static final String TAG = "Backlight";
    private RelativeLayout mRelativeLayout;
    private SeekBar mSeekBar;
    private TextView mTextView;
    private BacklightManager blManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // Request window feature action bar
        requestWindowFeature(Window.FEATURE_ACTION_BAR);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Get the application context
        mContext = getApplicationContext();

        // Change the action bar color
//        getSupportActionBar().setBackgroundDrawable(
//                new ColorDrawable(Color.parseColor("#FF246AB4"))
//        );

        // Get the widgets reference from XML layout
        mRelativeLayout = (RelativeLayout) findViewById(R.id.rl);
        mSeekBar = (SeekBar) findViewById(R.id.seek_bar);
        mTextView = (TextView) findViewById(R.id.tv);
//        final IBacklightService bl = IBacklightService.Stub.asInterface(ServiceManager.getService("backlight"));
        blManager = (BacklightManager)getSystemService(Context.BACKLIGHT_SERVICE);
        try {
            // Set the SeekBar initial progress from screen current brightness
            //int brightness = getScreenBrightness();
            int brightness = blManager.getBrightness(); // use our definition
            mSeekBar.setProgress(brightness);
            mTextView.setText("Screen Brightness : " + brightness);

            // Set a SeekBar change listener
            mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                    // Display the current progress of SeekBar
                    mTextView.setText("Screen Brightness : " + i);

                    // Change the screen brightness
                    //setScreenBrightness(i);
                    
                    blManager.setBrightness(i);
                }

                @Override
                public void onStartTrackingTouch(SeekBar seekBar) {

                }

                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {

                }
            });
        }
        catch (Exception e) {
            Log.d(TAG, "FAILED to call service");
            e.printStackTrace();
        }
    }

    // Change the screen brightness
    public void setScreenBrightness(int brightnessValue){
        /*
            public abstract ContentResolver getContentResolver ()
                Return a ContentResolver instance for your application's package.
        */
        /*
            Settings
                The Settings provider contains global system-level device preferences.

            Settings.System
                System settings, containing miscellaneous system preferences. This table holds
                simple name/value pairs. There are convenience functions for accessing
                individual settings entries.
        */
        /*
            public static final String SCREEN_BRIGHTNESS
                The screen backlight brightness between 0 and 255.
                Constant Value: "screen_brightness"
        */
        /*
            public static boolean putInt (ContentResolver cr, String name, int value)
                Convenience function for updating a single settings value as an integer. This will
                either create a new entry in the table if the given name does not exist, or modify
                the value of the existing row with that name. Note that internally setting values
                are always stored as strings, so this function converts the given value to a
                string before storing it.

            Parameters
                cr : The ContentResolver to access.
                name : The name of the setting to modify.
                value : The new value for the setting.
            Returns
                true : if the value was set, false on database errors
        */

        // Make sure brightness value between 0 to 255
        if (Settings.System.canWrite(mContext)) {
            if(brightnessValue >= 0 && brightnessValue <= 255){
                if (false) {
                    Settings.System.putInt(
                            mContext.getContentResolver(),
                            Settings.System.SCREEN_BRIGHTNESS,
                            brightnessValue
                    );
                } else {

                }
            }
        } else {
            Intent intent = new Intent(android.provider.Settings.ACTION_MANAGE_WRITE_SETTINGS);
            intent.setData(Uri.parse("package:" + this.getPackageName()));
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivity(intent);
        }
    }

    // Get the screen current brightness
    protected int getScreenBrightness(){
        /*
            public static int getInt (ContentResolver cr, String name, int def)
                Convenience function for retrieving a single system settings value as an integer.
                Note that internally setting values are always stored as strings; this function
                converts the string to an integer for you. The default value will be returned
                if the setting is not defined or not an integer.

            Parameters
                cr : The ContentResolver to access.
                name : The name of the setting to retrieve.
                def : Value to return if the setting is not defined.
            Returns
                The setting's current value, or 'def' if it is not defined or not a valid integer.
        */
        int brightnessValue = Settings.System.getInt(
                mContext.getContentResolver(),
                Settings.System.SCREEN_BRIGHTNESS,
                0
        );
        return brightnessValue;
    }
}
