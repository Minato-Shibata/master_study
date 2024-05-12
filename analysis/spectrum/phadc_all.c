#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <math.h>
#include <string.h>

#include "pcc.h"
#include "camlib.h"

#define LAMMASK 0xffff
#define TIMEOUT 1000

#define RECORDTIME 200
#define EVENTNUMBER 1000000

//#define N_OUTPUT 1
#define N_INPUT 1
#define N_PHADC 3
//#define N_CADC 21 
//#define N_TDC 9


volatile int wait = 0;
volatile int stop = 0;

void stop_handler(int dummy)
{
	stop = 1;
	fprintf(stderr, "OK, I'll terminate, please wait.\n");
}

int main(int argc, char **argv){
/*
	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(0);
	}
*/
	// stop handler
	struct sigaction sa;
	setvbuf(stdout, (char *)NULL, _IONBF,0);
	sa.sa_handler = stop_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	sigaction(SIGINT, &sa, 0);
	sigaction(SIGTERM, &sa, 0);
/*
	FILE * outfile;
	if((outfile=fopen(argv[1], "w")) == 0){     // ascii output
		fprintf(stderr, "outputfile open error : %s\n", argv[1]);
		exit(1);
	}
*/

//  FILE *outputfile;
//  FILE *clearfile;
//  clearfile=fopen("phadc.txt","w");
//  fclose(clearfile);
//  outputfile=fopen("phadc.txt","a"); 
  char write_data[20];
	/********************
		Buffer Setup 
	 ********************/
	int fd, status;
	int data, q, x;
	data = 0;
	q = 0;
	x = 0;

	// pipeline : clear
	const int npipe_clear = 3;
	int cmdbuf_clear[2*npipe_clear+2], replybuf_clear[2*npipe_clear+2];

	if( (status = cam_gen_init(npipe_clear, cmdbuf_clear)) < 0){
		fprintf(stderr, "cmdbuf cam_gen_init (clear) error...\n");
		exit(1);
	}

	if( (status = cam_gen_init(npipe_clear, replybuf_clear)) < 0){
		fprintf(stderr, "replybuf cam_gen_init (clear) error...\n");
		exit(1);
	}

	if( (status = cam_gen_cc(cmdbuf_clear, N_PHADC, 0, 9, 0)) < 0){
		fprintf(stderr, "OUTPUT clear data (clear) error...\n");
		exit(1);
	}  

	if( (status = cam_gen_cc(cmdbuf_clear, N_INPUT, 1, 9, 0)) < 0){
		fprintf(stderr, "INPUT clear data (clear) error...\n");
		exit(1);
	}
  
	/********************
		Initialize 
	 ********************/
	struct timeval start_time,end_time;

	gettimeofday(&start_time, 0);
	//fprintf(stdout, "started at %s", asctime(localtime(&(start_time.tv_sec))));

	if((fd = cam_open()) < 0){
		fprintf(stderr, "CAMAC open error\n");
		exit(1);
	}

	if( (status = cam_single_cc(fd, 25, 0, 17, &data, &x, &q)) < 0){
		fprintf(stderr, "CAMAC generate Z error...\n");
		exit(1);
	}

	if( (status = cam_single_cc(fd, 25, 0, 16, &data, &x, &q)) < 0){
		fprintf(stderr, "CAMAC generate C error...\n");
		exit(1);
	}

	if( (status = cam_single_cc(fd, 25, 0, 24, &data, &x, &q)) < 0){
		fprintf(stderr, "CAMAC clear inhibit error...\n");
		exit(1);
	}


	/********************
		Event Loop
	 ********************/
        int count=0;
	while(stop==0){

	// pipeline : clear
	if( (status = cam_exec_pio(fd, cmdbuf_clear, replybuf_clear)) < 0){  //clear executed 
	   fprintf(stderr, "cam_exec_pio (clear) error...\n");
	   exit(1);
	}

        // clear interrupt at Interrupt register
/*
            status = cam_single_cc(fd, N_INPUT, 1, 9, &data, &q, &x);
        if( status < 0) {
           fprintf(stderr, "cam_single_cc(fd, N_INPUT, 0, 9...) error...\n");
           return(0);
        }
*/
	// enable LAM at the controller
	data = 0xffff;
	if((status = cam_enable_lam(fd, data)) < 0){
	   fprintf(stderr, "enable LAM error\n");
       	   exit(1);
	}
/*
       data =0xffff;
        if ((status =cam_single_cc(fd,N_INPUT,0,17,&data,&q,&x))<0){
           fprintf(stderr,"INPUT LAM MASK error");
           exit(1);
        }

        // enable interrupt at Interrupt register
        if( (status = cam_single_cc(fd, N_INPUT, 0, 26, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "enable CADC lam  error...\n");
           return(0);
        }
*/     
        data=0xffff; 
        if( (status = cam_single_cc(fd, N_INPUT, 0,17, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "enable CADC lam  error...\n");
           return(0);
        }
        if( (status = cam_single_cc(fd, N_INPUT, 1,9, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "enable CADC lam  error...\n");
           return(0);
        }
        if( (status = cam_single_cc(fd, N_INPUT, 0, 26, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "enable CADC lam  error...\n");
           return(0);
        }
        if( (status = cam_single_cc(fd, N_INPUT, 1, 26, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "enable CADC lam  error...\n");
           return(0);
        }
        // wait LAM 
	int lam_pattern = 0;
        if((status = cam_wait_lam(fd, &lam_pattern, TIMEOUT)) < 0){
           fprintf(stderr, "cam_wait_lam error...\n");
           exit(1);
           }
	//fprintf(stderr, "lam pattern = %d\n", lam_pattern);
    /*    if( (status = cam_single_cc(fd, N_TDC, 0, 9, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "cam_single_cc(fd, N_TDC, 0, 9...) error...\n");
           return(0);
       }*/
/*
        data=0x5555;
        if( (status = cam_single_cc(fd, N_OUTPUT, 0, 16, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "cam_single_cc(fd, N_TDC, 0, 9...) error...\n");
           return(0);
       }
        if( (status = cam_single_cc(fd, N_OUTPUT, 0, 9, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "cam_single_cc(fd, N_TDC, 0, 9...) error...\n");
           return(0);
       }
       data=0xaaaa;
        if( (status = cam_single_cc(fd, N_OUTPUT, 0,16, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "cam_single_cc(fd, N_TDC, 0, 9...) error...\n");
           return(0);
       }
        if( (status = cam_single_cc(fd, N_OUTPUT, 0, 9, &data, &q, &x)) < 0 ) {
           fprintf(stderr, "cam_single_cc(fd, N_TDC, 0, 9...) error...\n");
           return(0);
       }
*/
        usleep(RECORDTIME);
        int signal_0,signal_1,signal_2,signal_3;
        if ( (status=cam_single_cc(fd, N_PHADC,0,0,&data,&q,&x)) <0 ){
           fprintf(stderr, "CADC ch1 read error ");
           exit(1);
        }
        signal_0=data;
        if ( (status=cam_single_cc(fd, N_PHADC,1,0,&data,&q,&x)) <0 ){
           fprintf(stderr, "CADC ch2 read error ");
           exit(1);
        }
        signal_1=data;
	if ( (status=cam_single_cc(fd, N_PHADC,2,0,&data,&q,&x)) <0 ){
           fprintf(stderr, "CADC ch3 read error ");
           exit(1);
        }
        signal_2=data;
        if ( (status=cam_single_cc(fd, N_PHADC,3,0,&data,&q,&x)) <0 ){
           fprintf(stderr, "CADC ch4 read error ");
           exit(1);
        }
        signal_3=data;
       	if ( signal_0>1 ){
          if (count%1000 ==0){
	   fprintf(stderr,"count; %d,  ch0; %d ch1; %d ch2; %d ch3; %d\n", count,signal_0,signal_1,signal_2,signal_3);
          }
	  snprintf(write_data,20,"%d %d %d %d",signal_0,signal_1,signal_2,signal_3);
//           fprintf(outputfile,write_data);
//           fprintf(outputfile,"\n");
           fprintf(stdout,write_data); 
	   fprintf(stdout,"\n");
           count=count+1;
        }
       if ((status=cam_single_cc(fd,N_INPUT,1,9,&data,&q,&x))<0){
          fprintf(stderr, "lam clear erro at ch1** ");
          exit(1);
	}

       if ((status=cam_single_cc(fd,N_PHADC,0,9,&data,&q,&x))<0){
          fprintf(stderr, "lam clear erro at ch1** ");
          exit(1);
	}
/*
        if (count%1000 ==0){
           fprintf(stderr, "status: %d of %d \n",count, EVENTNUMBER);
        }
*/
        if (count==EVENTNUMBER){
   	   gettimeofday(&end_time, 0);
	   fprintf(stderr, "acq. started at %s", asctime(localtime(&(start_time.tv_sec))));
	   fprintf(stderr, "acq. finished at %s", asctime(localtime(&(end_time.tv_sec))));
	   fprintf(stderr, "\a");
           exit(1);
        }

        }
	/********************
		Finalize 
	 ********************/
	// pipeline : clear
	if( (status = cam_exec_pio(fd, cmdbuf_clear, replybuf_clear)) < 0){
		fprintf(stderr, "cam_exec_pio (clear) error...\n");
		exit(1);
	}

	if((status = cam_close(fd)) < 0){
		fprintf(stderr, "CAMAC close error\n");
		exit(1);
	}

//	if(fclose(outputfile) < 0){
//		fprintf(stderr, "outputfile close error\n");
//		exit(1);
//	}

	return 0;
}
