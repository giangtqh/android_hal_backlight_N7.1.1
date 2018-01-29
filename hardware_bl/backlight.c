#define LOG_TAG "backlight"

#include "backlight.h"
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <cutils/log.h>
#include <cutils/atomic.h>
#include <cutils/properties.h>
#include <string.h>

#define MAX_BRIGHTNESS 255
#define DEF_BACKLIGHT_DEV "pwm-backlight"
#define DEF_BACKLIGHT_PATH "/sys/class/backlight/"

/*****************************************************************************/
struct backlight_module_t {
    struct hw_module_t common;
};

static int backlight_device_open(const struct hw_module_t* module,
                                 const char* name, struct hw_device_t** device);

static struct hw_module_methods_t backlight_module_methods = {
    .open= backlight_device_open
};

struct backlight_module_t HAL_MODULE_INFO_SYM = {
    .common= {
        .tag= HARDWARE_MODULE_TAG,
        .version_major= 1,
        .version_minor= 0,
        .id= BACKLIGHT_HARDWARE_MODULE_ID,
        .name= "Backlight module",
        .author= "Freescale Semiconductor",
        .methods= &backlight_module_methods,
    }
};

static char max_path[256], path[256];
// ****************************************************************************
// module
// ****************************************************************************
static int set_backlight(struct backlight_device_t* dev, int value) //light_state_t -> change to simple brightness value
{
    int result = -1;
    unsigned int brightness = 0, max_brightness = 0;
    unsigned int i = 0;
    FILE *file;

    brightness = ((77*((value>>16)&0x00ff)) + (150*((value>>8)&0x00ff)) +
                 (29*(value&0x00ff))) >> 8;
    ALOGE("set_backlight, get brightness=%d", brightness);

    file = fopen(max_path, "r");
    if (!file) {
        ALOGE("can not open file %s\n", max_path);
        return result;
    }
    fread(&max_brightness, 1, 3, file);
    fclose(file);

    max_brightness = atoi((char *) &max_brightness);
    /* any brightness greater than 0, should have at least backlight on */
    if (max_brightness < MAX_BRIGHTNESS)
        brightness = max_brightness *(brightness + MAX_BRIGHTNESS / max_brightness - 1) / MAX_BRIGHTNESS;
    else
        brightness = max_brightness * brightness / MAX_BRIGHTNESS;

    if (brightness > max_brightness) {
        brightness  = max_brightness;
    }
    
//    ALOGV("set_light, max_brightness=%d, target brightness=%d",
//        max_brightness, brightness);
    ALOGE("set_light, max_brightness=%d, target brightness=%d",
            max_brightness, brightness);

    file = fopen(path, "w");
    if (!file) {
        ALOGE("can not open file %s\n", path);
        return result;
    }
    fprintf(file, "%d", brightness);
    fclose(file);

    result = 0;
    return result;
}

static int get_backlight(struct backlight_device_t* dev)
{
    unsigned int brightness = 0, max_brightness = 0;
    FILE *file;

    file = fopen(max_path, "r");
    if (!file) {
        ALOGE("can not open file %s\n", max_path);
        return -1;
    }
    fread(&max_brightness, 1, 3, file);
    fclose(file);

    max_brightness = atoi((char *) &max_brightness);

    file = fopen(path, "r");
    if (!file) {
        ALOGE("can not open file %s\n", path);
        return -1;
    }
    fread(&brightness, 1, 3, file);
    fclose(file);

    brightness = atoi((char *) &brightness);
    ALOGE("get_light, max_brightness=%d, target brightness=%d",
                max_brightness, brightness);
    if (brightness < max_brightness)
      //brightness = max_brightness *(brightness + MAX_BRIGHTNESS / max_brightness - 1) / MAX_BRIGHTNESS;
        brightness = (brightness * MAX_BRIGHTNESS ) / max_brightness + MAX_BRIGHTNESS / max_brightness -1;
    else
        brightness = MAX_BRIGHTNESS * brightness / max_brightness;

    if (brightness > MAX_BRIGHTNESS) {
        brightness  = MAX_BRIGHTNESS;
    }

    ALOGE("get_backlight, brightness=%d", brightness);
    return brightness;
}


static int close_backlight(struct hw_device_t *dev)
{
    struct backlight_device_t *device = (struct backlight_device_t*)dev;
    if (device)
        free(device);
    return 0;
}

/*****************************************************************************/
static int backlight_device_open(const struct hw_module_t* module,
                              const char* name, struct hw_device_t** device)
{
    int status = -EINVAL;
    char value[PROPERTY_VALUE_MAX];
    ALOGV("backlight_device_open\n");

    struct backlight_device_t *dev = (struct backlight_device_t*)malloc(sizeof(*dev));
    /* initialize our state here */
    memset(dev, 0, sizeof(*dev));

    /* initialize the procs */
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*) module;
    dev->common.close = close_backlight;

    dev->set_backlight = set_backlight;
    dev->get_backlight = get_backlight;

    *device = &dev->common;

    property_get("hw.backlight.dev", value, DEF_BACKLIGHT_DEV);
    strcpy(path, DEF_BACKLIGHT_PATH);
    strcat(path, value);
    strcpy(max_path, path);
    strcat(max_path, "/max_brightness");
    strcat(path, "/brightness");

    ALOGI("max backlight file is %s\n", max_path);
    ALOGI("backlight brightness file is %s\n", path);

    status = 0;

    /* todo other lights device init */
    return status;
}
