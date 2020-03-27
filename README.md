# blinker-nRF52
Base nordic nrf5 SDK V16.0.0 and nrf5 mesh SDK V4.0.0

# Blinker Nordic nRF52支持库
针对 Nordic nRF52xx 系列芯片的Blinker库，需配合 [nRF5 SDK v16.0.0](https://infocenter.nordicsemi.com/topic/struct_sdk/struct/sdk_nrf5_latest.html?cp=7_1) 和 [nRF5 SDK for Mesh v4.0.0](https://infocenter.nordicsemi.com/topic/struct_sdk/struct/sdk_mesh_latest.html?cp=7_2)

## 注意
- 有较强的自学能力及文档阅读能力  
- nRF52xx 系列开发板

## 支持的硬件
* [nRF52xx boards](https://www.nordicsemi.com/Software-and-tools/Development-Kits)

## 支持的接入方式
* BLE 5.0
* BLE MESH

## 支持的功能
- BLE 5.0
  - 直接与手机端/用户端 BLE 连接通信
    - 上报/查询设备状态
- BLE MESH
  - MESH 网关
    - 上传节点数据到云端
    - 下发控制数据到节点
  - MESH 中继节点
    - 中继转发数据
  - MESH 低功耗节点
    - 上报节点数据
    - 执行控制指令
- BLE DFU
  - BLE Secure DFU 加密更新
  - BLE MESH Secure DFU 加密更新

## 支持的应用
- BLE 5.0
  - 心跳/睡眠/运动数据 检测上报
  - beacon 室内/外定位
  - 蓝牙遥控/键盘 等控制设备
- BLE MESH
  - 智能家居设备组网
    - 灯/开关/窗帘/门磁/温湿度检测
  - 工业设备组网
    - 工业设备定位/设备运行状态上传/设备控制下发
  - 农业设备组网
    - 农产品溯源/养殖环境数据上报
  - 室内外定位
    - 停车场寻车/运动定位及轨迹上报
- BLE DFU
  - 基于BLE 5.0加密DFU，可直接手机直连DFU更新
  - 基于BLE MESH加密DFU，可直接全部设备或按设备类型等进行分类DFU更新

## 准备工作
使用前你需要做好如下准备:
* 基于官方开发套件 [SES开发环境配置](https://infocenter.nordicsemi.com/pdf/getting_started_ses.pdf)
* [SEGGER Embedded Studio](https://www.segger.com/downloads/embedded-studio/)
* [nRF Command Line Tools](https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF-Command-Line-Tools/Download#infotabs)
* [micro_ecc](https://infocenter.nordicsemi.com/topic/sdk_nrf5_v16.0.0/lib_crypto_backend_micro_ecc.html#lib_crypto_backend_micro_ecc_install)
* [nrfutil](https://infocenter.nordicsemi.com/topic/ug_nrfutil/UG/nrfutil/nrfutil_installing.html)
* [Blinker-nRF52](https://github.com/blinker-iot/blinker-nRF52) 将 **Blinker** 文件放到 **nRF5SDK16\components** 目录下, **exmaples** 放到 **nRF5SDK16\examples\ble_peripheral** 目录下
