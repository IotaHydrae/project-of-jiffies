01 #include <stdio.h>
02 #include <stdlib.h>
03 #include <unistd.h>
04 #include <string.h>
05 #include <errno.h>
06 #include <fcntl.h>
07 #include <sys/stat.h>
08  /****************************************************************
09  * 宏定义，定义GPIO编号以及LED和按键各自对应的编号 ，计算方法为GPION_X对应的编号为 (N-1)*32+X 
10  ****************************************************************/
11 #define GPIO4_14	110
12 #define GPIO5_1 	129
13 #define GPIO5_3		131 	
14 
15 #define GPIO_KEY1     GPIO4_14
16 #define GPIO_KEY2     GPIO5_1
17 #define GPIO_LED	  GPIO5_3
18 //定义数组大小
19 #define MAX_BUF		  128
20 //定义用于GPIO控制的统一路径
21 #define SYSFS_GPIO_DIR "/sys/class/gpio"
22 /**********************************************************************
23 	 * 函数名称： sysfs_gpio_export
24 	 * 功能描述： 向/sys/class/gpio/export写入引脚编号，即通知系统需要导出的GPIO引脚编号
25 	 * 输入参数：@gpio :要导出的引脚编号	 			
26 	 * 输出参数：是否成功执行
27 	 * 返 回 值： 0 成功执行;其他：执行失败
28 	 * 修改日期 	   版本号	 修改人		  修改内容
29 	 * -----------------------------------------------
30 	 * 2020/05/14		 V1.0	  芯晓		  创建
31  ***********************************************************************/
32 int sysfs_gpio_export(unsigned int gpio)
33 {
34     int fd, len;
35     char buf[MAX_BUF];
36 	// /sys/class/gpio/export
37     fd = open( "/sys/class/gpio/export", O_WRONLY);//打开文件
38     if (fd < 0) {
39         perror("gpio/export");
40         return fd;
41     }
42  
43     len = snprintf(buf, sizeof(buf), "%d", gpio);//从数字变换为字符串，即1 变为”1“
44     write(fd, buf, len);//将需要导出的GPIO引脚编号进行写入
45     close(fd);//关闭文件
46  
47     return 0;
48 }
49 /**********************************************************************
50 	 * 函数名称： sysfs_gpio_unexport
51 	 * 功能描述： 向/sys/class/gpio/unexport写入引脚编号，即通知系统需要取消导出的GPIO引脚编号
52 	 * 输入参数：@gpio :要取消导出的引脚编号	 			
53 	 * 输出参数：是否成功执行
54 	 * 返 回 值： 0 成功执行;其他：执行失败
55 	 * 修改日期 	   版本号	 修改人		  修改内容
56 	 * -----------------------------------------------
57 	 * 2020/05/14		 V1.0	  芯晓		  创建
58  ***********************************************************************/
59 int sysfs_gpio_unexport(unsigned int gpio)
60 {
61     int fd, len;
62     char buf[MAX_BUF];
63 	// /sys/class/gpio/unexport
64     fd = open("/sys/class/gpio/unexport", O_WRONLY);//打开文件
65     if (fd < 0) {
66         perror("gpio/export");
67         return fd;
68     }
69  
70     len = snprintf(buf, sizeof(buf), "%d", gpio);//从数字变换为字符串，即1 变为”1“
71     write(fd, buf, len);//将需要取消导出的GPIO引脚编号进行写入
72     close(fd);//关闭文件
73     return 0;
74 }
75 /**********************************************************************
76 	 * 函数名称： sysfs_gpio_set_dir
77 	 * 功能描述： 向/sys/class/gpio/gpioN/direction写入引脚方向，即设置导出的GPIO引脚的方向
78 	 * 输入参数：@gpio :要设置方向的引脚编号	 			
79 					@out_flag ：要设置的引脚方向。1:表示设置为输出。0:表示设置为输入。
80 	 * 输出参数：是否成功执行
81 	 * 返 回 值： 0 成功执行;其他：执行失败
82 	 * 修改日期 	   版本号	 修改人		  修改内容
83 	 * -----------------------------------------------
84 	 * 2020/05/14		 V1.0	  芯晓		  创建
85  ***********************************************************************/
86 int sysfs_gpio_set_dir(unsigned int gpio, unsigned int out_flag)
87 {
88     int fd, len;
89     char buf[MAX_BUF];
90 	// /sys/class/gpio/gpioN/direction
91     len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);
92  
93     fd = open(buf, O_WRONLY);//打开文件
94     if (fd < 0) {
95         perror(buf);
96         return fd;
97     }
98  
99     if (out_flag)//为1，则写入“out"，即设置为输出
100         write(fd, "out", 4);
101     else//为0，则写入“in"，即设置为输入
102         write(fd, "in", 3);
103  
104     close(fd);//关闭文件
105     return 0;
106 }
107 /**********************************************************************
108 	 * 函数名称： sysfs_gpio_set_value
109 	 * 功能描述： 向/sys/class/gpio/gpioN/value写入输出电平，即设置GPION输出的电平高低
110 					（在引脚为输出的时候进行使用）
111 	 * 输入参数：@gpio :要设置输出电平的引脚编号	 			
112 					@value ：要设置的输出电平。1:表示设置输出为高电平。0:表示设置输出为低电平。
113 	 * 输出参数：是否成功执行
114 	 * 返 回 值： 0 成功执行;其他：执行失败
115 	 * 修改日期 	   版本号	 修改人		  修改内容
116 	 * -----------------------------------------------
117 	 * 2020/05/14		 V1.0	  芯晓		  创建
118  ***********************************************************************/
119 int sysfs_gpio_set_value(unsigned int gpio, unsigned int value)
120 {
121     int fd, len;
122     char buf[MAX_BUF];
123 	// /sys/class/gpio/gpioN/value
124     len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
125  
126     fd = open(buf, O_WRONLY);//打开文件
127     if (fd < 0) {
128         perror(buf);
129         return fd;
130     }
131  
132     if (value)//为1，则写入“1"，即设置为输出高电平
133         write(fd, "1", 2);
134     else//为0，则写入“0"，即设置为输出低电平
135         write(fd, "0", 2);
136  
137     close(fd);//关闭文件
138     return 0;
139 }
140 /**********************************************************************
141 	 * 函数名称： sysfs_gpio_get_value
142 	 * 功能描述： 从/sys/class/gpio/gpioN/value读取引脚电平，即读取GPION输入的电平高低
143 					（在引脚为输入的时候进行使用）
144 	 * 输入参数：@gpio :要读取输入电平的引脚编号	 			
145 					@value ：读取的电平高低存储的位置
146 	 * 输出参数：是否成功执行
147 	 * 返 回 值： 0 成功执行;其他：执行失败
148 	 * 修改日期 	   版本号	 修改人		  修改内容
149 	 * -----------------------------------------------
150 	 * 2020/05/14		 V1.0	  芯晓		  创建
151  ***********************************************************************/
152 int sysfs_gpio_get_value(unsigned int gpio, unsigned int *value)
153 {
154     int fd, len;
155     char buf[MAX_BUF];
156     char ch;
157 	// /sys/class/gpio/gpioN/value
158     len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
159  
160     fd = open(buf, O_RDONLY);//打开文件
161     if (fd < 0) {
162         perror("gpio/get-value");
163         return fd;
164     }
165  
166     read(fd, &ch, 1);//读取外部输入电平
167 
168     if (ch != '0') {//为'1'，则设置为1，即输入为高电平
169         *value = 1;
170     } else {//为'0'，则设置为0，即输入为低电平
171         *value = 0;
172     }
173  
174     close(fd);//关闭文件
175     return 0;
176 }
177 /**********************************************************************
178 	 * 函数名称： main函数
179 	 * 功能描述： 导出相关引脚并进行功能实现
180 
181 	 * 2020/05/14		 V1.0	  芯晓		  创建
182  ***********************************************************************/
183 int main(int argc, char **argv) {
184     unsigned int i;
185     unsigned int value1,value2;
186    
187 	printf("\t********************************************\n");
188     printf("\t********  SYSFS_GPIO_TEST_DEMO**************\n");
189     printf("\t******** version date: 2020/05    **********\n");
190     printf("\t********************************************\n");    
191 		
192 	printf("key begin to init\r\n");
193     sysfs_gpio_export(GPIO_KEY1);//export gpio key1
194     sysfs_gpio_export(GPIO_KEY2);//export gpio key2
195     
196     sysfs_gpio_set_dir(GPIO_KEY1, 0);//set as input
197     sysfs_gpio_set_dir(GPIO_KEY2, 0);//set as input
198    
199     printf("key init ok\r\n");
200 
201 
202     /* Confirm INIT_B Pin as High */
203 	while(1)
204 	{
205     
206         sysfs_gpio_get_value(GPIO_KEY1, &value1);	//read key1 value	
207 		//printf("@@key1 value 1is %d \n\r",value1);
208 		if(value1==0)//key1 pressed
209 		{
210 			printf("@@key1 is pressed 0\n\r");			
211 		}
212 		sysfs_gpio_get_value(GPIO_KEY2, &value2);//read key2 value	
213 		//printf("##key2 value 1is %d \n\r",value2);
214 		if(value2==0)//key2 pressed
215 		{
216 			printf("##key2 is pressed 0\n\r");			
217 		}
218 		usleep(100000);//delay
219 				
220     }
221 	
222 	sysfs_gpio_unexport(GPIO_KEY1);//unexport gpio key1
223     sysfs_gpio_unexport(GPIO_KEY2);//unexport gpio key2
224    
225 
226     return 0;
227 }
