#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "map.hpp"
#include "tree/node.hpp"
#include "tree/rb_balance.hpp"
#include "tree/tree.hpp"
#include "type.hpp"
#include "utility.hpp"

using namespace std;

class exampleClass
{
public:
	std::string a;
	int b;
	std::pair<char, int> pa;

	exampleClass(void) : a("HelloWorld"), b(42){};

	exampleClass(int i) : a("HelloWorld"), b(i){};

	exampleClass(const exampleClass& src) : a(src.a), b(src.b){};

	~exampleClass(void){};

	exampleClass& operator=(exampleClass const& rhs)
	{
		if (this != &rhs)
		{
			this->a = rhs.a;
			this->b = rhs.b;
		}
		return *this;
	}
};

void test_basic_operator_equal(ft::map<int, int>& ft_c0, ft::map<int, int>& ft_c1, std::map<int, int>& std_c0, std::map<int, int>& std_c1)
{
	ft::map<int, int>::iterator ft_it1 = ft_c1.begin();
	// std::map<int, int>::iterator std_it1 = std_c1.begin();

	// ft_c0.print();
	std::cout << "\n";
	// ft_c1.print();
	std::cout << "c0: \n";
	ft_c0.print();
	std::cout << "\n";
	std::cout << "c1: \n";
	ft_c1.print();

	std::cout << __func__ << "[ Operator= on 2 maps ]" << std::endl;
	std::cout << "just before of std assignment\n";
	std_c0 = std_c1;

	std::cout << "just before of ft assignment\n";
	ft_c0 = ft_c1;

	std::cout << "c0: \n";
	ft_c0.print();
	std::cout << "\n";
	std::cout << "c1: \n";
	ft_c1.print();

	std::cout << "[ test if iterator stayed valid after = ]" << std::endl;
	// if (std_c1.empty() == false)
	// 	testBool(*std_it1 == *std_c1.begin(), __FILE__, __LINE__);
	// if (ft_c1.empty() == false)
	// 	testBool(*ft_it1 == *ft_c1.begin(), __FILE__, __LINE__);
}

int main(void)
{
	{
		ft::map<char, int> mp;

		for (int i = 0; i < 24; i++)
		{
			mp.insert(ft::make_pair('a' + i, i));
			std::cout << "size: " << mp.size() << std::endl;
		}
		mp.print();

		mp.erase('h');
		mp.erase('t');
		mp.print();
		mp.erase('w');
		mp.print();
		mp.erase('e');
		mp.print();
		mp.erase('r');
		mp.print();
		mp.erase('a');
		mp.print();
		mp.erase('q');
		mp.erase('c');
		mp.print();
		std::cout << mp.size() << std::endl;
	}
	{
		ft::map<char, int> mp;

		for (int i = 0; i < 24; i++)
		{
			mp.insert(ft::make_pair('a' + i, i));
			std::cout << "size: " << mp.size() << std::endl;
		}
		mp.print();

		mp.erase('r');
		mp.print();
		mp.erase('c');
		mp.print();
		mp.erase('t');
		mp.print();
		mp.erase('w');
		mp.print();
		mp.erase('q');
		mp.print();
		mp.erase('e');
		mp.print();
		mp.erase('h');
		mp.print();
		mp.erase('a');
		mp.print();
		mp.erase('b');
		mp.print();
		mp.erase('x');
		mp.print();
		mp.erase('v');
		mp.print();
		mp.erase('f');
		mp.print();
		mp.erase('i');
		mp.print();
		mp.erase('l');
		mp.print();
		mp.erase('u');
		mp.print();
		mp.erase('d');
		mp.print();
		mp.erase('g');
		mp.print();
		mp.erase('j');
		mp.print();
		mp.erase('l');  // duplicated
		mp.print();
		mp.erase('m');
		mp.print();
		mp.erase('n');
		mp.print();
		mp.erase('o');
		mp.print();
		mp.erase('p');
		mp.print();
		mp.erase('s');
		mp.print();
		mp.erase('k');
	}
	{
		ft::map<char, int> mp;

		for (int i = 0; i < 24; i++)
		{
			mp.insert(ft::make_pair('a' + i, i));
		}

		std::cout << "size: " << mp.size() << std::endl;
		std::cout << (*mp.begin()).first << std::endl;
		// std::cout << (*mp.end()).first << std::endl;
		std::cout << "find: " << mp.count('k') << std::endl;
		std::cout << "find: " << mp.count('z') << std::endl;
		std::cout << "maxsize: " << mp.max_size() << std::endl;
		mp['z'] = 26;
		mp['a'] = 26;
		std::cout << "z: " << mp['z'] << std::endl;
		std::cout << "a: " << mp['a'] << std::endl;
		ft::map<char, int>::iterator mpi = mp.begin();
		mp.print();
		++mpi;
		std::cout << mp.size() << std::endl;
		std::cout << "main check\n";
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a++: " << (*(mpi++)).first << std::endl;
		std::cout << "*a: " << reinterpret_cast<void*>(&mpi) << std::endl;  // end node

		--mpi;  // last node
		// std::cout << "*a:  " << (*(mpi)).first << std::endl;

		std::cout << "*a--: " << (*(mpi--)).first << std::endl;
		std::cout << "*a--: " << (*(mpi--)).first << std::endl;
		std::cout << "*a--: " << (*(mpi--)).first << std::endl;
		std::cout << "*a--: " << (*(mpi--)).first << std::endl;
		std::cout << "*a--: " << (*(mpi--)).first << std::endl;
		std::cout << "*a--: " << (*(mpi--)).first << std::endl;
		std::cout << "*a--: " << (*(mpi--)).first << std::endl;
		std::cout << "*a--: " << (*(mpi--)).first << std::endl;
		// std::cout << "*a--: " << (*(mpi--)).first << std::endl;
		// std::cout << (*(mp.find('y'))).first << std::endl;
		std::cout << (*(mp.find('x'))).first << std::endl;
		std::cout << (*(mp.find('c'))).second << std::endl;
	}
	{
		ft::map<char, int> mp;
		ft::pair<ft::map<char, int>::iterator, bool> pair;

		for (int i = 0; i < 16; i++)
		{
			pair = mp.insert(ft::make_pair('a' + i, i));
		}
		std::cout << "1\n";
		std::cout << "size: " << mp.size() << std::endl;

		mp.print();

		ft::map<char, int>::iterator begin = mp.begin();
		ft::map<char, int>::iterator end = mp.end();
		std::cout << "2\n";
		for (; begin != end; begin++)
		{
			std::cout << (*begin).first << std::endl;
		}
		begin = mp.begin();
		for (int i = 16; i < 25; i++)
		{
			mp.insert(begin, ft::pair<char, int>('a' + i, i));
		}
		std::cout << "size: " << mp.size() << std::endl;

		for (; begin != end; begin++)
		{
			std::cout << (*begin).first << ' ';
		}
		std::cout << "\n";

		begin = mp.begin();
		for (int i = 16; i < 25; i++)
		{
			mp.erase(begin);
			begin = mp.begin();
		}

		for (; begin != end; begin++)
		{
			std::cout << (*begin).first << ' ';
		}

		std::cout << "\n";
		ft::map<char, int> mp2;

		std::cout << "mp1 size: " << mp.size() << std::endl;
		mp.swap(mp2);

		mp.print();
		mp2.print();
		begin = mp2.begin();
		end = mp2.end();
		std::cout << "mp2 swapped\n";
		std::cout << "mp2 size: " << mp2.size() << std::endl;
		for (; begin != end; begin++)
		{
			std::cout << (*begin).first << std::endl;
			// std::cout << reinterpret_cast<void*>(&(begin.base())) << std::endl;
		}
		ft::swap(mp2, mp);
		begin = mp2.begin();
		end = mp2.end();
		std::cout << "mp2 size: " << mp2.size() << std::endl;
		for (; begin != end; begin++)
		{
			std::cout << (*begin).first << std::endl;
		}
		std::cout << "done\n";
	}
	{
		ft::map<char, int> mp;
		for (int i = 0; i < 16; i++)
		{
			mp.insert(ft::make_pair('a' + i, i));
		}
		ft::map<char, int> mp2;
		for (int i = 0; i < 17; i++)
		{
			mp2.insert(ft::make_pair('a' + i, i));
		}

		std::cout << std::boolalpha;
		std::cout << (mp == mp2) << std::endl;
		std::cout << (mp < mp2) << std::endl;
	}
	{
		ft::map<char, int> mp1;
		for (int i = 0; i < 17; i++)
		{
			mp1.insert(ft::make_pair('a' + i, i));
		}
		ft::map<char, int> mp2(mp1.begin(), mp1.end());
		ft::map<char, int> mp3 = mp2;
		ft::map<char, int>::iterator begin = mp3.begin();
		ft::map<char, int>::iterator end = mp3.end();
		std::cout << "check\n";
		mp2['a'] = 100;
		for (; begin != end; begin++)
		{
			std::cout << (*begin).first << ": ";
			std::cout << (*begin).second << std::endl;
		}
		ft::map<char, int> mp4 = mp2;
		// mp4 = mp3;
		mp2['a'] = 200;
		begin = mp4.begin();
		end = mp4.end();
		for (; begin != end; begin++)
		{
			std::cout << (*begin).first << ": ";
			std::cout << (*begin).second << std::endl;
		}
		begin = mp4.begin();
		ft::map<char, int>::reverse_iterator mri(begin);
		// std::cout <<  << std::endl;
	}
	{
		std::cout << "################################################################################\n\n";
		ft::map<char, exampleClass> ft_c0;
		ft_c0['a'] = 10;
		ft_c0['b'] = 20;
		ft_c0['c'] = 30;
		ft_c0['d'] = 40;
		ft_c0['e'] = 50;
		ft_c0['f'] = 60;
		ft_c0['g'];

		ft::map<char, exampleClass>::iterator ft_it = ft_c0.begin();
		ft::map<char, exampleClass>::iterator ft_ite = ft_c0.end();
		std::cout << ft_c0.size() << std::endl;
		// std::cout << (*ft_it).first << std::endl;

		ft_c0.erase(ft_it);
		std::cout << ft_c0.size() << std::endl;
		std::cout << (--ft_ite)->first << std::endl;
		ft_ite++;

		std::cout << "################### round 2 #####################\n\n\n";
		ft_c0.erase('c');
		ft_c0.erase('g');
		std::cout << "################### round 2 #####################\n\n\n";
		// testMap<char, exampleClass>(ft_c0, std_c0, NOPRINT);

		ft_c0.print();
		ft_it = ft_c0.begin();
		std::cout << "################### round 2 #####################\n\n\n";
		ft_c0.erase(ft_it, ft_ite);
		std::cout << "################### round 2 #####################\n\n\n";
	}
	{
		int testSize = 35;
		int findKey = 42;
		std::vector<ft::pair<int, int> > ft_val_0(testSize);
		std::vector<std::pair<int, int> > std_val_0(testSize);
		srand(time(NULL));
		for (int i = 0; i < testSize; i++)
		{
			int val = rand() % testSize;
			if (i == testSize / 2)
				findKey = val;
			ft_val_0[i] = ft::make_pair(val, i);
			std_val_0[i] = std::make_pair(val, i);
		}

		std::map<int, int> std_c0;
		ft::map<int, int> ft_c0;

		std_c0.insert(std_val_0.begin(), std_val_0.end());
		ft_c0.insert(ft_val_0.begin(), ft_val_0.end());

		std::cout << "[ Insert with Hint ]" << std::endl;

		std::map<int, int>::iterator std_it = std_c0.find(findKey);
		ft::map<int, int>::iterator ft_it = ft_c0.find(findKey);

		std_c0.insert(std_it, std::make_pair(testSize * 2, 1));
		ft_c0.insert(ft_it, ft::make_pair(testSize * 2, 1));

		std_c0.insert(std_c0.end(), std::make_pair(-42, 1));
		ft_c0.insert(ft_c0.end(), ft::make_pair(-42, 1));

		std_c0.insert(std_c0.find(-42), std::make_pair(-43, 2));
		ft_c0.insert(ft_c0.find(-42), ft::make_pair(-43, 2));
		std_c0.insert(std_c0.find(-42), std::make_pair(-41, -1));
		ft_c0.insert(ft_c0.find(-42), ft::make_pair(-41, -1));

		std_c0.insert(std_c0.find(testSize * 2), std::make_pair((testSize * 2) + 1, 2));
		ft_c0.insert(ft_c0.find(testSize * 2), ft::make_pair((testSize * 2) + 1, 2));

		std_c0.insert(std_c0.find(testSize * 2), std::make_pair((testSize * 2) - 1, -1));
		ft_c0.insert(ft_c0.find(testSize * 2), ft::make_pair((testSize * 2) - 1, -1));

		ft_c0.print();

		ft::map<int, int>::iterator ft_begin = ft_c0.begin();
		ft::map<int, int>::iterator ft_end = ft_c0.end();
		std::map<int, int>::iterator std_begin = std_c0.begin();
		std::map<int, int>::iterator std_end = std_c0.end();
		for (; ft_begin != ft_end; ft_begin++, std_begin++)
		{
			if (((*ft_begin).first != (*std_begin).first) || (*ft_begin).second != (*std_begin).second)
			{
				std::cout << (*ft_begin).first << " --- " << (*std_begin).first << std::endl;
				// ft_c0.print();
				std::cout << (*ft_begin).second << " --- " << (*std_begin).second << std::endl;
				exit(1);
			}
			// std::cout << ((*ft_begin).first == (*std_begin).first) << std::endl;
			// std::cout << ((*ft_begin).second == (*std_begin).second) << std::endl;
		}
		std::cout << "pass##########################################################################################\n\n";
		// int insertVal;
		// int removeVal;
		// int j;
		// for (int i = 0; i < 3; i++)
		// {
		// 	if (i == 0)
		// 	{
		// 		insertVal = 25;
		// 		removeVal = 24;
		// 	}
		// 	else if (i == 1)
		// 	{
		// 		insertVal = 36;
		// 		removeVal = 32;
		// 	}
		// 	else if (i == 2)
		// 	{
		// 		insertVal = 25;
		// 		removeVal = 5;
		// 	}
		// 	std_c0.insert(std_it, std::make_pair(insertVal - 1, 42));
		// 	ft_c0.insert(ft_it, ft::make_pair(insertVal - 1, 42));
		// 	std_c0.insert(std_it, std::make_pair(insertVal + 1, 21));
		// 	ft_c0.insert(ft_it, ft::make_pair(insertVal + 1, 21));
		// 	std_c0.erase(removeVal);
		// 	ft_c0.erase(removeVal);
		// 	j = 0;
		// 	std::cout << "size: " << ft_c0.size() << std::endl;
		// 	ft_c0.print();

		// 	ft_begin = ft_c0.begin();
		// 	ft_end = ft_c0.end();
		// 	std_begin = std_c0.begin();
		// 	std_end = std_c0.end();
		// 	if (ft_c0.size() != std_c0.size())
		// 	{
		// 		std::cout << "############################### Diffenret Size!! #########################\n";
		// 	}

		// 	for (; ft_begin != ft_end && std_begin != std_end; ft_begin++, std_begin++)
		// 	{
		// 		if (ft_end != ft_c0.end())
		// 		{
		// 			std::cout << "not same, is it possible?!\n";
		// 		}
		// 		if (ft_begin == ft_c0.end())
		// 		{
		// 			std::cout << "finished, but im still running, is it possible?!\n";
		// 		}
		// 		if (ft_begin == ft_end)
		// 		{
		// 			std::cout << "2finished, but im still running, is it possible?!\n";
		// 		}
		// 		std::cout << "J: " << j << " \n";
		// 		if (((*ft_begin).first != (*std_begin).first) || (*ft_begin).second != (*std_begin).second)
		// 		{
		// 			std::cout << "Pos is [ " << j << " ]" << std::endl;
		// 			std::cout << (*ft_begin).first << " --- " << (*std_begin).first << std::endl;
		// 			// ft_c0.print();
		// 			std::cout << (*ft_begin).second << " --- " << (*std_begin).second << std::endl;
		// 			exit(1);
		// 		}
		// 		j++;
		// 	}
		// 	if (ft_begin != ft_end)
		// 	{
		// 		std::cout << "############################### Diffenret iterator!! #########################\n";
		// 	}
		// 	std::cout << "\n\n";
		// }
		for (int i = 0; i < 10; i++)
		{
			int val_insert = rand() % testSize + 10;
			int val_remove = std_val_0[rand() % (testSize / 2)].first;
			std::cout << "[Insert key: " << val_insert << " Remove key: " << val_remove << " ]" << std::endl;

			std_it = std_c0.find(val_insert);
			ft_it = ft_c0.find(val_insert);
			if (std_it == std_c0.end())
				std::cout << "[Hint position is end()]" << std::endl;
			std_c0.insert(std_it, std::make_pair(val_insert + 1, 42));
			ft_c0.insert(ft_it, ft::make_pair(val_insert + 1, 42));
			std_it = std_c0.insert(std_it, std::make_pair(val_insert - 1, 21));
			ft_it = ft_c0.insert(ft_it, ft::make_pair(val_insert - 1, 21));

			std_c0.erase(val_remove);
			ft_c0.erase(val_remove);
			std::cout << std::boolalpha;
			ft_c0.print();
			std::cout << (ft_c0.size() == std_c0.size()) << std::endl;
			int j = 0;
			ft_begin = ft_c0.begin();
			ft_end = ft_c0.end();
			std_begin = std_c0.begin();
			std_end = std_c0.end();
			for (; ft_begin != ft_end; ft_begin++, std_begin++)
			{
				// if (((*ft_begin).first != (*std_begin).first) || (*ft_begin).second != (*std_begin).second)
				if (((*ft_begin).first != (*std_begin).first) || (*ft_begin).second != (*std_begin).second)
				{
					std::cout << "Pos is [ " << j << " ]" << std::endl;
					std::cout << (*ft_begin).first << " --- " << (*std_begin).first << std::endl;
					// ft_c0.print();
					std::cout << (*ft_begin).second << " --- " << (*std_begin).second << std::endl;
					exit(1);
				}
				std::cout << "J: " << j << std::endl;
				j++;
			}
			std::cout << "i: " << i << std::endl;
		}
		std::cout << "[ Remove key 10000000 which does not exist ]" << std::endl;
		std_c0.erase(10000000);
		ft_c0.erase(10000000);
	}
	{
		int test_size = 35;

		std::vector<ft::pair<int, exampleClass> > ft_val_0(test_size);
		std::vector<std::pair<int, exampleClass> > std_val_0(test_size);
		srand(0);
		for (int i = 0; i < test_size; i++)
		{
			int val = rand() % test_size;
			ft_val_0[i] = ft::make_pair(val, i);
			std_val_0[i] = std::make_pair(val, i);
			std::cout << "inserted value: " << val << " ";
		}
		std::cout << "\n";

		std::map<int, exampleClass> std_c0(std_val_0.begin(), std_val_0.end());
		std::map<int, exampleClass>::iterator std_ci0;
		ft::map<int, exampleClass> ft_c0(ft_val_0.begin(), ft_val_0.end());
		ft::map<int, exampleClass>::iterator ft_ci0;

		ft_c0.print();

		ft_c0.clear();
		std_c0.clear();

		std::cout << ft_c0.size() << std::endl;
		std::cout << std_c0.size() << std::endl;
		ft_c0.print();

		ft_c0[42] = 21;
		ft_c0.print();
		ft_c0[21] = 42;
		ft_c0.print();
		std_c0[42] = 21;
		std_c0[21] = 42;

		ft_c0.print();
		// for (ft::map<int, exampleClass>::iterator first = ft_c0.begin(); first != ft_c0.end(); first++)
		// {
		// 	std::cout << (*first).first;
		// }
	}
	{
		std::map<char, int> mp;
		mp.insert(std::make_pair('a', 1));
		std::map<char, int>::iterator iter = mp.end();
		std::cout << (*iter).first << std::endl;

		ft::map<char, int> mpft;
		mpft.insert(ft::make_pair('a', 1));
		ft::map<char, int>::iterator iterft = mpft.end();
		std::cout << (*iterft).first << std::endl;
	}
	{
		// exampleClass* a = new exampleClass();
		// exampleClass b;

		// int a[10];
		// a[1000];
		ft::map<char, int> mp;
		mp.insert(ft::make_pair('a', 1));
		mp.insert(ft::make_pair('b', 1));
		ft::map<char, int>::iterator iter = mp.begin();
		ft::map<char, int>::iterator iter1 = ++iter;
		ft::map<char, int>::iterator iter2 = mp.end();
		// *iter;
		// *iter2;
		std::cout << (*iter1 > *(--iter)) << std::endl;
		std::cout << (*iter2 > *iter) << std::endl;
	}
	{
		std::cout << "~~~~~~~~~~~ " << __func__ << " with ints ~~~~~~~~~~~" << std::endl;
		{
			int test_size = 1000;
			std::vector<ft::pair<int, int> > ft_val_0(test_size);
			std::vector<std::pair<int, int> > std_val_0(test_size);
			for (int i = 0; i < test_size; i++)
			{
				srand(i);
				int val = rand() % test_size;
				ft_val_0[i] = ft::make_pair(val, i);
				std_val_0[i] = std::make_pair(val, i);
			}

			std::cout << "[ Instanciate map0 with iterator to vector of pair val_0 ]" << std::endl;
			std::map<int, int> std_c0(std_val_0.begin(), std_val_0.end());
			ft::map<int, int> ft_c0(ft_val_0.begin(), ft_val_0.end());
			// testMap(ft_c0, std_c0, NOPRINT);

			std::cout << "[ Instanciate map1 with default ctor]" << std::endl;
			ft::map<int, int> ft_c1;
			std::map<int, int> std_c1;
			// testMap(ft_c1, std_c1, NOPRINT);

			std::vector<ft::pair<int, int> > ft_val_1(test_size);
			std::vector<std::pair<int, int> > std_val_1(test_size);
			for (int i = 0; i < test_size / 2; i++)
			{
				srand(i);
				int val = rand() % test_size;
				ft_val_1[i] = ft::make_pair(val, test_size - i);
				std_val_1[i] = std::make_pair(val, test_size - i);
			}
			std::cout << "[ Instanciate map2 with iterator to vector of pair val_1 ]" << std::endl;
			std::vector<int> val_1(1, 42);
			std::map<int, int> std_c2(std_val_1.begin(), std_val_1.end());
			ft::map<int, int> ft_c2(ft_val_1.begin(), ft_val_1.end());

			std::cout << "print\n";

			// testMap(ft_c2, std_c2, NOPRINT);

			std::cout << "[ Instanciate map3 with custom Compare empty then insert 1 pair with insert function ]" << std::endl;
			ft::map<int, int> ft_c3;
			std::map<int, int> std_c3;
			ft_c3.insert(ft::make_pair(42, 21));
			std_c3.insert(std::make_pair(42, 21));
			// testMap(ft_c3, std_c3, NOPRINT);

			std::cout << "[ map_0 = map_1 ]" << std::endl;
			std::cout << "c0: \n";
			ft_c0.print();
			std::cout << "\n";
			std::cout << "c1: \n";
			ft_c1.print();

			std::cout << "before call of basic operator equal\n";
			test_basic_operator_equal(ft_c0, ft_c1, std_c0, std_c1);
			// testMap(ft_c0, std_c0, NOPRINT, "test ft_c0/std_c0");
			// testMap(ft_c1, std_c1, NOPRINT, "test ft_c1/std_c1");
			// testMap(ft_c2, std_c2, NOPRINT, "test ft_c2/std_c2");
			// testMap(ft_c3, std_c3, NOPRINT, "test ft_c3/std_c3");
			std::cout << "[ map_1 = map_2 ]" << std::endl;
			test_basic_operator_equal(ft_c1, ft_c2, std_c1, std_c2);
			// testMap(ft_c0, std_c0, NOPRINT, "test ft_c0/std_c0");
			// testMap(ft_c1, std_c1, NOPRINT, "test ft_c1/std_c1");
			// testMap(ft_c2, std_c2, NOPRINT, "test ft_c2/std_c2");
			// testMap(ft_c3, std_c3, NOPRINT, "test ft_c3/std_c3");
			std::cout << "[ map_3 = map_2 ]" << std::endl;
			test_basic_operator_equal(ft_c3, ft_c2, std_c3, std_c2);
			// testMap(ft_c0, std_c0, NOPRINT, "test ft_c0/std_c0");
			// testMap(ft_c1, std_c1, NOPRINT, "test ft_c1/std_c1");
			// testMap(ft_c2, std_c2, NOPRINT, "test ft_c2/std_c2");
			// testMap(ft_c3, std_c3, NOPRINT, "test ft_c3/std_c3");
			std::cout << "[ map_2 = map_2 ]" << std::endl;
			test_basic_operator_equal(ft_c2, ft_c2, std_c2, std_c2);
			// testMap(ft_c0, std_c0, NOPRINT, "test ft_c0/std_c0");
			// testMap(ft_c1, std_c1, NOPRINT, "test ft_c1/std_c1");
			// testMap(ft_c2, std_c2, NOPRINT, "test ft_c2/std_c2");
			// testMap(ft_c3, std_c3, NOPRINT, "test ft_c3/std_c3");
			std::cout << "[ map_0 = map_0 ]" << std::endl;
			test_basic_operator_equal(ft_c0, ft_c0, std_c0, std_c0);
			// testMap(ft_c0, std_c0, NOPRINT, "test ft_c0/std_c0");
			// testMap(ft_c1, std_c1, NOPRINT, "test ft_c1/std_c1");
			// testMap(ft_c2, std_c2, NOPRINT, "test ft_c2/std_c2");
			// testMap(ft_c3, std_c3, NOPRINT, "test ft_c3/std_c3");
		}
	}
	{
		ft::map<char, int> mp;
		ft::map<char, int>::const_iterator cmpi;

		cmpi = mp.begin();
	}
	system("leaks a.out");

	return (0);
}
