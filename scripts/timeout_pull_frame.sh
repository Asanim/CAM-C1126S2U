#!/bin/bash

# Directory to store the captured frames
OUTPUT_DIR="captured_frames"
mkdir -p "$OUTPUT_DIR"

# Iterate over all video devices
for DEVICE in /dev/video*; do
    # Check if the device is a valid video device
    if [[ -c "$DEVICE" ]]; then
        echo "Processing $DEVICE..."
        
        # Generate a unique output filename
        OUTPUT_FILE="$OUTPUT_DIR/$(basename "$DEVICE")_frame.jpg"
        
        # Capture a single frame with a timeout of 5 seconds using ffmpeg
        timeout 5 ffmpeg -y -f v4l2 -i "$DEVICE" -frames:v 1 "$OUTPUT_FILE"
        
        # Check if the frame was successfully captured
        if [[ $? -eq 0 ]]; then
            echo "Frame captured for $DEVICE and saved to $OUTPUT_FILE"
        else
            echo "Failed to capture frame for $DEVICE (timeout or error)"
        fi
    else
        echo "$DEVICE is not a valid video device"
    fi
done

echo "Frame capture process completed."