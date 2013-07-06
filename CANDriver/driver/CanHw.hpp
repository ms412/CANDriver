/*
 * CanHw.hpp
 *
 * Created: 05.07.2013 22:48:29
 *  Author: Administrator
 */ 


#ifndef CANHW_H_
#define CANHW_H_

#include <avr/io.h>
#include "utils/fifo.hpp"


 typedef struct{
	  uint32_t address;
	  uint8_t MObID;
	  uint8_t length;
	  uint8_t data[8];
 }CanPacket;

  enum Baudrate {CAN_50K, CAN_100K, CAN_125K, CAN_200K, CAN_250K, CAN_500K, CAN_1000K };

  enum MObMode{DISABLED, TX_DATA, RX_DATA, TX_REMOTE, AUTO_REPLY};

class CanDriver 
{
public:
	CanDriver(){
		t_MaxMob = 15;
		static CBuffer<struct MOb_struct,5> TxBuffer;
		static CBuffer<struct MOb_struct,5> RxBuffer;
	}
	
	bool CanInit(uint8_t Baudrate);
	bool IOControl(uint8_t MObId,  uint8_t MObMode, uint32_t CANAddrLow, uint32_t CANAddrHigh);
	bool Receive(CanPacket *MOb);
	bool Send(CanPacket *MOb);
	
private:
	bool CanSpeed(uint8_t Baudrate);
		      
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
	
	void CanPageSet(uint8_t MObId){
		CANPAGE = (MObId << 4);
	}
	uint8_t CanPageGet(void){
		return (CANPAGE >> 4);
	}
	
	bool CanMaskExtSet(uint32_t maskId){
		CANIDM1 = (((*((uint8_t *)(&maskId)+3))<<3)+((*((uint8_t *)(&maskId)+2))>>5));
		CANIDM2 = (((*((uint8_t *)(&maskId)+2))<<3)+((*((uint8_t *)(&maskId)+1))>>5));
		CANIDM3 = (((*((uint8_t *)(&maskId)+1))<<3)+((* (uint8_t *)(&maskId)   )>>5));
		CANIDM4 = ((*  (uint8_t *)(&maskId)   )<<3);
		CANIDM4 = (1<<IDEMSK);
		return true;
	}
	
	bool CanAddrExtSet(uint32_t maskId){
		CANIDT1 = (((*((uint8_t *)(&maskId)+3))<<3)+((*((uint8_t *)(&maskId)+2))>>5));
		CANIDT2 = (((*((uint8_t *)(&maskId)+2))<<3)+((*((uint8_t *)(&maskId)+1))>>5));
		CANIDT3 = (((*((uint8_t *)(&maskId)+1))<<3)+((* (uint8_t *)(&maskId)   )>>5));
		CANIDT4 = ((*  (uint8_t *)(&maskId)   )<<3);
		CANCDMOB |= (1<<IDE);
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
	}
	
	bool MObEnableRx(void){
		CANCDMOB |= (1<<CONMOB1);
		CANCDMOB &= (~(1<<CONMOB0));	
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