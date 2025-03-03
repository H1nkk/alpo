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
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~LinearArrTable() {};

};

class LinearListTable : Table {
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~LinearListTable() {};
};

class OrderedTable : Table {
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~OrderedTable() {};
};

class TreeTable : Table {
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~TreeTable() {};
};

class OpenAdressHashTable : Table {
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~OpenAdressHashTable() {};
};

class SeparateChainingHashTable : Table {
	virtual polynomial findPolynomial(const std::string& polName); // find polynomial named polName
	virtual void addPolynomial(const polynomial& pol);
	virtual void delPolynomial(const std::string& polName);
	virtual ~SeparateChainingHashTable() {};
};
