
package com.android.server.backlight;

import android.content.Context;
import android.os.IBinder;
import android.backlight.IBacklightService;
import android.os.Handler;
import android.os.Message;
import android.os.Trace;
import com.android.server.SystemService;
import android.util.Slog;

public class BacklightService extends SystemService {
	static final String TAG = "BacklightService";
	private final Context mContext;
	private static final boolean DEBUG = true;
	
	public BacklightService(Context context) {
		super(context);
		if (DEBUG){
            Slog.d(TAG, "Build service");
        }
        mContext = context;
        //mNativePointer = init_native();
        publishBinderService(context.BACKLIGHT_SERVICE, mService);
	}
	
    /**
     * Called when service is started by the main system service
     */
    @Override
    public void onStart() {
        if (DEBUG){
            Slog.d(TAG, "Start BacklightService");
        }
        mNativePointer = init_native();
    }
    // Android N7
//    @Override
//    public void onStart() {
//        publishLocalService(BacklightManager.class, mService);
//    }
    
    /**
     * Implementation of AIDL service interface
     */
    private final IBinder mService = new IBacklightService.Stub() {
        /**
         * Implementation of the methods described in AIDL interface
         */
        @Override
        public int getBrightness() {
            if (DEBUG){
                Slog.d(TAG, "Call getBrightness service");
            }
            // TODO: add getLight_native
            return getLight_native(mNativePointer);
//            return 5;
        }
        
        @Override
        public void setBrightness(int value) {
    		synchronized (this) {
    			Slog.d(TAG, "value:" + value);
    			int brightness = value & 0x000000ff;
    			brightness = 0xff000000 | (brightness << 16) | (brightness << 8)
    					| brightness;
    			if (DEBUG){
                    Slog.d(TAG, "setBrightness(" + brightness + ")");
                }
    			try {
    				setLight_native(mNativePointer, brightness);
    			} finally {
    				Trace.traceEnd(Trace.TRACE_TAG_POWER);
    			}
    		}
    	}
    };

	

	private long mNativePointer;
	private static native long init_native();
	private static native void setLight_native(long ptr, int brightness);
	private static native int getLight_native(long ptr);
}
