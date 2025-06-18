/*-------------------------------------------------------------------*
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Control_III (enip)                                 *
 *   module name: control.h                                          *
 *        author: Christian Sommerfeld                               *
 *          date: 2017-04-04                                         *
 *                                                                   *
 *  version: 1.0 first version                                       *
 *           1.1 CCtrlDisplay added                                  *
 *           1.2 enet.h                                              *
 *               new ethernet functions added                        *
 *               CCtrlEnetSetipdata                                  *
 *               CCtrlEnetSocket                                     *
 *               CCtrlEnetSetsockopt                                 *
 *               CCtrlEnetBind                                       *
 *               CCtrlEnetListen                                     *
 *               CCtrlEnetAccept                                     *
 *               CCtrlEnetConnect                                    *
 *               CCtrlEnetSend                                       *
 *               CCtrlEnetSendto                                     *
 *               CCtrlEnetRecv                                       *
 *               CCtrlEnetRecvfrom                                   *
 *               CCtrlEnetSocketclose                                *
 *           1.3 new functions added                                 *
 *               function not supported                              *
 *               AASiReadPE                                          *
 *               AASiSetCPRL                                         *
 *               AASiSendParameterNonBlocking                        *
 *           1.4 GetSystime added                                    *
 *           1.5 CCtrlReadParameterAdditional                        *
 *               CCtrlWriteParameterAdditional                       *
 *           1.6 CCtrlFcntl                                          *
 *           1.7 ASi-5 API added                                     *
 *               BACnet API added                                    *
 *                                                                   *
 *-------------------------------------------------------------------*/

#ifndef _CCONTROL_H_
#define _CCONTROL_H_

#include "asi5.h"

/*-------------------------------------------------------------------*
 *  global definitions                                               *
 *-------------------------------------------------------------------*/

/*  Execution Control flags
 */
#define ASI_CONFIG_OK               0x0001			/* there is no configuration error */
#define ASI_LDS0                    0x0002			/* a slave with address zero is existing */
#define ASI_AUTO_ADDRESS_ASSIGN     0x0004			/* automatic programming is activated */
#define ASI_AUTO_ADDRESS_AVAILABLE  0x0008			/* automatic programming is avalible */
#define ASI_CONFIGURATION_ACTIVE    0x0010			/* the configuration mode is active */
#define ASI_NORMAL_OPERATION_ACTIVE 0x0020			/* the normal operation mode is active */
#define ASI_APF                     0x0040			/* ASI has power fail */
#define ASI_OFFLINE_READY           0x0080			/* the offline phase is active */
#define ASI_PERIPHERY_OK            0x0100			/* there is no peripheral fault */

/*  Fault detector Flags
 */
#define	ASI_MASK_24V_ERROR		    0x08          	/* 24V error (retundant) */
#define	ASI_EFLT			    	0x10          	/* earth fault */
#define	ASI_OVERVOLTAGE	    		0x20          	/* overvoltage */
#define	ASI_NOIS			    	0x40          	/* noise */
#define	ASI_DUPLICATE_ADR      		0x80			/* duplicate adr */

/*  Host Interface flags
 */
#define ASI_DATA_EXCHANGE_ACTIVE    0x01			/* bit reset: data exchange with the slaves is locked */
#define ASI_OFFLINE                 0x02			/* bit set: master is set to offline state */
#define ASI_AUTO_ADDRESS_ENABLE     0x04			/* bit set: automatic programming is allowed */

/* error codes for the "write extended ID code 1" function
 * error codes for the "address slave" function
 */
#define ASI_NOK 					0x00			/*	error						*/
#define ASI_OK  					0x01			/*	no error					*/
#define ASI_ERR_SND 				0x02 		    /*  Slave (adr1) Not Detected   */
#define ASI_ERR_SD0 				0x03   		  	/*  Slave 0 Detected            */
#define ASI_ERR_SD2 				0x04   		  	/*  Slave (adr2) Detected       */
#define ASI_ERR_DE  				0x05   		  	/*  Delete Error                */
#define ASI_ERR_SE  				0x06   		 	/*  Set Error                   */
#define ASI_ERR_AT  				0x07 			/*  Adress stored temporarily   */
#define ASI_ERR_APF 				0x08 			/*  AS-Interface power failure  */


/*  Bitcodes for ASiCtrlFlags
 */
#define CCTRL_FLAG_RUN         		0x01			/* bit set: start control program */
#define CCTRL_FLAG_RESET       		0x02			/* bit set: reset control program */
#define CCTRL_FLAG_AUTO_START  		0x08			/* bit set: automatic program start */
#define CCTRL_FLAG_BREAK_ENABLE  	0x40			/* bit set: Breakpoints enable */
#define CCTRL_FLAG_ACTIVE      		0x80			/* bit set: a program is running */

/*  C-Control Display
 */
#define CCTRL_DISP_MODE_TRADITIONAL	0x00
#define CCTRL_DISP_MODE_SPONTANEOUS	0x01
#define CCTRL_DISP_TYP_4LINES 0x00
#define CCTRL_DISP_TYP_BIGNUM 0x01

/*-------------------------------------------------------------------*
 *  type declarations                                                *
 *-------------------------------------------------------------------*/

typedef unsigned char    AASiProcessData [32];
typedef unsigned char    AASiCtrlAccODI  [32];
typedef unsigned char    AASiCtrlAccAODI [16];
typedef unsigned char    AASiSlaveList [8];
typedef unsigned char    AASiHiFlags;
typedef unsigned short   AASiEcFlags;
typedef unsigned char    AASiSlaveData;
typedef unsigned char    AASiSlaveAddr;
typedef unsigned short   AASiConfigData;
typedef unsigned char    AASiErrorCounters [64];
typedef signed short     AASi16BitData [4];

typedef struct 
{
    AASiConfigData PCD [64];
    AASiSlaveData PP [64];
} 
AASiConfig;

typedef struct
{
	unsigned char DataLen;
	unsigned char Opcode;
	unsigned char Circuit;
	unsigned char Data [36];
} 
AASiMbRequestType;

typedef struct
{
	unsigned char DataLen;
	unsigned char Opcode;
	unsigned char Result;
	unsigned char Data [36];
} 
AASiMbResponseType;

typedef struct
{
	unsigned char  show;
	unsigned char  type;
	unsigned short time;
	unsigned char  big [5 +1];
	unsigned char  lines [4][16 +1];
}
cctrl_disp_t;

typedef struct 
{
	/*****************************************************************
	 *
	 *  CCtrlFlags aka 'Merker'
	 *
	 */
	unsigned char CCtrlFlags[256];

	/*********************************************************************
	 *
	 *  error = AASiDataExchange (Circuit, ODI, IDI, &EcFlags);
	 *
	 *  description:
	 *		exchange of ASi Slave data and reading of the execution control flags.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		ODI: 32 bytes output data to the slaves (output data image).
	 *			Every slave uses one nibble of a byte. The format is as follows:
	 *			Byte  0, low  nibble: data of slave  0/0A
	 *			Byte  0, high nibble: data of slave  1/1A
	 *			...
	 *			Byte 15, low  nibble: data of slave 30/30A
	 *			Byte 15, high nibble: data of slave 31/31A
	 *			Byte 16, low  nibble: data of slave  0B
	 *			Byte 16, high nibble: data of slave  1B
	 *			...
	 *			Byte 31, low  nibble: data of slave 30B
	 *			Byte 31, high nibble: data of slave 31B
	 *
	 *	returns:
	 *		IDI: 32 bytes input data from the slave (input data image).
	 *			The format is the same is in ODI.
	 *
	 *		EcFlags: Execution control flags (2 bytes) of the AS-Interface Master.
	 */
	int (*AASiDataExchange) (unsigned char Circuit, AASiProcessData ODI, AASiProcessData IDI, AASiEcFlags *EcFlags);

	/*********************************************************************
	 *
	 *  error = AASiReadIDI (Circuit, IDI, First, Amount, &EcFlags);
	 *
	 *  description:
	 *		read the input data of specific slave and the execution control flags.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		First: Index of the first slave to get data from (0..63)
	 *		Amount: Number of slaves to get data from. (1..64)
	 *
	 *	returns:
	 *		IDI: 32 bytes input data of the slaves (input data image).
	 *			Every slave uses one nibble of a byte. Unused bytes are set to zero.
	 *			The format is as follows:
	 *			Byte 0, low  nibble: Slave 'First' data
	 *			Byte 0, high nibble: Slave 'First'+1 data
	 *			...
	 *
	 *		EcFlags: Execution control flags (2 bytes) of the ASi Master.
	 */
	int (*AASiReadIDI) (unsigned char Circuit, AASiProcessData IDI, AASiSlaveAddr First, unsigned char Amount, AASiEcFlags *EcFlags);

	/*****************************************************************
	 *
	 *  error = AASiWriteODI (Circuit, ODI, First, Amount);
	 *
	 *  description:
	 *		Writes output data to specific slaves.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		ODI: 32 bytes of output data (output data image).
	 *			Every slave uses one nibble of a byte.
	 *			The format is as follows:
	 *			Byte  0, low  nibble: Slave  'First' data
	 *			Byte  0, high nibble: Slave  'First'+1 data
	 *			...
	 *
	 *		First: Index of the first slave to send data to (0..63)
	 *
	 *		Amount: Number of slaves to send data to. (1..64)
	 *
	 *	returns:
	 */
	int (*AASiWriteODI) (unsigned char Circuit, AASiProcessData ODI, AASiSlaveAddr First, unsigned char Amount);

	/************************************************************
	 *
	 *  error = AASiReadODI (Circuit, ODI);
	 *
	 *  description:
	 *		Read back the actual output data from the AS-Interface master.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		ODI: 32 bytes output data to the slaves (output data image).
	 *			Every slave uses one nibble of a byte. The format is as follows:
	 *			Byte  0, low  nibble: data of slave  0/0A
	 *			Byte  0, high nibble: data of slave  1/1A
	 *			...
	 *			Byte 15, low  nibble: data of slave 30/30A
	 *			Byte 15, high nibble: data of slave 31/31A
	 *			Byte 16, low  nibble: data of slave  0B
	 *			Byte 16, high nibble: data of slave  1B
	 *			...
	 *			Byte 31, low  nibble: data of slave 30B
	 *			Byte 31, high nibble: data of slave 31B
	 *          
	 */
	int (*AASiReadODI) (unsigned char Circuit, AASiProcessData ODI);

	/**************************************************************
	 *
	 *  error = AASiWritePP (Circuit, Address, PP);
	 *
	 *  description:
	 *		Set the permanent parameters of one slave
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Address of slave to set the parameters for (1..31, 33..63).
	 *		PP:	Permanent parameters to set (low nibble).
	 *
	 *  returns:
	 */
	int (*AASiWritePP) (unsigned char Circuit, AASiSlaveAddr Address, AASiSlaveData PP);

	/***************************************************************
	 *
	 *  error = AASiReadPP (Circuit, Address, &PP);
	 *
	 *  description:
	 *		Read the permanent parameters of one slave
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Address of slave to get the parameters from (1..31, 33..63).
	 *
	 *  returns:
	 *		PP: Permanent parameters of the slave (low nibble)
	 */
	int (*AASiReadPP) (unsigned char Circuit, AASiSlaveAddr Address, AASiSlaveData *PP);
	                                                 
	/*********************************************************************
	 *
	 *  error = AASiSendParameter (Circuit, Address, PI, &Ret);
	 *
	 *  description:
	 *		Send parameters to one slave.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Address of slave to send parameters to (1..31, 33..63).
	 *		PI: Parameters to send (low nibble)
	 *
	 *	returns:
	 *		Ret: On error PI is returned inverted.
	 */
	int (*AASiSendParameter) (unsigned char Circuit, AASiSlaveAddr Address, AASiSlaveData PI, AASiSlaveData *Return);

	/*********************************************************************
	 *
	 *  error = AASiReadPI (Circuit, Address, &PI);
	 *
	 *  description:
	 *		Read the actual parameters of one slave
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Address of slave to read the parameters from (1..31, 33..63).
	 *
	 *	returns:
	 *		PI: Parameters of the slave. (low nibble).
	 */
	int (*AASiReadPI) (unsigned char Circuit, AASiSlaveAddr Address, AASiSlaveData *PI);

	/*********************************************************************
	 *
	 *  error = AASiStorePI (Circuit);
	 *
	 *  description:
	 *		Store the actual slave parameters as permanent parameters.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *	returns:
	 */
	int (*AASiStorePI) (unsigned char Circuit);

	/*************************************************************
	 *
	 *  error = AASiReadDuplicateAdrList (Circuit, Address, SingleTransfer, Request, Response);
	 *
	 *  description:
	 *		Read List of duplicate address
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		DpAdrList: List of duplicate address
	 *
	 *  returns:
	 *	
	 */
	int (*AASiReadDuplicateAdrList) (unsigned char Circuit, AASiSlaveList *DpAdrList);

	/*************************************************************
	 *
	 *  error = AASiReadFaultDetector (Circuit, &pucFaultDetectorActiv, &pucFaultDetectorHistoric);
	 *
	 *  description:
	 *		Read Overvoltage, Nois, EFLT, Duplicate adr.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		pucFaultDetectorActiv:    active fault detector
	 *		pucFaultDetectorHistoric: historic fault detector
	 *
	 *  returns:
	 *	
	 */
	int (*AASiReadFaultDetector) (unsigned char Circuit, unsigned char  *pucFaultDetectorActiv, unsigned char  *pucFaultDetectorHistoric);

	/*********************************************************************
	 *
	 *  error = AASiWritePCD (Circuit, Address, PCD);
	 *
	 *  description:
	 *		Write the projected configuration of one slave.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Address of the slave to write the configuration to (1..31, 33..63).
	 *		PCD:	 Configuration to write.
	 *	             Byte 0, low nibble: I/O configuration
	 *	             Byte 0, high nibble: slave ID
	 *	             Byte 1, low nibble: extended ID1
	 *	             Byte 1, high nibble: extended ID2
	 *
	 *  returns:
	 */
	int (*AASiWritePCD) (unsigned char Circuit, AASiSlaveAddr Address, AASiConfigData PCD);

	/************************************************************************
	 *
	 *  error = AASiReadPCD (Circuit, Address, &PCD);
	 *
	 *  description:
	 *		read the projected configuration of one slave.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Address of slave to read the configuration from (1..31, 33..63).
	 *
	 *  returns:
	 *		PCD: Configuration of the slave
	 *	             Byte 0, low nibble: I/O configuration
	 *	             Byte 0, high nibble: slave ID
	 *	             Byte 1, low nibble: extended ID1
	 *	             Byte 1, high nibble: extended ID2
	 */
	int (*AASiReadPCD) (unsigned char Circuit, AASiSlaveAddr Address, AASiConfigData *PCD);

	/*************************************************************************
	 *
	 *  error = AASiStoreCDI (Circuit);
	 *
	 *  description:
	 *		Store the actual configuration as permanent configuration.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 */
	int (*AASiStoreCDI) (unsigned char Circuit);

	/***********************************************************************
	 *
	 *  error = AASiReadCDI (Circuit, Address, &CDI);
	 *
	 *  description:
	 *		Read the actual configuration of one slave.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Address of slave to get the configuration from (0..31, 33..63).
	 *
	 *  returns:
	 *		CDI: Configuration of the slave
	 *	             Byte 0, low nibble: I/O configuration
	 *	             Byte 0, high nibble: slave ID
	 *	             Byte 1, low nibble: extended ID1
	 *	             Byte 1, high nibble: extended ID2
	 */
	int (*AASiReadCDI) (unsigned char Circuit, AASiSlaveAddr Address, AASiConfigData *CDI);

	/*********************************************************************
	 *
	 *  error = AASiWriteExtID1 (Circuit, ID1);
	 *
	 *  description:
	 *		Write the extended ID code 1 of slave 0
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		ID1: extended ID code 1
	 *
	 *  returns:
	 *		This function returns special error codes reporting
	 *		the reason of operation failure.
	 */
	int (*AASiWriteExtID1) (unsigned char Circuit, AASiSlaveData ID1);

	/*****************************************************************
	 *
	 *  error = AASiWriteLPS (Circuit, LPS);
	 *
	 *  description:
	 *		Write list of projected slave.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		LPS: 8 bytes projected slave list.
	 *			Every bit in the LPS corresponds to one slave after the following scheme:
	 *			Bit 0:	Slave 0/0A - can not be set!
	 *			Bit 1:	Slave 1/1A
	 *			...
	 *			Bit31:	Slave 31/31A
	 *			Bit32 :	Slave 0B - can not be set!
	 *			Bit33:	Slave 1B
	 *			...
	 *			Bit63:	Slave 31B
	 *			The slave is projected if the bit is set.
	 * 
	 * 	returns:
	 */
	int (*AASiWriteLPS) (unsigned char Circuit, AASiSlaveList LPS);
	                            
	/*********************************************************************
	 *
	 *  error = AASiReadLPS (Circuit, &LPS);
	 *
	 *  description:
	 *		Read list of projected slaves.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *	returns:
	 *		LPS: 8 bytes projected slave list.
	 *			Bit 0:	Slave 0/0A - can not be set!
	 *			Bit 1:	Slave 1/1A
	 *			...
	 *			Bit31:	Slave 31/31A
	 *			Bit32 :	Slave 0B - can not be set!
	 *			Bit33:	Slave 1B
	 *			...
	 *			Bit63:	Slave 31B
	 *			The slave is projected if the bit is set.
	 */
	int (*AASiReadLPS) (unsigned char Circuit, AASiSlaveList *LPS);
	                            
	/*****************************************************************
	 *
	 *  error = AASiReadLAS (Circuit, &LAS);
	 *
	 *  description:
	 *		Read list of activated slaves.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		LAS: 8 bytes activated slave list.
	 *			Bit 0:	Slave 0/0A
	 *			Bit 1:	Slave 1/1A
	 *			...
	 *			Bit31:	Slave 31/31A
	 *			Bit32 :	Slave 0B
	 *			Bit33:	Slave 1B
	 *			...
	 *			Bit63:	Slave 31B
	 *			The slave is activated if the bit is set.
	 */
	int (*AASiReadLAS) (unsigned char Circuit, AASiSlaveList *LAS);
	                            
	/****************************************************************
	 *
	 *  error = AASiReadLDS (Circuit, &LDS);
	 *
	 *  description:
	 *		Read list of detected slaves.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		LDS: 8 bytes detected slave list.
	 *			Every bit in the LDS corresponds to one slave after the following scheme:
	 *			Bit 0:	Slave 0/0A
	 *			Bit 1:	Slave 1/1A
	 *			...
	 *			Bit31:	Slave 31/31A
	 *			Bit32 :	Slave 0B
	 *			Bit33:	Slave 1B
	 *			...
	 *			Bit63:	Slave 31B
	 *			The slave is detected if the bit is set.
	 */
	int (*AASiReadLDS) (unsigned char Circuit, AASiSlaveList *LDS);
	                            
	/****************************************************************
	 *
	 *  error = AASiReadLCS (Circuit, &LCS);
	 *
	 *  description:
	 *		Read list of corrupted slaves. Every read resets the list.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		LCS: 8 bytes corrupted slave list.
	 *			Every bit in the LCS corresponds to one slave after the following scheme:
	 *			Bit 0:	Slave 0/0A can not be corrupted, AS-Interface power fail indication instead
	 *			Bit 1:	Slave 1/1A
	 *			...
	 *			Bit31:	Slave 31/31A
	 *			Bit32 :	Slave 0B can not be corrupted.
	 *			Bit33:	Slave 1B
	 *			...
	 *			Bit63:	Slave 31B
	 *			The slave is corrupted if the bit is set.
	 */
	int (*AASiReadLCS) (unsigned char Circuit, AASiSlaveList *LCS);
	                            
	/*****************************************************************
	 *
	 *  error = AASiWriteLOS (Circuit, LOS);
	 *
	 *  description:
	 *		Write list of slave addresses which trigger offline on config error.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		LOS: 8 bytes slave addresses list.
	 *			Every bit in the LOS corresponds to one slave after the following scheme:
	 *			Bit 0:	Slave 0/0A
	 *			Bit 1:	Slave 1/1A
	 *			...
	 *			Bit31:	Slave 31/31A
	 *			Bit32 :	Slave 0B
	 *			Bit33:	Slave 1B
	 *			...
	 *			Bit63:	Slave 31B
	 *			The slave is projected if the bit is set.
	 * 
	 * 	returns:
	 */
	int (*AASiWriteLOS) (unsigned char Circuit, AASiSlaveList LOS);
	                            
	/****************************************************************
	 *
	 *  error = AASiReadLOS (Circuit, &LOS);
	 *
	 *  description:
	 *		Write list of slave addresses which trigger offline on config error.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		LOS: 8 bytes slave addresses list.
	 *			Every bit in the LOS corresponds to one slave after the following scheme:
	 *			Bit 0:	Slave 0/0A
	 *			Bit 1:	Slave 1/1A
	 *			...
	 *			Bit31:	Slave 31/31A
	 *			Bit32 :	Slave 0B
	 *			Bit33:	Slave 1B
	 *			...
	 *			Bit63:	Slave 31B
	 *			The slave is detected if the bit is set.
	 */
	int (*AASiReadLOS) (unsigned char Circuit, AASiSlaveList *LOS);
	                            
	/****************************************************************
	 *
	 *  error = AASiReadLPF (Circuit, &LPF);
	 *
	 *  description:
	 *		Read list of slave addresses with peripheral fault.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		LPF: 8 bytes slave addresses list.
	 *			Every bit in the LOS corresponds to one slave after the following scheme:
	 *			Bit 0:	Slave 0/0A
	 *			Bit 1:	Slave 1/1A
	 *			...
	 *			Bit31:	Slave 31/31A
	 *			Bit32:	Slave 0B
	 *			...
	 *			Bit63:	Slave 31B
	 *			The slave has a peripheral fault if the bit is set.
	 */
	int (*AASiReadLPF) (unsigned char Circuit, AASiSlaveList *LPF);
	                            
	/**********************************************************************
	 *
	 *  error = AASiReadEcFlags (Circuit, &EcFlags);
	 *
	 *  description:
	 *		Read execution control flags.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		EcFlags: Two bytes execution control flags.
	 */
	int (*AASiReadEcFlags) (unsigned char Circuit, AASiEcFlags *EcFlags);
	                            
	/********************************************************************
	 *
	 *  error = AASiSetConfigMode (Circuit, Mode);
	 *
	 *  description:
	 *		Set ASi Master to configuration or protected operation mode.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Mode: 1 = Configuration mode, 0 = Protected operation mode.
	 *
	 *  returns:
	 */
	int (*AASiSetConfigMode) (unsigned char Circuit, unsigned char Mode);

	/*******************************************************************
	 *
	 *  error = AASiWriteHiFlags (Circuit, HiFlags);			
	 *
	 *  description:
	 *		Write host interface flags to the ASi Master.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		HiFlags: Host interface flags to write.
	 *
	 *  returns:
	 */
	int (*AASiWriteHiFlags) (unsigned char Circuit, AASiHiFlags HiFlags);

	/*******************************************************************
	 *
	 *  error = AASiReadHiFlags (Circuit, &HiFlags);
	 *
	 *  description:
	 *		Read acatual host interface flags from the master.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		HiFlags: host interface flags (one byte).
	 */
	int (*AASiReadHiFlags) (unsigned char Circuit, AASiHiFlags *HiFlags);
	                            
	/****************************************************************
	 *
	 *  error = AASiAddressSlave (Circuit, Addr1, Addr2);
	 *
	 *  description:
	 *		Change the address of one slave.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Addr1: Old slave address.
	 *		Addr2: New slave address.
	 *
	 *  returns:
	 *		This function returns special error codes reporting
	 *		the reason of operation failure.
	 */
	int (*AASiAddressSlave) (unsigned char Circuit, AASiSlaveAddr OldAddress, AASiSlaveAddr NewAddress);
	                            
	/*************************************************************
	 *
	 *  error = AASiExecuteCommand (Circuit, Address, Request, &Response);
	 *
	 *  description:
	 *		Send directly an AS-Interface command.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Slave address to send command to.
	 *		Request: AS-Interface request command.
	 *
	 *  returns:
	 *		Response: AS-Interface response.
	 */
	int (*AASiExecuteCommand) (unsigned char Circuit, AASiSlaveAddr Address, AASiSlaveData Request, AASiSlaveData *Response);

	/*********************************************************************
	 *
	 *  error = AASiReadAllConfig (Circuit, Configurations);
	 *
	 *  description:
	 *		Read all the configuration data (i.e. LPS, PP[] and PCD[]) for 
	 *		all AS-Interface slaves of all AS-Interface circuits of the connected master.
	 *
	 *  parameters:
	 *		Circuit:        AS-i Master circuit
	 *		Configurations:	vector to hold the configuration data.
	 *
	 *	returns:
	 *		the configuration data in Configurations[].
	 */
	int (*AASiReadAllConfig) (unsigned char Circuit, AASiConfig *Configurations );

	/*********************************************************************
	 *
	 *  error = AASiWriteAllConfig (Circuit, Configurations);
	 *
	 *  description:
	 *		Write all the configuration data (i.e. LPS, PP[] and PCD[]) for
	 *		all AS-Interface slaves of all AS-Interface circuits of the connected master.
	 *
	 *  parameters:
	 *		Circuit:        AS-i Master circuit
	 *		Configurations:	vector holding the configuration data to be written.
	 *
	 *	returns:
	 */
	int (*AASiWriteAllConfig) (unsigned char Circuit, AASiConfig Configurations);

	/*********************************************************************
	 *
	 *  error = AASiReadErrorCounters(Circuit, Counters)
	 *
	 *  description:
	 *		Read the slave error counters. Every read resets the counters.
	 *
	 *  parameters:
	 *		Circuit:        AS-i Master circuit
	 *
	 *	returns:
	 *		Counters: 64 bytes holding one error counter per slave.
	 */
	int (*AASiReadErrorCounters) (unsigned char Circuit, AASiErrorCounters Counters);

	/*************************************************************
	 *
	 *  error = AASiMailbox (Circuit, Request, Response);
	 *
	 *  description:
	 *		Generic mailbox function
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Request: structure for mailbox request
	 *      ExpResLen: expected response length (-1 == don't know)
	 *
	 *  returns:
	 *		Response: structure for mailbox response
	 */ 
	int (*AASiMailbox) (unsigned char Circuit, AASiMbRequestType Request, int ExpResLen, AASiMbResponseType *Response);

	/*********************************************************************
	 *
	 *  error = AASiWrite16BitODI (Circuit, Address, Out);
	 *
	 *  description:
	 *      writes four channels of 16bit ODI to an AS-Interface slave
	 *      with e.g. analog slave profil 7.3 or 7.4
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Slave address (1..31)
	 *      Out: 4 channels of 16-Bit values
	 *           Word 0: channel 1
	 *           ...
	 *           Word 3: channel 4
	 *
	 *	returns:
	 */
	int (*AASiWrite16BitODI) (unsigned char Circuit, AASiSlaveAddr Address, AASi16BitData Out);

	/*********************************************************************
	 *
	 *  error = AASiRead16BitODI (Circuit, Address, In);
	 *
	 *  description:
	 *      reads four channels of 16bit data from an AS-Interface slave
	 *      with e.g. analog slave profil 7.3 or 7.4
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Slave address (1..31)
	 *
	 *	returns:
	 *      In:  4 channels of 16-Bit values
	 *           Word 0: channel 1
	 *           ...
	 *           Word 3: channel 4
	 */
	int (*AASiRead16BitODI) (unsigned char Circuit, AASiSlaveAddr Address, AASi16BitData In);

	/*********************************************************************
	 *
	 *  error = AASiRead16BitIDI (Circuit, Address, In);
	 *
	 *  description:
	 *      reads four channels of 16bit data from an AS-Interface slave
	 *      with e.g. analog slave profil 7.3 or 7.4
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Slave address (1..31)
	 *
	 *	returns:
	 *      In:  4 channels of 16-Bit values
	 *           Word 0: channel 1
	 *           ...
	 *           Word 3: channel 4
	 */
	int (*AASiRead16BitIDI) (unsigned char Circuit, unsigned char Address, AASi16BitData In);

	/************************************************************
	 *
	 *  error = AASiReadCtrlAccODI (Circuit, ODI);
	 *
	 *  description:
	 *		Read back the actual Ctrl access data for the Ctrl output data image.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		ODI: 32 bytes Ctrl access data.
	 *			Every slave uses one nibble of a byte. The format is as follows:
	 *			Byte  0, low  nibble: Ctrl access data of slave  0/0A
	 *			Byte  0, high nibble: Ctrl access data of slave  1/1A
	 *			...
	 *			Byte 15, low  nibble: Ctrl access data of slave 30/30A
	 *			Byte 15, high nibble: Ctrl access data of slave 31/31A
	 *			Byte 16, low  nibble: Ctrl access data of slave  0B
	 *			Byte 16, high nibble: Ctrl access data of slave  1B
	 *			...
	 *			Byte 31, low  nibble: Ctrl access data of slave 30B
	 *			Byte 31, high nibble: Ctrl access data of slave 31B
	 *          
	 */
	int  (*AASiReadCtrlAccODI)	( unsigned char Circuit, AASiCtrlAccODI ODI );

	/*****************************************************************
	 *
	 *  error = AASiWriteCtrlAccODI (Circuit, ODI, First, Amount);
	 *
	 *  description:
	 *		Writes Ctrl access data for the Ctrl output data image.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		ODI: 32 bytes of Ctrl access output data.
	 *			Every slave uses one nibble of a byte.
	 *			The format is as follows:
	 *			Byte  0, low  nibble: Slave  'First'   Ctrl access data
	 *			Byte  0, high nibble: Slave  'First'+1 Ctrl access data
	 *			...
	 *
	 *		First: Index of the first slave for Ctrl access data (0..63)
	 *
	 *		Amount: Number of slaves for Ctrl access data. (1..64)
	 *
	 *	returns:
	 */
	int  (*AASiWriteCtrlAccODI)	( unsigned char Circuit, AASiCtrlAccODI ODI, AASiSlaveAddr First, unsigned char Amount );

	/************************************************************
	 *
	 *  error = AASiReadCtrlAccAODI (Circuit, AODI);
	 *
	 *  description:
	 *		Read back the actual Ctrl access analogue data for the Ctrl analogue output data image.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *
	 *  returns:
	 *		Circuit: AS-i Master circuit
	 *		AODI: 16 bytes of Ctrl access output data.
	 *			Every slave uses one nibble of a byte.
	 *			Every analogue channel of each slave use one bit.
	 *			The format is as follows:
	 *			Byte  0, Bit 0: Slave 0   Ctrl access analogue channel 0
	 *			Byte  0, Bit 1: Slave 0   Ctrl access analogue channel 1
	 *			Byte  0, Bit 2: Slave 0   Ctrl access analogue channel 2
	 *			Byte  0, Bit 3: Slave 0   Ctrl access analogue channel 3
	 *			Byte  0, Bit 4: Slave 1   Ctrl access analogue channel 0
	 *			Byte  0, Bit 5: Slave 1   Ctrl access analogue channel 1
	 *			Byte  0, Bit 6: Slave 1   Ctrl access analogue channel 2
	 *			Byte  0, Bit 7: Slave 1   Ctrl access analogue channel 3
	 *			...
	 *          
	 */
	int  (*AASiReadCtrlAccAODI)	( unsigned char Circuit, AASiCtrlAccAODI AODI );

	/*****************************************************************
	 *
	 *  error = AASiWriteCtrlAccAODI (Circuit, AODI, First, Amount);
	 *
	 *  description:
	 *		Writes Ctrl access analogue data for the Ctrl analogue output data image.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		AODI: 16 bytes of Ctrl access output data.
	 *			Every slave uses one nibble of a byte.
	 *			Every analogue channel of each slave use one bit.
	 *			The format is as follows:
	 *			Byte  0, Bit 0: Slave  'First'   Ctrl access analogue channel 0
	 *			Byte  0, Bit 1: Slave  'First'   Ctrl access analogue channel 1
	 *			Byte  0, Bit 2: Slave  'First'   Ctrl access analogue channel 2
	 *			Byte  0, Bit 3: Slave  'First'   Ctrl access analogue channel 3
	 *			Byte  0, Bit 4: Slave  'First'+1 Ctrl access analogue channel 0
	 *			Byte  0, Bit 5: Slave  'First'+1 Ctrl access analogue channel 1
	 *			Byte  0, Bit 6: Slave  'First'+1 Ctrl access analogue channel 2
	 *			Byte  0, Bit 7: Slave  'First'+1 Ctrl access analogue channel 3
	 *			...
	 *
	 *		First: Index of the first slave to send data to (0..31)
	 *
	 *		Amount: Number of slaves to send data to. (1..32)
	 *
	 *	returns:
	 */
	int  (*AASiWriteCtrlAccAODI)( unsigned char Circuit, AASiCtrlAccAODI AODI, AASiSlaveAddr First, unsigned char Amount );

	/*****************************************************************
	 *
	 *  error = CCtrlInitTimer (ticks_ms, *timer_func() );
	 *
	 *  description:
	 *		Setup Timer Interrupt Function 
	 *
	 *  parameters:
	 *		ticks_ms: timer isr periode	in ms
	 * 		timer_func: callback of the timer isr
	 *
	 *	returns:
	 */
	int  (*CCtrlInitTimer) 		( unsigned long ticks_ms, void (*timer_func)( void ) );

	/*****************************************************************
	 *
	 *  error = CCtrlDelay ( ticks_ms );
	 *
	 *  description:
	 *		Setup a delay in ms
	 *
	 *  parameters:
	 *		ticks_ms: Delay in ms
	 *
	 *	returns:
	 */
	int  (*CCtrlDelay) 			( unsigned long ticks_ms );

	/*****************************************************************
	 *
	 *  error = CCtrlInitWdg ( ticks );
	 *
	 *  description:
	 *		Initialise Control Watchdog
	 *
	 *  parameters:
	 *		ticks: Watchdog Time in 10 ms
	 *
	 *	returns:
	 */
	int  (*CCtrlInitWdg)		( unsigned long ticks );

	/*****************************************************************
	 *
	 *  error = CCtrlTriggerWdg ();
	 *
	 *  description:
	 *		Trigger Control Watchdog
	 *
	 *	returns:
	 */
	int  (*CCtrlTriggerWdg)		( void );

	/*****************************************************************
	 *
	 *  error = CCtrlEvalCycletime ();
	 *
	 *  description:		
	 *		Must be called in the Main loop to evaluate the cycletime of the Control Program
	 *
	 *	returns:
	 */
	int  (*CCtrlEvalCycletime)	( void );

	/*****************************************************************
	 *
	 *  error = CCtrlReadParameter ( *buffer, len, adr );
	 *
	 *  description:
	 *		Read Control NV Parameter
	 *
	 *  parameters:
	 *		len: length of the buffer
	 * 		adr: address of the first byte to read data from
	 *
	 *	returns:
	 *		buffer: read buffer
	 */
	int  (*CCtrlReadParameter)	( unsigned char *buffer, unsigned short len, unsigned short adr );

	/*****************************************************************
	 *
	 *  error = CCtrlWriteParameter ( *buffer, len, adr );
	 *
	 *  description:
	 *		Write Control NV Parameter
	 *
	 *  parameters:
	 *		buffer: write buffer
	 *		len: length of the buffer
	 * 		adr: address of the first byte to write data to
	 *
	 *	returns:
	 */
	int  (*CCtrlWriteParameter)	( unsigned char *buffer, unsigned short len, unsigned short adr );

	/*****************************************************************
	 *
	 *  error = CCtrlReadFlags ( *flags );
	 *
	 *  description:
	 *		Read Control Flags
	 *
	 *  parameters:
	 *
	 *	returns:
	 *  	flags: AS-i Control flags
	 */
	int  (*CCtrlReadFlags)		( unsigned char *flags );

	/*****************************************************************
	 *
	 *  error = CCtrlWriteFlags ( flags );
	 *
	 *  description:
	 *		Read Control Flags
	 *
	 *  parameters:
	 *  	flags: AS-i Control flags
	 *
	 *	returns:
	 */
	int  (*CCtrlWriteFlags)		( unsigned char  flags );

	/*****************************************************************
	 *
	 *  error = CCtrlWriteFlags ( flags );
	 *
	 *  description:
	 *		Read user defined unique control key
	 *
	 *  parameters:
	 *
	 *	returns:
	 *  	key: control key
	 */
	int  (*CCtrlReadKey)		( unsigned int *key );

	/*****************************************************************
	 *
	 *  error = CCtrlPrintf ( format, ... );
	 *
	 *  description:
	 *		Well known Printf Function
	 *
	 *  parameters:
	 *	returns:
	 */
	int  (*CCtrlPrintf)			( const char *format, ... );

	/*****************************************************************
	 *
	 *  error = CCtrlBreakpoint ();
	 *
	 *  description:
	 *		Function to hit the first breakpoint
	 *
	 *  parameters:
	 *	returns:
	 */
	void (*CCtrlBreakpoint) 	(void);

	/*****************************************************************
	 *
	 *  error = CCtrlDisplay ();
	 *
	 *  description:
	 *		Function to print text on Display
     *
	 *  parameters:
	 *
     *     mode == CCTRL_DISP_MODE_TRADITIONAL
     *	      disp_buffer.show:  0 = clear printed msg, 
     *                           1 = print msg
	 *        disp_buffer.type:  CCTRL_DISP_TYP_4LINES = print 4 lines of text, 
	 *                           CCTRL_DISP_TYP_BIGNUM = print big numbers and 1 line of text
	 *        disp_buffer.time:  time in ms the msg is printed in traditional mode ( min. 2000ms )
	 *        disp_buffer.big:   buffer to hold the big numbers
	 *        disp_buffer.lines: buffer to hold the 4 lines of text
     *
     *     mode == CCTRL_DISP_MODE_SPONTANEOUS
     *	      disp_buffer.show:  0 = clear printed msg, 
     *                           1 = print msg
	 *        disp_buffer.type:  CCTRL_DISP_TYP_4LINES = print 4 lines of text, 
	 *                           CCTRL_DISP_TYP_BIGNUM = print big numbers and 1 line of text
	 *        disp_buffer.time:  time in ms the msg is printed in traditional mode ( min. 2000ms )
	 *        disp_buffer.big:   buffer to hold the big numbers
	 *        disp_buffer.lines: buffer to hold the 4 lines of text
     *
	 *	returns:
	 *        0  if ok
	 *        !0 if not ok
	 */
	int (*CCtrlDisplay)				( unsigned char mode, cctrl_disp_t disp_buffer );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetSocket ( address_family, port, ip, buffer);
	 *
	 *  description:
	 *		Function to set ip stack information in the right format
	 *
	 *  parameters:
	 *  	address_family 	-> enet.h ( adress families )
	 *  						Internet IPV4: AF_INET
	 *  						Internet IPV6: AF_INET6
	 *  	port			port number
	 *		ip				ip address string
	 *		buffer			buffer to hold the following information
	 *							u_char		sa_len;		 	total length
	 *							u_int		sa_family;	 	address family
	 *							char		sa_data[30];	actually longer; address value
	 *										-> [0] - [1] port
	 *										-> [2] - [5] ip adress
	 *
	 *
	 *	returns:
	 *		0 if ok
	 *		1 if error
	 */
	int (*CCtrlEnetSetipdata) 	( unsigned char address_family, unsigned short port, char *ip, unsigned char *buffer );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetSocket ( address_family, types, protocol);
	 *
	 *  description:
	 *		Function to open a socket (max 3)
	 *		Forbidden Ports ->
	 *				port == 80     /HTTP/
	 *	 			port == 44818  /ENIP_C01_PORT offically assigned as 'rockwell-encap/
	 *	 			port == 2222   /ENIP_PORT officially assigned as 'unreg-ab2'/
	 *	 			port == 502    /TCP listener port/
	 *	 			port == 546    /DHCPv6 Client/
	 *	 			port == 547    /DHCPv6 Server/
	 *	 			port == 57     /UDP 57 already used/
	 *	 			port == 87     /UDP 87 already used/
	 *	 			port == 67     /Bootstrap Protocol (BOOTP) Client; auch genutzt von DHCP/
	 *	 			port == 68     /Bootstrap Protocol (BOOTP) Client; auch genutzt von DHCP/
	 *	 			port == 69     /TFTP Port/
	 *
	 *  parameters:
	 *  	address_family 	-> enet.h ( adress families )
	 *  						Internet IPV4: AF_INET
	 *  						Internet IPV6: AF_INET6
	 *  	types			-> enet.h
	 *							TCP: SOCK_STREAM
	 *							UDP: SOCK_DGRAM
	 *							Raw protocol at network layer: SOCK_RAW
	 *  	protocol		-> enet.h
	 *  						Internet Protocol (IP): 0 or IPPROTO_IP
	 *							ICMP: 1

	 *	returns:
	 *		>=0 = socket number
	 *		-1 = error
	 */
	int (*CCtrlEnetSocket) 	( unsigned char address_family, unsigned char types, int protocol );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetSetsockopt ( sock, level, optname, optval, len);
	 *
	 *  description:
	 *		Function to set the options of a socket
	 *
	 *  parameters:
	 *  	sock  			the socket to be set
	 *  	level 			SOL_SOCKET
	 *  	optval			-> enet.h ( Option flags per-socket / Additional options, not kept in so_options )
	 *  	len 			sizeof(optval)
	 *
	 *	returns:
	 *      0  if ok
	 *      -1 failed
	 *      1  no control III socket
	 */
	int (*CCtrlEnetSetsockopt) ( int sock, int level, int optname, const void *optval, int len );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetBind ( sock, address_family, port, address);
	 *
	 *  description:
	 *		Bind the socket to a local socket address. This assigns a name to the socket
	 *
	 *  parameters:
	 *  	sock  			Socket file descriptor. Returned by call to "socket".
	 *  	address_family	-> enet.h ( adress families )
	 *  	port			port
	 *  	adress 			selected adress
	 *
	 *	returns:
	 *      0  if ok
	 *      1  forbidden port
	 *      2  bind failure
	 *      3  no control III port
	 */
	int (*CCtrlEnetBind) ( int sock, unsigned char address_family, unsigned short port, unsigned int address );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetListen ( sock, backlog);
	 *
	 *  description:
	 *		The function shall mark a connection-mode socket, specified by the socket argument, as accepting connections
	 *
	 *  parameters:
	 *  	sock  		the socket which should be bind
	 *  	backlog		number of buffered requests
	 *
	 *	returns:
	 *      0  if ok
	 *      -1 if no control III port or listen failed
	 */
	int (*CCtrlEnetListen)	( int sock, int backlog );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetAccept ( sock, addr, len);
	 *
	 *  description:
	 *		function shall extract the first connection on the queue of pending
	 *		function connections, create a new socket with the same socket type protocol
	 *		function and address family as the specified socket, and allocate a new file
	 *		function descriptor for that socket.
	 *
	 *  parameters:
	 *  	sock  			the socket which should be bind
	 *  	*addr			points on struct sockaddr
	 *  	len				sizeof struct sockaddr
	 *
	 *	returns:
	 *      0  if ok
	 *      -1 if accept failure
	 *      1  if no control III socket
	 */
	int (*CCtrlEnetAccept) ( int sock, const void *addr, const void *len );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetConnect ( sock, address);
	 *
	 *  description:
	 *		The connect() function shall attempt to make a connection on a socket.
	 *
	 *  parameters:
	 *  	sock  			the socket which should be bind
	 *  	address     	points on struct sockaddr returned from SetIpData
	 *
	 *	returns:
	 *      0  if ok
	 *      -1 if connection failure
	 */
	int (*CCtrlEnetConnect) ( int sock, unsigned char *address );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetSend ( sock, buffer, len, flag);
	 *
	 *  description:
	 *		send a packet
	 *
	 *  parameters:
	 *  	sock  			the socket to send data to
	 *  	buffer	 		send buffer
	 *  	len				length of the buffer
	 *  	flag			Specifies the type of message reception.
	 *  					Values of this argument are formed by logically
	 *  					OR'ing zero or more of the following values
	 *  					-> enet.h
	 *
	 *	returns:
	 *      number of bytes sent
	 *      -1 if no control III socket or error
	 *
	 */
	int (*CCtrlEnetSend) ( int sock, unsigned char *buffer, int len, int flag );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetSend ( sock, address, buffer, len);
	 *
	 *  description:
	 *		send a packet
	 *
	 *  parameters:
	 *  	sock  			the socket to send data
	 *		address			stack information from EnetSetipdata();
	 *  	buffer: 		write buffer
	 *  	len				length of the buffer
	 *
	 *	returns:
	 *      number of bytes sent
	 *      -1 if no control III socket or error
	 */
	int (*CCtrlEnetSendto) ( int sock, unsigned char *address, unsigned char *buffer, int len );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetRecv ( sock, buffer, len, flag);
	 *
	 *  description:
	 *		receive a packet
	 *
	 *  parameters:
	 *  	sock  			the socket to receive data
	 *  	buffer	 		receive buffer
	 *  	len				length of the buffer
	 *  	flag			Specifies the type of message reception.
	 *  					Values of this argument are formed by logically
	 *  					OR'ing zero or more of the following values
	 *  					-> enet.h
	 *
	 *	returns:
	 *      number of bytes received
	 *
	 */
	int (*CCtrlEnetRecv) ( int sock, unsigned char *buffer, int len, int flag );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetRecv ( sock, buffer, len, address, address_len);
	 *
	 *  description:
	 *		receive a packet
	 *
	 *  parameters:
	 *  	sock  			the socket to receive data
	 *  	buffer	 		receive buffer
	 *  	len				length of the buffer
     *		address			buffer to hold stack information
	 *		address_len		length of address
	 *
	 *	returns:
	 *      number of bytes received
	 *
	 */
	int (*CCtrlEnetRecvfrom) ( int sock, unsigned char *buffer, int len, unsigned char *address, unsigned int *address_len );

	/*****************************************************************
	 *
	 *  error = CCtrlEnetclose ( sock );
	 *
	 *  description:
	 *		close a socket
	 *
	 *  parameters:
	 *  	sock  			the socket to be closed
	 *
	 *	returns:
	 *
	 */
	void (*CCtrlEnetSocketclose) ( int sock );

	/***************************************************************
	 *
	 *  error = AASiReadPE (Circuit, Address, &PE);
	 *
	 *  description:
	 *		Read the echo parameters of one slave
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Address of slave to get the echo parameters from (1..31, 33..63).
	 *
	 *  returns:
	 *		PE: Echo parameters of the slave (low nibble)
	 */
	int (*AASiReadPE) (unsigned char Circuit, AASiSlaveAddr Address, AASiSlaveData *PE);

	/*****************************************************************
	 *
	 *  error = AASiSetCPRL (Circuit, CPRL);
	 *
	 *  description:
	 *		Write list of cyclic parameter requests.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		LOS: 8 bytes slave addresses list.
	 *			Every bit in the SPL corresponds to one slave after the following scheme:
	 *			Bit 0:	Slave 0/0A
	 *			Bit 1:	Slave 1/1A
	 *			...
	 *			Bit31:	Slave 31/31A
	 *			Bit32 :	Slave 0B
	 *			Bit33:	Slave 1B
	 *			...
	 *			Bit63:	Slave 31B
	 *			The cyclic parameter requests is active if the bit is set.
	 *
	 * 	returns:
	 */
	int (*AASiSetCPRL) (unsigned char Circuit, AASiSlaveList CPRL);

	/*********************************************************************
	 *
	 *  error = AASiSendParameter (Circuit, Address, PI);
	 *
	 *  description:
	 *		Send parameters to one slave without slave echo!
	 *		please read the echo to make sure parameter was send
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Address of slave to send parameters to (1..31, 33..63).
	 *		PI: Parameters to send (low nibble)
	 *
	 *	returns:
	 *		!0	error
	 *		0	no error
	 */
	int (*AASiSendParameterNonBlocking) (unsigned char Circuit, AASiSlaveAddr Address, AASiSlaveData PI);

	/*********************************************************************
	 *
	 *  systime = GetSystime ();
	 *
	 *  description:
	 *		returns the current systime (1ms)
	 *		!!!Attention!!!
	 *		this is a 32 bit value that overflows after ~49 days
	 *
	 *  parameters:
	 *
	 *	returns:
	 *		systime
	 *
	 */
	unsigned long (*GetSystime) (void);

	/*****************************************************************
	 *
	 *  error = CCtrlReadParameterAdditonal ( *buffer, len, adr );
	 *
	 *  description:
	 *		Read Control additional NV Parameter
	 *
	 *
	 *  parameters:
	 *		len: length of the buffer
	 * 		adr: address of the first byte to read data from
	 *
	 *	returns:
	 *		buffer: read buffer
	 */
	int  (*CCtrlReadParameterAdditional) ( unsigned char *buffer, unsigned int len, unsigned int adr );

	/*****************************************************************
	 *
	 *  error = CCtrlWriteParameter ( *buffer, len, adr );
	 *
	 *  description:
	 *		Write Control NV Parameter
	 *
	 *  **** Attention:
	 *		Parameter will not be stored on chipcard
	 *
	 *  parameters:
	 *		buffer: write buffer
	 *		len: length of the buffer
	 * 		adr: address of the first byte to write data to
	 *
	 *	returns:
	 */
	int  (*CCtrlWriteParameterAdditional) ( unsigned char *buffer, unsigned int len, unsigned int adr );

	/*****************************************************************
	 *
	 *  error = CCtrlFcntl ( sock, command, argument );
	 *
	 *  description:
	 *		Set or get fcntl flags with specific command
	 *
	 *  parameters:
	 *		sock:     the control III socket
	 *		command:  command, see enet.h
	 *		argument: the falgs to write
	 *
	 *	returns:
	 *  flags if ok
	 *	   !0 if not ok
	 */
	int  (*CCtrlFcntl)	( int sock, int command, long argument );

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetWriteBinaryOutputPresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Write Present-Value of specified BACnet object instance of type Binary Output.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	Priority: Priority for writing (1-16)
	 *  	value   : Value to write (0:BACNET_BINARY_INACTIVE, 1: BACNET_BINARY_ACTIVE)
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetWriteBinaryOutputPresentValue) (unsigned int instance, unsigned char priority, unsigned char value);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetReadBinaryOutputPresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Read Present-Value of specified BACnet object instance of type Binary Output.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	value   : Pointer to memory where result will be stored (0:BACNET_BINARY_INACTIVE, 1: BACNET_BINARY_ACTIVE)
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetReadBinaryOutputPresentValue)	(unsigned int instance, unsigned char * value);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetClearBinaryOutputPresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Write NULL to Priority-Array of specified BACnet object instance of type Binary Output.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	Priority: Priority for writing (1-16)
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetClearBinaryOutputPresentValue) (unsigned int instance, unsigned char priority);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetWriteAnalogOutputPresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Write Present-Value of specified BACnet object instance of type Analog Output.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	Priority: Priority for writing (1-16)
	 *  	value   : Value to write
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetWriteAnalogOutputPresentValue) (unsigned int instance, unsigned char priority, float value);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetReadAnalogOutputPresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Read Present-Value of specified BACnet object instance of type Analog Output.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	value   : Pointer to memory where result will be stored
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetReadAnalogOutputPresentValue) (unsigned int instance, float * value);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetClearAnalogOutputPresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Write NULL to Priority-Array of specified BACnet object instance of type Analog Output.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	Priority: Priority for writing (1-16)
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetClearAnalogOutputPresentValue) (unsigned int instance, unsigned char priority);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetWriteIntegerValuePresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Write Present-Value of specified BACnet object instance of type Integer Value.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	Priority: Priority for writing (1-16)
	 *  	value   : Value to write
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetWriteIntegerValuePresentValue) (unsigned int instance, unsigned char priority, int value);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetReadIntegerValuePresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Read Present-Value of specified BACnet object instance of type Integer Value.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	value   : Pointer to memory where result will be stored
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetReadIntegerValuePresentValue) (unsigned int instance, int * value);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetClearIntegerValuePresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Write NULL to Priority-Array of specified BACnet object instance of type Integer Value.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	Priority: Priority for writing (1-16)
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetClearIntegerValuePresentValue) (unsigned int instance, unsigned char priority);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetReadBinaryInputPresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Read Present-Value of specified BACnet object instance of type Binary Input.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	value   : Pointer to memory where result will be stored
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetReadBinaryInputPresentValue) (unsigned int instance, unsigned char * value);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetReadAnalogInputPresentValue (instance, prio, value);
	 *
	 *  description:
	 *		Read Present-Value of specified BACnet object instance of type Analog Input.
	 *
	 *  parameters:
	 *  	instance: Object instance number
	 *  	value   : Pointer to memory where result will be stored
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int  (*CCtrlBACnetReadAnalogInputPresentValue) (unsigned int instance, float * value);

	/* Asi style api */

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetWriteODI (Circuit, ODI, First, Amount, Priority);
	 *
	 *  description:
	 *		Writes output data to specific slaves via BACnet WriteProperty service.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		ODI: 32 bytes of output data (output data image).
	 *			Every slave uses one nibble of a byte.
	 *			The format is as follows:
	 *			Byte  0, low  nibble: Slave  'First' data
	 *			Byte  0, high nibble: Slave  'First'+1 data
	 *			...
	 *
	 *		First: Index of the first slave to send data to (0..63)
	 *
	 *		Amount: Number of slaves to send data to. (1..64)
	 *		Priority: Priority for writing (1-16)
	 *
	 *	returns:
	 */
	int  (*CCtrlBACnetWriteODI) (unsigned char Circuit, AASiProcessData ODI, AASiSlaveAddr First, unsigned char Amount, unsigned char Priority);

	/*********************************************************************
	 *
	 *  error = CCtrlBACnetWrite16BitODI (Circuit, Address, Out; Priority);
	 *
	 *  description:
	 *      writes four channels of 16bit ODI to an AS-Interface slave
	 *      with e.g. analog slave profil 7.3 or 7.4
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		Address: Slave address (1..31)
	 *      Out: 4 channels of 16-Bit values
	 *           Word 0: channel 1
	 *           ...
	 *           Word 3: channel 4
	 *     Priority: Priority for writing (1-16)
	 *	returns:
	 */
	int (*CCtrlBACnetWrite16BitODI) ( unsigned char Circuit, AASiSlaveAddr Address, AASi16BitData Out, unsigned char Priority);

	/*********************************************************************
	 *
	 *  error = AASiDataExchange (Circuit, ODI, IDI, &EcFlags, Priority);
	 *
	 *  description:
	 *		exchange of ASi Slave data and reading of the execution control flags.
	 *
	 *  parameters:
	 *		Circuit: AS-i Master circuit
	 *		ODI: 32 bytes output data to the slaves (output data image).
	 *			Every slave uses one nibble of a byte. The format is as follows:
	 *			Byte  0, low  nibble: data of slave  0/0A
	 *			Byte  0, high nibble: data of slave  1/1A
	 *			...
	 *			Byte 15, low  nibble: data of slave 30/30A
	 *			Byte 15, high nibble: data of slave 31/31A
	 *			Byte 16, low  nibble: data of slave  0B
	 *			Byte 16, high nibble: data of slave  1B
	 *			...
	 *			Byte 31, low  nibble: data of slave 30B
	 *			Byte 31, high nibble: data of slave 31B
	 *	    Priority: Priority for ODI writes
	 *
	 *	returns:
	 *		IDI: 32 bytes input data from the slave (input data image).
	 *			The format is the same is in ODI.
	 *
	 *		EcFlags: Execution control flags (2 bytes) of the AS-Interface Master.
	 */
	int (*CCtrlBACnetDataExchange) (unsigned char Circuit, AASiProcessData ODI, AASiProcessData IDI, AASiEcFlags *EcFlags, unsigned char Priority);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetReadInstance (objtype, instance, property, arrayindex, data_buffer, data_buffer_size, number_elements, datatype_tag)
	 *
	 *  description:
	 *		Read Property.
	 *
	 *  parameters:
	 *  	objtype : Object type identifier
	 *  	instance: Object instance number
	 *  	property: Property identifier
	 *  	arrayindex: Index or all ones (~0) for non array types
	 *  	data_buffer: Pointer to buffer of suitable size of desired data type
	 *  	data_buffer_size: Size of buffer of desired data type
	 *  	number_elements: Number of elements in buffer
	 *  	datatype_tag: Tag that specifies buffer content
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int (*CCtrlBACnetReadInstance) (unsigned char objtype, unsigned int instance, int property,	unsigned int arrayindex, void * data_buffer,
			unsigned int data_buffer_size, unsigned int number_elements, unsigned int datatype_tag);

	/*****************************************************************
	 *
	 *  error = CCtrlBACnetWriteInstance (objtype, instance, property, arrayindex, priority, data_buffer, data_buffer_size, number_elements, datatype_tag)
	 *
	 *  description:
	 *		Read Property.
	 *
	 *  parameters:
	 *  	objtype : Object type identifier
	 *  	instance: Object instance number
	 *  	property: Property identifier
	 *  	arrayindex: Index or all ones (~0) for non array types
	 *  	priority: Priority: Priority for writing (1-16)
	 *  	data_buffer: Pointer to buffer of suitable size of desired data type
	 *  	data_buffer_size: Size of buffer of desired data type
	 *  	number_elements: Number of elements in buffer
	 *  	datatype_tag: Tag that specifies buffer content
	 *	returns:
	 *		0 : No error occured
	 *		!0: Function returned error code.
	 *
	 */
	int (*CCtrlBACnetWriteInstance) (unsigned char objtype,	unsigned int instance, int property, unsigned int arrayindex, unsigned char priority, void * data_buffer,
			unsigned int data_buffer_size, unsigned int number_elements, unsigned int datatype_tag);

	/*****************************************************************
	 *
	 *  error = Asi5ReadCtrlAccODI ( circuit, ...);
	 *
	 *  description:
	 *		Read back the actual Ctrl access data for the ASi-5 Ctrl output data image.
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		mask            the bit mask of the acc
	 *  	length          the length of the mask to read
	 *  	offset          the offset of the mask to read
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ReadCtrlAccODI) (const unsigned char circuit, unsigned char *mask, unsigned short *length, const unsigned short offset);

	/*****************************************************************
	 *
	 *  error = Asi5WriteCtrlAccODI ( circuit, ...);
	 *
	 *  description:
	 *		Write the actual Ctrl access data for the ASi-5 Ctrl output data image.
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		mask            the bit mask of the acc
	 *  	length          the length of the mask to write
	 *  	offset          the offset of the mask to write
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5WriteCtrlAccODI) (const unsigned char circuit, const unsigned char *mask, const unsigned short length, const unsigned short offset);

	/*****************************************************************
	 *
	 *  error = Asi5ClearCtrlAccODI ( circuit, ...);
	 *
	 *  description:
	 *		Clear the actual Ctrl access data for the ASi-5 Ctrl output data image.
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *  	length          the length of the mask to clear
	 *  	offset          the offset of the mask to clear
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ClearCtrlAccODI) (const unsigned char circuit, const unsigned short length, const unsigned short offset);

	/*****************************************************************
	 *
	 *  error = Asi5ReadEcFlags ( circuit, ...);
	 *
	 *  description:
	 *		Read ASi-5 execution control flags
	 *
	 *  parameters:
	 *  	ecflags         the execution flags
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ReadEcFlags) (const unsigned char circuit, unsigned short *ecflags);

	/*****************************************************************
	 *
	 *  error = Asi5OdcRequest ( circuit, ...);
	 *
	 *  description:
	 *		Send ASi-5 ODC request by logical address
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		respData        response data
	 *  	logAddr         logical address of the ASi-5 Slave
	 *  	reqData         request data
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5OdcRequest) (const unsigned char circuit, odc_data_t *respData, const unsigned short logAddr, const odc_data_t *reqData);

	/*****************************************************************
	 *
	 *  error = Asi5OdcRequestByAsiid ( circuit, ...);
	 *
	 *  description:
	 *		Send ASi-5 ODC request by asiid
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		respData        response data
	 *  	asiid           asiid of the ASi-5 Slave
	 *  	reqData         request data
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5OdcRequestByAsiid) (const unsigned char circuit, odc_data_t *respData, const asiid_t asiid, const odc_data_t *reqData);

	/*****************************************************************
	 *
	 *  error = Asi5SetOperatingMode ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 operating Mode
	 *
	 *  parameters:
	 *  	circuit             circuit select
	 *		asi_5_cfg_instance  contains the requested configuration instance
	 *  	asi_5_opmode        requested ASi-5 operating mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetOperatingMode) (const unsigned char circuit, const unsigned char asi_5_cfg_instance, const unsigned char asi_5_opmode);

	/*****************************************************************
	 *
	 *  error = Asi5SetOperatingMode ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 operating Mode
	 *
	 *  parameters:
	 *  	circuit             circuit select
	 *		asi_5_cfg_instance  contains the requested configuration instance
	 *  	asi_5_opmode        responded ASi-5 operating mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetOperatingMode) (const unsigned char circuit, unsigned char *asi_5_cfg_instance, unsigned char *asi_5_opmode);

	/*****************************************************************
	 *
	 *  error = Asi5SetDataExchangeMode ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 data exchange mode
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		asi5_demode     requested ASi-5 data exchange mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetDataExchangeMode) (const unsigned char circuit, const unsigned char asi5_demode);

	/*****************************************************************
	 *
	 *  error = Asi5GetDataExchangeMode ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 data exchange mode
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		asi5_demode     responded ASi-5 data exchange mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetDataExchangeMode) (const unsigned char circuit, unsigned char *asi5_demode);

	/*****************************************************************
	 *
	 *  error = Asi5SetOfflineMode ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 offline mode
	 *		The function is used to set AS-i 5 data exchange mode that starts or stops
	 *      the cyclic data exchange during normal operation
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		asi5_offmode    requested ASi-5 offline mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetOfflineMode) (const unsigned char circuit, const unsigned char asi5_offmode);

	/*****************************************************************
	 *
	 *  error = Asi5GetOfflineMode ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 offline mode
	 *		The function is used to get the status of the AS-i 5 offline mode.
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		asi5_offmode    responded ASi-5 offline mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetOfflineMode) (const unsigned char circuit, unsigned char *asi5_offmode);

	/*****************************************************************
	 *
	 *  error = Asi5SetAutoAddressEnable ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 auto address enable mode
	 *		The function is used to set the AS-i 5 automatic address assignment mode
	 *      active or inactive. This bit shall be stored non-volatile.
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		asi5_aaamode    requested ASi-5 auto address mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetAutoAddressEnable) (const unsigned char circuit, const unsigned char asi5_aaamode);

	/*****************************************************************
	 *
	 *  error = Asi5GetAutoAddressAssignMode ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 auto address enable mode
	 *		The function is used to get the status of the AS-i 5 automatic address
	 *      assignment mode.
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		asi5_aaamode    responded ASi-5 data auto address mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetAutoAddressAssignMode) (const unsigned char circuit, unsigned char *asi5_aaamode);

	/*****************************************************************
	 *
	 *  error = Asi5SetASi3CoexistingMode ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 ASi-3 coexisting mode
	 *		The function is used to set the AS-i 5 master to AS-i 3 CoExisting Mode.
	 *      This flag shall be stored non-volatile. The Mode will change after next offline Phase.
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		asi3_coexisting requested ASi-5 coexisting mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetASi3CoexistingMode) (const unsigned char circuit, const unsigned char asi3_coexisting);

	/*****************************************************************
	 *
	 *  error = Asi5GetASi3CoexistingMode ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 ASi-3 coexisting mode
	 *		The function is used to get the state of the AS-i 3 CoExisting Mode of the
	 *      AS-i 5 master. This flag represents the non-volatile stored value. The actual mode might be
	 *      different.
	 *
	 *  parameters:
	 *  	circuit         circuit select
	 *		asi3_coexisting responded ASi-5 coexisting mode
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetASi3CoexistingMode) (const unsigned char circuit, unsigned char *asi3_coexisting);

	/*****************************************************************
	 *
	 *  error = Asi5ChangeSlaveAddress ( circuit, ...);
	 *
	 *  description:
	 *		Change ASi-5 slave address
	 *		The function is used in order to request that the execution control shall
	 *      change the logical address to a new logical address of a specific AS-i 5 slave.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		newLogAddr     new logical address
	 *		oldLogAddr     old logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ChangeSlaveAddress) (const unsigned char circuit, const unsigned short newLogAddr, const unsigned short oldLogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5ChangeSlaveAddressByAsiId ( circuit, ...);
	 *
	 *  description:
	 *		Change ASi-5 slave address
	 *		The function is used in order to request that the execution control shall
	 *      change the logical address to a new logical address of a specific AS-i 5 slave which will be identify
	 *      by its ASIID.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		newLogAddr     new logical address
	 *		ASIID          asiid of the slave
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ChangeSlaveAddressByAsiId) (const unsigned char circuit, const unsigned short newLogAddr, const asiid_t ASIID);

	/*****************************************************************
	 *
	 *  error = Asi5InLds ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 slave in detected slave list
	 *		The function is used to check for a specific logical AS-i 5 slave address
	 *      whether it is in the ASI5LDS.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		in_lds         0: ASI5Saddr is not in ASI5LDS, !0: ASI5Saddr is in ASI5LDS
	 *		LogAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5InLds) (const unsigned char circuit, unsigned char *in_lds, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5InLps ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 slave in protected slave list
	 *		The function is used to check for a specific logical AS-i 5 slave address
	 *      whether it is in the ASI5LPS of the actual configuration instance.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		in_lds         0: ASI5Saddr is not in ASI5LPS, !0: ASI5Saddr is in ASI5LPS
	 *		LogAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5InLps) (const unsigned char circuit, unsigned char *in_lps, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5InLpf ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 slave in peripheral error slave list
	 *		The function is used to get the AS-i 5 slave status code of a specific
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		in_lpf         in_lpf AS-i 5 slave status code
	 *		LogAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5InLpf) (const unsigned char circuit, asi_5_lpf_t *in_lpf, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5InLms ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 slave
	 *		The function is used to check for a specific logical AS-i 5 slave address
	 *      whether it is in the ASI5LMS.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		in_lms         0: ASI5Saddr is not in Asi5InLms, !0: ASI5Saddr is in Asi5InLms
	 *		LogAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5InLms) (const unsigned char circuit, unsigned char *in_lms, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5InLas ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 slave activated slave list
	 *		The function is used to check for a specific logical AS-i 5 slave address
	 *      whether it is in the ASI5LAS.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		in_las         0: ASI5Saddr is not in ASI5LAS, !0: ASI5Saddr is in ASI5LAS
	 *		LogAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5InLas) (const unsigned char circuit, unsigned char *in_las, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5InLis ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 slave
	 *		The function is used to check for a specific logical AS-i 5 slave address
	 *      whether it is in the ASI5LIS.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		in_lis         0: ASI5Saddr is not in ASI5LIS, !0: ASI5Saddr is in ASI5LIS
	 *		LogAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5InLis) (const unsigned char circuit, unsigned char *in_lis, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5InLda ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 slave
	 *		The function is used to check for a specific logical AS-i 5 slave address
	 *      whether it is in the ASI5LDA.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		in_lda         0: ASI5Saddr is not in ASI5LDA, !0: ASI5Saddr is in ASI5LDA
	 *		LogAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5InLda) (const unsigned char circuit, unsigned char *in_lda, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5InLdd ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 slave in double address list
	 *		The function is used to check for a specific logical AS-i 5 slave address
	 *      whether it is in the ASI5LDD.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		in_ldd         0: ASI5Saddr is not in ASI5LDD, !0: ASI5Saddr is in ASI5LDD
	 *		LogAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5InLdd) (const unsigned char circuit, unsigned char *in_ldd, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5SetCdi ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 cdi
	 *		The function is used to change the configuration data image for a specific
	 *      AS-i 5 slave.
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *		cdi          requested configuration data image
	 *		LogAddr      logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetCdi) (const unsigned char circuit, const asi_5_slv_config_t *cdi, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5GetCdi ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 cdi
	 *		The function is used to get the configuration data for a specific address
	 *      from the AS-i 5 configuration data image (ASI5CDI).
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *		cdi          responded configuration data image
	 *		LogAddr      logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetCdi) (const unsigned char circuit, asi_5_slv_config_t *cdi, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5SetPcd ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 pcd
	 *		The function is used to store a delivered configuration value for a specific
	 *      logical address into the permanent configuration data (ASI5PCD) of the actual configuration
	 *      instance non-volatile. This function shall be processed in the AS-i 5 configuration mode only.
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *		pcd          configuration data to be set
	 *		LogAddr      logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetPcd) (const unsigned char circuit, const asi_5_slv_config_t *pcd, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5GetPcd ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 pcd
	 *		The function is used to get the configuration data for a specific logical
	 *      address from the AS-i 5 permanent configuration data (ASI5PCD) of the actual configuration instance.
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *		pcd          permanent configuration data
	 *		LogAddr      logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetPcd) (const unsigned char circuit, asi_5_slv_config_t *pcd, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5StoreCdi ( circuit, ...);
	 *
	 *  description:
	 *		Store ASi-5 cdi
	 *		The function is used to copy the values of the AS-i 5 configuration data
	 *      image (ASI5CDI) into the AS-i 5 permanent configuration data (ASI5PCD) of the actual configuration
	 *      instance for a single AS-i 5 slave. If the AS-i 5 slave is present in the AS-i 5 list of activated AS-i
	 *      5 slaves (ASI5LAS) the logical address is added to the AS-i 5 list of projected AS-i 5 slaves
	 *      (ASI5LPS) of the actual configuration instance. This function shall be processed in AS-i 5 configuration
	 *      mode only.
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5StoreCdi) (const unsigned char circuit);

	/*****************************************************************
	 *
	 *  error = Asi5StoreSlaveCdi ( circuit, ...);
	 *
	 *  description:
	 *		Stored ASi-5 cdi
	 *		The function is used to copy the values of the AS-i 5 configuration data
	 *      image (ASI5CDI) into the AS-i 5 permanent configuration data (ASI5PCD) of the actual configuration
	 *      instance for a single AS-i 5 slave. If the AS-i 5 slave is present in the AS-i 5 list of activated AS-i
	 *      5 slaves (ASI5LAS) the logical address is added to the AS-i 5 list of projected AS-i 5 slaves
	 *      (ASI5LPS) of the actual configuration instance. This function shall be processed in AS-i 5 configuration
	 *      mode only.
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	LogAddr      logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5StoreSlaveCdi) (const unsigned char circuit, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5ResetPcd ( circuit, ...);
	 *
	 *  description:
	 *		Reset ASi-5 pcd
	 *		The function is used to set the permanent configuration data for a specific
	 *      logical address into the AS-i 5 permanent configuration data (ASI5PCD) of the actual configuration
	 *      instance non-volatile to the default value. This function shall be processed in the AS-i 5
	 *      configuration mode only.
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	LogAddr      logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ResetPcd) (const unsigned char circuit, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5ResetAllPcd ( circuit, ...);
	 *
	 *  description:
	 *		Reset all ASi-5 pcd
	 *		The function is used to set the permanent configuration data for a specific
	 *      logical address into the AS-i 5 permanent configuration data (ASI5PCD) of the actual configuration
	 *      instance non-volatile to the default value. This function shall be processed in the AS-i 5
	 *      configuration mode only.
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ResetAllPcd) (const unsigned char circuit);

	/*****************************************************************
	 *
	 *  error = Asi5GetLas ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 list of activated slaves
	 *		The function is used to get the complete slave list ASI5LAS.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		length         length of the las buffer
	 *		las            list of activated slaves
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetLas) (const unsigned char circuit, unsigned char *length, unsigned short *las);

	/*****************************************************************
	 *
	 *  error = Asi5GetLps ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 list of protected slaves
	 *		The function is used to get the complete slave list ASI5LPS.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		length         length of the lps buffer
	 *		lps            list of protected slaves
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetLps) (const unsigned char circuit, unsigned char *length, unsigned short *lps);

	/*****************************************************************
	 *
	 *  error = Asi5GetLds ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 list of detected slaves
	 *		The function is used to get the complete slave list ASI5LDS.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		length         length of the lds buffer
	 *		lds            list of detected slaves
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetLds) (const unsigned char circuit, unsigned char *length, unsigned short *lds);

	/*****************************************************************
	 *
	 *  error = Asi5GetLms ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 list
	 *		The function is used to get the complete slave list ASI5LMS.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		length         length of the lms buffer
	 *		lms            list of
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetLms) (const unsigned char circuit, unsigned char *length, unsigned short *lms);

	/*****************************************************************
	 *
	 *  error = Asi5GetLpf ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 list of peripheral faults
	 *		The function is used to get the complete slave list ASI5LPF.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		length         length of the lpf buffer
	 *		lpf            list of
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetLpf) (const unsigned char circuit, unsigned char *length, asi_5_lpf_list_t *lpf);

	/*****************************************************************
	 *
	 *  error = Asi5GetLis ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 list
	 *		The function is used to get the complete slave list ASI5LIS.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		length         length of the lis buffer
	 *		lis            list of
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetLis) (const unsigned char circuit, unsigned char *length, unsigned short *lis);

	/*****************************************************************
	 *
	 *  error = Asi5GetLda ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 list
	 *		The function is used to get the complete slave list ASI5LDA.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		length         length of the lda buffer
	 *		lda            list of
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetLda) (const unsigned char circuit, unsigned char *length, unsigned short *lda);

	/*****************************************************************
	 *
	 *  error = Asi5GetLdd ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 detected double addresses
	 *		The function is used to get the complete slave list ASI5LDD.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		length         length of the ldd buffer
	 *		ldd            list of
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetLdd) (const unsigned char circuit, unsigned char *length, unsigned short *ldd);

	/*****************************************************************
	 *
	 *  error = Asi5GetSlaveAsiid ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 Slave asiid's
	 *		The function is used to get a list of ASIID of AS-i 5 slave which uses a
	 *      specific logical address.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		elements       Length of ASIID list
	 *		ASIIDList      ASIID list of AS-i 5 Slave which has the logical address ASI5Saddr
	 *		logAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetSlaveAsiid) (const unsigned char circuit, unsigned char *elements, asiid_t *ASIIDList, const unsigned short logAddr);

	/*****************************************************************
	 *
	 *  error = Asi5GetSlaveInfo ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 Slave information
	 *		The function is used by the host interface to get the complete AS-i 5 slave info structure
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		slv_info       AS-i 5 slave info structure
	 *		logAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetSlaveInfo) (const unsigned char circuit, asi_5_slvinfo_t *slv_info, const unsigned short logAddr);

	/*****************************************************************
	 *
	 *  error = Asi5GetSlaveInfoAsiid ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 Slave information by asiid
	 *		The function is used to get the complete AS-i 5 slave info structure. The
	 *      specific AS-i 5 slave will be identify by its ASIID.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		slv_info       AS-i 5 slave info structure
	 *		ASIID          ASIID of AS-i 5 slave to get the AS-i 5 slave info structure from
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetSlaveInfoAsiid) (const unsigned char circuit, asi_5_slvinfo_t *slv_info, asiid_t ASIID);

	/*****************************************************************
	 *
	 *  error = Asi5GetSystemResources ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 system resources
	 *		The function is used to get an overview of the used resources of the AS-i 5
	 *      system.
	 *
	 *  parameters:
	 *  	circuit          circuit select
	 *		systemResources  overview of the used resources of the AS-i 5 system
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetSystemResources) (const unsigned char circuit, cm_systemResources_t *systemResources);

	/*****************************************************************
	 *
	 *  error = Asi5IdentifySlaveByLogicalAddress ( circuit, ...);
	 *
	 *  description:
	 *		Identify ASi-5 slave
	 *		The function is used to force the AS-i 5 slave to start an optical
	 *      identification. If the logical AS-i 5 slave address is part of the ASI5LDA more than one slave will do
	 *      optical identification.
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		logAddr        logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5IdentifySlaveByLogicalAddress) (const unsigned char circuit, const unsigned short LogAddr);

	/*****************************************************************
	 *
	 *  error = Asi5IdentifySlaveByAsiid ( circuit, ...);
	 *
	 *  description:
	 *		Identify ASi-5 slave
	 *		The function is used to force the AS-i 5 slave to start an optical
	 *      identification. The specific AS-i 5 slave will be identify by its ASIID
	 *
	 *  parameters:
	 *  	circuit        circuit select
	 *		ASIID          ASIID of AS-i 5 slave
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5IdentifySlaveByAsiid) (const unsigned char circuit, const asiid_t ASIID);

	/*****************************************************************
	 *
	 *  error = Asi5SetDeviceDesignatorByLogicalAddress ( circuit, ...);
	 *
	 *  description:
	 *		Set device designator from ASi-5 slave
	 *		This function stores the device designator of the AS-i 5 slave.
	 *
	 *  parameters:
	 *  	circuit          circuit select
	 *		logAddr          logical address
	 *		deviceDesignator contains device designator for the AS-i 5 device
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetDeviceDesignatorByLogicalAddress) (const unsigned char circuit, const unsigned short LogAddr, const unsigned char *deviceDesignator);

	/*****************************************************************
	 *
	 *  error = Asi5SetDeviceDesignatorByAsiid ( circuit, ...);
	 *
	 *  description:
	 *		Set device designator from ASi-5 slave
	 *		This function stores the device designator of the AS-i 5 slave.
	 *
	 *  parameters:
	 *  	circuit          circuit select
	 *		ASIID            ASIID of AS-i 5 slave
	 *		deviceDesignator contains device designator for the AS-i 5 device
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetDeviceDesignatorByAsiid) (const unsigned char circuit, const asiid_t ASIID, const unsigned char *deviceDesignator);

	/*****************************************************************
	 *
	 *  error = Asi5GetDeviceDesignatorByLogicalAddress ( circuit, ...);
	 *
	 *  description:
	 *		Get device designator from ASi-5 slave
	 *		This function returns the device designator of the AS-i 5 slave.
	 *
	 *  parameters:
	 *  	circuit          circuit select
	 *		logAddr          logical address
	 *		deviceDesignator contains device designator from the AS-i 5 device
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetDeviceDesignatorByLogicalAddress) (const unsigned char circuit,const  unsigned short LogAddr, unsigned char *deviceDesignator);

	/*****************************************************************
	 *
	 *  error = Asi5GetDeviceDesignatorByAsiid ( circuit, ...);
	 *
	 *  description:
	 *		Get device designator from ASi-5 slave
	 *		This function returns the device designator of the AS-i 5 slave.
	 *
	 *  parameters:
	 *  	circuit          circuit select
	 *		ASIID            ASIID of AS-i 5 slave
	 *		deviceDesignator contains device designator from the AS-i 5 device
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetDeviceDesignatorByAsiid) (const unsigned char circuit, const asiid_t ASIID, unsigned char *deviceDesignator);

	/*****************************************************************
	 *
	 *  error = Asi5AutoSetLogicalAddresses ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 logical address auto
	 *		The function is used by the host interface to automatically set the logical address of all AS-i 5
	 *      slaves with logical address 0. The slaves are ordered by device designator
	 *
	 *  parameters:
	 *  	circuit          circuit select
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5AutoSetLogicalAddresses) (const unsigned char circuit);

	/*****************************************************************
	 *
	 *  error = Asi5SetEnergySavingState ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 energy saving state
	 *		This function changes the energy saving state of all AS-i 5 slave which are assigned to the
	 *      selected energy saving group
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *  	EnSvGrp       AS-i 5 energy saving group
	 *  	EnSvState     energy saving state:
	 *                    0	Switch Off energy saving
	 *                    1	Switch On energy
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetEnergySavingState) (const unsigned char circuit, const unsigned char EnSvGrp, const unsigned char EnSvState);

	/*****************************************************************
	 *
	 *  error = Asi5GetEnergySavingState ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 energy saving state
	 *		This function returns the energy saving state of the selected energy saving group
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *  	EnSvGrp       AS-i 5 energy saving group
	 *  	EnSvState     energy saving state:
	 *                    0	energy saving is switched off
	 *                    1	energy saving is switched on
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetEnergySavingState) (const unsigned char circuit, const unsigned char EnSvGrp, unsigned char *EnSvState);

	/*****************************************************************
	 *
	 *  error = Asi5WriteASi5Odi ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 output data image
	 *		This function sets the output data image of ASi-5 slaves
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *  	odi           output data image
	 *  	length        the length of the image to set
	 *  	offset        the offset for the image to set
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5WriteASi5Odi) (const unsigned char circuit, const unsigned char *odi, const unsigned short length, const unsigned short offset);

	/*****************************************************************
	 *
	 *  error = Asi5ReadASi5Odi ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 output data image
	 *		This function gets the output data image of ASi-5 slaves
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *  	odi           output data image
	 *  	length        the length of the image to get
	 *  	offset        the offset for the image to get
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ReadASi5Odi) (const unsigned char circuit, unsigned char *odi, unsigned short *length, const unsigned short offset);

	/*****************************************************************
	 *
	 *  error = Asi5ReadASi5Idi ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 input data image
	 *		This function gets the input data image of ASi-5 slaves
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *  	idi           input data image
	 *  	length        the length of the image to get
	 *  	offset        the offset for the image to get
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ReadASi5Idi) (const unsigned char circuit, unsigned char *idi, unsigned short *length, const unsigned short offset);

	/*****************************************************************
	 *
	 *  error = Asi5DiagGetMasterStatus ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 master status
	 *		Returns the Diagnostic Status of the AS-i 5 master
	 *
	 *  parameters:
	 *  	circuit           circuit select
	 *  	startIndex        Index of the first item that shall be read from the Diagnostic Status. First index is 0
	 *  	reqNumOfEntries   Requested number of entries of Diagnostic Information from the Master Diagnostics Status
	 *  	totalNumOfEntries Total number of items of Diagnostic Information in the Master Diagnostic Status
	 *  	retNumOfEntries   Returned number of entries of Diagnostics Information from the Master Diagnostic Status
	 *  	entries           entries Entries of Diagnostic Information of the Diagnostic status
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5DiagGetMasterStatus) (const unsigned char circuit, const unsigned char startIndex, unsigned char reqNumOfEntries, unsigned char *totalNumOfEntries, unsigned char *retNumOfEntries, diag_info_t *entries);

	/*****************************************************************
	 *
	 *  error = Asi5GetParameters ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 parameters
	 *		This function is used to get parameters out of the parameter image of an AS-i 5 slave
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	logAddr      logical address
	 *  	startIndex   First index to get part of PI
	 *  	count        Count of indexes for readout
	 *  	data         Data to place in the PI part of the StartIndex and the following indexes
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetParameters) (const unsigned char circuit, const unsigned short logAddr, const unsigned char startIndex, const unsigned char count, unsigned char *data);

	/*****************************************************************
	 *
	 *  error = Asi5SetParameters ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 parameters
	 *		This function is used to assign parameters into the parameter image of the AS-i 5 slave
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	logAddr      logical address
	 *  	startIndex   First index for modification in the PI
	 *  	count        Count of indexes for modification
	 *  	data         Data to place in the PI on the StartIndex and the following indexes
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetParameters) (const unsigned char circuit, const unsigned short logAddr, const unsigned char startIndex, const unsigned char count, const unsigned char *data);

	/*****************************************************************
	 *
	 *  error = Asi5SetPsMode ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 ps mode
	 *		This function is used to set the current operation mode (ASI5PSMode) of the parameter server
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	psMode       Next operation mode of the parameter server
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetPsMode) (const unsigned char circuit, ps_mode_e psMode);

	/*****************************************************************
	 *
	 *  error = Asi5GetPsMode ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 ps mode
	 *		This function is used to get the current operation mode (ASI5PSMode) of the parameter server
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	actualMode   Returns the actual mode of the parameter server
	 *  	storedMode   Returns the stored operation mode of the parameter server
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetPsMode) (const unsigned char circuit, ps_mode_e *actualMode, ps_mode_e *storedMode);

	/*****************************************************************
	 *
	 *  error = Asi5GetStoredPi ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 stored pi
	 *		This function reads the stored PI for an AS-i 5 slave from the AS-i Master. The returned PI
	 *      depends on the parameter image set of the current active configuration
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	logAddr      logical address
	 *  	profileID    ProfileID which the slave had, when the parameter backup was made
	 *  	vendorID     VendorID which the slave had, when the parameter backup was made
	 *  	PI           Pointer for parameter image of AS-i 5 slave
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetStoredPi) (const unsigned char circuit, const unsigned short logAddr, unsigned long *profileID, unsigned short *vendorID, unsigned char *PI);

	/*****************************************************************
	 *
	 *  error = Asi5SetStoredPi ( circuit, ...);
	 *
	 *  description:
	 *		Set ASi-5 stored pi
	 *		This function stores the PI for an AS-i 5 slave into the parameter image set of the current active
	 *      configuration in the AS-i Master
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	logAddr      logical address
	 *  	PI           Pointer for parameter image of AS-i 5 slave to store
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SetStoredPi) (const unsigned char circuit, const unsigned short logAddr, const unsigned char *PI);

	/*****************************************************************
	 *
	 *  error = Asi5StorePi ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 store pi
	 *		This function uploads a new parameter image version from the AS-i 5 slave to the AS-i 5 master and
	 *      stores it. The parameter image is stored dependent to the current configuration in the actual
	 *      parameter image set
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	logAddr      logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5StorePi) (const unsigned char circuit, const unsigned short logAddr);

	/*****************************************************************
	 *
	 *  error = Asi5RestorePi ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 restore pi
	 *		This function downloads the stored PI into the AS-i 5 slave. The PI depends on the current active
	 *      configuration
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	logAddr      logical address
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5RestorePi) (const unsigned char circuit, const unsigned short logAddr);

	/*****************************************************************
	 *
	 *  error = Asi5StoreAllSlavePi ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 store all slave pi
	 *      This function is used by the host interface to store the current used parameter images of all AS-i
	 *      5 slaves into the AS-i 5 master. The parameter images are stored non-volatile. If the storing for
	 *      one slave fails then the AS-i 5 master deletes the whole PI set
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5StoreAllSlavePi) (const unsigned char circuit);

	/*****************************************************************
	 *
	 *  error = Asi5RestoreAllSlavePi ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 restore all slave pi
	 *      This function is used by the host interface to download the parameter image to all AS-i 5 slaves
	 *      from the AS-i 5 master
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5RestoreAllSlavePi) (const unsigned char circuit);

	/*****************************************************************
	 *
	 *  error = Asi5SlavenumberToLogAddr ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 slave number to logical address
	 *      This function converts the slave number to logical address
	 *
	 *  parameters:
	 *  	circuit      circuit select
	 *  	slavenumber  slave number of the slave
	 *  	p_logAddr    the logical address of the requested slave
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5SlavenumberToLogAddr) (const unsigned char circuit, const unsigned char slavenumber, unsigned short *p_logAddr);

	/*****************************************************************
	 *
	 *  error = Asi5LogAddrToSlavenumber ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 logical address to slave number
	 *      This function converts a logical address to a slave number
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *  	logAddr       logical address of the slave
	 *  	p_slavenumber the slave number of the requested slave
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5LogAddrToSlavenumber) (const unsigned char circuit, const unsigned short logAddr, unsigned char *p_slavenumber);

	/*****************************************************************
	 *
	 *  error = Asi5DeltaList ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 delat list
	 *      This function returns the list of configuration errors
	 *
	 *  parameters:
	 *  	circuit     circuit select
	 *  	length      length of the delta list buffer
	 *  	deltaList   return of the delta list
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5DeltaList) (const unsigned char circuit, unsigned short *length, unsigned short *deltaList);

	/*****************************************************************
	 *
	 *  error = Asi5InDeltaList ( circuit, ...);
	 *
	 *  description:
	 *		ASi-5 in delta list
	 *      This function checks if the logical address is in the delta list
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *  	logAddr       logical address
	 *  	in_deltaList  0: ASI5Saddr is not in delta list, !0: ASI5Saddr is in delta list
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5InDeltaList) (const unsigned char circuit, unsigned char *in_deltaList, const unsigned short logAddr);

	/*****************************************************************
	 *
	 *  error = Asi5GetIoLength ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 IO length
	 *      This function return the i/o length of the slave
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *  	logAddr       logical address
	 *  	odiLength     length of the ouput data
	 *  	idiLength     length of the input data
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetIoLength) (const unsigned char circuit, const unsigned short logAddr, unsigned char *odiLength, unsigned char *idiLength);

	/*****************************************************************
	 *
	 *  error = Asi5GetIoLengthByAsiid ( circuit, ...);
	 *
	 *  description:
	 *		Get ASi-5 IO length by asiid
	 *      This function return the i/o length of the slave by the asiid
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *  	ASIID         asiid
	 *  	odiLength     length of the ouput data
	 *  	idiLength     length of the input data
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5GetIoLengthByAsiid) (const unsigned char circuit, const asiid_t ASIID, unsigned char *odiLength, unsigned char *idiLength);

	/*****************************************************************
	 *
	 *  error = Asi5ResetAllSlaves ( circuit, ...);
	 *
	 *  description:
	 *		Reset all ASi-5 slaves
	 *      This function resets all ASi-5 slaves
	 *
	 *  parameters:
	 *  	circuit       circuit select
	 *
	 *	returns:
	 *      0  if ok
	 *      !0 if not ok
	 */
	return_t (*Asi5ResetAllSlaves) (const unsigned char circuit);
}
ccontrol_func_t;

/*-------------------------------------------------------------------*
 *  function prototypes                                              *
 *-------------------------------------------------------------------*/

int main ( void );

/*-------------------------------------------------------------------*
 *  global definitions                                               *
 *-------------------------------------------------------------------*/
extern volatile ccontrol_func_t cctrl_func __attribute__ ((__section__ (".funcarea")));

/*-------------------------------------------------------------------*
 *  global data                                                      *
 *-------------------------------------------------------------------*/ 

#endif	/*	ifndef _CCONTROL_H_	*/

/*-------------------------------------------------------------------*
 *  eof                                                              *
 *-------------------------------------------------------------------*/

