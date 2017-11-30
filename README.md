# 绝影

---

绝影（英文名Prestissimo）是一个应用于移动端的轻量级高性能神经网络前向计算框架。ios, android 上的速度均快于目前的开源框架（ncnn、mdl 等等）。

---

## 功能概述

### 基础功能
* 支持卷积神经网络，支持多输入和多分支结构
* 精炼简洁的API设计，使用方便
* 提供调试接口，支持打印各个层的数据以及耗时
* 不依赖任何第三方计算框架，整体库体积 500K 左右（32位 约400k，64位 约600k）
* 纯 C++ 实现，跨平台，支持 android 和 ios
* 模型为纯二进制文件，不暴露开发者设计的网络结构

### 极快的速度
* 大到框架设计，小到汇编书写上全方位的优化，iphone7 上跑 SqueezeNet 仅需 26ms（单线程）
* 支持浮点(float)和整型(int)两种运算模式，float模式精度与caffe相同，int模式运算速度快，大部分网络用int的精度便已经足够
* 以巧妙的内存布局提升cpu的cache命中率，在中低端机型上性能依然强劲
* 针对 float-arm32, float-arm64, int-arm32, int-arm64 四个分支均做了细致的优化，保证arm32位和arm64位版本都有非常好的性能



#### SqueezeNet-v1.1 测试结果
**Note**: 手机测试性能存在一定的抖动，连续多次运算取平均时间

**Note**: 像华为mate8, mate9，Google nexus 6 虽然是64位的CPU，但测试用的是 32位的库，因此cpu架构依然写 arm-v7a

|CPU架构|机型|CPU|ncnn（4线程）|mdl|Prestissimo_float_st|Prestissimo_int_st|
|:---:|:----:|:----:|:----:|:----:|:----:|:----:|
|arm-v7a|小米2|高通APQ8064 1.5GHz | 185 ms| 370 ms|184 ms| 115 ms|
|arm-v7a|小米2s| 四核 骁龙APQ8064 Pro 1.7GHz | 166 ms| - | 136 ms |96 ms |
|arm-v7a|红米Note 4x|骁龙625 四核2.0GHz|124 ms |306 ms |202 ms| 110 ms|
|arm-v7a|Google Nexus 6| 骁龙805 四核 2.7GHz|84 ms | 245 ms | 103 ms |  63 ms |
|arm-v7a|Vivo x6d |联发科 MT6752 1.7GHz | 245 ms | 502 ms | 370 ms | 186 ms |
|arm-v7a|华为 Mate 8| 海思麒麟950 4大4小 2.3GHz 1.8GHz | 75 ms | 180 ms | 95 ms | 57 ms |
|arm-v7a|华为 Mate 9| 海思麒麟960 4大4小 2.4GHz 1.8GHz | 61 ms | 170 ms | 94 ms | 48 ms |
|arm-v8|iphone7|Apple A10 Fusion 2.34GHz|- |- |27 ms|26 ms|

### 未开放特性

* 多核并行加速（多核机器可以再提升30%-100% 的速度）
* depthwise卷积运算（支持mobilenet）
* 模型压缩功能，压缩后的模型体积可缩小到20%以下
* GPU 运算模式（Android 基于opengl es 3.1，ios 基于metal）

### 同类框架对比
| 框架    | caffe | tensorflow |mdl-android |mdl-ios| ncnn | CoreML | Prestissimo |
|:---:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|
| 计算硬件| cpu |  cpu  | cpu | gpu |cpu | gpu | cpu （gpu版本未开放）|
| 计算速度 | 慢 | 慢 |慢|很快| 很快 |极快 | 极快 |
| 库大小 | 大 | 较大 |中等|小| 小 | 小 | 小 | 
| 兼容性 | 好 | 好 |好|限ios8以上| 很好 | 仅支持 ios11 | 很好 | 
|模型支持度|很好|好 | - | 差（仅限指定模型）|较好|-|中等（当前版本不支持mobilenet）|

## 使用方法

### 库文件
绝影相应的库见 ./prestissimo 目录

主要API接口见 ./prestissimo/include/InFactoryApi.h 和 ./prestissimo/include/InNetApi.h 

对android ios 均提供 32/64 位的库

* [模型支持说明](document/layerSupport.md)
* [数据格式说明](document/image.md)

### Demo 工程
IOS 工程见 ./ios/Detect/Detect.xcodeproj ，仅包含基本运行与速度测试

Android 工程见 ./android/Detect/ ，包含基本运行，速度测试，以及图像识别

### 模型转换工具
绝影支持的是私有的模型文件格式，需要把 caffe 训练出来的模型转换为 .prestissimo 格式，模型转换工具在 tools 目录下，为 caffe2Prestissimo.out。

caffe2Prestissimo.out 依赖 protobuf 3.30，请先安装 Google 的 protobuffer，如果您的电脑上已经安装过 caffe，这个应该已经安装过了。

将 XXX.prototxt 和 YYY.caffemodel 转化为 Prestissimo 模型 ZZZ.prestissimo：

./caffe2Prestissimo.out XXX.prototxt YYY.caffemodel ZZZ.prestissimo

## 开发人员
* 小棠 (jxt1234@zju.edu.cn)
* 咖啡 (kafei@in66.com)
* 汤圆
* 乐涤
* 晨晨
* 可召
* 芹菜
