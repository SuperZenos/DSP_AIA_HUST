/*
 * main.c
 */
#include "DSP28x_Project.h"
#include "LED_TM1638.h"

// interrupt void cpu_timer0_isr(void);  //timer0
// interrupt void myXint1_isr(void);     //xint1
// interrupt void EPWM4Int_isr(void);    //EPWM4
// interrupt void Ecap1Int_isr(void);    //ECAP1
// interrupt void MyAdcInt1_isr(void);   //ADCINT1

void HorseRunning1(int16 no);
void HorseRunning2(int16 no);
interrupt void myXint1_isr(void);
interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
interrupt void Ecap1Int_isr(void);
interrupt void EPWM1Int_isr(void);
interrupt void MyAdcInt1_isr(void);


#define Led0Blink() GpioDataRegs.GPACLEAR.bit.GPIO0 = 1
#define Led1Blink() GpioDataRegs.GPACLEAR.bit.GPIO1 = 1
#define Led2Blink() GpioDataRegs.GPACLEAR.bit.GPIO2 = 1
#define Led3Blink() GpioDataRegs.GPACLEAR.bit.GPIO3 = 1
#define Led0Blank() GpioDataRegs.GPASET.bit.GPIO0 = 1
#define Led1Blank() GpioDataRegs.GPASET.bit.GPIO1 = 1
#define Led2Blank() GpioDataRegs.GPASET.bit.GPIO2 = 1
#define Led3Blank() GpioDataRegs.GPASET.bit.GPIO3 = 1

#define EPWM1_MAX_DB   0x03FF
#define EPWM1_MIN_DB   0

int HorseRunningAction = 0;
int KeyDLTime = 0;

Uint16  EPwm1_DB_Direction;

void  Xint1_Init()
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;                        /*common GPIO*/
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;                         /*set to intput*/
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;                         /*pull up*/
    GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;                       //clock timer
    GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 12;                   ///GPIO12��Ϊxint1���ж�Դ

    XIntruptRegs.XINT1CR.bit.POLARITY = 3;
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;
    EDIS;
}

void HorseIO_Init()
{
	EALLOW;
    GpioDataRegs.GPASET.bit.GPIO0 = 1;
    GpioDataRegs.GPASET.bit.GPIO1 = 1;
    GpioDataRegs.GPASET.bit.GPIO2 = 1;
    GpioDataRegs.GPASET.bit.GPIO3 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
    EDIS;
	
}

void ECap_Init(void)
{
    ECap1Regs.ECEINT.all = 0x0000;             // Disable all capture interrupts
    ECap1Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags
    ECap1Regs.ECCTL1.bit.CAPLDEN = 0;          // Disable CAP1-CAP4 register loads
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;        // Make sure the counter is stopped

    // Configure peripheral registers
    ECap1Regs.ECCTL2.bit.CONT_ONESHT = 1;      // One-shot
    ECap1Regs.ECCTL2.bit.STOP_WRAP = 3;        // Stop at 4 events
    ECap1Regs.ECCTL1.bit.CAP1POL = 1;          // Falling edge
    ECap1Regs.ECCTL1.bit.CAP2POL = 0;          // Rising edge
    ECap1Regs.ECCTL1.bit.CAP3POL = 1;          // Falling edge
    ECap1Regs.ECCTL1.bit.CAP4POL = 0;          // Rising edge
    ECap1Regs.ECCTL1.bit.CTRRST1 = 1;          // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST2 = 1;          // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST3 = 1;          // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST4 = 1;          // Difference operation
    ECap1Regs.ECCTL2.bit.SYNCI_EN = 1;         // Enable sync in
    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;        // Pass through
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable capture units

    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;        // Start Counter
    ECap1Regs.ECCTL2.bit.REARM = 1;            // arm one-shot
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable CAP1-CAP4 register loads
    ECap1Regs.ECEINT.bit.CEVT4 = 1;            // 4 events = interrupt
}

void EPwm1_Init(void)
{
    /*EPwm1Regs.TBPRD = 2000;
    EPwm1Regs.TBPHS.all = 0;
    EPwm1Regs.TBCTR = 0;
    EPwm1Regs.CMPA.half.CMPA = 800;
    EPwm1Regs.CMPB = 1200;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV2;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.CBU = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CBD = AQ_CLEAR;
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;*/
    EPwm1Regs.TBPRD = 6000;                        // Set timer period
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                      // Clear counter

    // Setup TBCLK
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;       // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV4;
    /***�ȽϼĴ���***/
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Setup compare
    EPwm1Regs.CMPA.half.CMPA = 3000;
    /***�����޶��Ĵ���***/
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM1A on Zero
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;

    EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;          // Set PWM1A on Zero
    EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;
    // Active Low PWMs - Setup Deadband
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;     //��ȫʹ��deadband
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;//DB_ACTV_LO;
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    //00   EPWMxA In�����Զ����޶��������½����ӳٺ��������ӳٵ�Դ
    //01   EPWMxB In�����Զ����޶��������������ӳ��źŵ�Դ
    //     EPWMxA In�����Զ����޶��������½����ӳ��źŵ�Դ
    //10   EPWMxA In�����Զ����޶��������������ӳ��źŵ�Դ
    //     EPWMxB In�����Զ����޶��������½����ӳ��źŵ�Դ
    //11   EPWMxB In�����Զ����޶��������������ӳٺ��½����ӳ��źŵ�Դ
    EPwm1Regs.DBRED = EPWM1_MIN_DB;//�ӳټ�������Ƶ�ʿ�ѡΪʱ��Ƶ�ʵ�2����1��
    EPwm1Regs.DBFED = EPWM1_MIN_DB;
    EPwm1_DB_Direction = EPWM1_MIN_DB;
    /***�¼��������������жϣ�soc***/
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;           // �¼�Ԥ��Ƶ
}

void main(void)
{
    InitSysCtrl();  	//��ʼ��ϵͳʱ�ӣ�ѡ���ڲ�����1��10MHZ��12��Ƶ��2��Ƶ����ʼ������ʱ�ӣ���������,4��Ƶ
    DINT; 				//�����ж�
    IER = 0x0000;  	 	//��CPU�ж�ʹ��
    IFR = 0x0000;   	//��CPU�жϱ�־
    InitPieCtrl();  	//��pie�ж�
    InitPieVectTable();	//���ж�������

    EALLOW;				/**�����ж�������*****/
    PieVectTable.TINT0 = &cpu_timer0_isr;
    PieVectTable.TINT1 = &cpu_timer1_isr;
    PieVectTable.XINT1 = &myXint1_isr;
    PieVectTable.ECAP1_INT = &Ecap1Int_isr;
    PieVectTable.EPWM1_INT = &EPWM1Int_isr;
    PieVectTable.ADCINT1 = &MyAdcInt1_isr;
    EDIS;

    // MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();

    InitCpuTimers();   	// ��ʼ����ʱ��
    ConfigCpuTimer(&CpuTimer0, 60, 500000);
    ConfigCpuTimer(&CpuTimer1, 60, 10000);

    EALLOW;
    CpuTimer0Regs.TCR.all = 0x4001;
    EDIS;

    HorseIO_Init();
    Xint1_Init();
    TM1638_Init(); 		//��ʼ��LED
    ECap_Init();
    EPwm1_Init();

    IER |= M_INT1;
    IER |= M_INT13;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;

    IER |= M_INT4;
    PieCtrlRegs.PIEIER4.bit.INTx1 = 1;

    EINT;
    ERTM;
    while(1) {}
}

void HorseRunning1(int no)
{
    if(no & 0x1)Led0Blink();
    else Led0Blank();
    if(no & 0x2)Led1Blink();
    else Led1Blank();
    if(no & 0x4)Led2Blink();
    else Led2Blank();
    if(no & 0x8)Led3Blink();
    else Led3Blank();
}

void HorseRunning2(int no)
{
    no = no % 0x5;
    if(no == 0x1)Led0Blink();
    else Led0Blank();
    if(no == 0x2)Led1Blink();
    else Led1Blank();
    if(no == 0x3)Led2Blink();
    else Led2Blank();
    if(no == 0x4)Led3Blink();
    else Led3Blank();
}

interrupt void myXint1_isr(void)
{
    if(GpioDataRegs.GPADAT.bit.GPIO12 == 1){
        CpuTimer1Regs.TCR.all = 0x4001;
    }
      else{
        if(KeyDLTime > 20){
            CpuTimer0.InterruptCount = 0;
            HorseRunningAction = !HorseRunningAction;
        }
        CpuTimer1Regs.TCR.bit.TRB = 1;
        CpuTimer1Regs.TCR.bit.TSS = 0;
        KeyDLTime = 0;
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    if(HorseRunningAction == 0)
        HorseRunning1(CpuTimer0.InterruptCount);
    else HorseRunning2(CpuTimer0.InterruptCount);
  	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer1_isr(void)
{
    KeyDLTime++;
}

interrupt void Ecap1Int_isr(void)
{

}
interrupt void EPWM1Int_isr(void)
{

}
interrupt void MyAdcInt1_isr(void)
{

}
