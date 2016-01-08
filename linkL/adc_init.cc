//PARA COMPILAR ES NECESARIO
// mkoctfile adc_init.cc perifericos.o

#include <octave/oct.h>

extern "C"
{
	#include "perifericos.h"
}  /* end extern "C" */

DEFUN_DLD (adc_init, args, , "Habilita el ADC")
{
  int nargin = args.length ();
  int canal=0;
  octave_value_list resultado;

  resultado(0)=adc_init();
  return resultado;
}
