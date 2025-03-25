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
	Node* p = pFirst;
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
	Node* p = pFirst;
	if (!p) 
	{
		pFirst = new Node{ polName, pol, nullptr };
		return;
	}
	while (p->pNext) 
		p = p->pNext;

	p->pNext = new Node{ polName, pol, nullptr };
}

void LinearListTable::delPolynomial(const std::string& polName) // needs a lot of testing
{
	Node* p = pFirst;
	if (!pFirst) return;
	if (pFirst->key == polName) 
	{
		Node* tmp = pFirst->pNext;
		delete pFirst;
		pFirst = tmp;
		mTableSize--;
		return;
	}
	while (p->pNext) 
	{
		if (p->pNext->key == polName) {
			Node* tmp = p->pNext->pNext;
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
	Node* p = pFirst;
	while (p) {
		Node* tmp = p;
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

OpenAddressHashTable::OpenAddressHashTable(): mTableSize(28), mCurrentSize(0), step(3) // mTableSize and step are mutually prime numbers
{
	mTable.resize(mTableSize, { }); // int status initialized with zero
}

unsigned int OpenAddressHashTable::hashFunc(const std::string& key) //polynomial hash function
{
	long long h = 0, pow = 1;
	long long p = 31, m = 1e9 + 7;
	for (char c : key)
	{
		h = (h + pow * (int)(c - 'a' + 1)) % m;
		pow = (pow * p) % m;
	}

	return h % mTableSize;
}

void OpenAddressHashTable::addPolynomial(const std::string& polName, const polynomial& pol)
{
	if (findPolynomial(polName) != std::nullopt) return;
	mCurrentSize++;

	if (mCurrentSize / mTableSize > 0.5) // repacking
	{
		mTableSize *= 2;
		std::vector<Node> helpTable;
		helpTable.resize(mTableSize, { });
		for (int i = 0; i < mTableSize / 2; i++)
		{
			if (mTable[i].status == 1)
			{
				int ind = hashFunc(mTable[i].key);
				while (helpTable[ind].status != 0)
					ind += step;
				helpTable[ind] = mTable[i]; // hashFunc uses mTableSize
			}
		}
		mTable = helpTable;
	}

	int ind = hashFunc(polName);
	while (mTable[ind].status != 0 || mTable[ind].status != -1)
		ind += step;
	mTable[ind].key = polName;
	mTable[ind].value = pol;
	mTable[ind].status = 1;
}

std::optional<polynomial> OpenAddressHashTable::findPolynomial(const std::string& polName)
{
	int ind = hashFunc(polName);
	while (mTable[ind].status != 0)
	{
		if (mTable[ind].key == polName)
			return mTable[ind].value;
		ind += step;
	}
	return {};

}

void OpenAddressHashTable::delPolynomial(const std::string& polName)
{
	int ind = hashFunc(polName);
	while (mTable[ind].status != 0)
	{
		if (mTable[ind].status == 1 && mTable[ind].key == polName)
		{
			mTable[ind].status = -1;
			break;
		}
		ind += step;
	}
}

unsigned int OpenAddressHashTable::size()
{
	return mCurrentSize;

}

bool OpenAddressHashTable::empty()
{
	return mCurrentSize == 0;

}

// *** SeparateChainingHashTable ***

SeparateChainingHashTable::SeparateChainingHashTable(): mTableSize(15), mCurrentSize(0)
{
	mTable.resize(mTableSize, nullptr);
}

unsigned int SeparateChainingHashTable::hashFunc(const std::string& key) //polynomial hash function
{
	long long h = 0, pow = 1;
	long long p = 31, m = 1e9 + 7;
	for (char c : key)
	{
		h = (h + pow * (int)(c - 'a' + 1)) % m;
		pow = (pow * p) % m;
	}

	return h % mTableSize;
}

void SeparateChainingHashTable::addPolynomial(const std::string& polName, const polynomial& pol)
{
	if (findPolynomial(polName) != std::nullopt) return;
	mCurrentSize++;
	if (!mTable[hashFunc(polName)])
	{
		mTable[hashFunc(polName)] = new Node{ polName, pol, nullptr };
		return;
	}
	Node* p = mTable[hashFunc(polName)];
	while (p->pNextInChain)
		p = p->pNextInChain;
	p->pNextInChain = new Node{ polName, pol, nullptr };
}

std::optional<polynomial> SeparateChainingHashTable::findPolynomial(const std::string& polName)
{
	Node* p = mTable[hashFunc(polName)];
	while (p)
	{
		if (p->key == polName)
		{
			return p->value;
		}
		p = p->pNextInChain;
	}
	return {};
}

void SeparateChainingHashTable::delPolynomial(const std::string& polName)
{
	Node* p = mTable[hashFunc(polName)];
	if (!p) return;
	if (p->key == polName)
	{
		Node* tmp = p->pNextInChain;
		delete p;
		p = tmp;
		mCurrentSize--;
		return;
	}
	while (p->pNextInChain)
	{
		if (p->pNextInChain->key == polName) {
			Node* tmp = p->pNextInChain->pNextInChain;
			delete p->pNextInChain;
			p->pNextInChain = tmp;
			mCurrentSize--;
			return;
		}
		p = p->pNextInChain;
	}
}

unsigned int SeparateChainingHashTable::size()
{
	return mCurrentSize;
}

bool SeparateChainingHashTable::empty()
{
	return mCurrentSize == 0;

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
	for (int i = 0; i < 5; i++)
		tables[i]->addPolynomial(polName, pol);
}

void Aggregator::delPolynomial(const std::string& polName) {
	for (int i = 0; i < 5; i++)
		tables[i]->delPolynomial(polName);
}

unsigned int Aggregator::size() {
	return tables[currentTable]->size();
}

Aggregator::~Aggregator() {
	for (auto table : tables) {
		delete table; // i dont know if this calls tables' destructors
	}
}
