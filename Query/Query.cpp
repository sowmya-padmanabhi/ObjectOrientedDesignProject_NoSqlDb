///////////////////////////////////////////////////////////////////////
// Query.cpp - test file for Query package                           //
// ver 1.0                                                           //
// Author : Sowmya Padmanabhi                                        //
///////////////////////////////////////////////////////////////////////
#include "Query.h"
//#include "../DbCore/DbCore.h"
//#include "../DateTime/DateTime.h"

#ifdef TEST_QUERY

//----Main method to test Query package----
int main() {

	// ----------------- Requirements 4 and 5
	/*std::cout << "Testing Query package\n\n";*/
	Query<PayLoad> q;
	DbElement<PayLoad> dbe;
	std::vector<std::string> children;
	dbe.name("Sowmya");
	dbe.descrip("Student of CSE 687");
	dbe.dateTime(DateTime().now());
	//dbe.payLoad("Hello world!");
	children.push_back("Child1");
	children.push_back("Child2");
	dbe.children(children);

	//std::cout << "\nAdding key: Kumar\n\n";
	q.addKey("Kumar", dbe);

	dbe.descrip("An interested student of CSE 687");
	////std::cout << "\nModifying key: Kumar\n";
	q.modifyDb("Kumar", dbe);

	dbe.name("Sowmya");
	//dbe.payLoad("Hello world from me too!");
	children.push_back("Child3");
	children.push_back("Child4");
	dbe.children(children);

	//std::cout << "\nAdding key: Sarkar\n";
	q.addKey("Sarkar", dbe);
	//std::cout << "\nDeleting key: aaa\n";
	//q.deleteKey("aaa").show();
	//std::cout << "\nDeleting key: Sarkar\n";
	//q.deleteKey("Sarkar").show();
	//q.addKey("Sarkar", dbe);

	// -------------- Requirement 6
	//std::cout << "\nShowing value for key: Kumar";
	//q.valueForKey("Kumar").showElement();

	/*Query<PayLoad> qp;
	qp.addKey("Kumar", dbe);*/
	std::vector<std::string> cat;
	cat.push_back("name");
	cat.push_back("description");
	q.getDbCore().dbStore().at("Kumar").payLoad().setFilePath("Sowmya");
	q.getDbCore().dbStore().at("Kumar").payLoad().setCategories(cat);
	q.show();
	/*qp.show();

	qp.childrenForKey("Kumar");*/

	//---- Requirement 6
	//std::cout << "\nRunning query 2\n";

	//bool res = q.dateRange(DateTime().makeTime(2018, 1, 5, 12, 50, 30)-DateTime().makeDuration(48, 30, 15, 10),
	//	DateTime().makeTime(2018, 2, 7, 12, 50, 30)+DateTime().makeDuration(48, 30, 15, 10));
	//std::cout << "\nthe result is " << res << "\n";
	dbe.name("Nammu");
	q.addKey("Prabhakara", dbe);

	std::vector<std::string> v1 = q.from().select("name").where("(Nam)(.*)").getQueryKeys();
	for (auto i : v1)
		std::cout << i << " ";
	std::cout << "\n";
	std::vector<std::string> v2 = q.from().select("name").where("(Namm)(.*)").getQueryKeys();
	for (auto i : v2)
		std::cout << i << " ";
	std::cout << "\n";
	q.showKeys(q.andKeys(v1, v2));

	std::cin.get();
	return 0;
}

#endif // TEST_QUERY

