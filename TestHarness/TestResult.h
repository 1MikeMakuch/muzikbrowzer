

#ifndef _TESTHARNESS_TESTRESULT_H_
#define _TESTHARNESS_TESTRESULT_H_

#include "StdAfx.h"
#include "MyString.h"

// TestResult is a simple little class which accepts notifications from
// tests.  In its current form, it takes these notifications and prints
// them on the standard output.
//
// If you need different behavior, you can alter it in place or subclass 
// TestResult to provide it.  The TestResult hierarchy can be as simple
// or complex as you require to support your application.


class Failure;

class TestResult
{
public:
		 TestResult () : mFailureCount (0), testCount(0) {}
	void testWasRun ();
	void startTests ();
	void addFailure (Failure failure);
	void endTests ();
	CString results() { return m_Result; }
	int failures() { return mFailureCount; }

private:
	int mFailureCount;
	int testCount;
	CString m_Result;
};

#endif
