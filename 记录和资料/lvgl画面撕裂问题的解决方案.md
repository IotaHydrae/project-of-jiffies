LVGL画面撕裂的解决方案
----

在注册lvgl缓冲区时可以提供两个buffer，在以前的实验中
我只定义了一个buffer，大小为屏幕分辨率的1/10即61440，
设置这样大小的buffer所造成的后果就是画面撕裂严重，特
别是进行整体画面切换的时候，具体现象就是两帧画面的两
半同时显示到了显示屏的上半部分和下半部分。

解决过程：
①尝试增大buffer大小，经实验后，与画面撕裂程度呈负相关
```c

```
②开启framebuffer双缓冲区机制，
```c
```