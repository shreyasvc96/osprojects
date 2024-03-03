#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	//When no command line arguments are passed
	if(argc<=1){
		printf(2,"Please enter some command line arguments\n");
		//return 0;
		exit();
	}

	//printf(1,"No. of Command Line Arguments Passed: %d\n",argc-1);
	//int i;
	int timeGiven;
	
		//printf(1,"Time passed: %d\n",atoi(argv[1]));
		timeGiven = atoi(argv[1]);
		sleep(timeGiven);


	//return 0;
	exit();

}
