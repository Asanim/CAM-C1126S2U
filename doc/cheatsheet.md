this document provides some instructions on certain commands and usage tutorial. 


### Using `v4l2-ctl`:

the device does not have ffmpeg installed, only v4l


1. Capture a single frame from the camera:
   ```bash
   v4l2-ctl --device=/dev/video0 --stream-mmap=3 --stream-to=test_frame.raw --stream-count=1
   ```
   Replace video0 with your camera's device path. The captured frame will be saved as `test_frame.raw`.

3. Convert the raw frame to a viewable image using `ffmpeg`:
   ```bash
   ffmpeg -f rawvideo -pix_fmt yuyv422 -s 640x480 -i test_frame.raw output.jpg
   ```
   Adjust the resolution (`640x480`) to match your camera's settings.


### Copying Files to an Android Device
To copy a file from your computer to your Android device:
```bash
adb push <local_file_path> <remote_device_path>
```
Example:
```bash
adb push myfile.txt /sdcard/
```

### Copying Files from an Android Device
To copy a file from your Android device to your computer:
```bash
adb pull <remote_device_path> <local_file_path>
```
Example:
```bash
adb pull /sdcard/myfile.txt .
```
