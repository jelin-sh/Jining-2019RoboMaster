//
// Created by Administrator on 2019/5/4.
//

#ifndef INFENTERY_COURTSYSTEM_H
#define INFENTERY_COURTSYSTEM_H

#include "stm32plus.h"

#define COURT_UART_HANDLER huart7

#define COURT_CMD_GAMESTATE                       0x0001              /*比赛状态数据，1Hz周期发送*/
#define COURT_CMD_GAMERESULT                      0x0002              /*比赛结果数据，比赛结束后发送*/
#define COURT_CMD_GAMEROBOTSURVIVORS              0x0003              /*比赛机器人存活数据，1Hz 周期发送*/
#define COURT_CMD_ENVENTDATA                      0x0101              /*场地事件数据，事件改变后发送*/
#define COURT_CMD_SUPPLYPROJECTILEACTION          0x0102              /*场地补给站动作标识数据，动作改变后发送*/
#define COURT_CMD_SUPPLYPROJECTILEBOKING          0x0103              /*请求补给站补弹数据，由参赛队发送，上限 10Hz。*/
#define COURT_CMD_GAMEROBOTSTATE                  0x0201              /*机器人状态数据，10Hz 周期发送*/
#define COURT_CMD_POWERHEATDATA                   0x0202              /*实时功率热量数据，50Hz 周期发送*/
#define COURT_CMD_GAMEROBOTPOS                    0x0203              /*机器人位置数据，10Hz 发送*/
#define COURT_CMD_BUFFMUSK                        0x0204              /*机器人增益数据，增益状态改变后发送*/
#define COURT_CMD_AERIALROBOTENERG                0x0205              /*空中机器人能量状态数据，10Hz 周期发送，只有空中机器人主控发送*/
#define COURT_CMD_ROBOTHURT                       0x0206              /*伤害状态数据，伤害发生后发送*/
#define COURT_CMD_SHOOTDATA                       0x0207              /*实时射击数据，子弹发射后发送*/

class CourtSystem
{
public:
    struct ext_game_robot_state_t
    {
        uint8_t robot_id;
        uint8_t robot_level;
        uint16_t remain_HP;
        uint16_t max_HP;
        uint16_t shooter_heat0_cooling_rate;
        uint16_t shooter_heat0_cooling_limit;
        uint16_t shooter_heat1_cooling_rate;
        uint16_t shooter_heat1_cooling_limit;
        uint8_t mains_power_gimbal_output : 1;
        uint8_t mains_power_chassis_output : 1;
        uint8_t mains_power_shooter_output : 1;
    };
    struct ext_power_heat_data_t
    {
        uint16_t chassis_volt;
        uint16_t chassis_current;
        float chassis_power;
        uint16_t chassis_power_buffer;
        uint16_t shooter_heat0;
        uint16_t shooter_heat1;
    } ;
public:
    class CourtRxData
    {
    private:
        struct frame_header
        {
            u8 SOF;
            u8 data_lengthL;
            u8 data_lengthH;
            u8 seq;
            u8 CRC8;
        };

        frame_header header;
        u8 frame_id[2];
        u8 data[50];
        u8 frame_tail[2];

        u16 data_length;
        u16 cmd;
        bool ready;
    public:
        void DecodeData(u8 data);

        u8* GetData();
        u16 GetCMD();
        void ClearData();
        bool Ready();

    };
    static void Init();
    static void SerialPortRxCallBack();
    static ext_power_heat_data_t* GetPowerHeatData();
};


#endif //INFENTERY_COURTSYSTEM_H
