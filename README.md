# Prefetch Side-Channel Attack (x86_64 Assembly + C)

This project demonstrates a **microarchitectural side-channel attack** using the `prefetch` instruction to leak data from memory. It was developed as part of a pwn.college challenge and combines low-level assembly with a C-based brute-force harness.

---

## Overview

The goal of this challenge is to **extract a secret byte from a protected memory region** by exploiting timing differences introduced by CPU caching behavior.

The implementation consists of two components:
- **Assembly payload**: Performs the timing-based side-channel attack using `rdtsc` and `prefetcht2`.
- **C harness**: Automates execution, modifies shellcode dynamically, and extracts the leaked byte via process exit codes.

---

## Attack Concept

The attack relies on the fact that:
- `prefetch` instructions can bring data into cache without triggering faults.
- Access timing differs depending on whether a memory address is valid/mapped.
- These timing differences can be measured using `rdtsc`.

By scanning memory and measuring access latency, the program identifies a valid mapped region and then leaks a byte from it.

---

## Assembly Implementation

### Key Techniques

- **Timing Measurement**
  - Uses `rdtsc` before and after `prefetcht2` to measure memory access latency.
  - Combines `rdx` and `rax` to form a 64-bit timestamp.

- **Prefetch Side-Channel**
  - `prefetcht2 [r14]` is used to probe memory without causing a crash.
  - Cached vs non-cached access times are used to infer validity.

- **Averaging for Stability**
  - Each address is probed 20 times.
  - Average latency is computed to reduce noise.

- **Memory Scanning**
  - Iterates through memory range:
    ```
    0x2000000000 → 0x9000000000
    ```
  - Stops when latency falls below a threshold.

- **Data Exfiltration**
  - Once a valid region is found:
    - Reads byte at `[r14 + 1]`
    - Returns it via `exit` syscall

---

## C Harness

### Functionality

- Reads compiled shellcode from file: babyprefetch-shellcode-raw
- Modifies a specific byte (`buf[0x85]`) dynamically
- Writes updated shellcode back to disk
- Executes target binary: /challenge/babyarch_prefetchpeek
  - Captures leaked byte via process exit status

---

### Brute-Force Loop

- Repeatedly executes the challenge binary
- Filters valid ASCII outputs: 32 <= exit_code < 127
- Uses `nanosleep` to stabilize timing and reduce noise
- Prints recovered character

---

## Execution Flow
C Program
├── Load shellcode
├── Modify payload byte
├── Write back to file
├── Loop:
│ ├── Execute challenge binary
│ ├── Capture exit code
│ ├── Validate ASCII range
│ └── Print leaked byte
└── Repeat


---

## Key Concepts Demonstrated

### Microarchitectural Side Channels
- Exploiting CPU cache behavior for information leakage
- Timing-based inference using `rdtsc`

### Prefetch-Based Attacks
- Using `prefetcht2` to probe inaccessible memory
- Avoiding faults while still observing cache effects

### Noise Reduction
- Averaging multiple timing samples
- Introducing delays (`nanosleep`) for stability

### Low-Level Exploitation
- Pure assembly payload without libc
- Direct syscall usage for exfiltration

---
