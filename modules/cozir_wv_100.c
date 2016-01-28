/*
 * cozir_wv_100.c
 *
 * Created: 21/01/2016 20:41:30
 *  Author: GroundLayer
 */ 


#include "modules/drivers/cozir_wv_100.h"
#include "modules/modules/cozir_wv_100.h"

#include "registrar/registrar.h"
#include "tasks/modules.h"

#define MODULE_NAME "COZIR_WV_100_CO2"
#define MODULE_DATA_SIZE 1
#define MODULE_DATA "co2"

data_name COZIR_WV_names[MODULE_DATA_SIZE] = { MODULE_DATA  };
data_type COZIR_WV_types[] = { FLOAT};
	
	void COZIR_WV_tick(void);
	definition *COZIR_WV_def;

void COZIR_WV_10_module_init(void) {
	COZIR_WV_100_init();
	COZIR_WV_def = register_definition(EVENT , MODULE_NAME , COZIR_WV_names , COZIR_WV_types , MODULE_DATA_SIZE , NULL, COZIR_WV_tick , 1000);
}

void COZIR_WV_tick(void) {
	float CO2 , temp , humi;
	
	if(COZIR_WV_100_read(&CO2 , &temp , &humi)) {
		instance inst = create_instance(MODULE_NAME);
		if(inst.def != NULL && inst.values != NULL){
			(*(float*) inst.values[0]) = CO2;
			portBASE_TYPE xStatus = xQueueSendToBack(module_out, &inst , 10 / portTICK_RATE_MS );
			if(xStatus != pdPASS){
				release_instance(inst);
			}
		}
	}
}