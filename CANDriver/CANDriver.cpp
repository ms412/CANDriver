/*
 * CANDriver.cpp
 *
 * Created: 05.07.2013 22:44:33
 *  Author: Administrator
 */ 


#include <avr/io.h>
#include "driver/CanHw.hpp"

int main(void)
{
	CanPacket MOb;
	uint8_t counter = 0;
	
	CanDriver t_CAN;
	
	t_CAN.CanInit(CAN_125K);
	
	t_CAN.IOControl(0,RX_DATA,0x00000200,0x0000020F);
	t_CAN.IOControl(1,RX_DATA,0x00001200,0x0000120F);
	t_CAN.IOControl(7,TX_DATA,0x00002200,0x0000220F);
	
    while(1)
    {
		
		MOb.address=0x000000AA1;
		MOb.MObID = 7;
		MOb.length = 8;
		MOb.data[0]= 0x00;
		MOb.data[1]= 0x02;
		MOb.data[2]= 0x03;
		MOb.data[3]= 0x04;
		MOb.data[4]= 0x05;
		MOb.data[5]= 0x06;
		MOb.data[6]= 0x07;
		MOb.data[7]= counter;
		counter++;
		
		t_CAN.Send(&MOb);

        //TODO:: Please write your application code 
    }
}