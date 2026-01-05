# # CppWinLog README (中英文双版本)

A lightweight, dependency-free native Windows C++ logging library with flexible log header configuration and multiple output targets.

---

一个轻量级、无依赖的原生Windows C++日志工具库，支持灵活的日志头部配置与多输出目标，单头文件设计可快速集成至各类Windows C++项目。


## 📋 特性

- **轻量无依赖**：仅单个头文件`CppWinLog.h`，无需额外编译链接，直接引入即可使用

- **灵活头部配置**：支持时间戳、PE文件名、模块名、函数名等多种日志头部信息可选配置

- **多输出目标**：支持控制台打印、文件输出、默认行为三种输出模块，可通过位运算组合使用

- **丰富日志接口**：提供普通日志、格式化日志、带函数签名/文件名/行号等多种场景的日志输出接口

- **链式调用支持**：配置接口支持链式调用，简化代码书写逻辑

- **快捷宏封装**：内置多种快捷宏，无需手动创建日志对象即可快速输出日志

- **日志上下文获取**：支持获取原始日志缓存数据或格式化日志字符串，便于二次处理

- **MIT开源许可**：自由使用、修改、分发，适配个人与商业项目

## 🔧 快速开始

### 环境要求

- 操作系统：Windows（支持Win32/Win64平台）

- 编译器：支持C++11及以上标准（MSVC、MinGW等）

- 项目类型：原生Windows C++项目（非.NET框架依赖）

### 集成步骤

1. 将`CppWinLog.h`文件放入项目目录，建议放置在`include`子目录下（便于统一管理头文件）

2. 在需要使用日志功能的源文件中引入头文件：
        `#include <include/CppWinLog.h>`

### 最简示例

```Plain Text

#include <iostream>
#include <include/CppWinLog.h> 

int main()
{
    // 创建日志对象
    CppWinLog log;

    // 链式配置：日志头部显示函数名，输出至控制台+文件
    log.set_header(CppWinLog::eheader::function_name)
        ->set_module(CppWinLog::emodule::print | CppWinLog::emodule::file);

    // 输出格式化日志
    int test_val = 666;
    log.push_string("Hello CppWinLog! 测试数值：%d", test_val);

    return 0;
}
```

### 完整使用示例

```Plain Text

#include <iostream>
#include <include/CppWinLog.h> 

int main()
{
    // 创建日志对象
    CppWinLog log;

    // 链式设置日志头和模块
    log.set_header(CppWinLog::eheader::function_name)
        ->set_module(CppWinLog::emodule::file | CppWinLog::emodule::print);

    // 打印普通日志
    log.push_string("Hello, this is a simple log");

    // 打印格式化日志
    int value = 123;
    log.push_string("The value is %d", value);

    // 打印函数签名日志
    log.push_string_funcsign("This log shows full function signature");

    // 打印文件名日志
    log.push_string_fine("This log shows the file name");

    // 打印文件名+行号日志
    log.push_string_fine_line("This log shows file name and line number");

    // 获取日志上下文（缓存）
    void* data = nullptr;
    size_t size = 0;
    log.get_context(&data, &size);
    if (data && size > 0)
        std::cout << "Log context size: " << size << std::endl;

    // 获取日志上下文字符串
    char* contextStr = log.get_context();
    if (contextStr) {
        std::cout << "Log context: " << contextStr << std::endl;
        // 提示：使用完 get_context() 返回的内存后需要释放
        delete[] contextStr;
        contextStr = nullptr;
    }
    return 0;
}
```

## 📖 核心接口说明

### 1. 枚举配置

#### 日志头部枚举（eheader）

用于指定日志中携带的头部信息，可选值如下：

- `time`：携带时间戳

- `pe_file_name`：携带PE文件名（可执行文件/动态库名）

- `module_name`：携带模块名

- `function_name`：携带函数名

#### 输出模块枚举（emodule）

用于指定日志的输出目标，支持通过位运算（`|`）组合多个输出目标，可选值如下：

- `print = 1 << 0`：输出到控制台

- `file = 1 << 1`：输出到日志文件

- `Default = 1 << 2`：使用默认输出行为

### 2. 配置接口

- `CppWinLog* set_header(eheader header)`功能：设置日志头部信息返回值：日志对象指针，支持链式调用

- `CppWinLog* set_module(emodule module_)`功能：设置日志输出模块返回值：日志对象指针，支持链式调用

- `CppWinLog* set_file_header(eheader header)`功能：单独设置文件输出的日志头部信息（与控制台输出头部可差异化配置）返回值：日志对象指针，支持链式调用

### 3. 日志输出接口

#### 基础接口

- `char* push_string_imp(const char* fun_name, const char* format, ...)`功能：底层日志输出实现，支持格式化输入参数：fun_name（日志关联的函数名）、format（格式化日志模板）、可变参数（格式化参数）

#### 快捷宏接口（推荐使用）

无需手动创建日志对象，直接调用宏即可输出日志，自动关联相关上下文信息：

- `push_string(format, ...)`：输出普通格式化日志，自动携带当前函数名

- `push_string_funcsign(format, ...)`：输出带完整函数签名的格式化日志

- `push_string_line(format, ...)`：输出带当前行号的格式化日志

- `push_string_fine(format, ...)`：输出带当前文件名（仅文件名，不含路径）的格式化日志

- `push_string_fine_line(format, ...)`：输出带当前文件名+行号的格式化日志

### 4. 日志上下文获取接口

- `CppWinLog* get_context(void** data, size_t* size)`功能：获取原始日志缓存数据的指针和大小，用于底层数据处理参数：data（输出参数，接收日志缓存数据指针）、size（输出参数，接收日志缓存数据大小）

- `char* get_context()`功能：获取格式化后的完整日志字符串注意：调用者需手动释放返回的内存（使用`delete[]`），避免内存泄漏

## ⚠️ 注意事项

- 本库仅支持Windows平台，不具备跨平台兼容性

- 使用`get_context()`接口获取日志字符串后，必须手动释放内存，否则会导致内存泄漏

- 当使用`emodule::file`输出模块时，日志文件默认存储在可执行文件同目录，文件名由库内部定义

- 格式化日志语法与C语言`printf`一致，支持`%d`、`%s`、`%f`等常用格式化符

- 使用快捷宏之前，需确保已引入`CppWinLog.h`头文件，且宏调用所在作用域可正常访问库相关定义

## 📜 许可证

本项目采用MIT许可证开源，详细许可条款如下：

> 版权所有 (c) 2026
> 
> 许可在此授予，免费向任何获得本软件及相关文档文件（下称“软件”）的人士提供软件使用权，包括但不限于使用、复制、修改、合并、发布、分发、再许可及/或出售软件副本的权利，以及授权被授权人如此行事的权利，条件是上述版权声明和本许可声明应包含在软件的所有副本或重要部分中。
> 
> 本软件按“原样”提供，不提供任何形式的保证，包括但不限于对适销性、适用性及非侵权的默示保证。在任何情况下，作者或版权持有人均不承担因本软件或本软件的使用或其他交易所引起的任何索赔、损害或其他责任（无论是在合同、侵权或其他方面）。
> 
> 

## 📊 版本信息

当前版本：1.0.0

## 📌 知识产权

Copyright (c) 2026

根据MIT许可证授权，分发源代码或项目的重要部分时，请保留原始版权声明和许可声明。

---

A lightweight, dependency-free native Windows C++ logging library that supports flexible log header configuration and multiple output targets. With a single-header design, it can be quickly integrated into various Windows C++ projects.

## 📋 Features

- **Lightweight & Dependency-Free**: Only one header file `CppWinLog.h`, no additional compilation or linking required, can be used directly by including it

- **Flexible Header Configuration**: Supports optional configuration of multiple log header information such as timestamp, PE file name, module name, function name

- **Multiple Output Targets**: Supports three output modules (console print, file output, default behavior), which can be combined using bitwise operations

- **Rich Log Interfaces**: Provides log output interfaces for various scenarios such as normal logs, formatted logs, logs with function signatures/file names/line numbers

- **Chain Call Support**: Configuration interfaces support chain calls, simplifying code writing logic

- **Quick Macro Encapsulation**: Built-in multiple quick macros, allowing quick log output without manually creating log objects

- **Log Context Acquisition**: Supports obtaining raw log cache data or formatted log strings for secondary processing

- **MIT Open Source License**: Free to use, modify and distribute, suitable for personal and commercial projects

## 🔧 Quick Start

### Environment Requirements

- Operating System: Windows (supports Win32/Win64 platforms)

- Compiler: Supports C++11 and above standards (MSVC, MinGW, etc.)

- Project Type: Native Windows C++ project (no .NET framework dependency)

### Integration Steps

1. Place the `CppWinLog.h` file in the project directory, it is recommended to place it in the `include` subdirectory (for unified management of header files)

2. Include the header file in the source files that need to use logging functions:
        `#include <include/CppWinLog.h>`

### Minimum Example

```Plain Text

#include <iostream>
#include <include/CppWinLog.h> 

int main()
{
    // Create a log object
    CppWinLog log;

    // Chain configuration: log header displays function name, output to console + file
    log.set_header(CppWinLog::eheader::function_name)
        ->set_module(CppWinLog::emodule::print | CppWinLog::emodule::file);

    // Push a formatted log
    int test_val = 666;
    log.push_string("Hello CppWinLog! Test value: %d", test_val);

    return 0;
}
```

### Complete Usage Example

```Plain Text

#include <iostream>
#include <include/CppWinLog.h> 

int main()
{
    // Create a log object
    CppWinLog log;

    // Chain setting header and module
    log.set_header(CppWinLog::eheader::function_name)
        ->set_module(CppWinLog::emodule::file | CppWinLog::emodule::print);

    // Push a normal log with automatic function name
    log.push_string("Hello, this is a simple log");

    // Push a formatted log
    int value = 123;
    log.push_string("The value is %d", value);

    // Push log using full function signature
    log.push_string_funcsign("This log shows full function signature");

    // Push log with file name
    log.push_string_fine("This log shows the file name");

    // Push log with file name and line number
    log.push_string_fine_line("This log shows file name and line number");

    // Get log context (raw pointer and size)
    void* data = nullptr;
    size_t size = 0;
    log.get_context(&data, &size);
    if (data && size > 0)
        std::cout << "Log context size: " << size << std::endl;

    // Get log context as string
    char* contextStr = log.get_context();
    if (contextStr) {
        std::cout << "Log context: " << contextStr << std::endl;
        // Note: Caller should release memory returned by get_context() after use
        delete[] contextStr;
        contextStr = nullptr;
    }
    return 0;
}
```

## 📖 Core Interface Description

### 1. Enumeration Configuration

#### Log Header Enumeration (eheader)

Used to specify the header information carried in the log, optional values are as follows:

- `time`: Carries timestamp

- `pe_file_name`: Carries PE file name (executable file/dynamic library name)

- `module_name`: Carries module name

- `function_name`: Carries function name

#### Output Module Enumeration (emodule)

Used to specify the output target of the log, supports combining multiple output targets through bitwise operations (`|`), optional values are as follows:

- `print = 1 << 0`: Output to console

- `file = 1 << 1`: Output to log file

- `Default = 1 << 2`: Use default output behavior

### 2. Configuration Interfaces

- `CppWinLog* set_header(eheader header)`Function: Set log header informationReturn Value: Log object pointer, supports chain calls

- `CppWinLog* set_module(emodule module_)`Function: Set log output moduleReturn Value: Log object pointer, supports chain calls

- `CppWinLog* set_file_header(eheader header)`Function: Independently set the log header information for file output (can be configured differently from the console output header)Return Value: Log object pointer, supports chain calls

### 3. Log Output Interfaces

#### Basic Interface

- `char* push_string_imp(const char* fun_name, const char* format, ...)`Function: Underlying log output implementation, supports formatted inputParameters: fun_name (function name associated with the log), format (formatted log template), variable parameters (formatted parameters)

#### Quick Macro Interfaces (Recommended)

No need to manually create a log object, directly call the macro to output logs, which automatically associates relevant context information:

- `push_string(format, ...)`: Outputs a normal formatted log, automatically carrying the current function name

- `push_string_funcsign(format, ...)`: Outputs a formatted log with a complete function signature

- `push_string_line(format, ...)`: Outputs a formatted log with the current line number

- `push_string_fine(format, ...)`: Outputs a formatted log with the current file name (only the file name, no path)

- `push_string_fine_line(format, ...)`: Outputs a formatted log with the current file name + line number

### 4. Log Context Acquisition Interfaces

- `CppWinLog* get_context(void** data, size_t* size)`Function: Obtain the pointer and size of the original log cache data for underlying data processingParameters: data (output parameter, receives the log cache data pointer), size (output parameter, receives the log cache data size)

- `char* get_context()`Function: Obtain the formatted complete log stringNote: The caller must manually release the memory returned by `get_context()` (using `delete[]`) to avoid memory leaks

## ⚠️ Notes

- This library only supports Windows platforms and does not have cross-platform compatibility

- After obtaining the log string using the `get_context()` interface, the memory must be manually released, otherwise memory leaks will occur

- When using the `emodule::file` output module, the log file is stored in the same directory as the executable file by default, and the file name is defined internally by the library

- The syntax of formatted logs is consistent with C language`printf`, supporting common format specifiers such as `%d`, `%s`, `%f`

- Before using quick macros, ensure that the `CppWinLog.h` header file has been included, and the scope where the macro is called can normally access the relevant definitions of the library

## 📜 License

This project is open-sourced under the MIT License. The detailed license terms are as follows:

> Copyright (c) 2026
> 
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, subject to the following conditions:
> 
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
> 
> 

## 📊 Version Information

Current Version: 1.0.0

## 📌 Intellectual Property

Copyright (c) 2026

Licensed under the MIT License. When redistributing the source code or substantial portions of the project, please keep the original copyright notice and permission notice intact.
