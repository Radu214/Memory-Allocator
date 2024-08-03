# Memory Allocator

## Description

This project implements a memory allocator responsible for reserving and freeing memory at the library level, similar to `malloc()` or `calloc()` calls. The allocator uses a segregated free lists structure to manage a preallocated pool of memory. It handles memory allocation and deallocation, with functionality to read and write memory, as well as dump memory for diagnostic purposes.

## Getting Started

### Prerequisites

- `gcc` C compiler

### Installation

1. **Clone the Repository:**
   ```sh
   git clone https://github.com/Radu214/Memory-Allocator
   cd Memory-Allocator

2. **Build the Project:**
    ```sh
    make

3. **Run the Program:**
    ```sh
    make run
    
4. **Clean the Build**
    ```sh
    make clean

## Features

### Commands

The input is provided via stdin and output via stdout, following this format:

#### INIT_HEAP `<start_address>` `<num_lists>` `<bytes_per_list>` `<reconstitution_type>`
- Initializes the segregated free lists structure for a heap starting at `<start_address>`, with `<num_lists>` doubly linked lists each holding `<bytes_per_list>` bytes.
- Creates `<num_lists>` lists containing contiguous and increasing start addresses for each size.
- Lists will hold blocks sized in powers of 2, starting from 8.
- Addresses in the heap will range from `[heap_base, heap_base + heap_size)`.
- `<start_address>` is a positive, non-zero number.
- Ensures that the bytes for any list are at least equal to the assigned size.
- `<reconstitution_type>` should always be `0`

#### MALLOC `<num_bytes>`
- Allocates `<num_bytes>` of memory.
- Searches for a block in the lists with a size greater than or equal to `<num_bytes>` and removes it from the list.
- If multiple blocks of the exact size exist, chooses the one with the smallest address.
- If only larger blocks are available, chooses the smallest address block, fragments it, and allocates `<num_bytes>`, adding the remaining bytes back to the appropriate list.
- If no suitable block is found, outputs “Out of memory”.
- Marks the block as allocated in the doubly linked list for managing occupied memory zones.

#### FREE `<address>`
- Frees the block allocated at `<address>` based on the reconstitution policy set by INIT_HEAP.
- Searches for and removes `<address>` from the occupied memory structure.
- If the address was not already allocated or is not the start of a block, outputs “Invalid free”.
- Inserts the newly freed memory block back into the segregated free lists.
- `<reconstitution_type>` = `0`, adds it to the list corresponding to its size.

#### READ `<address>` `<num_bytes>`
- Displays `<num_bytes>` bytes from `<address>` in ASCII format.
- If no block starting from `<address>` was allocated, outputs “Segmentation fault (core dumped)” and performs a memory dump.
- If any byte in the range `[address, address + num_bytes)` was not allocated, outputs “Segmentation fault (core dumped)” and performs a memory dump.
- If all bytes to be displayed were allocated, outputs the requested content.

#### WRITE `<address>` `<data>` `<num_bytes>`
- Writes `<num_bytes>` bytes from the `<data>` string to `<address>`. If `<num_bytes>` exceeds the length of `<data>`, writes only `strlen(data)`.
- If no block starting from `<address>` was allocated, outputs “Segmentation fault (core dumped)” and performs a memory dump.
- If any byte in the range `[address, address + num_bytes)` was not allocated, outputs “Segmentation fault (core dumped)” and performs a memory dump.
- If the entire portion to be written to was fully allocated, writes the requested content.

#### DUMP_MEMORY
- Displays free and allocated zones along with the content of written blocks:
```sh
+++++DUMP+++++
Total memory: <heap_size> bytes
Total allocated memory: <allocated_bytes> bytes
Total free memory: <free_bytes> bytes
Number of free blocks: <num_free_blocks>
Number of allocated blocks: <num_allocated_blocks>
Number of malloc calls: <malloc_calls>
Number of fragmentations: <fragmentations>
Number of free calls: <free_calls>
Blocks with <x1> bytes - <num_x1_free_blocks> free block(s) : 0x<block_start_address_1> 0x<block_start_address_2> ... 0x<block_start_address_n>
...
Allocated blocks : (0x<block_start_address_1> - <block_size_1>) (0x<block_start_address_2> - <block_size_2>) ... (0x<block_start_address_n> - <block_size_n>)
-----DUMP-----
```
- Blocks are displayed in ascending order of size and, within lists of the same size, in ascending order of addresses.

#### DESTROY_HEAP
- Frees all allocated memory and terminates the program.

### 3. Error Handling

- **OUT_OF_MEMORY**: Outputs “Out of memory” if no block with the required size or larger is available for allocation.
- **INVALID_FREE**: Outputs “Invalid free” if trying to free an address that was not previously allocated or does not start a block.
- **SEGMENTATION_FAULT**: Outputs “Segmentation fault (core dumped)” and performs a memory dump if trying to read from or write to an unallocated address or partially allocated block.

## Example

### Input
```sh
INIT_HEAP 0x1 4 256 0
MALLOC 10
MALLOC 16
MALLOC 2000
WRITE 0x111 "GIGEL" 5
DUMP_MEMORY
READ 0x113 2
FREE 0x111
FREE 0x111
DESTROY_HEAP
```

### Output
```sh
Out of memory
+++++DUMP+++++
Total memory: 1024 bytes
Total allocated memory: 26 bytes
Total free memory: 998 bytes
Free blocks: 59
Number of allocated blocks: 2
Number of malloc calls: 2
Number of fragmentations: 1
Number of free calls: 0
Blocks with 6 bytes - 1 free block(s) : 0x10b
Blocks with 8 bytes - 32 free block(s) : 0x1 0x9 0x11 0x19 0x21 0x29 0x31 0x39 0x41 0x49 0x51 0x59 0x61 0x69 0x71 0x79 0x81 0x89 0x91 0x99 0xa1 0xa9 0xb1 0xb9 0xc1 0xc9 0xd1 0xd9 0xe1 0xe9 0xf1 0xf9
Blocks with 16 bytes - 14 free block(s) : 0x121 0x131 0x141 0x151 0x161 0x171 0x181 0x191 0x1a1 0x1b1 0x1c1 0x1d1 0x1e1 0x1f1
Blocks with 32 bytes - 8 free block(s) : 0x201 0x221 0x241 0x261 0x281 0x2a1 0x2c1 0x2e1
Blocks with 64 bytes - 4 free block(s) : 0x301 0x341 0x381 0x3c1
Allocated blocks : (0x101 - 10) (0x111 - 16)
-----DUMP-----
GE
Invalid free
```
