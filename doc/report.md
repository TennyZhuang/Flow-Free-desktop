# Flow Free Desktop 设计文档
	
<br><br><br><br>

2014012056

庄天翼 

<zty@gmail.com>

2015年8月28日


## 目录
1. 概览
	1. 游戏简介
	2. 游戏说明
2. 项目结构
	1. 文件目录
	2. 项目概览
3. 实现细节
	


	
## 概览

### 游戏简介

> Flow Free 是一款简单但令人着迷的益智游戏。
> 
> 连接颜色匹配的管线，建立水流通道。 在 Flow Free 中，请将所有颜色进行配对，使管线覆盖整个区域，即可顺利过关。 请注意，如果发生交叉或重叠，管线将会破裂！		

![image](http://ww4.sinaimg.cn/mw690/da7720a3gw1eviar0ngs3j208w0fsdgl.jpg)

Flow Free 目前有 Android 版和 iOS 版，在各大应用市场均可免费下载，本项目使用 Qt 制作了 Flow Free 桌面版，支持 Windows、Mac OS X 和 Linux 操作系统。

项目开源链接 <https://github.com/TennyZhuang/Flow-Free-desktop>

本项目目前移植了 Flow Free 的 Regular Pack 模式，目前共三个难度九关，游戏地图分别为 5 * 5 到 7 * 7 各三关，其中第 1、2、3、4 关有五色彩球，第 5、6、7 关有六色彩球，第 8、9 关有七色彩球。

本项目在原游戏的基础上，本项目提供了自动求解的功能，但推荐玩家优先自行通关。

### 游戏说明

游戏的基本模式与移动端相同。

玩家连接相同颜色的两个彩球，当且仅当所有彩球都连接且所有管道覆盖地图时，通关。

管道之间不能重叠，交叉，当连接的管道撞到已连接的管道时，管道会破裂。

可以设置音效。



## 项目结构

### 文件目录


项目根文件下的目录结构：

+ Flow-Free-desktop.pro  `// 项目配置文件 `
+ README.md  `// 项目简介 `
+ doc  ` // 设计文档 `
+ src  ` // 项目源码目录 `
+ res  ` // 资源文件目录 `


### 项目概览

1. 界面类与控制器类（View and Controller）： 
	+ `FlowFree` 是游戏的主窗口控制器类，共三个文件，`flowfree.h`、 `flowfree.cpp`、 `flowfree.ui`，主要负责游戏的关卡切换，游戏按钮和界面之间的信号连接。通过编译 `flowfree.ui` 生成的 `UI::FlowFree` 为游戏主窗口的界面类。
	+ `GameScene` 是游戏主场景的控制器类，继承自 `QWidget`，是游戏的主体部分，负责处理游戏逻辑及与用户的交互等。通过编译 `GameScene` 生成的 `UI::GameScene` 是主场景的界面类。
	+ `CompleteDialog`是自定义对话框，在用户通关时显示，继承自 `QDialog`。
	
2. 模型类（Model）
	+ `GameModal` 是游戏的主模型，用了单件（Singleton）设计模式，保存游戏的各种元素模型的集合，处理数据读取和向其他模块提供数据获取接口。
	+ `Level` 是游戏关卡抽象的模型类，记录关卡数据，为了便于从文件读取数据，声明了 `GameModel` 为友元类。`Level` 类负责向 `GameScene` 提供只读接口，保护关卡数据仅在文件输入时被修改。
	+ `GamePoint` 是每个网格抽象的模型类，记录当前点的颜色，终点等数据。
	+ `GameRoute` 是每根管道抽象的模型类，记录管道的颜色，点集合等数据，向 `GameScene` 提供只读和必要的修改接口。

3. 其他文件
	`common.h` 定义了项目共用的常数等信息。

4. 资源文件
	`data/data.json` 通过 JSON 的形式记录了所有关卡的数据，由 `GameModel` 负责解析。
	`sounds/*.wav` 音效文件

## 实现细节

