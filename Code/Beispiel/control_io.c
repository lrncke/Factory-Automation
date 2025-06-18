/*-------------------------------------------------------------------*
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Control_III                                        *
 *   module name: control_io.c                                       *
 *        author: Christian Sommerfeld                               *
 *          date: 2015-02-26                                         *
 *                                                                   *
 *  version: 1.0 first version                                       *
 *                                                                   *
 *                                                                   *                                                                *
 *-------------------------------------------------------------------*/

#include "control.h"
#include "control_io.h"

int read_bit (AASiProcessData idi, int slave_addr, int bit)
{
	int idi_byte_nr;
	int idi_nibble; //1 = high, 0= low
	char mask;

	//Funktion wird nur ausgeführt wenn Werte gültig sind
	if ( (slave_addr < 62) && (bit < 4 ) ){

		idi_byte_nr = slave_addr / 2;
		idi_nibble = slave_addr % 2;

		if ( idi_nibble ){
			mask = 0x10;
		}
		else {
			mask = 0x01;
		}

		mask = (mask << bit);

		if ( (idi[idi_byte_nr] & mask) == mask ){
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

void write_bit(AASiProcessData odi, int slave_addr, int bit, int value)
{
	int odi_byte_nr;
	int odi_nibble; //1 = high, 0= low
	char mask=0;

		if ( (slave_addr < 62) && (bit < 4 ) ){

			odi_byte_nr = slave_addr / 2;
			odi_nibble = slave_addr % 2;

			if ( odi_nibble ){
				mask = 0x10;
			}
			else {
				mask = 0x01;
			}

			mask = (mask << bit);

			if ( value ){
				odi[odi_byte_nr] |= mask;
			}
			else{
				odi[odi_byte_nr] &= ~mask;
			}
		}
}


int read_flag_bit (int byte, int bit)
{
	int mask = 0x01;

	//check if value are valid
	if (byte <= 255 && bit <= 7 ){
		mask = (mask << bit);
		if ( (cctrl_func.CCtrlFlags[byte] & mask) == mask){
			return 1;
		}
		else{
			return 0;
		}
	}
	else {
		return 0;
	}
}

void write_flag_bit (int byte, int bit, int value)
{
	int mask = 0x01;

	//check if value are valid
	if (byte <= 255 && bit <= 7 ){
		mask = (mask << bit);

		if ( value ){
			cctrl_func.CCtrlFlags[byte] |= mask;
		}
		else{
			cctrl_func.CCtrlFlags[byte] &= ~mask;
		}
	}
}

void read_ossd (unsigned char *ossd_data)
{
	AASiMbRequestType  req;
	AASiMbResponseType res;

	unsigned char i;

	do {
		req.Opcode = 0x4E;
		req.DataLen = 0x0A;
		req.Circuit = 0x00;
		req.Data[0] = 0x00;
		req.Data[1] = 0x00;
		req.Data[2] = 0x00;
		req.Data[3] = 0x0F;
		req.Data[4] = 0x01; //length of answer puffer
		req.Data[5] = 0x00;

		cctrl_func.AASiMailbox(0, req, -1, &res);

	} while (res.Result == 0xFF);

		//read answer part 1
		req.DataLen = 0x05;
		req.Opcode = 0x4F;
		req.Circuit = 0x00;
		req.Data[0] = 0x00;
		req.Data[1] = 0x00;
		req.Data[2] = 0x00;

		cctrl_func.AASiMailbox(0, req, -1, &res);

		for (i=0; i<32; i++)
		{
			ossd_data[i] = res.Data[4+i];
		}

		//read answer part 2
		req.DataLen = 0x05;
		req.Opcode = 0x4F;
		req.Circuit = 0x00;
		req.Data[0] = 0x00;
		req.Data[1] = 0x00;
		req.Data[2] = 0x22;

		cctrl_func.AASiMailbox(0, req, -1, &res);

		for (i=0; i<32; i++)
		{
			ossd_data[32+i] = res.Data[2+i];
		}
}



