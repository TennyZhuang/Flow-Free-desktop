# Flow Free Desktop 设计文档
	
<br><br><br><br>

2014012056

庄天翼 

<zty0826@gmail.com>

2015年8月28日


## 目录
1. 概览
	1. 游戏简介
	2. 游戏说明
	3. 游戏界面介绍
2. 项目结构
	1. 文件目录
	2. 项目概览
3. 实现细节
	1. 游戏运行逻辑
	2. 主要信号槽
	3. 性能优化
4. 游戏运行方式


	
## 概览

### 游戏简介

> Flow Free 是一款简单但令人着迷的益智游戏。
> 
> 连接颜色匹配的管线，建立水流通道。 在 Flow Free 中，请将所有颜色进行配对，使管线覆盖整个区域，即可顺利过关。 请注意，如果发生交叉或重叠，管线将会破裂！		

![image](http://ww4.sinaimg.cn/mw690/da7720a3gw1eviar0ngs3j208w0fsdgl.jpg)

Flow Free 目前有 Android 版和 iOS 版，在各大应用市场均可免费下载，本项目使用 Qt 制作了 Flow Free 桌面版，支持 Windows、Mac OS X 和 Linux 操作系统。

项目开源链接 <https://github.com/TennyZhuang/Flow-Free-desktop>

本项目目前移植了 Flow Free 的 Regular Pack 模式，目前共四个难度十关，游戏地图分别为 5 * 5 到 7 * 7 各三关，8 * 8 一关（便于展示双线程），其中第 1、2、3、4 关有五色彩球，第 5、6、7 关有六色彩球，第 8、9、10 关有七色彩球。

本项目在原游戏的基础上，本项目提供了自动求解的功能，但推荐玩家优先自行通关。

### 游戏说明

游戏的基本模式与移动端相同。

玩家连接相同颜色的两个彩球，当且仅当所有彩球都连接且所有管道覆盖地图时，通关。

管道之间不能重叠，交叉，当连接的管道撞到已连接的管道时，管道会破裂。

可以设置音效。

### 游戏界面介绍

![game screenshot](https://s3.amazonaws.com/f.cl.ly/items/1M0m1v2V0X0p370d1Y1q/flow-free-screenshot.png)

![game screenshot2](https://s3.amazonaws.com/f.cl.ly/items/3o0n0R2Z1w3m2b2Z1I1e/693927C3-70D8-44E7-9A9F-96C2B0F7632A.png?t=1440911217736)

1. 游戏计时器：记录玩家加载关卡后用的的时间。
2. 音效设置：玩家可以自由开启或者关闭音效，默认开启。
3. 游戏主界面：游戏相关的所有操作限制在游戏主界面内，主界面固定大小 400 * 400
4. 上一关按钮：加载上一关，在第一关时默认不可点按。
5. 刷新按钮：重新加载本关。
6. 下一关按钮：加载下一关，最后一关时默认不可点按。
7. 显示结果按钮：结果仅显示一次，在计算线程返回结果后可以点按。
8. 完成窗口：显示花费的移动操作数和时间。




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
	+ `GameSolver` 是自动求解的算法类，继承自 `QThread`，`GameSolver` 的实例在加载关卡时被创建并在玩家游戏的同时计算当前关卡的解，提供了获取解的接口。

3. 其他文件
	`common.h` 定义了项目共用的常数等信息。

4. 资源文件
	`data/data.json` 通过 JSON 的形式记录了所有关卡的数据，由 `GameModel` 负责解析。
	`sounds/*.wav` 音效文件

## 实现细节

### 游戏运行逻辑

程序打开后，加载主窗口，同时从 json 文件中读取游戏的关卡数据，创建 `Level` 的实例并加载第一关。

主线程：加载关卡时开始计时，并启动自动求解线程。用户点按 `Result` 按钮时将计算线程输出的结果显示。用户加载下一关卡时终止计算线程。

自动求解线程：加载关卡后被启动，计算**完成**后将 `Result` 按钮设为可点按，

### 主要信号槽


	connect(this, SIGNAL(loadLevel(quint32)),
    	    ui->gameScene, SLOT(onLoadLevel(quint32)));

+ 位置：`flowfree.cpp`
+ 功能：发送加载关卡 id 给游戏主窗口


---


	connect(ui->lastLevelButton, &QPushButton::clicked,
   	[=]() {
   	emit loadLevel(--currentLevelId);
        ui->resultButton->setEnabled(false);
    	ui->lastLevelButton->setEnabled(currentLevelId != 1);
	    ui->nextLevelButton->setEnabled(currentLevelId != gameSize);
    });

+ 位置：`flowfree.cpp`
+ 功能：`发送加载上一关的信号，更新当前关卡 ID，更新按钮状态。

---

	connect(ui->reloadButton, &QPushButton::clicked,
    [=]() {
        emit loadLevel(currentLevelId);
        ui->resultButton->setEnabled(false);
    });
    
+ 位置：`flowfree.cpp`
+ 功能：发送加载当前 ID 的信号，将 `Result` 按钮设为不可点按

---

	connect(ui->nextLevelButton, &QPushButton::clicked,
    [=]() {
        emit loadLevel(++currentLevelId);
        ui->resultButton->setEnabled(false);
        ui->lastLevelButton->setEnabled(currentLevelId != 1);
        ui->nextLevelButton->setEnabled(currentLevelId != gameSize);
    });
    
+ 位置：`flowfree.cpp`
+ 功能：加载下一关，同上一关

---

    connect(ui->resultButton, &QPushButton::clicked,
            ui->gameScene, &GameScene::autoSolve);

+ 位置：`flowfree.cpp`
+ 功能：向主场景发送将自动求解线程计算返回的结果显示在界面上的信号。

---

	connect(ui->resultButton, &QPushButton::clicked,
    [=]() {
        ui->resultButton->setEnabled(false);
    });
    
+ 位置：`flowfree.cpp`
+ 功能：在 `Result` 按钮被点按后将其设为不可点按。

---

	connect(ui->gameScene, &GameScene::nextLevel,
    [=]() {
        ui->nextLevelButton->click();
    });
    
+ 位置：`flowfree.cpp`
+ 功能：玩家通过完成对话框点按下一关按钮后加载下一关。

---

	connect(ui->soundCheckBox, &QCheckBox::stateChanged,
            ui->gameScene, &GameScene::setSound);

+ 位置：`flowfree.cpp`
+ 功能：向主场景发送音效设置的信号。

---

	connect(ui->gameScene, SIGNAL(timeChanged(int)),
            ui->lcdNumber, SLOT(display(int)));
            
+ 位置：`flowfree.cpp`
+ 功能：更新时间

---

	connect(ui->gameScene, &GameScene::enableResultButton,
    [=]() {
        ui->resultButton->setEnabled(true);
    });
    
+ 位置：`flowfree.cpp`
+ 功能：接收到来自主场景的信号后将 `Result` 按钮设为可以点按。

---

	connect(timer, &QTimer::timeout,
    [=]() {
        emit timeChanged(++currentTime);
    });
    
+ 位置：`gamescene.cpp`
+ 功能：每秒发送更新时钟的信号

---

	connect(this, SIGNAL(completeWith(int,int)),
            dialog, SLOT(changeText(int,int)));
            
+ 位置：`gamescene.cpp`
+ 功能：用户通关后将用户的通关数据发给 完成对话框。

---

	connect(solver, &GameSolver::finished,
            this, &GameScene::onSolved);
            
+ 位置：`gamescene.cpp`
+ 功能：计算线程完成后将计算获得的结果发送给主线程。



### 性能优化

1. 在管道类和格点类保存相同的冗余的信息来保证快速访问，减少鼠标移动事件和绘图事件中遍历的次数，将鼠标移动事件的时间复杂度降为 O(1)。

2. 自动求解算法：基于深度遍历优先的暴搜算法。

3. 双线程非阻塞运算：为可能耗费大量时间甚至造成程序未响应的计算新开一个线程，避免用户的游戏体验受到影响。用户可以在计算的同时进行游戏，并在按下 `Result` 按钮的第一时间得到答案。

## 运行方式

#### 方法一

在 QtCreator 中打开目录，运行项目。

#### 方法二

	cd Flow-Free-desktop && qmake && make


注：编译时间可能**较长**


