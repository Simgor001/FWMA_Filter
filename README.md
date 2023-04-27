# FWMA_Filter
分式加权移动平均滤波算法（Fractional Weighted Moving Average Filter）

注意：作者是初学者，代码和思路仅供参考！
# 介绍
这是一种简易的滤波算法，使用FIFO进行加权求平均值。

- 代码面向对象，支持计算多个滤波器
- 请注意，这种滤波算法有滞后性，实时性很差，仅适合温度检测等领域使用

## 模拟
该算法预先使用Python实现，然后移植到C语言上，下面是模拟的效果
### 方波
![模拟（方波）](https://blog.simgor.cn/img/github/%E6%A8%A1%E6%8B%9F%EF%BC%88%E6%96%B9%E6%B3%A2%EF%BC%89.jpg)
### 线性
![模拟（线性）](https://blog.simgor.cn/img/github/%E6%A8%A1%E6%8B%9F%EF%BC%88%E7%BA%BF%E6%80%A7%EF%BC%89.jpg)

# 使用说明
下面的例子主控为CH32V203，其他主控请参考修改。
## 使用流程
- 设置队列长度```FWMA_LEN```
- 调用```FWMA_Init```函数，设置存储地址和参数
- 如果需要使用完整的滤波器，请调用```FWMA_filter```函数
- 如果仅使用中值滤波，请调用```FWMA_median```函数

## 参数说明
有以下几种参数：
- FWMA_LEN：队列长度（默认为5）
- pres：上次结果权
- af：衰减系数
- data_min：数据范围最小值
- data_max：数据范围最大值

### 参数：pres
上次结果权，控制前一次计算结果对当前计算结果的影响程度，值越大，前一次计算结果对当前结果的影响越大，反应越慢，数据越平滑。

### 参数：af
衰减系数，数值越小，输出结果更敏感于新数据的变化，但对噪声的去除效果可能会降低。较小的衰减系数适用于需要更快响应的实时应用。

## 初始化用例
``` c
FWMA_InitTypeDef FWMA_InitStruct;
FWMA_InitStruct.data_max = 2.5;
FWMA_InitStruct.data_min = 0;
FWMA_InitStruct.pres = 1;
FWMA_InitStruct.af = 2;
FWMA_t FWMA_s;
double FWMA_fifo[FWMA_LEN] = {0};
double FWMA_buf[3] = {0};

FWMA_Init(&FWMA_s, FWMA_fifo, FWMA_buf, &FWMA_InitStruct);
```
# 实际效果
## 直流
![直流](https://blog.simgor.cn/img/github/%E5%AE%9E%E9%99%85%E9%87%87%E6%A0%B7%EF%BC%88%E7%9B%B4%E6%B5%81%EF%BC%89.jpg)
## 锯齿波
![锯齿波](https://blog.simgor.cn/img/github/%E5%AE%9E%E9%99%85%E9%87%87%E6%A0%B7%EF%BC%88%E9%94%AF%E9%BD%BF%E6%B3%A2%EF%BC%89.jpg)

# 开发使用的工具
[MounRiver](http://www.mounriver.com/)

[安信可串口调试助手](https://docs.ai-thinker.com/%E5%BC%80%E5%8F%91%E5%B7%A5%E5%85%B72)

[serial_port_plotter](https://github.com/CieNTi/serial_port_plotter)
