/*
 * -----------------------------------------------------------------------------
 * ENGLISH
 * -----------------------------------------------------------------------------
 * CppWinLog.h
 *
 * License: MIT License
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Description:
 *   Simple lightweight Windows logging helper. Supports optional headers
 *   (time, PE file name, module name, function name) and output modules
 *   (console print, file output, default behavior). Designed to be small and
 *   easy to integrate in native Windows C++ projects.
 *
 * Version: 1.0.0
 *
 * Intellectual Property:
 *   Copyright (c) 2026. Licensed under the MIT License. Keep this notice
 *   intact when redistributing the source or substantial portions.
 *
 * Usage (English):
 *   // Basic usage
 *   CppWinLog log;
 *   log.set_header(CppWinLog::eheader::function_name);
 *   log.set_module(CppWinLog::emodule::Default);
 *   log.push_string("hello %s", "world");
 *
 *   // Using macros provided in header
 *   push_string("value=%d", 42);
 *
 * -----------------------------------------------------------------------------
 * 中文
 * -----------------------------------------------------------------------------
 * CppWinLog.h
 *
 * 许可证：MIT 许可证
 * SPDX-License-Identifier: MIT
 *
 * 版权所有 (c) 2026
 *
 * 许可在此授予，免费向任何获得本软件及相关文档文件（下称“软件”）的人士提供软件使用权，
 * 包括但不限于使用、复制、修改、合并、发布、分发、再许可及/或出售软件副本的权利，
 * 以及授权被授权人如此行事的权利，条件是上述版权声明和本许可声明应包含在软件的
 * 所有副本或重要部分中。
 *
 * 本软件按“原样”提供，不提供任何形式的保证，包括但不限于对适销性、适用性及非侵权的
 * 默示保证。在任何情况下，作者或版权持有人均不承担因本软件或本软件的使用或其他交易
 * 所引起的任何索赔、损害或其他责任（无论是在合同、侵权或其他方面）。
 *
 * 描述：
 *   一个小型的 Windows 日志辅助类。支持可选的头部信息（时间、PE 文件名、模块名、函数名）
 *   以及输出目标（控制台、文件、默认行为）。易于在原生 Windows C++ 项目中集成使用。
 *
 * 版本：1.0.0
 *
 * 知识产权：
 *   版权所有 (c) 2026。根据 MIT 许可证授权。分发源代码或重要部分时请保留本声明。
 *
 * 使用示例（中文）：
 *   // 基本用法
 *   CppWinLog log;
 *   log.set_header(CppWinLog::eheader::function_name);
 *   log.set_module(CppWinLog::emodule::Default);
 *   log.push_string("hello %s", "world");
 *
 *   // 使用头文件中提供的宏
 *   push_string("值=%d", 42);
 *
 */

#pragma once
#define stringify(x) #x
#define tostring(x) stringify(x)
#define filename_only (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : \
                       (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))
#define push_string(format, ...) push_string_imp(__func__, format, ##__VA_ARGS__)
#define push_string_funcsign(format, ...) push_string_imp(__FUNCSIG__, format, ##__VA_ARGS__)
#define push_string_line(format, ...) push_string_imp(tostring(__LINE__), format, ##__VA_ARGS__)
#define push_string_fine(format, ...) push_string_imp(filename_only, format, ##__VA_ARGS__)
#define push_string_fine_line(format, ...) push_string_imp((std::string(filename_only) + "->" + tostring(__LINE__)).c_str(), format, ##__VA_ARGS__)
class CppWinLog
{
public:
	CppWinLog();
	enum class eheader : short
	{
		time,
		pe_file_name,
		module_name,
		function_name,
	};
	enum class emodule : short
	{
		print = 1 << 0,
		file = 1 << 1,
		Default = 1 << 2,
	};
	friend inline emodule operator|(emodule a, emodule b)
	{
		return static_cast<emodule>(static_cast<short>(a) | static_cast<short>(b));
	}
	friend inline emodule& operator|=(emodule& a, emodule b)
	{
		a = a | b;
		return a;
	}
	CppWinLog* set_header(eheader header);
	CppWinLog* set_module(emodule module_);
	CppWinLog* set_file_header(eheader header);
	char* push_string_imp(const char* fun_name, const char* format, ...);
	CppWinLog* get_context(void** data, size_t* size);
	char* get_context();
private:
	eheader header = eheader::time;
	eheader file_header = eheader::time;
	short module_ = 4;
	struct CContext;
	CContext* pContext;
	char* get_filename(const char* fun_name);
};

