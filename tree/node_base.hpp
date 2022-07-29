#ifndef __FT_NODE_BASE_H__
#define __FT_NODE_BASE_H__

#include "../iterator.hpp"
#include "../type_traits.hpp"
#include "node_wrapper.hpp"

namespace ft
{
	namespace node
	{
		template <class P1, class P2, class Compare, class Alloc>
		class NodeBase;
	}
}

/**
 * @class
 *
 * @param P1: real pointer used for compatibility with const NodeBase
 * @param P2: origin pointer used for compatiblity with const NodeBase
 * @param Compare: class comparator for compare between values
 * @param Alloc: class for allocate value type
 */
template <class P1, class P2, class Compare, class Alloc>
class ft::node::NodeBase
{
public:
	typedef Compare compare_type;
	typedef Alloc allocator_type;

	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;

	typedef typename allocator_type::value_type value_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_reference const_reference;

	typedef P1 value_iterator_type;
	typedef P2 value_pointer_type;

private:
	value_type val_;
	compare_type compare_value_;

public:
	NodeBase()
	{
	}

	NodeBase(const value_type& value)
		: val_(value), compare_value_(compare_type())
	{
	}

	template <class Pointer>
	NodeBase(const NodeBase<Pointer,
							typename ft::enable_if<ft::is_same<Pointer, P2>::value, P2>::type,
							Compare, Alloc>& other)
		: val_(other.getValue()), compare_value_(compare_type())
	{
	}

	~NodeBase()
	{
	}

	reference getValue(void) const
	{
		return (val_);
	}
	reference getValue(void)
	{
		return (val_);
	}

	void setValue(const_reference value)
	{
		val_ = value;
	}

	bool compareValue(const_reference lhs, const_reference rhs) const
	{
		return compare_value_(lhs, rhs);
	}
};

#endif
