////////////////////////////////////////////////////////////////////////
// Persistence.cpp - Saves DB to XML file and parses it back from XML //
// ver 1.0                                                            //
// Author:Sowmya Padmanabhi                                           //
// Source: Jim Fawcett												  //
////////////////////////////////////////////////////////////////////////
#include "Persistence.h"
#include "../DbCore/DbCore.h"

#ifdef TEST_PERSISTENCE

int main() {
		DbCore<std::string> db;
		Persistence<std::string> p;
		db = p.convertIntoObj("test.xml");
		std::cout << "\n\nprinting DB in main\n";
		std::string res = toXml(db);
		std::cout << "the XML result is:\n\n" << res << "\n\n";
	std::cin.get();
	return 0;
}
#endif // TEST_PERSISTENCE

