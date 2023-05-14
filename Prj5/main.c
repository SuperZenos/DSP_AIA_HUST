#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

interrupt void tim0_isr(void);
interrupt void ADC_convered(void);

Uint16 ADCINA4_Voltage_sum = 0;
Uint16 ADCINB4_Voltage_sum = 0;
Uint16 ADCINA4_Voltage = 0;
Uint16 ADCINB4_Voltage = 0;
Uint16 convered_count = 0;

void main(void)
{

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2802x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2802x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example


// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2802x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2802x_DefaultIsr.c.
// This function is found in DSP2802x_PieVect.c.
   InitPieVectTable();


// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2802x_InitPeripherals.c
// InitPeripherals(); // Not required for this example

// Step 5. User specific code:

   InitAdc();

   EALLOW;

   AdcRegs.ADCSAMPLEMODE.bit.SIMULEN0 = 1; //ͬʱ����
   AdcRegs.ADCSOC0CTL.bit.CHSEL = 4;       //socͨ��ѡ��
   AdcRegs.ADCSOC1CTL.bit.CHSEL = 12;
   AdcRegs.ADCSOC0CTL.bit.ACQPS = 6;     //����ʱ��
   AdcRegs.ADCSOC1CTL.bit.ACQPS = 6;
   AdcRegs.ADCSOC0CTL.bit.TRIGSEL = 1;    //soc����ѡ��,TIM0
   AdcRegs.ADCCTL1.bit.INTPULSEPOS  = 1;  //�������Ĵ����Ų����ж�

   PieVectTable.ADCINT1 = &ADC_convered;
   AdcRegs.INTSEL1N2.bit.INT1SEL = 1;    //�ж���1ѡ��soc1
   AdcRegs.INTSEL1N2.bit.INT1CONT  = 0;
   AdcRegs.INTSEL1N2.bit.INT1E  = 1;    //�ж�ʹ��

   PieCtrlRegs.PIEIER1.bit.INTx1 = 1;   //ʹ��int1.1
   EDIS;

/****************���ö�ʱ�������Դ���ADC*****************/
   CpuTimer0Regs.TPR.bit.TDDR = 59;
   CpuTimer0Regs.TPRH.bit.TDDRH = 0; //������ʱ��60��Ƶ��60M/60=1M
   CpuTimer0Regs.PRD.all = 500000;//��ʱ0.5s
   CpuTimer0Regs.TCR.bit.TRB = 1; //reload
   CpuTimer0Regs.TCR.bit.TIE = 1; //ʹ���ж�
   CpuTimer0Regs.TCR.bit.TSS = 0; //��ʼ����

   EALLOW;
   PieVectTable.TINT0 = &tim0_isr;
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   //ʹ��PIE
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;   //ʹ��int1.7
   IER |= 0x0001;//ʹ��GROUP1
   EINT;
   EDIS;

   while(1)
   {
   };
}

interrupt void ADC_convered(void)
{
    ADCINA4_Voltage_sum += AdcResult.ADCRESULT0;
    ADCINB4_Voltage_sum += AdcResult.ADCRESULT1;
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    convered_count++;
    /*********ת��16�Σ�ȡƽ��ֵ*********/
    if(convered_count > 15)
    {
        ADCINA4_Voltage = ADCINA4_Voltage_sum >> 4;//�൱�ڳ���16
        ADCINB4_Voltage = ADCINB4_Voltage_sum >> 4;
        ADCINA4_Voltage_sum = 0;
        ADCINB4_Voltage_sum = 0;
        convered_count = 0;
    }
}

interrupt void tim0_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
