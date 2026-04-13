/* Displaying process group ID information 
 *  de Interprocess Communication in Unix, John Shapley Gray
 *  Alterações (MMMoura, 10/0ut/2005) para retirar warnings na compilação
*/
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int Pid, PidTerminou, Estado;

int main()
{
   int 			i;
   printf("\n\n Initial process \t PID %6d \t PPID %6d \t GID %6d \n\n",
				getpid(),  getppid(), getpgid(0) );
   for ( i = 0; i < 3; ++i)
      if ( fork() == 0){
	printf( "New process %d \t\t PID %6d \t PPID %6d \t GID %6d \n",
			    i,  getpid(), getppid(), getpgrp());
      } else {
      PidTerminou = wait( &Estado);
      }

   return(0);
}