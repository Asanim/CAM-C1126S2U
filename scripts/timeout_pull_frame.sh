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
        OUTPUT_FILE="$OUTPUT_DIR/$(basename "$DEVICE")_frame.raw"
        
        # Capture a single frame with a timeout of 5 seconds
        timeout 5 v4l2-ctl --device="$DEVICE" --stream-mmap=3 --stream-to="$OUTPUT_FILE" --stream-count=1
        
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