# Mini Container Runtime

This project implements a lightweight container runtime in C using Linux system calls and kernel mechanisms.

## Features
- Multi-container execution
- Process isolation using clone()
- Filesystem isolation using chroot()
- Kernel module for monitoring
- Basic scheduling experiments

## How to Run

### 1. Build
cd boilerplate
make

### 2. Load Kernel Module
sudo insmod monitor.ko

### 3. Start Containers
sudo ./engine start alpha ./rootfs-alpha /bin/sleep 1000
sudo ./engine start beta ./rootfs-beta /bin/sleep 1000

### 4. Check Running Containers
ps aux | grep sleep

### 5. Stop Containers
sudo pkill sleep

## Project Structure
- engine.c → Container runtime
- monitor.c → Kernel module
- rootfs → Container filesystem
- cpu_hog → CPU workload

## Concepts Used
- clone() system call
- Linux namespaces
- chroot()
- Kernel modules
- Process scheduling

## Conclusion
This project demonstrates core operating system concepts such as process isolation, resource management, and kernel-level monitoring.
