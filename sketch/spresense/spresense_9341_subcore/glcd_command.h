/*
 * glcd_command.h
 *
 *  Created on: 2017/11/23
 *      Author: Takashi
 */

#ifndef GLCD_QUE_H_
#define GLCD_QUE_H_

#include "stdint.h"

//�J���[�O���t�B�b�NLCD
#define	GLCDCMD_INIT			0x0000
#define	GLCDCMD_PSET			0x0100
#define	GLCDCMD_LINE			0x1000
#define	GLCDCMD_VLINE			0x1100
#define	GLCDCMD_HLINE			0x1200
#define	GLCDCMD_RECT			0x2000
#define	GLCDCMD_RECT_FILL		0x2100
#define	GLCDCMD_BITBLT			0x3000
#define	GLCDCMD_PRINT_STRING	0x4000
#define	GLCDCMD_PRINT_STRING_ADA	0x4100

typedef struct{
	uint16_t 	CMD;
	uint16_t 	XS;		//
	uint16_t 	YS;		//
	uint16_t 	XE;		//
	uint16_t 	YE;		//
	uint16_t	COLOR;	//
	uint16_t 	FONT_SEL;
	char 		*print_str;
} GLCD_CMD;


#endif /* GLCD_QUE_H_ */
