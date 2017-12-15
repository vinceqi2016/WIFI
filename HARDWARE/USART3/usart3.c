#include "usart3.h"
#include "gizwits_product.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口3驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//串口3中断服务函数
void USART3_IRQHandler(void)
{
	u8 res;	      
	if(USART3->SR&(1<<5))//接收到数据
	{	 
		res=USART3->DR; 			 
		gizPutData(&res, 1);//数据写入到缓冲区
	}  				 											 
}

//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void usart3_init(u32 pclk1,u32 bound)
{  	 
	RCC->APB2ENR|=1<<3;   	//使能PORTB口时钟  
 	GPIOB->CRH&=0XFFFF00FF;	//IO状态设置
	GPIOB->CRH|=0X00008B00;	//IO状态设置 
	
	RCC->APB1ENR|=1<<18;  	//使能串口时钟 	 
	RCC->APB1RSTR|=1<<18;   //复位串口3
	RCC->APB1RSTR&=~(1<<18);//停止复位	
	//波特率设置
 	USART3->BRR=(pclk1*1000000)/(bound);// 波特率设置	 
	USART3->CR1|=0X200C;  	//1位停止,无校验位.
	//使能接收中断 
	USART3->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(2,3,USART3_IRQn,2);//组2


}

