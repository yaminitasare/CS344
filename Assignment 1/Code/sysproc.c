#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"



int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_draw(void){
  char *a = 
"    .--. \n"
"   |o_o |\n"
"   |:_/ |\n"
"  //   \\ \\\n"
" (|     | )\n"
" /'\\_ _/`\\\n"
" \\_/\\__/\n";

  void* buf;
  int size;

  if(argint(1,&size) < 0){
    return -1;
  }

  if(argptr(0,(char**)&buf,size) < 0){
    return -1;
  }
  int p = 0;
  for(p = 0; a[p] != '\0'; p++);

  if(p > size) p = -1;
  else {
    for(int i = 0; i < p; i++) ((char*)buf)[i] = a[i];
  }
  // returning size of wolfie
  // -1 is returned if buffer created is not enough to hold wolfie otherwise wolfie size is returned
  return p;
}