
#include <gtest/gtest.h>

#include <iostream>
#include <map>
#include <string>

#include "map.hpp"
#include "tree/tree.hpp"
#include "tree/rb_balance.hpp"

// namespace ft

using std::string;

TEST(map_constructor, default_constructor)
{
	std::map<string, int> msi_1;
	ft::map<string, int> msi_2;
	EXPECT_EQ(msi_1.size(), msi_2.size());
}

TEST(map_constructor, range_constructor)
{
	std::map<string, int> msi_1;
	std::map<string, int> msi_2(msi_1.begin(), msi_1.end());
	EXPECT_EQ(msi_1.size(), msi_2.size());
}

TEST(map_constructor, copy_constructor)
{
	std::map<string, int> msi_1;
	std::map<string, int> msi_2(msi_1);
	EXPECT_EQ(msi_1.size(), msi_2.size());
}

TEST(map_oper_assign, copy_assign)
{
	std::map<string, int> msi_1;
	std::map<string, int> msi_2;
	msi_2 = msi_1;
	EXPECT_EQ(msi_1.size(), msi_2.size());
}

TEST(map_iterator, iterator)
{
	std::map<string, int> msi_1;
	msi_1["a"] = 10;
	msi_1["b"] = 20;
	msi_1["c"] = 30;
	msi_1["d"] = 40;
	std::map<string, int>::iterator first(msi_1.begin());
	std::map<string, int>::iterator last(msi_1.end());
	EXPECT_EQ(msi_1.begin(), first);
	EXPECT_EQ(msi_1.end(), last);
}

TEST(mapIterator, reverse_iterator)
{
	std::map<string, int> msi_1;
	msi_1["a"] = 10;
	msi_1["b"] = 20;
	msi_1["c"] = 30;
	msi_1["d"] = 40;
	std::map<string, int>::reverse_iterator rfirst(msi_1.rbegin());
	std::map<string, int>::reverse_iterator rlast(msi_1.rend());
	EXPECT_EQ(msi_1.rbegin(), rfirst);
	EXPECT_EQ(msi_1.rend(), rlast);
}

TEST(mapIterator, convertible_to_const_iterator)
{
	std::map<string, int> msi_1;
	std::map<string, int>::iterator it(msi_1.begin());
	std::map<string, int>::const_iterator cit(it);
}

TEST(mapCapacity, capacity)
{
	std::map<string, int> msi_1;
	msi_1.empty();
	msi_1.size();
	msi_1.max_size();
}

TEST(mapElementAceess, element_access)
{
	std::map<string, int> msi_1;
	msi_1["answer"] = 42;
	EXPECT_EQ(msi_1["answer"], 42);
}

TEST(mapModifiers, insert)
{
	std::map<string, int> msi_1;
	std::map<string, int> msi_2;

	msi_1.insert(std::pair<string, int>("zero", 0));  // single element(1)
	msi_1.insert(std::pair<string, int>("answer", 42));

	std::pair<std::map<string, int>::iterator, bool> ret;
	ret = msi_1.insert(std::pair<string, int>("answer", 42));  // with hint(2)
	EXPECT_EQ(ret.second, false);

	msi_2.insert(msi_1.begin(), msi_1.end());  // range(3)
}

TEST(mapModifiers, erase)
{
	std::map<char, int> mci_1;
	std::map<char, int>::iterator it;
	mci_1['a'] = 10;
	mci_1['b'] = 20;
	mci_1['c'] = 30;
	mci_1['d'] = 40;
	it = mci_1.begin();
	mci_1.erase(it);   // erasing by iterator
	mci_1.erase('c');  // erasing by key
	it = mci_1.find('d');
	mci_1.erase(it, mci_1.end());  // erasing by range
}

TEST(mapModifiers, swap)
{
	std::map<char, int> mci_1;
	std::map<char, int> mci_2;
	mci_1['x'] = 100;
	mci_1['y'] = 200;
	mci_2['a'] = 100;
	mci_2['b'] = 200;
	mci_2['c'] = 300;
	std::map<char, int> mci_1__copied(mci_1);
	mci_1.swap(mci_2);
	EXPECT_EQ(mci_1, mci_1__copied);
}

TEST(mapModifiers, clear)
{
	std::map<char, int> mci_1;
	std::map<char, int> mci_2;
	mci_1['x'] = 100;
	mci_1['y'] = 200;
	mci_1.clear();
	mci_1['x'] = 1100;
	mci_1['y'] = 2200;
	EXPECT_EQ(mci_1['x'], 1100);
	EXPECT_EQ(mci_1['y'], 2200);
}

TEST(mapObserver, key_comp)
{
	std::map<char, int> mci_1;
	std::map<char, int>::key_compare mci_cmp = mci_1.key_comp();
	ft::map<char, int> __mci_1;
	ft::map<char, int>::key_compare __mci_cmp = __mci_1.key_comp();
	EXPECT_EQ(mci_cmp('a', 'a'), __mci_cmp('a', 'a'));
	EXPECT_EQ(mci_cmp('a', 'b'), __mci_cmp('a', 'b'));
}

TEST(mapObserver, value_comp)
{
	std::map<char, int> mci_1;
	std::map<char, int>::value_compare mci_cmp = mci_1.value_comp();
	mci_1['a'] = 20;
	mci_1['b'] = 40;
	mci_1['c'] = 60;
	mci_1['d'] = 80;
	mci_1['e'] = 100;
	std::map<char, int>::iterator it = mci_1.begin();
	EXPECT_EQ(mci_cmp(*it, *it++), 1);
	EXPECT_EQ(mci_cmp(*it, *it), 0);
}

TEST(mapOperations, find)
{
	std::map<char, int> mci_1;
	std::map<char, int>::iterator it;
	mci_1['a'] = 50;
	mci_1['b'] = 100;
	mci_1['c'] = 150;
	mci_1['d'] = 200;
	it = mci_1.find('b');
	EXPECT_EQ((*it).second, 100);
}

TEST(mapOperations, count)
{
	std::map<char, int> mci_1;
	char ch;
	mci_1['a'] = 101;
	mci_1['c'] = 202;
	mci_1['f'] = 303;
	EXPECT_EQ(mci_1.count('a'), 1);
	EXPECT_EQ(mci_1.count('e'), 0);
}

TEST(mapOperations, lower_upper_bound)
{
	std::map<char, int> mci_1;
	std::map<char, int>::iterator itlow, itup;
	mci_1['a'] = 20;
	mci_1['b'] = 40;
	mci_1['c'] = 60;
	mci_1['d'] = 80;
	mci_1['e'] = 100;
	itlow = mci_1.lower_bound('b');  // itlow points to b
	EXPECT_EQ(itlow, mci_1.find('b'));
	itup = mci_1.upper_bound('d');  // itup points to e (not d!)
	EXPECT_EQ(--itup, mci_1.find('d'));
}

TEST(mapOperations, equal_range)
{
	std::map<char, int> mci_1;
	mci_1['a'] = 20;
	mci_1['b'] = 40;
	mci_1['c'] = 60;
	mci_1['d'] = 80;
	mci_1['e'] = 100;
	std::pair<std::map<char, int>::iterator, std::map<char, int>::iterator> ret;
	ret = mci_1.equal_range('b');
	EXPECT_EQ(ret.first->first, 'b');
	EXPECT_EQ(ret.second->first, 'c');
}

TEST(mapAllocator, get_allocator)
{
	std::map<string, int> msi_1;
	msi_1.get_allocator();
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
