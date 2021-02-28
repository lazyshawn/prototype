## § What is CRC
### 纠错码 vs 检错码
我们进行通信时的网络信道并不总是可靠的。
为了增加可靠性，我们需要在传输数据后加上一些冗余的码字。
如果接收方能够通过它们直接纠正错误，
那么我们就称之为纠错码（Error Correcting Code）。
如果接收方仅能通过它们发现错误，而真正纠正错误的过程需要通知发送方进行重传，
那么我们就称之为检错码（Error Detecting Code）。
我们设计冗余码的原则就是：用最小的代价，检测（纠正）最多的错误。

CRC就是一种优秀的检错码。它的计算原理，说白了就是作长除法。
把比特流看作多项式的系数。设定一个生成多项式（generator polynomial）作为除数。
数据流看作被除数。
发送方需要在数据流末尾加上一段冗余码，使得组合后的新数据流能够整除除数。
这段冗余码就是所谓的CRC。

### Introduction to CRC
循环冗余码校验英文名称为Cyclical Redundancy Check，简称CRC。
它是利用除法及余数的原理来作错误侦测（Error Detecting）的。
实际应用时，发送装置计算出CRC值并随数据一同发送给接收装置，
接收装置对收到的数据重新计算CRC并与收到的CRC相比较，
若两个CRC值不同，则说明数据通讯出现错误。

根据应用环境与习惯的不同，CRC又可分为以下几种标准：

① CRC－12码；② CRC－16码；
③ CRC－CCITT码；④ CRC－32码。

CRC-12码通常用来传送6-bit字符串。
CRC-16 及CRC-CCITT码则是用来传送8-b it字符，
其中CRC-16为美国采用，而CRC-CCITT为欧洲国家所采用。
CRC-32码大都被采用在一种称为Point-to-Point的同步传输中。

## § How to calculate CRC
如何计算？在数据流末尾补CRC长度的0，然后做除法得到的余数就是了。
只是这个除法与普通的十进制除法不同，它是模2除法。
换句话说,CRC的计算就是：
> 基于有限域GF(2)（即除以2的同余）的多项式环。

### 二进制求余 vs 异或
二进制运算中，求余等价于异或运算(最高位对齐)。
如 $110 \% 10 = 010\% 10 = 0$ 。

### 手算步骤
已知：原始数据data, 初始值init, 多项式poly, 结果异或值xorout,
输入数据翻转标志 refin， 输出数据翻转标志 refout.

以CRC-8/MAXIM模型计算0x34的CRC值为例。
```matlab
data = 0x34;  poly = 0x31 (首位1省略:1 0110 0001); init = 0x00;
XOROUT = 0x00; REFIN = TRUE; REFOUT = TRUE;
```
1. data 与 init 异或。init = 0x00, 原始数据高8位和初始值进行异或运算保持不变。
1. 输入翻转。REFIN = TRUE， 输入翻转，即~~0011~~ ~~0100~~ > ~~0010~~ ~~1100~~。
1. data 左移补足CRC位。data 左移8位: 0010 1100 0000 0000。
1. data 与 poly 进行模2除法, 求余数。模2除法取余数低8位：1111 1011。
1. 结果与 xorout 异或。xorout = 0x00, 结果不变。
1. 输出翻转。REFOUT = TRUE, 输出翻转，得1101 1111 = 0xDF。
1. 合并数据帧。data + CRC。

### 算法
CRC-16/MODBUS的多项式为：x16+x15+x2+1（8005），宽度为16。
运算时，首先将一个16位的寄存器预置为0xffff，
然后连续把数据帧中的每个字节中的8位与该寄存器的当前值进行运算。
仅仅每个字节的8位数据位参与生成CRC。

在生成CRC时，每个字节的8位与寄存器中的内容进行异或，
然后将结果向低位位移，高位则用0补充，最低位（LSB）移出并检测，
如果是1，该寄存器就与一个预设的固定值（0A001H: 由8005反转得到）进行一次异或运算，
如果低位为0，不作任何处理。
**Remark:** 注意两点。
1: 反转8005的原因是异或操作不需要借位，因此反转除数或被除数，计算结果不变。
2: 反转除数为8005而非10085是因为每次数据最高位为1，
余数只与次高位之后的数据有关, 即只需要用数据右移一位的结果与A001异或。

上述处理重复进行，直到执行完了8次位移操作，当最后一位移完以后，下一个8位字节与寄存器当前的值进行异或运算，同样进行上述的另一轮8次位移异或才做，当数据帧中的所有字节都做了处理，生成最终值就是CRC值。


## § Reference
1. [CRC（循环冗余校验）在线计算][1]
1. [【基汤】关于CRC（循环冗余校验）][2]
1. [CRC校验原理及实现][3]
1. [C++与JAVA代码实现CRC-16/MODBUS算法][4]

[1]:http://www.ip33.com/crc.html
[2]:https://zhuanlan.zhihu.com/p/81294341
[3]:https://zhuanlan.zhihu.com/p/256487370
[4]:https://www.cnblogs.com/yanwu0527/p/11424730.html
