/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

namespace async{

/**
����dump�ļ�
	\ref hFile �� \ref strDumpName������, ��ʹ��Ĭ��������dump�ļ�
	�����⵽�е�����, �򷵻�ֵ��ΪEXCEPTION_CONTINUE_SEARCH, ����������

@param pExceptionInfo
	ͨ��GetExceptionInformation()��ȡ���쳣��Ϣ
@param hFile
	����������, ��dump��Ϣд��������ָ����ļ�; ������������strDumpName����
@param strDumpName
	��� \ref hFile ������, �����ڲ���ʹ��strDumpName����һ���ļ������ʹ��

@return long
	#define EXCEPTION_EXECUTE_HANDLER       1
	#define EXCEPTION_CONTINUE_SEARCH       0
	#define EXCEPTION_CONTINUE_EXECUTION    -1
*/
extern long WINAPI ExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile = nullptr, const char* strDumpName = nullptr);

/**
����dump�ļ�
	\ref hFile �� \ref strDumpName������, ��ʹ��Ĭ��������dump�ļ�

@param pExceptionInfo
	ͨ��GetExceptionInformation()��ȡ���쳣��Ϣ
@param hFile
	����������, ��dump��Ϣд��������ָ����ļ�; ������������strDumpName����
@param strDumpName
	��� \ref hFile ������, �����ڲ���ʹ��strDumpName����һ���ļ������ʹ��

@return long
	#define EXCEPTION_EXECUTE_HANDLER       1
	#define EXCEPTION_CONTINUE_SEARCH       0
	#define EXCEPTION_CONTINUE_EXECUTION    -1
*/
extern long WINAPI GenerateMiniDump(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile = nullptr, const char* strDumpName = nullptr);

}
