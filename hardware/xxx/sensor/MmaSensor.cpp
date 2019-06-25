/*
    Copyright (C) 2008 The Android Open Source Project

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <stdio.h>
#include <math.h>
#include <cutils/log.h>

#include "MmaSensor.h"

MmaSensor::MmaSensor()
    : SensorBase(MMA_DEVICE_NAME, "gsensor"),
      mEnabled(0),
      mInputReader(32)
{
    memset(accel_offset, 0, sizeof(accel_offset));
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_A;
    mPendingEvent.type = SENSOR_TYPE_ACCELEROMETER;
    mPendingEvent.acceleration.status = SENSOR_STATUS_ACCURACY_HIGH;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    int err = 0;
    err = open_device();
    err = err < 0 ? -errno : 0;

    if (err) {
        LOGE("%s:%s\n", __func__, strerror(-err));
        return;
    }
}

MmaSensor::~MmaSensor()
{
    if (mEnabled) {
        enable(0, 0);
    }

    if (dev_fd > 0) {
        close(dev_fd);
        dev_fd = -1;
    }
}

int MmaSensor::enable(int32_t /* handle */, int en)
{
    int newState  = en ? 1 : 0;
    int err = 0;

    if (mEnabled != newState) {
        if (dev_fd < 0) {
            open_device();
        }

        if (0 > (err = ioctl(dev_fd, GSENSOR_IOC_ENABLE, &newState))) {
            LOGE("fail to perform GSENSOR_IOC_ENABLE, err = %d, error is '%s'", err, strerror(errno));
            goto EXIT;
        }

        mEnabled = newState;
    }

EXIT:
    return err;
}

int MmaSensor::setDelay(int32_t   handle, int64_t ns)
{
    int err = 0;

    if (ns < 0) {
        return -EINVAL;
    }

    if (dev_fd < 0) {
        open_device();
    }

    short delay = ns / 1000000;

    if ((err = ioctl(dev_fd, GSENSOR_IOC_SET_RATE, &delay)) < 0) {
        LOGE("fail to perform GSENSOR_IOC_SET_RATE, result = %d, error is '%s'", err, strerror(errno));
    }

    return err;
}

int MmaSensor::isActivated(int /* handle */)
{
    return mEnabled;
}

int MmaSensor::readEvents(sensors_event_t* data, int count)
{
    if (count < 1) {
        return -EINVAL;
    }

    ssize_t n = mInputReader.fill(data_fd);

    if (n < 0) {
        return n;
    }

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;

        if (type == EV_REL) {
            processEvent(event->code, event->value);
        } else if (type == EV_SYN) {
            mPendingEvent.timestamp = getTimestamp();
            *data++ = mPendingEvent;
            count--;
            numEventReceived++;
        } else {
            LOGE("MmaSensor: unknown event (type=%d, code=%d)",
                 type, event->code);
        }

        mInputReader.next();
    }

    return numEventReceived;
}

void MmaSensor::processEvent(int code, int value)
{
    switch (code) {
        case EVENT_TYPE_ACCEL_X:
            mPendingEvent.acceleration.x = (value - accel_offset[0]) * ACCELERATION_RATIO_ANDROID_TO_HW;
            break;

        case EVENT_TYPE_ACCEL_Y:
            mPendingEvent.acceleration.y = (value - accel_offset[1]) * ACCELERATION_RATIO_ANDROID_TO_HW;
            break;

        case EVENT_TYPE_ACCEL_Z:
            mPendingEvent.acceleration.z = (value - accel_offset[2]) * ACCELERATION_RATIO_ANDROID_TO_HW;
            break;
    }
}
