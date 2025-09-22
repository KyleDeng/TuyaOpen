# MicroPython 编译系统详解

## 目录
1. [概述](#概述)
2. [源代码架构](#源代码架构)
3. [编译工具链](#编译工具链)
4. [编译流程](#编译流程)
5. [STM32平台编译示例](#stm32平台编译示例)
6. [内存布局和链接](#内存布局和链接)

## 概述

MicroPython 是 Python 3 的精简高效实现，专门为微控制器和嵌入式系统设计。本文档详细说明 MicroPython 的编译系统架构、源代码组织和构建流程。

## 源代码架构

MicroPython 采用分层架构设计，代码组织清晰，各部分职责明确：

### 1. `py/` - Python 虚拟机核心（最重要）

这是 MicroPython 的心脏，包含完整的 Python 解释器实现：

#### 词法分析和解析器
- `lexer.c` - 词法分析器，将源码分解为 token
- `parse.c` - 语法解析器，构建抽象语法树(AST)
- `scope.c` - 作用域分析，处理变量作用域

#### 编译器
- `compile.c` - 主编译器，将 AST 转换为字节码
- `emitbc.c` - 字节码发射器
- `emitglue.c` - 编译器粘合代码
- `emitnative.c` - 本地代码生成器（可选）
- `asmthumb.c/asmx64.c/asmarm.c` - 各架构汇编器

#### 虚拟机运行时
- `vm.c` - 字节码虚拟机主循环
- `runtime.c` - 运行时支持（函数调用、异常等）
- `bc.c` - 字节码相关工具
- `nlr*.c` - 非局部返回（异常处理机制）

#### 内存管理
- `malloc.c` - 内存分配器
- `gc.c` - 垃圾回收器（标记-清除算法）
- `pystack.c` - Python 栈管理
- `qstr.c` - 字符串池（QSTR）管理

#### 对象系统 (obj*.c)
```
objint.c      - 整数对象
objfloat.c    - 浮点数对象
objstr.c      - 字符串对象
objlist.c     - 列表对象
objdict.c     - 字典对象
objtuple.c    - 元组对象
objset.c      - 集合对象
objarray.c    - 数组对象
objfun.c      - 函数对象
objtype.c     - 类型系统
objexcept.c   - 异常对象
objgenerator.c - 生成器
```

#### 内置模块
- `modbuiltins.c` - builtins 模块
- `modsys.c` - sys 模块
- `modmath.c` - math 模块
- `modio.c` - io 模块
- `modstruct.c` - struct 模块
- `modgc.c` - gc 模块

### 2. `extmod/` - 扩展模块

提供硬件抽象和高级功能：

#### machine 模块（硬件抽象）
```
machine_i2c.c    - I2C 总线
machine_spi.c    - SPI 总线
machine_uart.c   - UART 串口
machine_adc.c    - ADC 模数转换
machine_pwm.c    - PWM 脉宽调制
machine_timer.c  - 定时器
machine_wdt.c    - 看门狗
machine_pinbase.c - GPIO 基础
```

#### 网络和通信
- `modbluetooth.c` - 蓝牙 BLE
- `modlwip.c` - TCP/IP 协议栈
- `modssl.c` - SSL/TLS 加密
- `modwebsocket.c` - WebSocket
- `modasyncio.c` - 异步 I/O

#### 文件系统
- `vfs.c` - 虚拟文件系统框架
- `vfs_fat.c` - FAT 文件系统
- `vfs_lfs.c` - LittleFS 文件系统
- `vfs_posix.c` - POSIX 文件系统

### 3. `shared/` - 共享组件

可在多个平台间复用的代码：

- **`runtime/`** - 运行时辅助
  - `pyexec.c` - REPL 执行器
  - `gchelper.c` - GC 辅助（汇编）
  - `interrupt_char.c` - Ctrl+C 中断
  - `softtimer.c` - 软件定时器

- **`libc/`** - 精简 C 库
- **`readline/`** - 命令行编辑
- **`netutils/`** - 网络工具（DHCP 等）
- **`timeutils/`** - 时间处理
- **`tinyusb/`** - USB 栈

### 4. `lib/` - 第三方库

```
stm32lib/     - STM32 HAL 库
littlefs/     - 小型文件系统
lwip/         - TCP/IP 协议栈
mbedtls/      - TLS/SSL 加密
btstack/      - 蓝牙协议栈
libm/         - 数学库
micropython-lib/ - Python 标准库
```

### 5. `ports/stm32/` - STM32 平台代码

#### 系统核心
```
main.c         - 主入口
stm32_it.c     - 中断处理
systick.c      - 系统时钟
pendsv.c       - PendSV 中断（任务切换）
powerctrl.c    - 电源管理
```

#### 外设驱动
```
uart.c         - UART 实现
i2c.c          - I2C 实现
spi.c          - SPI 实现
adc.c          - ADC 实现
dac.c          - DAC 实现
timer.c        - 定时器
rtc.c          - 实时时钟
dma.c          - DMA 控制器
can.c          - CAN 总线
```

#### 存储系统
```
flash.c        - 内部 Flash
flashbdev.c    - Flash 块设备
sdcard.c       - SD 卡驱动
storage.c      - 存储管理
```

#### USB 功能
```
usb.c          - USB 核心
usbd_conf.c    - USB 配置
usbd_desc.c    - USB 描述符
usbd_cdc_interface.c  - USB CDC（串口）
usbd_hid_interface.c  - USB HID（键鼠）
usbd_msc_interface.c  - USB MSC（存储）
```

## 编译工具链

### GCC 工具链配置

MicroPython 使用 ARM GCC 交叉编译工具链。工具链的配置分布在多个文件中：

#### 1. 交叉编译器前缀配置
在 `ports/stm32/Makefile` 第63行定义：
```makefile
CROSS_COMPILE ?= arm-none-eabi-
```

#### 2. 工具链组件定义
在 `py/mkenv.mk` 第32-42行定义了具体的工具：
```makefile
AS = $(CROSS_COMPILE)as         # 汇编器
CC = $(CROSS_COMPILE)gcc        # C 编译器
CPP = $(CC) -E                  # C 预处理器
CXX = $(CROSS_COMPILE)g++       # C++ 编译器
GDB = $(CROSS_COMPILE)gdb       # 调试器
LD = $(CROSS_COMPILE)ld         # 链接器
OBJCOPY = $(CROSS_COMPILE)objcopy  # 对象文件转换
SIZE = $(CROSS_COMPILE)size     # 大小分析
STRIP = $(CROSS_COMPILE)strip   # 符号剥离
AR = $(CROSS_COMPILE)ar         # 静态库工具
```

通过 `?=` 操作符，用户可以通过环境变量或命令行参数覆盖默认的工具链：
```bash
# 使用自定义工具链
make CROSS_COMPILE=/path/to/custom/toolchain/bin/arm-none-eabi-
```

### 编译参数配置

#### CPU 架构相关（以 STM32F4 为例）
```makefile
# Cortex-M4 核心配置
CFLAGS_CORTEX_M = -mthumb                    # Thumb 指令集
CFLAGS_MCU_f4 = -mtune=cortex-m4 -mcpu=cortex-m4  # M4 核心
CFLAGS += -mfpu=fpv4-sp-d16                  # FPU 硬件浮点
CFLAGS += -mfloat-abi=hard                   # 硬浮点 ABI
```

#### 优化选项
```makefile
# Debug 模式
DEBUG=1: COPT = -Og -g -DPENDSV_DEBUG

# Release 模式
COPT = -Os -DNDEBUG      # 优化大小
CSUPEROPT = -O3          # 性能关键代码用 -O3

# 代码优化
CFLAGS += -fdata-sections -ffunction-sections  # 分段编译
LDFLAGS += -Wl,--gc-sections                  # 移除未使用代码

# LTO（链接时优化）
LTO=1: CFLAGS += -flto=auto
```

## 编译流程

### 1. 准备阶段

```bash
# 编译交叉编译器 mpy-cross
cd mpy-cross
make

# 初始化子模块
cd ports/stm32
make BOARD=NUCLEO_F446RE submodules
```

### 2. 编译阶段

```bash
# 预处理 -> 编译 -> 汇编
arm-none-eabi-gcc -c source.c -o source.o \
    -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 \
    -Os -fdata-sections -ffunction-sections

# 生成依赖关系
arm-none-eabi-gcc -MM -MF source.d source.c

# QSTR（字符串池）生成
python tools/makeqstrdata.py > qstrdefs.generated.h

# 冻结 Python 模块
python tools/make-frozen.py frozen_modules > frozen.c
```

### 3. 链接阶段

```bash
arm-none-eabi-gcc \
    -T boards/stm32f411.ld \         # 链接脚本
    -Wl,-Map=firmware.map \          # 生成 map 文件
    -Wl,--gc-sections \              # 删除未使用段
    -nostdlib \                      # 不用标准库
    obj1.o obj2.o ... \              # 目标文件
    -lm -lgcc \                      # 数学库和编译器库
    -o firmware.elf
```

### 4. 固件生成

```bash
# ELF -> BIN（二进制）
arm-none-eabi-objcopy -O binary firmware.elf firmware.bin

# ELF -> HEX（Intel HEX）
arm-none-eabi-objcopy -O ihex firmware.elf firmware.hex

# 生成 DFU 文件（USB 烧录）
python tools/pydfu.py -b 0x08000000:firmware.bin firmware.dfu

# 查看固件大小
arm-none-eabi-size firmware.elf
```

## STM32平台编译示例

以 NUCLEO-F446RE 开发板为例的完整编译流程：

### 1. 准备环境

```bash
# 安装 ARM 交叉编译工具链
sudo apt-get install gcc-arm-none-eabi

# 获取 MicroPython 源码
git clone https://github.com/micropython/micropython.git
cd micropython
```

### 2. 编译 mpy-cross

```bash
cd mpy-cross
make
cd ..
```

### 3. 编译固件

```bash
cd ports/stm32
make BOARD=NUCLEO_F446RE submodules
make BOARD=NUCLEO_F446RE
```

### 4. 板级配置文件

**mpconfigboard.mk**：
```makefile
MCU_SERIES = f4              # MCU 系列
CMSIS_MCU = STM32F446xx      # 具体型号
AF_FILE = boards/stm32f446_af.csv  # 引脚复用配置
LD_FILES = boards/stm32f411.ld     # 链接脚本
TEXT0_ADDR = 0x08000000      # Flash 起始地址
```

**mpconfigboard.h**：
```c
// 硬件配置
#define MICROPY_HW_BOARD_NAME "NUCLEO-F446RE"
#define MICROPY_HW_CLK_PLLN (336)  // PLL 倍频
#define MICROPY_HW_CLK_PLLP (RCC_PLLP_DIV2)  // 168MHz CPU

// 外设定义
#define MICROPY_HW_UART2_TX (pin_A2)  // UART 引脚
#define MICROPY_HW_I2C1_SCL (pin_B8)  // I2C 引脚
#define MICROPY_HW_SPI1_SCK (pin_A5)  // SPI 引脚
#define MICROPY_HW_LED1 (pin_A5)      // LED 引脚
```

### 5. 烧录固件

```bash
# 使用 ST-LINK
make BOARD=NUCLEO_F446RE deploy-stlink

# 使用 DFU 模式
make BOARD=NUCLEO_F446RE deploy

# 使用 OpenOCD
make BOARD=NUCLEO_F446RE deploy-openocd
```

## 内存布局和链接

### 链接脚本内存布局

```ld
MEMORY {
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS {
    .text : {          /* 代码段 */
        *(.isr_vector)  /* 中断向量表 */
        *(.text*)       /* 程序代码 */
        *(.rodata*)     /* 只读数据 */
    } > FLASH

    .data : {          /* 初始化数据 */
        *(.data*)
    } > RAM AT> FLASH

    .bss : {           /* 未初始化数据 */
        *(.bss*)
    } > RAM

    .heap : {          /* Python 堆 */
        _heap_start = .;
        . = . + 100K;
        _heap_end = .;
    } > RAM
}
```

### 编译产物组成

典型的 STM32F446RE 固件组成：

```
固件大小分析（约 380KB）：
├── Python VM 核心 (150KB)
│   ├── 解释器 (40KB)
│   ├── 编译器 (35KB)
│   ├── 对象系统 (45KB)
│   └── 内置模块 (30KB)
├── 扩展模块 (60KB)
│   ├── machine 模块
│   ├── 网络模块
│   └── 文件系统
├── HAL 驱动库 (80KB)
├── 平台驱动 (50KB)
├── 共享组件 (20KB)
└── 冻结代码 (20KB)

内存使用（128KB RAM）：
├── 静态数据 (2KB)
├── BSS 段 (28KB)
├── Python 堆 (80KB)
└── 系统栈 (16KB)
```

## 特殊编译技术

### 1. QSTR 优化
所有字符串在编译时收集到字符串池，减少内存使用。

### 2. 冻结模块
Python 代码预编译为字节码嵌入固件，节省 RAM 空间。

### 3. ROM 压缩
使用压缩算法减小固件大小。

### 4. 内联汇编
关键代码使用 Thumb 汇编优化，提高执行效率。

## 编译选项说明

### 常用编译参数

```bash
# 启用链接时优化（减小固件大小）
make BOARD=NUCLEO_F446RE LTO=1

# 详细编译输出
make BOARD=NUCLEO_F446RE V=1

# 清理编译文件
make BOARD=NUCLEO_F446RE clean

# 自定义冻结模块
make BOARD=NUCLEO_F446RE FROZEN_MANIFEST=my_manifest.py
```

### 调试选项

```bash
# 启用调试信息
make BOARD=NUCLEO_F446RE DEBUG=1

# 生成汇编文件
make BOARD=NUCLEO_F446RE CFLAGS_EXTRA="-save-temps"
```

## 总结

MicroPython 的编译系统通过精心设计的模块化架构，实现了：

1. **高度可移植性**：核心 VM 与平台无关，易于移植到新平台
2. **可裁剪性**：通过配置选择需要的功能，适应不同资源限制
3. **易扩展性**：模块化设计便于添加新功能
4. **代码复用**：共享组件在各平台间复用
5. **优化效率**：多级优化技术确保代码大小和执行效率

整个编译系统通过 Makefile 精心组织，支持灵活配置，能够针对不同 MCU 和应用场景进行优化，是嵌入式 Python 实现的典范。