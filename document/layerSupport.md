# 绝影各个Layer支持情况
## Convolution (卷积层)
### 支持项
dilation, stride, pad, kernel_size 均正常支持

若无常数项，caffe2Pretissmo 会自动产生全为零的常数项

### 不支持项
group （group必须为1）

axis

dilation, stride, pad, kernel_size 宽高不相等的情况

### 建议项
总体而言，绝影的优化重点在普通卷积上，但针对3x3，1x1 的卷积依然有特殊优化


## Pool (池化层)
### 支持项
MAX、AVE 正常支持

kernel_size, stride, pad 正常支持

global_pooling 正常支持

### 不支持项
STOCHASTIC 运算不支持

kernel_h, kernel_w, pad_h , pad_w, stride_w, stride_h 等宽高不等的情况不支持

## InnerProductt (内积层)
### 支持项
基础运算及bias正常支持

在没有 bias 的情况上按零计算，同 Convolution

### 不支持项
axis 不支持，默认在 channel 方向作内积

transpose 不支持

## Relu (正则层)
Prestissmo 中不支持单独的正则层，正则层必须合并到上一层作运算
### 支持项
以下层支持后接 Relu

Convolution

InnerProduct

Eltwise

### 不支持项
非以上layer，网络中不能后接 relu

## Interp （双线性插值）
### 支持项
支持放大

支持 height、width 同时设定的情况

支持 zoom_factor 设定的情况

当 height、width 和 zoom_factor 同时设定时，以 zoom_factor 为准


### 不支持项
不支持缩小

shrink_factor 不支持

width 和 height 比 输入 的 width 和 height 小时不支持

pad 不支持

pad_beg, pad_end 不支持


## Concat
### 支持项
支持任意输入的合并，只支持 depth/channel 方向的合并

### 不支持项
不支持 axis 的指定

所有输入的depth/channel 必须为4的倍数

## Eltwise
### 支持项
支持两个输入的求和

### 不支持项
PROD 和 MAX 不支持，不支持带权重的求和


## Softmax
### 支持项
支持 depth/channel 方向上的 softmax 运算
### 不支持项
不支持 axis 的指定


## Deconvolution
### 支持项
仅支持 depthwise，即 group = input.channels 的情况，用于放大

### 不支持项
只支持用 Deconvolution 放大，其他的均不支持


## ROIPool
完整支持，此部分完全等同于caffe-fast-rcnn


## Reshape
此部分完全等同于caffe-fast-rcnn

## Proposal
此部分完全等同于caffe-fast-rcnn


## Dropout
遇到此层直接删除，将上一层的结果连到其输出


## OpenGL ES 版本
OpenGL ES 版本暂不支持如下Layer
```
Deconvolution
ROIPool
Reshape
Proposal
```

## 修改纪录
2017.9.30 初订

2017.11.10 修改：补充OpenGL ES版本的说明

2017.11.22 修改：全连接层的建议项

2017.12.8 修改：修正一些拼写错误

2017.12.21 修改：补充 concat 层的限制
