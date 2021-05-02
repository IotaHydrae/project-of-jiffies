![image-20210429115426123](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429115426123.png)

从uboot提供的启动信息可以得知

uboot以SPI方式启动，卡在SF: unrecognized JEDEC id bytes: 0b, 40, 18
表示未识别的JEDEC id

初步分析问题原因是我这块板子上的spi flash的信息并没有添加到uboot中，导致uboot无法识别

使用grep工具查找打印出这段信息的函数

` grep unrecognized * -nR`

![image-20210429120649469](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429120649469.png)

可以看到 drivers/mtd/spi/spi_flash.c文件中第919行打印出了该信息

`vim drivers/mtd/spi/spi_flash.c +919`打开文件查看![image-20210429121028515](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429121028515.png)

发现这三个数值是从数组id中获取的，查看id定义

`u8		id[SPI_FLASH_MAX_ID_LEN]`

![image-20210429121342440](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429121342440.png)

这里将info中的id逐一与上方spi_flash_cmd获取到的id进行比较
本例中结果是比对失败，将id中的信息打印

查看info定义以及spi_flash_ids
`const struct spi_flash_info *info`

使用grep查找spi_flash_ids定义`grep spi_flash_ids * -nR`
![image-20210429121944382](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429121944382.png)

推测`drivers/mtd/spi/spi_flash_ids.c`中存放spi flash JEDEC信息

`vim drivers/mtd/spi/spi_flash_ids.c +46`打开查看

![image-20210429122123692](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429122123692.png)

可以看到这里有大量的预先定义好的各种品牌flash信息
我们来到文件底部，仿照上方代码，定义一个spi flash信息
我手上这块板子上搭载的是16MB大小的芯天下XT25F128B

![image-20210429122929874](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429122929874.png)

在文件头部，宏INFO定义

![image-20210429123048941](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429123048941.png)

第一个参数是JEDEC id

修改完成之后我们要查找这些宏在哪里被控制开启关闭
`grep CONFIG_SPI_FLASH_ATMEL * -nR`

结果显示在configs目录下，找到板子对应的config文件

![image-20210429124154852](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429124154852.png)

添加`CONFIG_SPI_FLASH_XTX=y`

修改设备树`arch/arm/dts/suniv-f1c100s-licheepi-nano.dts`

![image-20210429141639040](C:\Users\huazheng\Desktop\毕设\记录和资料\uboot调试记录.assets\image-20210429141639040.png)

编译测试