#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
 /****************************************************************
 * 宏定义，定义GPIO编号以及LED和按键各自对应的编号 ，计算方法为GPION_X对应的编号为 (N-1)*32+X 
 ****************************************************************/
#define GPIO4_14	110
#define GPIO5_1 	129
#define GPIO5_3		131 	

#define GPIO_KEY1     GPIO4_14
#define GPIO_KEY2     GPIO5_1
#define GPIO_LED	  GPIO5_3
//定义数组大小
#define MAX_BUF		  128
//定义用于GPIO控制的统一路径
#define SYSFS_GPIO_DIR "/sys/class/gpio"
/**********************************************************************
	 * 函数名称： sysfs_gpio_export
	 * 功能描述： 向/sys/class/gpio/export写入引脚编号，即通知系统需要导出的GPIO引脚编号
	 * 输入参数：@gpio :要导出的引脚编号	 			
	 * 输出参数：是否成功执行
	 * 返 回 值： 0 成功执行;其他：执行失败
	 * 修改日期 	   版本号	 修改人		  修改内容
	 * -----------------------------------------------
	 * 2020/05/14		 V1.0	  芯晓		  创建
 ***********************************************************************/
int sysfs_gpio_export(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];
	// /sys/class/gpio/export
    fd = open( "/sys/class/gpio/export", O_WRONLY);//打开文件
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }
 
    len = snprintf(buf, sizeof(buf), "%d", gpio);//从数字变换为字符串，即1 变为”1“
    write(fd, buf, len);//将需要导出的GPIO引脚编号进行写入
    close(fd);//关闭文件
 
    return 0;
}
/**********************************************************************
	 * 函数名称： sysfs_gpio_unexport
	 * 功能描述： 向/sys/class/gpio/unexport写入引脚编号，即通知系统需要取消导出的GPIO引脚编号
	 * 输入参数：@gpio :要取消导出的引脚编号	 			
	 * 输出参数：是否成功执行
	 * 返 回 值： 0 成功执行;其他：执行失败
	 * 修改日期 	   版本号	 修改人		  修改内容
	 * -----------------------------------------------
	 * 2020/05/14		 V1.0	  芯晓		  创建
 ***********************************************************************/
int sysfs_gpio_unexport(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];
	// /sys/class/gpio/unexport
    fd = open("/sys/class/gpio/unexport", O_WRONLY);//打开文件
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }
 
    len = snprintf(buf, sizeof(buf), "%d", gpio);//从数字变换为字符串，即1 变为”1“
    write(fd, buf, len);//将需要取消导出的GPIO引脚编号进行写入
    close(fd);//关闭文件
    return 0;
}
/**********************************************************************
	 * 函数名称： sysfs_gpio_set_dir
	 * 功能描述： 向/sys/class/gpio/gpioN/direction写入引脚方向，即设置导出的GPIO引脚的方向
	 * 输入参数：@gpio :要设置方向的引脚编号	 			
					@out_flag ：要设置的引脚方向。1:表示设置为输出。0:表示设置为输入。
	 * 输出参数：是否成功执行
	 * 返 回 值： 0 成功执行;其他：执行失败
	 * 修改日期 	   版本号	 修改人		  修改内容
	 * -----------------------------------------------
	 * 2020/05/14		 V1.0	  芯晓		  创建
 ***********************************************************************/
int sysfs_gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
    int fd, len;
    char buf[MAX_BUF];
	// /sys/class/gpio/gpioN/direction
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);
 
    fd = open(buf, O_WRONLY);//打开文件
    if (fd < 0) {
        perror(buf);
        return fd;
    }
 
    if (out_flag)//为1，则写入“out"，即设置为输出
        write(fd, "out", 4);
    else//为0，则写入“in"，即设置为输入
        write(fd, "in", 3);
 
    close(fd);//关闭文件
    return 0;
}
/**********************************************************************
	 * 函数名称： sysfs_gpio_set_value
	 * 功能描述： 向/sys/class/gpio/gpioN/value写入输出电平，即设置GPION输出的电平高低
					（在引脚为输出的时候进行使用）
	 * 输入参数：@gpio :要设置输出电平的引脚编号	 			
					@value ：要设置的输出电平。1:表示设置输出为高电平。0:表示设置输出为低电平。
	 * 输出参数：是否成功执行
	 * 返 回 值： 0 成功执行;其他：执行失败
	 * 修改日期 	   版本号	 修改人		  修改内容
	 * -----------------------------------------------
	 * 2020/05/14		 V1.0	  芯晓		  创建
 ***********************************************************************/
int sysfs_gpio_set_value(unsigned int gpio, unsigned int value)
{
    int fd, len;
    char buf[MAX_BUF];
	// /sys/class/gpio/gpioN/value
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
 
    fd = open(buf, O_WRONLY);//打开文件
    if (fd < 0) {
        perror(buf);
        return fd;
    }
 
    if (value)//为1，则写入“1"，即设置为输出高电平
        write(fd, "1", 2);
    else//为0，则写入“0"，即设置为输出低电平
        write(fd, "0", 2);
 
    close(fd);//关闭文件
    return 0;
}
/**********************************************************************
	 * 函数名称： sysfs_gpio_get_value
	 * 功能描述： 从/sys/class/gpio/gpioN/value读取引脚电平，即读取GPION输入的电平高低
					（在引脚为输入的时候进行使用）
	 * 输入参数：@gpio :要读取输入电平的引脚编号	 			
					@value ：读取的电平高低存储的位置
	 * 输出参数：是否成功执行
	 * 返 回 值： 0 成功执行;其他：执行失败
	 * 修改日期 	   版本号	 修改人		  修改内容
	 * -----------------------------------------------
	 * 2020/05/14		 V1.0	  芯晓		  创建
 ***********************************************************************/
int sysfs_gpio_get_value(unsigned int gpio, unsigned int *value)
{
    int fd, len;
    char buf[MAX_BUF];
    char ch;
	// /sys/class/gpio/gpioN/value
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
 
    fd = open(buf, O_RDONLY);//打开文件
    if (fd < 0) {
        perror("gpio/get-value");
        return fd;
    }
 
    read(fd, &ch, 1);//读取外部输入电平

    if (ch != '0') {//为'1'，则设置为1，即输入为高电平
        *value = 1;
    } else {//为'0'，则设置为0，即输入为低电平
        *value = 0;
    }
 
    close(fd);//关闭文件
    return 0;
}
/**********************************************************************
	 * 函数名称： main函数
	 * 功能描述： 导出相关引脚并进行功能实现

	 * 2020/05/14		 V1.0	  芯晓		  创建
 ***********************************************************************/
int main(int argc, char **argv) {
    unsigned int i;
    unsigned int value1,value2;
   
	printf("\t********************************************\n");
    printf("\t********  SYSFS_GPIO_TEST_DEMO**************\n");
    printf("\t******** version date: 2020/05    **********\n");
    printf("\t********************************************\n");    
		
	printf("led begin to init\r\n");
    sysfs_gpio_export(GPIO_LED);//export gpio led

    sysfs_gpio_set_dir(GPIO_LED, 1);//set as output
    printf("led init ok\r\n");


    /* Confirm INIT_B Pin as High */
	while(1)
	{
    
       
		sysfs_gpio_set_value(GPIO_LED, 1);//output high 
		printf("led off\r\n");
		usleep(500000);	//delay	
		sysfs_gpio_set_value(GPIO_LED, 0);//output low 
		printf("led on\r\n");
		usleep(500000);//delay
    }
	
    sysfs_gpio_unexport(GPIO_LED);//unexport gpio led

    return 0;
}
