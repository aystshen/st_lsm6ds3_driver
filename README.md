# STMicroelectronics LSM6DS3 driver
This is the Linux/Android driver for STMicroelectronics LSM6DS3. The LSM6DS3 is a system-in-package featuring a 3D
digital accelerometer and a 3D digital gyroscope.  

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be a reference to you, when you are integrating the Lontium's LT8911EXB IC into your system, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

## Usage
1. Copy lsm6ds3 to kernel/drivers/input/sensors/ directory.
2. Modify the kernel/drivers/input/sensors/Kconfig as follows
```
source "drivers/video/lsm6ds3/Kconfig"
```
3. Modify the kernel/drivers/input/sensors/Makefile as follows
```
obj-$(CONFIG_INPUT_LSM6DS3) += lsm6ds3/
```
4. Modify the dts as follows
```
&i2c2 {
	status = "okay";
	
	lsm6ds3@6b {
		compatible = "st,lsm6ds3";
		reg = <0x6b>;
		st,irq_gpio = <&gpio1 GPIO_A3 GPIO_ACTIVE_LOW>;

		st,drdy-int-pin = <1>;
		
		// 0: xyz 1:xzy 2:yxz 3:yzx 4:zxy 5:zyx
		st,xyz = <2>;
		st,x-reverse = <1>;
		st,y-reverse = <1>;
		st,z-reverse = <0>;
	};
};
```

## For Android
Android HAL, please refer to the file modification in the hardware.

## Developed By
* ayst.shen@foxmail.com

## License
	Copyright 2019 Shen Haibo.

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
