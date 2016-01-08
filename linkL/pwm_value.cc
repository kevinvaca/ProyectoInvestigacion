#include <octave/oct.h>
extern "C"{
	#include "perifericos.h"
}

DEFUN_DLD (pwm_value, args, , "Ajuste el ciclo de trabajo de los pulsos PWM")
{
  int nargin = args.length ();
  int porcentaje=0;
  octave_value valor=0;

  if (nargin != 1)
    print_usage ();
  else
    {
      	porcentaje = args(0).int_value ();
	if(porcentaje > 100)
		porcentaje = 100;
	else if (porcentaje < 0)
		porcentaje = 0;
	pwm_value(porcentaje);
	return octave_value( 0 );
    }

  return octave_value_list ();
}
