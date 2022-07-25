#include <list>
#include <vector>

#include "vector.hpp"
int main(void)
{
	{
		std::vector<int> vector;

		int _ratio = 10000;
		std::vector<int> v;
		std::cout << "size: " << vector.size() << std::endl;
		vector.assign(9900 * _ratio, 1);
		std::cout << "size: " << vector.size() << std::endl;
		vector.resize(5000 * _ratio);
		std::cout << "size: " << vector.size() << std::endl;
		vector.reserve(5000 * _ratio);
		std::cout << "size: " << vector.size() << std::endl;
		v.push_back(vector.size());
		v.push_back(vector.capacity());
		vector.resize(7000 * _ratio);
		std::cout << "size: " << vector.size() << std::endl;
		v.push_back(vector.size());
		v.push_back(vector.capacity());
		vector.resize(15300 * _ratio, 3);
		std::cout << "size: " << vector.size() << std::endl;
		v.push_back(vector.size());
		v.push_back(vector.capacity());
		v.push_back(vector[65]);
		std::cout << "size: " << vector.size() << std::endl;
		std::cout << "capacity: " << vector.capacity() << std::endl;

		ft::vector<int> vectorft;

		std::vector<int> vft;
		std::cout << "size: " << vectorft.size() << std::endl;
		vectorft.assign(9900 * _ratio, 1);
		std::cout << "size: " << vectorft.size() << std::endl;
		vectorft.resize(5000 * _ratio);
		std::cout << "size: " << vectorft.size() << std::endl;
		vectorft.reserve(5000 * _ratio);
		std::cout << "size: " << vectorft.size() << std::endl;
		vft.push_back(vectorft.size());
		vft.push_back(vectorft.capacity());
		vectorft.resize(7000 * _ratio);
		std::cout << "size: " << vectorft.size() << std::endl;
		vft.push_back(vectorft.size());
		vft.push_back(vectorft.capacity());
		vectorft.resize(15300 * _ratio, 3);
		std::cout << "size: " << vectorft.size() << std::endl;
		vft.push_back(vectorft.size());
		vft.push_back(vectorft.capacity());
		vft.push_back(vectorft[65]);
		std::cout << "size: " << vectorft.size() << std::endl;
		std::cout << "capacity: " << vectorft.capacity() << std::endl;

		for (size_t i = 0; i < vector.size(); i++)
		{
			if (vector[i] != vectorft[i])
				std::cout << "i: " << i << ", value-> (" << vector[i] << ", " << vectorft[i] << ")" << std::endl;
		}
	}
	{
		std::list<int> stdli(10000, 1);
		ft::vector<int> stdvec(stdli.begin(), stdli.end());
		std::cout << "vec from list size: " << stdvec.size() << std::endl;
	}
	{
		ft::vector<int>::iterator veci;
		ft::vector<int>::const_iterator vecci;

		vecci = veci;
	}
}
