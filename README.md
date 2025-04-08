
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


## Steps taken

download and install dpkg https://ftp.debian.org/debian/pool/main/d/dpkg/









### Appendix 1:

google drive folders: https://drive.google.com/file/d/1IZdh5glkCnq0s2vJfRMubDnMPyrqqLNt/view


https://wiki.t-firefly.com/en/CAM-C11092U/Algorithm_adaptation.html

https://wiki.t-firefly.com/en/CAM-C11092U/Algorithm_diy.html