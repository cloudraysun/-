#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ����� 
}
void LED_RGB_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_ResetBits(GPIOB,GPIO_Pin_3); 						 //PE.5 ����� .
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_ResetBits(GPIOB,GPIO_Pin_5); 						 //PE.5 ����� .
}
//led��
//0���죬1���̣�2����
//mode
//0��������������

void LED_RGB(unsigned int led,unsigned int mode)
{
	if(mode==0)
	{
		
		switch(led)
		{
			case 0:{red=1;green=0;blue=0;}break;
			case 1:{red=0;green=1;blue=0;}break;
			case 2:{red=0;green=0;blue=1;}break;	
		}
		
	}
	else
	{
		red=0;
		green=0;
		blue=0;
	}
}
//LED��˸���ߵ�ƽ����
//0����
//1����
//2����
void LED_shine(unsigned int color)
{
	switch(color)
	{
		case 0:{LED_RGB(0,1);delay_ms(100);	LED_RGB(0,0);delay_ms(100);	}break;
		case 1:{LED_RGB(1,1);delay_ms(100);	LED_RGB(1,0);delay_ms(100);	}break;
		case 2:{LED_RGB(2,1);delay_ms(100);	LED_RGB(2,0);delay_ms(100);	}break;
	}
}
void led_test()
{
	LED_RGB(0,1);delay_ms(10000);	LED_RGB(0,0);delay_ms(10000);
	LED_RGB(0,1);delay_ms(10000);	LED_RGB(0,0);delay_ms(10000);
}


//void led_shing(void)
//{
//	LED0=0;
//	delay_ms(100);
//	LED0=1;
//	delay_ms(100);
//}
