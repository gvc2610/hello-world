# Kernel Locking & Concurrency Lab

This project demonstrates the interaction between Interrupts (simulated via Timers), Workqueues, and the three major locking primitives: Spinlocks, Mutexes, and Semaphores.

---

## 1. The Theory: A "Cheat Sheet" Review

**Before you run the code, review this consolidated theory so you know what to look for.**

### A. The Three Contexts
1.  **Interrupt Context (Atomic)**
    * **What is it?** The CPU is actively handling a hardware signal (or simulated timer).
    * **CRITICAL RULE:** You **cannot sleep**. You cannot access user memory (copy_to_user).
    * **Used in:** Top Half (ISR), Timers.

2.  **Process Context (Kernel)**
    * **What is it?** The kernel is running code on behalf of a specific process (e.g., inside a `read()` system call) or a kernel thread (Workqueue).
    * **Rule:** You **can sleep**. You are associated with a valid task structure.

3.  **User Space**
    * **What is it?** Standard application code running in Ring 3.

+------------+---------------+-----------+-------------+--------------------------------------------------------------------------+
| Mechanism  | Type          | Can Sleep?| Ownership?  | Best Use Case                                                           |
+------------+---------------+-----------+-------------+--------------------------------------------------------------------------+
| Spinlock   | Busy Wait     | NO        | NO (CPU)    | Protecting data inside an ISR (Top Half). Fast, non-blocking            |
| Mutex      | Sleeping Lock | YES       | YES (Strict)| Protecting complex data in process context. Owner must unlock           |
| Semaphore  | Signaling     | YES*      | NO          | Signaling across contexts. Can be given in IRQ, but cannot block there  |
+------------+---------------+-----------+-------------+--------------------------------------------------------------------------+

---

## 2. The Architecture of this Demo

1.  **Simulated Hardware (Timer):**
    * Fires every 2000ms. Acts as our "Top Half" (ISR).
    * Uses **Spinlock** to protect a fast counter.
    * Uses **Semaphore** (`up()`) to signal that data is ready.
    * Schedules a **Workqueue** for post-processing.

2.  **Bottom Half (Workqueue):**
    * Runs in process context.
    * Uses **Mutex** to protect a slow, shared variable ("heavy processing").

3.  **User Space App:**
    * Reads from `/dev/locking_demo`.
    * Uses **Semaphore** (`down_interruptible()`) to **block (sleep)** until the Timer fires.

---

## 3. How to Compile & Run

1.  **Build the Module:**
    ```bash
    make
    ```

2.  **Load the Kernel Module:**
    ```bash
    sudo insmod locking_demo.ko
    # Check the logs to see the timer starting
    dmesg | tail
    ```

3.  **Create the Device Node:**
    Check `dmesg` to find the Major Number (e.g., 240).
    ```bash
    # Replace <MAJOR_NUMBER> with the number from dmesg
    sudo mknod /dev/locking_demo c <MAJOR_NUMBER> 0
    sudo chmod 666 /dev/locking_demo
    ```

4.  **Run the User App:**
    ```bash
    ./user_tester
    ```
    * **Observation:** You will see the app "hang" (sleep) for 2 seconds, then wake up immediately when the timer fires.

5.  **Clean Up:**
    ```bash
    sudo rmmod locking_demo
    make clean
    ```
