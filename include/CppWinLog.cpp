#define _CRT_SECURE_NO_WARNINGS 1
#include "CppWinLog.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <ctime>
#include <windows.h>
struct CppWinLog::CContext {
    std::vector<char*> ctx;
};
CppWinLog::CppWinLog()
{
	this->pContext = new CContext();    
}
CppWinLog* CppWinLog::set_header(eheader header)
{
	this->header = header;
    return this;
}
CppWinLog* CppWinLog::set_module(emodule module_)
{
	this->module_ = (short)module_;
    return this;
}
CppWinLog* CppWinLog::set_file_header(eheader header)
{
	this->file_header = header;
    return this;
}


char* CppWinLog::push_string_imp(const char* fun_name, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    va_list args_copy;
    va_copy(args_copy, args);
    int len = std::vsnprintf(nullptr, 0, format, args_copy);
    va_end(args_copy);
    if (len < 0) { va_end(args); return nullptr; }
    char prefix[128] = { 0 };
    switch (this->header)
    {
    case eheader::time:
    {
        std::time_t t = std::time(nullptr);
        std::tm tm;
        localtime_s(&tm, &t);
        std::snprintf(prefix, sizeof(prefix), "[%04d %02d %02d %02d:%02d] ",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min);
        break;
    }
    case eheader::pe_file_name:
    {
        char path[MAX_PATH];
        GetModuleFileNameA(nullptr, path, MAX_PATH);
        const char* file = strrchr(path, '\\');
        if (!file) file = path;
        else file++;
        std::snprintf(prefix, sizeof(prefix), "[%s] ", file);
        break;
    }
    case eheader::module_name:
    {
        char path[MAX_PATH];
        GetModuleFileNameA(nullptr, path, MAX_PATH);
        const char* file = strrchr(path, '\\');
        if (!file) file = path;
        else file++;
        std::snprintf(prefix, sizeof(prefix), "[%s] ", file);
        break;
    }
    case eheader::function_name:
        if (fun_name)
            std::snprintf(prefix, sizeof(prefix), "[%s] ", fun_name);
        break;
    }
    int total_len = (int)strlen(prefix) + len;
    char* buffer = (char*)malloc(total_len + 1);
    if (!buffer) { va_end(args); return nullptr; }
    std::strcpy(buffer, prefix);
    std::vsnprintf(buffer + strlen(prefix), len + 1, format, args);
    va_end(args);
	this->pContext->ctx.push_back(buffer);
    if (this->module_ & static_cast<short>(emodule::print))
        printf("%s\n", buffer);
    if (this->module_ & static_cast<short>(emodule::file)) {
        auto filename = get_filename(fun_name);
        FILE* fp = fopen(filename, "a+");
        if (fp) {
            fprintf(fp, "%s\n", buffer);
            fclose(fp);
           delete[128] filename;
		}
    }
    return buffer;
}

CppWinLog* CppWinLog::get_context(void** data, size_t* size)
{
    *size = this->pContext->ctx.size();
	*data = this->pContext->ctx.data();
    return this;
}
char* CppWinLog::get_context()
{
    if (this->pContext->ctx.empty()) return nullptr;
    size_t total_len = 0;
    for (auto s : this->pContext->ctx)
        total_len += strlen(s) + 1; // +1 for '\n'
    char* result = (char*)malloc(total_len + 1); // +1 for final '\0'
    if (!result) return nullptr;
    char* p = result;
    for (auto s : this->pContext->ctx)
    {
        size_t len = strlen(s);
        memcpy(p, s, len);
        p += len;
        *p++ = '\n';
    }
    *p = '\0';
    return result;
}
char* CppWinLog::get_filename(const char* fun_name)
{
    char* filename = new char[128];
    switch (this->header)
    {
    case eheader::time:
    {
        time_t t = time(nullptr);
        tm local{};
        localtime_s(&local, &t);
        std::snprintf(filename, 128, "%04d-%02d-%02d.log",
            local.tm_year + 1900,
            local.tm_mon + 1,
            local.tm_mday);
        return filename;
        break;
    }
    case eheader::pe_file_name:
    {
        char path[MAX_PATH];
        GetModuleFileNameA(nullptr, path, MAX_PATH);
        const char* file = strrchr(path, '\\');
        if (!file) file = path;
        else file++;
       std::snprintf(filename, 128, "%s.log", file);
        break;
    }
    case eheader::module_name:
    {
        char path[MAX_PATH];
        GetModuleFileNameA(nullptr, path, MAX_PATH);
        const char* file = strrchr(path, '\\');
        if (!file) file = path;
        else file++;
        std::snprintf(filename, 128, "%s.log", file);
        break;
    }
    case eheader::function_name:
        if (fun_name)
            std::snprintf(filename, 128, "%s.log", fun_name);
        break;
    }
    return filename;
}
