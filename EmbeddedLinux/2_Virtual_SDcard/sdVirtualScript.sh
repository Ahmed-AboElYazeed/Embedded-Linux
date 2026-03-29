#!/bin/bash

# Script to create and set up an image file as an SD card with boot and rootfs partitions
# Usage: ./setup_sd_image.sh <image_file> <size_in_MB> [boot_size_MB]

# Check if running as root
if [ "$EUID" -ne 0 ]; then 
    echo "Error: This script must be run as root (use sudo)"
    exit 1
fi

# Check arguments
if [ $# -lt 1 ]; then
    echo "Usage: $0 <image_file> [total_size_MB] [boot_partition_MB]"
    echo "Example: sudo $0 sdcard.img 1024 200"
    echo "Defaults: total_size=1024MB, boot_partition=200MB"
    exit 1
fi

IMAGE_FILE="$1"
TOTAL_SIZE_MB="${2:-1024}"     # Default 1024MB total
BOOT_SIZE_MB="${3:-200}"       # Default 200MB for boot partition

# Calculate rootfs size
ROOTFS_SIZE_MB=$((TOTAL_SIZE_MB - BOOT_SIZE_MB))

if [ "$ROOTFS_SIZE_MB" -le 0 ]; then
    echo "Error: Boot partition size ($BOOT_SIZE_MB MB) is too large for total size ($TOTAL_SIZE_MB MB)"
    exit 1
fi

# Convert to absolute path
IMAGE_FILE=$(realpath "$IMAGE_FILE")

echo "=========================================="
echo "SD Card Image Setup"
echo "=========================================="
echo "Image file: $IMAGE_FILE"
echo "Total size: ${TOTAL_SIZE_MB}MB"
echo "Partition 1 (boot): ${BOOT_SIZE_MB}MB FAT16"
echo "Partition 2 (rootfs): ${ROOTFS_SIZE_MB}MB ext4"
echo "=========================================="
echo ""

# Step 1: Create the image file if it doesn't exist or is empty
EXPECTED_SIZE=$((TOTAL_SIZE_MB * 1024 * 1024))

if [ -f "$IMAGE_FILE" ]; then
    ACTUAL_SIZE=$(stat -c%s "$IMAGE_FILE")
    echo "Found existing file: $IMAGE_FILE ($ACTUAL_SIZE bytes)"
    
    if [ "$ACTUAL_SIZE" -eq 0 ]; then
        echo "File is empty, recreating..."
        rm -f "$IMAGE_FILE"
    elif [ "$ACTUAL_SIZE" -ne "$EXPECTED_SIZE" ]; then
        echo "Warning: File size ($ACTUAL_SIZE bytes) doesn't match requested size ($EXPECTED_SIZE bytes)"
        echo "Using existing file anyway..."
    else
        echo "Using existing image file with correct size"
    fi
fi

if [ ! -f "$IMAGE_FILE" ]; then
    echo "Creating image file: $TOTAL_SIZE_MB MB..."
    dd if=/dev/zero of="$IMAGE_FILE" bs=1M count="$TOTAL_SIZE_MB" status=progress
    if [ $? -ne 0 ]; then
        echo "Error: Failed to create image file"
        rm -f "$IMAGE_FILE"
        exit 1
    fi
    sync  # Ensure data is written to disk
fi

# Verify file size
ACTUAL_SIZE=$(stat -c%s "$IMAGE_FILE")
echo "Final image file size: $ACTUAL_SIZE bytes ($(($ACTUAL_SIZE / 1024 / 1024)) MB)"

if [ "$ACTUAL_SIZE" -eq 0 ]; then
    echo "Error: Image file is still empty after creation"
    exit 1
fi

# Step 2: Set up loop device
echo "Setting up loop device..."
LOOP_DEV=$(losetup -f --show "$IMAGE_FILE")
if [ $? -ne 0 ] || [ -z "$LOOP_DEV" ]; then
    echo "Error: Failed to create loop device"
    exit 1
fi
echo "Loop device created: $LOOP_DEV"

# Verify loop device is properly attached
if ! losetup -l | grep -q "$LOOP_DEV"; then
    echo "Error: Loop device not properly attached"
    losetup -d "$LOOP_DEV" 2>/dev/null
    exit 1
fi

# Step 3: Partition the image (MBR/DOS partition table with 2 partitions)
echo "Creating MBR partition table..."
parted -s "$LOOP_DEV" mklabel msdos
if [ $? -ne 0 ]; then
    echo "Error: Failed to create partition table"
    losetup -d "$LOOP_DEV"
    exit 1
fi

# Calculate partition boundaries
BOOT_END_MB=$((BOOT_SIZE_MB + 1))  # Start at 1MB, end at BOOT_SIZE+1

echo "Creating partition 1: ${BOOT_SIZE_MB}MB FAT16 boot partition..."
parted -s "$LOOP_DEV" mkpart primary fat16 1MiB ${BOOT_END_MB}MiB
if [ $? -ne 0 ]; then
    echo "Error: Failed to create boot partition"
    losetup -d "$LOOP_DEV"
    exit 1
fi

echo "Creating partition 2: ${ROOTFS_SIZE_MB}MB ext4 rootfs partition..."
parted -s "$LOOP_DEV" mkpart primary ext4 ${BOOT_END_MB}MiB 100%
if [ $? -ne 0 ]; then
    echo "Error: Failed to create rootfs partition"
    losetup -d "$LOOP_DEV"
    exit 1
fi

# Set boot flag on first partition
echo "Setting boot flag on partition 1..."
parted -s "$LOOP_DEV" set 1 bo
