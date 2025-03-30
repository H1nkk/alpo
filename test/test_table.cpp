#include <gtest/gtest.h>
#include "table.h"

template <class Table>
class TableTest : public ::testing::Test 
{
protected:
	Table table;
};

using TableTypes = ::testing::Types<OrderedTable, LinearArrTable, LinearListTable, OrderedTable, OpenAddressHashTable, SeparateChainingHashTable>; // add TreeTable after realization
TYPED_TEST_SUITE(TableTest, TableTypes);


TYPED_TEST(TableTest, defaultTableConstructor) 
{
	EXPECT_TRUE(this->table.empty());
	EXPECT_EQ(this->table.size(), 0);
}

TYPED_TEST(TableTest, canAddUnpresentPolynomial) 
{
	polynomial a;
	std::vector<std::pair< std::string, polynomial>> pol(6);
	a.from_string("12x2-4y");
	this->table.addPolynomial("Pol1", a);
	this->table.addPolynomial("Pol2", a);
	this->table.addPolynomial("Pol4", a);
	this->table.addPolynomial("Pol6", a);
	this->table.addPolynomial("Pol5", a);
	this->table.addPolynomial("Pol3", a);
	pol[0].first = "Pol1";
	pol[0].second = a;
	pol[1].first = "Pol2";
	pol[1].second = a;
	pol[2].first = "Pol3";
	pol[2].second = a;
	pol[3].first = "Pol4";
	pol[3].second = a;
	pol[4].first = "Pol5";
	pol[4].second = a;
	pol[5].first = "Pol6";
	pol[5].second = a;
	EXPECT_FALSE(this->table.empty());
	EXPECT_EQ(this->table.size(), 6);
	//EXPECT_EQ(this->table.getPolynomials(), pol);
}

TYPED_TEST(TableTest, cannotAddPresentPolynomial) 
{
	polynomial a;
	a.from_string("12x2-4y");
	this->table.addPolynomial("Pol1", a);
	this->table.addPolynomial("Pol1", a);
	EXPECT_FALSE(this->table.empty());
	EXPECT_EQ(this->table.size(), 1);
}

TYPED_TEST(TableTest, canFindExsistingPolynomial) 
{
	polynomial a;
	a.from_string("12x2-4y");
	this->table.addPolynomial("Pol1", a);
	EXPECT_EQ(this->table.findPolynomial("Pol1"), a);
}

TYPED_TEST(TableTest, cannotFindUnexsistingPolynomial) 
{
	EXPECT_EQ(this->table.findPolynomial("Pol1"), std::nullopt);
}

TYPED_TEST(TableTest, canDelPresentPolynomial) 
{
	polynomial a;
	a.from_string("12x2-4y");
	this->table.addPolynomial("Pol1", a);
	this->table.delPolynomial("Pol1");
	EXPECT_TRUE(this->table.empty());
	EXPECT_EQ(this->table.size(), 0);
}

TYPED_TEST(TableTest, delOfUnpresentPolynomialDoesNothing) 
{
	polynomial a, b;
	a.from_string("12x2-4y");
	b.from_string("12x3-6z");
	this->table.addPolynomial("Pol1", a);
	this->table.addPolynomial("Pol2", b);
	this->table.delPolynomial("Pol3");
	EXPECT_FALSE(this->table.empty());
	EXPECT_EQ(this->table.size(), 2);
}

TYPED_TEST(TableTest, canGetSizeWhenEmpty) 
{
	EXPECT_EQ(this->table.size(), 0);
}

TYPED_TEST(TableTest, canGetSizeWhenNonEmpty) 
{
	polynomial a;
	a.from_string("12x2-4y");
	this->table.addPolynomial("Pol1", a);
	EXPECT_EQ(this->table.size(), 1);
}

TYPED_TEST(TableTest, emptyTableIsEmpty) 
{
	EXPECT_TRUE(this->table.empty());
}

TYPED_TEST(TableTest, nonEmptyTableIsNonEmpty) 
{
	polynomial a;
	a.from_string("12x2-4y");
	this->table.addPolynomial("Pol1", a);
	EXPECT_FALSE(this->table.empty());
}


TEST(Aggregator, defaultAggregatorConstructor)
{
	Aggregator a;
	EXPECT_TRUE(a.empty());
	EXPECT_EQ(a.size(), 0);
}

TEST(Aggregator, canSelectCorrectTable)
{
	Aggregator a;
	EXPECT_NO_THROW(a.selectTable("liar"));
	EXPECT_NO_THROW(a.selectTable("lili"));
	EXPECT_NO_THROW(a.selectTable("ordr"));
	EXPECT_NO_THROW(a.selectTable("tree"));
	EXPECT_NO_THROW(a.selectTable("opha"));
	EXPECT_NO_THROW(a.selectTable("seha"));
}

TEST(Aggregator, cannotSelectIncorrectTable)
{
	Aggregator a;
	EXPECT_ANY_THROW(a.selectTable("fooo"));
}

TEST(Aggregator, canFindExsistingPolynomialUsingAggregator)
{
	Aggregator* pAggregator = new Aggregator();
	std::vector<polynomial> pols;
	std::vector<std::string> tableNames = { "liar" , "lili", "ordr", "tree", "opha", "seha" };
	int polyCount = 10;
	for (int i = 1; i < polyCount + 1; i++) {
		polynomial a;
		std::string st;
		switch (i % 4)
		{
		case 0:
			st += "w";
			break;
		case 1:
			st += "x";
			break;
		case 2:
			st += "y";
			break;
		case 3:
			st += "z";
			break;
		}
		st += std::to_string(i);
		a = std::get<polynomial>(polynomial::from_string(st));
		pols.push_back(a);
	}
	for (int i = 0; i < polyCount; i++) {
		pAggregator->addPolynomial(std::to_string(i), pols[i]);
	}

	for (int i = 0; i < polyCount; i++) {
		pAggregator->selectTable(tableNames[i % 6]);
		if (i % 6 == 3) { // TODO потом убрать, когда будет реализация treeTable aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
			continue;
		}
		std::optional<polynomial> foo;
		if (i == 8) {
			foo = pAggregator->findPolynomial(std::to_string(i)); // БАГ ВОТ ТУТ
			pAggregator->selectTable(tableNames[0]);
			foo = pAggregator->findPolynomial(std::to_string(i));
		}
		EXPECT_EQ((pAggregator->findPolynomial(std::to_string(i))).value(), pols[i]);
	}
}


TEST(Aggregator, cannotFindUnexsistingPolynomialUsingAggregator)
{
	ADD_FAILURE();
}

TEST(Aggregator, canAddUnpresentPolynomialUsingAggregator)
{
	ADD_FAILURE();
}

TEST(Aggregator, cannotAddPresentPolynomialUsingAggregator)
{
	ADD_FAILURE();
}

TEST(Aggregator, canDelPresentPolynomialUsingAggregator)
{
	ADD_FAILURE();
}

TEST(Aggregator, delOfUnpresentPolynomialDoesNothingUsingAggregator)
{
	ADD_FAILURE();
}

TEST(Aggregator, canGetSizeWhenEmptyUsingAggregator)
{
	ADD_FAILURE();
}

TEST(Aggregator, canGetSizeWhenNonEmptyUsingAggregator)
{
	ADD_FAILURE();
}
