//Add this in the end of the proc.c file

int
cps()
{
struct proc *p;
//Enables interrupts on this processor.
sti();

//Loop over process table looking for process with pid.
acquire(&ptable.lock);
cprintf("name \t pid \t state \t priority \n");
for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
  if(p->state == SLEEPING)
	  cprintf("%s \t %d \t SLEEPING \t %d \n ", p->name,p->pid,p->priority);
	else if(p->state == RUNNING)
 	  cprintf("%s \t %d \t RUNNING \t %d \n ", p->name,p->pid,p->priority);
	else if(p->state == RUNNABLE)
 	  cprintf("%s \t %d \t RUNNABLE \t %d \n ", p->name,p->pid,p->priority);
}
release(&ptable.lock);
return 22;
}

int 
chpr(int pid, int priority)
{
	struct proc *p;
	acquire(&ptable.lock);
	for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
	  if(p->pid == pid){
			p->priority = priority;
			break;
		}
	}
	release(&ptable.lock);
	return pid;
}

//Add this under the "found:" part of the allocproc function in proc.c

found:
  p->state = EMBRYO;
  p->pid = nextpid++;
  p->priority = 10;  //Default Priority of a process is set to be 10

//Replace the scheduler function with the one below for priority round robin scheduling

void
scheduler(void)
{
  struct proc *p, *p1;
  struct cpu *c = mycpu();
  c->proc = 0;
  
  for(;;){
    // Enable interrupts on this processor.
    sti();
    struct proc *highP;
    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      highP = p;
      //choose one with highest priority
      for(p1 = ptable.proc; p1 < &ptable.proc[NPROC]; p1++){
	if(p1->state != RUNNABLE)
	  continue;
	if(highP->priority > p1->priority)   //larger value, lower priority
	  highP = p1;
      }
      p = highP;
      c->proc = p;
      switchuvm(p);
      p->state = RUNNING;

      swtch(&(c->scheduler), p->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      c->proc = 0;
    }
    release(&ptable.lock);

  }
}
