#include "lcd.h"
#include "lcd_init.h"
#include "lcdfont.h"
#include "delay.h"


/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//������ʾ��Χ
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	} 					  	    
}

/******************************************************************************
      ����˵������ָ��λ�û���
      ������ݣ�x,y ��������
                color �����ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(color);
} 


/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   �ߵ���ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      ����˵����������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   ���ε���ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
                color   Բ����ɫ
      ����ֵ��  ��
******************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}


/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	u16 i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //����6x12����
		else if(sizey==16)temp=ascii_1608[num][i];		 //����8x16����
		else if(sizey==24)temp=ascii_2412[num][i];		 //����12x24����
		else if(sizey==32)temp=ascii_3216[num][i];		 //����16x32����
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//�ǵ���ģʽ
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//����ģʽ
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//��һ����
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      ����˵������ʾ��������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ��������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/******************************************************************************
      ����˵������ʾ��λС������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}

//==================================================================================================
//  ʵ�ֹ���: TFT ָ��λ�� ��ʾ�������� 16x16����
//  �������: ������������
//  ����˵��: 1. λ��ѡȡ��ǡ�� �������������ʾ����ȫ
//            2. float�����ݵ���Ч����7λ double�����ݵ���Ч����16λ
//--------------------------------------------------------------------------------------------------
//  �������: X - X��������  ȡֵ��Χ - ������궨��TFT_SHOWSIZE_X
//            Y - Y��������  ȡֵ��Χ - ������궨��TFT_SHOWSIZE_Y
//            FloatNumber - ����ʾ����������  ȡֵ��Χ - -99999.99999~99999.99999
//            Count1 - ������ʾλ��  ȡֵ��Χ - 0~5
//            Count2 - С����ʾλ��  ȡֵ��Χ - 0~5
//            FontColor - ������ɫ  ȡֵ��Χ - �����ö������enumTFT_Color
//            BackgroundColor - ������ɫ ȡֵ��Χ - �����ö������enumTFT_Color
//  �������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void TFT_ShowNumber_Float_16x16(unsigned short X, unsigned short Y, float FloatNumber, unsigned char Count1, unsigned char Count2,u16 fc,u16 bc,u8 sizey,u8 mode)
{
    unsigned char Number_Integer_Array[6];          // ����ֲ����� ���ڴ洢����λ��λ����
    unsigned char Number_Decimal_Array[6];          // ����ֲ����� ���ڴ洢С��λ��λ����

    unsigned long int Number_Integer = 0;           // ����ֲ����� ��ʾ�������� ��������
    unsigned long int Number_Decimal = 0;           // ����ֲ����� ��ʾ�������� С������
    

    // ���� ��ʾ�ַ���
    // ��������float�͵���Чʮ������ֵ���Ϊ7λ��������λ + С��λ ����һ��С�ڵ���7
    while((Count1 + Count2 > 7 ))
    {
        if((Count1 > 5) && (Count1 != 0))
        {
            --Count1;
        }
        else
        {
            --Count2;
        }
    }

    // ���㸡�����ֵ�����������С������
    Number_Integer = (unsigned long int)(FloatNumber);                                          // ȡ��������
    Number_Decimal = (unsigned long int)((FloatNumber - Number_Integer + 0.000005) * 1e5);      // ȡС�����֣�1e5��ѧ������

    // װ����ʾ�ַ�
    Number_Integer_Array[0] = Number_Integer/10000 % 10 + 0x30;     // ������������
    Number_Integer_Array[1] = Number_Integer/ 1000 % 10 + 0x30;
    Number_Integer_Array[2] = Number_Integer/  100 % 10 + 0x30;
    Number_Integer_Array[3] = Number_Integer/   10 % 10 + 0x30;
    Number_Integer_Array[4] = Number_Integer/    1 % 10 + 0x30;
    Number_Integer_Array[5] = '\0';

    Number_Decimal_Array[0] = Number_Decimal/10000 % 10 + 0x30;     // ����С������
    Number_Decimal_Array[1] = Number_Decimal/ 1000 % 10 + 0x30;
    Number_Decimal_Array[2] = Number_Decimal/  100 % 10 + 0x30;
    Number_Decimal_Array[3] = Number_Decimal/   10 % 10 + 0x30;
    Number_Decimal_Array[4] = Number_Decimal/    1 % 10 + 0x30;
    Number_Decimal_Array[Count2] = '\0';
    
    // ִ����ʾ����
	LCD_ShowString(X+8,Y,&Number_Integer_Array[5-Count1],fc,bc,sizey,mode);
	
    LCD_ShowChar( X+(1+Count1)*8, Y, '.',fc,bc,sizey,mode);                                           
    LCD_ShowString(X+(2+Count1)*8, Y,&Number_Decimal_Array[0],fc,bc,sizey,mode);
}


//==================================================================================================
//  ʵ�ֹ���: TFT ָ��λ�� ��ʾ���� 16x16����
//  �������: ������������
//  ����˵��: 1. λ��ѡȡ��ǡ�� ��������ַ�����ʾ����ȫ
//            2. ����ֿⲻ���ڸ���������ʾ
//            3. 1�����ֵ�ͬ2���ֽ����
//--------------------------------------------------------------------------------------------------
//  �������: X - X��������  ȡֵ��Χ - ������궨��TFT_SHOWSIZE_X
//            Y - Y��������  ȡֵ��Χ - ������궨��TFT_SHOWSIZE_Y
//            Chinese - ����ʾ����  ȡֵ��Χ - ����������ɵ��ַ���
//            FontColor - ������ɫ  ȡֵ��Χ - �����ö������enumTFT_Color
//            BackgroundColor - ������ɫ ȡֵ��Χ - �����ö������enumTFT_Color
//  �������: ��
//--------------------------------------------------------------------------------------------------
//  |   -   |   -   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   
//==================================================================================================
void LCD_ShowChinese_16x16(uint16_t X, uint16_t Y, uint8_t *Chinese, u16 FontColor, u16 BackgroundColor)
{
    uint8_t i,j,k;                            // ����ֲ����� ����ѭ������
    uint8_t Data;                             // ����ֲ����� ������ʱ�洢

    uint8_t FontCount;                        // ����ֲ����� ���ڼ�¼�ַ��ֿ����    
    
	FontCount = sizeof(TFT_CHINESE_16x16_FontCode)/sizeof(TFT_CHINESE_16x16_FontCode[0]);

    for(i=0; i<FontCount; i++)                      // ��ʼ�ֿ�����ƥ��                             
    {
        if((*Chinese     == TFT_CHINESE_16x16_FontCode[i].Char[0])
        && (*(Chinese+1) == TFT_CHINESE_16x16_FontCode[i].Char[1]))
        {
            LCD_Address_Set(X, Y, X+16-1, Y+16-1);      // ������ʾλ��
            for(j=0; j<32; j++)                     // ѭ��д��32�ֽ� һ��������Ϣ����32�ֽ�
            {
                Data = TFT_CHINESE_16x16_FontCode[i].Code[j];
                for(k=0; k<8; k++)
                {
                    if((Data&0x80) == 0x80)         // �ж����λ�Ƿ�Ϊ1
                    {
						//LCD_WR_DATA(Data);
                        LCD_WR_DATA(FontColor);             // ���λΪ1 д���ַ���ɫ
                    }
                    else
                    {
                        LCD_WR_DATA(BackgroundColor);       // ���λΪ0 д�뱳����ɫ
                    }
                    Data <<= 1;
                }
            }
            break;                                  // �����ֿ�����ƥ��                              
        }
    }
}


