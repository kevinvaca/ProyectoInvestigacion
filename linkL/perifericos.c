#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"
#include "perifericos.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define CHANNEL1 	   0x00001
//FUNCIONA PARA EL ADC LEER EN CUALQUIER PUERTO
//			ADC_BASE + 0X04	<== 1 PARA ACTIVAR LA CONVERSION A-D EN TODOS LOS CANALES
//OFFSET LECTURA: 	ADC_BASE 		CH0
//					ADC_BASE + 0X04	CH1
//					ADC_BASE + 0X08	CH2
//					ADC_BASE + 0X0C	CH3
//					ADC_BASE + 0X10	CH4
//					ADC_BASE + 0X14	CH5
//					ADC_BASE + 0X18	CH6
//					ADC_BASE + 0X1C	CH7

float adc_value(int channel) {

	void *virtual_base;
	int fd;
	void *h2p_lw_adc_addr;
	int value;
	int offset=0;

	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span

	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ) , MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	switch(channel)
	{
		case 0:	offset = 0;
			break;
		case 1: offset = 0x04;
			break;
		case 2: offset = 0x08;
			break;
		case 3: offset = 0x0C;
			break;
		case 4: offset = 0x10;
			break;
		case 5: offset = 0x14;
			break;
		case 6: offset = 0x18;
			break;
		case 7: offset = 0x1C;
			break;
		default:
			offset = 0;
			break;
	}

	h2p_lw_adc_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST +  DE1_SOC_ADC_0_BASE + offset ) & ( unsigned long)( HW_REGS_MASK ) );
	value=*(long int *)(h2p_lw_adc_addr);
	printf("CH 0=%.3fV (0x%04x)\r\n", (float)value/1000.0, value);

	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );

	return (float)value/1000.0;
}

int adc_init(void)
{
	void *virtual_base;
	int fd;
	void *h2p_lw_adc_addr;

	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span

	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ) , MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	printf("ADC ON\n");

	h2p_lw_adc_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST +  DE1_SOC_ADC_0_BASE + 0x00000004 ) & ( unsigned long)( HW_REGS_MASK ) );
	*(uint32_t *)h2p_lw_adc_addr=(uint32_t)0x01;
	usleep(100);

	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );

	return 0;
}

void  pwm_value(int porcentaje)
{
	void *virtual_base;
	int fd;
	long int value;
	int i;
	void *h2p_lw_pwm_addr;

	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span

	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return ;
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return ;
	}

	h2p_lw_pwm_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PWM_GENERATOR_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	//SE ESCRIBE DIRECTAMENTE SOBRE EL REGISTRO BASE EL VALOR ENTRE 0 Y 100 EL PORCENTAJE DEL CICLO DE TRABAJO
	//EL OFFSET NO ES NECESARIO POR USAR OFFSET: 0X00

	if(porcentaje > 100)
		value = 100;
	else if ( porcentaje < 0)
		value = 0;
	else
		value=porcentaje;
	*(uint32_t *)h2p_lw_pwm_addr = value;

	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return ;
	}

	close( fd );
	return ;
}
