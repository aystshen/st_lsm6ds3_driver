/*
 * Definitions for lsm6ds3 accel+gyro chip.
 */
 
#ifndef LSM6DS3_H
#define LSM6DS3_H

#include <linux/ioctl.h>

#define LSM6DS3_IOC_MAGIC               'l'

#define LSM6DS3_IOC_ENABLE              _IOW(LSM6DS3_IOC_MAGIC, 1, int)
#define LSM6DS3_IOC_SET_RATE            _IOW(LSM6DS3_IOC_MAGIC, 2, int)

#define LSM6DS3_GSENSOR_DEV_PATH        "/dev/accel"
#define LSM6DS3_GYRO_DEV_PATH           "/dev/gyro"
#define LSM6DS3_SIGN_MOTION_DEV_PATH    "/dev/sign_m"
#define LSM6DS3_STEP_COUNTER_PATH       "/dev/step_c"
#define LSM6DS3_STEP_DETECTOR_DEV_PATH  "/dev/step_d"
#define LSM6DS3_TILT_DEV_PATH           "/dev/tilt"

#endif

