
#include "TestResult.h"
#include "Failure.h"

#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void TestResult::testWasRun()
{
	testCount++;
}

void TestResult::startTests () 
{
}

void TestResult::addFailure (Failure failure) 
{
	AutoBuf buf(1000);
	sprintf(buf.p, "%s failure: \"%s\" line: %d in %s\r\n",
		failure.testName, failure.condition, failure.lineNumber,
		failure.fileName);
	m_Result += buf.p;

	mFailureCount++;

//		<< "Failure: \"" << failure.condition.c_str () << "\" " 
//		<< "line " << failure.lineNumber << " in "
//		<< failure.fileName.c_str ()
//		<< std::endl;
}

void TestResult::endTests () 
{
	AutoBuf buf(1000);
	sprintf(buf.p, "%d tests run\r\n", testCount);
	m_Result += buf.p;
	if (mFailureCount > 0) {
		sprintf(buf.p, "There were %d failures\r\n", mFailureCount);
		m_Result += buf.p;
	} else {
		m_Result += "There were no test failures\r\n";
	}
}
