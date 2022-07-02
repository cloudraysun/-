#include "SR04.h"
#include "stdio.h"	
#include "delay.h"
#include "usart.h"
#include "timer.h"



float Distance;
 
void HC_SR04_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(HC_SR04_TRIG_CLK, ENABLE);	 //ʹ��PC�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
 
	GPIO_InitStructure.GPIO_Pin = HC_SR04_TRIG_PIN;				 // ���崥���˿ڣ�Trig������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(HC_SR04_TRIG_PORT, &GPIO_InitStructure);					 //�����趨������ʼ���˿�
	//GPIO_ResetBits(HC_SR04_TRIG_PORT,HC_SR04_TRIG_PIN);						 //�˿ڳ�ʼ��Ϊ�͵�ƽ
 
	GPIO_InitStructure.GPIO_Pin = HC_SR04_ECHO_PIN;				 // �ز����ն˿ڣ�Echo������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(HC_SR04_ECHO_PORT, &GPIO_InitStructure);					 //�����趨������ʼ���˿�
 
 
	//���ն˿� �ж����Լ��жϳ�ʼ������
	GPIO_EXTILineConfig(HC_SR04_ECHO_Exit_PORT,HC_SR04_ECHO_Exit_PIN);
 
	EXTI_InitStructure.EXTI_Line=HC_SR04_ECHO_Exit_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
 
	NVIC_InitStructure.NVIC_IRQChannel = HC_SR04_ECHO_Exit_Channel;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
 
	//TIM3_Int_Init(49999,7199);  //��ʼ��TIM3��ʱ��������һ��Ϊ1/10000S��0.1ms����ÿ500ms����һ�ζ�ʱ�ж�
  //TIM_Cmd(TIM3,DISABLE);
}
	
//����20us�����崥���ź�
void HC_SR04_start(void)
{
	GPIO_SetBits(HC_SR04_TRIG_PORT,HC_SR04_TRIG_PIN);
	delay_us(20);
	GPIO_ResetBits(HC_SR04_TRIG_PORT,HC_SR04_TRIG_PIN);
	delay_ms(10);
}
 
 
 void EXTI9_5_IRQHandler(void)
{			
	delay_us(10);
	
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		//while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == RESET);
		TIM_SetCounter(HC_SR04_TIM,0);  //��������
		TIM_Cmd(HC_SR04_TIM,ENABLE);  //ʹ��TIM3��ʱ��
		
		while(GPIO_ReadInputDataBit(HC_SR04_ECHO_PORT,HC_SR04_ECHO_PIN));  //�ȴ���ƽ��Ϊ�͵�ƽ
		
		TIM_Cmd(HC_SR04_TIM,DISABLE); //�رն�ʱ��
		
		Distance = TIM_GetCounter(HC_SR04_TIM)*344/200.0;  //������룺cnt * 1/10000 * 340 / 2(��λ��m)
		
		//printf("Counter:%d\n",TIM_GetCounter(TIM3));
		//printf("Distance:%f cm\r\n",Distance);
			
		EXTI_ClearITPendingBit(HC_SR04_ECHO_Exit_Line);
	}  
}


