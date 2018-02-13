///////////////////////////////////////////////////////////////////////
// TestNoSqlDb.h - Contains tests for each requirement of solution   //
// ver 1.0                                                           //
// Author : Sowmya Padmanabhi                                        //
///////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* This package contains tests for each requirement.
* It calls the different packages mentioned below to execute requirement.
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* Utilities.h, Utilities.cpp
* Persistence.h
* Query.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 Feb 2018
* - first release
*/

#pragma once
#include <iostream>
#include <iomanip>
#include <functional>
#include <unordered_map>
#include"../DbCore/DbCore.h"
#include"../Utilities/TestUtilities/TestUtilities.h"
#include"../Utilities/StringUtilities/StringUtilities.h"
#include "../Persistence/Persistence.h"
#include "../Query/Query.h"


using namespace NoSqlDb;
using namespace XmlProcessing;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};


/////////////////////////////////////////////////////////
//////////////// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
	DbCore<std::string>& db() { return db_; }
private:
	static DbCore<std::string> db_;
};

DbCore<std::string> DbProvider::db_;

///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------

bool testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	DbCore<std::string> db;
	DbProvider dbp;
	dbp.db() = db;

	// create some demo elements and insert into db

	DbElement<std::string> demoElem;

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad("The good news is ...");

	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad() != "The good news is ...")
		return false;

	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();

	DbElement<std::string> demoElem = db["Fawcett"];

	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad("You should try ...");
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;

	demoElem.name("Jianan");
	demoElem.payLoad("Dr. Fawcett said ...");
	db["Sun"] = demoElem;

	demoElem.payLoad("You didn't demonstrate Requirement #4");
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;

	demoElem.payLoad("You didn't demonstrate Requirement #5");
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;

	demoElem.payLoad("You didn't demonstrate Requirement #6");
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;

	if (db.size() != 6)
		return false;

	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	putLine();

	std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");

	showHeader();
	showElem(db["Fawcett"]);

	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");

	// display the results

	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	std::cout << "\n  database keys are: ";
	showKeys(db);

	putLine();
	return true;
}

//----< requirement #4 and #5 >------------------------------------------
bool testR4and5()
{
	Utilities::title("Demonstrating Requirement #4, #5 - adding, deleting and modifying DB records");
	Query<std::string> q;
	DbElement<std::string> dbe;
	std::vector<std::string> children;
	dbe.name("Kumar");
	dbe.descrip("Student of CSE 687");
	dbe.dateTime(DateTime().now());
	dbe.payLoad("Super excited!");
	children.push_back("Children1");
	children.push_back("Children2");
	dbe.children(children);

	std::cout << "\nAdding new Key: Shankar\n\n";
	q.addKey("Shankar", dbe).show();

	dbe.descrip("A student at Syracuse");
	std::cout << "\nModifying key: Shankar\n";
	q.modifyDb("Shankar", dbe).show();

	dbe.name("Sowmya");
	dbe.payLoad("I am excited too!");
	children.push_back("Children3");
	children.push_back("Children4");
	dbe.children(children);

	std::cout << "\nAdding new Key: Sarkar\n";
	q.addKey("Sarkar", dbe).show();
	std::cout << "\nDeleting nonexistent key: aaa\n";
	q.deleteKey("aaa").show();
	std::cout << "\nDeleting existing key: Sarkar\n";
	q.deleteKey("Sarkar").show();
	q.addKey("Sarkar", dbe);
	return true;
}

//----< requirement #6 and #7 >------------------------------------------
bool testR6and7()
{
	Utilities::title("Demonstrating Requirement #6 - Querying on DbCore");
	Query<PayLoad> q;
	DbElement<PayLoad> dbe;
	std::vector<std::string> children;
	dbe.name("Kumar");
	dbe.descrip("Student at Syracuse");
	dbe.dateTime(DateTime().now());
	children.push_back("Children1");
	children.push_back("Children2");
	dbe.children(children);
	q.addKey("Shankar", dbe);

	dbe.name("Sowmya");
	children.push_back("Children3");
	children.push_back("Children4");
	dbe.children(children);
	q.addKey("Sarkar", dbe);

	std::vector<std::string> cat;
	cat.push_back("name");
	cat.push_back("description");
	q.getDbCore().dbStore().at("Shankar").payLoad().setFilePath("fileName");
	q.getDbCore().dbStore().at("Shankar").payLoad().setCategories(cat);

	dbe.name("Kumu");
	q.addKey("Shashank", dbe);

	q.show();

	std::cout << "\nStarting query #1: Value for key \"Shankar\"\n";
	q.valueForKey("Shankar").showElement();

	std::cout << "\nStarting query #2: Children for key \"Shankar\"\n";
	q.childrenForKey("Shankar");

	std::cout << "\nStarting query #3: Keys starting with \"Sha\"\n";
	q.from().select("key").where("(Sha)(.*)");

	std::cout << "\nStarting query #4: Keys with names starting with \"Kum\"\n";
	q.from().select("name").where("(Kum)(.*)");

	std::cout << "\nStarting query #5: checking for keys with date range from January to February 2018\n";
	bool res = q.dateRange(DateTime().makeTime(2018, 1, 5, 12, 50, 30) - DateTime().makeDuration(48, 30, 15, 10),
		DateTime().makeTime(2018, 2, 7, 12, 50, 30) + DateTime().makeDuration(48, 30, 15, 10));

	Utilities::title("Demonstrating Requirement #7 - Anding and Oring of keys which have returned from querying");
	std::cout << "\nResult from the first query which gets keys with name Kum:\n";
	std::vector<std::string> v1 = q.from().select("name").where("(Kum)(.*)").getQueryKeys();
	std::cout << "\nResult from the second query which gets keys with name Kuma:\n";
	std::vector<std::string> v2 = q.from().select("name").where("(Kuma)(.*)").getQueryKeys();
	std::cout << "\nResult from anding query #1 and #2:\n";
	q.andKeys(v1, v2);
	std::cout << "\nResult from oring query #1 and #2:\n";
	q.orKeys(v1, v2);
	return true;
}

//----< requirement #8 >------------------------------------------
bool testR8()
{
	Utilities::title("Demonstrating Requirement #8 - Persisting and Restoring the database to and from XML file");
	DbCore<std::string> db;
	Persistence<std::string> p;

	//Converts XML to DbCore object
	db = p.convertIntoObj("test.xml");

	//Converts DbCore to XML
	std::string res = toXml(db);
	std::cout << "\n\n Result after converting to XML: " << res << "\n\n";

	return true;
}

//----< requirement #9 >------------------------------------------
bool testR9()
{
	Utilities::title("Demonstrating Requirement #9 - Payload has been implemented as a class containing file path and vector of categories");
	DbCore<PayLoad> db;
	DbElement<PayLoad> dbe;
	dbe.name("Rashmi");
	dbe.dateTime(DateTime().now());
	dbe.descrip("Demonstrating DB core instantiated with PayLoad");
	dbe.payLoad().setFilePath("../HelloWorld");
	std::vector<std::string> v;
	v.push_back("Children 1");
	v.push_back("Children 2");
	dbe.children(v);
	v.clear();
	v.push_back("Category 1");
	v.push_back("Category 2");
	dbe.payLoad().setCategories(v);
	db["Prakash"] = dbe;
	std::cout << "\nDisplaying DB with PayLoad:\n";
	showDb(db);
	return true;
}

//----< requirement #10 >------------------------------------------
bool testR10()
{
	Utilities::title("Implementation of the solution in packages: DBCore, Query, Test, Persistence, Executive");
	std::cout << "\n The package structure is shown through a Visual examination.\n";
	std::cout << "DBCore: To implement the NoSQLDB, the DbCore contains the classes and the corresponding functionality.\n";
	std::cout << "Query: The required functionality to query from NoSQLDB is contained in this package.\n";
	std::cout << "Persistence: In order to persist the DB as an XML file on the disk, this contains the required functionality.\n";
	std::cout << "Test: Contains the required functions to execute every test case.\n";
	std::cout << "Executive: The starting point of execution of the test cases which are defined in Test package.\n";
	std::cout << "\nAlternatively, running \"dir\" command under solution folder displays the packages.\n\n";
	return true;
}

//----< requirement #11 >------------------------------------------
bool testR11() {
	Utilities::title("Demonstrating Requirement #11 - Project's package structure and the dependency being described by the XML");
	std::string filePath = "PkgStr.txt";
	std::ifstream in(filePath);
	if (!in.good())
		throw(std::exception(("can't open source file " + filePath).c_str()));
	std::ostringstream out;
	out << in.rdbuf();
	std::string forOpening = std::move(out.str());
	std::cout << "\nThe XML with package structure is:\n" << forOpening << "\n\n";
	return true;
}

//----< requirement #12 >------------------------------------------
bool testR12() {
	Utilities::title("Demonstrating Requirement #12 - The requirements #2 to #9 are met");
	std::cout << "\nIf the statement \"all tests passed\" is printed in the end, requirement #12 is met.\n";
	return true;
}

//----< requirement #13 >------------------------------------------
bool testR13() {
	Utilities::title("Demonstrating Requirement #13 - Package diagram PDF");
	std::string path = "..\\PackageDiagram.pdf";
	std::cout << "\nThe PDF is present in path: " << path << " i.e. in the same path as the solution file.\n\n";
	return true;
}

//----< test stub >----------------------------------------------------
