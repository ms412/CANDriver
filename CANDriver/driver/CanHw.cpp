/*
 * CanHw.cpp
 *
 * Created: 05.07.2013 22:48:05
 *  Author: Administrator
 */ 

#include "CanHw.hpp"

bool CanDriver::CanInit(uint8_t Baudrate){
	uint8_t mob;
		
	//CanReset();
	CanControl(RESET);
		
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
		
		CanIrqTx(ENABLE);
		CanIrqRx(ENABLE);
		CanGenIrq(ENABLE);
		CanControl(ENABLE);
		loop_until_bit_is_set(CANGSTA, ENFG);
	return true;
}
bool CanDriver::IOControl(uint8_t MObId,  uint8_t MObMode, uint32_t CANAddrLow, uint32_t CANAddrHigh)
{
	uint8_t savepage;
	
	//verify that MOb is free
	if (MObId > t_MaxMob){
		return false;
	}
	savepage = CanPageGet();

	//select MOb
	CanPageSet(MObId);

	//calculate and set mask
	CanMaskSet_Ext(~(CANAddrHigh - CANAddrLow));

	//calculate and set CAN ID
	CanAddrSet_Ext((~(CANAddrHigh - CANAddrLow)) & CANAddrLow);

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

bool CanDriver::Receive(CanPacket *MOb){
		if (RxBuffer.isEmpty()){
			return false;
		}
		RxBuffer.Get(*MOb);
		return true;	
}

bool CanDriver::Send(CanPacket *MOb){
	uint8_t savepage;
	uint8_t mob;
	
	savepage = CanPageGet();
		
	//SET_CANPAGE(MOb->number);
	CanPageSet(MOb->MObID);
			
	CANCDMOB = MOb->length;

	CanAddrSet_Ext(MOb->address);
	
	//SET_EXTENDED_ID(mob);
		
	for (uint8_t i=0; i < MOb->length; i++){
		CANMSG = MOb->data[i];
	}
		
	MObEnableTx();
		
	//while(!getbit(CANSTMOB, TXOK));
	while (!CANSTMOB &(1<< TXOK));
		
	//clearbit(CANSTMOB, TXOK);

	PORTB ^= (1<<PB7);
		
	return mob;
}

void CanDriver::IrqService(void){
	uint8_t savepage;
	uint8_t MOb, i;
	CanPacket MObRx, MObTx;
	

	if ((CANHPMOB & 0xf0) != 0xf0){
		//save current CANPage and set new one
		savepage = CanPageGet();
	
		MOb = (CANHPMOB >> 4);
		
		CanPageSet(MOb);

		//if Rx interrupt is present
		if (CANSTMOB & (1<<RXOK)){
			PORTB ^= (1<<PB7);
			//				MObRx.number = mob;
			MObRx.length = CANCDMOB & 0x0f;
			CanAddrGet_Ext(MObRx.address);
			for (i = 0; i < MObRx.length; i++){
				MObRx.data[i] = CANMSG;
			}

			if (!RxBuffer.isFull()){
				RxBuffer.Put(MObRx);
			}
			CANSTMOB &= 0;
			//MOb_ENABLE_RX;
			
			//MOB_IRQ_ENABLE(mob);
			
			}else if (CANSTMOB & (1<<TXOK)){  	

			}else{

			CANGIT |=0;
			}	
		}
	CANPAGE = savepage;
	
	
}

void CANIT_vect()
{
	//t_CAN.IrqService();
}