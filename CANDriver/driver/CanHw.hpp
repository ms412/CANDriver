/*
 * CanHw.hpp
 *
 * Created: 05.07.2013 22:48:29
 *  Author: Administrator
 */ 


#ifndef CANHW_H_
#define CANHW_H_

#include <avr/io.h>
#include "../utils/fifo.hpp"


 typedef struct{
	  uint32_t address;
	  uint8_t MObID;
	  uint8_t length;
	  uint8_t data[8];
 }CanPacket;

  enum Baudrate {CAN_50K, CAN_100K, CAN_125K, CAN_200K, CAN_250K, CAN_500K, CAN_1000K };

  enum MObMode{DISABLED, TX_DATA, RX_DATA, TX_REMOTE, AUTO_REPLY};
	  
  enum Status{DISABLE, ENABLE, RESET};
	  
  extern "C" void CANIT_vect(void) __attribute__ ((signal));
	  
  //extern class CanDriver t_CAN;

class CanDriver 
{
public:
	CanDriver(){
		t_MaxMob = 15;
	}
	
	bool CanInit(uint8_t Baudrate);
	bool IOControl(uint8_t MObId,  uint8_t MObMode, uint32_t CANAddrLow, uint32_t CANAddrHigh);
	bool Receive(CanPacket *MOb);
	bool Send(CanPacket *MOb);
	
	void IrqService(void);
	
private:

	static Fifo<CanPacket,5> TxBuffer;
	static Fifo<CanPacket,5> RxBuffer;
	bool CanSpeed(uint8_t Baudrate);
	
	bool CanControl(uint8_t Status){
		if(Status == ENABLE){
			CANGCON |= (1<<ENASTB);
			return true;
		}else if (Status == DISABLE){
			CANGCON &= (~(1<<ENASTB));
			return true;
		}else{
			CANGCON = (1<<SWRES);
			return true;
		}
		return false;
	}      
	
	bool CanReset(void){
		CANGCON = (1<<SWRES);
		return true;
	}
	
	bool CanDisable(void){
		CANGCON &= (~(1<<ENASTB));
		return true;
	}
	
	bool CanEnable(void){
		CANGCON |= (1<<ENASTB);
		return true;
	}
	
	bool CanGenIrq(uint8_t Status){
		if(Status == ENABLE){
			CANGIE |= (1<<ENIT);
		}else{
			CANGIE &= (~(1<<ENIT));
		}	
		return true;	
	}
	
	bool CanIrqTx(uint8_t Status){
		if(Status == ENABLE){
			CANGIE |= (1<<ENRX);
		}else{
			CANGIE &= (~(1<<ENRX));
		}	
		return true;
	}
	
	bool CanIrqRx(uint8_t Status){
		
		if(Status == ENABLE){
			CANGIE |= (1<<ENTX);	
		}else{
			CANGIE &= (~(1<<ENTX));
		}	
		return true;
	}
	
	void CanPageSet(uint8_t MObId){
		CANPAGE = (MObId << 4);
	}
	
	uint8_t CanPageGet(void){
		return (CANPAGE >> 4);
	}
	
	bool CanMaskSet_Ext(uint32_t maskId){
		CANIDM1 = (((*((uint8_t *)(&maskId)+3))<<3)+((*((uint8_t *)(&maskId)+2))>>5));
		CANIDM2 = (((*((uint8_t *)(&maskId)+2))<<3)+((*((uint8_t *)(&maskId)+1))>>5));
		CANIDM3 = (((*((uint8_t *)(&maskId)+1))<<3)+((* (uint8_t *)(&maskId)   )>>5));
		CANIDM4 = ((*  (uint8_t *)(&maskId)   )<<3);
		CANIDM4 = (1<<IDEMSK);
		return true;
	}
	
	bool CanAddrSet_Ext(uint32_t maskId){
		CANIDT1 = (((*((uint8_t *)(&maskId)+3))<<3)+((*((uint8_t *)(&maskId)+2))>>5));
		CANIDT2 = (((*((uint8_t *)(&maskId)+2))<<3)+((*((uint8_t *)(&maskId)+1))>>5));
		CANIDT3 = (((*((uint8_t *)(&maskId)+1))<<3)+((* (uint8_t *)(&maskId)   )>>5));
		CANIDT4 = ((*  (uint8_t *)(&maskId)   )<<3);
		CANCDMOB |= (1<<IDE);
		return true;
	}
	
	bool CanAddrGet_Ext(uint32_t address){
		*((uint8_t *)(&(address))+3) =  CANIDT1>>3; 
		*((uint8_t *)(&(address))+2) = (CANIDT2>>3)+(CANIDT1<<5);
		*((uint8_t *)(&(address))+1) = (CANIDT3>>3)+(CANIDT2<<5);
	    *((uint8_t *)(&(address))  ) = (CANIDT4>>3)+(CANIDT3<<5); 
		return true;
	}
	
	bool MObDisable(void){
		CANCDMOB &= (~(1<<CONMOB1));
		CANCDMOB &= (~(1<<CONMOB0));
		return true;	
	}
	
	bool MObEnableTx(void){
		CANCDMOB &= (~(1<<CONMOB1));
		CANCDMOB |= (1<<CONMOB0);
		return true;
	}
	
	bool MObEnableRx(void){
		CANCDMOB |= (1<<CONMOB1);
		CANCDMOB &= (~(1<<CONMOB0));	
		return true;
	}
	
	bool MObIrqDisable(uint8_t MObId){
		(((MObId) < (8))?(CANIE2 &= (~(1<<(MObId)))):(CANIE1 &= (~(1<<MObId - 8))));
		return true;
	}
	
	bool MObIrqEnable(uint8_t MObId){
		(((MObId) < (8))?(CANIE2 |= (1<<(MObId))):(CANIE1 |= (1<<MObId - 8)));
		return true;
	}

	uint8_t t_MaxMob;
	
	};
	
	
#endif /* CANHW_H_ */