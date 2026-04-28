/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "blueserial.h"
#include "encoder.h"
#include "iic.h"
#include "key.h"
#include "led.h"
#include "motor.h"
#include "mpu6050.h"
#include "OLED.h"
#include "pwm.h"
#include "pid.h"
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Pid_t AnglePID, SpeedPID, TurnPID;
int16_t AX, AY, AZ, GX, GY, GZ;
int16_t LeftPWM, RightPWM;
int16_t AvePWM, DiffPWM;
uint8_t Runflag = 1;
float AngleAcc, AngleGyro, Angle;
float LeftSpeed, RightSpeed;
float AveSpeed, DiffSpeed;
float TargetSpeed, TargetTurn;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  encoder_init();
  HAL_TIM_Base_Start_IT(&htim1); 
  HAL_TIM_Base_Start_IT(&htim2); 
  HAL_TIM_Base_Start_IT(&htim3); 
  HAL_TIM_Base_Start_IT(&htim4); 
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t key_num;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (Runflag) led_on();
    else led_off();

    key_num = key_getnum();
    if (key_num == 1) Runflag = !Runflag;

    OLED_Clear();
		OLED_Printf(0, 0, OLED_6X8, " Angle");
		OLED_Printf(0, 8, OLED_6X8, " P:%05.2f", AnglePID.kp);
		OLED_Printf(0, 16, OLED_6X8, " I:%05.2f", AnglePID.ki);
		OLED_Printf(0, 24, OLED_6X8, " D:%05.2f", AnglePID.kd);
		OLED_Printf(0, 32, OLED_6X8, " T:%05.1f", AnglePID.setpoint); //T is for Target, but in this case it's always 0, so it can be omitted if desired.
		OLED_Printf(0, 40, OLED_6X8, " A:%05.1f", Angle);
		OLED_Printf(0, 48, OLED_6X8, " O:%05.0f", AnglePID.output);

		OLED_Printf(50, 0, OLED_6X8, "Speed");
		OLED_Printf(50, 8, OLED_6X8, " %05.2f", SpeedPID.kp);
		OLED_Printf(50, 16, OLED_6X8, " %05.2f", SpeedPID.ki);
		OLED_Printf(50, 24, OLED_6X8, " %05.2f", SpeedPID.kd);
		OLED_Printf(50, 32, OLED_6X8, " %05.1f", SpeedPID.setpoint);
		OLED_Printf(50, 40, OLED_6X8, " %05.1f", AveSpeed);
		OLED_Printf(50, 48, OLED_6X8, " %05.0f", SpeedPID.output);

		OLED_Printf(88, 0, OLED_6X8, "Turn");
		OLED_Printf(88, 8, OLED_6X8, " %05.2f", TurnPID.kp);
		OLED_Printf(88, 16, OLED_6X8, " %05.2f", TurnPID.ki);
		OLED_Printf(88, 24, OLED_6X8, " %05.2f", TurnPID.kd);
		OLED_Printf(88, 32, OLED_6X8, " %05.1f", TurnPID.setpoint);
		OLED_Printf(88, 40, OLED_6X8, " %05.1f", DiffSpeed);
		OLED_Printf(88, 48, OLED_6X8, " %05.0f", TurnPID.output);
		OLED_Update();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (BlueSerial_RxFlag == 1){
			char *Tag = strtok(BlueSerial_RxPacket, ",");
			if (strcmp(Tag, "key") == 0){
				char *Name = strtok(NULL, ",");
				char *Action = strtok(NULL, ",");
			}
			else if (strcmp(Tag, "slider") == 0){
				char *Name = strtok(NULL, ",");
				char *Value = strtok(NULL, ",");				
			
				if (strcmp(Name, "AngleKp") == 0){
					AnglePID.kp = atof(Value);
				}
				else if (strcmp(Name, "AngleKi") == 0){
					AnglePID.ki = atof(Value);
				}
				else if (strcmp(Name, "AngleKd") == 0){
					AnglePID.kd = atof(Value);
				}

				else if (strcmp(Name, "SpeedKp") == 0){
					SpeedPID.kp = atof(Value);
				}
				else if (strcmp(Name, "SpeedKi") == 0){
					SpeedPID.ki = atof(Value);
				}
				else if (strcmp(Name, "SpeedKd") == 0){
					SpeedPID.kd = atof(Value);
				}

				else if (strcmp(Name, "TurnKp") == 0){
					TurnPID.kp = atof(Value);
				}
				else if (strcmp(Name, "TurnKi") == 0){
					TurnPID.ki = atof(Value);
				}
				else if (strcmp(Name, "TurnKd") == 0){
					TurnPID.kd = atof(Value);
				}
			}
			else if (strcmp(Tag, "joystick") == 0){
				int8_t LH = atoi(strtok(NULL, ","));
				int8_t LV = atoi(strtok(NULL, ","));
				int8_t RH = atoi(strtok(NULL, ","));
				int8_t RV = atoi(strtok(NULL, ","));
				
				SpeedTarget = (float)LV / 25.0f;
				// SpeedPID->setpoint = (float)LV / 25.0f;
				TurnTarget = (float)RH / 25.0f;
			}
			
			BlueSerial_RxFlag = 0;
		}
		
		BlueSerial_Printf("[plot,%f,%f,%f]", AngleAcc, AngleGyro, Angle);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){ // 1ms
  static uint32_t cnt1 = 0, cnt2 = 0;
    if (htim->Instance == TIM1) {
      key_tick();

      if (++cnt1 >= 10){
        cnt1 = 0;
        mpu6050_getdata(&AX, &AY, &AZ, &GX, &GY, &GZ);
        GY -= 16;

        float alpha = 0.99;
        AngleAcc = atan2f(AY, AZ) * 180 / 3.1415926 - 0.1;
        AngleGyro += GY/32768.0 * 2000 * 0.01; //陀螺仪角度增量
        Angle = alpha * AngleGyro + (1 - alpha) * AngleAcc;
        if (Angle > 60 || Angle < -60) Runflag = 0;

        if (Runflag){
          AvePWM = pid_calc(AnglePID, AnglePID.setpoint, Angle);

          LeftPWM = AvePWM + DiffPWM / 2;
          RightPWM = AvePWM - DiffPWM / 2;
          if (LeftPWM > 100) LeftPWM = 100; if (LeftPWM < -100) LeftPWM = -100;
          if (RightPWM > 100) RightPWM = 100; if (RightPWM < -100) RightPWM = -100;

          motor_setPWM(1, LeftPWM);
          motor_setPWM(2, RightPWM);
        }
        else {
          motor_setPWM(1, 0);
          motor_setPWM(2, 0);
          pid_reset(&AnglePID);
          pid_reset(&SpeedPID);
          pid_reset(&TurnPID);
          AvePWM = DiffPWM = LeftPWM = RightPWM = 0;
          AveSpeed = DiffSpeed = LeftSpeed = RightSpeed = 0;
          TargetSpeed = TargetTurn = 0;
        }
      }

      if (++cnt2 >= 50){
        cnt2 = 0;
        LeftSpeed = encoder_get(1);
        RightSpeed = encoder_get(2);
        DiffSpeed = 
        AveSpeed = (LeftSpeed + RightSpeed) / 2;
        DiffSpeed = LeftSpeed - RightSpeed;

        if (Runflag){
          SpeedPID.setpoint = SpeedTarget;
          AnglePID.setpoint = pid_calc(&SpeedPID, TargetSpeed, DiffSpeed);
          if (AnglePID.setpoint > 15) {AnglePID.setpoint = 15;}
				  if (AnglePID.setpoint < -15) {AnglePID.setpoint = -15;}

          TurnPID.setpoint = TargetTurn;
          DiffPWM = pid_calc(&TurnPID, TargetTurn, DiffSpeed);
        }
      }
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
