

#include "Test.h"
#include "TestResult.h"
#include "TestRegistry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void TestRegistry::addTest (Test *test) 
{
	instance ().add (test);
}


void TestRegistry::runAllTests (TestResult& result) 
{
	instance ().run (result);
}


TestRegistry& TestRegistry::instance () {
	static TestRegistry registry;
	return registry;
}


void TestRegistry::add (Test *test) {
	tests.push_back (test);
}


void TestRegistry::run (TestResult& result) {
	result.startTests ();
	for (std::vector<Test *>::iterator it = tests.begin ();
			it != tests.end (); ++it) {
		CString name = (*it)->name();
		OutputDebugString("TestHarness: " + name + "\r\n");
		(*it)->run (result);
	}
	result.endTests ();
	OutputDebugString("TestHarness ended\r\n");
}



