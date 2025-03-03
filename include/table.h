#pragma once
#include "polynomial.h"
#include <string>
#include <vector>

class Table {

public:
	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual ~Table() = 0 {};
};

class LinearArrTable: Table {
public:
	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual ~LinearArrTable() {};
};

class LinearListTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual ~LinearListTable() {};
};

class OrderedTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual ~OrderedTable() {};
};

class TreeTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual ~TreeTable() {};
};

class OpenAdressHashTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual ~OpenAdressHashTable() {};
};

class SeparateChainingHashTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName) override; // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol) override;
	virtual void delPolynomial(const std::string& polName) override;
	virtual ~SeparateChainingHashTable() {};
};

class Aggregator {
	std::vector<Table*> tables;
public:
	void selectTable(const std::string& tableName) ; // select table by its name: tables[0] - liar (short for linear array table - contains LinearArrTable object)
													//							 tables[1] - lili (short for linear list table - contains LinearListTable object)
													//							 tables[2] - ordr (short for ordered array table - contains OrderedTable object)
													//							 tables[3] - tree (short for tree table - contains TreeTable object)
													//							 tables[4] - opha (short for open adress hash table - contains OpenAdressHashTable object)
													//							 tables[5] - seha (short for separate chaining hash table - contains SeparateChainingHashTable object)
	polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	void addPolynomial(const polynomial& pol);
	void delPolynomial(const std::string& polName);
	~Aggregator();
};
