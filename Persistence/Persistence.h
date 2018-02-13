#pragma once
//////////////////////////////////////////////////////////////////////
// Persistence.h - Saves DB to XML file and parses it back from XML //
// ver 1.0                                                          //
// Author:Sowmya Padmanabhi                                          //
// Source: Jim Fawcett												//
//////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* The Persistence class converts DBCore to XML and vice versa.
* The XML file is persisted in the path of the execution point of the solution.
* Contains the following classes:
* Persistence which contains methods to convert into XML and convert from XML
*
* Required Files:
* ---------------
* XmlElement.h, XmlElement.cpp
* XmlParser.h, XmlParser.cpp
* XmlDocument.h, XmlDocument.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 Feb 2018
*/
#include "../DbCore/DbCore.h"
#include"../XmlParser/XmlParser.h"
#include"../XmlDocument/XmlDocument.h"
#include"../XmlElement/XmlElement.h"
#include <sstream>

using namespace NoSqlDb;
using namespace XmlProcessing;

template<typename T>
class Persistence
{
public:
	using xml = std::string;
	using Key = std::string;

	//----Opens file and converts string to input stream instance----
	DbCore<T> convertIntoObj(const Key xmlString) {
		std::ifstream in(xmlString);
		//in.clear(in.goodbit);
		if (!in.good())
			throw(std::exception(("can't open source file " + xmlString).c_str()));
		std::ostringstream out;
		out << in.rdbuf();
		std::string forOpening = std::move(out.str());
		DbCore<T> db = fromXml(forOpening);
		return db;
	}

	DbCore<T> fromXml(const xml xmlStr);

private:
	DbCore<T> db_;
};

//----Converts from DBCore to XML and saves XML file to current directory----
template<typename T>
std::string toXml(DbCore<T> db)
{
	std::cout << "\n\nconverting db object back to XML\n\n";
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	Sptr pDb = makeTaggedElement("NoSQLDB");
	Sptr pDocElem = makeDocElement(pDb);
	XmlDocument xDoc(pDocElem);

	for (auto item : db)
	{
		Sptr pRecord = makeTaggedElement("record");
		pDb->addChild(pRecord);
		Sptr pKey = makeTaggedElement("key", item.first);
		pRecord->addChild(pKey);

		Sptr pValue = makeTaggedElement("value");
		pRecord->addChild(pValue);
		Sptr pName = makeTaggedElement("name", item.second.name());
		pValue->addChild(pName);
		Sptr pDateTime = makeTaggedElement("dateTime", item.second.dateTime());
		pValue->addChild(pDateTime);
		Sptr pPayLoad = makeTaggedElement("payLoad", item.second.payLoad());
		pValue->addChild(pPayLoad);
		Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
		pValue->addChild(pDescrip);
		Sptr pChildren = makeTaggedElement("children");
		pValue->addChild(pChildren);
		for (auto child : item.second.children()) {
			Sptr pchild = makeTaggedElement("key", child);
			pChildren->addChild(pchild);
		}
	}
	std::string Xml = xDoc.toString();
	std::ofstream out("test.xml");
	out << Xml;
	out.close();
	return Xml;
}

//----Converts from XML to DBCore and returns DBCore object----
template<typename T>
DbCore<T> Persistence<T>::fromXml(const xml xmlString)
{
	std::cout << "\n\nPrinting XML to be converted into DB Store\n\n" << xmlString << std::endl;

	XmlDocument newXDoc(xmlString, XmlDocument::str);
	std::cout << newXDoc.toString();
	DbCore<std::string> db;

	using Sptr = std::shared_ptr<AbstractXmlElement>;
	std::vector<Sptr> records = newXDoc.descendents("record").select();
	for (auto pRecord : records)
	{
		Key key;
		//Key childrenTag;
		DbElement<std::string> elem;
		std::vector<std::string> childList;
		std::vector<Sptr> pChildren = pRecord->children();
		for (auto pChild : pChildren)
		{
			// record's children are key and value

			if (pChild->tag() == "key")
			{
				// get value of TextElement child
				key = pChild->children()[0]->value();
			}
			else
			{
				// value represents a MockDbElem with children name and description

				std::vector<Sptr> pValueChildren = pChild->children();

				for (auto pValueChild : pValueChildren)
				{
					if (pValueChild->tag() == "name")
					{
						// get value of TextElement child
						elem.name(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "description")
					{
						// get value of TextElement child
						elem.descrip(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "dateTime")
					{
						// get value of TextElement child
						elem.dateTime(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "payLoad")
					{
						// get value of TextElement child
						elem.payLoad(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "children") {
						std::vector<Sptr> pValueKeyChildren = pValueChild->children();
						for (auto pValueChild : pValueKeyChildren) {
							if (pValueChild->tag() == "key") {
								childList.push_back(pValueChild->children()[0]->value());
							}
						}
					}
				}
			}
		}
		elem.children(childList);
		db[key] = elem;
	}
	std::cout << "\n\nShowing DB extracted from XML\n\n";
	showDb(db);

	std::cout << "\n\n";
	return db;
}
