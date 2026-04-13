/* Processing command line using getop
da pág. 46 de Interprocess Communication in Unix, John Shapley Gray
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char	*optarg;
extern int	optind, opterr;


int main(int argc, char *argv[]) {
   int	c;
   static char	optstring[] = "abs:";

   opterr = 0; 	/* turn off automatic error messages */

   while ( (c = getopt(argc, argv, optstring)) != -1 )
      switch (c) {
		 case 'a':
			printf("Found option a\n");
			break;
		 case 'b':
			printf("Found option b\n");
			break;
		 case 's':
			printf("Found option s\n");
			printf("       with an argument of %d \n", atoi(optarg));
			break;
		 case '?':
			printf("Found an option that was not in optstring\n");
      }

    printf(" optind = %d \n", optind);

    while (optind < argc) 
	   printf("Left  %s \n", argv[optind++] );

return (0);
}
