#include <gtest/gtest.h>
#include "table.h"

template <class Table>
class TableTest : public ::testing::Test 
{
protected:
	Table table;
};

using TableTypes = ::testing::Types<LinearArrTable, LinearListTable, OrderedTable, OpenAddressHashTable, SeparateChainingHashTable>; // add TreeTable after realization
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
	this->table.addPolynomial("Pol3", a);
	this->table.addPolynomial("Pol4", a);
	this->table.addPolynomial("Pol5", a);
	this->table.addPolynomial("Pol6", a);
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
	/*Aggregator a;
	EXPECT_TRUE(a.empty());
	EXPECT_EQ(a.size(), 0);*/
	ADD_FAILURE();
}

TEST(TableTest, canSelectCorrectTable) 
{
	ADD_FAILURE();
}

TEST(TableTest, cannotSelectIncorrectTable) 
{
	ADD_FAILURE();
}

TEST(TableTest, canFindExsistingPolynomialUsingAggregator) 
{
	ADD_FAILURE();
}

TEST(TableTest, cannotFindUnexsistingPolynomialUsingAggregator) 
{
	ADD_FAILURE();
}

TEST(TableTest, canAddUnpresentPolynomialUsingAggregator) 
{
	ADD_FAILURE();
}

TEST(TableTest, cannotAddPresentPolynomialUsingAggregator) 
{
	ADD_FAILURE();
}

TEST(TableTest, canDelPresentPolynomialUsingAggregator) 
{
	ADD_FAILURE();
}

TEST(TableTest, delOfUnpresentPolynomialDoesNothingUsingAggregator) 
{
	ADD_FAILURE();
}

TEST(TableTest, canGetSizeWhenEmptyUsingAggregator) 
{
	ADD_FAILURE();
}

TEST(TableTest, canGetSizeWhenNonEmptyUsingAggregator) 
{
	ADD_FAILURE();
}
