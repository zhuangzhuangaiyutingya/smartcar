/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//�紴�����_����������С�� Made by wzz 2023.05.05

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
uint8_t isup=1;    //�����ر�־
uint16_t valueup;  //�洢�����ؿ�ʼ������
uint16_t valuedown;//�����½��ؿ�ʼ������
uint16_t w=0;      //ʵ���������
uint8_t updatecount=0;//��ʱ����ת����
int pwm1=0;
int pwm2=0;
uint16_t s = 500 ;//����w��s�ĶԱ�
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//���������Ⱥ���
{
	if(isup)
	{
		valueup = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);//����������أ��洢������
		__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//����Ϊ�½��ؼ��
		isup=0;//��һ��ִ��else���ִ���
	}
	else
	{
		valuedown = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);//������½��أ���������㣬���򲻸ı�
		__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//����Ϊ�����ؼ��
		isup=1;//��һ��ִ��if����
	     w = valuedown + updatecount*65536 - valueup;//����������
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�����ʱ����ת��ʹupdatecount��һ����֤valuedown��Զ����valueup
{
	updatecount++;
}	
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
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//һЩ��ʼ��
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,pwm1);//�����pwm��ʼ��Ϊ0
  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,pwm2);
     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);//led��˸3�κ�����
		HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
		HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
		HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
		HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
		HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,1);//�����������ģ��
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	  
	  if(w<s)//�ϰ���ƽ�
	  { 
	    pwm1=1000;//���ֹͣ
	    pwm2=1000;
	    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,pwm1);
	    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,pwm2);
	    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);//led��ʾ
	    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,2400);//�������Ƿ�ȫ
	    HAL_Delay(1500);//�ö��ת��ȥ
	    
	   if(w<s)
		  {  
		     __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,400);//����Ҳ��Ƿ�ȫ
			HAL_Delay(2000);//�ö��ת��ȥ
			if(w<s)
			{
				//���ת
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,1400);//�������
		  pwm1=750;pwm2=750;
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,pwm1);
	      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,pwm2);
			  HAL_Delay(1000);
			}		
	   else
			{
				//����ת
		   pwm1=750;pwm2=750;
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,pwm1);
	      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,pwm2);
			  HAL_Delay(500);
				
			}
			
		  }
	 else
		{
		  //��ת
		  pwm1=750;pwm2=750;
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,1);
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
	      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,pwm1);
	      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,pwm2);
			  HAL_Delay(500);
			
		 }

	  }
	  else//ǰ��
	  {
	       HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);//��ʾ��Ϩ��
	      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,1400);//�������
	    	//�����������ת
		  pwm1=500;pwm2=500;
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,1);
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,pwm1);
	      __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,pwm2);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

#ifdef  USE_FULL_ASSERT
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
