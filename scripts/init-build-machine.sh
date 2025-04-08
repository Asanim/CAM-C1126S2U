#!/bin/bash
# filepath: scripts/init-build-machine.sh

# Check for sudo privileges
if [ "$EUID" -ne 0 ]; then
  echo "This script must be run as root. Please use sudo."
  exit 1
fi

# Define variables
TOOLCHAIN_URL="https://releases.linaro.org/components/toolchain/binaries/6.3-2017.05/arm-linux-gnueabihf/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf.tar.xz"
INSTALL_DIR="/tools/toolchains"
TOOLCHAIN_ARCHIVE="gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf.tar.xz"

# Create installation directory if it doesn't exist
mkdir -p "$INSTALL_DIR"

# Download the toolchain
wget "$TOOLCHAIN_URL" -O "$TOOLCHAIN_ARCHIVE"

# Extract the toolchain to the installation directory
tar -xf "$TOOLCHAIN_ARCHIVE" -C "$INSTALL_DIR"

# Clean up the downloaded archive
rm "$TOOLCHAIN_ARCHIVE"

echo "Toolchain installed to $INSTALL_DIR"