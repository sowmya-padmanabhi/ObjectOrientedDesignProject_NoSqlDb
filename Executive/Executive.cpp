///////////////////////////////////////////////////////////////////////
// Executive.cpp - Execution starting point for NoSqlDb project      //
// ver 1.0                                                           //
// Author : Sowmya Padmanabhi                                        //
///////////////////////////////////////////////////////////////////////
#include "../Test/TestNoSqlDb.h"

#ifdef TEST_EXECUTIVE

using namespace Utilities;

//----Executes test function written in Test package----
int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();

	TestExecutive ex;

	// defining test structures with test function and message

	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<std::string>" };
	TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };
	TestExecutive::TestStr ts4and5{ testR4and5, "Implementatoin of the functions add, delete and modify" };
	TestExecutive::TestStr ts6and7{ testR6and7, "Anding and Oring querying of the keys" };
	TestExecutive::TestStr ts8{ testR8,"Persisting and restoring the database to and from the XML file" };
	TestExecutive::TestStr ts9{ testR9, "Payload has been implemented as a class containing file path and vector of categories" };
	TestExecutive::TestStr ts10{ testR10, "Implementation of the solution in packages: DBCore, Query, Test, Persistence, Executive" };
	TestExecutive::TestStr ts11{ testR11, "Project's package structure being described by the XML" };
	TestExecutive::TestStr ts12{ testR12, "#2 to #9 requirements are working" };
	TestExecutive::TestStr ts13{ testR13, "Package diagram PDF" };

	// register test structures with TestExecutive instance, ex

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4and5);
	ex.registerTest(ts6and7);
	ex.registerTest(ts8);
	ex.registerTest(ts9);
	ex.registerTest(ts10);
	ex.registerTest(ts11);
	ex.registerTest(ts12);
	ex.registerTest(ts13);

	// run tests

	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	std::cin.get();
	return 0;
}
#endif