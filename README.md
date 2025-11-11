# 🏊‍♂️ Swimming Pool Synchronization (Semaphores & Shared Memory)

This project simulates swimmers sharing limited resources (cabins and baskets) using **System V semaphores** and **shared memory** in C.

## 🧠 Description
- `main.c`: Initializes semaphores and shared memory, then creates multiple swimmer processes.
- `nag.c`: Each swimmer requests a basket and a cabin, swims, then releases them.

## ⚙️ Compilation

You can compile the programs using `gcc`:

```bash
gcc main.c -o main
gcc nag.c -o nag
