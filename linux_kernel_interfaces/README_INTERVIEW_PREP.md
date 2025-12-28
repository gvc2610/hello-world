# Linux Kernel Memory Interfaces: Interview Cheat Sheet

This project demonstrates the two primary ways to transfer data between User Space and Kernel Space.

## 1. The Standard Way: `copy_to/from_user`
Used in 90% of drivers (char devices, configuration, logs).

### The Mechanism
1.  **System Call:** User calls `write(fd, buf, len)`.
2.  **Context:** Kernel runs in **Process Context** of the calling app.
3.  **The Copy:** Kernel validates the user pointer (`access_ok`) and physically copies bytes from User RAM to Kernel RAM.

### Why not `memcpy`? (Interview Question)
* **Security:** `memcpy` doesn't check if the pointer is valid user memory.
* **Page Faults:** User memory might be swapped out. `copy_from_user` can sleep; `memcpy` cannot.
* **SMAP:** Modern CPUs block the kernel from touching user memory directly.

### Key Functions
* `copy_from_user(to, from, n)`: Returns 0 on success.
* `copy_to_user(to, from, n)`: Returns 0 on success.

---

## 2. The High Performance Way: `mmap` (Zero Copy)
Used for video, audio, high-speed data acquisition (ADC/FPGA).

### The Mechanism
Instead of copying data, we manipulate the **Page Tables** so that the User and Kernel see the **same physical RAM**.

### Key Functions
* `remap_pfn_range(vma, addr, pfn, size, prot)`
    * **VMA:** Virtual Memory Area (The user's request).
    * **PFN:** Page Frame Number (`Physical Address >> 12`).

### Quick Setup
See the Makefiles in the subdirectories.
