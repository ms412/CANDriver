/*
 * CanHw.cpp
 *
 * Created: 05.07.2013 22:48:05
 *  Author: Administrator
 */ 

#include "CanHw.hpp"

bool CanDriver::CanInit(uint8_t Baudrate){
	uint8_t mob;
		
	CanReset();
		
	CANGIT = 0;
	CANGIE = 0;
		
	CANIE1 = 0;
	CANIE2 = 0;

	//initial all mob objects
	for (mob = 0; mob < t_MaxMob; mob++)
	{
		CanPageSet(mob);
			
		CANCDMOB = 0;
		CANSTMOB &= 0;
			
		MObDisable();
			
		CANIDT1 = 0;
		CANIDT2 = 0;
		CANIDT3 = 0;
		CANIDT4 = 0;
			
		CANIDM1 = 0;
		CANIDM2 = 0;
		CANIDM3 = 0;
		CANIDM4 = 0;
		}
	return true;
}
bool CanDriver::IOControl(uint8_t MObId,  uint8_t MObMode, uint32_t CANAddrLow, uint32_t CANAddrHigh)
{
	uint8_t savepage;
	uint32_t maskId, canId;

	//verify that MOb is free
	if (MObId > t_MaxMob){
		return false;
	}
	savepage = CanPageGet();

	//select MOb
	CanPageSet(MObId);

	//calculate and set mask
	CanMaskExtSet(~(CANAddrHigh - CANAddrLow));

	//calculate and set CAN ID
	CanAddrExtSet((~(CANAddrHigh - CANAddrLow)) & CANAddrLow);

	// set Mode of MOb
	switch(MObMode)
	{
		case DISABLED:
		MObIrqDisable(MObId);
		MObDisable();
		break;

		case TX_DATA:
		//MObStatus &= (~(1<<mob));
		MObIrqEnable(MObId);
		MObEnableTx();		
		break;

		case RX_DATA:
		MObIrqEnable(MObId);
		MObEnableRx();
		//MObStatus |= (1<<mob);
		break;

		default:
		return false;
	}
	CanPageSet(savepage);

	return true;
}
	
bool CanDriver::CanSpeed(uint8_t Baudrate){
	CANBT1		= 0x0e;
	CANBT2		= 0x0c;
	CANBT3		= 0x37;
	
	return true;
}
