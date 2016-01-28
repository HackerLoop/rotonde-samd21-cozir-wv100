/*
 * cozir_wv_100.h
 *
 * Created: 21/01/2016 20:31:19
 *  Author: GroundLayer
 */ 


#ifndef COZIR_WV_100_H_
#define COZIR_WV_100_H_

#include "asf.h"

bool COZIR_WV_100_init(void);
bool COZIR_WV_100_read(float* CO2 , float* temp , float* humi);
usart_callback_t COZIR_WV_100_buffer_sent(void);
void COZIR_WV_100_buffer_received(struct usart_module *const module);



#endif /* COZIR_WV_100_H_ */