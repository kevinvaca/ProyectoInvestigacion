extern "C" {
  #include "perifericos.h" //a C header, so wrap it in extern "C" 
}

int main()
{
	int i;
	adc_init();
	for(i=0;i<500;i++)
		adc_value(0);
	return 0;
}

