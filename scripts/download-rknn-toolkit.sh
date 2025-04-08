#!/bin/bash

# File ID and output file name
FILE_ID="1IZdh5glkCnq0s2vJfRMubDnMPyrqqLNt"
OUTPUT_FILE="downloaded_file"

# Step 1: Fetch the confirmation token and download the file
echo "Downloading file from Google Drive..."
CONFIRM=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate \
    "https://drive.google.com/uc?export=download&id=${FILE_ID}" -O- | \
    sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1/p')

wget --load-cookies /tmp/cookies.txt "https://drive.google.com/uc?export=download&confirm=${CONFIRM}&id=${FILE_ID}" \
    -O "${OUTPUT_FILE}" && rm -rf /tmp/cookies.txt

# Step 2: Verify the download
if [ -f "${OUTPUT_FILE}" ]; then
    echo "File downloaded successfully: ${OUTPUT_FILE}"
else
    echo "Failed to download the file."
    exit 1
fi