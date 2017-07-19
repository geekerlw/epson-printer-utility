/*_______________________________  epson-escpr-closed.h   ______________________________*/

/*       1         2         3         4         5         6         7         8        */
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*******************************************|********************************************/
/*
 *   Copyright (c) 2009  Seiko Epson Corporation                 All rights reserved.
 *
 *   Copyright protection claimed includes all forms and matters of
 *   copyrightable material and information now allowed by statutory or judicial
 *   law or hereinafter granted, including without limitation, material generated
 *   from the software programs which are displayed on the screen such as icons,
 *   screen display looks, etc.
 *
 */
/*******************************************|********************************************/
/*                                                                                      */
/*                    Epson ESC/PR Application Programming Interfaces                   */
/*                                                                                      */
/*******************************************|********************************************/
#ifndef __EPSON_ESCPR_CLOSED_H__
#define __EPSON_ESCPR_CLOSED_H__
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include "epson-typedefs.h"
#include "epson-escpr-pvt.h"

/*----------------------------------  Generic Macros   ---------------------------------*/
/*******************************************|********************************************/
#ifdef EPS_PRV_COMMANDS
#undef EPS_PRV_COMMANDS
#endif
#define EPS_PRV_COMMANDS(p1, p2, p3)	clsAddPrvCommands(p1, p2, p3);

/*-----------------------------------  Definitions  ------------------------------------*/
/*******************************************|********************************************/
	/* Platform  */
#define EPS_PF_STANDALON		0x00	/* Reserved (stand alone)                       */
#define EPS_PF_WINDOWS			0x02	/* Windows                                      */
#define EPS_PF_MAC				0x03	/* Mac                                          */
#define EPS_PF_LINUX			0x04	/* Linux                                        */
#define EPS_PF_EPSON_CONNECT	0x05	/* Epson Connect                                */
#define EPS_PF_PS3				0x06	/* PlayStation 3                                */
#define EPS_PF_IPRINT_IOS		0x07	/* iPrint iOS                                   */
#define EPS_PF_IPRINT_ANDROID	0x08	/* iPrint Android                               */
#define EPS_PF_OTHER_IOS		0x09	/* other iOS                                    */
#define EPS_PF_OTHER_ANDROID	0x0A	/* other Android                                */
#define EPS_PF_SAMSUNG			0x0B	/* Samsung                                      */	
#define EPS_PF_OTHER			0xFF	/* Other IA device, Windows CE                  */


/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*--------------------------------  Global Declarations   ------------------------------*/
/*******************************************|********************************************/

/*----------------------------  API Function Declarations   ----------------------------*/
/*******************************************|********************************************/

extern EPS_ERR_CODE epsSetPlatform       (EPS_UINT8                                     );
extern EPS_ERR_CODE epsInfoCommand       (EPS_INT32, EPS_UINT8*, EPS_INT32*             );
extern void         epsSetNonCheckPrintMode(EPS_BOOL                                    );

extern EPS_ERR_CODE clsAddPrvCommands    (EPS_UINT8**, EPS_UINT8**, EPS_UINT32*         );


#ifdef __cplusplus
}
#endif

#endif  /* def __EPSON_ESCPR_CLOSED_H__ */

/*_______________________________  epson-escpr-closed.h   ______________________________*/
  
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
