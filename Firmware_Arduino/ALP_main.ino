
#define FOSC 24576000//16000000 // Clock Speed 
#define BAUD 115200
#define MYUBRR FOSC/16/BAUD-1

//Laser Modulation Related
#define FRAME_BUFFER_SIZE 80
#define PIXEL_REG 25 
#define MAX_LINES_PER_FRAME 8
#define CHAR_WIDTH 6
#define MAX_PIXELS_FRAME 2000
#define MAX_CHAR_FRAME 8
#define MESSAGE_SIZE 500

//define ports
int MOTOR_L = 12; 
int MOTOR_R = 13; 
int MOTOR_PORT = 10;
int LASER_PORT = 3;
int SENSOR_IN = 7;
int SENSOR_OUT = 2; 

int firstByte = 0;

//Laser Modulation Related
int pixelID = 0; //frame buffer related
int sectionID = 0;//frame buffer related

//Motor Speed Detection Related
int Setpoint = 0; 
int Output = 500;
boolean alpha = true; 
boolean motorON = false; 
boolean motorError = false; 
float motor_rate_step = 1; 

volatile unsigned long time; 
volatile unsigned long dTime; 


//commandHandler related
char command[MESSAGE_SIZE+2];

//frameBuffer
unsigned int frameBuffer[FRAME_BUFFER_SIZE];
char mode; 
char flow;  
int temp; 
int input_array_index;
int cont_mess_size; 
unsigned int messageBuffer[MESSAGE_SIZE];

//scrolling
int cnt = 0; 

PROGMEM const prog_uint8_t BitMap[MAX_LINES_PER_FRAME*76][6] = {

{0,1,1,1,0,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,0,0},

{0,0,1,1,0,0}, 
{0,1,0,1,0,0},
{1,0,0,1,0,0},
{0,0,0,1,0,0},
{0,0,0,1,0,0},
{0,0,0,1,0,0},
{0,0,0,1,0,0},
{0,0,1,1,1,0}, 

{0,1,1,1,0,0}, 
{1,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,1,0,0},
{0,0,1,0,0,0},
{0,1,0,0,0,0},
{1,0,0,0,0,0},
{1,1,1,1,1,0},

{1,1,1,1,0,0}, 
{0,0,0,0,1,0},
{1,1,1,1,0,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{1,1,1,1,0,0},

{0,0,0,1,1,0}, 
{0,0,1,0,1,0},
{0,1,0,0,1,0},
{1,1,1,1,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},

{0,1,1,1,1,0}, 
{1,0,0,0,0,0},
{1,1,1,1,0,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{1,1,1,1,0,0},

{0,1,1,1,0,0}, 
{1,0,0,0,0,0},
{1,1,1,1,0,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,0,0},

{1,1,1,1,1,0}, 
{0,0,0,0,1,0},
{0,0,0,1,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},

{0,1,1,1,0,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,0,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,0,0},

{0,1,1,1,0,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,0,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,1,1,1,0,0},
   
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
  
{0,1,1,1,0,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,1,1,1,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},

{0,1,1,1,0,0}, 
{1,0,0,0,1,0},
{1,1,1,1,1,0},
{1,0,0,0,0,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,1,1,1,0,0},

{0,1,1,1,1,0}, 
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{0,1,1,1,1,0},

{1,1,1,1,0,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,1,1,1,0,0},

{0,1,1,1,1,0}, 
{1,0,0,0,0,0},
{1,1,1,1,1,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{0,1,1,1,1,0},

{1,1,1,1,1,0}, 
{1,0,0,0,0,0},
{1,1,1,1,1,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},

{0,1,1,1,1,0}, 
{1,0,0,0,0,0},
{1,0,1,1,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,0,0},

{1,0,0,0,1,0}, 
{1,0,0,0,1,0},
{1,1,1,1,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},

{1,1,1,1,1,0}, 
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{1,1,1,1,1,0},

{0,0,1,1,1,0}, 
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,0,0},

{1,0,0,0,1,0}, 
{1,0,0,1,0,0},
{1,0,1,0,0,0},
{1,1,0,0,0,0},
{1,1,0,0,0,0},
{1,0,1,0,0,0},
{1,0,0,1,0,0},
{1,0,0,0,1,0},

{1,0,0,0,0,0}, 
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,1,1,1,1,0},

{1,0,0,0,1,0}, 
{1,1,0,1,1,0},
{1,0,1,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},

{1,0,0,0,1,0}, 
{1,1,0,0,1,0},
{1,0,1,0,1,0},
{1,0,0,1,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},

{1,1,1,1,1,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,1,1,1,1,0},

{1,1,1,1,0,0}, 
{1,0,0,0,1,0},
{1,1,1,1,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},

{1,1,1,1,1,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,1,1,0},
{0,1,1,1,1,0},
{0,0,0,0,0,1},

{1,1,1,1,0,0}, 
{1,0,0,0,1,0},
{1,1,1,1,0,0},
{1,0,0,1,0,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},

{0,1,1,1,1,0}, 
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{0,1,1,1,0,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{1,1,1,1,0,0},

{1,1,1,1,1,0}, 
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},

{1,0,0,0,1,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,0,0},

{1,0,0,0,1,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,0,1,0,0},
{0,0,1,0,0,0},

{1,0,0,0,1,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,1,0,1,0},
{1,1,0,1,1,0},
{1,0,0,0,1,0},

{1,0,0,0,1,0}, 
{1,0,0,0,1,0},
{0,1,0,1,0,0},
{0,0,1,0,0,0},
{0,1,0,1,0,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},

{1,0,0,0,1,0}, 
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,1,0},
{0,0,0,1,0,0},
{0,0,1,0,0,0},
{0,1,0,0,0,0},
{1,0,0,0,0,0},

{1,1,1,1,1,0}, 
{0,0,0,0,1,0},
{0,0,0,1,0,0},
{0,0,1,0,0,0},
{0,1,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,1,1,1,1,0},
//a
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,1,1,1,0,0},
{0,0,0,1,0,0},
{0,1,1,1,0,0},
{1,0,0,1,0,0},
{1,0,0,1,0,0},
{0,1,1,0,1,0},
//b
{1,0,0,0,0,0}, 
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,1,1,1,0,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,1,1,1,0,0},
//c
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,1,1,1,1,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{0,1,1,1,1,0},
//d
{0,0,0,0,1,0}, 
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,1,1,1,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,1,0},
//e
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,1,1,1,0,0},
{1,0,0,0,1,0},
{1,1,1,1,1,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
{0,1,1,1,1,0},
//f
{0,0,1,1,1,0}, 
{0,1,0,0,0,0},
{1,1,1,1,1,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{1,1,1,1,0,0},
//g
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,1,1,0,1,0},
{1,0,0,1,0,0},
{1,1,1,1,0},
{0,0,0,1,0,0},
{0,0,0,1,0,0},
{0,1,1,0,0,0},
//h
{0,1,0,0,0,0}, 
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{0,1,1,1,0,0},
{0,1,0,0,1,0},
{0,1,0,0,1,0},
{1,1,0,0,1,0},
//i
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,0,1,0,0,0},
{0,0,0,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{1,1,1,1,0,0},
//j
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,1,1,1,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,1,1,1,0,0},
//k
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,1,0,0,1,0},
{0,1,0,1,0,0},
{0,1,1,0,0,0},
{0,1,0,1,0,0},
{0,1,0,0,1,0},
{1,1,0,0,1,0},
//l
{0,1,1,0,0,0}, 
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{1,1,1,1,0,0},
//m
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,1,0,1,0,0},
{1,0,1,0,1,0},
{1,0,1,0,1,0},
{1,0,1,0,1,0},
{1,0,1,0,1,0},
{1,0,1,0,1,0},
///n
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,1,0,0,0,0},
{0,1,1,1,1,0},
{0,1,0,0,1,0},
{0,1,0,0,1,0},
{0,1,0,0,1,0},
{1,1,0,0,1,0},
//o
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,1,1,1,0,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,1,1,0,0},
//p
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,1,1,1,0,0},
{0,1,0,0,1,0},
{0,1,0,0,1,0},
{0,1,1,1,0,0},
{0,1,0,0,0,0},
{1,1,0,0,0,0},
//q
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,1,1,0,1,0},
{1,0,0,1,0,0},
{1,0,0,1,0,0},
{0,1,1,1,0,0},
{0,0,0,1,0,0},
{0,0,1,1,1,0},
//r
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,0,1,1,1,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{1,1,1,1,1,0},
//s
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,1,1,1,1,0},
{1,0,0,0,0,0},
{0,1,1,1,0,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{1,1,1,1,0,0},
//t
{0,1,0,0,0,0}, 
{1,1,1,1,1,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{0,1,0,0,0,0},
{0,1,0,0,1,0},
{0,0,1,1,0,0},
//u
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,0,0,1,0,0},
{1,0,0,1,0,0},
{1,0,0,1,0,0},
{1,0,0,1,0,0},
{1,0,0,1,0,0},
{0,1,1,0,1,0},

//v
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
{0,1,0,1,0,0},
{0,0,1,0,0,0},
//w
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,0,1,0,1,0},
{1,0,1,0,1,0},
{1,0,1,0,1,0},
{1,0,1,0,1,0},
{1,0,1,0,1,0},
{1,1,0,1,1,0},
//x
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,0,0,0,1,0},
{0,1,0,1,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,1,0,1,0,0},
{1,0,0,0,1,0},
//y
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,0,0,0,1,0},
{0,1,0,1,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,1,1,0,0,0},
//z
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{1,1,1,1,1,0},
{0,0,0,1,0,0},
{0,0,1,0,0,0},
{0,1,0,0,0,0},
{1,0,0,0,0,0},
{1,1,1,1,1,0},
//;
{0,0,0,0,0,0}, 
{0,0,0,1,0,0},
{0,0,0,0,0,0},
{0,0,0,1,0,0},
{0,0,0,1,0,0},
{0,0,0,1,0,0},
{0,0,1,0,0,0},
{0,0,0,0,0,0},
///:
{0,0,0,0,0,0}, 
{0,0,1,1,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,1,1,0,0},
{0,0,0,0,0,0},
//)
{1,1,1,0,0,0}, 
{0,0,0,1,0,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,1,0,0},
{1,1,1,0,0,0},
//%
{0,0,0,0,1,0}, 
{1,1,0,0,1,0},
{0,0,0,1,0,0},
{0,0,1,0,0,0},
{0,1,0,0,0,0},
{1,0,0,1,1,0},
{1,0,0,0,0,0},
{1,0,0,0,0,0},
//#
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,1,0,1,0,0},
{1,1,1,1,1,0},
{0,1,0,1,0,0},
{1,1,0,1,1,0},
{0,1,0,1,0,0},
{0,1,0,1,0,0},
//?
{0,1,1,1,0,0}, 
{1,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,0,1,0},
{0,0,0,1,0,0},
{0,0,1,0,0,0},
{0,0,0,0,0,0},
{0,0,1,0,0,0},
//!
{0,0,1,1,0,0}, 
{0,0,1,1,0,0},
{0,0,1,1,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,1,1,0,0},
{0,0,1,1,0,0},
//*
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,0,1,0,0,0},
{1,1,1,1,1,0},
{0,0,1,0,0,0},
{0,1,0,1,0,0},
{1,0,0,0,1,0},
{1,0,0,0,1,0},
//.
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,1,1,0,0},
{0,0,1,1,0,0},

//,
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,1,0,0},
{0,0,0,1,0,0},
{0,0,0,1,0,0},
{0,0,1,0,0,0},

//+
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,1,1,1,0,0},
{0,0,1,0,0,0},
{0,0,1,0,0,0},
{0,0,0,0,0,0},

//-
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,1,1,1,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},

//=
{0,0,0,0,0,0}, 
{0,0,0,0,0,0},
{0,0,0,0,0,0},
{0,1,1,1,0,0},
{0,0,0,0,0,0},
{0,1,1,1,0,0},
{0,0,0,0,0,0},
{0,0,0,0,0,0},

};


/****************************************************************************
function: motorSpeedDetector()
interrupts when the IR sensor detects one motor loop and gets
the spin rate time. Also readjusts motor speed creating a closed loop control
*****************************************************************************/
void motorSpeedDetector() { 
  //Serial.println(Output);
  dTime = micros()-time;
  if(alpha)
    Serial.println(dTime);  
  //Serial.println("HOLA"); 
  time = time + dTime; //instead of calling call micros twice 
  if (motorON) {
    if(int(dTime-Setpoint) > 50){
        Output= Output+motor_rate_step;
        
        if (Output > 1000) { 
  	    Output = 1000; 
  	    motorError = true;
        }
        //Serial.println(Output);
        analogWrite(MOTOR_PORT, Output);
        }
        else if (int(Setpoint-dTime) > 50) {
        Output=Output-motor_rate_step;
        if (Output < 20) { 
  	  Output = 20; 
  	  motorError = true;
        }
        //Serial.println(Output);
        analogWrite(MOTOR_PORT, Output);
        }
        else {
  	    motorError = false; 
        }
  
  			//readFrameBufferPixel(FRAME_RESET); //Reset the pixel buffer index to 0 (this also achieves synchronization)
  			//TCNT2  = 0;  //reset the laser interrupt timer to 0 to start the count again (this also part of sychronization)
  			//laserFlag = 1;
      }
      else {
  	  Output = 500;
  		//analogWrite(MOTOR_PORT, 0);  
      }
      TCNT2  = 0;  //reset the laser interrupt timer to 0 to start the count again (this also part of sychronization)
      pixelID=0; 
      sectionID=0; 
}

/*****************************************************
Function: ISR(TIMER2_COMPA_vect)
Interrupts when timer reaches pixelReg value and sets
laser depending on FrameBuffer value.
******************************************************/

ISR(TIMER2_COMPA_vect)         
{
    if (readFrameBufferPixel()) {
		//digitalWrite(LASER_PORT, HIGH);
        //PORTD = PORTD & B11011111;
        PORTD = PORTD | B00001000; //turn laser ON if read pixel if ONE.
     }
     else {
        //digitalWrite(LASER_PORT, LOW);
	//PORTD = PORTD | B00100000;  
        PORTD = PORTD & B11110111; //turn laser OFF if read pixel is ZERO
        

     }
     TCNT2  = 0;  //restart the timer2   
}

/**************************************************************
Function: enableLaserModulation()
increments the timer2 resolution in order to have more accuracy
***************************************************************/
void enableLaserModulation()
{  
  cli(); // disable all interrupts
  //initialize timer2 8-bit = 256 positions, pins 3,11
  TCCR2A = 0;
  TCCR2B = 0;              
  TCNT2  = 0; 
  
  
  OCR2A = PIXEL_REG; //Compare match register 16MHz/prescaler/freq_timer -> pixel time = 12.5uS = 80000Hz
                      //so 25 = 16MHz/8/80000
  TCCR2B |= (1<<WGM22);  //TCCR2B = 0b00001010 -> Mode 4: CTC, 8 prescaler
  TCCR2B |= (1<<CS21); 
  TIMSK2 |= (1 << OCIE2A); //enable timer2 compare interrupt
  sei();  //enable interrupts
}

/**************************************************************
Function: disableLaserModulation()
***************************************************************/
void disableLaserModulation()
{  
  cli(); // disable all interrupts
  //initialize timer2 8-bit = 256 positions, pins 3,11
  TCCR2A = 0;
  TCCR2B = 0;              
  TCNT2  = 0; 
  TIMSK2 = 0;
  sei();  //enable interrupts
}

/********************************************************************
Function: readFrameBufferPixel()
reads the value allocate in frameBuffer everytime laser ISR is called 
depending on pixelID. 
*********************************************************************/
bool readFrameBufferPixel() {  //this function is called everytime laser modulation ISR is called
    bool px=false;

    switch (pixelID){
          case 0: { px = bitRead(frameBuffer[sectionID], 0); break; }
          case 1: { px = bitRead(frameBuffer[sectionID], 1); break; }
          case 2: { px = bitRead(frameBuffer[sectionID], 2); break; }
          case 3: { px = bitRead(frameBuffer[sectionID], 3); break; }
          case 4: { px = bitRead(frameBuffer[sectionID], 4); break; }
          case 5: { px = bitRead(frameBuffer[sectionID], 5); break; }
          case 6: { px = bitRead(frameBuffer[sectionID], 6); break; }
          case 7: { px = bitRead(frameBuffer[sectionID], 7); break; }
          case 8:  { px = bitRead(frameBuffer[sectionID], 8); break; }
          case 9:  { px = bitRead(frameBuffer[sectionID], 9); break; }
          case 10:  { px = bitRead(frameBuffer[sectionID], 10); break; }
          case 11:  { px = bitRead(frameBuffer[sectionID], 11); break; }
          case 12:  { px = bitRead(frameBuffer[sectionID], 12); break; }
          case 13:  { px = bitRead(frameBuffer[sectionID], 13); break; }
          case 14:  { px = bitRead(frameBuffer[sectionID], 14); break; }
          case 15:  { px = bitRead(frameBuffer[sectionID], 15); break; }
          default: { px = 0;                                 break; } //error, shut off laser
    }
    pixelID++; 
    if (pixelID > 15){
        pixelID = 0;
        sectionID++;
        
        if(sectionID >= FRAME_BUFFER_SIZE){  //make sure framebufersize does not exceed array index
            sectionID = 0;
            }
     }

     return px;
}

/**************************
Function clearFrameBuffer()
resets frameBuffer
***************************/
void clearFrameBuffer()
{
   for (int i=0; i < FRAME_BUFFER_SIZE; i++){
      frameBuffer[i] = 0;
  }
}



void USART_Init( unsigned int ubrr)
{

/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/*Enable receiver and transmitter andn interrups */
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(3<<UCSZ00);

}

void displayCharacter(int pos, int shape) 
{    	
    for(byte line=0; line < MAX_LINES_PER_FRAME; line++)
    {
      for(byte pixel=0; pixel<CHAR_WIDTH; pixel++)
      {
        byte data = pgm_read_byte (&BitMap[line+shape*MAX_LINES_PER_FRAME][pixel]);
        //byte data = myArray[line+shape*MAX_LINES_PER_FRAME][pixel]; 
        if (data) 
        {
          lightLines(line,pixel,pos);  
  	}	 
	
        else 
        { 
          extinguishLines(line,pixel,pos);
	}
        
      }
    }        
}

void lightLines(int lineNum, int pixelNum, int offst){
			/*Pixel coordinates (pixelNum,lineNum):*/  
  switch (lineNum){
            case 0:   { lightPixel(516+pixelNum+offst); break; } //line0
            case 1:   { lightPixel(425+pixelNum+offst); break; } //line1
            case 2:   { lightPixel(333+pixelNum+offst); break; } //line2
            case 3:   { lightPixel(608+pixelNum+offst); break; } //line3
            case 4:   { lightPixel(234+pixelNum+offst); break; } //line4
            case 5:   { lightPixel(706+pixelNum+offst); break; } //line5
            case 6:   { lightPixel(52+pixelNum+offst); break; }  //line6
            case 7:   { lightPixel(144+pixelNum+offst); break; } //line7
            default: { break; } //exit
  }
}

void extinguishLines(int lineNum, int pixelNum, int offst){
			/*Pixel coordinates (pixelNum,lineNum):*/  
  switch (lineNum){
            case 0:   { extinguishPixel(516+pixelNum+offst); break; } //line0
            case 1:   { extinguishPixel(425+pixelNum+offst); break; } //line1
            case 2:   { extinguishPixel(333+pixelNum+offst); break; } //line2
            case 3:   { extinguishPixel(608+pixelNum+offst); break; } //line3
            case 4:   { extinguishPixel(234+pixelNum+offst); break; } //line4
            case 5:   { extinguishPixel(706+pixelNum+offst); break; } //line5
            case 6:   { extinguishPixel(52+pixelNum+offst); break; }  //line6
            case 7:   { extinguishPixel(144+pixelNum+offst); break; } //line7
            default: { break; } //exit
  }
}

void lightPixel(int pixelID) {
        if(pixelID > MAX_PIXELS_FRAME-16) {
		return;
	}
	else {
  	  switch (pixelID%16){
              case 0:   { bitWrite(frameBuffer[int(pixelID/16)],   0,  1); break; }
              case 1:   { bitWrite(frameBuffer[int(pixelID/16)],   1,  1); break; }
              case 2:   { bitWrite(frameBuffer[int(pixelID/16)],   2,  1); break; }
              case 3:   { bitWrite(frameBuffer[int(pixelID/16)],   3,  1); break; }
              case 4:   { bitWrite(frameBuffer[int(pixelID/16)],   4,  1); break; }
              case 5:   { bitWrite(frameBuffer[int(pixelID/16)],   5,  1); break; }
              case 6:   { bitWrite(frameBuffer[int(pixelID/16)],   6,  1); break; }
              case 7:   { bitWrite(frameBuffer[int(pixelID/16)],   7,  1); break; }
              case 8:   { bitWrite(frameBuffer[int(pixelID/16)],  8,  1); break; }
              case 9:   { bitWrite(frameBuffer[int(pixelID/16)],  9,  1); break; }
              case 10:  { bitWrite(frameBuffer[int(pixelID/16)], 10, 1); break; }
              case 11:  { bitWrite(frameBuffer[int(pixelID/16)], 11, 1); break; }
              case 12:  { bitWrite(frameBuffer[int(pixelID/16)], 12, 1); break; }
              case 13:  { bitWrite(frameBuffer[int(pixelID/16)], 13, 1); break; }
              case 14:  { bitWrite(frameBuffer[int(pixelID/16)], 14, 1); break; }
              case 15:  { bitWrite(frameBuffer[int(pixelID/16)], 15, 1); break; }
              default: { break; } //exit
           }
      }
}

void extinguishPixel(int pixelID) {
        if(pixelID > MAX_PIXELS_FRAME-16) {
		return;
	}
	else {
  	  switch (pixelID%16){
              case 0:   { bitWrite(frameBuffer[int(pixelID/16)],   0,  0); break; }
              case 1:   { bitWrite(frameBuffer[int(pixelID/16)],   1,  0); break; }
              case 2:   { bitWrite(frameBuffer[int(pixelID/16)],   2,  0); break; }
              case 3:   { bitWrite(frameBuffer[int(pixelID/16)],   3,  0); break; }
              case 4:   { bitWrite(frameBuffer[int(pixelID/16)],   4,  0); break; }
              case 5:   { bitWrite(frameBuffer[int(pixelID/16)],   5,  0); break; }
              case 6:   { bitWrite(frameBuffer[int(pixelID/16)],   6,  0); break; }
              case 7:   { bitWrite(frameBuffer[int(pixelID/16)],   7,  0); break; }
              case 8:   { bitWrite(frameBuffer[int(pixelID/16)],  8,  0); break; }
              case 9:   { bitWrite(frameBuffer[int(pixelID/16)],  9,  0); break; }
              case 10:  { bitWrite(frameBuffer[int(pixelID/16)], 10, 0); break; }
              case 11:  { bitWrite(frameBuffer[int(pixelID/16)], 11, 0); break; }
              case 12:  { bitWrite(frameBuffer[int(pixelID/16)], 12, 0); break; }
              case 13:  { bitWrite(frameBuffer[int(pixelID/16)], 13, 0); break; }
              case 14:  { bitWrite(frameBuffer[int(pixelID/16)], 14, 0); break; }
              case 15:  { bitWrite(frameBuffer[int(pixelID/16)], 15, 0); break; }
              default: { break; } //exit
           }
      }
}

/**********************************************************
function: serialCommandHandler()
reads command from serialCOM and executes the functionality
indicated by the user. 
***********************************************************/
void serialCommandHandler() 
  {
  if (Serial.available()){
        //clear command
        for(int i=0;i<MESSAGE_SIZE+2;i++){
          command[i]=NULL; 
        }
 
        Serial.readBytesUntil('|',command,MESSAGE_SIZE+2);
        switch(command[0]){
           case 'f': //MOTOR ON
               
               if(command[1] == 'w'){
                   //set motor Clock-wise mode-> this is the good one!
                   digitalWrite(MOTOR_R,HIGH);
                   digitalWrite(MOTOR_L,LOW);
               }
               else{
                  digitalWrite(MOTOR_R,LOW);
                  digitalWrite(MOTOR_L,HIGH);                
               }
                   
               motorON = true; //starts motor ISR
               analogWrite(MOTOR_PORT, Output);
              //gets Setpoint from input
               
              Setpoint = byteToInt(command[2])*10000+byteToInt(command[3])*1000+byteToInt(command[4])*100+byteToInt(command[5])*10+byteToInt(command[6]);  
              //Serial.println(Setpoint); 
            break;
            case 'o': //MOTOR OFF
               motorON = false;              
               analogWrite(MOTOR_PORT, 0);
            break;
            case 'l': //LASER ON
               disableLaserModulation();
               PORTD = PORTD | B00001000; 
               //digitalWrite(LASER_PORT, HIGH);
            break;
            case 'k'://LASER OFF
               disableLaserModulation();
               PORTD = PORTD & B11110111;
               //digitalWrite(LASER_PORT, LOW); 
            break;
            case 'm':
             cont_mess_size = 0;
             alpha = false;  
             enableLaserModulation(); 
             clearFrameBuffer();
             mode = command[1];
             //Clear Command
             
             //Clear messageBuffer
             for(int i=0;i<MESSAGE_SIZE;i++){
                  messageBuffer[i]=NULL; 
             }
             
             for(int i; i<MESSAGE_SIZE;i++){
               if(command[i+2]==NULL)
                 break; 
               else{
                 messageBuffer[i] = byteToInt(command[i+2]);
                 cont_mess_size++; 
               }
                   
             }
             displayMessage(messageBuffer,mode, cont_mess_size);          
            break;
            case 's':
              disableLaserModulation();
              alpha = true; 
            break; 
        }
  }
}

/////fill frameBuffer
void displayMessage(unsigned int *messageBuff,byte mode, int cont_mess){
  
  int offs = 0; 
  switch(mode){
    case 'f': //FIXED
      flow = 'FIXED'; 
      for(int i=0;i<cont_mess;i++){
         displayCharacter(offs*CHAR_WIDTH,messageBuff[i]); 
         offs++; 
       }
    break; 
    case 's': //SCROLLING
    
    flow = 'SCROLL'; 
    input_array_index = 0;
    cnt = 0; 
    break; 
    default: 
    break; 
  }
  
}

/***************************
Function: byteToInt(byte val)
Input: byte to set
Output: Int value
Converts a byte value to int
****************************/
int byteToInt(byte val){
  switch(val){
     case '0': 
      return 0;
      break; 
     case '1': 
      return 1; 
      break; 
    case '2': 
      return 2; 
      break;
     case '3': 
      return 3;
      break;  
     case '4':
      return 4; 
      break; 
     case '5': 
      return 5; 
      break; 
     case '6':
      return 6; 
      break; 
     case '7': 
      return 7; 
      break; 
     case '8':
      return 8; 
      break; 
     case '9': 
      return 9; 
      break;
     case ' ': 
      return 10; 
      break;   
     case 'A': 
      return 11;
      break; 
    case 'B': 
      return 12; 
      break; 
    case 'C': 
      return 13; 
      break;
     case 'D': 
      return 14;
      break;  
     case 'E':
      return 15; 
      break; 
     case 'F': 
      return 16; 
      break; 
     case 'G':
      return 17; 
      break; 
     case 'H': 
      return 18; 
      break; 
     case 'I':
      return 19; 
      break; 
     case 'J': 
       return 20; 
      break; 
      case 'K': 
      return 21; 
      break;   
     case 'L': 
      return 22;
      break; 
    case 'M': 
      return 23; 
      break; 
    case 'N': 
      return 24; 
      break;
     case 'O': 
      return 25;
      break;  
     case 'P':
      return 26; 
      break; 
     case 'Q': 
      return 27; 
      break; 
     case 'R':
      return 28; 
      break; 
     case 'S': 
      return 29; 
      break; 
     case 'T':
      return 30; 
      break; 
     case 'U': 
      return 31; 
      break;  
     case 'V': 
      return 32;
      break; 
    case 'W': 
      return 33; 
      break; 
    case 'X': 
      return 34; 
      break;
     case 'Y': 
      return 35;
      break;  
     case 'Z':
      return 36; 
      break; 
     case 'a': 
      return 37; 
      break;
     case 'b': 
      return 38; 
      break; 
     case 'c': 
      return 39; 
      break;
     case 'd': 
      return 40;
      break;  
     case 'e':
      return 41; 
      break; 
     case 'f': 
      return 42; 
      break; 
     case 'g':
      return 43; 
      break; 
     case 'h': 
      return 44; 
      break; 
     case 'i':
      return 45; 
      break; 
     case 'j': 
       return 46; 
      break; 
      case 'k': 
      return 47; 
      break;   
     case 'l': 
      return 48;
      break; 
    case 'm': 
      return 49; 
      break; 
    case 'n': 
      return 50; 
      break;
     case 'o': 
      return 51;
      break;  
     case 'p':
      return 52; 
      break; 
     case 'q': 
      return 53; 
      break; 
     case 'r':
      return 54; 
      break; 
     case 's': 
      return 55; 
      break; 
     case 't':
      return 56; 
      break; 
     case 'u': 
      return 57; 
      break;  
     case 'v': 
      return 58;
      break; 
    case 'w': 
      return 59; 
      break; 
    case 'x': 
      return 60; 
      break;
     case 'y': 
      return 61;
      break;  
     case 'z':
      return 62; 
      break;
     case ';': 
      return 63; 
      break;  
     case ':': 
      return 64;
      break; 
    case ')': 
      return 65; 
      break; 
    case '%': 
      return 66; 
      break;
     case '#': 
      return 67;
      break;  
     case '?':
      return 68; 
      break;
     case '!':
      return 69; 
      break; 
     case '*': 
      return 70; 
      break;  
     case '.': 
      return 71;
      break; 
    case ',': 
      return 72; 
      break; 
    case '+': 
      return 73; 
      break; 
    case '-': 
      return 74; 
      break;
    case '=': 
      return 75; 
      break;              
  }
}

void setup()
{  
  Serial.begin(BAUD); 
  
  pinMode(MOTOR_PORT, OUTPUT);
  pinMode(MOTOR_R, OUTPUT);
  pinMode(MOTOR_L, OUTPUT);
  pinMode(LASER_PORT, OUTPUT);
  pinMode(SENSOR_IN, OUTPUT);
  pinMode(SENSOR_OUT, INPUT);
  
  //set current to IR sensor
  digitalWrite(SENSOR_IN,HIGH);
  
  //Set motor PWM faster 
  //timer1 16-bit 65536 positions, pins: 9,10
  TCCR1A |= (1 << WGM11);    //  TCCR1A = 0b00000011 -> Mode 7: Fast PWM 10-bit 
  TCCR1A |= (1 << WGM10);    //  TCCR1B = 0b00010000
  TCCR1B &= ~(1 << WGM13);    
  TCCR1B |=  (1 << WGM12); 
  
  clearFrameBuffer(); 
  //analogWrite(MOTOR_PORT, 700);
  //enableLaserModulation(); 
  attachInterrupt(0,motorSpeedDetector,FALLING); 
}

void loop()
{
  switch(flow){
    case 'SCROLL':
    if(cnt<MESSAGE_SIZE && messageBuffer[input_array_index]!=NULL){ 
    for(int k=0; k<6; k++)
    {
        for(int l=7; l>=0; l--){
          temp = 7-l; 
            
          if(input_array_index-temp>=0){
           displayCharacter(l*CHAR_WIDTH-k,messageBuffer[input_array_index-temp]);
           //Serial.println(l*CHAR_WIDTH-k); 
           //Serial.print(input_array_index-temp); 
          }
          else{  
           displayCharacter(l*CHAR_WIDTH-k, 10); //VOID CHAR
           //Serial.println("VOID CHAR"+l*CHAR_WIDTH); 
          }
        }
     } 
     cnt = 0; 
     input_array_index++; 
    }
    else if(input_array_index>=MESSAGE_SIZE || messageBuffer[input_array_index]==NULL){
      input_array_index=0; 
    }
    cnt++; 
   
    break; 
    default:
    break; 
  }
  serialCommandHandler(); 
}
