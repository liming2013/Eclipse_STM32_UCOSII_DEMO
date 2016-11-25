#include "usart.h"
#include "includes.h"

void USART1_SendData(u8 ch)
{
	RS_485_TX_EN;
	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);
	
	RS_485_RX_EN;
}

void USART1_SendNData(u8 *str,u16 len)
{
 	u16 i = 0;
	RS_485_TX_EN;
	for(i = 0;i<len;i++)
	{			
		USART_SendData(USART1,str[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);
	}	
	RS_485_RX_EN;
}

void USART1_SendString(u8 *str)
{
	RS_485_TX_EN;
	while(*str)
	{
		USART_SendData(USART1,*str);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);
		str++;
	}
	RS_485_RX_EN;
}

void UART4_Bsp_Init(u32 bdrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 

	/********************����ΪUSART4����**************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
	//	GPIO_PinRemapConfig(GPIO_Remap_UART4,ENABLE);
	/*
	*  UART4_TX -> PC10 , UART4_RX ->PC11       
	*/	 
	//��ʼ��PC10��PC11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);		   

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bdrate;                                    //������115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                             //��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //���ͺͽ���ʹ��
	USART_Init(UART4, &USART_InitStructure);                                       //��ʼ������
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);                                  //�����ж�ʹ��
	USART_Cmd(UART4, ENABLE);                                                      //ʹ��ʱ����                 
	USART_ClearITPendingBit(UART4, USART_IT_TC);                                   //����ж�TCλ

	{
		NVIC_InitTypeDef   NVIC_InitStructure;

		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);              //�жϷ��飺2��

		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	         //����2�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	         //�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           	 //IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);                              //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	}
}

void UART4_SendData(u8 ch)
{	
	USART_SendData(UART4,ch);
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==0);
}

void UART4_SendNData(u8 *str,u16 len)
{
 	u16 i = 0;
	for(i = 0;i < len;i++)
	{			
		USART_SendData(UART4,str[i]);
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==0);
	}	
}

void UART4_SendString(u8 *str)
{
	while(*str)
	{
		USART_SendData(UART4,*str);
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==0);
		str++;
	}
}

void UART5_ForSetting_Init(UartParam_t *UartParam)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO,ENABLE);
  
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	         
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOC, &GPIO_InitStructure);		   

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}
	
	{
		USART_InitTypeDef USART_InitStructure;
		UART_ParamSet(&USART_InitStructure,UartParam);
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(UART5, &USART_InitStructure);
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
		USART_Cmd(UART5, ENABLE);                
		USART_ClearITPendingBit(UART5, USART_IT_TC);
	}
	
	{
		NVIC_InitTypeDef   NVIC_InitStructure;

		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;	         //����2�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	         //�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           	 //IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);                              //����ָ���Ĳ�����ʼ��VIC�Ĵ���
	}
}

void UART5_SendNData(u8 *str,u16 len)
{
 	unsigned int i = 0;
	for(i = 0;i < len;i++)
	{			
		USART_SendData(UART5,*str);
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==0);
		str++;
	}	
}

void UART5_SendData(u8 ch)
{
	USART_SendData(UART5,ch);
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==0);
}

void UART5_SendString(u8 *str)
{
	while(*str)
	{
		USART_SendData(UART5,*str);
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==0);
		str++;
	}  
}

/***********************************************�����жϺ���***************************************/
Usart_Control_t U1_Control={
	.Usart_OutDelay=0,
	.Usart_Step=1,
};

Usart_Control_t U5_Control={
	.Usart_OutDelay=0,
	.Usart_Step=1,
};

void USART1_IRQHandler(void)
{
	u8 Res;
	//static u8 step=1;
	static u8 USART1_TempBuff[2];
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		Res =USART_ReceiveData(USART1);
		
		U1_Control.Usart_OutDelay=1000;
		
		switch(U1_Control.Usart_Step)
		{
			case 1:
			{
				if(Res==0xFB)
				{
					usart1_t.buff[0]=Res;
					usart1_t.rxCnt=1;
					U1_Control.Usart_Step++;
				}
				break;
			}
			case 2:
			{
				usart1_t.buff[usart1_t.rxCnt]=Res;
				usart1_t.rxCnt++;
				
				if(Res==0xFD)
				{
					if(usart1_t.rxCnt<8)	break;
					
					prot_usart1.type=USART1_ID;
					prot_usart1.pkg.data=usart1_t.buff;
					prot_usart1.pkg.funcId=*(prot_usart1.pkg.data+1);
					prot_usart1.pkg.len=*(u16 *)(prot_usart1.pkg.data+2);
					
					if(usart1_t.rxCnt<(prot_usart1.pkg.len+8))	break;
					else
					{
						U1_Control.Usart_Step=1;
						OSQPost(Prot_Q,&prot_usart1);			//������Ϣ���У�����Ϣ���д����߳��ж����ݽ��д���
					}
				}
				
				if(usart1_t.rxCnt>RX1_4_LEN)
				{
					U1_Control.Usart_Step=1;
				}
				break;
			}
		}
  }
	
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
}

u8 CMD_Type=0;
volatile u8 CMD_REQ[2];
u8 Rece_Pkg_Flag=0;
u8 Rece_Pkg_Len[4];
u16 Rece_Pkg_Len_16=0;
u8 Rece_Index;

volatile u8 ME909_CMD_Seg=0;
#if MOD_UART4==2
u8 GPS_DoneFlag=1;
void UART4_IRQHandler(void)				//����GPS
{
	u8 Res;
	static u8 step=1;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
        OSIntEnter();    
#endif
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//���յ�������
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		
		Res =USART_ReceiveData(UART4);
		
		switch(step)
		{
			case 1:
			{
//				if(GPS_DoneFlag!=1)	break;
				if(Res==0x24)
				{
					usart4_t.buff[0]=Res;
					usart4_t.rxCnt=1;
					step++;
				}
				break;
			}
			case 2:
			{
				usart4_t.buff[usart4_t.rxCnt]=Res;
				usart4_t.rxCnt++;
				if((usart4_t.buff[usart4_t.rxCnt-2]==0x0D) && (usart4_t.buff[usart4_t.rxCnt-1]==0x0A))
				{
					//һ֡���ݽ������
					//data_len=usart4_t.rxCnt;
					step=1;
					prot_usart4.type=UART4_ID;
					prot_usart4.pkg.data=usart4_t.buff;			//���������̶����ң���Ϊ�����ǲ��̶��ģ����Գ��ȵ�λ��Ҳ�ǲ��̶���
					prot_usart4.pkg.len=usart4_t.rxCnt;
					GPS_DoneFlag=0;
					//OSQPost(Prot_Q,&prot_usart4);
					GPS_PKG_Handle(&prot_usart4);
				}
				break;
			}
			case 3:
			{
				break;
			}
			default:
				break;
		}
	}

#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
        OSIntExit();  											 
#endif
}
#elif MOD_UART4==1
#define REC_MODE 2
void UART4_IRQHandler(void)				//����4Gģ��
{
	volatile u8 Res;			//����ӹؼ��֣��������׳���
	u8 sta=0;
	u8 validFlag=0;
	u8 *pkgp=0;
	u16 pkg_cnt=0;
	static u8 step=1;
	static u8 separNum=1;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
  OSIntEnter();    
#endif
	/*
	��ʼ��---0D 0A
	������---
					+ERROR:xxx 0D 0A
					OK 0D 0A
					0D 0A
	*/
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//���յ�������
	{
		static u8 ringNum=0;
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		
		Res =USART_ReceiveData(UART4);
		
		if(AT_Debug_Flag==1)
		{
			usart4_t.buff[usart4_t.rxCnt]=Res;
			usart4_t.rxCnt++;
			//return;
		}
		else
		{
			#if REC_MODE==0
			if(Rece_Pkg_Flag!=1)
			{
				CMD_REQ[0]=CMD_REQ[1];
				CMD_REQ[1]=Res;
				
				switch(step)
				{
					case 1:
						if(CMD_REQ[0]==0x0D&&CMD_REQ[1]==0x0A)
						{
							step++;
						}
						break;
					case 2:
						if(Res==0x0D||Res=='A')		//
						{
							step=1;
						}
						else
						{
							usart4_t.buff[usart4_t.rxCnt]=0x0D;
							usart4_t.buff[usart4_t.rxCnt+1]=0x0A;
							usart4_t.buff[usart4_t.rxCnt+2]=Res;
							usart4_t.rxCnt+=3;
							step=11;
						}
						break;
					case 11:		//
						usart4_t.buff[usart4_t.rxCnt]=Res;
						usart4_t.rxCnt++;
						if(CMD_REQ[0]==0x0D&&CMD_REQ[1]==0x0A)
						{
							if(strncmp((char *)(usart4_t.buff+2),"OK",2)==0)
							{
								step=1;
								pkgp=usart4_t.buff;
								pkg_cnt=usart4_t.rxCnt;
								usart4_t.rxCnt=0;
								Rece_Pkg_Flag=1;
								ME909_CMD_Seg=0;
							}
							else if(strncmp((char *)(usart4_t.buff+2),"+CME ERROR",10)==0)
							{
								step=1;
								pkgp=usart4_t.buff;
								pkg_cnt=usart4_t.rxCnt;
								usart4_t.rxCnt=0;
								Rece_Pkg_Flag=1;
								ME909_CMD_Seg=0;
							}
							else
							{
								if(ME909_CMD_Seg==0)
								{
									//ME909_CMD_Seg--;
									step=1;
									pkgp=usart4_t.buff;
									pkg_cnt=usart4_t.rxCnt;
									usart4_t.rxCnt=0;
									
									Rece_Pkg_Flag=1;
								}
								else if(ME909_CMD_Seg>0)
								{
									ME909_CMD_Seg--;
									step=1;
								}
							}
						}
						break;
					default:
						break;
				}
			}
			
			if(Rece_Pkg_Flag==1)		//���յ�һ�����������ݰ�
			{
				if(gb_flag.Debug_Flag==1)
				{
					PROT_SendNData(UART5_ID,pkgp,pkg_cnt);
				}
				else
				{
					ME909_ReceiveHandle(UART4_ID,pkgp,pkg_cnt);			//�����ݰ����д���
				}
				Rece_Pkg_Flag=0;
			}
			#elif REC_MODE==1
			if(Rece_Pkg_Flag!=1)
			{
				CMD_REQ[0]=CMD_REQ[1];
				CMD_REQ[1]=Res;
				
				switch(step)
				{
					case 1:
						if(CMD_REQ[0]==0x0D&&CMD_REQ[1]==0x0A)
						{
							step++;
						}
						break;
					case 2:
						if(Res==0x0D||Res=='A')		//
						{
							step=1;
						}
						else
						{
//							if(usart4_t.rxCnt==0)
//								buff_clear(usart4_t.buff,RX1_4_LEN);
							usart4_t.buff[usart4_t.rxCnt]=0x0D;
							usart4_t.buff[usart4_t.rxCnt+1]=0x0A;
							usart4_t.buff[usart4_t.rxCnt+2]=Res;
							usart4_t.rxCnt+=3;
							
							if(ME909_CMD_Seg==1)
							{
								step=11;
							}
							else
							{
								step=21;
							}
						}
						break;
					case 11:		//����ָ���ʶ��
						usart4_t.buff[usart4_t.rxCnt]=Res;
						usart4_t.rxCnt++;
						
						if((usart4_t.rxCnt>=6)&&(mystrncmp((u8 *)(usart4_t.buff+usart4_t.rxCnt-4),"OK\r\n",4)==0))
						{
							step=1;
							pkgp=usart4_t.buff;
							pkg_cnt=usart4_t.rxCnt;
							usart4_t.rxCnt=0;
							Rece_Pkg_Flag=1;
							ME909_CMD_Seg=0;
						}
						else if((usart4_t.rxCnt>=12)&&(mystrncmp((u8 *)(usart4_t.buff+usart4_t.rxCnt-10),"+CME ERROR",10)==0))
						{
							step++;
						}
						break;
					case 12:
						usart4_t.buff[usart4_t.rxCnt]=Res;
						usart4_t.rxCnt++;
					
						if(CMD_REQ[0]==0x0D&&CMD_REQ[1]==0x0A)
						{
							step=1;
							pkgp=usart4_t.buff;
							pkg_cnt=usart4_t.rxCnt;
							usart4_t.rxCnt=0;
							Rece_Pkg_Flag=1;
							ME909_CMD_Seg=0;
						}
						break;
					case 21:		//����ָ���ʶ��
						usart4_t.buff[usart4_t.rxCnt]=Res;
						usart4_t.rxCnt++;
					
						if(CMD_REQ[0]==0x0D&&CMD_REQ[1]==0x0A)
						{
							if(ME909_CMD_Seg==0)
							{
								//ME909_CMD_Seg--;
								step=1;
								pkgp=usart4_t.buff;
								pkg_cnt=usart4_t.rxCnt;
								usart4_t.rxCnt=0;
								
								Rece_Pkg_Flag=1;
							}
							else if(ME909_CMD_Seg>0)
							{
								ME909_CMD_Seg--;
								step=1;
							}
						}
						break;
					default:
						break;
				}
			}
			
			if(Rece_Pkg_Flag==1)		//���յ�һ�����������ݰ�
			{
				if(gb_flag.Debug_Flag==1)
				{
					PROT_SendNData(UART5_ID,pkgp,pkg_cnt);
				}
				else
				{
					ME909_ReceiveHandle(UART4_ID,pkgp,pkg_cnt);			//�����ݰ����д���
				}
				Rece_Pkg_Flag=0;
			}
			#elif REC_MODE==2
			if(Rece_Pkg_Flag!=1)
			{
				CMD_REQ[0]=CMD_REQ[1];
				CMD_REQ[1]=Res;
				
				switch(step)
				{
					case 1:
						if(CMD_REQ[0]==0x0D&&CMD_REQ[1]==0x0A)
						{
							step++;
						}
						break;
					case 2:
						if(Res==0x0D||Res=='A')		//��Ч��ʽ
						{
							step=1;
						}
						else											//��Ч��ʽ
						{
							ME909_cmd.type=0;
							usart4_t.rxCnt=0;
							usart4_t.buff[usart4_t.rxCnt]=0x0D;
							usart4_t.buff[usart4_t.rxCnt+1]=0x0A;
							usart4_t.buff[usart4_t.rxCnt+2]=Res;
							usart4_t.rxCnt+=3;
							step++;
						}
						break;
					case 3:
						usart4_t.buff[usart4_t.rxCnt]=Res;
						usart4_t.rxCnt++;
						if(CMD_REQ[0]==0x0D&&CMD_REQ[1]==0x0A)
						{
							switch(*(usart4_t.buff+2))
							{
								case '+':
									if(strncmp((char *)(usart4_t.buff+2),"+CME ERROR:",11)==0)
									{
										ME909_cmd.type=2;
										separNum=2;
										step=1;
										OSMboxPost(me909_mbox,&ME909_cmd);
										break;
									}
									else if(strncmp((char *)(usart4_t.buff+2),"+CNUM:",6)==0)
									{
										ME909_cmd.type=3;
										separNum=4;
										step++;
										break;
									}
									else if(strncmp((char *)(usart4_t.buff+2),"+COPS:",6)==0)
									{
										ME909_cmd.type=4;
										separNum=4;
										step++;
										break;
									}
									else if(strncmp((char *)(usart4_t.buff+2),"+CMT:",5)==0)
									{
										ME909_cmd.type=5;
										separNum=3;
										step++;
										break;
									}
									else if(strncmp((char *)(usart4_t.buff+2),"+CRING:",7)==0)				//��������
									{
										if(RingNum==0)
										{
											if(WM_Para.ConnectMode==2)		//�绰����
											{
												sms_param.con_type=2;		//�绰����---��ȡ����
												sms_param.phone=sms_srcphone;
												OSMboxPost(sms_mbox,&sms_param);
											}
										}
										RingNum++;
										
										ME909_cmd.type=6;
										separNum=2;
										step=1;
										break;
									}
									else if(strncmp((char *)(usart4_t.buff+2),"+CLCC:",6)==0)				//��ѯ�������
									{
										ME909_cmd.type=7;
										separNum=4;
										step++;
										break;
									}
									else
									{
										step=1;
									}
									break;
								case '^':
									if(strncmp((char *)(usart4_t.buff+2),"^IPDATA:",8)==0)
									{
										u8 *datap=0;
										u8 *lenp=0;
										
										ME909_cmd.type=21;
										separNum=2;
										step=1;
										
										hb_delay=hb_period;
										
										lenp=strchr((char *)(usart4_t.buff+2),',')+1;
										datap=strchr(lenp,',')+1;
										
										prot_usart4.type=UART4_ID;
										prot_usart4.pkg.data=datap;
										prot_usart4.pkg.len=atoi(lenp);
										
										#if 0
										SPI_Flash_Write(datap,BIN_ADDR+Bin_Control.Bin_Size,prot_usart4.pkg.len);
										Bin_Control.Bin_Size+=prot_usart4.pkg.len;
										#else
										OSQPost(Prot_Q,&prot_usart4);
										#endif
										break;
									}
									else if(strncmp((char *)(usart4_t.buff+2),"^HCSQ:",6)==0)
									{
										ME909_cmd.type=22;
										if(ME909_CMD_Seg>0)
										{
											separNum=4;
											step++;
											break;
										}
										else
										{
											extern u8 Get_RSSI_Rq;
											separNum=2;
											Signal_Param.csq=GetRSSI(usart4_t.buff);
											OSMboxPost(me909_mbox,&ME909_cmd);
											step=1;
											break;
										}
									}
									else if(strncmp((char *)(usart4_t.buff+2),"^IPSENDEX:",10)==0)
									{
										ME909_cmd.type=23;
										separNum=4;
										step++;
										break;
									}
									else if(strncmp((char *)(usart4_t.buff+2),"^IPSTATE:",9)==0)		//̽��TCP�����
									{
										u8 linkid=0,linksta=0,linkerr=0;
										u8 *srcp=usart4_t.buff+2;
										
										srcp=strchr(srcp,':')+2;
										linkid=*srcp;
										
										srcp=strchr(srcp,',')+1;
										linksta=*srcp;
										
										srcp=strchr(srcp,',')+1;
										linkerr=*srcp;
										
										if( (linkid==0x31) && (linksta==0x30) && (linkerr==0x30) )
										{
											hb_mbflag=20;
											OSMboxPost(hb_mbox,&hb_mbflag);				//��ʱ����������
										}
										
										separNum=2;
										step=1;
										ME909_cmd.type=24;
										break;
									}
									else if(strncmp((char *)(usart4_t.buff+2),"^CEND:",6)==0)			//����ֹͣ---CLCC������ʾ
									{
										RingNum=0;
										if(WM_Para.ConnectMode==2)		//�绰����
										{
											sms_param.con_type=3;		//�绰����---��������
											OSMboxPost(sms_mbox,&sms_param);
										}
										
										separNum=2;
										step=1;
										ME909_cmd.type=25;
									}
									else
									{
										step=1;
									}
									break;
								case 'O':
									if(strncmp((char *)(usart4_t.buff+2),"OK",2)==0)
									{
										ME909_cmd.type=1;
										separNum=2;
										step=1;
										OSMboxPost(me909_mbox,&ME909_cmd);
										break;
									}
									else
									{
										step=1;
									}
									break;
								default:
									step=1;
									break;
							}
						}
						
						break;
					case 4:
						usart4_t.buff[usart4_t.rxCnt]=Res;
						usart4_t.rxCnt++;
						if(CMD_REQ[0]==0x0D&&CMD_REQ[1]==0x0A)
						{
							if(separNum>3)	separNum--;
							else						//������
							{
								switch(ME909_cmd.type)
								{
									case 3:																											//+CNUM:
									{
										u8 *phonest=0,*phoneed=0;
										ME909_cmd.type=15;
										phonest=strstr((char *)(usart4_t.buff+2),"\",\"");
										if(phonest!=0)
										{
											phonest+=3;
											if(strncmp((char *)phonest,"+86",3)==0)	phonest+=3;
											phoneed=strchr((char *)phonest,0x22);										//����"��Ϊ������
											if(phoneed>phonest){
												u8 len=phoneed-phonest;
												strncpy(GPRS_Param.dial_num,phonest,len);							//�洢�绰����
												strncpy(Signal_Param.phone_num,phonest,len);					//�洢�绰����
											}
										}
										OSMboxPost(me909_mbox,&ME909_cmd);
										break;
									}
									case 4:																											//+COPS:
									{
										u8 *tempp=usart4_t.buff+2;
										//CHN-UNICOM/UNICOM   CHINA MOBILE/CMCC	CHN-CT/CT
										if(strstr((char *)tempp,"\"CHINA MOBILE\"")!=0)						gb_flag.sim_cops=1;						//�й��ƶ�---������
										else if(strstr((char *)tempp,"\"CMCC\"")!=0)							gb_flag.sim_cops=1;						//�й��ƶ�---������
										else if(strstr((char *)tempp,"\"46000\"")!=0)							gb_flag.sim_cops=1;						//�й��ƶ�---���ִ���
										else if(strstr((char *)tempp,"\"CHN-UNICOM\"")!=0)				gb_flag.sim_cops=2;						//�й���ͨ---������
										else if(strstr((char *)tempp,"\"UNICOM\"")!=0)						gb_flag.sim_cops=2;						//�й���ͨ---������
										else if(strstr((char *)tempp,"\"46001\"")!=0)							gb_flag.sim_cops=2;						//�й���ͨ---���ִ���
										else if(strstr((char *)tempp,"\"CHN-CT\"")!=0)						gb_flag.sim_cops=3;						//�й�����---������
										else if(strstr((char *)tempp,"\"CT\"")!=0)								gb_flag.sim_cops=3;						//�й�����---������
										else if(strstr((char *)tempp,"\"46011\"")!=0)							gb_flag.sim_cops=3;						//�й�����---���ִ���
									
										OSMboxPost(me909_mbox,&ME909_cmd);
										break;
									}
									case 5:																											//+CMT:
									{
										u8 i=0;
										u8 *tempp=usart4_t.buff+2;
										u8 *srcp=0,*datasp=0,*dataep=0;
										srcp=strchr((char *)tempp,0x22)+1;										//����"��Ϊ��ʼ��
										if(*srcp=='+')			//+86��ͷ
											srcp+=3;
										for(i=0;i<11;i++)
										{
											*(sms_srcphone+i)=*(srcp+i);
										}
										datasp=strstr((char *)srcp,"\r\n")+2;
										dataep=strstr((char *)datasp,"\r\n");
										if(dataep>datasp)
											sms_param.len=(dataep-datasp);
										for(i=0;i<sms_param.len;i++)
										{
											*(sms_buff+i)=*(datasp+i);
										}
										
										if(WM_Para.ConnectMode==1)				//���ż���
										{
											sms_param.con_type=1;		//���ż���
											sms_param.msg=sms_buff;
											sms_param.phone=sms_srcphone;
											sms_param.porttype=USB_ID;
											OSMboxPost(sms_mbox,&sms_param);
										}
										break;
									}
									case 7:																											//+CLCC:
									{
										u8 i=0;
										u8 *tempp=usart4_t.buff+2;
										u8 *srcp=0;
										srcp=strchr((char *)tempp,0x22)+1;										//����"��Ϊ��ʼ��
										if(*srcp=='+')			//+86��ͷ
											srcp+=3;
										for(i=0;i<11;i++)
										{
											*(sms_srcphone+i)=*(srcp+i);
										}
										
										OSMboxPost(me909_mbox,&ME909_cmd);
										break;
									}
									case 21:																										//^IPDATA:
									{
//										u8 i=0;
//										u8 *tempp=pkgp;
//										u8 *srcp=0;
//										srcp=strchr((char *)tempp,0x22)+1;										//����"��Ϊ��ʼ��
//										if(*srcp=='+')			//+86��ͷ
//											srcp+=3;
//										for(i=0;i<11;i++)
//										{
//											*(sms_srcphone+i)=*(srcp+i);
//										}
										
										break;
									}
									case 22:																										//^HCSQ:
									{
										extern u8 Get_RSSI_Rq;
				
										Signal_Param.csq=GetRSSI(usart4_t.buff);
										OSMboxPost(me909_mbox,&ME909_cmd);
										break;
									}
									case 23:																										//^IPSENDEX:
									{
										OSMboxPost(me909_mbox,&ME909_cmd);
										break;
									}
									default:
										step=1;
										break;
								}
								step=1;
								separNum=0;
							}
						}
						break;
					default:
						break;
				}
			}
			#endif
		}
	}

#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
        OSIntExit();  											 
#endif
}
#endif


void UART5_IRQHandler(void)
{
	u8 Res;
	//static u8 step=1;
	static u8 USART5_TempBuff[2];
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		
		Res =USART_ReceiveData(UART5);
		U5_Control.Usart_OutDelay=1000;
		if(gb_flag.Debug_Flag==1)
		{
//			usart5_t.buff[usart5_t.rxCnt]=Res;
//			usart5_t.rxCnt++;
			switch(U5_Control.Usart_Step)
			{
				case 1:
					if(Res=='A')
					{
						U5_Control.Usart_Step++;
						usart5_t.buff[0]=Res;
					}
					break;
				case 2:
					if(Res=='T')
					{
						U5_Control.Usart_Step++;
						usart5_t.buff[1]=Res;
						usart5_t.rxCnt=2;
					}
					break;
				case 3:
					usart5_t.buff[usart5_t.rxCnt]=Res;
					usart5_t.rxCnt++;
				
					if(Res=='\r')	U5_Control.Usart_Step++;
					break;
				case 4:
					if(Res=='\n')
					{
						usart5_t.buff[usart5_t.rxCnt]=Res;
						usart5_t.rxCnt++;
						ME909_SendNData(usart5_t.buff,usart5_t.rxCnt);
						U5_Control.Usart_Step=1;
					}
					break;
				default:
					break;
			}
		}
		else
		{
			switch(U5_Control.Usart_Step)
			{
				case 1:
				{
					if(Res==0xFB)
					{
						usart5_t.buff[0]=Res;
						usart5_t.rxCnt=1;
						U5_Control.Usart_Step++;
					}
					break;
				}
				case 2:
				{
					usart5_t.buff[usart5_t.rxCnt]=Res;
					usart5_t.rxCnt++;
					
					if(Res==0xFD)
					{
						if(usart5_t.rxCnt<8)	break;
						
						prot_usart5.type=UART5_ID;
						prot_usart5.pkg.data=usart5_t.buff;
						prot_usart5.pkg.funcId=*(prot_usart5.pkg.data+1);
						prot_usart5.pkg.len=*(u16 *)(prot_usart5.pkg.data+2);
						
						if(usart5_t.rxCnt<(prot_usart5.pkg.len+8))	break;
						else
						{
							U5_Control.Usart_Step=1;
							OSQPost(Prot_Q,&prot_usart5);			//������Ϣ���У�����Ϣ���д����߳��ж����ݽ��д���
						}
					}
					
					if(usart5_t.rxCnt>RX5_LEN)
					{
						U5_Control.Usart_Step=1;
					}
					break;
				}
			}
		}
  }
	
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
}
/**********************************************�����жϺ�������*****************************************/


void rs485_ctrl_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //��ת����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //���������ʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);     
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //��ת����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //���������ʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void buff_clear(u8 *buff,u32 len)
{
	u32 i=0;
	for(i=0;i<len;i++)
	{
		*(buff+i)=0;
	}
}

void QueueIn(u8 ch,DataQueue *m_cmdq)
{
	m_cmdq->buff[m_cmdq->rear]=ch;
	m_cmdq->rear=(m_cmdq->rear+1)%QUEUE_LEN;
}

u8 QueueOut(DataQueue *m_cmdq)
{
	u8 ch=0;
	if(m_cmdq->front!=m_cmdq->rear)
	{
		ch=m_cmdq->buff[m_cmdq->front];
		m_cmdq->buff[m_cmdq->front]=0;
		m_cmdq->front=(m_cmdq->front+1)%QUEUE_LEN;
	}
	return ch;
}

u8 QueueInForBuff(u8 *buff,DataQueue *m_cmdq,u16 len)
{
	u8 err=0;
	u16 i=0;
	for(i=0;i<len;i++)
	{
		QueueIn(*(buff+i),m_cmdq);
	}
	return 1;
}

u8 QueueOutForBuff(u8 *buff,DataQueue *m_cmdq,u16 len)
{
	u8 err=0;
	u16 i=0;
	for(i=0;i<len;i++)
	{
		if(m_cmdq->front!=m_cmdq->rear)
		{
			*(buff+i)=m_cmdq->buff[m_cmdq->front];
			m_cmdq->buff[m_cmdq->front]=0;
			m_cmdq->front=(m_cmdq->front+1)%QUEUE_LEN;
		}
		else
		{
			err=1;
			break;
		}
	}
	return err;
}

void QueueOutForUsart(USART_TypeDef *USARTx,DataQueue *qd,u16 len,u8 type)		//=1,RAW��ʽ���ͣ�=2,16����ʽ�ַ�������
{
	u16 i=0;
	for(i=0;i<len;i++)
	{
//		if(qd->front!=qd->rear)
//		{
			if(type==1)
			{
				USART_SendData(USARTx,qd->buff[qd->front]);
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==0);
			}
			else if(type==2)
			{
				u8 hexbuff[2];
				HexToStr(qd->buff[qd->front],hexbuff);
				USART_SendData(USARTx,hexbuff[0]);
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==0);
				USART_SendData(USARTx,hexbuff[1]);
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==0);
			}
			
			qd->buff[qd->front]=0;
			qd->front=(qd->front+1)%QUEUE_LEN;
//		}
//		else
//		{
//			USART_SendData(USARTx,0x0);
//			while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==0);
//			break;
//		}
	}
}

void ClearQueue(DataQueue *m_cmdq)
{
	buff_clear(m_cmdq->buff,QUEUE_LEN);
	m_cmdq->front=0;
	m_cmdq->rear=0;
}

/*
����0˵������Ϊ�գ�������в�Ϊ��
�涨rear=front��ʾ��
*/
u8 QueueStatus(DataQueue *dq)
{
	u8 sta=0;
	if(dq->front==dq->rear)
	{
		sta=0;
	}
	else
	{
		sta=1;
	}
	return sta;
}

/*
@descript:��ȡ������ʹ�õĳ���
@param1:
@param2:
@return:
*/
u16 QueueLen(DataQueue *dq)
{
	u16 len=0;
	if(dq->rear<dq->front)	len=dq->rear+QUEUE_LEN-dq->front;
	else	len=dq->rear-dq->front;
	return len;
}

/*
@descript:��ȡ����δʹ�õĳ���
@param1:
@param2:
@return:
*/
u16 QueueSurpLen(DataQueue *dq)
{
	u16 len=0;
	if(dq->rear<dq->front)	len=dq->rear+QUEUE_LEN-dq->front;
	else	len=dq->rear-dq->front;
	
	len=QUEUE_LEN-len;
	return len;
}

u16 GetCmd(u8 *m_cmdbuff,DataQueue *m_cmdq)
{
	u16 len,i;
	u8 cnt=0;
	u8 bufftemp[2];
	u16 index;
	
	u8 startflag=0;
	
	//��ȡ������Ч���ݳ���
	if(m_cmdq->rear<m_cmdq->front)	len=m_cmdq->rear+QUEUE_LEN-m_cmdq->front;
	else	len=m_cmdq->rear-m_cmdq->front;
	
	while(len--)
	{
		bufftemp[0]=bufftemp[1];
		bufftemp[1]=QueueOut(m_cmdq);
		
//		if(bufftemp[0]==CMD_START[0]&&bufftemp[1]==CMD_START[1])
//		{
//			startflag=1;
//			m_cmdbuff[0]=bufftemp[0];
//			//m_cmdbuff[1]=bufftemp[1];
//			index=1;
//		}
//		
//		if(startflag==1)
//		{
//			m_cmdbuff[index]=bufftemp[1];
//			
//			if(bufftemp[0]==CMD_END[0]&&bufftemp[1]==CMD_END[1])	break;
//			
//			index++;
//		}
	}
	return index;
}

//AT^IPSEND=1,"12345"\r\n
//AT^IPSENDEX=1,1,"31323334"\r\n
u16 Format_Data(unsigned char *buff,unsigned char *m_cmd,u8 *datatype)		//��ATָ������ȡ��Ч����
{
	u16 len=0,i=0;
	u8 checkflag=0;
	unsigned char *pstart,*pend;
	
	if(strncmp(m_cmd,"AT^IPSEND=",10)==0)
	{
		checkflag=1;
		*datatype=1;
	}
	else if(strncmp(m_cmd,"AT^IPSENDEX=",12)==0)
	{
		checkflag=1;
		*datatype=2;
	}
	else
	{
		checkflag=0;
		*datatype=0;
		return 0;
	}
	
	if(checkflag==1)
	{
		pstart=strchr(m_cmd,'\"');		//���ҵ�һ���ֽڷ�
		pend=strchr(pstart+1,'\"');		//���ҵڶ����ֽڷ�
		if(pend>pstart)
		{
			len=pend-pstart-1;					//��ȡ����
			for(i=0;i<len;i++)
			{
				*(buff+i)=*(pstart+1+i);
			}
			*(buff+len)=0;		//Ϊ�˱��ռ���һ��������
			return len;
		}
		else
		{
			return 0;
		}
	}
	
	return 0;
}


void USART5_Check()
{
	
}

