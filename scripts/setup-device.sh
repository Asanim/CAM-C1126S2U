#!/bin/bash

echo "ensure you have the device connected to the host machine and is available through the adb port"

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

install_inference_example() {
    echo "Installing inference example"
    adb push $SCRIPT_PATH/../rknpu/drivers/linux-armhf-puma/*   /
    adb push $SCRIPT_PATH/../rknpu/drivers/npu_ko/galcore_puma.ko /lib/modules/galcore.ko

    GCC_COMPILER=/tools/toolchains/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf
    RV1109_TOOL_CHAIN=/tools/toolchains/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf

    cd $SCRIPT_PATH/../rknpu/rknn/rknn_api/examples/rknn_yolov5_demo/
    ./build.sh

    adb push $SCRIPT_PATH/../rknpu/rknn/rknn_api/examples/rknn_yolov5_demo/install/rknn_yolov5_demo /userdata/
}

adb push $SCRIPT_PATH/target/install-aws-greengrass.sh /tmp/
adb push $SCRIPT_PATH/target/download/azul-zulu-java11-jdk.tar.gz /tmp/
adb push $SCRIPT_PATH/target/download/greengrass-nucleus-latest.zip /tmp/

# Run install-aws-greengrass.sh on the device to install 
adb shell "chmod +x /tmp/install-aws-greengrass.sh && /tmp/install-aws-greengrass.sh"
