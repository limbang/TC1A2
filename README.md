[![HF-LPT230](https://img.shields.io/badge/芯片-HF--LPT230-blue.svg)](http://www.hi-flying.com/iot-module/hf-lpt230)[![BL0937](https://img.shields.io/badge/芯片-BL0937-blue.svg)](http://www.belling.com.cn/product_info.html?id=139)[![GCCj](https://img.shields.io/badge/环境-Keil%205%20+%20GCC-green.svg)](https://pan.baidu.com/s/1nvHUQ1b)[![SecureCRT](https://img.shields.io/badge/烧录-SecureCRT-9cf.svg)](https://www.vandyke.com/download/index.html)[![downloads](https://img.shields.io/github/release/limbang/TC1A2.svg)]()![](http://progressed.io/bar/1?title=Progress)

**因疫情原因还有功能未完善，也没办法测试硬件，有机会在继续**

## 项目介绍

因为斐讯TC1智能插板服务器关闭，不能使用APP控制，特针对`HF-LPT230`芯片重新写的`SDK`，使用`MQTT `自动发现连接到`Home Assistance`

## 拆装连线方法

### 0.TC1外观

<img src=".\img\1.jpg" style="zoom:50%"><img src=".\img\2.jpg" style="zoom:50%">

### 1.先扭掉螺丝,红色区域拍2下,绿色区域拍4下,就可以拿下来了

<img src=".\img\3.jpg" style="zoom:50%">

### 2.烧录连线

普通`USB转TTL`就可以了,可以直接手按着连3.3v那`4个测试点`烧录,我是因为调试所以焊上去的

<img src=".\img\4.jpg" style="zoom:20%">

### 3.使用 SecureCRT 软件烧录,设置如下

- Baud rate(波特率)：230400
- Data bits(数据位)：8
- Parity(奇偶校验): None
- Stop bits(停止位):1
- X/Y/Zmodem 可以选择为 1024 bytes 提高发送速度
  <img src=".\img\5.png" style="zoom:70%"><img src=".\img\8.png" style="zoom:70%">

### 4.烧录

光标停在 SecureCRT 软件窗口，按住 TC1 插版的`按键`不要松开，拿上图蓝色的GND线触碰10引脚后，按键盘空格（必须触碰后马上按），会出现如下图显示，输入`S`在输入`Y`后，显示`cccc`，就可以如下图选择发送固件烧录进去

<img src=".\img\6.png" style="zoom:70%"><img src=".\img\7.png" style="zoom:70%">

## 芯片管脚功能表
### HF-LPR230

| 管脚 |    名称    | 信号类型 |  GPIO  |     插板功能     |
| :--: | :--------: | :------: | :----: | :--------------: |
|  1   |  SPI_MOSI  |  IPD/O   | GPIO12 | ~~切换电流电压~~ |
|  2   |  SPI_CLK   |  IPD/O   | GPIO4  |   ~~电流电压~~   |
|  3   |  SPI_MISO  |  IPD/O   | GPIO7  |      PW功率      |
|  4   |   SPI_CS   |  IPD/O   | GPIO5  |       LED        |
|  5   |  UART0_TX  |   0,PU   | GPIO2  |    TX(测试点)    |
|  6   |  UART0_RX  |    I     | GPIO1  |    RX(测试点)    |
|  7   | UART0_CTS  |  IPU/O   | GPIO22 |       SW1        |
|  8   | UART0_RTS  |  IPU/O   | GPIO23 |       SW4        |
|  9   |    ADC     |  IPD/O   | GPADC0 |       SW5        |
|  10  | EXT_RESETn |   I,PU   |        |                  |
|  11  |   nReady   |  IPU/O   | GPIO24 |       SW2        |
|  12  |  nReload   |  IPU/O   | GPIO25 |       KEY        |
|  13  |   nLink    |  IPD/O   | GPIO8  |       SW3        |
|  14  |   GPIO3    |  IPU/O   | GPIO3  |       SW6        |
|  15  |   +3.3V    |  Power   |        |   3.3V(测试点)   |
|  16  |    GND     |  Power   |        |   GND(测试点)    |
### BL0937

| 管脚 | 名称 | 信号类型 |     功能      |     插板功能     |
| :--: | :--: | :------: | :-----------: | :--------------: |
|  1   | VDD  |  Power   | 芯片电源3.3v  |                  |
|  2   |  IP  |    I     | 电流采样信号  |                  |
|  3   |  IN  |    I     | 电流采样信号  |                  |
|  4   |  VP  |    I     | 电压采样信号  |                  |
|  5   | GND  |  Power   |    芯片地     |                  |
|  6   |  CF  |    O     | 有功功率脉冲  |      PW功率      |
|  7   | CF1  |    O     | 电压/电流脉冲 |   ~~电流电压~~   |
|  8   | SEL  |    I     | 选择CF1输出值 | ~~切换电流电压~~ |

## 功能扩展
### 下面是我参照PCB画的原理图,想要有电压电流显示的需要把红框缺的元件补上去
<img src=".\img\9.png" style="zoom:70%">
<img src=".\img\10.png" style="zoom:66%"><img src=".\img\11.png" style="zoom:55%">