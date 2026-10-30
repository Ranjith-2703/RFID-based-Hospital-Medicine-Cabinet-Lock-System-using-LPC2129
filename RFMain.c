#include <LPC21xx.h>
#define LCD_D       (0xFF << 8)
#define RS          (1 << 16)
#define E           (1 << 17)
#define MOTOR_IN1   (1 << 18)
#define MOTOR_IN2   (1 << 19)
#define LED         (1 << 20)
#define BUZZER      (1 << 21)
#define SWITCH      (1 << 22)

const char tag1[] = "060067D961D9";
const char tag2[] = "09876543DS21";

char rfid[13];

void delay_milliseconds(unsigned int ms);
void UART0_INIT(void);
char UART0_RX(void);
void RFID_READ(void);
int RFID_CHECK(void);
void LCD_INIT(void);
void LCD_COMMAND(unsigned char c);
void LCD_DATA(unsigned char d);
void LCD_STRING(char *s);
void LCD_LINE2(void);
void LCD_STATUS(char *line1, char *line2);
void MOTOR_OPEN(void);
void MOTOR_CLOSE(void);
void MOTOR_STOP(void);
void BUZZER_ON(void);
void BUZZER_OFF(void);

void delay_milliseconds(unsigned int ms)
{
    T0TCR = 0x02;
    T0PR = 15000 - 1;
    T0TC = 0;
    T0TCR = 0x01;

    while (T0TC < ms);

    T0TCR = 0x00;
}

void UART0_INIT(void)
{
    PINSEL0 = (PINSEL0 & ~0x0000000F) | 0x00000005;
    U0LCR = 0x83;
    U0DLL = 97;
    U0DLM = 0;
    U0LCR = 0x03;
}

char UART0_RX(void)
{
    while (!(U0LSR & 0x01));

    return U0RBR;
}

void RFID_READ(void)
{
    unsigned int count = 0;
    char c;

    while (count < 12)
    {
        c = UART0_RX();

        if (c == '\r' || c == '\n')
        {
            if (count == 12)
                break;

            if (count == 0)
                continue;
        }
        else if ((c >= '0' && c <= '9') ||
                 (c >= 'A' && c <= 'F') ||
                 (c >= 'a' && c <= 'f'))
        {
            rfid[count++] = c;
        }
        else
        {
            count = 0;
        }
    }

    rfid[12] = '\0';
}

int RFID_CHECK(void)
{
    unsigned int i;
    int match1 = 1;
    int match2 = 1;

    for (i = 0; i < 12; i++)
    {
        if (rfid[i] != tag1[i])
            match1 = 0;

        if (rfid[i] != tag2[i])
            match2 = 0;
    }

    return match1 || match2;
}

void LCD_INIT(void)
{
    IODIR0 |= LCD_D | RS | E;
    IOCLR0 = LCD_D | RS | E;

    delay_milliseconds(20);

    LCD_COMMAND(0x38);
    LCD_COMMAND(0x0C);
    LCD_COMMAND(0x06);
    LCD_COMMAND(0x01);

    delay_milliseconds(2);
}

void LCD_COMMAND(unsigned char c)
{
    IOCLR0 = LCD_D;
    IOSET0 = ((unsigned int)c << 8) & LCD_D;
    IOCLR0 = RS;
    IOSET0 = E;

    delay_milliseconds(1);

    IOCLR0 = E;

    if (c == 0x01 || c == 0x02)
        delay_milliseconds(2);
    else
        delay_milliseconds(1);
}

void LCD_DATA(unsigned char d)
{
    IOCLR0 = LCD_D;
    IOSET0 = ((unsigned int)d << 8) & LCD_D;
    IOSET0 = RS;
    IOSET0 = E;

    delay_milliseconds(1);

    IOCLR0 = E;

    delay_milliseconds(1);
}

void LCD_STRING(char *s)
{
    while (*s)
        LCD_DATA(*s++);
}

void LCD_LINE2(void)
{
    LCD_COMMAND(0xC0);
}

void LCD_STATUS(char *line1, char *line2)
{
    LCD_COMMAND(0x01);
    LCD_STRING(line1);
    LCD_LINE2();
    LCD_STRING(line2);
}

void MOTOR_OPEN(void)
{
    IOCLR0 = MOTOR_IN2;
    IOSET0 = MOTOR_IN1;
}

void MOTOR_CLOSE(void)
{
    IOCLR0 = MOTOR_IN1;
    IOSET0 = MOTOR_IN2;
}

void MOTOR_STOP(void)
{
    IOCLR0 = MOTOR_IN1 | MOTOR_IN2;
}

void BUZZER_ON(void)
{
    IOCLR0 = BUZZER;
}

void BUZZER_OFF(void)
{
    IOSET0 = BUZZER;
}

int main(void)
{
    PINSEL0 &= ~0xFFFF0000;
    PINSEL1 &= ~0x00003FFF;

    IODIR0 |= LCD_D | RS | E | MOTOR_IN1 | MOTOR_IN2 | LED | BUZZER;
    IODIR0 &= ~SWITCH;

    MOTOR_STOP();
    IOSET0 = LED;
BUZZER_OFF();
    UART0_INIT();
    LCD_INIT();

    LCD_STATUS("Hospital Cabinet", "RFID System");
    delay_milliseconds(2000);

    while (1)
    {
        LCD_STATUS("Scan RFID Card", "Waiting...");
        RFID_READ();
        if (RFID_CHECK())
        {
  BUZZER_OFF();
            LCD_STATUS("Authorized ID", "Access Granted");
            delay_milliseconds(1000);

            IOCLR0 = LED;
            //BUZZER_OFF();

            LCD_STATUS("LED ON", "Motor ON-Opening");

            MOTOR_OPEN();
            delay_milliseconds(5000);
            MOTOR_STOP();

            IOSET0 = LED;

            LCD_STATUS("Cabinet Open", "Press Switch");

            while (IOPIN0 & SWITCH);

            delay_milliseconds(50);

            if (!(IOPIN0 & SWITCH))
            {
                LCD_STATUS("Motor ON", "Closing...");

                MOTOR_CLOSE();
                delay_milliseconds(5000);
                MOTOR_STOP();

                LCD_STATUS("Cabinet Closed", "Scan RFID Card");
                delay_milliseconds(1500);
            }

            while (!(IOPIN0 & SWITCH));

            delay_milliseconds(50);
        }
        else
        {
            MOTOR_STOP();
            IOCLR0 = LED;

            LCD_STATUS("Unauthorized ID", "Buzzer ON");

            BUZZER_ON();
            delay_milliseconds(1000);
            BUZZER_OFF();
            delay_milliseconds(1000);
        }
    }
}
