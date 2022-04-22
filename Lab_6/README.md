# Lab 6: Program Security Detect

## 版號

```text
vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ g++ --version
g++ (Debian 10.2.1-6) 10.2.1 20210110
Copyright (C) 2020 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## 常見的記憶體操作問題

### Heap out-of-bounds read/write

- Source code

  ```cpp
  int main()
  {
      int *ptr = new int[2];
      ptr[2] = 5;
      return 0;
  }
  ```
- Valgrind report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=1_heap-out-of-bounds all valg
  g++ -std=c++11 -fsanitize=address 1_heap-out-of-bounds.cpp -o asan-1_heap-out-of-bounds
  g++ -std=c++11 1_heap-out-of-bounds.cpp -o 1_heap-out-of-bounds
  valgrind ./1_heap-out-of-bounds
  ==15975== Memcheck, a memory error detector
  ==15975== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
  ==15975== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
  ==15975== Command: ./1_heap-out-of-bounds
  ==15975== 
  ==15975== Invalid write of size 4
  ==15975==    at 0x109153: main (in /workspaces/NYCU-Software-Testing-2022/Lab_6/1_heap-out-of-bounds)
  ==15975==  Address 0x4d53c88 is 0 bytes after a block of size 8 alloc'd
  ==15975==    at 0x483950F: operator new[](unsigned long) (vg_replace_malloc.c:431)
  ==15975==    by 0x109146: main (in /workspaces/NYCU-Software-Testing-2022/Lab_6/1_heap-out-of-bounds)
  ==15975== 
  ==15975== 
  ==15975== HEAP SUMMARY:
  ==15975==     in use at exit: 8 bytes in 1 blocks
  ==15975==   total heap usage: 2 allocs, 1 frees, 72,712 bytes allocated
  ==15975== 
  ==15975== LEAK SUMMARY:
  ==15975==    definitely lost: 8 bytes in 1 blocks
  ==15975==    indirectly lost: 0 bytes in 0 blocks
  ==15975==      possibly lost: 0 bytes in 0 blocks
  ==15975==    still reachable: 0 bytes in 0 blocks
  ==15975==         suppressed: 0 bytes in 0 blocks
  ==15975== Rerun with --leak-check=full to see details of leaked memory
  ==15975== 
  ==15975== For lists of detected and suppressed errors, rerun with: -s
  ==15975== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
  ```
- ASan report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=1_heap-out-of-bounds all asan
  g++ -std=c++11 -fsanitize=address 1_heap-out-of-bounds.cpp -o asan-1_heap-out-of-bounds
  g++ -std=c++11 1_heap-out-of-bounds.cpp -o 1_heap-out-of-bounds
  ./asan-1_heap-out-of-bounds
  =================================================================
  ==16153==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000000018 at pc 0x55afc94c01ba bp 0x7ffe49562cb0 sp 0x7ffe49562ca8
  WRITE of size 4 at 0x602000000018 thread T0
      #0 0x55afc94c01b9 in main (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-1_heap-out-of-bounds+0x11b9)
      #1 0x7f04d20ead09 in __libc_start_main ../csu/libc-start.c:308
      #2 0x55afc94c00a9 in _start (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-1_heap-out-of-bounds+0x10a9)

  0x602000000018 is located 0 bytes to the right of 8-byte region [0x602000000010,0x602000000018)
  allocated by thread T0 here:
      #0 0x7f04d23347a7 in operator new[](unsigned long) ../../../../src/libsanitizer/asan/asan_new_delete.cpp:102
      #1 0x55afc94c0176 in main (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-1_heap-out-of-bounds+0x1176)
      #2 0x7f04d20ead09 in __libc_start_main ../csu/libc-start.c:308

  SUMMARY: AddressSanitizer: heap-buffer-overflow (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-1_heap-out-of-bounds+0x11b9) in main
  Shadow bytes around the buggy address:
    0x0c047fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0c047fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0c047fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0c047fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0c047fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  =>0x0c047fff8000: fa fa 00[fa]fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c047fff8010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c047fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c047fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c047fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c047fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  Shadow byte legend (one shadow byte represents 8 application bytes):
    Addressable:           00
    Partially addressable: 01 02 03 04 05 06 07 
    Heap left redzone:       fa
    Freed heap region:       fd
    Stack left redzone:      f1
    Stack mid redzone:       f2
    Stack right redzone:     f3
    Stack after return:      f5
    Stack use after scope:   f8
    Global redzone:          f9
    Global init order:       f6
    Poisoned by user:        f7
    Container overflow:      fc
    Array cookie:            ac
    Intra object redzone:    bb
    ASan internal:           fe
    Left alloca redzone:     ca
    Right alloca redzone:    cb
    Shadow gap:              cc
  ==16153==ABORTING
  make: *** [Makefile:12: asan] Error 1
  ```
- Result

  - Valgrind: YES
  - ASan: YES

### Stack out-of-bounds read/write

- Source code

  ```cpp
  #include <iostream>
  int main()
  {
      int array[1];
      array[2] = 1;
      return 0;
  }
  ```
- Valgrind report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=2_stack-out-of-bounds all valg 
  g++ -std=c++11 -fsanitize=address 2_stack-out-of-bounds.cpp -o asan-2_stack-out-of-bounds
  g++ -std=c++11 2_stack-out-of-bounds.cpp -o 2_stack-out-of-bounds
  valgrind ./2_stack-out-of-bounds
  ==17290== Memcheck, a memory error detector
  ==17290== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
  ==17290== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
  ==17290== Command: ./2_stack-out-of-bounds
  ==17290== 
  ==17290== 
  ==17290== HEAP SUMMARY:
  ==17290==     in use at exit: 0 bytes in 0 blocks
  ==17290==   total heap usage: 1 allocs, 1 frees, 72,704 bytes allocated
  ==17290== 
  ==17290== All heap blocks were freed -- no leaks are possible
  ==17290== 
  ==17290== For lists of detected and suppressed errors, rerun with: -s
  ==17290== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
  ```
- ASan report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=2_stack-out-of-bounds all asan
  g++ -std=c++11 -fsanitize=address 2_stack-out-of-bounds.cpp -o asan-2_stack-out-of-bounds
  g++ -std=c++11 2_stack-out-of-bounds.cpp -o 2_stack-out-of-bounds
  ./asan-2_stack-out-of-bounds
  =================================================================
  ==18145==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7ffec25ce138 at pc 0x562d44dce267 bp 0x7ffec25ce100 sp 0x7ffec25ce0f8
  WRITE of size 4 at 0x7ffec25ce138 thread T0
      #0 0x562d44dce266 in main (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-2_stack-out-of-bounds+0x1266)
      #1 0x7fa69c962d09 in __libc_start_main ../csu/libc-start.c:308
      #2 0x562d44dce109 in _start (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-2_stack-out-of-bounds+0x1109)

  Address 0x7ffec25ce138 is located in stack of thread T0 at offset 40 in frame
      #0 0x562d44dce1d4 in main (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-2_stack-out-of-bounds+0x11d4)

    This frame has 1 object(s):
      [32, 36) 'array' (line 4) <== Memory access at offset 40 overflows this variable
  HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
        (longjmp and C++ exceptions *are* supported)
  SUMMARY: AddressSanitizer: stack-buffer-overflow (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-2_stack-out-of-bounds+0x1266) in main
  Shadow bytes around the buggy address:
    0x1000584b1bd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000584b1be0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000584b1bf0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000584b1c00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000584b1c10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  =>0x1000584b1c20: 00 00 f1 f1 f1 f1 04[f3]f3 f3 00 00 00 00 00 00
    0x1000584b1c30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000584b1c40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000584b1c50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000584b1c60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000584b1c70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  Shadow byte legend (one shadow byte represents 8 application bytes):
    Addressable:           00
    Partially addressable: 01 02 03 04 05 06 07 
    Heap left redzone:       fa
    Freed heap region:       fd
    Stack left redzone:      f1
    Stack mid redzone:       f2
    Stack right redzone:     f3
    Stack after return:      f5
    Stack use after scope:   f8
    Global redzone:          f9
    Global init order:       f6
    Poisoned by user:        f7
    Container overflow:      fc
    Array cookie:            ac
    Intra object redzone:    bb
    ASan internal:           fe
    Left alloca redzone:     ca
    Right alloca redzone:    cb
    Shadow gap:              cc
  ==18145==ABORTING
  make: *** [Makefile:12: asan] Error 1
  ```
- Result

  - Valgrind: NO
  - ASan: YES

### Global out-of-bounds read/write

- Source code

  ```cpp
  #include <iostream>
  using namespace std;
  int array[2] = {0};
  int main()
  {
      cout << array[2] << endl;
      return 0;
  }
  ```
- Valgrind report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=3_global-out-of-bounds all valg 
  g++ -std=c++11 -fsanitize=address 3_global-out-of-bounds.cpp -o asan-3_global-out-of-bounds
  g++ -std=c++11 3_global-out-of-bounds.cpp -o 3_global-out-of-bounds
  valgrind ./3_global-out-of-bounds
  ==20558== Memcheck, a memory error detector
  ==20558== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
  ==20558== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
  ==20558== Command: ./3_global-out-of-bounds
  ==20558== 
  0
  ==20558== 
  ==20558== HEAP SUMMARY:
  ==20558==     in use at exit: 0 bytes in 0 blocks
  ==20558==   total heap usage: 2 allocs, 2 frees, 73,728 bytes allocated
  ==20558== 
  ==20558== All heap blocks were freed -- no leaks are possible
  ==20558== 
  ==20558== For lists of detected and suppressed errors, rerun with: -s
  ==20558== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
  ```
- ASan report

  ```
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=3_global-out-of-bounds all asan
  g++ -std=c++11 -fsanitize=address 3_global-out-of-bounds.cpp -o asan-3_global-out-of-bounds
  g++ -std=c++11 3_global-out-of-bounds.cpp -o 3_global-out-of-bounds
  ./asan-3_global-out-of-bounds
  =================================================================
  ==20715==ERROR: AddressSanitizer: global-buffer-overflow on address 0x55c9babf2328 at pc 0x55c9babef20b bp 0x7ffccef2ad50 sp 0x7ffccef2ad48
  READ of size 4 at 0x55c9babf2328 thread T0
      #0 0x55c9babef20a in main (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-3_global-out-of-bounds+0x120a)
      #1 0x7fbf3a117d09 in __libc_start_main ../csu/libc-start.c:308
      #2 0x55c9babef119 in _start (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-3_global-out-of-bounds+0x1119)

  0x55c9babf2328 is located 0 bytes to the right of global variable 'array' defined in '3_global-out-of-bounds.cpp:3:5' (0x55c9babf2320) of size 8
  SUMMARY: AddressSanitizer: global-buffer-overflow (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-3_global-out-of-bounds+0x120a) in main
  Shadow bytes around the buggy address:
    0x0ab9b7576410: 00 00 00 00 00 00 00 00 00 00 00 00 f9 f9 f9 f9
    0x0ab9b7576420: f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9 f9
    0x0ab9b7576430: f9 f9 f9 f9 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ab9b7576440: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ab9b7576450: 00 00 00 00 00 00 00 00 00 00 00 00 01 f9 f9 f9
  =>0x0ab9b7576460: f9 f9 f9 f9 00[f9]f9 f9 f9 f9 f9 f9 00 00 00 00
    0x0ab9b7576470: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ab9b7576480: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ab9b7576490: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ab9b75764a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ab9b75764b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  Shadow byte legend (one shadow byte represents 8 application bytes):
    Addressable:           00
    Partially addressable: 01 02 03 04 05 06 07 
    Heap left redzone:       fa
    Freed heap region:       fd
    Stack left redzone:      f1
    Stack mid redzone:       f2
    Stack right redzone:     f3
    Stack after return:      f5
    Stack use after scope:   f8
    Global redzone:          f9
    Global init order:       f6
    Poisoned by user:        f7
    Container overflow:      fc
    Array cookie:            ac
    Intra object redzone:    bb
    ASan internal:           fe
    Left alloca redzone:     ca
    Right alloca redzone:    cb
    Shadow gap:              cc
  ==20715==ABORTING
  make: *** [Makefile:12: asan] Error 1
  ```
- Result

  - Valgrind: NO
  - ASan: YES

### Use-after-free

- Source code

  ```cpp
  int main()
  {
      int *array = new int[10];
      array[9] = 10;
      return 0;
  }
  ```
- Valgrind report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=4_use-after-free all valg 
  g++ -std=c++11 -fsanitize=address 4_use-after-free.cpp -o asan-4_use-after-free
  g++ -std=c++11 4_use-after-free.cpp -o 4_use-after-free
  valgrind ./4_use-after-free
  ==22361== Memcheck, a memory error detector
  ==22361== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
  ==22361== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
  ==22361== Command: ./4_use-after-free
  ==22361== 
  ==22361== Invalid read of size 4
  ==22361==    at 0x1091C4: main (in /workspaces/NYCU-Software-Testing-2022/Lab_6/4_use-after-free)
  ==22361==  Address 0x4d53c90 is 16 bytes inside a block of size 20 free'd
  ==22361==    at 0x483A58B: operator delete[](void*) (vg_replace_malloc.c:649)
  ==22361==    by 0x1091BB: main (in /workspaces/NYCU-Software-Testing-2022/Lab_6/4_use-after-free)
  ==22361==  Block was alloc'd at
  ==22361==    at 0x483950F: operator new[](unsigned long) (vg_replace_malloc.c:431)
  ==22361==    by 0x109196: main (in /workspaces/NYCU-Software-Testing-2022/Lab_6/4_use-after-free)
  ==22361== 
  10
  ==22361== 
  ==22361== HEAP SUMMARY:
  ==22361==     in use at exit: 0 bytes in 0 blocks
  ==22361==   total heap usage: 3 allocs, 3 frees, 73,748 bytes allocated
  ==22361== 
  ==22361== All heap blocks were freed -- no leaks are possible
  ==22361== 
  ==22361== For lists of detected and suppressed errors, rerun with: -s
  ==22361== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
  ```
- ASan report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=4_use-after-free all asan
  g++ -std=c++11 -fsanitize=address 4_use-after-free.cpp -o asan-4_use-after-free
  g++ -std=c++11 4_use-after-free.cpp -o 4_use-after-free
  ./asan-4_use-after-free
  =================================================================
  ==22705==ERROR: AddressSanitizer: heap-use-after-free on address 0x603000000020 at pc 0x56547c3ba2b2 bp 0x7ffe05250170 sp 0x7ffe05250168
  READ of size 4 at 0x603000000020 thread T0
      #0 0x56547c3ba2b1 in main (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-4_use-after-free+0x12b1)
      #1 0x7f7367063d09 in __libc_start_main ../csu/libc-start.c:308
      #2 0x56547c3ba149 in _start (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-4_use-after-free+0x1149)

  0x603000000020 is located 16 bytes inside of 20-byte region [0x603000000010,0x603000000024)
  freed by thread T0 here:
      #0 0x7f736747b127 in operator delete[](void*) ../../../../src/libsanitizer/asan/asan_new_delete.cpp:163
      #1 0x56547c3ba272 in main (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-4_use-after-free+0x1272)
      #2 0x7f7367063d09 in __libc_start_main ../csu/libc-start.c:308

  previously allocated by thread T0 here:
      #0 0x7f736747a7a7 in operator new[](unsigned long) ../../../../src/libsanitizer/asan/asan_new_delete.cpp:102
      #1 0x56547c3ba216 in main (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-4_use-after-free+0x1216)
      #2 0x7f7367063d09 in __libc_start_main ../csu/libc-start.c:308

  SUMMARY: AddressSanitizer: heap-use-after-free (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-4_use-after-free+0x12b1) in main
  Shadow bytes around the buggy address:
    0x0c067fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0c067fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0c067fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0c067fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0c067fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  =>0x0c067fff8000: fa fa fd fd[fd]fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fff8010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  Shadow byte legend (one shadow byte represents 8 application bytes):
    Addressable:           00
    Partially addressable: 01 02 03 04 05 06 07 
    Heap left redzone:       fa
    Freed heap region:       fd
    Stack left redzone:      f1
    Stack mid redzone:       f2
    Stack right redzone:     f3
    Stack after return:      f5
    Stack use after scope:   f8
    Global redzone:          f9
    Global init order:       f6
    Poisoned by user:        f7
    Container overflow:      fc
    Array cookie:            ac
    Intra object redzone:    bb
    ASan internal:           fe
    Left alloca redzone:     ca
    Right alloca redzone:    cb
    Shadow gap:              cc
  ==22705==ABORTING
  make: *** [Makefile:12: asan] Error 1
  ```
- Result

  - Valgrind: YES
  - ASan: YES

### Use-after-Return

- Source code

  ```cpp
  #include <iostream>
  using namespace std;
  int *ptr;
  void gg()
  {
      int lc = 5;
      ptr = &lc;
  }
  int main()
  {
      gg();
      cout << *ptr << endl;
      return 0;
  }
  ```
- Valgrind report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=5_use-after-return all valg 
  g++ -std=c++11 -fsanitize=address 5_use-after-return.cpp -o asan-5_use-after-return
  g++ -std=c++11 5_use-after-return.cpp -o 5_use-after-return
  valgrind ./5_use-after-return
  ==23941== Memcheck, a memory error detector
  ==23941== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
  ==23941== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
  ==23941== Command: ./5_use-after-return
  ==23941== 
  ==23941== Conditional jump or move depends on uninitialised value(s)
  ==23941==    at 0x496482C: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x4971E75: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x10919D: main (in /workspaces/NYCU-Software-Testing-2022/Lab_6/5_use-after-return)
  ==23941== 
  ==23941== Use of uninitialised value of size 8
  ==23941==    at 0x4964583: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x4964856: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x4971E75: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x10919D: main (in /workspaces/NYCU-Software-Testing-2022/Lab_6/5_use-after-return)
  ==23941== 
  ==23941== Conditional jump or move depends on uninitialised value(s)
  ==23941==    at 0x4964595: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x4964856: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x4971E75: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x10919D: main (in /workspaces/NYCU-Software-Testing-2022/Lab_6/5_use-after-return)
  ==23941== 
  ==23941== Conditional jump or move depends on uninitialised value(s)
  ==23941==    at 0x496488B: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x4971E75: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28)
  ==23941==    by 0x10919D: main (in /workspaces/NYCU-Software-Testing-2022/Lab_6/5_use-after-return)
  ==23941== 
  5
  ==23941== 
  ==23941== HEAP SUMMARY:
  ==23941==     in use at exit: 0 bytes in 0 blocks
  ==23941==   total heap usage: 2 allocs, 2 frees, 73,728 bytes allocated
  ==23941== 
  ==23941== All heap blocks were freed -- no leaks are possible
  ==23941== 
  ==23941== Use --track-origins=yes to see where uninitialised values come from
  ==23941== For lists of detected and suppressed errors, rerun with: -s
  ==23941== ERROR SUMMARY: 4 errors from 4 contexts (suppressed: 0 from 0)
  ```
- ASan report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=5_use-after-return all asan_return 
  g++ -std=c++11 -fsanitize=address 5_use-after-return.cpp -o asan-5_use-after-return
  g++ -std=c++11 5_use-after-return.cpp -o 5_use-after-return
  ASAN_OPTIONS=detect_stack_use_after_return=1 ./asan-5_use-after-return
  =================================================================
  ==24603==ERROR: AddressSanitizer: stack-use-after-return on address 0x7fc3e0072020 at pc 0x55b858461327 bp 0x7ffdae1cbab0 sp 0x7ffdae1cbaa8
  READ of size 4 at 0x7fc3e0072020 thread T0
      #0 0x55b858461326 in main (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-5_use-after-return+0x1326)
      #1 0x7fc3e3890d09 in __libc_start_main ../csu/libc-start.c:308
      #2 0x55b858461139 in _start (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-5_use-after-return+0x1139)

  Address 0x7fc3e0072020 is located in stack of thread T0 at offset 32 in frame
      #0 0x55b858461204 in gg() (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-5_use-after-return+0x1204)

    This frame has 1 object(s):
      [32, 36) 'lc' (line 6) <== Memory access at offset 32 is inside this variable
  HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
        (longjmp and C++ exceptions *are* supported)
  SUMMARY: AddressSanitizer: stack-use-after-return (/workspaces/NYCU-Software-Testing-2022/Lab_6/asan-5_use-after-return+0x1326) in main
  Shadow bytes around the buggy address:
    0x0ff8fc0063b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ff8fc0063c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ff8fc0063d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ff8fc0063e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ff8fc0063f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  =>0x0ff8fc006400: f5 f5 f5 f5[f5]f5 f5 f5 00 00 00 00 00 00 00 00
    0x0ff8fc006410: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ff8fc006420: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ff8fc006430: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ff8fc006440: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ff8fc006450: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  Shadow byte legend (one shadow byte represents 8 application bytes):
    Addressable:           00
    Partially addressable: 01 02 03 04 05 06 07 
    Heap left redzone:       fa
    Freed heap region:       fd
    Stack left redzone:      f1
    Stack mid redzone:       f2
    Stack right redzone:     f3
    Stack after return:      f5
    Stack use after scope:   f8
    Global redzone:          f9
    Global init order:       f6
    Poisoned by user:        f7
    Container overflow:      fc
    Array cookie:            ac
    Intra object redzone:    bb
    ASan internal:           fe
    Left alloca redzone:     ca
    Right alloca redzone:    cb
    Shadow gap:              cc
  ==24603==ABORTING
  make: *** [Makefile:15: asan_return] Error 1
  ```
- Result

  - Valgrind: YES
  - ASan: YES

## Redzone

- Source code

  ```cpp
  #include <iostream>
  using namespace std;
  int main()
  {
      int array_a[10], array_b[10];
      for (int i = 1; i <= 10; i++)
          *(&array_a[9] + 10 + i) = 5;
      for (auto n : array_b)
          cout << n << endl;
      *(&array_a[9] + 35) = 10;
      return 0;
  }
  ```
- ASan report

  ```text
  vscode ➜ /workspaces/NYCU-Software-Testing-2022/Lab_6 (lab6-add-program-security-detect ✗) $ make filename=6_redzone all asan
  g++ -std=c++11 -fsanitize=address 6_redzone.cpp -o asan-6_redzone
  g++ -std=c++11 6_redzone.cpp -o 6_redzone
  ./asan-6_redzone
  5
  5
  5
  5
  5
  5
  5
  5
  5
  5
  ```
- Result

  - ASan 掃不出錯誤，無論是下一個變數的位置或是 Redzone 之外都一樣。
