

#include "Test.h"
#include "TestResult.h"
#include "TestRegistry.h"
#include "MyLog.h"

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
		logger.logd("TestHarness: " + name);
		(*it)->run (result);
	}
	result.endTests ();
	logger.logd("TestHarness ended");
}



