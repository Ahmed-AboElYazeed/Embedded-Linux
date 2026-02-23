## Raspberry Pi

##### There are **TWO possible boot paths**:

1. **Direct Linux Boot (No U-Boot)**

​	BootROM → bootcode.bin → start.elf → load kernel + DTB → jump → CPU runs kernel.

2. **Boot Using U-Boot**

   BootROM → bootcode.bin → start.elf → load U-Boot → jump → CPU runs U-Boot
   → U-Boot loads kernel + DTB → jump → CPU runs kernel.

- | Feature          | Direct Boot    | U-Boot           |
  | ---------------- | -------------- | ---------------- |
  | Who loads kernel | start.elf      | U-Boot           |
  | CPU first runs   | Kernel         | U-Boot           |
  | Flexibility      | Low            | High             |
  | Debugging        | Harder         | Easier           |
  | Embedded use     | Simple systems | Advanced systems |

#### Raspberry Pi 3 Block Diagram

![Raspberry Pi 3 Block Diagram - element14 Community](https://community.element14.com/resized-image/__size/620x443/__key/communityserver-blogs-components-weblogfiles/00-00-00-00-86/0257.contentimage_5F00_78677.png)

### 🔷 Scenario 1: Direct Linux Boot (No U-Boot)

| Step | File / Stage                           | Loaded From  | Loaded To        | Loaded By          | CPU State    | What Happens                |
| ---- | -------------------------------------- | ------------ | ---------------- | ------------------ | ------------ | --------------------------- |
| 1    | BootROM (inside SoC)                   | Internal ROM | Internal cache   | Hardware           | ❌ CPU OFF    | GPU starts execution        |
| 2    | bootcode.bin                           | SD Card      | L2 cache / SRAM  | BootROM (GPU)      | ❌ CPU OFF    | Initializes SDRAM           |
| 3    | start.elf                              | SD Card      | DRAM             | bootcode.bin (GPU) | ❌ CPU OFF    | Main GPU firmware           |
| 4    | fixup.dat                              | SD Card      | Used by GPU      | start.elf          | ❌ CPU OFF    | Memory split configuration  |
| 5    | config.txt                             | SD Card      | Parsed (no load) | start.elf          | ❌ CPU OFF    | Boot settings               |
| 6    | cmdline.txt                            | SD Card      | Parsed (no load) | start.elf          | ❌ CPU OFF    | Kernel boot arguments       |
| 7    | *.dtb (Device Tree)                    | SD Card      | DRAM             | start.elf          | ❌ CPU OFF    | Hardware description        |
| 8    | overlays/*.dtbo (optional)             | SD Card      | DRAM             | start.elf          | ❌ CPU OFF    | DTB modifications           |
| 9    | kernel.img / kernel7.img / kernel8.img | SD Card      | DRAM             | start.elf          | ❌ → ✅ CPU ON | Kernel loaded               |
| 10   | Jump to kernel                         | —            | —                | start.elf          | ✅ CPU ON     | ARM starts executing kernel |

### 🔷 Scenario 2: Boot Using U-Boot

| Step | File / Stage               | Loaded From  | Loaded To       | Loaded By              | CPU State    | What Happens                           |
| ---- | -------------------------- | ------------ | --------------- | ---------------------- | ------------ | -------------------------------------- |
| 1    | BootROM (inside SoC)       | Internal ROM | Internal cache  | Hardware               | ❌ CPU OFF    | GPU starts execution                   |
| 2    | bootcode.bin               | SD Card      | L2 cache / SRAM | BootROM (GPU)          | ❌ CPU OFF    | Initializes SDRAM                      |
| 3    | start.elf                  | SD Card      | DRAM            | bootcode.bin (GPU)     | ❌ CPU OFF    | Main GPU firmware                      |
| 4    | fixup.dat                  | SD Card      | Used by GPU     | start.elf              | ❌ CPU OFF    | Memory split                           |
| 5    | config.txt                 | SD Card      | Parsed          | start.elf              | ❌ CPU OFF    | Boot configuration                     |
| 6    | u-boot.bin                 | SD Card      | DRAM            | start.elf              | ❌ → ✅ CPU ON | U-Boot loaded                          |
| 7    | Jump to U-Boot             | —            | —               | start.elf              | ✅ CPU ON     | ARM starts executing U-Boot            |
| 8    | cmdline.txt (optional)     | SD Card      | Parsed          | U-Boot OR passed later | ✅ CPU ON     | Boot args (sometimes embedded instead) |
| 9    | *.dtb                      | SD Card      | DRAM            | U-Boot                 | ✅ CPU ON     | Hardware description                   |
| 10   | overlays/*.dtbo (optional) | SD Card      | DRAM            | U-Boot                 | ✅ CPU ON     | DT adjustments                         |
| 11   | kernel Image               | SD Card      | DRAM            | U-Boot                 | ✅ CPU ON     | Kernel loaded                          |
| 12   | Jump to kernel             | —            | —               | U-Boot                 | ✅ CPU ON     | Kernel execution starts                |

#### 



## PC

in **PC boot architecture** there are **two main worlds**:

1. **Legacy BIOS boot**

   BIOS → MBR → Stage1 → Stage2 (GRUB) → Kernel → OS

   == **CPU → BIOS → MBR → GRUB → Kernel**

2. **Modern UEFI boot (used in almost all modern PCs)**

   UEFI → EFI Bootloader → Kernel → OS

   == **CPU → UEFI → EFI Bootloader → Kernel**

##### 🔥 Key Differences (BIOS vs UEFI)

| Feature         | BIOS                    | UEFI                                |
| --------------- | ----------------------- | ----------------------------------- |
| Boot style      | MBR                     | GPT                                 |
| Code size limit | 512 bytes (very small!) | No strict limit                     |
| Mode            | 16-bit → 32-bit         | Native 64-bit                       |
| Bootloader      | Required (multi-stage)  | Optional (can load kernel directly) |
| Speed           | Slower                  | Faster                              |
| Flexibility     | Low                     | Very high                           |

#### 🔷 Scenario 1: Legacy BIOS Boot (MBR-based)

| Step | Stage / File                      | Loaded From          | Loaded To     | Loaded By  | CPU State | Mode                  | What Happens                     |
| ---- | --------------------------------- | -------------------- | ------------- | ---------- | --------- | --------------------- | -------------------------------- |
| 1    | Reset Vector (0xFFFFFFF0)         | ROM (BIOS chip)      | CPU registers | Hardware   | ✅ CPU ON  | Real Mode (16-bit)    | CPU starts executing BIOS        |
| 2    | BIOS Firmware                     | Flash (Motherboard)  | RAM           | CPU        | ✅         | Real Mode             | POST (hardware init)             |
| 3    | Hardware Init (POST)              | —                    | —             | BIOS       | ✅         | Real Mode             | RAM, keyboard, disks initialized |
| 4    | Boot Device Selection             | CMOS / BIOS settings | —             | BIOS       | ✅         | Real Mode             | Selects HDD / USB                |
| 5    | MBR (Master Boot Record)          | Disk sector 0 (512B) | RAM (0x7C00)  | BIOS       | ✅         | Real Mode             | Loads first-stage bootloader     |
| 6    | Jump to MBR                       | —                    | —             | BIOS       | ✅         | Real Mode             | CPU executes MBR                 |
| 7    | Stage 1 Bootloader                | Same 512B            | RAM           | MBR code   | ✅         | Real Mode             | Very limited loader              |
| 8    | Stage 2 Bootloader (e.g., GRUB)   | Disk                 | RAM           | Stage 1    | ✅         | Real → Protected      | Loads full bootloader            |
| 9    | Switch to Protected Mode          | —                    | —             | Bootloader | ✅         | 32-bit                | Enables advanced memory          |
| 10   | Kernel Image (e.g., Linux kernel) | Disk (/boot)         | RAM           | Bootloader | ✅         | Protected             | Loads OS kernel                  |
| 11   | initrd / initramfs                | Disk                 | RAM           | Bootloader | ✅         | Protected             | Temporary root filesystem        |
| 12   | Kernel Parameters                 | Bootloader config    | RAM           | Bootloader | ✅         | Protected             | Passed to kernel                 |
| 13   | Jump to Kernel                    | —                    | —             | Bootloader | ✅         | Protected → Long Mode | Kernel starts execution          |



### 🔷 Scenario 2: Modern UEFI Boot (GPT-based)

| Step | Stage / File                                        | Loaded From         | Loaded To | Loaded By          | CPU State | Mode                          | What Happens          |
| ---- | --------------------------------------------------- | ------------------- | --------- | ------------------ | --------- | ----------------------------- | --------------------- |
| 1    | Reset Vector                                        | ROM (UEFI firmware) | CPU       | Hardware           | ✅ CPU ON  | Real Mode → quickly 32/64-bit | Start firmware        |
| 2    | UEFI Firmware                                       | Flash               | RAM       | CPU                | ✅         | 32/64-bit                     | Initializes system    |
| 3    | Hardware Init                                       | —                   | —         | UEFI               | ✅         | 64-bit                        | POST (modern version) |
| 4    | Boot Manager                                        | NVRAM               | —         | UEFI               | ✅         | 64-bit                        | Select boot entry     |
| 5    | EFI System Partition (ESP)                          | Disk (FAT32)        | —         | UEFI               | ✅         | 64-bit                        | Contains boot files   |
| 6    | EFI Bootloader (e.g., GRUB or Windows Boot Manager) | ESP                 | RAM       | UEFI               | ✅         | 64-bit                        | Loads .efi file       |
| 7    | Execute EFI Application                             | —                   | —         | UEFI               | ✅         | 64-bit                        | Bootloader runs       |
| 8    | Kernel Image                                        | Disk                | RAM       | Bootloader OR UEFI | ✅         | 64-bit                        | OS kernel loaded      |
| 9    | initramfs (Linux)                                   | Disk                | RAM       | Bootloader         | ✅         | 64-bit                        | Early filesystem      |
| 10   | Device Tree / ACPI Tables                           | Firmware            | RAM       | UEFI               | ✅         | 64-bit                        | Hardware description  |
| 11   | Kernel Parameters                                   | Bootloader config   | RAM       | Bootloader         | ✅         | 64-bit                        | Boot args             |
| 12   | Exit Boot Services                                  | —                   | —         | UEFI               | ✅         | 64-bit                        | OS takes control      |
| 13   | Jump to Kernel                                      | —                   | —         | Bootloader/UEFI    | ✅         | 64-bit                        | Kernel starts         |



# 🔷 Important Notes (High-Level Understanding) PC vs RPI

### 1. CPU Behavior

- Unlike Raspberry Pi:
  - CPU starts **immediately** (no GPU boot stage)
- Firmware (**BIOS/UEFI**) runs on CPU directly

------

### 2. Where is “bootcode.bin equivalent”?

| Raspberry Pi | PC Equivalent                   |
| ------------ | ------------------------------- |
| bootcode.bin | BIOS / UEFI firmware            |
| start.elf    | UEFI Boot Manager / BIOS loader |
| U-Boot       | GRUB                            |

------

### 3. Device Tree vs ACPI

- Raspberry Pi → **DTB (Device Tree)**
- PC → **ACPI tables (built into firmware)**

