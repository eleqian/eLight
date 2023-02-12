# eLight手电

## 功能
- 支持聚光灯和泛光灯分别打开和无极调光，无频闪；
- 支持爆闪和SOS模式，闪烁频率可调；
- 支持低电量自动提醒和关闭，支持手动查看电量；
- 支持充电宝功能，5V/2A输入和输出;
- 支持锁定按键；
- 支持强制复位；
- 支持ISP升级。

## 单片机
- STC8G1K08A，SOP8，10位ADC，PWM输出，宽电压范围，掉电模式唤醒；
- MCU主频4M，输出8位PWM，PWM频率约15.6k，gamma矫正后32级亮度。

## 按键操作
- 关状态：短按打开聚光灯，双击打开泛光灯，长按查看剩余电量；
- 聚光灯和泛光灯状态：短按关，双击调到最大亮度，3击打开对应灯爆闪，长按连续调亮度，每次按键切换调亮暗方向；
- 爆闪状态：短按切换SOS，再短按关，爆闪和SOS固定最大亮度，长按切换频率或间隔；
- 锁定和解锁：连续4击；
- 任意状态超长按强制复位。

## 按键定义
- 短按：50～500ms，长按：>500ms（之后每50ms连发），超长按：>8s，连击（连按2~4次）：相邻两次短按间隔<250ms；
- 每次短按立即处理以提高响应速度，连击重复处理问题依靠状态机设计保证最终状态正确：

    |       | 单击   | 双击  | 3击   | 4击   | 长按   | 超时  |
    | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
    | off   | on,聚光 | on,高亮 |    |       | bat   |       |
    | on    | off   | on,泛光 | flash |     | 调光  |       |
    | flash | sos   |       |       | lock  | 调频  |       |
    | sos   | off   | on,高亮 |     |       | 调频  |       |
    | lock  |       |       |       | off   |       |       |
    | bat   |       |       |       |       |       | off   |

## 状态记忆、复位和固件升级
- 调整的值自动记忆到RAM，超长按强制复位MCU，会自动清除状态；
- 复位后会先经过ISP模式，可以由STC ISP软件进行升级。

## 电量提醒和保护
- 长按查看时根据电压低亮度点亮泛光灯0.01~1.5s后关闭；
- 电压<3.3v为低电量，正常模式每隔10s闪烁2次提醒，爆闪和SOS模式不提示；
- 电压<2.9v自动关闭，如果再打开只闪烁一次；
- 电压判断有0.1v滞回，即电压上升时需要到3.4v或3v。

## 低功耗
- 关状态延时2s MCU进掉电模式，按键中断唤醒；
- 开状态不能停PWM时钟，进空闲模式，每0.5s测量电压。

## led选择
- 聚光灯单个1w led，泛光灯3个0.5w 5730，电流降额驱动可以提高光效，相同功率更高亮度。

## led驱动
- pam2804/sy8006/ln2401等开关降压恒流IC，效率>90%，可实现DC调光和PWM调光；
- DC调光由PWM滤波为DC后进FB控制，低亮度时误差大，但电流小光效更高；
    + PWM经1级RC滤波（10k-1uf），再经过200k-10k与电流采样叠加进FB，实现10%～100%恒流；
    + 爆闪模式可以>100%电流实现更高亮度，由于是间歇打开，散热可以满足；
    + 需要根据电源电压实时调整PWM占空比，以实现滤波后DC电压恒定；
- PWM调光可实现更低亮度，恒流时不用根据电压调占空比，可选开关EN或MOS；
    + 开关EN慢启动时间过长导致频率较低（<1kHz）；
    + 开关MOS打开时有脉冲电流，需要加入us级慢启动，例如使用开漏+上拉驱动。