#include <table.h>

#define DEFAUL_ORDERED_TABLE_SIZE 4

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
	return std::nullopt;
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

std::vector<std::pair< std::string, polynomial>> LinearArrTable::getPolynomials() 
{
	std::vector<std::pair< std::string, polynomial>> result(mTable.size());
	for (int i = 0; i < mTable.size(); i++) {
		result[i].first = mTable[i].key;
		result[i].second = mTable[i].value;
	}
	return result;
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
	return std::nullopt;
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

std::vector<std::pair< std::string, polynomial>> LinearListTable::getPolynomials() 
{
	std::vector<std::pair< std::string, polynomial>> result(mTableSize);

	Node* p = pFirst;
	int i = 0;
	while (p) {
		result[i++] = { p->key, p->value };
		p = p->pNext;
	}
	return result;
}

// *** OrderedTable ***

OrderedTable::OrderedTable(): mTableSize(DEFAUL_ORDERED_TABLE_SIZE), mCurrentSize(0) // тут сделал 100, потому что с 4 не работает - кака€-то ошибка с чтением вне доступа. Ћ®Ќя ѕќ‘» —» „“ќЅџ –јЅќ“јЋќ јјјјјјјјјјј (ошибка происходит, когда добавл€ешь п€тую запись в таблицу, у которой размер 4)
{
	pTable = new Pol[DEFAUL_ORDERED_TABLE_SIZE];
}

std::optional<polynomial> OrderedTable::findPolynomial(const std::string& polName)
{
	if (mCurrentSize == 0) return std::nullopt;
	int i = mCurrentSize / 2;
	int leftBorder = 0;
	int rightBorder = mCurrentSize - 1;
	bool flag = false;
	while (rightBorder > leftBorder && flag == false) // binary search
	{
		if (pTable[i].key.compare(polName) > 0)
		{
			rightBorder = i - 1;
			i = (leftBorder + rightBorder) / 2;
		}
		else if (pTable[i].key.compare(polName) < 0)
		{
			leftBorder = i + 1;
			i = (leftBorder + rightBorder) / 2;
		}
		else if (pTable[i].key == polName)
		{
			flag == true;
		}
	}
	if (pTable[i].key == polName)
		return pTable[i].value;
	return std::nullopt;
}

void OrderedTable::addPolynomial(const std::string& polName, const polynomial& pol)
{
	if (findPolynomial(polName) != std::nullopt) return; // uniqueness check
	int i = mCurrentSize / 2;
	int leftBorder = 0;
	int rightBorder = mCurrentSize - 1;
	while (rightBorder > leftBorder && pTable[i].key != polName) // binary search
	{
		if (pTable[i].key.compare(polName) > 0)
		{
			rightBorder = i - 1;
			i = (leftBorder + rightBorder) / 2;
		}
		else if (pTable[i].key.compare(polName) < 0)
		{
			leftBorder = i + 1;
			i = (leftBorder + rightBorder) / 2;
		}
	}
	if (mTableSize == mCurrentSize) // repacking and adding entry
	{
		Pol* pHelpTable = new Pol[mTableSize * 2];
		mTableSize *= 2;
		if (pTable[i].key.compare(polName) > 0)
		{
			i += 1;
		}
		for (int j = 0; j < i + 1; j++)
			pHelpTable[j] = pTable[j];	
		for (int j = i + 1; j < mCurrentSize; j++)
			pHelpTable[j+1] = pTable[j];
		pHelpTable[i + 1] = { polName,  pol };
		delete[]  pTable;
		pTable = pHelpTable;
	}
	else // adding entry
	{
		if (mCurrentSize != 0)
		{
			if (pTable[i].key.compare(polName) > 0)
			{
				i += 1;
			}
			for (int j = i + 1; j < mCurrentSize; j++)
				pTable[j + 1] = pTable[j];
			pTable[i + 1] = { polName,  pol };
		}
		else
		{
			pTable[i] = { polName,  pol };
		}
	}
	mCurrentSize++;
}

void OrderedTable::delPolynomial(const std::string& polName)
{
	if (findPolynomial(polName) == std::nullopt) return; // uniqueness check тут изменил != на ==
	int i = mCurrentSize / 2;
	int leftBorder = 0;
	int rightBorder = mTableSize;
	while (rightBorder > leftBorder && pTable[i].key != polName)
	{
		if (pTable[i].key.compare(polName) > 0)
		{
			rightBorder = i;
			i -= 1 + (rightBorder - leftBorder) / 2;
		}
		else if (pTable[i].key.compare(polName) < 0)
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

std::vector<std::pair< std::string, polynomial>> OrderedTable::getPolynomials()
{
	std::vector<std::pair< std::string, polynomial>> result(mCurrentSize);
	for (int i = 0; i < mCurrentSize; i++) {
		result[i].first = pTable[i].key;
		result[i].second = pTable[i].value;
	}
	return result;
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

std::vector<std::pair< std::string, polynomial>> TreeTable::getPolynomials()
{
	return {};
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
	while (mTable[ind].status != 0 && mTable[ind].status != -1) 
	{ 
		ind += step;
		ind %= mTableSize; // этого не было раньше
	}
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
	return std::nullopt;

}

void OpenAddressHashTable::delPolynomial(const std::string& polName)
{
	int ind = hashFunc(polName);
	while (mTable[ind].status != 0)
	{
		if (mTable[ind].status == 1 && mTable[ind].key == polName)
		{
			mTable[ind].status = -1;
			mCurrentSize--;
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

std::vector<std::pair< std::string, polynomial>> OpenAddressHashTable::getPolynomials()
{
	std::vector<std::pair< std::string, polynomial>> result(mCurrentSize);
	int j = 0;
	for (int i = 0; i < mTableSize; i++) {
		if (mTable[i].status == 1)
		{
			result[j].first = mTable[i].key;
			result[j].second = mTable[i].value;
			j++;
		}
	}
	return result;
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
	return std::nullopt;
}

void SeparateChainingHashTable::delPolynomial(const std::string& polName)
{
	Node* p = mTable[hashFunc(polName)];
	if (!p) return;
	if (p->key == polName)
	{
		Node* tmp = p->pNextInChain;
		delete p;
		mTable[hashFunc(polName)] = tmp; // заменил p на mTable[hashFunc(polName)], Ќќ Ё“ќ Ќ≈ѕ–ј¬»Ћ№Ќќ, я Ё“ќ —ƒ≈ЋјЋ „“ќЅџ Ќ≈  –јЎ»Ћќ—№, Ћ®Ќя ѕќ„»Ќ»»»»»»»»»»» (если оставить p, то мен€тс€ только локальна€ перменна€ p, а не поле класса)
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

std::vector<std::pair< std::string, polynomial>> SeparateChainingHashTable::getPolynomials()
{
	std::vector<std::pair< std::string, polynomial>> result(mCurrentSize);
	int j = 0;
	for (int i = 0; i < mTableSize; i++)
	{
		Node* p = mTable[i];
		while (p != nullptr)
		{
			result[j].first = p->key;
			result[j].second = p->value;
			p = p->pNextInChain;
			j++;
		}
	}
	return result;
}

// *** Aggregator ***

Aggregator::Aggregator() { // TODO јјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјјј ¬≈–Ќ”“№ Ќјƒќ 
	tables.resize(6, nullptr);
	tables[0] = new LinearArrTable();
	tables[1] = new LinearArrTable();
	tables[2] = new LinearArrTable();
	tables[3] = new LinearArrTable();
	tables[4] = new LinearArrTable();
	tables[5] = new LinearArrTable();
	/*
	tables[0] = new LinearArrTable();
	tables[1] = new LinearListTable();
	tables[2] = new OrderedTable();
	tables[3] = new TreeTable();
	tables[4] = new OpenAddressHashTable();
	tables[5] = new SeparateChainingHashTable();
	*/
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
	for (int i = 0; i < 6; i++)
		tables[i]->addPolynomial(polName, pol);
}

void Aggregator::delPolynomial(const std::string& polName) {
	for (int i = 0; i < 6; i++)
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

std::vector<std::pair< std::string, polynomial>> Aggregator::getPolynomials() {
	return tables[currentTable]->getPolynomials();
}