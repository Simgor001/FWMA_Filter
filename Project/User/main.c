/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include <CS1237/CS1237.h>
#include <FWMA_Filter/FWMA_Filter.h>
#include "debug.h"
/* Global typedef */

/* Global define */

/* Global Variable */

void tim2_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = SystemCoreClock / 1000000 - 1;
    TIM_TimeBaseInitStruct.TIM_Period = 1000;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    TIM_Cmd(TIM2, ENABLE);
}


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */

int main(void)
{
    double data = 0;
    double data_fl = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    tim2_init();
    Delay_Init();
    USART_Printf_Init(115200);

    CS1237_InitTypedef CS1237_InitStruct;
    CS1237_InitStruct.CS1237_Channel = CS1237_Channel_A;
    CS1237_InitStruct.CS1237_PGA = CS1237_PGA_1;
    CS1237_InitStruct.CS1237_REFO = CS1237_REFO_ENABLE;
    CS1237_InitStruct.CS1237_Speed = CS1237_Speed_10Hz;

    if(CS1237_Init(&CS1237_InitStruct) == 0)
    {
        printf("初始化超时！\r\n");
        while(1);
    }
    Delay_Ms(2);

    CS1237_Start();

    FWMA_InitTypeDef FWMA_InitStruct;
    FWMA_InitStruct.data_max = 2.5;
    FWMA_InitStruct.data_min = 0;
    FWMA_InitStruct.pres = 1;
    FWMA_InitStruct.af = 2;

    FWMA_t FWMA_s;
    double FWMA_fifo[FWMA_LEN] = {0};
    double FWMA_buf[3] = {0};

    FWMA_Init(&FWMA_s, FWMA_fifo, FWMA_buf, &FWMA_InitStruct);


    printf("正在校准浮空电压...\r\n");
    for(int i = 0; i < 30; ++i)
    {
        data = CS1237_GetData();
        data = FWMA_filter(&FWMA_s, data);
        Delay_Ms(20);
    }
    data_fl = data;

    printf("校准完成，data_fl=%.5lf\r\n开始测量：\r\n",data_fl);

    while(1)
    {

        data = CS1237_GetData() - data_fl;
        data = FWMA_filter(&FWMA_s, data);
        printf("$%.5lf;\r\n",data);

        Delay_Ms(50);
    }
}
