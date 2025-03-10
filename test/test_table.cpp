#include <gtest/gtest.h>

#include "table.h"

template <class Table>
class TableTest : public ::testing::TEST {
protected:
	Table table;
};

using TableTypes = ::testing::Types<LinearArrTable, LinearListTable, OrderedTable, TreeTable, OpenAdressHashTable, SeparateChainingHashTable>;
TYPED_TEST_SUITE(TableTest, TableTypes);

TYPED_TEST(TableTest, defaultConstructor) {
	ADD_FAILURE();
}



