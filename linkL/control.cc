//Compilacion de este archivo C++
//mkoctfile --link-stand-alone control.cc perifericos.o -o control

#include <iostream>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>

extern "C"{
	#include "perifericos.h"
}

int main (void)
{
  string_vector argv (2);
  argv(0) = "embedded";
  argv(1) = "-q";

  octave_main (2, argv.c_str_vec (), 1);

  float kd  = 0;
  float kp  = 27.392;
  float ki  = 602.9173;
  float ref = 3.0;
  float sensor =0;
  int i,j;
  int valor_actual = 0;
  int canalADC = 0;

  adc_init();
  Matrix num = Matrix (1,3);
  Matrix den = Matrix(1,2);
  Matrix t = Matrix(1,3);
  Matrix error = Matrix(1,3);
  Matrix resultado = Matrix(3,1);
  num(0,0) = kd; num(0,1)=kp; num(0,2)=ki;
  den(0,0) = 1.0; den(0,1)=0.0;
  t(0,0)=0.0; t(0,1)=0.1; t(0,2)=0.2;
  error(0,0)=3.3; error(0,1)=3.3; error(0,2)=3.3;

  std::cout << num;
  std::cout << den;
  std::cout << t;
  std::cout << error;

  octave_value_list in;
  in(0) = num; in(1)=den;

  octave_value_list C = feval("tf",in,2);

  if(error_state)
  {
	std::cout << "Error creando la funcion de transferencia";
	clean_up_and_exit (0);
  }

  octave_value_list entrada;

  entrada(0)=C(0); entrada(1)=error; entrada(2)=t;

  for(i=0;i<1000;i++)
  {
	sensor = adc_value(canalADC);
	error(0,0) = ref-sensor;
	error(0,1) = ref-sensor;
	error(0,2) = ref-sensor;
	entrada(1) = error;
//	std::cout 	<< "Error: "
//			<< ref - sensor;
  	octave_value_list salida = feval("lsim",entrada,3);

  	if(error_state)
  	{
        	std::cout 	<< "Error en lsim"
				<< std::endl;
        	clean_up_and_exit (0);
  	}

  	resultado = salida(0).matrix_value();
	valor_actual=valor_actual+resultado(0,0);

        if(valor_actual > 100)
		valor_actual=100;
	else if(valor_actual < 0)
		valor_actual=0;

	pwm_value(valor_actual);
	std::cout << "Iteracion "
              << i
              << ", Valor PID "
              << resultado(0,0)
              << ", Ajuste " << valor_actual
              << std::endl;

  }


  clean_up_and_exit (0);
}
