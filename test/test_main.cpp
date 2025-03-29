#include "googletest/include/gtest/gtest.h"

#include <polynomial.h>
#include <table.h>


//int main()
//{
//	polynomial a, b, c, d;
//	a.from_string("12x2-4y");
//	b.from_string("12x3-4y");
//	c.from_string("12y2-4z");
//	d.from_string("12x3-4z5");
//	OrderedTable t;
//	t.addPolynomial("Pol1", a);
//	t.addPolynomial("Pol2", b);
//	t.addPolynomial("Pol3", c);
//	t.addPolynomial("Pol4", d);
//	t.addPolynomial("Pol5", d);
//	t.addPolynomial("Pol6", d);
//	for (int i; i < t.size(); i++)
//	{
//
//	}
//	return 0;
//}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}