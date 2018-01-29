package android.backlight;

import android.backlight.IBacklightService;
import android.util.AndroidException;
import android.util.Log;
import android.util.Slog;
import android.content.Context;
import android.os.RemoteException;

public class BacklightManager {
	private final Context mContext;
    private final IBacklightService mService;
    static final String TAG = "BacklightManager";
    /**
     * Initialize the remote service and execution context
     * ContextImpl will build this manager object and provide the
     * remote service stub as parameter
     * 
     * @param ctx
     * @param service
     */
    public BacklightManager(Context ctx, IBacklightService service) {
        this.mContext = ctx;
        if (service == null) {
        	Slog.e(TAG, "mService is NULL");
        	//throw new IllegalArgumentException("BacklightManager: service is null");
        }
        this.mService = service;
    }
//    public BacklightManager(IBacklightService service) {
//		mService = service;
//	}
    
	public int getBrightness() {
		try {
			return mService.getBrightness();
		} catch (RemoteException e) {
			Slog.e(TAG, "Unable to contact the remote BacklightService");
			return 0;
		}
	}
	
	public void setBrightness(int value) {
		try {
			//return mService.setBrightness(value);
			mService.setBrightness(value);
		} catch (RemoteException e) {
			Slog.e(TAG, "Unable to setBrightness on the remote BacklightService");
		}
	}
}
