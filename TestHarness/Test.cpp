

#include "Test.h"
#include "TestRegistry.h"
#include "TestResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


Test::Test (CString testName) : m_name (testName) 
{
	TestRegistry::addTest (this);
}


void Test::run (TestResult& result) 
{
	runTest (result);
	result.testWasRun();
}

