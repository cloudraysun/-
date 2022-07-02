#include "main.h"

float Temperature;//�����¶�

int main()
{
	float up,down;
//	unsigned int i;
	delay_init();
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	TIM2_Int_Init(4999,7199);//����SR04
	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ50ms ��5000x7200/72=50ms
	USART1_Init(9600);
//	LED_RGB_Init();
//	key4x4_Init();
	MLX90614_Init();
	LCD_Init();//LCD��ʼ��
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	HC_SR04_init();
	up = 37.50;
	down = 10.00;
	USART_SendChar(USART1,'A');//����ʶ��ͨ�����л�������ʶ��	

	while(1)
	{	
		LCD_ShowChinese_16x16(5,2,"��",BLACK,WHITE);
		LCD_ShowChinese_16x16(20,2,"��",BLACK,WHITE);
//		LCD_ShowString(0,130,"distence:",BLACK,WHITE,16,1);
//		TFT_ShowNumber_Float_16x16(70,130,Distance,3,2,BLACK,WHITE,16,0);
		if(Temperature>25)//��ʱ���ԣ����ӽ�ʱ���������������
			HC_SR04_start();
		if(Distance<1.72 || Distance>100)//˵����1cm����
			Temperature=Read_Temp()+2;
		else if(Distance<=3.44)//˵����3~5cm
			Temperature=Read_Temp()+2.5;
		else if(Distance<=5.16)//˵����5~8cm
			Temperature=Read_Temp()+3.4;
		else if(Distance<=6.88)//˵����8~10cm
			Temperature=Read_Temp()+5;
		else if(Distance>6.88)//˵����10cm����
			Temperature=Read_Temp();
		if(Temperature>up)
		{			
			TFT_ShowNumber_Float_16x16(70,2,Temperature,2,1,RED,WHITE,16,0);
			//LED_shine(0); //�����
		}
		else
		{
			TFT_ShowNumber_Float_16x16(70,2,Temperature,2,1,GREEN,WHITE,16,0);
		}
		LCD_ShowChinese_16x16(110,2,"��",BLACK,WHITE);
		LCD_DrawRectangle(2,0,126,20,BLUE);
		
		LCD_ShowChinese_16x16(5,40,"��",BLACK,WHITE);
		LCD_ShowChinese_16x16(20,40,"��",BLACK,WHITE);
		LCD_ShowChinese_16x16(35,40,"ʶ",BLACK,WHITE);
		LCD_ShowChinese_16x16(50,40,"��",BLACK,WHITE);
		LCD_DrawRectangle(2,35,126,60,BLUE);
		if(test==1)
		{		
			LCD_ShowString(100,22,"TEST",RED,WHITE,12,1);
			LCD_ShowString(0,22,"FORMAL",WHITE,WHITE,12,1);
			face_pass=0;
			mask_pass=0;
		}
		else
		{
			LCD_ShowString(100,22,"TEST",WHITE,WHITE,12,1);
			LCD_ShowString(0,22,"FORMAL",GREEN,WHITE,12,1);
		}
		if(face_pass==1)//
		{
			LCD_ShowChinese_16x16(80,40,"��",GREEN,WHITE);
			LCD_ShowChinese_16x16(95,40,"ͨ",GREEN,WHITE);
			LCD_ShowChinese_16x16(110,40,"��",GREEN,WHITE);
			face_pass=1;
			USART_SendChar(USART1,'B');
		}
		else
		{
			LCD_ShowChinese_16x16(80,40,"δ",RED,WHITE);
			LCD_ShowChinese_16x16(95,40,"ͨ",RED,WHITE);
			LCD_ShowChinese_16x16(110,40,"��",RED,WHITE);			
		}
		
		LCD_ShowChinese_16x16(5,70,"��",BLACK,WHITE);
		LCD_ShowChinese_16x16(20,70,"��",BLACK,WHITE);
		LCD_ShowChinese_16x16(35,70,"ʶ",BLACK,WHITE);
		LCD_ShowChinese_16x16(50,70,"��",BLACK,WHITE);
		LCD_DrawRectangle(2,35,126,90,BLUE);	
		if(mask_pass==1)
		{
			LCD_ShowChinese_16x16(80,70,"��",GREEN,WHITE);
			LCD_ShowChinese_16x16(95,70,"ͨ",GREEN,WHITE);
			LCD_ShowChinese_16x16(110,70,"��",GREEN,WHITE);
			mask_pass=1;
			USART_SendChar(USART1,'A');		
		}
		else
		{
			LCD_ShowChinese_16x16(80,70,"δ",RED,WHITE);
			LCD_ShowChinese_16x16(95,70,"ͨ",RED,WHITE);
			LCD_ShowChinese_16x16(110,70,"��",RED,WHITE);			
		}
		
		LCD_ShowChinese_16x16(5,100,"ʶ",BLACK,WHITE);
		LCD_ShowChinese_16x16(20,100,"��",BLACK,WHITE);
		LCD_ShowChinese_16x16(35,100,"��",BLACK,WHITE);
		LCD_ShowChinese_16x16(50,100,"��",BLACK,WHITE);
		LCD_DrawRectangle(2,35,126,120,BLUE);	

		if(face_pass==1 && mask_pass==1 &&Temperature<up)
		{
			LCD_ShowChinese_16x16(80,100,"��",GREEN,WHITE);
			LCD_ShowChinese_16x16(95,100,"ͨ",GREEN,WHITE);
			LCD_ShowChinese_16x16(110,100,"��",GREEN,WHITE);			
			USART_SendChar(USART1,'A');//����ʶ��ͨ�����л�������ʶ������ʧЧ
			face_pass=0;
			mask_pass=0;
			delay_ms(200);
			delay_ms(2000);//�����۲�Ч��
			USART_SendChar(USART1,'A');
		}
		else
		{
			LCD_ShowChinese_16x16(80,100,"δ",RED,WHITE);
			LCD_ShowChinese_16x16(95,100,"ͨ",RED,WHITE);
			LCD_ShowChinese_16x16(110,100,"��",RED,WHITE);			
		}			
	}
	return 0;
}



