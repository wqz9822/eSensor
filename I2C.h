//==============宏定义==========================//
#define DATA_1 PORTE|=0X20  //PE5
#define DATA_0 PORTE&=0XDF //PE5
#define SCK_1 PORTE|=0X10   //PE4
#define SCK_0 PORTE&=0XEF  //PE4
#define DATA_SETINPUT DDRE&=0XDF;PORTE|=0X20
#define DATA_SETOUTPUT DDRE|=0X20
#define SCK_SETINPUT DDRE&=0XEF;PORTE|=0X10
#define SCK_SETOUTPUT DDRE|=0X10
#define DATA_IN (PINE & 0X20)
#define noACK 0
#define ACK 1


//===========adr command r/w ============
#define STATUS_REG_W 0x06  //000   0011   0
#define STATUS_REG_R 0x07  //000   0011   1
#define MEASURE_TEMP 0x03  //000   0001   1
#define MEASURE_HUMI 0x05  //000   0010   1
#define RESET 0x1e    //000   1111   0

#define TEMP 0
#define HUMI 1


//===================函数声明==================================

void s_transstart(void);                 //启动传输
char s_write_byte(unsigned char value);  //写入一个字节的命令，并且检查传感器是否正确接收了这个数据，返回值为0表示正确接收
char s_read_byte(unsigned char ack);     //读一个字节的数据，并且向传感器发出一个字节的“已接收”信号
int s_measure(unsigned char mode);       //进行一次测量 Command: TEMP HUMI



