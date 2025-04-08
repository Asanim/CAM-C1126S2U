#!/bin/bash
# filepath: scripts/install_sdk.sh

# Variables
SDK_PACKAGE="rv1126_rv1109_linux_release_20211022.tgz"
EXPECTED_MD5="596c6bc6bb3095aea97d54c9df4cf333"
CONFIG_FILE="device/rockchip/rv1126_rv1109/cam-crv1126s2u-facial_gate-BE-45.mk"

# Step 1: Download the SDK package
echo "Downloading the SDK package..."
wget -O $SDK_PACKAGE "https://example.com/path/to/$SDK_PACKAGE" # Replace with actual URL

# Step 2: Verify the MD5 checksum
echo "Verifying the MD5 checksum..."
ACTUAL_MD5=$(md5sum $SDK_PACKAGE | awk '{print $1}')
if [ "$ACTUAL_MD5" != "$EXPECTED_MD5" ]; then
    echo "MD5 checksum verification failed! Expected: $EXPECTED_MD5, Got: $ACTUAL_MD5"
    exit 1
fi
echo "MD5 checksum verified successfully."

# Step 3: Extract the SDK package
echo "Extracting the SDK package..."
tar xvf $SDK_PACKAGE

# Step 4: Synchronize the repository
echo "Synchronizing the repository..."
cd rv1126_rv1109_linux_release_20211022
.repo/repo/repo sync -l
.repo/repo/repo sync -c --no-tags
.repo/repo/repo start firefly --all

# Step 5: Install dependencies
echo "Installing dependencies..."
sudo apt-get update
sudo apt-get install -y repo git-core gitk git-gui gcc-arm-linux-gnueabihf u-boot-tools \
    device-tree-compiler gcc-aarch64-linux-gnu mtools parted libudev-dev libusb-1.0-0-dev \
    python-linaro-image-tools linaro-image-tools autoconf autotools-dev libsigsegv2 m4 \
    intltool libdrm-dev curl sed make binutils build-essential gcc g++ bash patch gzip \
    gawk bzip2 perl tar cpio python unzip rsync file bc wget libncurses5 libqt4-dev \
    libglib2.0-dev libgtk2.0-dev libglade2-dev cvs git mercurial rsync openssh-client \
    subversion asciidoc w3m dblatex graphviz python-matplotlib libc6:i386 libssl-dev \
    expect fakeroot cmake flex bison liblz4-tool libtool keychain

# Additional dependencies for Ubuntu 17.04
sudo apt-get install -y lib32gcc-7-dev g++-7 libstdc++-7-dev

# Step 6: Configure and compile
echo "Configuring and compiling the SDK..."
./build.sh $CONFIG_FILE

# Step 7: Global compilation
echo "Performing global compilation..."
./build.sh

echo "SDK installation and compilation completed successfully."