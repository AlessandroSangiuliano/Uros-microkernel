Uros microkernel actually is in early stage. It will become microkernel when an IPC system will be implented.
With an IPC and when the system will be able to start user processes all the in-kernel code that can be moved in user space, will be moved! 

The idea:

- IPC: fixed size send/recive well small chunk of data can be passed between processes. For big chunk of data usage of shared memory via kernel capabilities.

Each process will have a port that is a message queue (and maybe also a set of ports if necessary, like Mach)

- Kernel Capabilities: Something like granting from process A to process B the capability to do something like reading/writing a chunk of memory, and so on.

- Badges: Badges are needed to mark who is the sender of a message and the granter of a capability. In this way if a server receives more messages from differents clients, badges will identify who is the sender to reply!

Actual Status:

- x86 32 bit support
- Can read from a disk
- Idt loading
- GDT loading
- Few interrupt handlers (really few actually)

Next implementation:
- FAT16 filesystem (in progress).