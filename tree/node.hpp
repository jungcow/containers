#ifndef __FT_NODE_H__
#define __FT_NODE_H__

#include "../iterator.hpp"
#include "../type_traits.hpp"
#include "node_wrapper.hpp"

namespace ft
{
	namespace node
	{
		template <class NodeBase, class BalanceType>
		class Node;

		template <class Iterator, class ValueIter, class ValuePointer>
		class node_iterator;
	}
}

/**
 * @class
 *
 * @param NodeBase: base class of this Node class which have type definition all about the value_type
 * @param BalanceType: derived class of this Node class which have balance
 */
template <class NodeBase, class BalanceType>
class ft::node::Node : public NodeBase
{
private:
	typedef NodeBase base;
	typedef typename base::value_iterator_type value_iterator_type;
	typedef typename base::value_pointer_type value_pointer_type;

public:
	/**
	 * base type
	 */
	typedef typename base::compare_type compare_type;
	typedef typename base::allocator_type allocator_type;

	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;

	typedef typename allocator_type::value_type value_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_reference const_reference;

	/**
	 * derived type
	 */
	typedef BalanceType BalanceNode;
	typedef typename allocator_type::template rebind<BalanceType>::other node_allocator_type;
	// typedef typename node_allocator_type::value_type node_value_type;
	// typedef typename node_allocator_type::pointer node_pointer;
	// typedef typename node_allocator_type::reference node_reference;
	// typedef typename node_allocator_type::const_pointer node_const_pointer;
	// typedef typename node_allocator_type::const_reference node_const_reference;

	typedef typename node_allocator_type::size_type node_size_type;
	// typedef typename node_allocator_type::difference_type node_difference_type;

public:
	typedef node_iterator<Node*, value_iterator_type, value_pointer_type> iterator;
	typedef node_iterator<const Node*, value_iterator_type, value_pointer_type> const_iterator;

	// Node(BalanceType) {
	// 	std::cout << "conversion constructor\n";
	// }

private:
	size_type rank_;
	BalanceNode* left_;
	BalanceNode* right_;

public:
	Node() : NodeBase(), rank_(1)
	{
	}

	Node(const value_type& value) : NodeBase(value), rank_(1)
	{
	}

	template <class Pointer>
	Node(const Node<Pointer,
					typename ft::enable_if<ft::is_same<Pointer, value_pointer_type>::value,
										   value_pointer_type>::type>& other)
		: NodeBase(other),
		  rank_(other.getRank())
	{
	}

	~Node()
	{
	}

	BalanceNode& operator=(const BalanceNode& other)
	{
		return static_cast<BalanceNode&>(*this).operator=(other);
	}

	BalanceNode* getLeft(void) const
	{
		return left_;
		// return (static_cast<BalanceNode&>(*this).getLeft());
	}
	BalanceNode* getRight(void) const
	{
		return right_;
		// return (static_cast<BalanceNode&>(*this).getRight());
	}
	size_type getRank(void) const
	{
		return rank_;
	}

	void setLeft(BalanceNode* node)
	{
		left_ = node;
	}
	void setRight(BalanceNode* node)
	{
		right_ = node;
	}
	void setRank(size_type r)
	{
		rank_ = r;
	}

	BalanceNode* find(BalanceNode* node, const value_type& value) const
	{
		if (node == NULL)
			return NULL;

		if (compareValue(node->getValue(), value))
			return find(node->getRight(), value);
		else if (compareValue(value, node->getValue()))
			return find(node->getLeft(), value);
		return node;
	}

	BalanceNode* insert(BalanceNode* node, const value_type& value)
	{
		return static_cast<BalanceNode&>(*this).insert(node, value);
	}

	BalanceNode* erase(BalanceNode* node, const value_type& value, BalanceNode* parent)
	{
		return static_cast<BalanceNode&>(*this).erase(node, value, parent);
	}

	BalanceNode* getMinNodeFrom(BalanceNode* node)
	{
		if (node->getLeft() == NULL)
			return node;
		return (getMinNodeFrom(node->getLeft()));
	}

	BalanceNode* eraseMinNodeFrom(BalanceNode* node)
	{
		if (node->getLeft() == NULL)
			return node->getRight();
		node->setLeft(eraseMinNodeFrom(node->getLeft()));
		node->setRank(calculateRankFrom(node));
		return node;
	}

	BalanceNode* createNode(const value_type& value = value_type())
	{
		return static_cast<BalanceNode&>(*this).createNode(value);
	}

	void deleteNode(BalanceNode* node)
	{
		return static_cast<BalanceNode&>(*this).deleteNode(node);
	}

	void deleteAllNodes(BalanceNode* node)
	{
		return static_cast<BalanceNode&>(*this).deleteAllNodes(node);
	}

	void setRootNode(BalanceNode* node)
	{
		return static_cast<BalanceNode&>(*this).setRootNode(node);
	}

	void printNode(BalanceNode* node) const
	{
		return static_cast<const BalanceNode&>(*this).printNode(node);
	}

	size_type calculateRankFrom(BalanceNode* node)
	{
		size_type lRank = 0;
		size_type rRank = 0;
		if (node->getLeft())
			lRank = node->getLeft()->getRank();
		if (node->getRight())
			rRank = node->getRight()->getRank();
		return (lRank + rRank + 1);
	}

	BalanceNode* calculateAllNodesRank(BalanceNode* node)
	{
		if (!node->getLeft() && !node->getRight())
		{
			node->setRank(1);
			return node;
		}

		size_type lRank = 0;
		size_type rRank = 0;

		if (node->getLeft())
			lRank = calculateAllNodesRank(node->getLeft())->getRank();
		if (node->getRight())
			rRank = calculateAllNodesRank(node->getRight())->getRank();
		node->setRank(lRank + rRank + 1);
		return (node);
	}

	bool compareValue(const value_type& lhs, const value_type& rhs)
	{
		return base::compareValue(lhs, rhs);
	}
};

template <class Iterator, class ValueIter, class ValuePointer>
class ft::node::node_iterator
{
private:
	typedef ft::iterator<std::bidirectional_iterator_tag, typename ft::remove_pointer<Iterator>::type> iterator;

public:
	// for value type
	typedef typename ft::remove_pointer<ValueIter>::type inner_value_type;
	typedef inner_value_type* inner_pointer;
	typedef inner_value_type& inner_reference;
	typedef const inner_value_type* const_inner_pointer;
	typedef const inner_value_type& const_inner_reference;

	// for node type
	typedef typename iterator::value_type value_type;
	typedef typename iterator::pointer pointer;
	typedef typename iterator::reference reference;
	typedef typename iterator::difference_type difference_type;
	typedef typename iterator::iterator_category iterator_category;

private:
	Iterator base_;  // same with Node*

public:
	node_iterator()
	{
		std::cout << "node iterator default constructor\n";
	}

	template <class P, class Vp>
	node_iterator(const node_iterator<P, Vp,
									  typename ft::enable_if<ft::is_same<Vp, ValuePointer>::value, ValuePointer>::type>& other)
		: base_(reinterpret_cast<Iterator>(other.base()))  // TODO: 확인하기
	{
		std::cout << "node iterator copy constructor\n";
	}

	explicit node_iterator(const Iterator& otherNode)
		: base_(otherNode)
	{
	}

	node_iterator& operator=(const node_iterator& other)
	{
		base_ = other.base_;
		return (*this);
	}

	~node_iterator() {}

	Iterator base(void) const
	{
		return base_;
	}

	inner_reference operator*() const
	{
		return (base_->getValue());
	}

	inner_pointer operator->() const
	{
		return &(base_->getValue());
	}

	template <class I, class V, class Vp>
	friend bool operator==(const node_iterator<I, V, Vp>& lhs, const node_iterator<I, V, Vp>& rhs);
	template <class I, class V, class Vp>
	friend bool operator!=(const node_iterator<I, V, Vp>& lhs, const node_iterator<I, V, Vp>& rhs);
};

namespace ft
{
	namespace node
	{
		template <class I, class V, class Vp>
		bool operator==(const node_iterator<I, V, Vp>& lhs, const node_iterator<I, V, Vp>& rhs)
		{
			return (lhs.base_ == rhs.base_);
		}
		template <class I, class V, class Vp>
		bool operator!=(const node_iterator<I, V, Vp>& lhs, const node_iterator<I, V, Vp>& rhs)
		{
			return !(lhs == rhs);
		}
	}
}

#endif
