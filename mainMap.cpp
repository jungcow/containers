#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>

#include "map.hpp"
#include "tree/node.hpp"
#include "tree/rb_balance.hpp"
#include "tree/tree.hpp"
#include "type.hpp"
#include "utility.hpp"

using namespace std;

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
			std::cout << "size: " << mp.size() << std::endl;
		}

		std::cout << (*mp.begin()).first << std::endl;
		std::cout << (*mp.end()).first << std::endl;
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
		std::cout << "*a++: " << reinterpret_cast<void*>(&mpi) << std::endl;

		--mpi;
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
		std::cout << (*(mp.find('c'))).second << std::endl;
		std::cout << (*(mp.find('y'))).first << std::endl;
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
	system("leaks a.out");

	return (0);
}
