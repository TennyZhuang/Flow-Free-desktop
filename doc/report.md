# Flow Free Desktop 设计文档
	
<br><br><br><br>

2014012056

庄天翼 

<zty@gmail.com>

2015年8月28日


## 目录
1. 概览
	1. 游戏简介
	2. 游戏特性
	3. 游戏说明
2. 项目结构
	1. 文件目录
	2. 项目概览
	3. 游戏逻辑
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

玩家连接相同颜色的两个彩球，当且仅当所有彩球都连接且所有路线覆盖地图时，通关。







