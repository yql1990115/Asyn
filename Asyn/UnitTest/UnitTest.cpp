// UnitTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../Asyn/qlAsynHelper.h"
#include "../Asyn/qlAsynCall.h"
#include "../Asyn/qlAsynCallEx.h"
#include "../Asyn/qlScheduler.h"
#include "../Asyn/qlSchedulerTimer.h"
#include "../Asyn/qlThreadEngine.h"
#include "../Asyn/qlWindowEngine.h"

////////////////////////////////////////////////////////////////////////
void vtest0() { int i = 3; int j = 0; int m = i / j; };
void vtest1(int i1) { int i = 3; int j = 0; int m = i / j; };
void vtest2(int i1, int i2) { int i = 3; int j = 0; int m = i / j; };
void vtest3(int i1, int i2, int i3) { int i = 3; int j = 0; int m = i / j; };
void vtest4(int i1, int i2, int i3, int i4) { int i = 3; int j = 0; int m = i / j; };
void vtest5(int i1, int i2, int i3, int i4, int i5) { int i = 3; int j = 0; int m = i / j; };
void vtest6(int i1, int i2, int i3, int i4, int i5, int i6) { int i = 3; int j = 0; int m = i / j; };
void vtest7(int i1, int i2, int i3, int i4, int i5, int i6, int i7) { int i = 3; int j = 0; int m = i / j; };
void vtest8(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8) { int i = 3; int j = 0; int m = i / j; };
void vtest9(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9) { int i = 3; int j = 0; int m = i / j; };
void vtException(void* func, const char* strName)
{
	if (strName)
	{
		OutputDebugStringA("\r\n Exception");
		OutputDebugStringA(strName);
		OutputDebugStringA("\r\n");
	}
}

class vt0
{
public:
	vt0() {};
	virtual ~vt0() {};

public:
	void test0() { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vt1
{
public:
	vt1() {};
	virtual ~vt1() {};

public:
	void test1(int i1) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vt2
{
public:
	vt2() {};
	virtual ~vt2() {};

public:
	void test2(int i1, int i2) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vt3
{
public:
	vt3() {};
	virtual ~vt3() {};

public:
	void test3(int i1, int i2, int i3) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vt4
{
public:
	vt4() {};
	virtual ~vt4() {};

public:
	void test4(int i1, int i2, int i3, int i4) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vt5
{
public:
	vt5() {};
	virtual ~vt5() {};

public:
	void test5(int i1, int i2, int i3, int i4, int i5) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vt6
{
public:
	vt6() {};
	virtual ~vt6() {};

public:
	void test6(int i1, int i2, int i3, int i4, int i5, int i6) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vt7
{
public:
	vt7() {};
	virtual ~vt7() {};

public:
	void test7(int i1, int i2, int i3, int i4, int i5, int i6, int i7) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vt8
{
public:
	vt8() {};
	virtual ~vt8() {};

public:
	void test8(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vt9
{
public:
	vt9() {};
	virtual ~vt9() {};

public:
	void test9(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

////////////////////////////////////////////////////////
void vtestEx0(asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };
void vtestEx1(int i1, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };
void vtestEx2(int i1, int i2, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };
void vtestEx3(int i1, int i2, int i3, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };
void vtestEx4(int i1, int i2, int i3, int i4, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };
void vtestEx5(int i1, int i2, int i3, int i4, int i5, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };
void vtestEx6(int i1, int i2, int i3, int i4, int i5, int i6, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };
void vtestEx7(int i1, int i2, int i3, int i4, int i5, int i6, int i7, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };
void vtestEx8(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };
void vtestEx9(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

class vtEx0
{
public:
	vtEx0() {};
	virtual ~vtEx0() {};

public:
	void testEx0(asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vtEx1
{
public:
	vtEx1() {};
	virtual ~vtEx1() {};

public:
	void testEx1(int i1, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vtEx2
{
public:
	vtEx2() {};
	virtual ~vtEx2() {};

public:
	void testEx2(int i1, int i2, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vtEx3
{
public:
	vtEx3() {};
	virtual ~vtEx3() {};

public:
	void testEx3(int i1, int i2, int i3, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vtEx4
{
public:
	vtEx4() {};
	virtual ~vtEx4() {};

public:
	void testEx4(int i1, int i2, int i3, int i4, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vtEx5
{
public:
	vtEx5() {};
	virtual ~vtEx5() {};

public:
	void testEx5(int i1, int i2, int i3, int i4, int i5, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vtEx6
{
public:
	vtEx6() {};
	virtual ~vtEx6() {};

public:
	void testEx6(int i1, int i2, int i3, int i4, int i5, int i6, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vtEx7
{
public:
	vtEx7() {};
	virtual ~vtEx7() {};

public:
	void testEx7(int i1, int i2, int i3, int i4, int i5, int i6, int i7, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vtEx8
{
public:
	vtEx8() {};
	virtual ~vtEx8() {};

public:
	void testEx8(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class vtEx9
{
public:
	vtEx9() {};
	virtual ~vtEx9() {};

public:
	void testEx9(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

////////////////////////////////////////////////////////
int itest0() { int i = 3; int j = 0; int m = i / j; return 0; };
int itest1(int i1) { int i = 3; int j = 0; int m = i / j; return 0; };
int itest2(int i1, int i2) { int i = 3; int j = 0; int m = i / j; return 0; };
int itest3(int i1, int i2, int i3) { int i = 3; int j = 0; int m = i / j; return 0; };
int itest4(int i1, int i2, int i3, int i4) { int i = 3; int j = 0; int m = i / j; return 0; };
int itest5(int i1, int i2, int i3, int i4, int i5) { int i = 3; int j = 0; int m = i / j; return 0; };
int itest6(int i1, int i2, int i3, int i4, int i5, int i6) { int i = 3; int j = 0; int m = i / j; return 0; };
int itest7(int i1, int i2, int i3, int i4, int i5, int i6, int i7) { int i = 3; int j = 0; int m = i / j; return 0; };
int itest8(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8) { int i = 3; int j = 0; int m = i / j; return 0; };
int itest9(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9) { int i = 3; int j = 0; int m = i / j; return 0; };

void itException(void* func, const char* strName)
{
	if (strName)
	{
		OutputDebugStringA("\r\n Exception");
		OutputDebugStringA(strName);
		OutputDebugStringA("\r\n");
	}
}

class it0
{
public:
	it0() {};
	virtual ~it0() {};

public:
	int test0() { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class it1
{
public:
	it1() {};
	virtual ~it1() {};

public:
	int test1(int i1) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class it2
{
public:
	it2() {};
	virtual ~it2() {};

public:
	int test2(int i1, int i2) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class it3
{
public:
	it3() {};
	virtual ~it3() {};

public:
	int test3(int i1, int i2, int i3) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class it4
{
public:
	it4() {};
	virtual ~it4() {};

public:
	int test4(int i1, int i2, int i3, int i4) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class it5
{
public:
	it5() {};
	virtual ~it5() {};

public:
	int test5(int i1, int i2, int i3, int i4, int i5) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class it6
{
public:
	it6() {};
	virtual ~it6() {};

public:
	int test6(int i1, int i2, int i3, int i4, int i5, int i6) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class it7
{
public:
	it7() {};
	virtual ~it7() {};

public:
	int test7(int i1, int i2, int i3, int i4, int i5, int i6, int i7) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class it8
{
public:
	it8() {};
	virtual ~it8() {};

public:
	int test8(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class it9
{
public:
	it9() {};
	virtual ~it9() {};

public:
	int test9(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

////////////////////////////////////////////////////////
int itestEx0(asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };
int itestEx1(int i1, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };
int itestEx2(int i1, int i2, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };
int itestEx3(int i1, int i2, int i3, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };
int itestEx4(int i1, int i2, int i3, int i4, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };
int itestEx5(int i1, int i2, int i3, int i4, int i5, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };
int itestEx6(int i1, int i2, int i3, int i4, int i5, int i6, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };
int itestEx7(int i1, int i2, int i3, int i4, int i5, int i6, int i7, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };
int itestEx8(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };
int itestEx9(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

class itEx0
{
public:
	itEx0() {};
	virtual ~itEx0() {};

public:
	int testEx0(asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class itEx1
{
public:
	itEx1() {};
	virtual ~itEx1() {};

public:
	int testEx1(int i1, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class itEx2
{
public:
	itEx2() {};
	virtual ~itEx2() {};

public:
	int testEx2(int i1, int i2, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class itEx3
{
public:
	itEx3() {};
	virtual ~itEx3() {};

public:
	int testEx3(int i1, int i2, int i3, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class itEx4
{
public:
	itEx4() {};
	virtual ~itEx4() {};

public:
	int testEx4(int i1, int i2, int i3, int i4, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class itEx5
{
public:
	itEx5() {};
	virtual ~itEx5() {};

public:
	int testEx5(int i1, int i2, int i3, int i4, int i5, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class itEx6
{
public:
	itEx6() {};
	virtual ~itEx6() {};

public:
	int testEx6(int i1, int i2, int i3, int i4, int i5, int i6, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class itEx7
{
public:
	itEx7() {};
	virtual ~itEx7() {};

public:
	int testEx7(int i1, int i2, int i3, int i4, int i5, int i6, int i7, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class itEx8
{
public:
	itEx8() {};
	virtual ~itEx8() {};

public:
	int testEx8(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

class itEx9
{
public:
	itEx9() {};
	virtual ~itEx9() {};

public:
	int testEx9(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, asyn::IJobState*) { int i = 3; int j = 0; int m = i / j; return 0; };

	void Exception(void* func, const char* strName)
	{
		if (strName)
		{
			OutputDebugStringA("\r\n Exception");
			OutputDebugStringA(strName);
			OutputDebugStringA("\r\n");
		}
	}
};

asyn::CqlScheduler g_Scheduler;

int main()
{
	g_Scheduler.Startup();
	//	验证模板参数推导的正确性
	////////////////////////////////////////////////////////////////////////
	asyn::CqlAsynCall l_vcall;
	l_vcall.AsynCall(vtest0, &g_Scheduler, "vtest0", vtException);
	l_vcall.WaitFor();
	l_vcall.AsynCall(vtest1, 1, &g_Scheduler, "vtest1", vtException);
	l_vcall.WaitFor();
	l_vcall.AsynCall(vtest2, 1, 2, &g_Scheduler, "vtest2", vtException);
	l_vcall.WaitFor();
	l_vcall.AsynCall(vtest3, 1, 2, 3, &g_Scheduler, "vtest3", vtException);
	l_vcall.WaitFor();
	l_vcall.AsynCall(vtest4, 1, 2, 3, 4, &g_Scheduler, "vtest4", vtException);
	l_vcall.WaitFor();
	l_vcall.AsynCall(vtest5, 1, 2, 3, 4, 5, &g_Scheduler, "vtest5", vtException);
	l_vcall.WaitFor();
	l_vcall.AsynCall(vtest6, 1, 2, 3, 4, 5, 6, &g_Scheduler, "vtest6", vtException);
	l_vcall.WaitFor();
	l_vcall.AsynCall(vtest7, 1, 2, 3, 4, 5, 6, 7, &g_Scheduler, "vtest7", vtException);
	l_vcall.WaitFor();
	l_vcall.AsynCall(vtest8, 1, 2, 3, 4, 5, 6, 7, 8, &g_Scheduler, "vtest8", vtException);
	l_vcall.WaitFor();
	l_vcall.AsynCall(vtest9, 1, 2, 3, 4, 5, 6, 7, 8, 9, &g_Scheduler, "vtest9", vtException);
	l_vcall.WaitFor();
	vt0 l_vt0;
	vt1 l_vt1;
	vt2 l_vt2;
	vt3 l_vt3;
	vt4 l_vt4;
	vt5 l_vt5;
	vt6 l_vt6;
	vt7 l_vt7;
	vt8 l_vt8;
	vt9 l_vt9;
	l_vcall.AsynCall(&l_vt0, &vt0::test0, &g_Scheduler, "vt0::test0", &vt0::Exception);
	l_vcall.WaitFor();
	l_vcall.AsynCall(&l_vt1, &vt1::test1, 1, &g_Scheduler, "vt1::test1", &vt1::Exception);
	l_vcall.WaitFor();
	l_vcall.AsynCall(&l_vt2, &vt2::test2, 1, 2, &g_Scheduler, "vt2::test2", &vt2::Exception);
	l_vcall.WaitFor();
	l_vcall.AsynCall(&l_vt3, &vt3::test3, 1, 2, 3, &g_Scheduler, "vt3::test3", &vt3::Exception);
	l_vcall.WaitFor();
	l_vcall.AsynCall(&l_vt4, &vt4::test4, 1, 2, 3, 4, &g_Scheduler, "vt4::test4", &vt4::Exception);
	l_vcall.WaitFor();
	l_vcall.AsynCall(&l_vt5, &vt5::test5, 1, 2, 3, 4, 5, &g_Scheduler, "vt5::test5", &vt5::Exception);
	l_vcall.WaitFor();
	l_vcall.AsynCall(&l_vt6, &vt6::test6, 1, 2, 3, 4, 5, 6, &g_Scheduler, "vt6::test6", &vt6::Exception);
	l_vcall.WaitFor();
	l_vcall.AsynCall(&l_vt7, &vt7::test7, 1, 2, 3, 4, 5, 6, 7, &g_Scheduler, "vt7::test7", &vt7::Exception);
	l_vcall.WaitFor();
	l_vcall.AsynCall(&l_vt8, &vt8::test8, 1, 2, 3, 4, 5, 6, 7, 8, &g_Scheduler, "vt8::test8", &vt8::Exception);
	l_vcall.WaitFor();
	l_vcall.AsynCall(&l_vt9, &vt9::test9, 1, 2, 3, 4, 5, 6, 7, 8, 9, &g_Scheduler, "vt9::test9", &vt9::Exception);
	l_vcall.WaitFor();

	//////////////////////////////////////////////////////////////////////
	asyn::CqlAsynCall l_vcallEx;
	l_vcallEx.AsynCallEx(vtestEx0, &g_Scheduler, "vtestEx0", vtException);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(vtestEx1, 1, &g_Scheduler, "vtestEx1", vtException);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(vtestEx2, 1, 2, &g_Scheduler, "vtestEx2", vtException);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(vtestEx3, 1, 2, 3, &g_Scheduler, "vtestEx3", vtException);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(vtestEx4, 1, 2, 3, 4, &g_Scheduler, "vtestEx4", vtException);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(vtestEx5, 1, 2, 3, 4, 5, &g_Scheduler, "vtestEx5", vtException);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(vtestEx6, 1, 2, 3, 4, 5, 6, &g_Scheduler, "vtestEx6", vtException);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(vtestEx7, 1, 2, 3, 4, 5, 6, 7, &g_Scheduler, "vtestEx7", vtException);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(vtestEx8, 1, 2, 3, 4, 5, 6, 7, 8, &g_Scheduler, "vtestEx8", vtException);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(vtestEx9, 1, 2, 3, 4, 5, 6, 7, 8, 9, &g_Scheduler, "vtestEx9", vtException);
	l_vcallEx.WaitFor();
	vtEx0 l_vtEx0;
	vtEx1 l_vtEx1;
	vtEx2 l_vtEx2;
	vtEx3 l_vtEx3;
	vtEx4 l_vtEx4;
	vtEx5 l_vtEx5;
	vtEx6 l_vtEx6;
	vtEx7 l_vtEx7;
	vtEx8 l_vtEx8;
	vtEx9 l_vtEx9;
	l_vcallEx.AsynCallEx(&l_vtEx0, &vtEx0::testEx0, &g_Scheduler, "vtEx0::testEx0", &vtEx0::Exception);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(&l_vtEx1, &vtEx1::testEx1, 1, &g_Scheduler, "vtEx1::testEx1", &vtEx1::Exception);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(&l_vtEx2, &vtEx2::testEx2, 1, 2, &g_Scheduler, "vtEx2::testEx2", &vtEx2::Exception);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(&l_vtEx3, &vtEx3::testEx3, 1, 2, 3, &g_Scheduler, "vtEx3::testEx3", &vtEx3::Exception);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(&l_vtEx4, &vtEx4::testEx4, 1, 2, 3, 4, &g_Scheduler, "vtEx4::testEx4", &vtEx4::Exception);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(&l_vtEx5, &vtEx5::testEx5, 1, 2, 3, 4, 5, &g_Scheduler, "vtEx5::testEx5", &vtEx5::Exception);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(&l_vtEx6, &vtEx6::testEx6, 1, 2, 3, 4, 5, 6, &g_Scheduler, "vtEx6::testEx6", &vtEx6::Exception);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(&l_vtEx7, &vtEx7::testEx7, 1, 2, 3, 4, 5, 6, 7, &g_Scheduler, "vtEx7::testEx7", &vtEx7::Exception);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(&l_vtEx8, &vtEx8::testEx8, 1, 2, 3, 4, 5, 6, 7, 8, &g_Scheduler, "vtEx8::testEx8", &vtEx8::Exception);
	l_vcallEx.WaitFor();
	l_vcallEx.AsynCallEx(&l_vtEx9, &vtEx9::testEx9, 1, 2, 3, 4, 5, 6, 7, 8, 9, &g_Scheduler, "vtEx9::testEx9", &vtEx9::Exception);
	l_vcallEx.WaitFor();

	asyn::CqlAsynCall l_vcallEx1;
	l_vcallEx1.AsynCallEx(&l_vtEx9, &vtEx9::testEx9, 1, 2, 3, 4, 5, 6, 7, 8, 9, &g_Scheduler, "vtEx9::testEx9", &vtEx9::Exception);
	l_vcallEx1.WaitFor();
	////////////////////////////////////////////////////////////////////////
	asyn::CqlAsynCallEx<int> l_icall;
	l_icall.AsynCall(itest0, &g_Scheduler, "itest0", itException);
	l_icall.WaitFor();
	l_icall.AsynCall(itest1, 1, &g_Scheduler, "itest1", itException);
	l_icall.WaitFor();
	l_icall.AsynCall(itest2, 1, 2, &g_Scheduler, "itest2", itException);
	l_icall.WaitFor();
	l_icall.AsynCall(itest3, 1, 2, 3, &g_Scheduler, "itest3", itException);
	l_icall.WaitFor();
	l_icall.AsynCall(itest4, 1, 2, 3, 4, &g_Scheduler, "itest4", itException);
	l_icall.WaitFor();
	l_icall.AsynCall(itest5, 1, 2, 3, 4, 5, &g_Scheduler, "itest5", itException);
	l_icall.WaitFor();
	l_icall.AsynCall(itest6, 1, 2, 3, 4, 5, 6, &g_Scheduler, "itest6", itException);
	l_icall.WaitFor();
	l_icall.AsynCall(itest7, 1, 2, 3, 4, 5, 6, 7, &g_Scheduler, "itest7", itException);
	l_icall.WaitFor();
	l_icall.AsynCall(itest8, 1, 2, 3, 4, 5, 6, 7, 8, &g_Scheduler, "itest8", itException);
	l_icall.WaitFor();
	l_icall.AsynCall(itest9, 1, 2, 3, 4, 5, 6, 7, 8, 9, &g_Scheduler, "itest9", itException);
	l_icall.WaitFor();
	it0 l_it0;
	it1 l_it1;
	it2 l_it2;
	it3 l_it3;
	it4 l_it4;
	it5 l_it5;
	it6 l_it6;
	it7 l_it7;
	it8 l_it8;
	it9 l_it9;
	l_icall.AsynCall(&l_it0, &it0::test0, &g_Scheduler, "it0::itest0", &it0::Exception);
	l_icall.WaitFor();
	l_icall.AsynCall(&l_it1, &it1::test1, 1, &g_Scheduler, "it1::itest1", &it1::Exception);
	l_icall.WaitFor();
	l_icall.AsynCall(&l_it2, &it2::test2, 1, 2, &g_Scheduler, "it2::itest2", &it2::Exception);
	l_icall.WaitFor();
	l_icall.AsynCall(&l_it3, &it3::test3, 1, 2, 3, &g_Scheduler, "it3::itest3", &it3::Exception);
	l_icall.WaitFor();
	l_icall.AsynCall(&l_it4, &it4::test4, 1, 2, 3, 4, &g_Scheduler, "it4::itest4", &it4::Exception);
	l_icall.WaitFor();
	l_icall.AsynCall(&l_it5, &it5::test5, 1, 2, 3, 4, 5, &g_Scheduler, "it5::itest5", &it5::Exception);
	l_icall.WaitFor();
	l_icall.AsynCall(&l_it6, &it6::test6, 1, 2, 3, 4, 5, 6, &g_Scheduler, "it6::itest6", &it6::Exception);
	l_icall.WaitFor();
	l_icall.AsynCall(&l_it7, &it7::test7, 1, 2, 3, 4, 5, 6, 7, &g_Scheduler, "it7::itest7", &it7::Exception);
	l_icall.WaitFor();
	l_icall.AsynCall(&l_it8, &it8::test8, 1, 2, 3, 4, 5, 6, 7, 8, &g_Scheduler, "it8::itest8", &it8::Exception);
	l_icall.WaitFor();
	l_icall.AsynCall(&l_it9, &it9::test9, 1, 2, 3, 4, 5, 6, 7, 8, 9, &g_Scheduler, "it9::itest9", &it9::Exception);
	l_icall.WaitFor();

	////////////////////////////////////////////////////////////////////////
	asyn::CqlAsynCallEx<int> l_icallEx;
	l_icallEx.AsynCallEx(itestEx0, &g_Scheduler, "itestEx0", itException);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(itestEx1, 1, &g_Scheduler, "itestEx1", itException);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(itestEx2, 1, 2, &g_Scheduler, "itestEx2", itException);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(itestEx3, 1, 2, 3, &g_Scheduler, "itestEx3", itException);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(itestEx4, 1, 2, 3, 4, &g_Scheduler, "itestEx4", itException);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(itestEx5, 1, 2, 3, 4, 5, &g_Scheduler, "itestEx5", itException);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(itestEx6, 1, 2, 3, 4, 5, 6, &g_Scheduler, "itestEx6", itException);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(itestEx7, 1, 2, 3, 4, 5, 6, 7, &g_Scheduler, "itestEx7", itException);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(itestEx8, 1, 2, 3, 4, 5, 6, 7, 8, &g_Scheduler, "itestEx8", itException);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(itestEx9, 1, 2, 3, 4, 5, 6, 7, 8, 9, &g_Scheduler, "itestEx9", itException);
	l_icallEx.WaitFor();
	itEx0 l_itEx0;
	itEx1 l_itEx1;
	itEx2 l_itEx2;
	itEx3 l_itEx3;
	itEx4 l_itEx4;
	itEx5 l_itEx5;
	itEx6 l_itEx6;
	itEx7 l_itEx7;
	itEx8 l_itEx8;
	itEx9 l_itEx9;
	l_icallEx.AsynCallEx(&l_itEx0, &itEx0::testEx0, &g_Scheduler, "itEx0::testEx0", &itEx0::Exception);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(&l_itEx1, &itEx1::testEx1, 1, &g_Scheduler, "itEx1::testEx1", &itEx1::Exception);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(&l_itEx2, &itEx2::testEx2, 1, 2, &g_Scheduler, "itEx2::testEx2", &itEx2::Exception);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(&l_itEx3, &itEx3::testEx3, 1, 2, 3, &g_Scheduler, "itEx3::testEx3", &itEx3::Exception);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(&l_itEx4, &itEx4::testEx4, 1, 2, 3, 4, &g_Scheduler, "itEx4::testEx4", &itEx4::Exception);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(&l_itEx5, &itEx5::testEx5, 1, 2, 3, 4, 5, &g_Scheduler, "itEx5::testEx5", &itEx5::Exception);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(&l_itEx6, &itEx6::testEx6, 1, 2, 3, 4, 5, 6, &g_Scheduler, "itEx6::testEx6", &itEx6::Exception);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(&l_itEx7, &itEx7::testEx7, 1, 2, 3, 4, 5, 6, 7, &g_Scheduler, "itEx7::testEx7", &itEx7::Exception);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(&l_itEx8, &itEx8::testEx8, 1, 2, 3, 4, 5, 6, 7, 8, &g_Scheduler, "itEx8::testEx8", &itEx8::Exception);
	l_icallEx.WaitFor();
	l_icallEx.AsynCallEx(&l_itEx9, &itEx9::testEx9, 1, 2, 3, 4, 5, 6, 7, 8, 9, &g_Scheduler, "itEx9::testEx9", &itEx9::Exception);
	l_icallEx.WaitFor();

	g_Scheduler.Shutdown();
	return 0;
}

