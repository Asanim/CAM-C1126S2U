
this repo investigates application development on the rv1126, particularly the standalone camera module: 

documentation is here: https://wiki.t-firefly.com/en/CAM-C11092U/index.html

creating a self developed application 

the camera itself is like an ai enabled go pro. 

## purpose

to investigate the time to port our application to the target device as a standalone application. 

1. porting to arch of device including replace hardware dependant libraries ie camera interfacing, 
2. internet connection 
3. aws greengrass installation and reliable operating

quadcore armv7l which is already faster than our original resource constrained device (raspberry pi 2). 
32 bit native

the goal is to get a mvp setup as quickly as possible, hence I shall not attempt to recompile a fully workable os for it, simply use the busybox as is or by copying applications and tools as needed.

a sample application for the base firmware can be found: https://en.t-firefly.com/doc/download/85.html


arc face go sdk: https://drive.google.com/drive/folders/14G_Pdt5fiUMp1MxWn1fek2ezb9F3HRTm

the arc face go presents a simple example using a self contained percompiled application and dependencies stored as tar archives
the install script copies the lib, bin, web etc folders to appropriate locations on the hard drive 

the github project is here: https://gitlab.com/firefly-linux/external/arcuvc_app

while the example app is called a sdk, it does nto provide any method to compile or code to interface with the camera or NPU

of particular interest are the infrared and led floodlights, useful in scenarios of low brightness or monitoring nighttime environments 

## CAM-CRV1126S2U Specs

    CPU:Quad-Core ARM Cortex-A7，RISC-V MCU

    NPU:2.0Tops, support INT8/ INT16

    DDR: DDR3 / 1GB

    Flash: eMMC / 8GB

    Display: MIPI-DSI interface，1080P@60fps

    Camera: Dual MIPI-CSI interface to adapt to RGB & IR binocular camera

    ISP:14M ISP 2.0 with 3 frame HDR(Line-based/Frame-based/DCG)

    Hardware Encoding:Support 4K H.264/H.265 coding
    -3840 x 2160@30 fps+720p@30 fps encoding

    Hardware Decoding:Support 4K H.264/H.265 decoding
    -3840 x 2160@30 encoding + 3840 x 2160@30 fps decoding

    RTC：Peripheral RTC

    Peripheral Interface：Debug serial port, Typec otg interface, multi-function expansion interface


## Setup

This repo contains convenience scripts to setup and install the OS compilation sdk. Otherwise, you may follow the steps outlined in: https://wiki.t-firefly.com/en/CAM-C11092U/Source_code.html

```bash
./scripts/install-sdk.sh
```

rknn toolkit lite offers convenience scripts for running the inference engine with resnet in python

```bash
./scripts/download-rknn-toolkit.sh
```

## Building

/home/sam/cam-crv1126/rknpu/rknn/rknn_api/examples/rknn_yolov5_demo






## future work:

**Peripherals Adaptation**
the camera offers rich peripheral adaptation as noted here. it would be very useful to develop external circuits to utilize this: 
- GPIO: control pan / tilt 
- GPIO: Control power on/off with an external timer. For instance, to turn off a the device for a specific period of time in order to conserve power use.
- Ethernet support
- i2c bus for sensors like imu, light, humidity 
- UART interfaces
- measuring battery charge

https://wiki.t-firefly.com/en/CAM-C11092U/Device_adaptation.html

**alternate hardware**
battery connect cameras are highly available in multiple hardware options like the raspberry pi but more notably in any android phone.

create an andorid apk cmake
https://developer.android.com/ndk/guides


https://developer.android.com/studio/projects/add-native-code

https://developer.android.com/studio/projects/configure-cmake

other:
https://forum.qt.io/topic/148514/create-android-apk-with-cmake

## Steps taken

### logging in 

plug in the camera to your pc / development machine via the usb port on the side

enumerate devices connected 

adb devices 

adb shell

generates a root prompt. if there are multiple devices conencted to your machine ie an andoird phone, specify the id

### camera check
camera may be interface as with any standard linux machine:

v4l2-ctl --list-outputs

returns ioctl: VIDIOC_ENUMOUTPUT


cd /opt/

v4l2-ctl --device=/dev/video0 --stream-mmap=3 --stream-to=test_frame.raw --stream-count=1

### Controlling the LEDs

infrared:

[root@RV1126_RV1109:/]# echo 100 > /sys/class/leds/PWM-IR/brightness 
[root@RV1126_RV1109:/]# echo 0 > /sys/class/leds/PWM-IR/brightness 

RGB LED:

[root@RV1126_RV1109:/]# cat /sys/class/leds/PWM-RGB/max_brightness 
255

[root@RV1126_RV1109:/]# echo 0 > /sys/class/leds/PWM-RGB/brightness 
[root@RV1126_RV1109:/]# echo 255 > /sys/class/leds/PWM-RGB/brightness 


download and install dpkg https://ftp.debian.org/debian/pool/main/d/dpkg/









### Appendix 1:

google drive folders: https://drive.google.com/file/d/1IZdh5glkCnq0s2vJfRMubDnMPyrqqLNt/view


https://wiki.t-firefly.com/en/CAM-C11092U/Algorithm_adaptation.html

https://wiki.t-firefly.com/en/CAM-C11092U/Algorithm_diy.html