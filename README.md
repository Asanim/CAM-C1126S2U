# CAM-CRV1126 Project

This repository investigates application development on the RV1126, particularly the standalone camera module.

Documentation: [CAM-C11092U Documentation](https://wiki.t-firefly.com/en/CAM-C11092U/index.html)

The camera itself is like an AI-enabled GoPro.

---

## Purpose

To investigate the time required to port our application to the target device as a standalone application.

### Objectives:
1. Porting to the architecture of the device, including replacing hardware-dependent libraries (e.g., camera interfacing).
2. Establishing an internet connection.
3. Installing and ensuring reliable operation of AWS Greengrass.

The device features a quad-core ARMv7L processor, which is already faster than our original resource-constrained device (Raspberry Pi 2). It is 32-bit native.

The goal is to get an MVP setup as quickly as possible. Hence, I will not attempt to recompile a fully workable OS for it, but simply use BusyBox as is or by copying applications and tools as needed.

A sample application for the base firmware can be found here: [Sample Application](https://en.t-firefly.com/doc/download/85.html)

### Arc Face Go SDK

- [Arc Face Go SDK](https://drive.google.com/drive/folders/14G_Pdt5fiUMp1MxWn1fek2ezb9F3HRTm)
- The Arc Face Go presents a simple example using a self-contained precompiled application and dependencies stored as tar archives.
- The install script copies the `lib`, `bin`, `web`, etc., folders to appropriate locations on the hard drive.
- GitHub project: [ArcUVC App](https://gitlab.com/firefly-linux/external/arcuvc_app)

While the example app is called an SDK, it does not provide any method to compile or code to interface with the camera or NPU.

### Features of Interest

- Infrared and LED floodlights, useful in scenarios of low brightness or nighttime monitoring.

---

## CAM-CRV1126S2U Specifications

- **CPU**: Quad-Core ARM Cortex-A7, RISC-V MCU
- **NPU**: 2.0Tops, supports INT8/INT16
- **DDR**: DDR3 / 1GB
- **Flash**: eMMC / 8GB
- **Display**: MIPI-DSI interface, 1080P@60fps
- **Camera**: Dual MIPI-CSI interface to adapt to RGB & IR binocular camera
- **ISP**: 14M ISP 2.0 with 3-frame HDR (Line-based/Frame-based/DCG)
- **Hardware Encoding**:
  - 4K H.264/H.265 encoding: 3840 x 2160@30 fps + 720p@30 fps encoding
- **Hardware Decoding**:
  - 4K H.264/H.265 decoding: 3840 x 2160@30 encoding + 3840 x 2160@30 fps decoding
- **RTC**: Peripheral RTC
- **Peripheral Interface**: Debug serial port, Type-C OTG interface, multi-function expansion interface

---

## Setup

```bash
./scripts init-build-machine.sh
```

---

## Building

The target example is located in: `rknpu/rknn/rknn_api/examples/rknn_yolov5_demo`

### Steps:
1. Run the setup script:
   ```bash
   ./scripts/setup-device.sh
   ```
2. Log into the device:
   ```bash
   adb shell
   ```
3. Navigate to the demo directory:
   ```bash
   cd /userdata/rknn_yolov5_demo
   ```
4. Run the executable:
   ```bash
   ./run_rv1109_rv1126.sh
   ```

---

## Future Work

### Peripherals Adaptation

The camera offers rich peripheral adaptation as noted [here](https://wiki.t-firefly.com/en/CAM-C11092U/Device_adaptation.html). It would be very useful to develop external circuits to utilize this:

- **GPIO**: Control pan/tilt.
- **GPIO**: Control power on/off with an external timer (e.g., to conserve power by turning off the device for a specific period).
- **Ethernet Support**
- **I2C Bus**: For sensors like IMU, light, humidity.
- **UART Interfaces**
- **Battery Monitoring**: Measure battery charge.