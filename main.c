
/***********************************************************************************************
 * program to test adc features of board
 *
 **********************************************************************************************/
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit_kl26z.h"
#include "stdio.h"
#include "adc.h"

void adc_config_with_software_trigger();

int main(void)
{

    //Initialise the FRDM-KL26Z Board
	hardware_init();
	FRDM_KL26Z_LEDs_Configure();
	PIT_Configure_interrupt_mode(2);
	PRINTF("ADC  new TESTING\r\n");
	adc_config_with_software_trigger();

    while(1) {
    	//enable adc clock SIM_SCGC6 bit 27 for ADC0
    	//configure adc trigger in SIM_OPT7 if using h/w triggers
    	//configure ADC1_CFG1 to configure clock and # of conversion bits
    	//configure ADC1_SC2 for h/w s/w trigger
    	//write to ADC1_SCA register to configure interrupt, channel and source type
    }
    /* Never leave main */
    return 0;
}

void adc_config_with_software_trigger()
{
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	NVIC_ClearPendingIRQ(15);
	NVIC_EnableIRQ(15);

	// configure ADC registers
	ADC0_CFG1 = 0x7cu; // clock div = 8 16 bit conversion
	ADC0_CFG2 = 0; //default for lowspeed conversion
	ADC0_SC2 = 0; //sw trigger
	ADC0_SC3 = 0; // no calibration

	//start conversion
	ADC0_SC1A = 0x4Fu; // interrupt enabled single ended input on channel DAD3

}

void ADC0_IRQHandler()
{
	char sample;
	sample = ADC0_RA; // read result register to clear interrupt flag
	//float x = sample *3.3;
	//float y = x/65536;
	//float z = 25 - ((y - 0.716)/0.00162);
	PRINTF("\rSample is %04x\n",sample);
	//char temp;
	//temp = read_temperature();
	//PRINTF("\rChip Temp is %d\n",((unsigned int)temp));
}

void PIT_IRQHandler()
{
	PIT_TFLG0 = 0x01ul; // clear interrupt
	//write to ADC1_SC1A to initiate conversion
	ADC0_SC1A = 0x4Fu;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
