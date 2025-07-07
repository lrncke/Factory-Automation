/*-------------------------------------------------------------------*
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Control_III                                        *
 *   module name: main.c                                             *
 *        author: Christian Sommerfeld                               *
 *          date: 2015-02-26                                         *
 *                                                                   *
 *  version: 1.0 first version                                       *
 *                                                                   *
 *                                                                   *
                                                 *
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*
 *  include files                                                    *
 *-------------------------------------------------------------------*/
#include "control.h"
#include <string.h>
#include "control_io.h"
#include "enet.h"


/*-------------------------------------------------------------------*
 *  local definitions                                                *
 *-------------------------------------------------------------------*/

#define BANDENDE_LINKS 	0x01
#define BANDENDE_RECHTS 0x01<<4
#define START_BUTTON 0x01<<7
#define BAND_RECHTSLAUF 0x01<<1
#define LAMPE_ROT 0x01<<3
#define LAMPE_GRUEN 0x01<<7

/*-------------------------------------------------------------------*
 *  external declarations                                            *
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*
 *  public data                                                      *
 *-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*
 *  private data                                                     *
 *-------------------------------------------------------------------*/

static unsigned short system_ticks;
static unsigned short end_timer;

/*-------------------------------------------------------------------*
 *  private functions                                                *
 *-------------------------------------------------------------------*/

static void timer_function ( void )
{
        /* timer interrupt every 10 ms */
        system_ticks++;
}

/*-------------------------------------------------------------------*
 *  public functions                                                 *
 *-------------------------------------------------------------------*/

int main ( void )
{
        //initialization of the Debugger
        //cctrl_func.CCtrlBreakpoint();

        unsigned char                ctrl_flags;
        unsigned char                asi5_accodi[10];//Länge entspricht der Anzahl der in der Anlage vorhandenen logischen Adressen
        unsigned char                asi5_odi[10];//Ausgänge die gesetzt werden sollen (Hex-Werte)

        unsigned char               asi5_idi[10];//Eingänge die gelesen werden sollen (Hex-Werte)

        unsigned short 				length 			= 2;
        int zustand = 1;
        /* Access ASi5 logical Slave 1 (1 Byte / Slave) */
        asi5_accodi[0] = 0xFF;
        asi5_accodi[1] = 0xFF;
        asi5_accodi[2] = 0xFF;
		asi5_accodi[3] = 0xFF;
		asi5_accodi[4] = 0xFF;
		asi5_accodi[5] = 0xFF;
		asi5_accodi[6] = 0xFF;
		asi5_accodi[7] = 0xFF;
		asi5_accodi[8] = 0xFF;
		asi5_accodi[9] = 0xFF;

        cctrl_func.Asi5WriteCtrlAccODI(0, asi5_accodi, 2, 1);
        cctrl_func.Asi5WriteCtrlAccODI(0, &asi5_accodi[2], 2, 3);
        cctrl_func.Asi5WriteCtrlAccODI(0, &asi5_accodi[4], 2, 5);
        cctrl_func.Asi5WriteCtrlAccODI(0, &asi5_accodi[6], 2, 7);
        cctrl_func.Asi5WriteCtrlAccODI(0, &asi5_accodi[8], 2, 9);

        /* init timer function with 10ms ticks */
        cctrl_func.CCtrlInitTimer ( 1, timer_function );

        /* init watchdog */
        //cctrl_func.CCtrlInitWdg( 10 );

    	asi5_odi[0] = 0x00; 									//alle ausgänge 0 setzen
    	asi5_odi[1] = 0x00;
    	asi5_odi[2] = 0x00;
    	asi5_odi[3] = 0x00;
    	asi5_odi[4] = 0x00;
    	asi5_odi[5] = 0x00;
    	asi5_odi[6] = 0x00;
    	asi5_odi[7] = 0x00;
    	asi5_odi[8] = 0x00;
    	asi5_odi[9] = 0x00;
		cctrl_func.Asi5WriteASi5Odi(0, asi5_odi, 2, 1);			// odi schreiben
		cctrl_func.Asi5WriteASi5Odi(0, &asi5_odi[2], 2, 3);			// odi schreiben
		cctrl_func.Asi5WriteASi5Odi(0, &asi5_odi[4], 2, 5);
		cctrl_func.Asi5WriteASi5Odi(0, &asi5_odi[6], 2, 7);
		cctrl_func.Asi5WriteASi5Odi(0, &asi5_odi[8], 2, 9);

		//int flag =


        for(;;)
        {

                /* trigger watchdog */
                //cctrl_func.CCtrlTriggerWdg();

                /* Define data exchange for AS-i Circuit 1 and 2*/


                //Timer 1 100 * 10ms = 1sec.
                if ( ((unsigned short)(system_ticks - end_timer)) > 50)
                {
                        end_timer = system_ticks;

                        cctrl_func.Asi5ReadASi5Idi(0, asi5_idi, &length, 1);			// es werden 2 bytes von bwu8334 gelesen


                        //Zustandsübergänge
                        if(zustand == 1 && ((asi5_idi[0]) & (START_BUTTON)) && ((asi5_idi[0]) & (BANDENDE_LINKS))){
                        	zustand = 2;
                        }
                        if(zustand == 2 && ((asi5_idi[0]) & ((BANDENDE_RECHTS)))){
                        	zustand = 1;
                        }

                        //Ausgaben
                        if(zustand == 1){
							asi5_odi[1] = LAMPE_GRUEN;
							cctrl_func.Asi5WriteASi5Odi(0, &asi5_odi[0], 2, 1);			// odi schreiben
                        }
                        if(zustand == 2){
                    		asi5_odi[1] = BAND_RECHTSLAUF | LAMPE_ROT;
                    		cctrl_func.Asi5WriteASi5Odi(0, &asi5_odi[0], 2, 1);			// odi schreiben
                        }

                }

                /* to check Cycletime */
                cctrl_func.CCtrlEvalCycletime();

                /*read flags if we should stop control*/
                cctrl_func.CCtrlReadFlags( &ctrl_flags );
                if ( !( ctrl_flags & CCTRL_FLAG_RUN ) )
                {
                        return 1;
                }

        }
}
/*-------------------------------------------------------------------*
 *  eof                                                              *
 *-------------------------------------------------------------------*/
//E79 -> Fehler im Programm, Variable nicht definiert oder ähnliches
