#include "motor.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#define PWM_OPEN                1
#define PWM_STOP                        0
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

int fd;
int ret;
int outer_wheel_ratio_fast=100;
int inner_wheel_ratio_fast=30;
int outer_wheel_ratio_slow=100;
int inner_wheel_ratio_slow=50;
int res;
int running=1;
char buffer_write[BUFSIZ];
char buffer_read[BUFSIZ];

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_setWheelRatio
  (JNIEnv *env, jclass mc,jint a,jint b,jint c,jint d)
{
 outer_wheel_ratio_fast=a;
 inner_wheel_ratio_fast=b;
 outer_wheel_ratio_slow=c;
 inner_wheel_ratio_slow=d;
 return 1;
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_openMotor
  (JNIEnv *env, jclass mc)
{
 fd = open("/dev/motor", O_RDONLY);
 return fd;
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_closeMotor
  (JNIEnv *env, jclass mc)
{
 close(fd);
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_forward
  (JNIEnv *env, jclass mc)
{
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,100);
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,100);
 ret = ioctl(fd,FORWARD);
 return ret;
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_backward
  (JNIEnv *env, jclass mc)
{
 ret = ioctl(fd,BACKWARD);
 return ret;
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_fastRight
  (JNIEnv *env, jclass mc)
{
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,outer_wheel_ratio_fast);
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,inner_wheel_ratio_fast);
 ret = ioctl(fd,FAST_RIGHT);
 return ret;
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_fastLeft
  (JNIEnv *env, jclass mc)
{
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,inner_wheel_ratio_fast);
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,outer_wheel_ratio_fast);
 ret = ioctl(fd,FAST_LEFT);
 return ret;
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_slowRight
  (JNIEnv *env, jclass mc)
{
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,outer_wheel_ratio_slow);
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,inner_wheel_ratio_slow);
 ret = ioctl(fd,SLOW_RIGHT);
 return ret;
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_slowLeft
  (JNIEnv *env, jclass mc)
{
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_2,inner_wheel_ratio_slow);
 ret = ioctl(fd,PWM_IOCTL_SET_RATIO_ID_3,outer_wheel_ratio_slow);
 ret = ioctl(fd,SLOW_LEFT);
 return ret;
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_clockwise
  (JNIEnv *env, jclass mc)
{
 ret = ioctl(fd,CLOCKWISE);
 return ret; 
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_counterClockwise
  (JNIEnv *env, jclass mc)
{
 ret = ioctl(fd,COUNTER_CLOCKWISE);
 return ret;
}

JNIEXPORT jint JNICALL Java_com_ni_android_socketServer_Linuxc_stop
  (JNIEnv *env, jclass mc)
{
 ret = ioctl(fd,EMERGENCY_STOP);
 return ret;
}


static const char *classPathName = "com/ni/android/socketServer/Linuxc";

static JNINativeMethod methods[] = {
	  {"openMotor", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_openMotor},
	  {"closeMotor", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_closeMotor},
	  {"forward", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_forward},
	  {"backward", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_backward},
	  {"fastRight", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_fastRight},
	  {"fastLeft", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_fastLeft},
	  {"slowRight", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_slowRight},
	  {"slowLeft", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_slowLeft},
	  {"clockwise", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_clockwise},
	  {"counterClockwise", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_counterClockwise},
	  {"stop", "()Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_stop},
	  {"setWheelRatio","(IIII)Ljava/lang/String;", (void*)Java_com_ni_android_socketServer_Linuxc_setWheelRatio}
};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        fprintf(stderr, "Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        fprintf(stderr, "RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }

  return JNI_TRUE;
}


// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    printf("JNI_OnLoad");

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        fprintf(stderr, "ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;

    if (registerNatives(env) != JNI_TRUE) {
        fprintf(stderr, "ERROR: registerNatives failed");
        goto bail;
    }

    result = JNI_VERSION_1_4;

bail:
    return result;
}
