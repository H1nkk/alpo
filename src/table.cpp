#include <table.h>

#define DEFAULT_ORDERED_TABLE_SIZE 4

// *** LinearArrTable ***

LinearArrTable::LinearArrTable() {}

void LinearArrTable::addPolynomial(const std::string& polName, const polynomial& pol)
{
	if (findPolynomial(polName) == std::nullopt)
		mTable.push_back({ polName, pol });
	else
		throw "There already is a polynomial with that name";
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
			mTable.erase(std::next(mTable.begin(), i));
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
	if (findPolynomial(polName) != std::nullopt) // uniqueness check
		throw "There already is a polynomial with that name"; 
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

void LinearListTable::delPolynomial(const std::string& polName)
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
	return mTableSize == 0;
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

OrderedTable::OrderedTable()
{
}

std::optional<polynomial> OrderedTable::findPolynomial(const std::string& polName)
{
	if (mTable.size() == 0) return std::nullopt;
	int i = mTable.size() / 2;
	int leftBorder = 0;
	int rightBorder = mTable.size()-1;

	while (rightBorder >= leftBorder && mTable[i].key != polName) // binary search
	{
		if (mTable[i].key > polName == 1)
		{
			rightBorder = i - 1;
			i = (leftBorder + rightBorder) / 2;
		}
		else if (mTable[i].key < polName == 1)
		{
			leftBorder = i + 1;
			i = (leftBorder + rightBorder) / 2;
		}
	}
	if (mTable[i].key == polName)
		return mTable[i].value;
	return std::nullopt;
}

void OrderedTable::addPolynomial(const std::string& polName, const polynomial& pol)
{
	if (findPolynomial(polName) != std::nullopt) // uniqueness check
		throw "There already is a polynomial with that name";
	int i = mTable.size() / 2;
	int leftBorder = 0;
	int rightBorder = mTable.size() - 1;
	while (rightBorder >= leftBorder) // binary search
	{
		if (mTable[i].key > polName)
		{
			rightBorder = i - 1;
			i = (leftBorder + rightBorder) / 2;
		}
		else if (mTable[i].key < polName)
		{
			leftBorder = i + 1;
			i = (leftBorder + rightBorder) / 2;
		}
	}
	if (mTable.size() == 0)
		mTable.push_back({ polName, pol });
	else if (mTable[i].key > polName)
		mTable.insert(mTable.begin() + i, { polName, pol });
	else if (mTable[i].key < polName)
		mTable.insert(mTable.begin() + i + 1, { polName, pol });
}

void OrderedTable::delPolynomial(const std::string& polName)
{
	if (findPolynomial(polName) == std::nullopt) return; // uniqueness check
	int i = mTable.size() / 2;
	int leftBorder = 0;
	int rightBorder = mTable.size() - 1;
	while (rightBorder >= leftBorder && mTable[i].key != polName)
	{
		if (mTable[i].key > polName)
		{
			rightBorder = i - 1;
			i = (leftBorder + rightBorder) / 2;
		}
		else if (mTable[i].key < polName)
		{
			leftBorder = i + 1;
			i = (leftBorder + rightBorder) / 2;
		}
	}
	mTable.erase(mTable.begin() +  i);
}

unsigned int OrderedTable::size()
{
	return mTable.size();
}

bool OrderedTable::empty()
{
	return mTable.size() == 0;
}

std::vector<std::pair< std::string, polynomial>> OrderedTable::getPolynomials()
{
	std::vector<std::pair< std::string, polynomial>> result(mTable.size());
	for (int i = 0; i < mTable.size(); i++) {
		result[i].first = mTable[i].key;
		result[i].second = mTable[i].value;
	}
	return result;
}

// *** TreeTable ***

TreeTable::TreeTable() {

}

void TreeTable::addPolynomial(const std::string& polName, const polynomial& pol) 
{
	if (tree.find(polName) != std::nullopt)
	{
		throw std::runtime_error("Polynomial already exist");
	}

	tree.insert(polName, pol);
}

std::optional<polynomial> TreeTable::findPolynomial(const std::string& polName)
{
	return tree.find(polName);
}

void TreeTable::delPolynomial(const std::string& polName)
{
	tree.erase(polName);
}

unsigned int TreeTable::size()
{
	return tree.size();

}

bool TreeTable::empty()
{
	return tree.empty();

}

std::vector<std::pair< std::string, polynomial>> TreeTable::getPolynomials()
{
	return tree.toVector();
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
	if (findPolynomial(polName) != std::nullopt) 
		throw "There already is a polynomial with that name";
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
		ind %= mTableSize;
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
		if (mTable[ind].key == polName && mTable[ind].status != -1) // hotfixed by Z
			return mTable[ind].value;
		ind += step;
		ind %= mTableSize;
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
		ind %= mTableSize;
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
	if (findPolynomial(polName) != std::nullopt)
		throw "There already is a polynomial with that name";
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
	int ind = hashFunc(polName); 
	if (!mTable[ind]) return;
	if (mTable[ind]->key == polName)
	{
		Node* tmp = mTable[ind]->pNextInChain;
		delete mTable[ind];
		mTable[ind] = tmp;
		mCurrentSize--;
		return;
	}
	Node* p = mTable[ind];
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
	else {
		throw "Cannot select table " + tableName;
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

bool Aggregator::empty() {
	return size() == 0;
}

std::vector<std::pair< std::string, polynomial>> Aggregator::getPolynomials() {
	return tables[currentTable]->getPolynomials();
}

Aggregator::~Aggregator() {
	for (auto table : tables) {
		delete table;
	}
}