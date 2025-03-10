#include <gtest/gtest.h>
#include "table.h"

template <class Table>
class TableTest : public ::testing::Test {
protected:
	Table table;
};

using TableTypes = ::testing::Types<LinearArrTable/*, LinearListTable, OrderedTable, TreeTable, OpenAdressHashTable, SeparateChainingHashTable*/>;
TYPED_TEST_SUITE(TableTest, TableTypes);


TYPED_TEST(TableTest, defaultTableConstructor) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, canFindExsistingPolynomial) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, cannotFindUnexsistingPolynomial) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, canAddUnpresentPolynomial) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, cannotAddPresentPolynomial) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, canDelPresentPolynomial) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, delOfUnpresentPolynomialDoesNothing) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, canGetSizeWhenEmpty) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, canGetSizeWhenNonEmpty) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, emptyTableIsEmpty) {
	ADD_FAILURE();
}

TYPED_TEST(TableTest, nonEmptyTableIsNonEmpty) {
	ADD_FAILURE();
}

TEST(TableTest, defaultAggregatorConstructor) {
	ADD_FAILURE();
}

TEST(TableTest, canSelectCorrectTable) {
	ADD_FAILURE();
}

TEST(TableTest, cannotSelectIncorrectTable) {
	ADD_FAILURE();
}

TEST(TableTest, canFindExsistingPolynomialUsingAggregator) {
	ADD_FAILURE();
}

TEST(TableTest, cannotFindUnexsistingPolynomialUsingAggregator) {
	ADD_FAILURE();
}

TEST(TableTest, canAddUnpresentPolynomialUsingAggregator) {
	ADD_FAILURE();
}

TEST(TableTest, cannotAddPresentPolynomialUsingAggregator) {
	ADD_FAILURE();
}

TEST(TableTest, canDelPresentPolynomialUsingAggregator) {
	ADD_FAILURE();
}

TEST(TableTest, delOfUnpresentPolynomialDoesNothingUsingAggregator) {
	ADD_FAILURE();
}

TEST(TableTest, canGetSizeWhenEmptyUsingAggregator) {
	ADD_FAILURE();
}

TEST(TableTest, canGetSizeWhenNonEmptyUsingAggregator) {
	ADD_FAILURE();
}
