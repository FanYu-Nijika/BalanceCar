#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "blueserial.h"

char BlueSerial_RxPacket[100];
uint8_t BlueSerial_RxFlag;

void BlueSerial_Init(void)
{
	// Peripheral init is handled by CubeMX (MX_USART2_UART_Init, etc.)
	// Just enable RX interrupt to start parsing
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}

void BlueSerial_SendByte(uint8_t Byte)
{
	HAL_UART_Transmit(&huart2, &Byte, 1, HAL_MAX_DELAY);
}

void BlueSerial_SendArray(uint8_t *Array, uint16_t Length)
{
	HAL_UART_Transmit(&huart2, Array, Length, HAL_MAX_DELAY);
}

void BlueSerial_SendString(char *String)
{
	uint16_t len = 0;
	while (String[len] != '\0') len++;
	HAL_UART_Transmit(&huart2, (uint8_t *)String, len, HAL_MAX_DELAY);
}

uint32_t BlueSerial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void BlueSerial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		BlueSerial_SendByte(Number / BlueSerial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

//int fputc(int ch, FILE *f)
//{
//	BlueSerial_SendByte(ch);
//	return ch;
//}

void BlueSerial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	BlueSerial_SendString(String);
}

void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET)
	{
		uint8_t RxData = (uint8_t)(huart2.Instance->DR & (uint8_t)0x00FF);
		
		if (RxState == 0)
		{
			if (RxData == '[' && BlueSerial_RxFlag == 0)
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)
		{
			if (RxData == ']')
			{
				RxState = 0;
				BlueSerial_RxPacket[pRxPacket] = '\0';
				BlueSerial_RxFlag = 1;
			}
			else
			{
				BlueSerial_RxPacket[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
		
		__HAL_UART_CLEAR_PEFLAG(&huart2);
	}
	
	/* We also need to call the HAL IRQ handler in case of errors, but for simple parsing
	   we can just leave it to our custom code or optionally add: HAL_UART_IRQHandler(&huart2); 
	   Since we are intercepting DR manually, invoking HAL_UART_IRQHandler might consume the byte. 
	   So we handle it here explicitly. */
}
