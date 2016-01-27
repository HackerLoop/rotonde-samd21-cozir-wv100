/*
 * cozir_wv_100c.c
 *
 * Created: 21/01/2016 20:31:11
 *  Author: GroundLayer
 */ 

#include "./cozir_wv_100.h"
#include "string.h"

static struct usart_module COZIR_WV_100_uart;
static char buffer[50];
static uint16_t get_char_buffer;
static int index_data;

bool COZIR_WV_100_init(void){
	struct usart_config usart_conf;
	
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = USART_RX_3_TX_2_XCK_3;
	usart_conf.pinmux_pad0 = PINMUX_UNUSED;
	usart_conf.pinmux_pad1 = PINMUX_UNUSED;
	usart_conf.pinmux_pad2 = PINMUX_PA18C_SERCOM1_PAD2;
	usart_conf.pinmux_pad3 = PINMUX_PA19C_SERCOM1_PAD3;
	usart_conf.baudrate    = 9600;

	while (usart_init(&COZIR_WV_100_uart, SERCOM1, &usart_conf) != STATUS_OK ) {
		//add a timeout if blocked here
	}
	usart_register_callback(&COZIR_WV_100_uart, COZIR_WV_100_buffer_received , USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&COZIR_WV_100_uart, USART_CALLBACK_BUFFER_RECEIVED);

	usart_enable(&COZIR_WV_100_uart);
	usart_read_job(&COZIR_WV_100_uart , &get_char_buffer);
	//usart_write_buffer_wait(&COZIR_WV_100_uart  ,"*\r\n" , 3);
	return true;
}

bool COZIR_WV_100_read(float* CO2 , float* temp , float* humi){
	
	if(index_data < 24){
		return false;
	}
	
	int i = index_data -1;
	while(buffer[i] != '\n' && buffer[i-1] != '\r' && i >= 23){
		i--;
	}
	
	if(buffer[i-24] != 'H' || buffer[i-16] != 'T' || buffer[i-8] != 'Z' || buffer[i-1] != '\r' || buffer[i] != '\n'){
		memset(buffer , 0 , 50);
		index_data = 0;
		usart_read_job(&COZIR_WV_100_uart , &get_char_buffer);
		return false;
	}
	
	int zero = i -24;
	char temporary[5];
	strncpy(temporary , buffer + zero + 2 , 5);
	*humi = atof(temporary);
	*humi /= 10;
	strncpy(temporary , buffer + zero + 10 , 5);
	*temp = atof(temporary);
	*temp -= 1000;
	*temp /= 10;
	strncpy(temporary , buffer + zero + 18 , 5);
	*CO2 = atof(temporary);
	memset(buffer , 0 , 50);
	index_data = 0;
	usart_read_job(&COZIR_WV_100_uart , &get_char_buffer);
	return true;
}

void COZIR_WV_100_buffer_received(struct usart_module *const module){
	buffer[index_data] = (char) get_char_buffer;
	if(index_data < 49){
		index_data++;
		usart_read_job(&COZIR_WV_100_uart , &get_char_buffer);
	}
}