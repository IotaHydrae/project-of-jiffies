#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
 
/* eeprom所对应的I2C控制器的设备节点 */ 
#define EEPROM_DEVICE    	"/dev/i2c-0"	

/* eeprom的I2C设备地址 */
#define EEPROM_ADDR    0x50


int main()
{
	int fd,i,ret=0;
	unsigned char w_add=0x10;
	
	/* 将要读取的数据buf*/
	unsigned char rd_buf[13] = {0x10};  
	
	/* 要写的数据buf，第0个元素是要操作eeprom的寄存器地址*/
	unsigned char wr_buf[13] = {0};     

	printf("hello,this is read_write i2c test \n");
	
	/* 打开eeprom对应的I2C控制器文件 */
	fd =open(EEPROM_DEVICE, O_RDWR);
	if (fd< 0) 
	{
		printf("open"EEPROM_DEVICE"failed \n");
	}

	/*设置eeprom的I2C设备地址*/
	if (ioctl(fd,I2C_SLAVE_FORCE, EEPROM_ADDR) < 0) 
	{            
		printf("set slave address failed \n");
	}
	
	/* 将要操作的寄存器首地址赋给wr_buf[0] */
	wr_buf[0] = w_add;		

	/* 把要写入的数据写入到后面的buf中 */
	for(i=1;i<13;i++)
		wr_buf[i]=i;

	/* 通过write函数完成向eeprom写数据的功能 */
	write(fd, wr_buf, 13);

	/* 延迟一段时间 */
	sleep(1);
	
	/*重新开始下一个操作，先写寄存器的首地址*/
	write(fd, wr_buf, 1);

	/* 从wr_buf[0] = w_add的寄存器地址开始读取12个字节的数据 */
	ret=read(fd, rd_buf, 12);
	printf("ret is %d \r\n",ret);

	for(i=0;i<12;i++)
	{
		printf("rd_buf is :%d\n",rd_buf[i]);
	}
	
	/* 完成操作后，关闭eeprom对应的I2C控制器的设备文件 */
	close(fd);

	return 0;
}