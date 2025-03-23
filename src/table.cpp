#include <table.h>

// *** LinearArrTable ***

LinearArrTable::LinearArrTable() {}

void LinearArrTable::addPolynomial(const std::string& polName, const polynomial& pol) // TODO check for unique polName is in app section
{
	if (findPolynomial(polName) == std::nullopt)
		mTable.push_back({ polName, pol });
}

std::optional<polynomial> LinearArrTable::findPolynomial(const std::string& polName) 
{
	for (auto rec: mTable) 
		if (rec.key == polName)
			return rec.value;
	return {};
}

void LinearArrTable::delPolynomial(const std::string& polName) 
{
	for (int i = 0; i < mTable.size(); i++) {
		Pol rec = mTable[i];
		if (rec.key == polName) 
		{
			mTable.erase(std::next(mTable.begin(), i)); // needs a lot of testing
			return;
		}
	}
}

unsigned int LinearArrTable::size() 
{
	return mTable.size();
}

bool LinearArrTable::empty() 
{
	return mTable.empty();
}

// *** LinearListTable ***

LinearListTable::LinearListTable(): pFirst(nullptr), mTableSize(0) {}

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
	mTableSize++;
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
		mTableSize--;
		return;
	}
	while (p->pNext) 
	{
		if (p->pNext->key == polName) {
			TNode* tmp = p->pNext->pNext;
			delete p->pNext;
			p->pNext = tmp;
			mTableSize--;
			return;
		}
		p = p->pNext;
	}
}

unsigned int LinearListTable::size() 
{
	return mTableSize;
}

bool LinearListTable::empty()
{
	return mTableSize == 0; // or pFirst == nullptr
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

OrderedTable::OrderedTable(): pTable(new Pol[4]), mTableSize(4), mCurrentSize(0) {}

std::optional<polynomial> OrderedTable::findPolynomial(const std::string& polName)
{
	int i = mCurrentSize / 2;
	int leftBorder = 0;
	int rightBorder = mCurrentSize;
	while (pTable[i].key != polName || rightBorder - leftBorder != 1)
	{
		if (pTable[i].key.compare(polName) > 0)
		{
			rightBorder = i;
			i -= 1 + (rightBorder - leftBorder) / 2;
		}
		if (pTable[i].key.compare(polName) < 0)
		{
			leftBorder = i;
			i += 1 + (rightBorder - leftBorder) / 2;
		}
	}
	if (pTable[i].key == polName)
		return pTable[i].value;
	return {};
}

void OrderedTable::addPolynomial(const std::string& polName, const polynomial& pol)
{
	if (findPolynomial(polName) != std::nullopt) return; // uniqueness check
	int i = mCurrentSize / 2;
	int leftBorder = 0;
	int rightBorder = mTableSize;
	while (rightBorder - leftBorder != 1) // binary search
	{
		if (pTable[i].key.compare(polName) > 0)
		{
			rightBorder = i;
			i = (rightBorder - leftBorder) / 2;
		}
		if (pTable[i].key.compare(polName) < 0)
		{
			leftBorder = i;
			i = (rightBorder - leftBorder) / 2;
		}
	}
	if (mTableSize == mCurrentSize) // repacking and adding entry
	{
		Pol* pHelpTable = new Pol[mTableSize * 2];
		mTableSize *= 2;
		for (int j = 0; j < leftBorder + 1; j++)
			pHelpTable[j] = pTable[j];	
		pHelpTable[leftBorder + 1] = { polName,  pol };
		for (int j = leftBorder + 1; j < mCurrentSize; j++)
			pHelpTable[j+1] = pTable[j];
		delete[]  pTable;
		pTable = pHelpTable;
	}
	else // adding entry
	{
		Pol help;
		for (int j = leftBorder + 1; j < mCurrentSize; j++)
		{
			help = pTable[j + 1];
			pTable[j + 1] = pTable[j];
		}
		pTable[leftBorder + 1] = { polName,  pol };
	}
	mCurrentSize++;
}

void OrderedTable::delPolynomial(const std::string& polName)
{
	if (findPolynomial(polName) != std::nullopt) return; // uniqueness check
	int i = mCurrentSize / 2;
	int leftBorder = 0;
	int rightBorder = mTableSize;
	while (pTable[i].key != polName)
	{
		if (pTable[i].key.compare(polName) > 0)
		{
			rightBorder = i;
			i -= 1 + (rightBorder - leftBorder) / 2;
		}
		if (pTable[i].key.compare(polName) < 0)
		{
			leftBorder = i;
			i += 1 + (rightBorder - leftBorder) / 2;
		}
	}
	for (int j = 0; j < mCurrentSize; j++)
	{
		pTable[j] = pTable[j+1];
	}
	mCurrentSize--;
}

unsigned int OrderedTable::size()
{
	return mCurrentSize;

}

bool OrderedTable::empty()
{
	return mCurrentSize == 0;
}

OrderedTable::~OrderedTable() 
{ 
	delete[]  pTable;
};

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
