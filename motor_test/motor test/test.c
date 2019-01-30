#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define PWM_OPEN		1
#define PWM_STOP			0
#define PWM_IOCTL_SET_FREQ_ID_2         3
#define PWM_IOCTL_SET_RATIO_ID_2        4
#define PWM_IOCTL_STOP_ID_2             5
#define PWM_IOCTL_SET_FREQ_ID_3         6
#define PWM_IOCTL_SET_RATIO_ID_3        7
#define PWM_IOCTL_STOP_ID_3             8
#define GPIO_IOCTL_BACK_ID_2            9
#define GPIO_IOCTL_BACK_ID_3            10
#define GPIO_IOCTL_STOP_ID_2            11
#define GPIO_IOCTL_STOP_ID_3            12
#define FORWARD                         13
#define BACKWARD                        14
#define FAST_RIGHT                      15
#define FAST_LEFT                       16
#define SLOW_RIGHT                      17
#define SLOW_LEFT                       18
#define CLOCKWISE                       19
#define COUNTER_CLOCKWISE               20
#define EMERGENCY_STOP                  21
#define ultrasound_MAJOR 235
#define SET_PING_CM  _IOR(ultrasound_MAJOR,3,int)
#define GET_PING_CM  _IOW(ultrasound_MAJOR,4,int)
#define DEVICE_BLTEST "/dev/IR_track"

static int  ioctl_buf,temp;

void* thread_function_a(void *arg);
void* thread_function_b(void *arg);
int running_a=1;
int running_b=1;
int outer_wheel_ratio_fast;
int inner_wheel_ratio_fast;
int outer_wheel_ratio_slow;
int inner_wheel_ratio_slow;
int fd=-1;
int fd_ultrasound;
int fd_infrared;
int a,b,c,d;
int mode_number;
int command_number;
int ret;
void mannual_control(void);
void infrared_control(void);
void ultrasound_control(void);
int front_distance;
int left_distance;
int right_distance;

int main(int argc, char **argv)
{
	fd = open("/dev/motor", O_RDONLY);
	if (fd < 0) {
                perror("open pwm_motor device");
                exit(1);
        }
	printf("Please set the ratio of Outer Wheel for Fast Right & Left\n");
	scanf("%d",&outer_wheel_ratio_fast);
	printf("Please set the ratio of Inner Wheel fot Fast Right & Left\n");
	scanf("%d",&inner_wheel_ratio_fast);
	printf("Please set the ratio of Outer Wheel fot Slow Right & Left\n");
        scanf("%d",&outer_wheel_ratio_slow);
        printf("Please set the ratio of Inner Wheel fot Slow Right & Left\n");
        scanf("%d",&inner_wheel_ratio_slow);
	do{
	printf("Please choose the control mode :\n");
	printf("(1)Mannual Control Mode (2)Infrared Control Mode (3)Ultrasound Control Mode\n");
	scanf("%d",&mode_number);
	switch(mode_number){
		case 1:
			printf("----------Mannual Control----------\n");
			mannual_control();
			break;
		case 2:
			printf("----------Infrared Control----------\n");
			infrared_control();
			break;
		case 3:
			printf("----------Ultrasound Control----------\n");
			ultrasound_control();
			break;
		default:
			printf("You have entered wrong number\n");
			exit(1);
	}
	printf("Press 1 for other Control Mode,Or the test will be ended\n");
	scanf("%d",&a);
	}while(a==1);
	close(fd);
	return 0;
}
	   
void mannual_control(void){

	do{
		printf("(1)Move Forward\n");
		printf("(2)Move Backward\n");
		printf("(3)Fast Right\n");
		printf("(4)Fast Left\n");
		printf("(5)Slow Right\n");
		printf("(6)Slow Left\n");
		printf("(7)Clockwise\n");
		printf("(8)Counter Clockwise\n");
		printf("(9)Emergency Stop\n");
		scanf("%d",&command_number);
		switch(command_number){

		case 1:
			printf("Move Forward\n");
			ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,100);
			ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,100);
			ret = ioctl(fd,FORWARD);
                		if(ret < 0) {
                		perror("Move Forward Failed");
                		exit(1);
        		}
			break;
		
		case 2:
                      	printf("Move Backward\n");
			ret = ioctl(fd,BACKWARD);
                                if(ret < 0) {
                                perror("Move Backward Failed");
                                exit(1);
                        }
			break;
		
		case 3:
			printf("Fast Right\n");
			ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,outer_wheel_ratio_fast);
			ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,inner_wheel_ratio_fast);
			ret = ioctl(fd,FAST_RIGHT);
                                if(ret < 0) {
                                perror("Fast Right Failed");
                                exit(1);
                        }
                        break;
		
		case 4:
			printf("Fast Left\n");
			ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,inner_wheel_ratio_fast);
                        ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,outer_wheel_ratio_fast);
                        ret = ioctl(fd,FAST_LEFT);
                                if(ret < 0) {
                                perror("Fast Left Failed");
                                exit(1);
                        }
                        break;
		
		case 5:
			printf("Slow Right\n");
			ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,outer_wheel_ratio_slow);
			ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,inner_wheel_ratio_slow);
			ret = ioctl(fd,SLOW_RIGHT);
                                if(ret < 0) {
                                perror("Slow Right Failed");
                                exit(1);
                        }
                        break;
		
		case 6:
			printf("Slow Left\n");
			ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,inner_wheel_ratio_slow);
			ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,outer_wheel_ratio_slow);
			ret = ioctl(fd,SLOW_LEFT);
                                if(ret < 0) {
                                perror("Slow Left Failed");
                                exit(1);
                        }
                        break;
		
		case 7:
			printf("Clockwise\n");
			ret = ioctl(fd,CLOCKWISE);
                                if(ret < 0) {
                                perror("Clockwise Failed");
                                exit(1);
                        }
                        break;

		case 8:
			printf("Counter Clockwise\n");
			ret = ioctl(fd,COUNTER_CLOCKWISE);
                                if(ret < 0) {
                                perror("Counter Clockwise Failed");
                                exit(1);
                        }
                        break;
		
		case 9:
			printf("Emergency Stop\n");
			ret = ioctl(fd,EMERGENCY_STOP);
                                if(ret < 0) {
                                perror("Emergency Stop Failed");
                                exit(1);
                        }
                        break;
		
		default:
			printf("You have entered wrong number!\n");
			exit(1);
		}
	printf("(1)Continue to Mannual Control (2)Exit Mannual Control Mode\n");
	scanf("%d",&d);
	}while(d==1);
	ret = ioctl(fd,EMERGENCY_STOP);
        	if(ret < 0) {
                perror("Emergency Stop Failed");
                exit(1);
        }
}

void infrared_control(void){
	pthread_t b_thread;
	fd_infrared = open(DEVICE_BLTEST,O_RDONLY);
	if(fd_infrared < 0){
		perror("open infrared device");
		exit(1);
	}
	
	ret = pthread_create(&b_thread,NULL,thread_function_b,NULL);
	if(ret!=0){
		perror("Thread b creation failed");
		exit(EXIT_FAILURE);
	}
	do{
		scanf("%d",&running_b);
		if(running_b==2){
		pthread_cancel(b_thread);
		close(fd_infrared);
		}
	}while(running_b==1);
	ret = ioctl(fd,EMERGENCY_STOP);
                if(ret < 0) {
                perror("Emergency Stop Failed");
                exit(1);
        }

}

void* thread_function_b(void *arg){
	int buf[6];
	int left,center,right;
	while(running_b){
	ret = read(fd_infrared,buf,sizeof(buf));
	if(ret<0){
		perror("Read fd_infrared Failed");
		exit(1);
	}
	left=buf[0];
	center=buf[1];
	right=buf[2];
	
	if(left==0&&center==0&&right==0){
	printf("Emergency Stop\n");
	ret = ioctl(fd,EMERGENCY_STOP);
        	if(ret < 0) {
                perror("Emergency Stop Failed");
                exit(1);
        }
	}else if(left==0&&center==0&&right==1){
	printf("Fast Right\n");
	ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,outer_wheel_ratio_fast);
       	ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,inner_wheel_ratio_fast);
        ret = ioctl(fd,FAST_RIGHT);
                if(ret < 0) {
                perror("Fast Right Failed");
                exit(1);
        }
	}else if(left==0&&center==1&&right==0){
	printf("Move Forward\n");
	ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,100);
	ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,100);
	ret = ioctl(fd,FORWARD);
        	if(ret < 0) {
                perror("Move Forward Failed");
                exit(1);
        }
	}else if(left==0&&center==1&&right==1){
	printf("Slow Right\n");
        ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,outer_wheel_ratio_slow);
        ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,inner_wheel_ratio_slow);
        ret = ioctl(fd,SLOW_LEFT);
        	if(ret < 0) {
                perror("Slow Left Failed");
                exit(1);
        }
	}else if(left==1&&center==0&&right==0){
	printf("Fast Left\n");
	ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,inner_wheel_ratio_fast);
        ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,outer_wheel_ratio_fast);
        ret = ioctl(fd,FAST_LEFT);
        	if(ret < 0) {
              	perror("Fast Left Failed");
                exit(1);
        }
	}else if(left==1&&center==0&&right==1){
	printf("Emergency Stop\n");
	ret = ioctl(fd,EMERGENCY_STOP);
                if(ret < 0) {
               	perror("Emergency Stop Failed");
                exit(1);
        }
	}else if(left==1&&center==1&&right==0){
	printf("Slow Left\n");
	ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,inner_wheel_ratio_slow);
        ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,outer_wheel_ratio_slow);
        ret = ioctl(fd,SLOW_LEFT);
        	if(ret < 0) {
                perror("Slow Left Failed");
                exit(1);
        }
	}else if(left==1&&center==1&&right==1){
	printf("Emergency Stop\n");
	ret = ioctl(fd,EMERGENCY_STOP);
        	if(ret < 0) {
               	perror("Emergency Stop Failed");
                exit(1);
        }
	}
	}
	pthread_exit(NULL);
}
	
void ultrasound_control(void){
	pthread_t a_thread;
	void *thread_result;
	fd_ultrasound = open("/dev/ultrasound", O_RDONLY);
        if (fd_ultrasound < 0) {
                perror("open ultrasound device");
                exit(1);
        }
	printf("Enter how many cm to ping--[1cm~350cm]-->\n");
	scanf("%d",&ioctl_buf);
	temp=ioctl_buf;
	
	ret = pthread_create(&a_thread,NULL,thread_function_a,NULL);
	if(ret!=0){
                perror("Thread a creation failed");
                exit(EXIT_FAILURE);
        }
	do{
		scanf("%d",&running_a);
		if(running_a==2){
		pthread_cancel(a_thread);
		close(fd_ultrasound);
		}
	}while(running_a==1);
	ret = ioctl(fd,EMERGENCY_STOP);
                if(ret < 0) {
                perror("Emergency Stop Failed");
                exit(1);
        }
	
	
}
		
void* thread_function_a(void *arg){
	while(running_a){
	ioctl_buf=temp;
	if ( ioctl(fd_ultrasound,SET_PING_CM, &ioctl_buf) < 0 ) {
        printf("ioctl SET_PING_CM failed\n");
        exit(-1);
        }
	ioctl_buf/=29;
	front_distance=ioctl_buf;

//	ret = read(fd_ultrasound,&front_distance,sizeof(int));
//	        if (ret < 0) {
//                perror("read ultrasound device");
//                exit(1);
//        }
//	front_distance=front_distance/29;
	printf("----------Obstacle At Front %d cm----------\n",front_distance);
	if(front_distance>=80){
		printf("Move Forward Full Speed\n");
		ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,100);
		ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,100);
                ret = ioctl(fd,FORWARD);
                if(ret < 0) {
                	perror("Move Forward Failed");
                        exit(1);
                }
	}else if(front_distance>=50&&front_distance<80){
		printf("Move Forward 80% Speed\n");
		ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,80);
		ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,80);
		ret = ioctl(fd,FORWARD);
		if(ret < 0) {
			perror("Move Forward 80%Failed");
			exit(1);
		}
	}else if(front_distance>=30&&front_distance<50){
		printf("Move Forward 50% Speed\n");
		ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,50);
		ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,50);
		ret = ioctl(fd,FORWARD);
                if(ret < 0) {
                        perror("Move Forward 50%Failed");
                        exit(1);
                }
	}else if(front_distance>=10&&front_distance<30){
		printf("Move Forward 30% Speed\n");
		ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,30);
		ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,30);
		ret = ioctl(fd,FORWARD);
                if(ret < 0) {
                        perror("Move Forward 30%Failed");
                        exit(1);
                }
	}else if(front_distance<10){
		printf("Emergency Stop\n");
		ret = ioctl(fd,EMERGENCY_STOP);
                if(ret < 0) {
                        perror("Emergency Stop Failed");
                        exit(1);
                }
	}
	}
	pthread_exit(NULL);
}
	
	

	


