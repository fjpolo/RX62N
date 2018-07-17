/*
 * led.c
 *
 *  Created on: 10.07.2018
 *      Author: fpolo
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "led.h"
#include "iodefine.h"
#include "yrdkrx62ndef.h"

void led__toggle(uint8_t port){

	switch(port){
	case 0:
		LED0 = !LED0;
		break;
	case 1:
		LED1 = ! LED1;
		break;
	case 2:
		LED2 = !LED2;
		break;
	case 3:
		LED3 = !LED3;
		break;
	default:
		break;
	}

}
