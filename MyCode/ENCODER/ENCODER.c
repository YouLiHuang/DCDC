#include "encoder.h"

static void First_menu_data_Refresh(void)
{
	xyz.coordinates2+=Flag.Encoder_BF;
	if(xyz.coordinates2<1)	xyz.coordinates2=8;
	if(xyz.coordinates2>8)  xyz.coordinates2=1;
}

static void Calibration_V_data_Refresh(void)
{

	String_Calibration_Vlotage[Cursor_Position-9]+=Flag.Encoder_BF;
	if(String_Calibration_Vlotage[Cursor_Position-9]<'0')//退位
	{
		/*第三位退位*/
		if(Cursor_Position==2+9)
		{
			/*先看上一位是否大于0*/
			Cursor_Position--;
			if(String_Calibration_Vlotage[Cursor_Position-9]>'0')
			{
				String_Calibration_Vlotage[Cursor_Position-9]--;
				Cursor_Position++;
				String_Calibration_Vlotage[Cursor_Position-9]='9';

			}
			else if(String_Calibration_Vlotage[Cursor_Position-9]=='0')
			{
				/*否则再看上上位是否大于0*/
				Cursor_Position--;
				if(String_Calibration_Vlotage[Cursor_Position-9]>'0')
				{
					String_Calibration_Vlotage[Cursor_Position-9]--;
					Cursor_Position++;
					String_Calibration_Vlotage[Cursor_Position-9]='9';
					Cursor_Position++;
					String_Calibration_Vlotage[Cursor_Position-9]='9';


				}
				else
				{
					Cursor_Position+=2;
					String_Calibration_Vlotage[Cursor_Position-9]='0';
				}

			}

		}
		/*第二位退位*/
		else if(Cursor_Position==1+9)
		{
			Cursor_Position--;
			if(String_Calibration_Vlotage[Cursor_Position-9]>'0')
			{
				String_Calibration_Vlotage[Cursor_Position-9]--;
				Cursor_Position++;
				String_Calibration_Vlotage[Cursor_Position-9]='9';

			}
			else
			{
				Cursor_Position++;
				String_Calibration_Vlotage[Cursor_Position-9]='0';
			}
		}
		else String_Calibration_Vlotage[Cursor_Position-9]='0';


	}
	/*进位处理*/
	if(String_Calibration_Vlotage[Cursor_Position-9]>'9')
	{

		if(Cursor_Position!=0)
		{
			String_Calibration_Vlotage[Cursor_Position-9]='0';
			Cursor_Position--;
			String_Calibration_Vlotage[Cursor_Position-9]++;
			if(String_Calibration_Vlotage[Cursor_Position-9]>'9'&&Cursor_Position!=0)//下一位也进位
			{
				String_Calibration_Vlotage[Cursor_Position-9]='0';
				Cursor_Position--;
				String_Calibration_Vlotage[Cursor_Position-9]++;
				Cursor_Position++;
			}
			Cursor_Position++;
		}
		else if(Cursor_Position==0)
		{
			String_Calibration_Vlotage[Cursor_Position-9]='9';
		}

	}

}


static void Calibration_I_data_Refresh(void)
{

	String_Calibration_Current[Cursor_Position-9]+=Flag.Encoder_BF;
	if(String_Calibration_Current[Cursor_Position-9]<'0')//退位
	{
		/*第三位退位*/
		if(Cursor_Position==2+9)
		{
			/*先看上一位是否大于0*/
			Cursor_Position--;
			if(String_Calibration_Current[Cursor_Position-9]>'0')
			{
				String_Calibration_Current[Cursor_Position-9]--;
				Cursor_Position++;
				String_Calibration_Current[Cursor_Position-9]='9';

			}
			else if(String_Calibration_Current[Cursor_Position-9]=='0')
			{
				/*否则再看上上位是否大于0*/
				Cursor_Position--;
				if(String_Calibration_Current[Cursor_Position-9]>'0')
				{
					String_Calibration_Current[Cursor_Position-9]--;
					Cursor_Position++;
					String_Calibration_Current[Cursor_Position-9]='9';
					Cursor_Position++;
					String_Calibration_Current[Cursor_Position-9]='9';


				}
				else
				{
					Cursor_Position+=2;
					String_Calibration_Current[Cursor_Position-9]='0';
				}
			}

		}
		/*第二位退位*/
		else if(Cursor_Position==1+9)
		{
			Cursor_Position--;
			if(String_Calibration_Current[Cursor_Position-9]>'0')
			{
				String_Calibration_Current[Cursor_Position-9]--;
				Cursor_Position++;
				String_Calibration_Current[Cursor_Position-9]='9';

			}
			else
			{
				Cursor_Position++;
				String_Calibration_Current[Cursor_Position-9]='0';
			}
		}
		else String_Calibration_Current[Cursor_Position-9]='0';


	}
	/*进位处理*/
	if(String_Calibration_Current[Cursor_Position-9]>'9')
	{

		if(Cursor_Position!=0)
		{
			String_Calibration_Current[Cursor_Position-9]='0';
			Cursor_Position--;
			String_Calibration_Current[Cursor_Position-9]++;
			if(String_Calibration_Current[Cursor_Position-9]>'9'&&Cursor_Position!=0)//下一位也进位
			{
				String_Calibration_Current[Cursor_Position-9]='0';
				Cursor_Position--;
				String_Calibration_Current[Cursor_Position-9]++;
				Cursor_Position++;
			}
			Cursor_Position++;
		}
		else if(Cursor_Position==0)
		{
			String_Calibration_Current[Cursor_Position-9]='9';
		}

	}

}

static void Vset_data_Refresh(void)
{
	String_Voltage[Cursor_Position]+=Flag.Encoder_BF;
	if(Cursor_Position==3||Cursor_Position==4)
	{
		if(String_Voltage[Cursor_Position]<'0')
		{
			String_Voltage[Cursor_Position]='0';
			return;
		}
		if(String_Voltage[Cursor_Position]>'9')
		{
			String_Voltage[Cursor_Position]='9';
			return;
		}
	}

	if(String_Voltage[Cursor_Position]<'0')
	{
		/*第三位退位*/
		if(Cursor_Position==2)
		{
			/*先看上一位是否大于0*/
			Cursor_Position--;
			if(String_Voltage[Cursor_Position]>'0')
			{
				String_Voltage[Cursor_Position]--;
				Cursor_Position++;
				String_Voltage[Cursor_Position]='9';

			}
			else if(String_Voltage[Cursor_Position]=='0')
			{
				/*否则再看上上位是否大于0*/
				Cursor_Position--;
				if(String_Voltage[Cursor_Position]>'0')
				{
					String_Voltage[Cursor_Position]--;
					Cursor_Position++;
					String_Voltage[Cursor_Position]='9';
					Cursor_Position++;
					String_Voltage[Cursor_Position]='9';


				}
				else
				{
					Cursor_Position+=2;
					String_Voltage[Cursor_Position]='0';
				}
			}

		}
		/*第二位退位*/
		else if(Cursor_Position==1)
		{
			Cursor_Position--;
			if(String_Voltage[Cursor_Position]>'0')
			{
				String_Voltage[Cursor_Position]--;
				Cursor_Position++;
				String_Voltage[Cursor_Position]='9';

			}
			else
			{
				Cursor_Position++;
				String_Voltage[Cursor_Position]='0';
			}
		}
		else String_Voltage[Cursor_Position]='0';
	}
	if(String_Voltage[Cursor_Position]>'9')
	{
		/*进位处理*/
		if(Cursor_Position!=0)
		{
			String_Voltage[Cursor_Position]='0';
			Cursor_Position--;
			String_Voltage[Cursor_Position]++;
			if(String_Voltage[Cursor_Position]>'9'&&Cursor_Position!=0)//下一位也进位
			{
				String_Voltage[Cursor_Position]='0';
				Cursor_Position--;
				String_Voltage[Cursor_Position]++;
				Cursor_Position++;
			}
			Cursor_Position++;
		}
		else if(Cursor_Position==0)
		{
			String_Voltage[Cursor_Position]='9';
		}

	}

	if(Inner_Mode==ENABLE)
	{
		/*确认设定电压，发送给mini board*/
		Set_Voltage = String_To_Float(String_Voltage)*100;
		if(Set_Voltage > 14900) Set_Voltage = 14900;//150V
		if(Set_Voltage <100||Set_Voltage==100)	Set_Voltage=100;
		DAC_Cmd_send(1,4,Set_Voltage);

	}

}


static void Iset_data_Refresh(void)
{
	String_Current[Cursor_Position-9]+=Flag.Encoder_BF;
	if(Cursor_Position==13||Cursor_Position==14)
	{
		if(String_Current[Cursor_Position-9]<'0')
		{
			String_Current[Cursor_Position-9]='0';
			return;
		}
		if(String_Current[Cursor_Position-9]>'9')
		{
			String_Current[Cursor_Position-9]='9';
			return;
		}
	}
	if(String_Current[Cursor_Position-9]<'0')//退位
	{
		/*第三位退位*/
		if(Cursor_Position==2+9)
		{
			/*先看上一位是否大于0*/
			Cursor_Position--;
			if(String_Current[Cursor_Position-9]>'0')
			{
				String_Current[Cursor_Position-9]--;
				Cursor_Position++;
				String_Current[Cursor_Position-9]='9';

			}
			else if(String_Current[Cursor_Position-9]=='0')
			{
				/*否则再看上上位是否大于0*/
				Cursor_Position--;
				if(String_Current[Cursor_Position-9]>'0')
				{
					String_Current[Cursor_Position-9]--;
					Cursor_Position++;
					String_Current[Cursor_Position-9]='9';
					Cursor_Position++;
					String_Current[Cursor_Position-9]='9';


				}
				else
				{
					Cursor_Position+=2;
					String_Current[Cursor_Position-9]='0';
				}
			}

		}
		/*第二位退位*/
		else if(Cursor_Position==1+9)
		{
			Cursor_Position--;
			if(String_Current[Cursor_Position-9]>'0')
			{
				String_Current[Cursor_Position-9]--;
				Cursor_Position++;
				String_Current[Cursor_Position-9]='9';

			}
			else
			{
				Cursor_Position++;
				String_Current[Cursor_Position-9]='0';
			}
		}
		else String_Current[Cursor_Position-9]='0';


	}
	/*进位处理*/
	if(String_Current[Cursor_Position-9]>'9')
	{

		if(Cursor_Position!=0)
		{
			String_Current[Cursor_Position-9]='0';
			Cursor_Position--;
			String_Current[Cursor_Position-9]++;
			if(String_Current[Cursor_Position-9]>'9'&&Cursor_Position!=0)//下一位也进位
			{
				String_Current[Cursor_Position-9]='0';
				Cursor_Position--;
				String_Current[Cursor_Position-9]++;
				Cursor_Position++;
			}
			Cursor_Position++;
		}
		else if(Cursor_Position==0)
		{
			String_Current[Cursor_Position-9]='9';
		}

	}
	if(Inner_Mode==ENABLE)
	{
		/*确认设定电压，发送给mini board*/
		Set_Current = String_To_Float(String_Current)*100;
		if(Set_Current > 4900) Set_Current = 4900;//150V
		if(Set_Current <100||Set_Current==100)	Set_Current=100;
		DAC_Cmd_send(1,5,Set_Current);

	}

}



static void VRate_data_Refresh(void)
{
	String_V_Rate[Cursor_Position-6]+=Flag.Encoder_BF;
	/*退位处理*/
	if(String_V_Rate[Cursor_Position-6]<'0')
	{

		/*小数第二位退位*/
		if(Cursor_Position==3+6)
		{
			/*先看上一位是否大于0*/
			Cursor_Position--;
			if(String_V_Rate[Cursor_Position-6]>'0')
			{
				String_V_Rate[Cursor_Position-6]--;
				Cursor_Position++;
				String_V_Rate[Cursor_Position-6]='9';

			}
			else if(String_V_Rate[Cursor_Position-6]=='0')
			{
				/*否则再看个位是否大于0*/
				Cursor_Position--;
				Cursor_Position--;
				if(String_V_Rate[Cursor_Position-6]>'0')
				{
					String_V_Rate[Cursor_Position-6]--;
					Cursor_Position++;
					Cursor_Position++;
					String_V_Rate[Cursor_Position-6]='9';//小数一位
					Cursor_Position++;
					String_V_Rate[Cursor_Position-6]='9';//小数二位


				}
				else
				{
					Cursor_Position+=3;
					String_V_Rate[Cursor_Position-6]='0';
				}
			}

		}
		/*小数第一位退位*/
		else if(Cursor_Position==2+6)
		{
			Cursor_Position--;
			Cursor_Position--;
			if(String_V_Rate[Cursor_Position-6]>'0')//个位
			{
				String_V_Rate[Cursor_Position-6]--;
				Cursor_Position++;
				Cursor_Position++;
				String_V_Rate[Cursor_Position-6]='9';

			}
			else
			{
				Cursor_Position++;
				Cursor_Position++;
				String_V_Rate[Cursor_Position-6]='0';
			}
		}
		else String_V_Rate[Cursor_Position-6]='0';


	}
	/*进位处理*/
	if(String_V_Rate[Cursor_Position-6]>'9')
	{

		if(Cursor_Position==2+6)
		{
			String_V_Rate[Cursor_Position-6]='0';
			Cursor_Position--;
			Cursor_Position--;
			String_V_Rate[Cursor_Position-6]++;
			if(String_V_Rate[Cursor_Position-6]>'9')//下一位也进位
			{
				String_V_Rate[Cursor_Position-6]='9';
				Cursor_Position++;
				Cursor_Position++;
				String_V_Rate[Cursor_Position-6]='9';

			}
			Cursor_Position++;
			Cursor_Position++;
		}
		else if(Cursor_Position==3+6)
		{
			String_V_Rate[Cursor_Position-6]='0';
			Cursor_Position--;
			String_V_Rate[Cursor_Position-6]++;
			if(String_V_Rate[Cursor_Position-6]>'9')//小数一位也进位
			{
				Cursor_Position--;
				Cursor_Position--;
				String_V_Rate[Cursor_Position-6]++;
				if(String_V_Rate[Cursor_Position-6]>'9')//个位也进位
				{
					String_V_Rate[Cursor_Position-6]='9';
					Cursor_Position++;
					Cursor_Position++;
					String_V_Rate[Cursor_Position-6]='9';
					Cursor_Position++;
					String_V_Rate[Cursor_Position-6]='9';

				}
				else//个位正常进位，小数一位为0，小数二位
				{
					Cursor_Position++;
					Cursor_Position++;
					String_V_Rate[Cursor_Position-6]='0';//小数一位为0
					Cursor_Position++;
					String_V_Rate[Cursor_Position-6]='0';//小数二位为0

				}

			}

			Cursor_Position++;
			String_V_Rate[Cursor_Position-6]='0';
		}
		else if(Cursor_Position==0+6)
		{
			String_V_Rate[Cursor_Position-6]='9';
		}

	}
	/*补充指令发送*/
	Set_V_Slope = String_To_Float(String_V_Rate)*10;
	if(Set_V_Slope > 30) Set_V_Slope = 30;//150V
	if(Set_V_Slope <1||Set_Voltage==1)	Set_V_Slope=1;
	DAC_Cmd_send(1,8,Set_V_Slope);


}

static void IRate_data_Refresh(void)
{
	String_I_Rate[Cursor_Position-6]+=Flag.Encoder_BF;
	if(String_I_Rate[Cursor_Position-6]<'0')	String_I_Rate[Cursor_Position-6]='0';
	if(String_I_Rate[Cursor_Position-6]>'9')
	{
		/*进位处理*/
		if(Cursor_Position!=0+6)
		{
			String_I_Rate[Cursor_Position-6]='0';
			Cursor_Position--;
			String_I_Rate[Cursor_Position-6]++;
			if(String_I_Rate[Cursor_Position-6]>'9'&&Cursor_Position!=0+6)//下一位也进位
			{
				String_I_Rate[Cursor_Position-6]='0';
				Cursor_Position--;
				String_I_Rate[Cursor_Position-6]++;
				Cursor_Position++;
			}
			Cursor_Position++;
		}
		else if(Cursor_Position==0+6)
		{
			String_I_Rate[Cursor_Position-6]='9';
		}

	}
	/*补充指令发送*/
	Set_I_Slope = String_To_Float(String_I_Rate)*100;
	if(Set_I_Slope > 30) Set_I_Slope = 30;//150V
	if(Set_I_Slope <1||Set_Voltage==1)	Set_I_Slope=1;
	DAC_Cmd_send(1,9,Set_I_Slope);

}

static void Save_data_Refresh(void)
{
	if(xyz.coordinates3==1)//Vset
	{
		String_Save_V[Cursor_Position]+=Flag.Encoder_BF;
		if(String_Save_V[Cursor_Position]<'0')	String_Save_V[Cursor_Position]='0';
		if(String_Save_V[Cursor_Position]>'9')
		{
			/*进位处理*/
			if(Cursor_Position!=0)
			{
				String_Save_V[Cursor_Position]='0';
				Cursor_Position--;
				String_Save_V[Cursor_Position]++;
				if(String_Save_V[Cursor_Position]>'9'&&Cursor_Position!=0)//下一位也进位
				{
					String_Save_V[Cursor_Position]='0';
					Cursor_Position--;
					String_Save_V[Cursor_Position]++;
					Cursor_Position++;
				}
				Cursor_Position++;
			}
			else if(Cursor_Position==0)
			{
				String_Save_V[Cursor_Position]='9';
			}

		}

	}
	else if(xyz.coordinates3==2)//Iset
	{
		String_Save_I[Cursor_Position]+=Flag.Encoder_BF;
		if(String_Save_I[Cursor_Position]<'0')	String_Save_I[Cursor_Position]='0';
		if(String_Save_I[Cursor_Position]>'9')
		{
			/*进位处理*/
			if(Cursor_Position!=0)
			{
				String_Save_I[Cursor_Position]='0';
				Cursor_Position--;
				String_Save_I[Cursor_Position]++;
				if(String_Save_I[Cursor_Position]>'9'&&Cursor_Position!=0)//下一位也进位
				{
					String_Save_I[Cursor_Position]='0';
					Cursor_Position--;
					String_Save_I[Cursor_Position]++;
					Cursor_Position++;
				}
				Cursor_Position++;
			}
			else if(Cursor_Position==0)
			{
				String_Save_I[Cursor_Position]='9';
			}

		}

	}

}

static void Recall_data_Refresh(void)
{
	Recall_Save_Number+=Flag.Encoder_BF;
	if(Recall_Save_Number<0)	Recall_Save_Number=9;
	if(Recall_Save_Number>9)	Recall_Save_Number=0;
}

static void Timer_data_Refresh(void)
{
	if(xyz.coordinates3==1)
	{
		String_Delay[Cursor_Position-6]+=Flag.Encoder_BF;
		if(String_Delay[Cursor_Position-6]<'0')	String_Delay[Cursor_Position-6]='0';
		if(String_Delay[Cursor_Position-6]>'9')	String_Delay[Cursor_Position-6]='9';

	}
	else if(xyz.coordinates3==2)
	{
		String_Cycle_On[Cursor_Position-6]+=Flag.Encoder_BF;
		if(String_Cycle_On[Cursor_Position-6]<'0')	String_Cycle_On[Cursor_Position-6]='0';
		if(String_Cycle_On[Cursor_Position-6]>'9')	String_Cycle_On[Cursor_Position-6]='9';

	}
	else if(xyz.coordinates3==3)
	{
		String_Cycle_Close[Cursor_Position-6]+=Flag.Encoder_BF;
		if(String_Cycle_Close[Cursor_Position-6]<'0')	String_Cycle_Close[Cursor_Position-6]='0';
		if(String_Cycle_Close[Cursor_Position-6]>'9')	String_Cycle_Close[Cursor_Position-6]='9';
	}

}

static void Light_data_Refresh(void)
{
	if(Light==0x01 && Flag.Encoder_BF<0) return;
	if(Light==0x0f && Flag.Encoder_BF>0) return;
	else Light+=Flag.Encoder_BF;
	if(Light>=0x0f)	Light=0x0f;
}

static void FAN_data_Refresh(void)
{
	if(Flag.FAN_LEVEL==1 && Flag.Encoder_BF<0) return;
	if(Flag.FAN_LEVEL==1000 && Flag.Encoder_BF>0) return;
	else Flag.FAN_LEVEL+=Flag.Encoder_BF;
	if(Flag.FAN_LEVEL>=1000)	Flag.FAN_LEVEL=1000;

	sprintf(String_FAN, "%6.2f", Flag.FAN_LEVEL);

}

/**
 * @brief  编码器按键动作响应
 * @param  none
 * @retval None
 */
static void enter_action(void)
{
	/*first menu*/
	if((xyz.coordinates1==1) && (xyz.coordinates2!=0) && (xyz.coordinates3==0))
	{
		xyz.coordinates3=1;//enter second menu
	}
	/*second menu*/
	if((xyz.coordinates1==1) && (xyz.coordinates2!=0) && (xyz.coordinates3!=0) )
	{
		switch(xyz.coordinates2)
		{
			case 1:break;
			case 2://rate
			{
				Cursor_Position++;
				if(Cursor_Position>4+6) Cursor_Position=0+6;
				break;
			}
			case 3:break;//以后补充
			case 4://cycle on/off delay
			{
				if(Cursor_Position==3+6)	Cursor_Position=4+6;
				Cursor_Position++;
				if(Cursor_Position>7+6) 	Cursor_Position=0+6;
				break;
			}
			case 5:
			{
				if(xyz.coordinates3==2)//gain
				{
					if(Cursor_Position==4+6)
					{
						Cursor_Position=5+6;
					}
					else if(Cursor_Position==5+6)
					{
						Cursor_Position=4+6;
					}

				}
				if(xyz.coordinates3==3)//frq
				{
					;
				}
				break;

			}
			default: break;
		}

	}
	/*V I Set*/
	else if((xyz.coordinates1==2) && (xyz.coordinates2!=0))
	{
		switch(xyz.coordinates2)
		{
			case 1://I set
			{
				Cursor_Position++;
				if(Cursor_Position==3+9)	Cursor_Position=4+9;
				if(Cursor_Position==6+9)	Cursor_Position=0+9;
				break;
			}
			case 2://V set
			{
				Cursor_Position++;
				if(Cursor_Position==3)	Cursor_Position=4;
				if(Cursor_Position==6)	Cursor_Position=0;
				break;
			}
		}

	}
	/*save load */
	if((xyz.coordinates1==3) && (xyz.coordinates2!=0))
	{
		switch(xyz.coordinates2)
		{
			case 1://save
			{
				if(xyz.coordinates3==1)//v est
				{
					Cursor_Position++;
					if(Cursor_Position==2)	Cursor_Position=4;
					if(Cursor_Position==5)	Cursor_Position=0;
					break;
				}
				else if(xyz.coordinates3==2)//I set
				{
					Cursor_Position++;
					if(Cursor_Position==2+9)	Cursor_Position=4;
					if(Cursor_Position==5+9)	Cursor_Position=0;
					break;
				}


			}
			case 2://recall
			{
				break;
			}

		}
	}
	if(xyz.coordinates1==1 && xyz.coordinates2==8)
	{
		switch(xyz.coordinates3)
		{
			case 1:
			case 2:
			{
				Cursor_Position++;
				if(Cursor_Position==3)	Cursor_Position=4;
				if(Cursor_Position==6)	Cursor_Position=0;
				break;
			}
			case 3:
			case 4:
			{
				Cursor_Position++;
				if(Cursor_Position==3+9)	Cursor_Position=4+9;
				if(Cursor_Position==6+9)	Cursor_Position=0+9;
				break;
			}

		}
	}

}


/******************************************************************************************************************************************************************************/
/*********************************************************************************编码器动作************************************************************************************/
/******************************************************************************************************************************************************************************/

/**
 * @brief  编码器转动数据更新
 * @param  none
 * @retval None
 */
void Encoder_Data_refresh(void)
{
	if(Flag.TIM3_IT==1)
	{
		Flag.TIM3_IT=0;
		/*first menu*/
		if((xyz.coordinates1==1) && (xyz.coordinates2!=0) && (xyz.coordinates3==0))
		{
			First_menu_data_Refresh();
		}
		/*second menu*/
		if((xyz.coordinates1==1) && (xyz.coordinates2!=0) && (xyz.coordinates3!=0) )
		{
			switch(xyz.coordinates2)
			{
				case 1:break;
				case 2://rate
				{
					if(xyz.coordinates3==1)//I rate
					{
						IRate_data_Refresh();
					}
					else if(xyz.coordinates3==2)//V rate
					{
						VRate_data_Refresh();
					}
					break;
				}
				case 3:break;
				case 4://delay cycle on/off
				{
					Timer_data_Refresh();
					break;
				}
				case 5://gain frq
				{
					if(xyz.coordinates3==2)//gain
					{
						;
					}
					else if(xyz.coordinates3==3)//frq
					{
						;
					}
					break;
				}
				default: break;
			}
		}
		/*V I set*/
		if((xyz.coordinates1==2) && (xyz.coordinates2!=0))
		{
			switch(xyz.coordinates2)
			{
				case 1:
				{
					Iset_data_Refresh();
					break;
				}
				case 2:
				{
					Vset_data_Refresh();
					break;
				}

			}

		}
		/*save load */
		if((xyz.coordinates1==3) && (xyz.coordinates2!=0))
		{
			switch(xyz.coordinates2)
			{
				case 1://save
				{
					Save_data_Refresh();
					break;
				}
				case 2://recall
				{
					Recall_data_Refresh();
					break;
				}

			}
		}
		/*light set*/
		if(xyz.coordinates1==1 && xyz.coordinates2==6 && xyz.coordinates3==1)
		{
			Light_data_Refresh();
		}
		/*FAN set*/
		if(xyz.coordinates1==1 && xyz.coordinates2==6 && xyz.coordinates3==3)
		{
			FAN_data_Refresh();
		}

		if(xyz.coordinates1==1 && xyz.coordinates2==8)
		{
			switch(xyz.coordinates3)
			{
				case 1:
				case 2:
				{
					Calibration_V_data_Refresh();
					break;
				}
				case 3:
				case 4:
				{
					Calibration_I_data_Refresh();
					break;
				}

			}
		}


	}

}



/**
 * @brief  编码器按键动作
 * @param  none
 * @retval None
 */
void Encoder_Enter_Handle(void)
{

	static 	FunctionalState ENCODER_PRESS=DISABLE;
	static int count;

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)==GPIO_PIN_RESET)
	{

		if(++count>=5)
		{
			count=5;
			ENCODER_PRESS=ENABLE;
		}


	}
	else if(ENCODER_PRESS==ENABLE && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)==GPIO_PIN_SET)
	{
		if(--count<=0)
		{
			count=0;
			ENCODER_PRESS=DISABLE;
			enter_action();//动作响应
		}

	}


}




