#include <p32xxxx.h>
#include <stdio.h>
#include <string.h>
#include <plib.h>
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#define TRIS_MAIN_ADDR_DECODER {unsigned int portfMap = TRISF; portfMap &= 0xFFFFFEF8;TRISF=portfMap;}
#define MAIN_ADDR_DECODER(CBA) PORTF = CBA
#define MAIN_DECODER_CS _RF8
#define TRIS_DATA_OUT {unsigned int porteMap = TRISE; porteMap &= 0xFFFFFF00; TRISE = porteMap;}
#define TRIS_LED_CLK {unsigned int portdMap = TRISD; portdMap &= 0xFFFFFFEF; TRISD = portdMap;}
#define LED_CLK  _RD4
#define LED_EN  0x04
#define DISABLE 0x01
#define LINE1 0x80
#define LINE2 0xc0
#define CLEAR 0x01
void initPortB(void);
void initPortD(void);
void initPortE(void);
void initPortF(void);
void initPortG(void);
void initPorts(void);
void timeDelay(int j);
void printMessage(char* string, int line, int buzzerOn);
void printChar(char c, int line);
void clearLcd();
void initLcd();
int scanKey();
void writeXY(int x,int y,int lcd_cs);
void clearLcd_g(void);
void clearLeftLcd_g(void);
void initLcd_g(void);
void print_led(int x);
void writeLcd_g(unsigned int num,int lcd_cs);
void printPicLcd_g(unsigned int pic[], int col, int row, int xPos, int yPos,int cs);
int checkCode(void);
void timer1_Start(int totalTime);
void timer23_Start(int totalTime);
//Answers : 2, D, 2, 1 
//Code : #4748
/*
                                                         c=====e
                                                            H
   ____________                                         _,,_H__
  (__((__((___()                                       //|     |
 (__((__((___()()_____________________________________// | TNT |
(__((__((___()()()------------------------------------'  |_____|

*/
unsigned int one[4*46] =
{
	0x00, 0x00, 0xF0, 0x00, //                 ####            
	0x00, 0x00, 0xF8, 0x00, //                 #####           
	0x00, 0x00, 0x78, 0x00, //                  ####           
	0x00, 0x00, 0x7C, 0x00, //                  #####          
	0x00, 0x00, 0x3E, 0x00, //                   #####         
	0x00, 0x00, 0x3F, 0x00, //                   ######        
	0x00, 0x00, 0x1F, 0x80, //                    ######       
	0xFF, 0xFF, 0xFF, 0xC0, // ##########################      
	0xFF, 0xFF, 0xFF, 0xE0, // ###########################     
	0xFF, 0xFF, 0xFF, 0xE0, // ###########################     
	0xFF, 0xFF, 0xFF, 0xE0, // ###########################     
	0xFF, 0xFF, 0xFF, 0xE0, // ###########################     
	0x00, 0x00, 0x00, 0x00, //                                 
	0x00, 0x00, 0x00, 0x00, //                                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x0F, 0xFF, 0xFC, 0x00, //     ##################          
	0x0F, 0xFF, 0xFC, 0x00, //     ##################          
	0x0F, 0xFF, 0xFC, 0x00, //     ##################          
	0x0F, 0xFF, 0xFC, 0x00, //     ##################          
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x1E, 0x00, 0x00, //            ####                 
	0x00, 0x00, 0x00, 0x00, //                                 
	0x00, 0x00, 0x00, 0x00, //                                 
	0x00, 0x00, 0xF0, 0x00, //                 ####            
	0x00, 0x00, 0xF8, 0x00, //                 #####           
	0x00, 0x00, 0x78, 0x00, //                  ####           
	0x00, 0x00, 0x7C, 0x00, //                  #####          
	0x00, 0x00, 0x3E, 0x00, //                   #####         
	0x00, 0x00, 0x3F, 0x00, //                   ######        
	0x00, 0x00, 0x1F, 0x80, //                    ######       
	0xFF, 0xFF, 0xFF, 0xC0, // ##########################      
	0xFF, 0xFF, 0xFF, 0xE0, // ###########################     
	0xFF, 0xFF, 0xFF, 0xE0, // ###########################     
	0xFF, 0xFF, 0xFF, 0xE0, // ###########################     
	0xFF, 0xFF, 0xFF, 0xE0, // ###########################     
};
unsigned int questionMark[5*25] =
{
	// @0 '?' (25 pixels wide)
	0x00, 0x00, 0x00, 0x70, 0x00, //                          ###            
	0x00, 0x00, 0x00, 0x7C, 0x00, //                          #####          
	0x00, 0x00, 0x00, 0x7E, 0x00, //                          ######         
	0x00, 0x00, 0x00, 0xFF, 0x00, //                         ########        
	0x00, 0x00, 0x00, 0xFF, 0x80, //                         #########       
	0x00, 0x00, 0x00, 0xFF, 0x80, //                         #########       
	0x00, 0x00, 0x00, 0xFF, 0xC0, //                         ##########      
	0x00, 0x00, 0x00, 0x3F, 0xC0, //                           ########      
	0xFE, 0x7C, 0x00, 0x0F, 0xE0, // #######  #####              #######     
	0xFE, 0x7F, 0x00, 0x07, 0xE0, // #######  #######             ######     
	0xFE, 0x7F, 0x80, 0x07, 0xE0, // #######  ########            ######     
	0xFE, 0x7F, 0xC0, 0x03, 0xE0, // #######  #########            #####     
	0xFE, 0x7F, 0xE0, 0x03, 0xE0, // #######  ##########           #####     
	0xFE, 0x7F, 0xF0, 0x03, 0xE0, // #######  ###########          #####     
	0xFE, 0x7F, 0xF8, 0x03, 0xE0, // #######  ############         #####     
	0x00, 0x07, 0xF8, 0x07, 0xE0, //              ########        ######     
	0x00, 0x03, 0xFC, 0x07, 0xE0, //               ########       ######     
	0x00, 0x01, 0xFE, 0x1F, 0xC0, //                ########    #######      
	0x00, 0x00, 0xFF, 0xFF, 0xC0, //                 ##################      
	0x00, 0x00, 0x7F, 0xFF, 0x80, //                  ################       
	0x00, 0x00, 0x3F, 0xFF, 0x80, //                   ###############       
	0x00, 0x00, 0x3F, 0xFF, 0x00, //                   ##############        
	0x00, 0x00, 0x1F, 0xFE, 0x00, //                    ############         
	0x00, 0x00, 0x07, 0xFC, 0x00, //                      #########          
	0x00, 0x00, 0x01, 0xF0, 0x00, //                        #####            
};
unsigned int e[4*23] =
{
	// @0 'e' (23 pixels wide)
	0x00, 0x7F, 0xC0, 0x00, //          #########              
	0x03, 0xFF, 0xF8, 0x00, //       ###############           
	0x0F, 0xFF, 0xFC, 0x00, //     ##################          
	0x1F, 0xFF, 0xFF, 0x00, //    #####################        
	0x3F, 0xFF, 0xFF, 0x80, //   #######################       
	0x3F, 0xFF, 0xFF, 0x80, //   #######################       
	0x7F, 0xFF, 0xFF, 0xC0, //  #########################      
	0x7F, 0x8F, 0x1F, 0xC0, //  ########   ####   #######      
	0xFE, 0x0F, 0x07, 0xE0, // #######     ####     ######     
	0xFC, 0x0F, 0x07, 0xE0, // ######      ####     ######     
	0xF8, 0x0F, 0x03, 0xE0, // #####       ####      #####     
	0xF8, 0x0F, 0x03, 0xE0, // #####       ####      #####     
	0xF8, 0x0F, 0x03, 0xE0, // #####       ####      #####     
	0xF8, 0x0F, 0x07, 0xE0, // #####       ####     ######     
	0xFC, 0x0F, 0x07, 0xE0, // ######      ####     ######     
	0xFE, 0x0F, 0x1F, 0xC0, // #######     ####   #######      
	0x7F, 0x0F, 0xFF, 0xC0, //  #######    ##############      
	0x7F, 0x0F, 0xFF, 0x80, //  #######    #############       
	0x7F, 0x0F, 0xFF, 0x80, //  #######    #############       
	0x3F, 0x0F, 0xFF, 0x00, //   ######    ############        
	0x1E, 0x0F, 0xFE, 0x00, //    ####     ###########         
	0x0E, 0x0F, 0xF8, 0x00, //     ###     #########           
	0x02, 0x0F, 0xC0, 0x00, //       #     ######              
};
unsigned int lnD[2*17] = 
{
	//(2 pixels wide)
	0xFF, 0xC0, // ##########      
	0xFF, 0xC0, // ##########      
	//(1 pixels wide)
	0x00, 0x00, //                 
	//(6 pixels wide)
	0xFE, 0x00, // #######         
	0xFE, 0x00, // #######         
	0x04, 0x00, //      #          
	0x02, 0x00, //       #         
	0xFE, 0x00, // #######         
	0xFC, 0x00, // ######
	// @0 ' ' (1 pixels wide)
	0x00, 0x00, //
		// @2 'D' (7 pixels wide)
	0xFF, 0xC0, // ##########      
	0xFF, 0xC0, // ##########      
	0x80, 0x40, // #        #      
	0x80, 0x40, // #        #      
	0xC0, 0xC0, // ##      ##      
	0x7F, 0x80, //  ########       
	0x3F, 0x00, //   ######                         
};
unsigned int sin2x[2*48] =
{
	// @64 's' (7 pixels wide)
	0x49, 0x80, //  #  #  ##       
	0x5B, 0xC0, //  # ## ####      
	0x53, 0x40, //  # #  ## #      
	0x56, 0x40, //  # # ##  #      
	0x5E, 0xC0, //  # #### ##      
	0x4C, 0x80, //  #  ##  #       
	0x40, 0x00, //  #
	// @36 'i' (5 pixels wide)
	0x40, 0x00, //  #              
	0x5F, 0xD8, //  # ####### ##   
	0x5F, 0xD8, //  # ####### ##   
	0x40, 0x00, //  #              
	0x40, 0x00, //  #      
	// @46 'n' (9 pixels wide)
	0x40, 0x00, //  #              
	0x5F, 0xC0, //  # #######      
	0x5F, 0xC0, //  # #######      
	0x40, 0x80, //  #      #       
	0x40, 0x40, //  #       #      
	0x5F, 0xC0, //  # #######      
	0x5F, 0x80, //  # ######       
	0x40, 0x00, //  #              
	0x40, 0x00, //  #  
	// @0 '(' (5 pixels wide)
	0x40, 0x00, //  #              
	0x5F, 0xC0, //  # #######      
	0x7F, 0xF0, //  ###########    
	0xC0, 0x18, // ##         ##   
	0x40, 0x00, //  # 
	// @20 '2' (8 pixels wide)
	0x58, 0x10, //  # ##      #    
	0x5C, 0x18, //  # ###     ##   
	0x57, 0x08, //  # # ###    #   
	0x53, 0x88, //  # #  ###   #   
	0x51, 0xF8, //  # #   ######   
	0x50, 0x70, //  # #     ###    
	0x40, 0x00, //  #              
	0x40, 0x00, //  # 
	// @78 'x' (9 pixels wide)
	0x40, 0x00, //  #              
	0x58, 0xC0, //  # ##   ##      
	0x5D, 0xC0, //  # ### ###      
	0x47, 0x00, //  #   ###        
	0x47, 0x00, //  #   ###        
	0x5D, 0xC0, //  # ### ###      
	0x58, 0xC0, //  # ##   ##      
	0x40, 0x00, //  #              
	0x40, 0x00, //  #  
	// @10 ')' (5 pixels wide)
	0xC0, 0x18, // ##         ##   
	0x7F, 0xF0, //  ###########    
	0x5F, 0xC0, //  # #######      
	0x40, 0x00, //  #              
	0x40, 0x00, //  #
};
unsigned int cosxsinx[2*59] =
{
	// @12 'c' (6 pixels wide)
	0x0F, 0x80, //     #####       
	0x1F, 0xC0, //    #######      
	0x10, 0x40, //    #     #      
	0x10, 0x40, //    #     #      
	0x18, 0xC0, //    ##   ##      
	0x08, 0x80, //     #   #       
	// @40 'o' (6 pixels wide)
	0x0F, 0x80, //     #####       
	0x1F, 0xC0, //    #######      
	0x10, 0x40, //    #     #      
	0x10, 0x40, //    #     #      
	0x1F, 0xC0, //    #######      
	0x0F, 0x80, //     #####       
	// @52 's' (6 pixels wide)
	0x09, 0x80, //     #  ##       
	0x1B, 0xC0, //    ## ####      
	0x13, 0x40, //    #  ## #      
	0x16, 0x40, //    # ##  #      
	0x1E, 0xC0, //    #### ##      
	0x0C, 0x80, //     ##  #       
	// @0 '(' (3 pixels wide)
	0x1F, 0xC0, //    #######      
	0x7F, 0xF0, //  ###########    
	0xC0, 0x18, // ##         ##
	// @64 'x' (6 pixels wide)
	0x18, 0xC0, //    ##   ##      
	0x1D, 0xC0, //    ### ###      
	0x07, 0x00, //      ###        
	0x07, 0x00, //      ###        
	0x1D, 0xC0, //    ### ###      
	0x18, 0xC0, //    ##   ##      
	// @6 ')' (3 pixels wide)
	0xC0, 0x18, // ##         ##   
	0x7F, 0xF0, //  ###########    
	0x1F, 0xC0, //    #######     
		// @52 's' (6 pixels wide)
	0x09, 0x80, //     #  ##       
	0x1B, 0xC0, //    ## ####      
	0x13, 0x40, //    #  ## #      
	0x16, 0x40, //    # ##  #      
	0x1E, 0xC0, //    #### ##      
	0x0C, 0x80, //     ##  #  
	//(1 pixels wide)
	0x00, 0x00, //      
	// @24 'i' (2 pixels wide)
	0x1F, 0xD8, //    ####### ##   
	0x1F, 0xD8, //    ####### ##
	//(1 pixels wide)
	0x00, 0x00, //        
	// @28 'n' (6 pixels wide)
	0x1F, 0xC0, //    #######      
	0x1F, 0xC0, //    #######      
	0x00, 0x80, //         #       
	0x00, 0x40, //          #      
	0x1F, 0xC0, //    #######      
	0x1F, 0x80, //    ######
	//(1 pixels wide)
	0x00, 0x00, //      
	// @0 '(' (3 pixels wide)
	0x1F, 0xC0, //    #######      
	0x7F, 0xF0, //  ###########    
	0xC0, 0x18, // ##         ##
	// @64 'x' (6 pixels wide)
	0x18, 0xC0, //    ##   ##      
	0x1D, 0xC0, //    ### ###      
	0x07, 0x00, //      ###        
	0x07, 0x00, //      ###        
	0x1D, 0xC0, //    ### ###      
	0x18, 0xC0, //    ##   ##      
	// @6 ')' (3 pixels wide)
	0xC0, 0x18, // ##         ##   
	0x7F, 0xF0, //  ###########    
	0x1F, 0xC0, //    #######       
};
unsigned int lim[2*16] =
{
	// @4 'l' (2 pixels wide)
	0xFF, 0xC0, // ##########      
	0xFF, 0xC0, // ##########
	//(1 pixels wide)
	0x00, 0x00, //         
	// @0 'i' (2 pixels wide)
	0xFE, 0xC0, // ####### ##      
	0xFE, 0xC0, // ####### ##
	//(1 pixels wide)
	0x00, 0x00, //         
	// @8 'm' (10 pixels wide)
	0xFE, 0x00, // #######         
	0xFE, 0x00, // #######         
	0x04, 0x00, //      #          
	0x02, 0x00, //       #         
	0xFE, 0x00, // #######         
	0xFE, 0x00, // #######         
	0x04, 0x00, //      #          
	0x02, 0x00, //       #         
	0xFE, 0x00, // #######         
	0xFC, 0x00, // ######
};
unsigned int xTo0[1*23] =
{
	// @14 'x' (5 pixels wide)
	0xCC, // ##  ##  
	0xFC, // ######  
	0x30, //   ##    
	0xFC, // ######  
	0xCC, // ##  ## 
	// @1 '-' (6 pixels wide)
	0x18, //    ##     
	0x18, //    ##     
	0x18, //    ## 
	0x18, //    ##     
	0x18, //    ##     
	0x18, //    ## 
	// @9 '>' (5 pixels wide)
	0x42, //  #    # 
	0x24, //   #  #  
	0x24, //   #  #  
	0x18, //    ##   
	0x18, //    ## 
	//(1 pixels wide)
	0x00, 0x00, //   
	// @4 '0' (5 pixels wide)
	0x7E, //  ###### 
	0xFF, // ########
	0x81, // #      #
	0xFF, // ########
	0x7E, //  ###### 
};
unsigned int eMinus1[2*22] =
{
	// @28 'e' (9 pixels wide)
	0xC0, 0x00, // ##              
	0xC7, 0xC0, // ##   #####      
	0xCF, 0xE0, // ##  #######     
	0xDD, 0xB0, // ## ### ## ##    
	0xD9, 0xB0, // ## ##  ## ##    
	0xD9, 0xB0, // ## ##  ## ##    
	0xCD, 0xE0, // ##  ## ####     
	0xC5, 0xC0, // ##   # ###      
	0xC0, 0x00, // ##       
	// @0 '-' (5 pixels wide)
	0xC0, 0x00, // ##              
	0xC6, 0x00, // ##   ##        
	0xC6, 0x00, // ##   ##        
	0xC6, 0x00, // ##   ##        
	0xC6, 0x00, // ##   ##        
	// @0 '1' (8 pixels wide)
	0xC0, 0x00, // ##              
	0xC0, 0x60, // ##       ##     
	0xC0, 0x30, // ##        ##    
	0xC0, 0x18, // ##         ##   
	0xDF, 0xFC, // ## ###########  
	0xDF, 0xFC, // ## ###########  
	0xC0, 0x00, // ##              
	0xC0, 0x00, // ##               
};
unsigned int smallX[1*5] =
{
	// @0 'x' (5 pixels wide)
	0xCC, // ##  ##  
	0xFC, // ######  
	0x30, //   ##    
	0xFC, // ######  
	0xCC, // ##  ##
};
unsigned int bigX[1*6] =
{
	// @0 'x' (6 pixels wide)
	0xC6, // ##   ## 
	0xEE, // ### ### 
	0x38, //   ###   
	0x38, //   ###   
	0xEE, // ### ### 
	0xC6, // ##   ##  
};
int timeCounter = 0;
int digitCounter = 1;
char code[4];
char correctCode[4] = {'4','7','4','8'};
void __ISR(_TIMER_1_VECTOR, ipl2)  timer1_interrupt(void)
{
	PORTGbits.RG15 = 1;//buzzer on
	timeDelay(64000);
	PORTGbits.RG15 = 0;//buzzer off
	timeCounter++;
	IFS0bits.T1IF=0;
	if(timeCounter == 15)
		timer1_Start(0X9897/2);
	else if(timeCounter == 30)
		timer1_Start(0X9897/4);
	else if(timeCounter == 45)
		timer1_Start(0X9897/8);
		
}
void __ISR(_TIMER_3_VECTOR, ipl1) timer2_interrupt(void)
{
	initLcd();
	clearLcd(); 
	PORTGbits.RG15 = 1;
	printMessage("BOOM!",LINE1,0);
	while(1); 
}
int main(void)
{	
	int s,i,j;
	initPorts();	
	initLcd();
	clearLcd();
	printMessage("Press A to Start",LINE1,1);
	while(1)
	{
		s = scanKey();
		if(s == 'A') break;
	}
	initLcd_g();
	clearLcd_g();
	writeXY(0,0,1);
	printPicLcd_g(one,4,46,10,1,1);
	printPicLcd_g(questionMark,5,25,25,1,2);
	initLcd();
	clearLcd();
	printMessage("Timer: 1 minute",LINE1,1);
	printMessage("Good Luck...",LINE2,1);
	/*
	/TIMER START
	*/
	timer23_Start(0X9897*60);
	timer1_Start(0X9897);
	/*
	/TIMER START
	*/
	while(1)
	{
		s = scanKey();
		if(s == '2')
		{
			initLcd();
			clearLcd();
			printMessage("Correct!",LINE1,0);
			printMessage("Num:7 Line:4*4",LINE2,0);
			break;
		}		
	}
	initLcd_g();
	clearLeftLcd_g();
	writeXY(0,0,1);
	printPicLcd_g(e,4,23,5,3,1);
	printPicLcd_g(lnD,2,17,30,2,1);
	while(1)
	{
		s = scanKey();
		if(s == 'D')
		{
			initLcd();
			clearLcd();
			printMessage("Correct!",LINE1,0);
			printMessage("Num:4 Line:9",LINE2,0);
			break;
		}		
	}
	initLcd_g();
	clearLeftLcd_g();
	writeXY(0,0,1);
	printPicLcd_g(sin2x,2,48,6,1,1);
	printPicLcd_g(cosxsinx,2,59,1,3,1);
	while(1)
	{
		s = scanKey();
		if(s == '2')
		{
			initLcd();
			clearLcd();
			printMessage("Correct!",LINE1,0);
			printMessage("Num:4 Line:24",LINE2,0);
			break;
		}		
	}
	initLcd_g();
	clearLeftLcd_g();
	writeXY(0,0,1);
	printPicLcd_g(lim,2,16,5,0,1);
	printPicLcd_g(xTo0,1,23,5,2,1);
	printPicLcd_g(eMinus1,2,22,10,4,1);
	printPicLcd_g(smallX,1,5,18,4,1);
	printPicLcd_g(bigX,1,6,17,6,1);
	while(1)
	{
		s = scanKey();
		if(s == '1')
		{
			initLcd();
			clearLcd();
			printMessage("Correct!",LINE1,0);
			printMessage("Num:8 Line:5*5",LINE2,0);
			break;
		}
	}
	while(1)
	{
		s = scanKey();
		if(s == '#')
		{
			initLcd();
			clearLcd();
			T1CONbits.ON=0;
			printChar('#',LINE1);
			while(digitCounter != 5)
			{
				s = scanKey();
				if(s != 0xff && s!= 'A' && s!= 'B' && s!= 'C' && s!= 'D'
				&& s!= '*' && s!= '#')
				{
					initLcd();
					code[digitCounter-1] = s;
					printChar(s,LINE1 + digitCounter++);
					
				}
			}
			break;
		}
	}
	clearLcd_g();
	initLcd();
	clearLcd();
	T2CONbits.ON=0;
	printMessage("DID IT WORK?!",LINE1,0);
	timeDelay(64000*6);
	printChar('.',LINE2);
	timeDelay(64000*6);
	printChar('.',LINE2+1);
	timeDelay(64000*6);
	printChar('.',LINE2+2);
	timeDelay(128000*6);
	clearLcd();
	if(checkCode())
	{
		printMessage("Bomb Has Been",LINE1,1);
		printMessage("Defused",LINE2,1);
	}
	else
	{
		printMessage("Ohhhhhh",LINE1,0);
		printMessage("NOOOOOOO! :(",LINE2,0);
		initLcd_g();
		clearLcd_g();
	}
}
int checkCode(void)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		if(code[i] != correctCode[i])
			return 0;
	}
	return 1;		
}
void initPortB(void)
{              unsigned int portMap;
               portMap = TRISB;
	portMap &= 0xFFFF7FFF;
                portMap |= 0xF;
	TRISB = portMap;

AD1PCFG |= 0x800f; //Select PORTB to be digital port input
CNCONbits.ON = 0; //Change Notice Module On bit CN module is disabled
CNEN |= 0x3C;	
CNPUE |=0x3C;  	//Set RB0 - RB3 as inputs with weak pull-up
CNCONbits.ON = 1;// 1 = CN module is enabled
}

void initPortD(void)
{
	unsigned int portMap;
	portMap = TRISD;
	portMap &= 0xFFFFFFCF;
	TRISD = portMap;
}
void initPortE(void)
{
	unsigned int portMap;
	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	PORTE = 0x00;
}

void initPortF(void)
{
	unsigned int portMap;
	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
}

void initPortG(void)
{
	unsigned int portMap;
	portMap = TRISG;
	portMap &= 0xFFFFFFFC;
	TRISG = portMap;
	PORTG = 0x00;
}
void initPorts(void)
{
	initPortB();
	initPortD();
	initPortE();	
	initPortF();
	initPortG();
}
void timeDelay(int j)
{
	unsigned int i;
	for(i=0;i<j;i++);
}
void printMessage(char* string, int line,int buzzerOn)
	{
		int i;
		PORTBbits.RB15 = 0;
		PORTE = line;
		PORTDbits.RD4 = 1;//enable=1
		PORTDbits.RD4 = 0;//enable=0
		timeDelay(64000);
		PORTBbits.RB15 = 1;
		for (i = 0; i < strlen(string); i++)
		{
			PORTE = string[i];
			PORTDbits.RD4 = 1;//enable=1
			PORTDbits.RD4 = 0;//enable=0
			timeDelay(64000);
			if(buzzerOn)
			{
				PORTGbits.RG15 = 1;//buzzer on
				timeDelay(64000);
				PORTGbits.RG15 = 0;//buzzer off
			}	    		
		}
	}
void printChar(char c, int line)
	{
		PORTBbits.RB15 = 0;
		PORTE = line;
		PORTDbits.RD4 = 1;//enable=1
		PORTDbits.RD4 = 0;//enable=0
		timeDelay(64000);
		PORTBbits.RB15 = 1;
		PORTE = c;
		PORTDbits.RD4 = 1;//enable=1
		PORTDbits.RD4 = 0;//enable=0
		timeDelay(64000);
		PORTGbits.RG15 = 1;//buzzer on
		timeDelay(64000);
		timeDelay(64000);
		PORTGbits.RG15 = 0;//buzzer off	
	}
void clearLcd()
	{
		PORTBbits.RB15 = 0;
		PORTE = CLEAR;
		PORTDbits.RD4 = 1;//enable=1
		PORTDbits.RD4 = 0;//enable=0
		timeDelay(500);
		PORTBbits.RB15 = 1;
	}
void initLcd()
{
	int i;
	char control[6] = { 0x38,0x38,0x38,0x0e,0x06,0x1 };
	TRISF = 0;
	TRISD = 0;
	unsigned int portMap;
	portMap = TRISG;
	portMap &= 0xFFFF7FFF;
	TRISG = portMap;
	PORTGbits.RG15 = 0;//buzzer=0ff
	portMap = TRISB;
	portMap &= 0xFFFF7FFF;
	TRISB = portMap;
	AD1PCFG |= 0x8000;
	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	portMap = TRISD;
	portMap &= 0xFFFFFFCF;
	TRISD = portMap;
	PORTDbits.RD5 = 0;//w/r
	PORTBbits.RB15 = 0;//rs
	PORTF = 0x00;
	for (i = 0; i < 5; i++)
	{
		PORTE = control[i];
		PORTDbits.RD4 = 1;//enable=1
		PORTDbits.RD4 = 0;//enable=0
		timeDelay(64000);
	}
}
int scanKey()
{
	int RUN_ZERO[4] = {0xee,0xdd,0xbb,0x77};
	int scan_key_code_ascii[32]={0xee,'1',0xde,'2',0xbe,'3',0x7e,'A',
                                 0xed,'4',0xdd,'5',0xbd,'6',0x7d,'B',
                                 0xeb,'7',0xdb,'8',0xbb,'9',0x7b,'C',
                                 0xe7,'*',0xd7,'0',0xb7,'#',0x77,'D'};  
	int i,num_code,keyVal;
  	int  column = 0;
   	int  flag = 0;  
    PORTG = 0x00;
    PORTF = 0x07;
    for(i=0;i<10;i++)
	{
    	PORTE = RUN_ZERO[column];
		timeDelay(1);	
		keyVal = PORTB & 0x0F;
		if(keyVal != 0x0f)
    	{
			flag=1;
			break;
		}
    	column++;
		if(column == 4)
			column = 0;
	}
 	if(flag == 1)
    {
		num_code =((RUN_ZERO[column]&0xf0)|(keyVal));
        for(i=0;i<32;i+=2)
        {
	    	if(num_code==scan_key_code_ascii[i])
            {
				i=scan_key_code_ascii[i+1];  
                break;
			}		 
        }
    }
    else
    	i=0xff;
        return(i);
}
void print_led(int x)
{
	int lastTRISE = TRISE;
	TRISE=0;
	PORTF=4;
	PORTE=x;
	PORTDbits.RD4=1;
	PORTDbits.RD4=0;
	TRISE = lastTRISE;
}
void writeXY(int x,int y,int lcd_cs)
{
	PORTDbits.RD5 = 0;
	PORTBbits.RB15 = 0;
	PORTF = lcd_cs;
	PORTE = 0x40 + x;
	PORTDbits.RD4 = 1;//enable=1
    PORTDbits.RD4 = 0;//enable=0
	timeDelay(100);
	PORTE = 0xB8 + y;
	PORTDbits.RD4 = 1;//enable=1
    PORTDbits.RD4 = 0;//enable=0
	timeDelay(100);
	PORTFbits.RF8 = 1;
}
void initLcd_g(void)
{	
	int CONTROL[4] = {0x40,0xB8,0xFF,0x3F};
	int i;

	PORTDbits.RD5 = 0;
	PORTBbits.RB15 = 0;
	PORTF = 0x01;
	PORTDbits.RD7 = 0;
	PORTDbits.RD7 = 1;
	PORTF = 0x02;
	PORTDbits.RD7 = 0;
	PORTDbits.RD7 = 1;
	PORTFbits.RF8 = 1;

	for(i = 0;i < 4;i++)
	{
		PORTE = CONTROL[i];
		PORTF = 0x01;
		PORTDbits.RD4 = 1;//enable=1
    	PORTDbits.RD4 = 0;//enable=0
		timeDelay(100);
		PORTF = 0x02;
		PORTDbits.RD4 = 1;//enable=1
    	PORTDbits.RD4 = 0;//enable=0
		timeDelay(100);	
	}
	PORTFbits.RF8 = 1;
}
void writeLcd_g(unsigned int num,int lcd_cs)
{
	int i;

	PORTDbits.RD5 = 0;
	PORTBbits.RB15 = 1;
	PORTF = lcd_cs;
	PORTE = num;
    PORTDbits.RD4 = 1;//enable=1
    PORTDbits.RD4 = 0;//enable=0
	timeDelay(100);
	PORTFbits.RF8 = 1;
}
void clearLcd_g()
{
	int i,j;
	for(i = 0;i < 8;i++)
		{
			for(j = 0;j < 64;j++)
			{
				writeXY(j,i,1);
				writeLcd_g(0x00,1);
				writeXY(j,i,2);
				writeLcd_g(0x00,2);
			}
		}
}
void clearLeftLcd_g()
{
	int i,j;
	for(i = 0;i < 8;i++)
		{
			for(j = 0;j < 64;j++)
			{
				writeXY(j,i,1);
				writeLcd_g(0x00,1);
			}
		}
}
void printPicLcd_g(unsigned int pic[], int col, int row, int xPos, int yPos,int cs)
{	
	int i,j;
	for(i = col; i > 0; i--)
	{
		writeXY(xPos, yPos + col - i,cs);
		for(j = 0; j < row; j++)
			{
				writeLcd_g(pic[(i-1)+(col*j)],cs);
			}
	}
}
void timer1_Start(int totalTime)
{	// 0x9867 = 1 sec
	// 0x8F0D2 = 15 sec
	// 0x11E1A4 = 30 sec
	// 0x1AD276 = 45 sec
	T1CONbits.ON=0;
	T1CONbits.TCS=0;
	T1CONbits.TCKPS0=1;
	T1CONbits.TCKPS1=1;
	T1CONbits.TSYNC=1;
	TMR1=0;
	PR1 = totalTime;
	IFS0bits.T1IF=0;
	IPC1=4;
	IEC0bits.T1IE=1;
	INTCONbits.MVEC=1;//????? ??????
	IPTMR=0;//INTERRUPT PROXIMITY TIMER REGISTER
	T1CONbits.ON=1;
	asm("ei");
}
void timer23_Start(int totalTime)
{
	// 0x9867 = 1 sec
	// 0x8F0D2 = 15 sec
	// 0x11E1A4 = 30 sec
	// 0x1AD276 = 45 sec
	int timex = totalTime & 0xFFFF;
	int timey = totalTime & 0xFFFF0000;
	timey >>= 16;
	T2CONbits.ON=0;
	T2CONbits.TGATE=0;
	T2CONbits.TCS=0;
	T2CONbits.T32=1;	// mode 32bit
	T2CONbits.TCKPS0=1;	// 256
	T2CONbits.TCKPS1=1;
	T2CONbits.TCKPS2=1;
	T3CONbits.ON=0;
	TMR3=0;//TMRy
	TMR2=0;//TMRx
	PR2 = timex;//PR2x
	PR3 = timey;//PR2y
	IFS0bits.T2IF=0;
	IFS0bits.T3IF=0;
	//interrupt init
	IEC0bits.T2IE=1;
	IEC0bits.T3IE=1;
	INTCONbits.MVEC=1;	
	IPTMR=0;
	IPC3=4; //IPC0-11 according to handler IPC != 0 / IPC = 0 = Handler disabled
	//
	T2CONbits.ON=1;//start timer
		asm("ei");	
}
