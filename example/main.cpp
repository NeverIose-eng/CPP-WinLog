#include <iostream>
#include <include/CppWinLog.h>
int main()
{
    // 创建日志对象
    // Create a log object
    CppWinLog log;

    // 链式设置日志头和模块
    // Chain setting header and module
    log.set_header(CppWinLog::eheader::function_name)
        ->set_module(CppWinLog::emodule::file | CppWinLog::emodule::print);

    // 打印普通日志
    // Push a normal log with automatic function name
    log.push_string("Hello, this is a simple log");

    // 打印格式化日志
    // Push a formatted log
    int value = 123;
    log.push_string("The value is %d", value);

    // 打印函数签名日志
    // Push log using full function signature
    log.push_string_funcsign("This log shows full function signature");

    // 打印文件名日志
    // Push log with file name
    log.push_string_fine("This log shows the file name");

    // 打印文件名+行号日志
    // Push log with file name and line number
    log.push_string_fine_line("This log shows file name and line number");

    // 获取日志上下文（缓存）
    // Get log context (raw pointer and size)
    void* data = nullptr;
    size_t size = 0;
    log.get_context(&data, &size);
    if (data && size > 0)
        std::cout << "Log context size: " << size << std::endl;

    // 获取日志上下文字符串
    // Get log context as string
    char* contextStr = log.get_context();
    if (contextStr) {
        std::cout << "Log context: " << contextStr << std::endl;
        // 提示：使用完 get_context() 返回的内存后需要释放
        // Note: Caller should release memory returned by get_context() after use
        delete[] contextStr;
        contextStr = nullptr;
    }
    return 0;
}
