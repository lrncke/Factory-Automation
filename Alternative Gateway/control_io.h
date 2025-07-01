/*-------------------------------------------------------------------*
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Control_III                                        *
 *   module name: control_io.h                                       *
 *        author: Christian Sommerfeld                               *
 *          date: 2015-02-26                                         *
 *                                                                   *
 *  version: 1.0 first version                                       *
 *                                                                   *
 *                                                                   *                                                                *
 *-------------------------------------------------------------------*/

/*********************************************************************
 *
 *  error = read_bit (idi, slave_address, bit);
 *
 *  description:
 *		read one bit of one slave
 *
 *  parameters:
 *		idi: AS-i Master circuit
 *			filled by DataExchange with the input data of the selected circuit
 *		slave_addr: Adress of one Slave
 *				0 = Slave 0/0A
 *				1 = Slave 1/1A
 *				2 = Slave 2/2A
 *				.
 *				.
 *				32 = Slave 0B
 *				33 = Slave 1B
 *				34 = Slave 2B
 *				.
 *				.
 *				.
 *		bit: output to read
 *
 *	returns:
 *			the current state of the output
 *
 */
int read_bit (AASiProcessData idi, int slave_addr, int bit);

/*********************************************************************
 *
 *  error = write_bit (odi, slave_address, bit, value);
 *
 *  description:
 *		write one bit of one slave
 *
 *  parameters:
 *		odi: AS-i Master circuit
 *			written by DataExchange with the output data of the selected circuit
 *		slave_addr: Adress of one Slave
 *				0 = Slave 0/0A
 *				1 = Slave 1/1A
 *				2 = Slave 2/2A
 *				.
 *				.
 *				32 = Slave 0B
 *				33 = Slave 1B
 *				34 = Slave 2B
 *				.
 *				.
 *				.
 *		bit: output to set
 *
 */
void write_bit(AASiProcessData odi, int slave_addr, int bit, int value);

/*********************************************************************
 *
 *  error = read_flag (byte, bit);
 *
 *  description:
 *		reads one bit of a flag
 *
 *  parameters:
 *		byte: byte number
 *				0 - 255
 *
 *		bit: bit
 *				0 - 7
 *		0 -> 00000001
 *		1 -> 00000010
 *		2 -> 00000100
 *		3 -> 00001000
 *		4 -> 00010000
 *		5 -> 00100000
 *		6 -> 01000000
 *		7 -> 10000000
 *
 */
int read_flag_bit (int byte, int bit);

/*********************************************************************
 *
 *  error = write_flag (byte, bit);
 *
 *  description:
 *		write one bit of a flag
 *
 *  parameters:
 *		byte: byte number
 *				0 - 255
 *
 *		bit: bit
 *				0 - 7
 *		0 -> 00000001
 *		1 -> 00000010
 *		2 -> 00000100
 *		3 -> 00001000
 *		4 -> 00010000
 *		5 -> 00100000
 *		6 -> 01000000
 *		7 -> 10000000
 *
 *		value:
 *			0 or 1
 *
 */
void write_flag_bit (int register_nr, int bit, int value);


/*********************************************************************
 *
 *  error = read_ossd ( *ossd_data );
 *
 *  description:
 *		reads all ossd's
 *
 *  parameters:
 *		ossd_data: int array [64]

 *		ossd_data[0] = ossd 1
 *		ossd_data[1] = ossd 2
 *		ossd_data[2] = ossd 3
 *		*
 *		*
 *		*
 *		*
 *		ossd_data[63] = ossd 64
 *
 *		Results:
 *		0x00 = green permanent
 *		0x01 = green flashing
 *		0x02 = yellow permanent
 *		0x03 = yellow flashing
 *		0x04 = red permanent
 *		0x05 = red flashing
 *		0x06 = grey/off
 *		0x07 = green/yellow
 *
 */
void read_ossd (int *ossd_data);



