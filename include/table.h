#pragma once
#include "polynomial.h"
#include <string>
#include <vector>

class Table {

public:

	virtual polynomial findPolynomial(const std::string& polName) = 0; // find polynomial named polName
	virtual void addPolynomial(const std::string& polName, const polynomial& pol) = 0;
	virtual void delPolynomial(const std::string& polName) = 0;
	virtual unsigned int size() = 0;

	virtual ~Table() = 0 {};
};

class LinearArrTable: public Table {

public:
	LinearArrTable();

	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const std::string& polName, const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual unsigned int size() override;

	virtual ~LinearArrTable() {};
};

class LinearListTable : public Table {
public:
	LinearListTable();

	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const std::string& polName, const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual unsigned int size() override;

	virtual ~LinearListTable() {};
};

class OrderedTable : public Table {
public:
	OrderedTable();

	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const std::string& polName, const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual unsigned int size() override;

	virtual ~OrderedTable() {};
};

class TreeTable : public Table {
public:
	TreeTable();

	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const std::string& polName, const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual unsigned int size() override;

	virtual ~TreeTable() {};
};

class OpenAdressHashTable : public Table {
public:
	OpenAdressHashTable();

	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const std::string& polName, const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual unsigned int size() override;

	virtual ~OpenAdressHashTable() {};
};

class SeparateChainingHashTable : public Table {
public:
	SeparateChainingHashTable();

	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const std::string& polName, const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual unsigned int size() override;

	virtual ~SeparateChainingHashTable() {};
};

class Aggregator {
	std::vector<Table*> tables;
	int currentTable;
public:
	Aggregator() {
		tables.resize(6, nullptr);
		tables[0] = new LinearArrTable();
		tables[1] = new LinearListTable();
		tables[2] = new OrderedTable();
		tables[3] = new TreeTable();
		tables[4] = new OpenAdressHashTable();
		tables[5] = new SeparateChainingHashTable();
		currentTable = 0;
	}

	void selectTable(const std::string& tableName) ; // select table by its name: tables[0] - liar (short for linear array table - contains LinearArrTable object)
													 //			 				  tables[1] - lili (short for linear list table - contains LinearListTable object)
													 //		 					  tables[2] - ordr (short for ordered array table - contains OrderedTable object)
													 //							  tables[3] - tree (short for tree table - contains TreeTable object)
													 //							  tables[4] - opha (short for open adress hash table - contains OpenAdressHashTable object)
													 //							  tables[5] - seha (short for separate chaining hash table - contains SeparateChainingHashTable object)
	polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	void addPolynomial(const std::string& polName, const polynomial& pol);
	void delPolynomial(const std::string& polName);

	~Aggregator();
};
