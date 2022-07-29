#ifndef __FT_STACK_H__
#define __FT_STACK_H__

#include "vector.hpp"

namespace ft
{

	template <class T, class Container = ft::vector<T> >
	class stack
	{
	public:
		typedef T value_type;
		typedef Container container_type;
		typedef typename container_type::size_type size_type;

	protected:
		container_type c;

	public:
		explicit stack(const container_type& cntr = container_type())
			: c(cntr) {}

		bool empty() const
		{
			return (c.empty());
		}

		void pop()
		{
			return (c.pop_back());
		}

		void push(const value_type& val)
		{
			return (c.push_back(val));
		}

		size_type size() const
		{
			return (c.size());
		}

		value_type& top()
		{
			return (c.back());
		}
		const value_type& top() const
		{
			return (c.back());
		}

		template <class U, class Cont>
		friend bool operator==(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs);
		template <class U, class Cont>
		friend bool operator!=(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs);
		template <class U, class Cont>
		friend bool operator<(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs);
		template <class U, class Cont>
		friend bool operator<=(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs);
		template <class U, class Cont>
		friend bool operator>(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs);
		template <class U, class Cont>
		friend bool operator>=(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs);
	};
	template <class U, class Cont>
	bool operator==(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
	{
		return (lhs.c == rhs.c);
	}
	template <class U, class Cont>
	bool operator!=(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
	{
		return !(lhs == rhs);
	}
	template <class U, class Cont>
	bool operator<(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
	{
		return (lhs.c < rhs.c);
	}
	template <class U, class Cont>
	bool operator<=(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
	{
		return !(rhs < lhs);
	}
	template <class U, class Cont>
	bool operator>(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
	{
		return (rhs < lhs);
	}
	template <class U, class Cont>
	bool operator>=(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
	{
		return !(lhs < rhs);
	}
}

#endif
