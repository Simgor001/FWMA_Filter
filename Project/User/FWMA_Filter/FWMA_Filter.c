/*
 * FWMA_Filter.c
 *
 *  Created on: 2023年4月25日
 *      Author: Simgor001
 */

#include "FWMA_Filter.h"


/**
 * @brief 初始化FWMA
 * @param FWMA_p 指向 FWMA_t 结构体的指针，相当于句柄
 * @param List 指向 FIFO 列表的指针，滤波过程的数据保存到这里
 * @param buf 指向缓冲区的指针
 * @param FWMA_InitStruct 指向 FWMA_InitTypeDef 的指针，初始化参数
 * @return 无返回值
 */
void FWMA_Init(FWMA_t* FWMA_p, double* List, double* buf, FWMA_InitTypeDef* FWMA_InitStruct)
{

    FWMA_p->param.data_max = FWMA_InitStruct->data_max;
    FWMA_p->param.data_min = FWMA_InitStruct->data_min;
    FWMA_p->param.pres = FWMA_InitStruct->pres;
    FWMA_p->param.af = FWMA_InitStruct->af;

    FWMA_p->FIFO = List;
    FWMA_p->buf = buf;
    FWMA_p->len = 0;
    FWMA_p->avg_pre = 0;
    FWMA_p->pos = 0;
    FWMA_p->cnt = 0;
}

//压入数据
static void _FWMA_push(FWMA_t* FWMA_p, double dat)
{
    if(FWMA_p->len < FWMA_LEN)
        FWMA_p->len += 1;

    if(FWMA_p->pos < FWMA_p->len)
    {
        FWMA_p->FIFO[FWMA_p->pos] = dat;
        FWMA_p->pos += 1;
    }
    else
    {
        FWMA_p->FIFO[0] = dat;
        FWMA_p->pos = 1;
    }
}

//计算平均值
static double _FWMA_avg(FWMA_t* FWMA_p)
{
    int i = 0;
    double af = FWMA_p->param.af;
    double w = 0;
    double sum_l = 0;
    double sum_w = 0;
    double avg = 0;
    for(i = 0; i < FWMA_p->len; ++i)
    {
        w = af / (af + i);

        int cn = (i + (FWMA_p->pos) - 1) % (FWMA_p->len);
        sum_l += w * (FWMA_p->FIFO[cn]);
        sum_w += w;
    }
    avg = (sum_l + (FWMA_p->avg_pre * FWMA_p->param.pres)) / (sum_w + FWMA_p->param.pres);
    FWMA_p->avg_pre = avg;
    return avg;
}


static double _FWMA_median(double* buf)
{
    double _dat = 0;
    uint32_t mask = 0;
    mask |= (buf[0] > buf[1]) << 0;
    mask |= (buf[1] > buf[2]) << 1;
    mask |= (buf[0] > buf[2]) << 2;

    switch(mask)
    {
    case 1:
    case 6:
        _dat = buf[0];
        break;
    case 0:
    case 7:
        _dat = buf[1];
        break;
    case 2:
    case 5:
        _dat = buf[2];
        break;
    }
    return _dat;
}


/**
 * @brief 中位数滤波
 * @param FWMA_p 滤波器句柄
 * @param dat 新添加的数据
 * @return 滤波后的值
 */
double FWMA_median(FWMA_t* FWMA_p, double dat)
{
    double _dat = 0;

    if(dat < FWMA_p->param.data_min)
        return FWMA_p->param.data_min;
    if(dat > FWMA_p->param.data_max)
        return FWMA_p->param.data_max;

    FWMA_p->buf[FWMA_p->cnt] = dat;
    if(FWMA_p->cnt < 2)
    {
        FWMA_p->cnt += 1;
        return FWMA_p->avg_pre;
    }

    FWMA_p->cnt = 0;

    _dat = _FWMA_median(FWMA_p->buf);
    _dat = (_dat + (FWMA_p->avg_pre * FWMA_p->param.pres)) / (1 + FWMA_p->param.pres);

    FWMA_p->avg_pre = _dat;

    return _dat;
}


/**
 * @brief FWMA滤波
 * @param FWMA_p 滤波器句柄
 * @param dat 新添加的数据
 * @return 滤波后的值
 */
double FWMA_filter(FWMA_t* FWMA_p, double dat)
{
    double _dat = 0;
    double avg = 0;

    if(dat < FWMA_p->param.data_min)
        return FWMA_p->param.data_min;
    if(dat > FWMA_p->param.data_max)
        return FWMA_p->param.data_max;

    FWMA_p->buf[FWMA_p->cnt] = dat;
    if(FWMA_p->cnt < 2)
    {
        FWMA_p->cnt += 1;
        return FWMA_p->avg_pre;
    }

    FWMA_p->cnt = 0;

    _dat = _FWMA_median(FWMA_p->buf);

    _FWMA_push(FWMA_p, _dat);
    avg = _FWMA_avg(FWMA_p);

    return avg;
}
