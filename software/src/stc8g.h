#ifndef __STC8G_H__
#define __STC8G_H__

/////////////////////////////////////////////////

// 包含本头文件后,不用另外再包含"REG51.H"

sfr P0 = 0x80;
sbit P00 = P0 ^ 0;
sbit P01 = P0 ^ 1;
sbit P02 = P0 ^ 2;
sbit P03 = P0 ^ 3;
sbit P04 = P0 ^ 4;
sbit P05 = P0 ^ 5;
sbit P06 = P0 ^ 6;
sbit P07 = P0 ^ 7;
sfr SP = 0x81;
sfr DPL = 0x82;
sfr DPH = 0x83;
sfr S4CON = 0x84;
sfr S4BUF = 0x85;
sfr PCON = 0x87;
sfr TCON = 0x88;
sbit TF1 = TCON ^ 7;
sbit TR1 = TCON ^ 6;
sbit TF0 = TCON ^ 5;
sbit TR0 = TCON ^ 4;
sbit IE1 = TCON ^ 3;
sbit IT1 = TCON ^ 2;
sbit IE0 = TCON ^ 1;
sbit IT0 = TCON ^ 0;
sfr TMOD = 0x89;
sfr TL0 = 0x8A;
sfr TL1 = 0x8B;
sfr TH0 = 0x8C;
sfr TH1 = 0x8D;
sfr AUXR = 0x8E;
sfr INTCLKO = 0x8F;
sfr P1 = 0x90;
sbit P10 = P1 ^ 0;
sbit P11 = P1 ^ 1;
sbit P12 = P1 ^ 2;
sbit P13 = P1 ^ 3;
sbit P14 = P1 ^ 4;
sbit P15 = P1 ^ 5;
sbit P16 = P1 ^ 6;
sbit P17 = P1 ^ 7;
sfr P1M1 = 0x91;
sfr P1M0 = 0x92;
sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr SCON = 0x98;
sbit SM0 = SCON ^ 7;
sbit SM1 = SCON ^ 6;
sbit SM2 = SCON ^ 5;
sbit REN = SCON ^ 4;
sbit TB8 = SCON ^ 3;
sbit RB8 = SCON ^ 2;
sbit TI = SCON ^ 1;
sbit RI = SCON ^ 0;
sfr SBUF = 0x99;
sfr S2CON = 0x9A;
sfr S2BUF = 0x9B;
sfr IRCBAND = 0x9D;
sfr LIRTRIM = 0x9E;
sfr IRTRIM = 0x9F;
sfr P2 = 0xA0;
sbit P20 = P2 ^ 0;
sbit P21 = P2 ^ 1;
sbit P22 = P2 ^ 2;
sbit P23 = P2 ^ 3;
sbit P24 = P2 ^ 4;
sbit P25 = P2 ^ 5;
sbit P26 = P2 ^ 6;
sbit P27 = P2 ^ 7;
sfr P_SW1 = 0xA2;
sfr IE = 0xA8;
sbit EA = IE ^ 7;
sbit ELVD = IE ^ 6;
sbit EADC = IE ^ 5;
sbit ES = IE ^ 4;
sbit ET1 = IE ^ 3;
sbit EX1 = IE ^ 2;
sbit ET0 = IE ^ 1;
sbit EX0 = IE ^ 0;
sfr SADDR = 0xA9;
sfr WKTCL = 0xAA;
sfr WKTCH = 0xAB;
sfr S3CON = 0xAC;
sfr S3BUF = 0xAD;
sfr TA = 0xAE;
sfr IE2 = 0xAF;
sfr P3 = 0xB0;
sbit P30 = P3 ^ 0;
sbit P31 = P3 ^ 1;
sbit P32 = P3 ^ 2;
sbit P33 = P3 ^ 3;
sbit P34 = P3 ^ 4;
sbit P35 = P3 ^ 5;
sbit P36 = P3 ^ 6;
sbit P37 = P3 ^ 7;
sfr P3M1 = 0xB1;
sfr P3M0 = 0xB2;
sfr P4M1 = 0xB3;
sfr P4M0 = 0xB4;
sfr IP2 = 0xB5;
sfr IP2H = 0xB6;
sfr IPH = 0xB7;
sfr IP = 0xB8;
sbit PPCA = IP ^ 7;
sbit PLVD = IP ^ 6;
sbit PADC = IP ^ 5;
sbit PS = IP ^ 4;
sbit PT1 = IP ^ 3;
sbit PX1 = IP ^ 2;
sbit PT0 = IP ^ 1;
sbit PX0 = IP ^ 0;
sfr SADEN = 0xB9;
sfr P_SW2 = 0xBA;
sfr ADC_CONTR = 0xBC;
sfr ADC_RES = 0xBD;
sfr ADC_RESL = 0xBE;
sfr P4 = 0xC0;
sbit P40 = P4 ^ 0;
sbit P41 = P4 ^ 1;
sbit P42 = P4 ^ 2;
sbit P43 = P4 ^ 3;
sbit P44 = P4 ^ 4;
sbit P45 = P4 ^ 5;
sbit P46 = P4 ^ 6;
sbit P47 = P4 ^ 7;
sfr WDT_CONTR = 0xC1;
sfr IAP_DATA = 0xC2;
sfr IAP_ADDRH = 0xC3;
sfr IAP_ADDRL = 0xC4;
sfr IAP_CMD = 0xC5;
sfr IAP_TRIG = 0xC6;
sfr IAP_CONTR = 0xC7;
sfr P5 = 0xC8;
sbit P50 = P5 ^ 0;
sbit P51 = P5 ^ 1;
sbit P52 = P5 ^ 2;
sbit P53 = P5 ^ 3;
sbit P54 = P5 ^ 4;
sbit P55 = P5 ^ 5;
sbit P56 = P5 ^ 6;
sbit P57 = P5 ^ 7;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
sfr P6M1 = 0xcb;
sfr P6M0 = 0xcc;
sfr SPSTAT = 0xCD;
sfr SPCTL = 0xCE;
sfr SPDAT = 0xCF;
sfr PSW = 0xD0;
sbit CY = PSW ^ 7;
sbit AC = PSW ^ 6;
sbit F0 = PSW ^ 5;
sbit RS1 = PSW ^ 4;
sbit RS0 = PSW ^ 3;
sbit OV = PSW ^ 2;
sbit F1 = PSW ^ 1;
sbit P = PSW ^ 0;
sfr T4T3M = 0xD1;
sfr T4H = 0xD2;
sfr T4L = 0xD3;
sfr T3H = 0xD4;
sfr T3L = 0xD5;
sfr T2H = 0xD6;
sfr T2L = 0xD7;
sfr CCON = 0xD8;
sbit CF = CCON ^ 7;
sbit CR = CCON ^ 6;
sbit CCF2 = CCON ^ 2;
sbit CCF1 = CCON ^ 1;
sbit CCF0 = CCON ^ 0;
sfr CMOD = 0xD9;
sfr CCAPM0 = 0xDA;
sfr CCAPM1 = 0xDB;
sfr CCAPM2 = 0xDC;
sfr ADCCFG = 0xDE;
sfr IP3 = 0xDF;
sfr ACC = 0xE0;
sfr P7M1 = 0xe1;
sfr P7M0 = 0xe2;
sfr DPS = 0xE3;
sfr DPL1 = 0xE4;
sfr DPH1 = 0xE5;
sfr CMPCR1 = 0xE6;
sfr CMPCR2 = 0xE7;
sfr P6 = 0xe8;
sbit P60 = P6 ^ 0;
sbit P61 = P6 ^ 1;
sbit P62 = P6 ^ 2;
sbit P63 = P6 ^ 3;
sbit P64 = P6 ^ 4;
sbit P65 = P6 ^ 5;
sbit P66 = P6 ^ 6;
sbit P67 = P6 ^ 7;
sfr CL = 0xE9;
sfr CCAP0L = 0xEA;
sfr CCAP1L = 0xEB;
sfr CCAP2L = 0xEC;
sfr IP3H = 0xEE;
sfr AUXINTIF = 0xEF;
sfr B = 0xF0;
sfr PWMSET = 0xF1;
sfr PCA_PWM0 = 0xF2;
sfr PCA_PWM1 = 0xF3;
sfr PCA_PWM2 = 0xF4;
sfr IAP_TPS = 0xF5;
sfr PWMCFG01 = 0xF6;
sfr PWMCFG23 = 0xF7;
sfr P7 = 0xf8;
sbit P70 = P7 ^ 0;
sbit P71 = P7 ^ 1;
sbit P72 = P7 ^ 2;
sbit P73 = P7 ^ 3;
sbit P74 = P7 ^ 4;
sbit P75 = P7 ^ 5;
sbit P76 = P7 ^ 6;
sbit P77 = P7 ^ 7;
sfr CH = 0xF9;
sfr CCAP0H = 0xFA;
sfr CCAP1H = 0xFB;
sfr CCAP2H = 0xFC;
sfr PWMCFG45 = 0xFE;
sfr RSTCFG = 0xFF;

// 如下特殊功能寄存器位于扩展RAM区域
// 访问这些寄存器,需先将P_SW2的BIT7设置为1,才可正常读写

/////////////////////////////////////////////////
// FF00H-FFFFH
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// FE00H-FEFFH
/////////////////////////////////////////////////

#define CLKSEL (*(unsigned char volatile xdata *)0xfe00)
#define CLKDIV (*(unsigned char volatile xdata *)0xfe01)
#define HIRCCR (*(unsigned char volatile xdata *)0xfe02)
#define XOSCCR (*(unsigned char volatile xdata *)0xfe03)
#define IRC32KCR (*(unsigned char volatile xdata *)0xfe04)
#define MCLKOCR (*(unsigned char volatile xdata *)0xfe05)
#define IRCDB (*(unsigned char volatile xdata *)0xfe06)
#define X32KCR (*(unsigned char volatile xdata *)0xfe08)

#define P0PU (*(unsigned char volatile xdata *)0xfe10)
#define P1PU (*(unsigned char volatile xdata *)0xfe11)
#define P2PU (*(unsigned char volatile xdata *)0xfe12)
#define P3PU (*(unsigned char volatile xdata *)0xfe13)
#define P4PU (*(unsigned char volatile xdata *)0xfe14)
#define P5PU (*(unsigned char volatile xdata *)0xfe15)
#define P6PU (*(unsigned char volatile xdata *)0xfe16)
#define P7PU (*(unsigned char volatile xdata *)0xfe17)
#define P0NCS (*(unsigned char volatile xdata *)0xfe18)
#define P1NCS (*(unsigned char volatile xdata *)0xfe19)
#define P2NCS (*(unsigned char volatile xdata *)0xfe1a)
#define P3NCS (*(unsigned char volatile xdata *)0xfe1b)
#define P4NCS (*(unsigned char volatile xdata *)0xfe1c)
#define P5NCS (*(unsigned char volatile xdata *)0xfe1d)
#define P6NCS (*(unsigned char volatile xdata *)0xfe1e)
#define P7NCS (*(unsigned char volatile xdata *)0xfe1f)
#define P0SR (*(unsigned char volatile xdata *)0xfe20)
#define P1SR (*(unsigned char volatile xdata *)0xfe21)
#define P2SR (*(unsigned char volatile xdata *)0xfe22)
#define P3SR (*(unsigned char volatile xdata *)0xfe23)
#define P4SR (*(unsigned char volatile xdata *)0xfe24)
#define P5SR (*(unsigned char volatile xdata *)0xfe25)
#define P6SR (*(unsigned char volatile xdata *)0xfe26)
#define P7SR (*(unsigned char volatile xdata *)0xfe27)
#define P0DR (*(unsigned char volatile xdata *)0xfe28)
#define P1DR (*(unsigned char volatile xdata *)0xfe29)
#define P2DR (*(unsigned char volatile xdata *)0xfe2a)
#define P3DR (*(unsigned char volatile xdata *)0xfe2b)
#define P4DR (*(unsigned char volatile xdata *)0xfe2c)
#define P5DR (*(unsigned char volatile xdata *)0xfe2d)
#define P6DR (*(unsigned char volatile xdata *)0xfe2e)
#define P7DR (*(unsigned char volatile xdata *)0xfe2f)
#define P0IE (*(unsigned char volatile xdata *)0xfe30)
#define P1IE (*(unsigned char volatile xdata *)0xfe31)
#define P2IE (*(unsigned char volatile xdata *)0xfe32)
#define P3IE (*(unsigned char volatile xdata *)0xfe33)
#define P4IE (*(unsigned char volatile xdata *)0xfe34)
#define P5IE (*(unsigned char volatile xdata *)0xfe35)
#define P6IE (*(unsigned char volatile xdata *)0xfe36)
#define P7IE (*(unsigned char volatile xdata *)0xfe37)

#define I2CCFG (*(unsigned char volatile xdata *)0xfe80)
#define I2CMSCR (*(unsigned char volatile xdata *)0xfe81)
#define I2CMSST (*(unsigned char volatile xdata *)0xfe82)
#define I2CSLCR (*(unsigned char volatile xdata *)0xfe83)
#define I2CSLST (*(unsigned char volatile xdata *)0xfe84)
#define I2CSLADR (*(unsigned char volatile xdata *)0xfe85)
#define I2CTXD (*(unsigned char volatile xdata *)0xfe86)
#define I2CRXD (*(unsigned char volatile xdata *)0xfe87)
#define I2CMSAUX (*(unsigned char volatile xdata *)0xfe88)

#define TM2PS (*(unsigned char volatile xdata *)0xfea2)
#define TM3PS (*(unsigned char volatile xdata *)0xfea3)
#define TM4PS (*(unsigned char volatile xdata *)0xfea4)
#define ADCTIM (*(unsigned char volatile xdata *)0xfea8)
#define T3T4PS (*(unsigned char volatile xdata *)0xfeac)

/////////////////////////////////////////////////
// FD00H-FDFFH
/////////////////////////////////////////////////

#define P0INTE (*(unsigned char volatile xdata *)0xfd00)
#define P1INTE (*(unsigned char volatile xdata *)0xfd01)
#define P2INTE (*(unsigned char volatile xdata *)0xfd02)
#define P3INTE (*(unsigned char volatile xdata *)0xfd03)
#define P4INTE (*(unsigned char volatile xdata *)0xfd04)
#define P5INTE (*(unsigned char volatile xdata *)0xfd05)
#define P6INTE (*(unsigned char volatile xdata *)0xfd06)
#define P7INTE (*(unsigned char volatile xdata *)0xfd07)
#define P0INTF (*(unsigned char volatile xdata *)0xfd10)
#define P1INTF (*(unsigned char volatile xdata *)0xfd11)
#define P2INTF (*(unsigned char volatile xdata *)0xfd12)
#define P3INTF (*(unsigned char volatile xdata *)0xfd13)
#define P4INTF (*(unsigned char volatile xdata *)0xfd14)
#define P5INTF (*(unsigned char volatile xdata *)0xfd15)
#define P6INTF (*(unsigned char volatile xdata *)0xfd16)
#define P7INTF (*(unsigned char volatile xdata *)0xfd17)
#define P0IM0 (*(unsigned char volatile xdata *)0xfd20)
#define P1IM0 (*(unsigned char volatile xdata *)0xfd21)
#define P2IM0 (*(unsigned char volatile xdata *)0xfd22)
#define P3IM0 (*(unsigned char volatile xdata *)0xfd23)
#define P4IM0 (*(unsigned char volatile xdata *)0xfd24)
#define P5IM0 (*(unsigned char volatile xdata *)0xfd25)
#define P6IM0 (*(unsigned char volatile xdata *)0xfd26)
#define P7IM0 (*(unsigned char volatile xdata *)0xfd27)
#define P0IM1 (*(unsigned char volatile xdata *)0xfd30)
#define P1IM1 (*(unsigned char volatile xdata *)0xfd31)
#define P2IM1 (*(unsigned char volatile xdata *)0xfd32)
#define P3IM1 (*(unsigned char volatile xdata *)0xfd33)
#define P4IM1 (*(unsigned char volatile xdata *)0xfd34)
#define P5IM1 (*(unsigned char volatile xdata *)0xfd35)
#define P6IM1 (*(unsigned char volatile xdata *)0xfd36)
#define P7IM1 (*(unsigned char volatile xdata *)0xfd37)
#define P0WKUE (*(unsigned char volatile xdata *)0xfd40)
#define P1WKUE (*(unsigned char volatile xdata *)0xfd41)
#define P2WKUE (*(unsigned char volatile xdata *)0xfd42)
#define P3WKUE (*(unsigned char volatile xdata *)0xfd43)
#define P4WKUE (*(unsigned char volatile xdata *)0xfd44)
#define P5WKUE (*(unsigned char volatile xdata *)0xfd45)
#define P6WKUE (*(unsigned char volatile xdata *)0xfd46)
#define P7WKUE (*(unsigned char volatile xdata *)0xfd47)
#define PINIPL (*(unsigned char volatile xdata *)0xfd60)
#define PINIPH (*(unsigned char volatile xdata *)0xfd61)

/////////////////////////////////////////////////
// FC00H-FCFFH
/////////////////////////////////////////////////

#define MD3 (*(unsigned char volatile xdata *)0xfcf0)
#define MD2 (*(unsigned char volatile xdata *)0xfcf1)
#define MD1 (*(unsigned char volatile xdata *)0xfcf2)
#define MD0 (*(unsigned char volatile xdata *)0xfcf3)
#define MD5 (*(unsigned char volatile xdata *)0xfcf4)
#define MD4 (*(unsigned char volatile xdata *)0xfcf5)
#define ARCON (*(unsigned char volatile xdata *)0xfcf6)
#define OPCON (*(unsigned char volatile xdata *)0xfcf7)

/////////////////////////////////////////////////
// FB00H-FBFFH
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// FA00H-FAFFH
/////////////////////////////////////////////////

/* interrupt vector */
#define INT0_VECTOR 0
#define TIMER0_VECTOR 1
#define INT1_VECTOR 2
#define TIMER1_VECTOR 3
#define UART1_VECTOR 4
#define ADC_VECTOR 5
#define LVD_VECTOR 6
#define PCA_VECTOR 7
#define UART2_VECTOR 8
#define SPI_VECTOR 9
#define INT2_VECTOR 10
#define INT3_VECTOR 11
#define TIMER2_VECTOR 12
#define INT4_VECTOR 16
#define UART3_VECTOR 17
#define UART4_VECTOR 18
#define TIMER3_VECTOR 19
#define TIMER4_VECTOR 20
#define CMP_VECTOR 21
#define PWM0_VECTOR 22
#define PWMFD_VECTOR 23
#define I2C_VECTOR 24
#define USB_VECTOR 25
#define PWMA_VECTOR 26
#define PWMB_VECTOR 27
#define PWM1_VECTOR 28
#define PWM2_VECTOR 29
#define PWM3_VECTOR 30
#define PWM4_VECTOR 31
#define PWM5_VECTOR 32

/////////////////////////////////////////////////

#endif
