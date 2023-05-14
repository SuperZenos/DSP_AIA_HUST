/*
 * main.c
 */
#include "DSP28x_Project.h"
#include "LED_TM1638.h"

interrupt void Ecap1Int_isr(void);
interrupt void EPWM1Int_isr(void);
void Fail(void);

// Configure the start/end period for the timer
#define PWM1_TIMER_MIN     10
#define PWM1_TIMER_MAX     8000

// Global variables used in this example
Uint32  ECap1IntCount;
Uint32  ECap1PassCount;
Uint32  EPwm1TimerDirection;

// To keep track of which way the timer value is moving
#define EPwm_TIMER_UP   1
#define EPwm_TIMER_DOWN 0

#define EPWM1_MAX_DB   0x03FF
#define EPWM1_MIN_DB   0
#define DB_UP   1
#define DB_DOWN 0

Uint16  EPwm1_DB_Direction;
Uint32  EPwm1TimerIntCount;



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
    ECap1Regs.ECCTL1.bit.CTRRST1 = 0;          // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST2 = 0;          // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST3 = 0;          // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST4 = 0;          // Difference operation
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
    // Active Low PWMs - Setup Deadband
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;     //��ȫʹ��deadband
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;//DB_ACTV_LO;
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm1Regs.DBRED = EPWM1_MIN_DB;//�ӳټ�������Ƶ�ʿ�ѡΪʱ��Ƶ�ʵ�2����1��
    EPwm1Regs.DBFED = EPWM1_MIN_DB;
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
    PieVectTable.ECAP1_INT = &Ecap1Int_isr;
    PieVectTable.EPWM1_INT = &EPWM1Int_isr;
    EDIS;

    // MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();

    InitCpuTimers();   	// ��ʼ����ʱ��
    ConfigCpuTimer(&CpuTimer0, 60, 500000);
    ConfigCpuTimer(&CpuTimer1, 60, 10000);

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    TM1638_Init(); 		//��ʼ��LED
    ECap_Init();
    EPwm1_Init();
    InitLEDGpio();
    InitEPwm1Gpio();
    InitECap1Gpio();

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    IER |= M_INT4;
    IER |= M_INT3;
    PieCtrlRegs.PIEIER4.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

    EINT;
    ERTM;
    while(1)
    {
        //printf("period:%.2f,duty:%.2f", &Period1, &Duty);
    }
}

interrupt void Ecap1Int_isr(void)
{
    float TSt1 = 0; //��t1����Ԥȡָʱ���
    float TSt2 = 0; //��t2����Ԥȡָʱ���
    float TSt3 = 0; //��t3����Ԥȡָʱ���
    float TSt4 = 0; //��t4����Ԥȡָʱ���
    float Period1 = 0; // �����1������
    float Duty = 0;

    TSt1 = ECap1Regs.CAP1; //��t1����Ԥȡָʱ���
    TSt2 = ECap1Regs.CAP2; //��t2����Ԥȡָʱ���
    TSt3 = ECap1Regs.CAP3; //��t3����Ԥȡָʱ���
    TSt4 = ECap1Regs.CAP4; //��t4����Ԥȡָʱ���
    Period1 = TSt3-TSt1; // �����1������
    Duty = (TSt2-TSt1)/Period1; // ���㵼ͨʱ��

    ECap1Regs.ECCLR.bit.CEVT4 = 1;
    ECap1Regs.ECCLR.bit.INT = 1;
    ECap1Regs.ECCTL2.bit.REARM = 1;

    LED_showfloat(5, Duty);

    // Acknowledge this interrupt to receive more interrupts from group 4
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

interrupt void EPWM1Int_isr(void)
{
    // Acknowledge this interrupt to receive more interrupts from group 3
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
