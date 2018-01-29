
#ifndef ANDROID_BACKLIGHT_INTERFACE_H
#define ANDROID_BACKLIGHT_INTERFACE_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS

/**
 * The id of this module
 */
#define BACKLIGHT_HARDWARE_MODULE_ID "backlight"

struct backlight_device_t {
    struct hw_device_t common;

    /**
     * Set the provided lights to the provided values.
     *
     * Returns: 0 on succes, error code on failure.
     */
    int (*set_backlight)(struct backlight_device_t* dev, int value);
    int (*get_backlight)(struct backlight_device_t* dev);
};


__END_DECLS

#endif  // ANDROID_BACKLIGHT_INTERFACE_H
