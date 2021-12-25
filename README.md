Uros microkernel actually is in early stage. It will become microkernel when an IPC system will be implented.
With an IPC and when the system will be able to start user processes all the in-kernel code that can be moved in user space, will be moved! 

Actual Status:

- x86 32 bit support
- Can read from a disk
- Idt loading
- GDT loading
- Few interrupt handlers (really few actually)

Next implementation:
- FAT16 filesystem (in progress).