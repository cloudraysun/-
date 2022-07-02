#ifndef __SR04_H
#define __SR04_H

#include "sys.h"


#define HC_SR04_TRIG_CLK   		RCC_APB2Periph_GPIOB
#define HC_SR04_TRIG_PORT  		GPIOB
#define HC_SR04_TRIG_PIN   		GPIO_Pin_7

 
#define HC_SR04_ECHO_CLK   		RCC_APB2Periph_GPIOB
#define HC_SR04_ECHO_PORT  		GPIOB
#define HC_SR04_ECHO_PIN   		GPIO_Pin_6

#define HC_SR04_ECHO_Exit_PORT 	GPIO_PortSourceGPIOB//�ж�Դ����
#define HC_SR04_ECHO_Exit_PIN	GPIO_PinSource6

#define HC_SR04_ECHO_Exit_Line	EXTI_Line6//�ж��ߺ�ͨ������
#define HC_SR04_ECHO_Exit_Channel  EXTI9_5_IRQn

#define HC_SR04_TIM TIM2          //���ڼ�ʱ�������Ķ�ʱ��
 
extern float Distance;
 
 
void HC_SR04_init(void);
void HC_SR04_start(void);


#endif

