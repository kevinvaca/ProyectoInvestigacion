#include <octave/oct.h>
extern "C"{
	#include "perifericos.h"
}

DEFUN_DLD (adc_value, args, , "Obtiene el valor del ADC")
{
  int nargin = args.length ();
  int canal=0;
  octave_value valor=0;

  if (nargin != 1)
    print_usage ();
  else
    {
      canal = args(0).int_value ();
      return octave_value( adc_value(canal));
    }

  return octave_value_list ();
}
