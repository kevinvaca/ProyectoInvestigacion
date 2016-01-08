//COMPILACION DE LIBRERIAS EN C, NECESARIAS
//gcc -I/root/hps_lib/hwlib/include/ -fPIC -c perifericos.c
#ifdef __cplusplus
extern "C" {
#endif
	#ifndef SOMECODE_H_
	#define SOMECODE_H_

	float adc_value(int channel);
	int   adc_init(void);
	void  pwm_value(int porcentaje);
	#endif
#ifdef __cplusplus
}
#endif
