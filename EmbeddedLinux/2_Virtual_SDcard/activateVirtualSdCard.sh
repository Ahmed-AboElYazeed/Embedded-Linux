#!/bin/bash

# ==============================================================
# Script: activateVirtualSdCard.sh
# Description: Setup and activate virtual SD card (sd.img)
# ==============================================================

set -e  # Exit immediately if a command fails

# ── Colors ──────────────────────────────────────────────────
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# ── Variables ────────────────────────────────────────────────
SD_IMG_PATH="$HOME/ITI_Files/linux/Embedded-Linux"
SD_IMG="sd.img"
MOUNT_BASE="/media/$(whoami)"
BOOT_MOUNT="$MOUNT_BASE/boot"
ROOTFS_MOUNT="$MOUNT_BASE/rootfs"

# ── Step 1: Navigate to the directory ───────────────────────
echo -e "${CYAN}[*] Navigating to: ${SD_IMG_PATH}${NC}"
cd "$SD_IMG_PATH" || {
    echo -e "${RED}[ERROR] Directory not found: ${SD_IMG_PATH}${NC}"
    exit 1
}

# Check if sd.img exists
if [[ ! -f "$SD_IMG" ]]; then
    echo -e "${RED}[ERROR] '${SD_IMG}' not found in ${SD_IMG_PATH}${NC}"
    exit 1
fi

echo -e "${GREEN}[✔] Found ${SD_IMG}${NC}"

# ── Step 2: Attach the image using losetup ──────────────────
echo -e "${CYAN}[*] Attaching ${SD_IMG} as a loop device...${NC}"

LOOP_DEV=$(sudo losetup -f --partscan --show "$SD_IMG")

if [[ -z "$LOOP_DEV" ]]; then
    echo -e "${RED}[ERROR] Failed to create loop device.${NC}"
    exit 1
fi

echo -e "${GREEN}[✔] Loop device created: ${LOOP_DEV}${NC}"

# ── Step 3: Wait for partition nodes to be available ────────
echo -e "${YELLOW}[*] Waiting for partition nodes to be ready...${NC}"
sleep 1

PART1="${LOOP_DEV}p1"
PART2="${LOOP_DEV}p2"

if [[ ! -b "$PART1" || ! -b "$PART2" ]]; then
    echo -e "${RED}[ERROR] Partition nodes not found: ${PART1} / ${PART2}${NC}"
    echo -e "${YELLOW}[!] Running partprobe on ${LOOP_DEV}...${NC}"
    sudo partprobe "$LOOP_DEV"
    sleep 1
fi

# ── Step 4: Format partition 1 as FAT16 (boot) ──────────────
echo -e "${CYAN}[*] Formatting ${PART1} as FAT16 (boot)...${NC}"
sudo mkfs.vfat -F 16 -n boot "$PART1"
echo -e "${GREEN}[✔] ${PART1} formatted as FAT16 with label 'boot'${NC}"

# ── Step 5: Format partition 2 as EXT4 (rootfs) ─────────────
echo -e "${CYAN}[*] Formatting ${PART2} as EXT4 (rootfs)...${NC}"
sudo mkfs.ext4 -L rootfs "$PART2"
echo -e "${GREEN}[✔] ${PART2} formatted as EXT4 with label 'rootfs'${NC}"

# ── Step 6: Create mount points ──────────────────────────────
echo -e "${CYAN}[*] Creating mount points...${NC}"

sudo mkdir -p "$BOOT_MOUNT"
sudo mkdir -p "$ROOTFS_MOUNT"

echo -e "${GREEN}[✔] Mount points created:${NC}"
echo -e "    ${YELLOW}${BOOT_MOUNT}${NC}"
echo -e "    ${YELLOW}${ROOTFS_MOUNT}${NC}"

# ── Step 7: Mount partitions ─────────────────────────────────
echo -e "${CYAN}[*] Mounting ${PART1} → ${BOOT_MOUNT}${NC}"
sudo mount "$PART1" "$BOOT_MOUNT"
echo -e "${GREEN}[✔] boot partition mounted${NC}"

echo -e "${CYAN}[*] Mounting ${PART2} → ${ROOTFS_MOUNT}${NC}"
sudo mount "$PART2" "$ROOTFS_MOUNT"
echo -e "${GREEN}[✔] rootfs partition mounted${NC}"

# ── Step 8: Verify with lsblk ────────────────────────────────
echo ""
echo -e "${CYAN}[*] Verifying block device layout...${NC}"
lsblk "$LOOP_DEV"

# ── Done ─────────────────────────────────────────────────────
echo ""
echo -e "${GREEN}══════════════════════════════════════════════════${NC}"
echo -e "${GREEN} Virtual SD Card Setup Complete!${NC}"
echo -e "${GREEN}══════════════════════════════════════════════════${NC}"
echo -e "  Loop Device : ${YELLOW}${LOOP_DEV}${NC}"
echo -e "  Boot  (p1)  : ${YELLOW}${PART1}  →  mounted at ${BOOT_MOUNT}${NC}"
echo -e "  RootFS (p2) : ${YELLOW}${PART2}  →  mounted at ${ROOTFS_MOUNT}${NC}"
echo -e "${GREEN}══════════════════════════════════════════════════${NC}"
echo ""
echo -e "${YELLOW}[!] To unmount when done, run:${NC}"
echo -e "    sudo umount ${BOOT_MOUNT} ${ROOTFS_MOUNT}"
echo -e "    sudo losetup -d ${LOOP_DEV}"
