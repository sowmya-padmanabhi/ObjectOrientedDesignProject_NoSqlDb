#pragma once
///////////////////////////////////////////////////////////////////////
// Query.h - Implements querying functionality for DBCore NoSQLDB    //
// ver 1.0                                                           //
// Author : Sowmya Padmanabhi                                        //
///////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* This package provides querying functionality to NoSQLDB
* The different methods meet different querying requirements
* Class PayLoad contains the string file path and vector of string categories
* The template class query is implemented in order to accept generic type and therefore, the PayLoad.
* This allows querying to be performed on PayLoad class included as member variable to DBElement
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 Feb 2018
* - first release
*/

#include <regex>
#include "../DbCore/DbCore.h"

using namespace NoSqlDb;

class PayLoad {
public:
	using Categories = std::vector<std::string>;

	//----setter and getter for filePath variable----
	std::string& getFilePath() { return filePath; }
	std::string getFilePath() const { return filePath; }
	void setFilePath(const std::string& fp) { filePath = fp; }

	//----setter and getter for categories variable----
	Categories& getCategories() { return categories; }
	Categories getCategories() const { return categories; }
	void setCategories(const Categories& cat) { categories = cat; }
private:
	std::string filePath;
	std::vector<std::string> categories;
};

//----overloads << operator to print PayLoad object when called from showDb()----
std::ostream& operator<<(std::ostream& out, PayLoad p) {
	out << "\nPath: " << p.getFilePath();
	out << "\nCategories: ";
	for (auto item : p.getCategories())
		out << item << " ";
	out << "\n";
	return out;
}

template<typename T>
class Query
{
public:
	//----Displays DB----
	using Keys = std::vector<std::string>;
	Query<T> show() {
		showDb(db);
		return *this;
	}

	//----Displays DBElement----
	Query<T> showElement(std::ostream& out = std::cout) {
		showHeader();
		showElem(dbe);
		return *this;
	}

	//----Adds key to DB----
	Query<T> addKey(std::string key, DbElement<T> ele) {
		db[key] = ele;
		return *this;
	}

	//----Deletes key to DB----
	Query<T> deleteKey(std::string key) {
		if (db.contains(key)) {
			dbe = db[key];
			std::vector<std::string> childKeys = dbe.children();
			childKeys.clear();
			db.dbStore().erase(key);
		}
		return *this;
	}

	//----Modifies DB----
	Query<T> modifyDb(std::string key, DbElement<T> ele) {
		deleteKey(key);
		addKey(key, ele);
		return *this;
	}

	//----Gets value for key----
	Query<T> valueForKey(std::string key) {
		dbe = db[key];
		return *this;
	}

	//----Gets children for key----
	Query<T> childrenForKey(std::string key) {
		std::cout << "\nPrinting children of key: " << key << "\n";
		std::vector<std::string> children;
		children = db.dbStore().at(key).children();
		std::cout << "\n";
		for (auto item : children)
			std::cout << item << " ";
		std::cout << "\n";
		return *this;
	}

	//----setter and getter of DBCore object----
	DbCore<T> getDbCore() const { return db; }
	DbCore<T>& getDbCore() { return db; }
	void setDbCore(const DbCore<T>& dbCore) { db = dbCore; }

	//----setter and getter of result key set----
	Keys getQueryKeys() const { return result; }
	Keys& getQueryKeys() { return result; }
	void setQueryKeys(const Keys& keys) { result = keys; }

	//----initial set of keys of DBCore object----
	Query<T> from(){
		std::cout << "\nIntial set of keys: ";
		result = db.keys();
		showKeys(result);
		return *this;
	}

	//----select field to be queried----
	Query<T> select(std::string searchField){
		if (searchField == "name")
			value = 1;
		if (searchField == "description")
			value = 7;
		if (searchField == "payLoad")
			value = 8;
		if (searchField == "children")
			value = 9;
		if (searchField == "key")
			value = 2;
		
		return *this;
	}

	//----matches search condition with field----
	Query<T> where(std::string searchString){
		Keys keylistStart = result;
		int field = value;
		std::regex conditionMatch(searchString);
		bool matched = false;
		Keys keylistResult;

		for (std::vector<std::string>::iterator itkeys = keylistStart.begin(); itkeys != keylistStart.end(); ++itkeys)
		{
			if (field == 2)
				matched = regEx(*itkeys, conditionMatch);
			else {
				dbe = db[*itkeys];
				switch (field) {
				case 1: matched = regEx(dbe.name(), conditionMatch); break;
				case 7: matched = regEx(dbe.descrip(), conditionMatch); break;
				case 8: matched = regEx(dbe.payLoad(), conditionMatch); break;
				case 9: matched = regEx(dbe.children(), conditionMatch); break;
				default: break;
				}
			}
			if (matched == true)
			{
				keylistResult.push_back(*itkeys);
			}
		}
		std::cout << "\n  ";
		showKeys(keylistResult);
		result.clear();
		result = keylistResult;
		return *this;
	}

	//----returns true if DBElement's dateTime is within range provided----
	bool dateRange(DateTime d1, DateTime d2) {
		bool flag = false;
		result.clear();
		DateTime *dte = &d2;
		if (dte == NULL)
			d2 = DateTime().now();
		for (auto key : db.keys()) {
			dbe = db[key];
			if (dbe.dateTime() > d1 && dbe.dateTime() < d2) {
				flag = true;
				result.push_back(key);
			}
		}
		showKeys(result);
		return flag;
	}

	//----prints vector of key strings----
	Query<T> showKeys(Keys keys) {
		for (auto key : keys)
		{
			std::cout << key << " ";
		}
		return *this;
	}

	//----regex matches 2 inputs and returns true if matched----
	bool regEx(std::string inp, std::regex searchExp){
		bool match = false;
		if (std::regex_match(inp, searchExp) == true)
		{
			match = true;
		}
		return match;
	}

	//----same as above, function overloaded----
	bool regEx(std::vector<std::string> inp, std::regex searchExp) {
		bool match = false;
		for (auto i : inp) {
			if (std::regex_match(i, searchExp) == true)
			{
				match = true;
				break;
			}
		}
		return match;
	}

	//----same as above, function overloaded----
	bool regEx(PayLoad inp, std::regex searchExp) {
		bool match = false;
		if (std::regex_match(inp.getFilePath(), searchExp) == true)
		{
			match = true;
		}
		else {
			for (auto cat : inp.getCategories()) {
				if (std::regex_match(cat, searchExp) == true) {
					match = true;
				}
			}
		}
		return match;
	}

	//----Oring of keys----
	Keys orKeys(Keys k1, Keys k2) {
		k1.insert(k1.end(), k2.begin(), k2.end());
		std::sort(k1.begin(), k1.end());
		k1.erase(std::unique(k1.begin(), k1.end()), k1.end());
		std::cout << "\nprinting elements of or:\n";
		for (auto i : k1)
			std::cout << i << " ";
		std::cout << "\n";
		//result = k1;
		//return *this;
		return k1;
	}

	//----Anding of keys----
	Keys andKeys(Keys k1, Keys k2) {
		std::sort(k1.begin(), k1.end());
		std::sort(k2.begin(), k2.end());
		std::vector<std::string> k3;

		std::set_intersection(k1.begin(), k1.end(),
			k2.begin(), k2.end(),
			std::back_inserter(k3));
		std::cout << "\nprinting elements of and:\n";
		for (auto i : k3)
			std::cout << i << " ";
		std::cout << "\n";
		//result = k3;
		//return *this;
		return k3;
	}

private:
	DbCore<T> db;
	DbElement<T> dbe;
	int value = 0;
	Keys result;
};