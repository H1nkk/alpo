#include <table.h>

// *** LinearArrTable ***

LinearArrTable::LinearArrTable() {}

void LinearArrTable::addPolynomial(const std::string& polName, const polynomial& pol) // TODO check for unique polName is in app section
{
	if (findPolynomial(polName) == std::nullopt)
		table.push_back({ polName, pol });
}

std::optional<polynomial> LinearArrTable::findPolynomial(const std::string& polName) 
{
	for (auto rec: table) 
		if (rec.key == polName)
			return rec.value;
	return {};
}

void LinearArrTable::delPolynomial(const std::string& polName) 
{
	for (int i = 0; i < table.size(); i++) {
		Pol rec = table[i];
		if (rec.key == polName) 
		{
			table.erase(std::next(table.begin(), i)); // needs a lot of testing
			return;
		}
	}
}

unsigned int LinearArrTable::size() 
{
	return table.size();
}

bool LinearArrTable::empty() 
{
	return table.empty();
}

// *** LinearListTable ***

LinearListTable::LinearListTable(): pFirst(nullptr), tableSize(0) {}

std::optional<polynomial> LinearListTable::findPolynomial(const std::string& polName) 
{
	TNode* p = pFirst;
	while (p) 
	{
		if (p->key == polName) 
		{
			return p->value;
		}
		p = p->pNext;
	}
	return {};
}

void LinearListTable::addPolynomial(const std::string& polName, const polynomial& pol) 
{
	if (findPolynomial(polName) != std::nullopt) return; // uniqueness check
	tableSize++;
	TNode* p = pFirst;
	if (!p) 
	{
		pFirst = new TNode{ polName, pol, nullptr };
		return;
	}
	while (p->pNext) 
		p = p->pNext;

	p->pNext = new TNode{ polName, pol, nullptr };
}

void LinearListTable::delPolynomial(const std::string& polName) // needs a lot of testing
{
	TNode* p = pFirst;
	if (!pFirst) return;
	if (pFirst->key == polName) 
	{
		TNode* tmp = pFirst->pNext;
		delete pFirst;
		pFirst = tmp;
		tableSize--;
		return;
	}
	while (p->pNext) 
	{
		if (p->pNext->key == polName) {
			TNode* tmp = p->pNext->pNext;
			delete p->pNext;
			p->pNext = tmp;
			tableSize--;
			return;
		}
		p = p->pNext;
	}
}

unsigned int LinearListTable::size() 
{
	return tableSize;
}

bool LinearListTable::empty()
{
	return tableSize == 0; // or pFirst == nullptr
}

LinearListTable::~LinearListTable()
{
	TNode* p = pFirst;
	while (p) {
		TNode* tmp = p;
		p = p->pNext;
		delete tmp;
	}
}

// *** OrderedTable ***

OrderedTable::OrderedTable() {

}

void OrderedTable::addPolynomial(const std::string& polName, const polynomial& pol) 
{

}

std::optional<polynomial> OrderedTable::findPolynomial(const std::string& polName)
{
	return {};
}

void OrderedTable::delPolynomial(const std::string& polName)
{

}

unsigned int OrderedTable::size()
{
	return -1;

}

bool OrderedTable::empty()
{
	return true;
}

// *** TreeTable ***

TreeTable::TreeTable() {

}

void TreeTable::addPolynomial(const std::string& polName, const polynomial& pol) 
{

}

std::optional<polynomial> TreeTable::findPolynomial(const std::string& polName)
{
	return {};

}

void TreeTable::delPolynomial(const std::string& polName)
{

}

unsigned int TreeTable::size()
{
	return -1;

}

bool TreeTable::empty()
{
	return true;

}

// *** OpenAddressHashTable ***

OpenAddressHashTable::OpenAddressHashTable() {

}

void OpenAddressHashTable::addPolynomial(const std::string& polName, const polynomial& pol)
{

}

std::optional<polynomial> OpenAddressHashTable::findPolynomial(const std::string& polName)
{
	return {};

}

void OpenAddressHashTable::delPolynomial(const std::string& polName)
{

}

unsigned int OpenAddressHashTable::size()
{
	return -1;

}

bool OpenAddressHashTable::empty()
{
	return true;

}

// *** SeparateChainingHashTable ***

SeparateChainingHashTable::SeparateChainingHashTable() {

}

void SeparateChainingHashTable::addPolynomial(const std::string& polName, const polynomial& pol)
{

}

std::optional<polynomial> SeparateChainingHashTable::findPolynomial(const std::string& polName)
{
	return {};
}

void SeparateChainingHashTable::delPolynomial(const std::string& polName)
{

}

unsigned int SeparateChainingHashTable::size()
{
	return -1;
}

bool SeparateChainingHashTable::empty()
{
	return true;

}

// *** Aggregator ***

Aggregator::Aggregator() {
	tables.resize(6, nullptr);
	tables[0] = new LinearArrTable();
	tables[1] = new LinearListTable();
	tables[2] = new OrderedTable();
	tables[3] = new TreeTable();
	tables[4] = new OpenAddressHashTable();
	tables[5] = new SeparateChainingHashTable();
	currentTable = 0;
}

void Aggregator::selectTable(const std::string& tableName) {
	if (tableName == "liar") 
	{
		currentTable = 0;
	} 
	else if (tableName == "lili")
	{
		currentTable = 1;
	}
	else if (tableName == "ordr")
	{
		currentTable = 2;
	}
	else if (tableName == "tree")
	{
		currentTable = 3;
	}
	else if (tableName == "opha")
	{
		currentTable = 4;
	}
	else if (tableName == "seha")
	{
		currentTable = 5;
	}
}

std::optional<polynomial> Aggregator::findPolynomial(const std::string& polName) {
	return tables[currentTable]->findPolynomial(polName);
}

void Aggregator::addPolynomial(const std::string& polName, const polynomial& pol) {
	tables[currentTable]->addPolynomial(polName, pol);
}

void Aggregator::delPolynomial(const std::string& polName) {
	tables[currentTable]->delPolynomial(polName);
}

unsigned int Aggregator::size() {
	return tables[currentTable]->size();
}

Aggregator::~Aggregator() {
	for (auto table : tables) {
		delete table; // i dont know if this calls tables' destructors
	}
}
