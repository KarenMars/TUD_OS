/*secure loader*/
/*SHA-256 signature*/
/*intended usage pattern*/

/*loader <secured binary> <public key>*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAXSIZE 256

int main(int argc, char const *argv[])
{	

	FILE *fp;

	char dump[MAXSIZE];
	char remove[MAXSIZE];
	char verify[MAXSIZE];

	if (argc<3)
	{
		fprintf(stderr, "too few inputs\n");
		exit(1);
	}
	if (argc>3)
	{
		fprintf(stderr, "too many inputs");
		exit(1);
	}
	if (argc==3)
	{
		printf("Secure loader......\n");
	}

	/*objcopy --dump-section <name>=<file>*/
	strcpy(dump,"objcopy --dump-section .sha=message_digest ");
	strcat(dump,argv[1]);
	//strcat(dump,">2&1");
	printf("DUMP\n");
	//fp=popen(dump,"r");
	/*SHA-256 256 bits*/
	/*check whether contain a valid secure digest or no digest*/
	/*system("objcopy --dump-section .sha=message_digest test_signed 2>&1");*/
	system(dump);

	strcpy(remove,"objcopy --remove-section .sha ");
	strcat(remove,argv[1]);
	system(remove);
	printf("REMOVE\n");
	/*verify*/

	strcpy(verify,"openssl dgst -verify public_key.pem -signature message_digest ");
	strcat(verify,argv[1]);
	system(verify);
	printf("...load successfully\n");
	/*output Verification OK or Verification Failure*/
	return 0;
}