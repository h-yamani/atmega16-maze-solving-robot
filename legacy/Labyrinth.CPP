#include <mega16.h>
#include <delay.h>
#include <stdlib.h>

// Alphanumeric LCD Module functions
#asm
PORTC.equ __lcd_port = 0x15;
#endasm
#include <lcd.h>

#define ADC_VREF_TYPE 0x20 

// Read the 8 most significant bits
// of the AD conversion result
unsigned char read_adc(unsigned char adc_input)
{
	ADMUX = adc_input | ADC_VREF_TYPE;
	// Start the AD conversion
	ADCSRA |= 0x40;
	// Wait for the AD conversion to complete
	while ((ADCSRA & 0x10) == 0);
	ADCSRA |= 0x10;
	return ADCH;
}

// Declare your global variables here  
//~~~~~~~~~~~~~~~~~~~~~~~~~~~VARIABLES
char flag_move = '2';
char location[10][10][5];
int xG = 2, yG = 5;
int a[3] = { 0 };
char heading = 'n';
int x = 3;
int y = 1;
int x1;
int y1;
int w;
char flag = '0';
bit fsen, bsen, lsen, rsen;
bit frsen, flsen, brsen, blsen;
int count = 0;
bit color = 1;
bit left_error;
int dl = 4;
char goal;
int sl = 150;
bit error_left;
char rmotor[4] = { 0x09, 0x0c, 0x06, 0x03 };// Array for right motor
char lmotor[4] = { 0x30, 0x60, 0xc0, 0x90 };// Array for left motor 
int rm = 0, lm = 0;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~FUNCTIONS
int promissing(int, int, char);
void go_front_motors();
void go_left_motors();
void go_right_motors();
void go_back_motors();
void stop_motors();
void fill_cost();
void set(char, int, int);
void go_front();
void go_left();
void go_right();
void go_back();
void front();
void left();
void right();
void back();
void sensor();


void main(void)
{
	int move = 0;
	// Declare your local variables here

	// Input/Output Ports initialization
	// Port A initialization
	// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
	// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
	PORTA = 0x00;
	DDRA = 0x00;

	// Port B initialization
	// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
	// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
	PORTB = 0x00;
	DDRB = 0xFF;

	// Port C initialization
	// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
	// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
	PORTC = 0x00;
	DDRC = 0x00;

	// Port D initialization
	// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
	// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
	PORTD = 0x00;
	DDRD = 0x00;

	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: Timer 0 Stopped
	// Mode: Normal top=FFh
	// OC0 output: Disconnected
	TCCR0 = 0x00;
	TCNT0 = 0x00;
	OCR0 = 0x00;

	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: Timer 1 Stopped
	// Mode: Normal top=FFFFh
	// OC1A output: Discon.
	// OC1B output: Discon.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer 1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCNT1H = 0x00;
	TCNT1L = 0x00;
	ICR1H = 0x00;
	ICR1L = 0x00;
	OCR1AH = 0x00;
	OCR1AL = 0x00;
	OCR1BH = 0x00;
	OCR1BL = 0x00;

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: Timer 2 Stopped
	// Mode: Normal top=FFh
	// OC2 output: Disconnected
	ASSR = 0x00;
	TCCR2 = 0x00;
	TCNT2 = 0x00;
	OCR2 = 0x00;

	// External Interrupt(s) initialization
	// INT0: On
	// INT0 Mode: Rising Edge
	// INT1: On
	// INT1 Mode: Rising Edge
	// INT2: On
	// INT2 Mode: Rising Edge
	GICR |= 0xE0;
	MCUCR = 0x0F;
	MCUCSR = 0x40;
	GIFR = 0xE0;

	// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK = 0x00;

	// Analog Comparator initialization
	// Analog Comparator: Off
	// Analog Comparator Input Capture by Timer/Counter 1: Off
	ACSR = 0x80;
	SFIOR = 0x00;

	// ADC initialization
	// ADC Clock frequency: 1000.000 kHz
	// ADC Voltage Reference: AREF pin
	// ADC Auto Trigger Source: None
	// Only the 8 most significant bits of
	// the AD conversion result are used
	ADMUX = ADC_VREF_TYPE;
	ADCSRA = 0x84;

	// LCD module initialization
	lcd_init(16);
	while (1)
	{
		// Place your code here
		// Place your code here  

		lcd_clear();
		/* lcd_gotoxy(0,0);
		x = read_adc(0);
		// itoa(x,a);
		// lcd_puts(a);
		if(x<120)
		lcd_putsf("0");
		else
		lcd_putsf("1");*/

		//lcd_gotoxy(4,0);
		// x = read_adc(1); 
		//itoa(x,a);
		// lcd_puts(a); 
		/*if(y<120)
		lcd_putsf("0");
		else
		lcd_putsf("1");*/
		// BACK SENSOR     
		lcd_gotoxy(12, 0);
		w = read_adc(2);
		//itoa(w,a);
		//lcd_puts(a);
		if (w<240)
		{
			lcd_putsf("1");
			bsen = 1;
		}
		else
		{
			lcd_putsf("0");
			bsen = 0;
		}
		//RIGHT SENSOR      
		lcd_gotoxy(4, 0);
		w = read_adc(3);
		itoa(w, a);
		lcd_puts(a);
		if (w<230)
		{
			//lcd_putsf("1");
			rsen = 1;
		}
		else
		{
			//lcd_putsf("0");
			rsen = 0;
		}

		/* lcd_gotoxy(0,1);
		x = read_adc(4);
		//itoa(x,a);
		// lcd_puts(a);
		if(p<175)
		{
		lcd_putsf("1");

		else
		lcd_putsf("0");*/

		// FRONT SENSOR
		lcd_gotoxy(0, 0);
		w = read_adc(5);
		itoa(w, a);
		lcd_puts(a);
		if (w<240)
		{
			//lcd_putsf("1"); 
			fsen = 1;
		}
		else
		{
			//lcd_putsf("0");
			fsen = 0;
		}
		/*
		lcd_gotoxy(8,1);
		x = read_adc(6);
		itoa(x,a);
		lcd_puts(a);
		if(r<190)
		lcd_putsf("0");
		else
		lcd_putsf("1");*/

		//LEFT SENSOR     
		lcd_gotoxy(8, 0);
		w = read_adc(7);
		itoa(w, a);
		lcd_puts(a);
		if (w<230)
		{
			//lcd_putsf("1");
			lsen = 1;
		}
		else
		{
			//lcd_putsf("0");
			lsen = 0;
		}
		delay_ms(50);

		//START MAIN..................................   

		lcd_gotoxy(13, 1);
		itoa(x, a);
		lcd_puts(a);
		lcd_gotoxy(15, 1);
		itoa(y, a);
		lcd_puts(a);

		flag_move = '2';
		if (x == xG && y == yG)
			stop_motors();
		else{
			if (location[x][y][4] == NULL)
				location[x][y][4] = heading;
			else
				if (heading != location[x][y][4])
					set(location[x][y][4], x, y);
			if (flag == '0')
				location[x][y][3] = '1';
			if (flag == '1')
				location[x][y][3] = '3';
			if (flag == '2')
				location[x][y][0] = '3';
			if (flag == '3')
				location[x][y][0] = '1';
			fill_cost();
			if (goal != NULL)
			{
				switch (goal)
				{
				case 'f':go_front_motors(); stop_motors(); break;
				case 'r':go_right_motors(); stop_motors(); break;
				case 'l':go_left_motors(); stop_motors(); break;
				case 'b':go_back_motors(); stop_motors(); break;

				}
			}

			for (move = 0; move <= 3; move++)
				if (location[x][y][move] == '0')
				{
					flag_move = '0';
					break;
				}

			if (flag_move == '2')
				for (move = 0; move<4; move++)
					if (location[x][y][move] == '1')
					{
						flag_move = '1';
						break;
					}
			switch (move)
			{
			case 0: go_front(); break;
			case 1: go_right(); break;
			case 2: go_left(); break;
			case 3: go_back(); break;
				// default:stop_motors(); break;
			}//end of switch
		}//end of else




	};

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SET
void set(char old_head, int X, int Y)
{
	int i;
	char a[4];
	if ((heading == 'n'&& old_head == 's') || (heading == 'e' && old_head == 'w') || (heading == 's'&&  old_head == 'n') || (heading == 'w'&&  old_head == 'e'))
	{
		for (i = 0; i<4; i++)
			a[i] = location[X][Y][i];
		location[X][Y][0] = a[3];
		location[X][Y][1] = a[2];
		location[X][Y][2] = a[1];
		location[X][Y][3] = a[0];

	}

	if ((heading == 'n'&&  old_head == 'e') || (heading == 'e' &&  old_head == 's') || (heading == 's'&&  old_head == 'w') || (heading == 'w'&&  old_head == 'n'))
	{
		for (i = 0; i<4; i++)
			a[i] = location[X][Y][i];
		location[X][Y][0] = a[2];
		location[X][Y][1] = a[0];
		location[X][Y][2] = a[3];
		location[X][Y][3] = a[1];

	}
	if ((heading == 'n'&& old_head == 'w') || (heading == 'e' &&  old_head == 'n') || (heading == 's'&&  old_head == 'e') || (heading == 'w'&&  old_head == 's'))
	{
		for (i = 0; i<4; i++)
			a[i] = location[X][Y][i];
		location[X][Y][0] = a[1];
		location[X][Y][1] = a[3];
		location[X][Y][2] = a[0];
		location[X][Y][3] = a[2];
	}
	location[X][Y][4] = heading;
}//end of set


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FILL_COST
void fill_cost()
{
	if (location[x][y][0] == NULL)
	{
		front();

		if (!promissing(x1, y1, 'f'))
			location[x][y][0] = '3';
		else
		{
			location[x][y][0] = '0';
			if (x1 == xG && y1 == yG)
				goal = 'f';
		}

	}
	if (location[x][y][1] == NULL)
	{
		right();
		if (!promissing(x1, y1, 'r'))
			location[x][y][1] = '3';
		else{
			location[x][y][1] = '0';
			if (location[x][y][0] == '0')
			{
				if (location[x1][y1][4] != NULL)
					set(location[x1][y1][4], x1, y1);
				else
					location[x1][y1][4] = heading;
				location[x1][y1][2] = '3';
			}
			if (x1 == xG && y1 == yG)
				goal = 'r';
		}
	}
	if (location[x][y][2] == NULL)
	{
		left();
		if (!promissing(x1, y1, 'l'))
			location[x][y][2] = '3';
		else{
			location[x][y][2] = '0';
			if (location[x][y][0] == '0' || location[x][y][1] == '0')
			{
				if (location[x1][y1][4] != NULL)
					set(location[x1][y1][4], x1, y1);
				else
					location[x1][y1][4] = heading;
				location[x1][y1][1] = '3';
			}
			if (x1 == xG && y1 == yG)
				goal = 'l';
		}
	}
	if (location[x][y][3] == NULL)
	{
		back();

		if (!promissing(x1, y1, 'b'))
			location[x][y][3] = '3';
		else{
			location[x][y][3] = '0';
			if (location[x][y][0] == '0' || location[x][y][1] == '0' || location[x][y][2] == '0')
			{
				if (location[x1][y1][4] != NULL)
					set(location[x1][y1][4], x1, y1);
				else
					location[x1][y1][4] = heading;
				location[x1][y1][0] = '3';

			}
			if (x1 == xG && y1 == yG)
				goal = 'b';
		}
	}
}//end of fill_cost


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FRONT
void front()
{
	x1 = x;
	y1 = y;
	switch (heading)
	{
	case 'n': x1--; break;
	case 's': x1++; break;
	case 'w': y1--; break;
	case 'e': y1++; break;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RIGHT
void right()
{
	x1 = x;
	y1 = y;
	switch (heading)
	{
	case 'n': y1++; break;
	case 's': y1--; break;
	case 'w': x1--; break;
	case 'e': x1++; break;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~LEFT
void left()
{
	x1 = x;
	y1 = y;
	switch (heading)
	{
	case 'n': y1--; break;
	case 's': y1++; break;
	case 'w': x1++; break;
	case 'e': x1--; break;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BACK
void back()
{
	x1 = x;
	y1 = y;
	switch (heading)
	{
	case 'n': x1++; break;
	case 's': x1--; break;
	case 'w': y1++; break;
	case 'e': y1--; break;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GO_FRONT
void go_front()
{
	if (location[x][y][0] == '0')
		location[x][y][0] = '1';
	switch (heading)
	{
	case 'n': x--; break;
	case 's': x++; break;
	case 'w': y--; break;
	case 'e': y++; break;
	}
	switch (flag_move)
	{
	case '0':flag = '0'; break;
	case '1':flag = '1'; break;
	}
	go_front_motors();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GO_RIGHT
void go_right()
{
	if (location[x][y][1] == '0')
		location[x][y][1] = '1';
	switch (heading)
	{
	case 'n': y++; heading = 'e'; break;
	case 's': y--; heading = 'w'; break;
	case 'w': x--; heading = 'n'; break;
	case 'e': x++; heading = 's'; break;
	}
	switch (flag_move)
	{
	case '0':flag = '0'; break;
	case '1':flag = '1'; break;
	}
	go_right_motors();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GO_LEFT
void go_left()
{
	if (location[x][y][2] == '0')
		location[x][y][2] = '1';
	switch (heading)
	{
	case 'n': y--; heading = 'w'; break;
	case 's': y++; heading = 'e'; break;
	case 'e': x--; heading = 'n'; break;
	case 'w': x++; heading = 's'; break;
	}
	switch (flag_move)
	{
	case '0':flag = '0'; break;
	case '1':flag = '1'; break;
	}
	go_left_motors();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GO_BACK
void go_back()
{
	if (location[x][y][3] == '0')
		location[x][y][3] = '1';
	switch (heading)
	{
	case 's': x--; break;
	case 'n': x++; break;
	case 'e': y--; break;
	case 'w': y++; break;
	}
	switch (flag_move)
	{
	case '0':flag = '3'; break;
	case '1':flag = '2'; break;
	}
	go_back_motors();
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PROMISSING
int promissing(int m, int n, char mov)
{
	if (m == 0 || n == 0 || m == 4 || n == 6)
		return 0;
	if (mov == 'f' && fsen == 1)
		return 0;
	if (mov == 'l' && lsen == 1)
		return 0;
	if (mov == 'r' && rsen == 1)
		return 0;
	if (mov == 'b' && bsen == 1)
		return 0;
	return 1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GO_FRONT_MOTORS
void go_front_motors(){
	int j = 1;
	for (; j <= 218; j++){
		PORTB = rmotor[rm] + lmotor[lm];
		delay_ms(dl);
		rm = (++rm) % 4;
		lm = (++lm) % 4;
		check_sensor();
	}
	//delay_ms(200);
	//PORTB =0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GO_LEFT_MOTORS  
void go_left_motors(){
	int j = 1;
	for (; j <= 70; j++)
	{

		lm = (--lm) % 4;
		if (lm == -1)
			lm = 3;
		PORTB = rmotor[rm] + lmotor[lm];
		delay_ms(dl);
		rm = (++rm) % 4;
	}
	go_front_motors();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GO_RIGHT_MOTORS
void go_right_motors(){
	int j = 1;
	for (; j <= 70; j++)
	{
		rm = (--rm) % 4;
		if (rm == -1)
			rm = 3;
		PORTB = rmotor[rm] + lmotor[lm];
		delay_ms(dl);
		lm = (++lm) % 4;

	}
	go_front_motors();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GO_BACK_MOTORS
void go_back_motors(){
	int j = 1;
	for (; j <= 218; j++){

		PORTB = rmotor[rm] + lmotor[lm];
		delay_ms(dl);
		rm = (--rm) % 4;
		lm = (--lm) % 4;
		if (rm == -1)
			rm = 3;
		if (lm == -1)
			lm = 3;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~STOP_MOTORS 
void stop_motors()
{

	PORTB = 0;
	delay_ms(200);
	lcd_gotoxy(2, 0);
	while (1)
	{
		lcd_clear();
		if (x == xG && y == yG)
			lcd_putsf("****END*****");
		else
			lcd_putsf("**DEAD END**");
		delay_ms(500);
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~SENSOR
void check_sensor()
{
	//FLOOR SENSOR RIGHT
	lcd_gotoxy(3, 0);
	w = read_adc(4);
	itoa(w, a);
	lcd_puts(a);
	if (w < sl)
	{
		// lcd_putsf("1"); 
		frsen = 1;
	}
	else
	{
		//lcd_putsf("0");
		frsen = 0;
	}

	// FLOOR SENSOR  LEFT  
	lcd_gotoxy(7, 0);
	w = read_adc(6);

	if (w < sl)
	{
		flsen = 1;
	}
	else
	{
		flsen = 0;
	}
	itoa(w, a);
	lcd_puts(a);
	delay_ms(5);

	if (frsen != flsen)
		correct_path();



}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~CORRECT_PATH
void correct_path()
{
	if (flsen != color)
		error_left = 1;
	else
		error_left = 0;
	if (!error_left)
	{
		while (flsen != frsen)
		{
			PORTB = 51;
			delay_ms(dl);
			PORTB = 102;
			delay_ms(dl);
			PORTB = 204;
			delay_ms(dl);
			PORTB = 153;
			delay_ms(dl);
			w = read_adc(4);
			if (w < sl)
				frsen = 1;
			else
				frsen = 0;
			w = read_adc(6);
			if (w < sl)
				flsen = 1;
			else
				flsen = 0;
		}
	}
	else
	{
		while (flsen != frsen)
		{
			PORTB = 153;
			delay_ms(dl);
			PORTB = 204;
			delay_ms(dl);
			PORTB = 102;
			delay_ms(dl);
			PORTB = 51;
			delay_ms(dl);
			w = read_adc(4);
			if (w < sl)
				frsen = 1;
			else
				frsen = 0;
			w = read_adc(6);
			if (w < sl)
				flsen = 1;
			else
				flsen = 0;
		}
	}


	color = flsen;

}
