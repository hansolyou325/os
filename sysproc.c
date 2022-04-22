//Add this where the other system calls are defined in syscall.c
extern int sys_cps(void);
extern int sys_chpr(void);

/* 
  .
  .
  .
*/

//Add this inside static int (*syscalls[])(void)
[SYS_cps]     sys_cps,
[SYS_chpr]    sys_chpr,

//Add this in the end of the sysproc.c file

int
sys_cps(void)
{
  return cps();
}

int
sys_chpr(void)
{
  int pid, pr;
  if(argint(0, &pid) < 0)
    return -1;
  if(argint(1, &pr) < 0)
    return -1;

  return chpr(pid, pr);
}


