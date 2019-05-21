/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

namespace async{

/**
生成dump文件
	\ref hFile 和 \ref strDumpName不存在, 则使用默认名生成dump文件
	如果检测到有调试器, 则返回值置为EXCEPTION_CONTINUE_SEARCH, 交给调试器

@param pExceptionInfo
	通过GetExceptionInformation()获取的异常信息
@param hFile
	如果句柄存在, 则dump信息写入这个句柄指向的文件; 句柄存在则忽略strDumpName参数
@param strDumpName
	如果 \ref hFile 不存在, 则函数内部将使用strDumpName创建一个文件句柄并使用

@return long
	#define EXCEPTION_EXECUTE_HANDLER       1
	#define EXCEPTION_CONTINUE_SEARCH       0
	#define EXCEPTION_CONTINUE_EXECUTION    -1
*/
extern long WINAPI ExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile = nullptr, const char* strDumpName = nullptr);

/**
生成dump文件
	\ref hFile 和 \ref strDumpName不存在, 则使用默认名生成dump文件

@param pExceptionInfo
	通过GetExceptionInformation()获取的异常信息
@param hFile
	如果句柄存在, 则dump信息写入这个句柄指向的文件; 句柄存在则忽略strDumpName参数
@param strDumpName
	如果 \ref hFile 不存在, 则函数内部将使用strDumpName创建一个文件句柄并使用

@return long
	#define EXCEPTION_EXECUTE_HANDLER       1
	#define EXCEPTION_CONTINUE_SEARCH       0
	#define EXCEPTION_CONTINUE_EXECUTION    -1
*/
extern long WINAPI GenerateMiniDump(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile = nullptr, const char* strDumpName = nullptr);

}
