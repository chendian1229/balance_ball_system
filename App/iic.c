/*
 * 包含头文件
 */
#include "include.h"


/*
 * 注：1.单片机作为主机 只是用程序去操作单片机的GPIO（对应于IIC总线的“SCL线”“SDA线”）
 *       要分清“SCL_GPIO”和“SCL线”的区别（另一个同理）
 *     2.关于GPIO从输入模式变为输出模式后其输出是0还是1还是以前为输出模式时的值
 *       这个问题不用担心 编程时已经绕过了这个问题
 *     3.如果怎么调程序都不出数据 很有可能是总线上缺上拉电阻
 */

//软件IIC相关宏定义         SCL:PTA28 SDA:PTA26
#define SCL_GPIO_INIT       gpio_init(PTA28,GPO,1)//要求初始化GPIO SCL为输出模式 且输出1
#define SCL_GPIO_IN_MODE    gpio_ddr(PTA28,GPI)
#define SCL_GPIO_GET        gpio_get(PTA28)
#define SCL_GPIO_OUT_MODE   gpio_ddr(PTA28,GPO)
#define SCL_GPIO_OUT_0      gpio_set(PTA28,0)
#define SCL_GPIO_OUT_1      gpio_set(PTA28,1)


#define SDA_GPIO_INIT       gpio_init(PTA26,GPO,1)//要求初始化GPIO SDA为输出模式 且输出1
#define SDA_GPIO_IN_MODE    gpio_ddr(PTA26,GPI)
#define SDA_GPIO_GET        gpio_get(PTA26)
#define SDA_GPIO_OUT_MODE   gpio_ddr(PTA26,GPO)
#define SDA_GPIO_OUT_1      gpio_set(PTA26,1)
#define SDA_GPIO_OUT_0      gpio_set(PTA26,0)

#define IIC_DELAY           DELAY_US(10)

/**********************************************************************************************
                                  软件IIC GPIO初始化
**********************************************************************************************/
void IIC_Init()
{
    //当总线空闲时 两根线均为高电平

    SCL_GPIO_INIT;//SCL_GPIO 输出模式 且输出高电平
    SDA_GPIO_INIT;//SDA_GPIO 输出模式 且输出高电平
}
/**********************************************************************************************
                                      IIC_Start
**********************************************************************************************/
void IIC_Start()
{
    //SCL_GPIO和SDA_GPIO均输出1 模拟总线空闲状态
    SCL_GPIO_OUT_1;
    SDA_GPIO_OUT_1;
    IIC_DELAY;
    SDA_GPIO_OUT_0;//起始信号
    IIC_DELAY;
    SCL_GPIO_OUT_0;
    IIC_DELAY;
}
/**********************************************************************************************
                                     IIC_SendByte
**********************************************************************************************/
uint8 IIC_SendByte(uint8 byte)
{
    uint8 ack_flag;//从机应答标志位 1从机已应答 0从机未应答
    uint8 i;       //用于for循环


    for(i=0;i<8;i++)
    {
        if( ((byte<<i)&0x80)==0x80 )//MSB先传送
        {//说明该位为1
            SDA_GPIO_OUT_1;
        }
        else
        {//说明该位为0
            SDA_GPIO_OUT_0;
        }
        SCL_GPIO_OUT_1;
        IIC_DELAY;
        SCL_GPIO_OUT_0;
        IIC_DELAY;
    }

    //发送完一个字节数据后释放SDA线 SDA_GPIO置为输入模式 准备接受来自从机的应答信号
    SDA_GPIO_IN_MODE;
    //第9个时钟信号 准备接受来自从机的应答信号
    SCL_GPIO_OUT_1;
    IIC_DELAY;
    if( SDA_GPIO_GET==1 )//从机没响应
        ack_flag=0;
    else                 //从机响应了
        ack_flag=1;
    SCL_GPIO_OUT_0;   //从机响应信号接收完毕
    SDA_GPIO_OUT_MODE;//SDA_GPIO恢复输出模式
    SDA_GPIO_OUT_1;   //且输出1
    IIC_DELAY;


    if( ack_flag==0 )
        return 0;
    else
        return 1;
}
/**********************************************************************************************
                                    IIC_ReceiveByte_Ack
**********************************************************************************************/
uint8 IIC_ReceiveByte_Ack()
{
    uint8 datA;//接收到数据
    uint8 i;   //用于for循环


    SDA_GPIO_IN_MODE;//SDA输入模式 准备接受数据
    for(i=0;i<8;i++)//MSB先传送
    {
        SCL_GPIO_OUT_1;//开始接受数据
        IIC_DELAY;
        datA=datA<<1;
        if( SDA_GPIO_GET==1 ) datA=datA+1;
        SCL_GPIO_OUT_0;
        IIC_DELAY;
    }
    SDA_GPIO_OUT_MODE;//恢复SDA输出模式 准备发送向从机发送响应信号
    SDA_GPIO_OUT_0;//发送响应信号
    SCL_GPIO_OUT_1;
    IIC_DELAY;
    SCL_GPIO_OUT_0;
    SDA_GPIO_OUT_1;
    IIC_DELAY;


    return datA;
}
/**********************************************************************************************
                                    IIC_ReceiveByte_NoAck
**********************************************************************************************/
uint8 IIC_ReceiveByte_NoAck()
{
    uint8 datA;//接收到数据
    uint8 i;   //用于for循环


    SDA_GPIO_IN_MODE;//SDA输入模式 准备接受数据
    for(i=0;i<8;i++)//MSB先传送
    {
        SCL_GPIO_OUT_1;//开始接受数据
        IIC_DELAY;
        datA=datA<<1;
        if( SDA_GPIO_GET==1 ) datA=datA+1;
        SCL_GPIO_OUT_0;
        IIC_DELAY;
    }
    SDA_GPIO_OUT_MODE;//恢复SDA输出模式 准备发送向从机发送响应信号
    SDA_GPIO_OUT_1;//发送不响应信号
    SCL_GPIO_OUT_1;
    IIC_DELAY;
    SCL_GPIO_OUT_0;
    SDA_GPIO_OUT_1;
    IIC_DELAY;


    return datA;
}
/**********************************************************************************************
                                       IIC_Stop
**********************************************************************************************/
void IIC_Stop()
{
    SDA_GPIO_OUT_0;
    SCL_GPIO_OUT_1;
    IIC_DELAY;
    SDA_GPIO_OUT_1;
    IIC_DELAY;
    //SCL SDA均置高 总线空闲状态
}