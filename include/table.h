#pragma once
#include "polynomial.h"
#include <string>

class Table {

public:
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~Table() = 0 {};
};

class LinearArrTable: Table {
public:
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~LinearArrTable() {};
};

class LinearListTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~LinearListTable() {};
};

class OrderedTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~OrderedTable() {};
};

class TreeTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~TreeTable() {};
};

class OpenAdressHashTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~OpenAdressHashTable() {};
};

class SeparateChainingHashTable : Table {
public:
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~SeparateChainingHashTable() {};
};
