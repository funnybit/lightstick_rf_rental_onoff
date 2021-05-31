/*******************************************************************************
1. Project Name     : 
2. Author	        : 
3. Company	        : 
4. A Drafter	    : Christopher, Lee ( DongSeok, Lee )
5. Homepage	        : 
6. Email	        : strchrislee@gmail.com
7. Phone Number     : 010-2464-0102
8. Filename	        : UserDataFlash.c
9. Version	        : 1.0
10. CreatedDate     : 2017/06/27
11. ModifiedDate	: 
12. License	        : Heumtech
13. MCU Type	    :
14. Clock Frequency : 
15. Add Source Name :
16. Compiler	    : 
17. SubMenu	        :
*******************************************************************************/
//Defined Device Header
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "Function_define.h"
#include "SFR_Macro.h"

static bit BIT_TMP;

//Defined User Header

#include "UserDataFlash.h"
#include "UserDefine.h"

//Define variables
DataFlash_t  DataFlash;   

/*
#define BASE_ADDR   0x4700

#define FLASH_CALIBRATION_L_LOW     BASE_ADDR+1     //������ Ķ���극�̼� LOW BYTE
#define FLASH_CALIBRATION_L_HIGH    BASE_ADDR+1     //������ Ķ���극�̼� HIGH BYTE
#define FLASH_CALIBRATION_H_LOW     BASE_ADDR+3     //������ Ķ���극�̼� LOW BYTE
#define FLASH_CALIBRATION_H_HIGH    BASE_ADDR+4     //������ Ķ���극�̼� HIGH BYTE
*/


/*----------------------------------------------------------------------*/
/*Dataflash use APROM area, please ALWAYS care the address of you code  */
/*APROM 0x3800~0x38FF demo as dataflash 				      			*/
/*Please use Memory window key in C:0x3800 to check earse result		*/
/*----------------------------------------------------------------------*/

/******************************************************************************************************************/
/*write_DATAFLASH_BYTE :*/
/*user can copy all this subroutine into project, then call this function in main.*/
/******************************************************************************************************************/
void write_DATAFLASH_BYTE(UINT16 u16_addr,UINT8 u8_data)
{
	UINT8 looptmp=0,u8_addrl_r;
	unsigned char code *cd_longaddr;
	unsigned char xdata *xd_tmp;
	
//Check page start address
	u8_addrl_r = u16_addr;
	if (u8_addrl_r<0x80)
	{
		u8_addrl_r = 0;
	}
	else 
	{
		u8_addrl_r = 0x80;
	}
//Save APROM data to XRAM
	xd_tmp = 0x80;
	cd_longaddr = (u16_addr&0xff00)+u8_addrl_r;	
	while (xd_tmp !=0x100)
	{
		*xd_tmp = *cd_longaddr;
		looptmp++;
		xd_tmp++;
		cd_longaddr++;
	}
// Modify customer data in XRAM
	u8_addrl_r = u16_addr;
	if (u8_addrl_r<0x80)
	{
		xd_tmp = u8_addrl_r+0x80;
	}
	else
	{
		xd_tmp = u8_addrl_r+0;
	}
	*xd_tmp = u8_data;
//Erase APROM DATAFLASH page
		IAPAL = u16_addr;
		IAPAH = u16_addr>>8;
		IAPFD = 0xFF;
	  set_IAPEN; 
		set_APUEN;
    IAPCN = 0x22; 		
 		set_IAPGO; 
//Save changed RAM data to APROM DATAFLASH
	u8_addrl_r = u16_addr;
	if (u8_addrl_r<0x80)
	{
		u8_addrl_r =0;
	}
	else
	{
		u8_addrl_r = 0x80;
	}
		xd_tmp = 0x80;
	  IAPAL = u8_addrl_r;
    IAPAH = u16_addr>>8;
		set_IAPEN; 
		set_APUEN;
	  IAPCN = 0x21;
		while (xd_tmp !=0xFF)
		{
			IAPFD = *xd_tmp;
			set_IAPGO;
			IAPAL++;
			xd_tmp++;
		}
		clr_APUEN;
		clr_IAPEN;
}	
	
//-------------------------------------------------------------------------
UINT8 read_APROM_BYTE(UINT16 code *u16_addr)
{
	UINT8 rdata;
	rdata = *u16_addr>>8;
	return rdata;
}
