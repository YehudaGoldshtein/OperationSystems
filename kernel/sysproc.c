#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  //Modify the exit system call to receive an additional argument oftype char* and save it in exit_msg.
  int n;
  argint(0, &n);
  //retrieve the exit message from the stack
  argstr(1, myproc()->exit_msg, 32);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  //another var for the exit message
  uint64 
  exit_msg_addr; // User-space pointer for the exit message
  argaddr(0, &p);
  argaddr(1, &exit_msg_addr);
  char exit_msg[32];
  
  int pid = wait(p, exit_msg);

    // Copy the exit message to user space
    if (pid >= 0 && exit_msg_addr != 0) {
        if (copyout(myproc()->pagetable, exit_msg_addr, exit_msg, strlen(exit_msg) + 1) < 0) {
            return -1;
        }
    }

    return pid;
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
uint64
sys_memsize(void)
{
  return myproc()->sz;
}