/*-----------------------------------------------------------------------------------
*   FWMA_Filter.h
*   全称：Fractional Weighted Moving Average Filter
*   名称：分式加权移动平均滤波器
*   2023-4-25   Ver1.0.0
*   By Simgor001 https://blog.simgor.cn
*   Copyright (C) 2023 Simgor001
*------------------------------------------------------------------------------------
*   # 介绍
*   - 这是一个简单的滤波库，通过队列取均值滤波
*   - 代码面向对象，支持计算多个滤波器
*   - 请注意，这种滤波方式有滞后性，实时性很差，适合温度检测等领域使用
*
*   # 使用流程
*   - 设置队列长度FWMA_LEN
*   - 调用FWMA_Init函数，设置存储地址和参数
*   - 如果需要使用完整的滤波器，请调用FWMA_filter函数
*   - 如果仅使用中值滤波，请调用FWMA_median函数
*
*   # 参数说明
*   有以下几种参数：
*   - FWMA_LEN：队列长度（默认为5）
*   - pres：上次结果权
*   - af：衰减系数
*   - data_min：数据范围最小值
*   - data_max：数据范围最大值
*
*   ## pres
*   上次结果权，控制前一次计算结果对当前计算结果的影响程度，
*   值越大，前一次计算结果对当前结果的影响越大，反应越慢，数据越平滑
*
*   ## af
*   衰减系数，数值越小，输出结果更敏感于新数据的变化，
*   但对噪声的去除效果可能会降低。
*   较小的衰减系数适用于需要更快响应的实时应用。
*
*   # 初始化用例
*   ``` c
*   FWMA_InitTypeDef FWMA_InitStruct;
*   FWMA_InitStruct.data_max = 2.5;
*   FWMA_InitStruct.data_min = 0;
*   FWMA_InitStruct.pres = 1;
*   FWMA_InitStruct.af = 2;
*   FWMA_t FWMA_s;
*   double FWMA_fifo[FWMA_LEN] = {0};
*   double FWMA_buf[3] = {0};
*
*   FWMA_Init(&FWMA_s, FWMA_fifo, FWMA_buf, &FWMA_InitStruct);
*   ```
*------------------------------------------------------------------------------------
*   The MIT License (MIT)
*
*   Copyright (C) 2023 Simgor001
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*-----------------------------------------------------------------------------------*/
/*==================================================================
 *     引入芯片头文件
 * ===============================================================*/
//CH32系列
#include "debug.h"
//STM32F103系列
//#include "stm32f10x.h"

/*==================================================================
 *     驱动库头文件
 * ===============================================================*/
#ifndef _FWMA_FILTER_H_
#define _FWMA_FILTER_H_
#include "stdint.h"

/* 队列长度 */
#define FWMA_LEN ((uint32_t)(5))

typedef struct
{
    double pres;            /* 上次结果权 */
    double af;              /* 衰减系数 */
    double data_min;        /* 输入范围最小值 */
    double data_max;        /* 输入范围最大值 */
}FWMA_InitTypeDef;

typedef struct
{
    double* FIFO;           /* 列表 */
    uint32_t pos;           /* 当前位置 */
    uint32_t len;           /* 长度 */
    double avg_pre;         /* 上一次的转换结果 */
    uint32_t cnt;           /* 缓冲计数 */
    double* buf;            /* 缓冲区 */
    FWMA_InitTypeDef param;
} FWMA_t;

/**
 * @brief 初始化FWMA
 * @param FWMA_p 指向 FWMA_t 结构体的指针，相当于句柄
 * @param List 指向 FIFO 列表的指针，滤波过程的数据保存到这里
 * @param buf 指向缓冲区的指针
 * @param FWMA_InitStruct 指向 FWMA_InitTypeDef 的指针，初始化参数
 * @return 无返回值
 */
void FWMA_Init(FWMA_t* FWMA_p, double* List, double* buf, FWMA_InitTypeDef* FWMA_InitStruct);

/**
 * @brief 中位数滤波
 * @param FWMA_p 滤波器句柄
 * @param dat 新添加的数据
 * @return 滤波后的值
 */
double FWMA_median(FWMA_t* FWMA_p, double dat);

/**
 * @brief FWMA滤波
 * @param FWMA_p 滤波器句柄
 * @param dat 新添加的数据
 * @return 滤波后的值
 */
double FWMA_filter(FWMA_t* FWMA_p, double dat);

#endif /* _FWMA_FILTER_H_ */
