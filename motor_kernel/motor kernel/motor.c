#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#define DEVICE_NAME			"motor"

#define PWM_IOCTL_SET_FREQ_ID_2		3
#define PWM_IOCTL_SET_RATIO_ID_2        4
#define PWM_IOCTL_STOP_ID_2		5
#define PWM_IOCTL_SET_FREQ_ID_3         6
#define PWM_IOCTL_SET_RATIO_ID_3        7
#define PWM_IOCTL_STOP_ID_3             8
#define GPIO_IOCTL_BACK_ID_2		9
#define GPIO_IOCTL_BACK_ID_3            10
#define GPIO_IOCTL_STOP_ID_2		11
#define GPIO_IOCTL_STOP_ID_3            12
#define FORWARD				13
#define BACKWARD			14
#define FAST_RIGHT			15
#define FAST_LEFT			16
#define SLOW_RIGHT			17
#define SLOW_LEFT  			18
#define CLOCKWISE			19
#define COUNTER_CLOCKWISE		20
#define EMERGENCY_STOP			21

#define NS_IN_1HZ				(1000000000UL)

#define MOTOR_PWM_ID_2			2
#define MOTOR_PMW_GPIO_2			EXYNOS4_GPD0(2)
#define MOTOR_PWM_ID_3			3
#define MOTOR_PMW_GPIO_3                        EXYNOS4_GPD0(3)

static struct pwm_device *pwmMotor2;
static struct pwm_device *pwmMotor3;
static struct semaphore lock;
static unsigned long freq2=1000;
static unsigned long freq3=1000;
static int ratio2=100;
static int ratio3=100;
//static int outer_wheel_ratio_fast=100;
//static int inner_wheel_ratio_fast=80;
//static int outer_wheel_ratio_slow=100;
//static int inner_wheel_ratio_slow=50;

static void pwm_set_freq_ID_2(unsigned long freq) {
	freq2=freq;
}

static void pwm_set_freq_ID_3(unsigned long freq) {
	freq3=freq;
}

static void pwm_set_ratio_ID_2(int ratio) {
	ratio2=ratio;
}

static void pwm_set_ratio_ID_3(int ratio) {
	ratio3=ratio;
}

static void pwm_start_ID_2(void){
	int period_ns = NS_IN_1HZ / freq2;

        pwm_config(pwmMotor2, period_ns*ratio2/100, period_ns);
        pwm_enable(pwmMotor2);
        s3c_gpio_cfgpin(MOTOR_PMW_GPIO_2, S3C_GPIO_SFN(2));
}

static void pwm_start_ID_3(void){
	int period_ns = NS_IN_1HZ / freq3;

        pwm_config(pwmMotor3, period_ns*ratio3/100, period_ns);
        pwm_enable(pwmMotor3);
        s3c_gpio_cfgpin(MOTOR_PMW_GPIO_3, S3C_GPIO_SFN(2));
}

static void gpio_start_ID_2(void){
	gpio_direction_output(EXYNOS4_GPX2(5),1);
}

static void gpio_start_ID_3(void){
	gpio_direction_output(EXYNOS4_GPX2(7),1);
}

static  void pwm_stop_ID_2(void) {
	s3c_gpio_cfgpin(MOTOR_PMW_GPIO_2, S3C_GPIO_OUTPUT);

	pwm_config(pwmMotor2, 0, NS_IN_1HZ / 100);
	pwm_disable(pwmMotor2);
}

static  void pwm_stop_ID_3(void) {
        s3c_gpio_cfgpin(MOTOR_PMW_GPIO_3, S3C_GPIO_OUTPUT);

        pwm_config(pwmMotor3, 0, NS_IN_1HZ / 100);
        pwm_disable(pwmMotor3);
}

static void stop_ID_2(void){
	s3c_gpio_cfgpin(MOTOR_PMW_GPIO_2, S3C_GPIO_OUTPUT);

        pwm_config(pwmMotor2, 0, NS_IN_1HZ / 100);
        pwm_disable(pwmMotor2);
	gpio_direction_output(EXYNOS4_GPX2(5),0);
}

static void stop_ID_3(void){
	s3c_gpio_cfgpin(MOTOR_PMW_GPIO_3, S3C_GPIO_OUTPUT);

        pwm_config(pwmMotor3, 0, NS_IN_1HZ / 100);
        pwm_disable(pwmMotor3);
	gpio_direction_output(EXYNOS4_GPX2(7),0);
}

static int Exynos4412_pwm_open(struct inode *inode, struct file *file) {
	if (!down_trylock(&lock)) 
		return 0;
	else
		return -EBUSY;
}

static int Exynos4412_pwm_close(struct inode *inode, struct file *file) {
	up(&lock);
	return 0;
}

static long Exynos4412_pwm_ioctl(struct file *filep, unsigned int cmd,
		unsigned long arg)
{
	switch (cmd) {
		case PWM_IOCTL_SET_FREQ_ID_2:
			if (arg == 0)
				return -EINVAL;
			else
				pwm_set_freq_ID_2(arg);
			break;

		case PWM_IOCTL_SET_FREQ_ID_3:
                        if (arg == 0)
                                return -EINVAL;
                        else
                                pwm_set_freq_ID_3(arg);
                        break;
		
		case PWM_IOCTL_SET_RATIO_ID_2:
			if (arg == 0)
				return -EINVAL;
			else
				pwm_set_ratio_ID_2(arg);
			break;
		
		case PWM_IOCTL_SET_RATIO_ID_3:
                        if (arg == 0)
                                return -EINVAL;
                        else
                                pwm_set_ratio_ID_3(arg);
                        break;
		
		case GPIO_IOCTL_BACK_ID_2:
			pwm_stop_ID_2();
			gpio_start_ID_2();
			break;
		
		case GPIO_IOCTL_BACK_ID_3:
                        pwm_stop_ID_3();
                        gpio_start_ID_3();
                        break;
	
		case PWM_IOCTL_STOP_ID_2:
			pwm_stop_ID_2();
			break;
		
		case PWM_IOCTL_STOP_ID_3:
                        pwm_stop_ID_3();
                        break;
	
		case GPIO_IOCTL_STOP_ID_2:
			gpio_direction_output(EXYNOS4_GPX2(5),0);
			break;
		
		case GPIO_IOCTL_STOP_ID_3:
                        gpio_direction_output(EXYNOS4_GPX2(7),0);
                        break;
		
		case FORWARD:
			stop_ID_2();
			stop_ID_3();
			pwm_set_ratio_ID_2(ratio2);
			pwm_set_ratio_ID_3(ratio3);
			pwm_start_ID_2();
			pwm_start_ID_3();	
			break;
		
		case BACKWARD:
			stop_ID_2();
			stop_ID_3();
			gpio_start_ID_2();
			gpio_start_ID_3();
			break;
		
		case FAST_RIGHT:
			stop_ID_2();
			stop_ID_3();
			pwm_set_ratio_ID_2(ratio2);
			pwm_set_ratio_ID_3(ratio3);
			pwm_start_ID_2();
			pwm_start_ID_3();
			break;
		
		case FAST_LEFT:
			stop_ID_2();
                        stop_ID_3();
                        pwm_set_ratio_ID_2(ratio2);
                        pwm_set_ratio_ID_3(ratio3);
                        pwm_start_ID_2();
                        pwm_start_ID_3();
			break;
		
		case SLOW_RIGHT:
			stop_ID_2();
                        stop_ID_3();
                        pwm_set_ratio_ID_2(ratio2);
                        pwm_set_ratio_ID_3(ratio3);
                        pwm_start_ID_2();
                        pwm_start_ID_3();
			break;
		
		case SLOW_LEFT:
			stop_ID_2();
                        stop_ID_3();
                        pwm_set_ratio_ID_2(ratio2);
                        pwm_set_ratio_ID_3(ratio3);
                        pwm_start_ID_2();
                        pwm_start_ID_3();
			break;
		
		case CLOCKWISE:
			stop_ID_2();
                        stop_ID_3();
			pwm_set_ratio_ID_2(100);
			pwm_start_ID_2();
			gpio_start_ID_3();
			break;
		
		case COUNTER_CLOCKWISE:
			stop_ID_2();
                        stop_ID_3();
                        pwm_set_ratio_ID_3(100);
                        pwm_start_ID_3();
                        gpio_start_ID_2();
			break;
		
		case EMERGENCY_STOP:
			stop_ID_2();
                        stop_ID_3();
			break;

		default:
			pwm_stop_ID_2();
			pwm_stop_ID_3();
			break;
	}

	return 0;
}

static  struct file_operations Exynos4412_pwm_ops = {
	.owner			= THIS_MODULE,  			
	.open			= Exynos4412_pwm_open,		
	.release		= Exynos4412_pwm_close,         
	.unlocked_ioctl	= Exynos4412_pwm_ioctl,
};

static struct miscdevice Exynos4412_misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &Exynos4412_pwm_ops,
};
static int __init Exynos4412_pwm_dev_init(void) {
	int ret;
	ret = gpio_request(MOTOR_PMW_GPIO_2, DEVICE_NAME);
	if (ret) {
		printk("request GPIO %d for pwm2 failed\n", MOTOR_PMW_GPIO_2);
		gpio_free(MOTOR_PMW_GPIO_2);
		return ret;
	}

	gpio_set_value(MOTOR_PMW_GPIO_2, 0);
	s3c_gpio_cfgpin(MOTOR_PMW_GPIO_2, S3C_GPIO_OUTPUT);

	pwmMotor2 = pwm_request(MOTOR_PWM_ID_2, DEVICE_NAME);
	if (IS_ERR(pwmMotor2)) {
		printk("request pwm %d for %s failed\n", MOTOR_PWM_ID_2, DEVICE_NAME);
		return -ENODEV;
	}

	pwm_stop_ID_2();
	
	ret = gpio_request(MOTOR_PMW_GPIO_3, DEVICE_NAME);
        if (ret) {
                printk("request GPIO %d for pwm3 failed\n", MOTOR_PMW_GPIO_3);
                gpio_free(MOTOR_PMW_GPIO_3);
                return ret;
        }

        gpio_set_value(MOTOR_PMW_GPIO_3, 0);
        s3c_gpio_cfgpin(MOTOR_PMW_GPIO_3, S3C_GPIO_OUTPUT);

        pwmMotor3 = pwm_request(MOTOR_PWM_ID_3, DEVICE_NAME);
        if (IS_ERR(pwmMotor3)) {
                printk("request pwm %d for %s failed\n", MOTOR_PWM_ID_3, DEVICE_NAME);
                return -ENODEV;
        }

        pwm_stop_ID_3();
	
	ret=gpio_request(EXYNOS4_GPX2(5),DEVICE_NAME);
        if(ret<0){
                printk(KERN_EMERG "EXYNOS4_GPX2(5) fail !\n");
                gpio_free(EXYNOS4_GPX2(5));
                return ret;
        }
	s3c_gpio_cfgpin(EXYNOS4_GPX2(5), S3C_GPIO_OUTPUT);
	
	ret=gpio_request(EXYNOS4_GPX2(7),DEVICE_NAME);
        if(ret<0){
                printk(KERN_EMERG "EXYNOS4_GPX2(7) fail !\n");
                gpio_free(EXYNOS4_GPX2(7));
                return ret;
        }
	s3c_gpio_cfgpin(EXYNOS4_GPX2(7), S3C_GPIO_OUTPUT);
	
	gpio_direction_output(EXYNOS4_GPX2(5),0);
        gpio_direction_output(EXYNOS4_GPX2(7),0);
	sema_init(&lock, 1);
	ret = misc_register(&Exynos4412_misc_dev);

	printk(DEVICE_NAME "\tinitialized\n");

	return ret;
}

static void __exit Exynos4412_pwm_dev_exit(void) {
	stop_ID_2();
	stop_ID_3();
	pwm_free(pwmMotor2);
	pwm_free(pwmMotor3);
	misc_deregister(&Exynos4412_misc_dev);
	gpio_free(MOTOR_PMW_GPIO_2);
	gpio_free(MOTOR_PMW_GPIO_3);
	gpio_free(EXYNOS4_GPX2(5));
	gpio_free(EXYNOS4_GPX2(7));
}

module_init(Exynos4412_pwm_dev_init);
module_exit(Exynos4412_pwm_dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stevenchen");
MODULE_DESCRIPTION("Exynos4412 PWM Driver");
