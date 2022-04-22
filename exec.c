/* Add this above the "bad:" part in the exec.c file where all other child process 
attributes are mentioned */

curproc->priority = 2; //Giving child process default priority of 2
