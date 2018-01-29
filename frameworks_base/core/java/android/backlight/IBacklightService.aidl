// IBacklightInterface.aidl
package android.backlight;
// Declare any non-default types here with import statements

interface IBacklightService {
    int getBrightness();
    void setBrightness(int value);
}
