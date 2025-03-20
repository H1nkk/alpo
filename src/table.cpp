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
