#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

/* eeprom所对应的I2C控制器的设备节点 */ 
#define EEPROM_DEVICE    	"/dev/i2c-0"	

/* eeprom的I2C设备地址 */
#define EEPROM_ADDR    0x50				

/*函数名：eeprom_write
**功能：向eeprom写数据
**参数：fd：eeprom对应I2C控制器设备节点的文件名
**		dev_addr：eeprom的I2C从设备地址
**		reg_addr：eeprom的寄存器地址
**		data_buf：要向eeprom写数据的数据buf
**		len：要写多少个字节。本例中当前最大支持为8个字节
**返回值：负数表示操作失败，其他为成功
*/
int eeprom_write(int fd, unsigned char dev_addr, unsigned char reg_addr, unsigned char * data_buf,int len)
{
	int ret;

	unsigned char msg_buf[9];
	struct i2c_rdwr_ioctl_data data;

	struct i2c_msg messages;


	/* 1. 构建msg_buf*/
	/* 1.1. 将要操作的寄存器首地址赋给要进行I2C数据通信的首字节数据 */
	msg_buf[0] = reg_addr;
	
	/* 1.2. 将要向eeprom写数据的数据buf赋在I2C数据通信中eeprom寄存器的后面 */
	if (len < 9) {			/* 本demo最大支持一次向eeprom写一页大小的8个字节数据 */
        memcpy((void *) &msg_buf[1], data_buf, len);  //第1位之后是数据
    } else {
        printf("This function supports up to 8 bytes at a time !!!\n");
        return -1;
    }

	/* 2. 构建 struct i2c_msg messages */
	/* 2.1. 赋值eeprom的I2C从设备地址 */
	messages.addr = dev_addr;  

	/* 2.2. 赋值flags为本次I2C通信完成写功能 */
	messages.flags = 0;    

	/* 2.3. 赋值len为数据buf的长度 + eeprom寄存器地址的数据长度 */
	messages.len = len+1;

	/* 2.4. 构建消息包的数据buf*/
	messages.buf = msg_buf;  

	/* 3. 构建struct i2c_rdwr_ioctl_data data */
	/* 3.1. 将准备好的消息包赋值给i2c_rdwr_ioctl_data中的msgs消息*/
	data.msgs = &messages;

	/* 3.2. 由于本次I2C通信只有写动作，所以消息数为1次 */
	data.nmsgs = 1;

	/* 4. 调用驱动层的读写组合的I2C数据传输 */
	if(ioctl(fd, I2C_RDWR, &data) < 0)
	{
		printf("I2C_RDWR err \n");
		return -1;
	}

	/* 5. 等待I2C总线写入完成 */
	sleep(1);

	return 0;
}

/*函数名：eeprom_read
**功能：从eeprom读数据
**参数：fd：eeprom对应I2C控制器设备节点的文件名
**		dev_addr：eeprom的I2C从设备地址
**		reg_addr：eeprom的寄存器地址
**		data_buf：存放从eeprom读数据的buf
**		len：要读多少个字节。
**返回值：负数表示操作失败，其他为成功
*/
int eeprom_read(int fd, unsigned char dev_addr, unsigned char reg_addr, unsigned char * data_buf,int len)
{
	int ret;

	unsigned char msg_buf[9];
	struct i2c_rdwr_ioctl_data data;

	struct i2c_msg messages[2];

	/* 1. 构建 struct i2c_msg messages */
	/* 1.1. 构建第一条消息 messages[0] */
	/* 1.1.1. 赋值eeprom的I2C从设备地址 */
	messages[0].addr = dev_addr;  

	/* 1.1.2. 赋值flags为本次I2C通信完成写动作 */
	messages[0].flags = 0;    

	/* 1.1.3. 赋值len为eeprom寄存器地址的数据长度是1 */
	messages[0].len = 1;

	/* 1.1.4. 本次写动作的数据是要读取eeprom的寄存器首地址*/
	messages[0].buf = &reg_addr;  
	
	/* 1.2. 构建第二条消息 messages[1] */
	/* 1.2.1. 赋值eeprom的I2C从设备地址 */
	messages[1].addr = dev_addr;  

	/* 1.1.2. 赋值flags为本次I2C通信完成读动作 */
	messages[1].flags = I2C_M_RD;    

	/* 1.1.3. 赋值len为要读取eeprom寄存器数据长度len */
	messages[1].len = len;

	/* 1.1.4. 本次读动作的数据要存放的buf位置*/
	messages[1].buf = data_buf; 

	/* 2. 构建struct i2c_rdwr_ioctl_data data */
	/* 2.1. 将准备好的消息包赋值给i2c_rdwr_ioctl_data中的msgs消息*/
	data.msgs = messages;

	/* 2.2. 由于本次I2C通信既有写动作也有读动作，所以消息数为2次 */
	data.nmsgs = 2;

	/* 3. 调用驱动层的读写组合的I2C数据传输 */
	if(ioctl(fd, I2C_RDWR, &data) < 0)
	{
		printf("I2C_RDWR err \n");
		return -1;
	}

	/* 4. 等待I2C总线读取完成 */
	sleep(1);

	return 0;
}
 
int main()
{
	int fd,i,ret=0;
	unsigned char w_add=0x10;
	
	/* 将要读取的数据buf*/
	unsigned char rd_buf[8] = {0};  
	
	/* 要写的数据buf*/
	unsigned char wr_buf[8] = {0};  

	printf("hello,this is I2C_RDWR i2c test \n");
	
	/* 打开eeprom对应的I2C控制器文件 */
	fd =open(EEPROM_DEVICE, O_RDWR);
	if (fd< 0) 
	{
		printf("open"EEPROM_DEVICE"failed \n");
	}	

	/* 把要写入的数据写入到后面的buf中 */
	for(i=0;i<8;i++)
		wr_buf[i]=i;

	/* 通过I2C_RDWR完成向eeprom读数据的功能 */
	eeprom_write(fd,EEPROM_ADDR,w_add,wr_buf,8);

	
	/* 通过I2C_RDWR完成向eeprom写数据的功能 */
	eeprom_read(fd,EEPROM_ADDR,w_add,rd_buf,8);

	for(i=0;i<8;i++)
	{
		printf("rd_buf is :%d\n",rd_buf[i]);
	}
	
	/* 完成操作后，关闭eeprom对应的I2C控制器的设备文件 */
	close(fd);

	return 0;
}

 