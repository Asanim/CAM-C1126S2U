
this repo investigates application development on the rv1126, particularly the standalone camera module: 

documentation is here: https://wiki.t-firefly.com/en/CAM-C11092U/index.html

creating a self developed application 

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



download and install dpkg https://ftp.debian.org/debian/pool/main/d/dpkg/









### Appendix 1:

google drive folders: https://drive.google.com/file/d/1IZdh5glkCnq0s2vJfRMubDnMPyrqqLNt/view


https://wiki.t-firefly.com/en/CAM-C11092U/Algorithm_adaptation.html

https://wiki.t-firefly.com/en/CAM-C11092U/Algorithm_diy.html