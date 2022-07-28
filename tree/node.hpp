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

	typedef typename allocator_type::value_type value_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;

	/**
	 * derived type
	 */
	typedef BalanceType BalanceNode;
	typedef typename allocator_type::template rebind<BalanceNode>::other node_allocator_type;
	typedef typename node_allocator_type::value_type node_value_type;
	typedef typename node_allocator_type::pointer node_pointer;
	typedef typename node_allocator_type::reference node_reference;
	typedef typename node_allocator_type::const_pointer node_const_pointer;
	typedef typename node_allocator_type::const_reference node_const_reference;

	typedef typename node_allocator_type::size_type node_size_type;
	typedef typename node_allocator_type::difference_type node_difference_type;

public:
	typedef node_iterator<BalanceNode*, value_iterator_type, value_pointer_type> iterator;
	typedef node_iterator<const BalanceNode*, value_iterator_type, value_pointer_type> const_iterator;

private:
	node_size_type rank_;
	BalanceNode* left_;
	BalanceNode* right_;

public:
	Node() : NodeBase(), rank_(1), left_(NULL), right_(NULL)
	{
	}

	Node(const value_type& value) : NodeBase(value), rank_(1), left_(NULL), right_(NULL)
	{
	}

	template <class Pointer>
	Node(const Node<Pointer,
					typename ft::enable_if<ft::is_same<Pointer, value_pointer_type>::value,
										   value_pointer_type>::type>& other)
		: NodeBase(other),
		  rank_(other.getRank()),
		  left_(NULL),
		  right_(NULL)
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
	}
	BalanceNode* getRight(void) const
	{
		return right_;
	}

	node_size_type getRank(void) const
	{
		return rank_;
	}

	node_size_type getOrder(BalanceNode* node, const value_type& value) const
	{
		node_size_type order;

		order = 0;
		while (node)
		{
			if (compareValue(value, node->getValue()))
			{
				node = node->getLeft();
			}
			else if (compareValue(node->getValue(), value))
			{
				if (!node->getLeft())
					order += 1;
				else
					order += node->getLeft()->getRank() + 1;
				node = node->getRight();
			}
			else
			{
				if (!node->getLeft())
					order += 1;
				else
					order += node->getLeft()->getRank() + 1;
				break;
			}
		}
		return order;
	}

	void setLeft(BalanceNode* node)
	{
		left_ = node;
	}
	void setRight(BalanceNode* node)
	{
		right_ = node;
	}
	void setRank(node_size_type r)
	{
		rank_ = r;
	}

	BalanceNode* find(BalanceNode* node, const value_type& value, BalanceNode* parent, BalanceNode** arrived) const
	{
		if (node == NULL)
		{
			// std::cout << "\n\nNot Found!!\n\n"; // TODO: 주석 지우기
			*arrived = parent;
			return NULL;
		}

		if (compareValue(node->getValue(), value))
			return find(node->getRight(), value, node, arrived);
		else if (compareValue(value, node->getValue()))
			return find(node->getLeft(), value, node, arrived);
		*arrived = node;
		return node;
	}

	BalanceNode* insert(BalanceNode* node, const value_type& value, bool& inserted)
	{
		return static_cast<BalanceNode&>(*this).insert(node, value, inserted);
	}

	BalanceNode* erase(BalanceNode* node, const value_type& value, BalanceNode* parent, bool& erased)
	{
		return static_cast<BalanceNode&>(*this).erase(node, value, parent, erased);
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
	BalanceNode* createNode(const BalanceNode& other)
	{
		return static_cast<BalanceNode&>(*this).createNode(other);
	}

	BalanceNode* deleteNode(BalanceNode* node)
	{
		return static_cast<BalanceNode&>(*this).deleteNode(node);
	}

	BalanceNode* deleteAllNodes(BalanceNode* node)
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

	node_size_type calculateRankFrom(BalanceNode* node)
	{
		if (!node)
			return 0;
		node_size_type lRank = 0;
		node_size_type rRank = 0;
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

		node_size_type lRank = 0;
		node_size_type rRank = 0;

		if (node->getLeft())
			lRank = calculateAllNodesRank(node->getLeft())->getRank();
		if (node->getRight())
			rRank = calculateAllNodesRank(node->getRight())->getRank();
		node->setRank(lRank + rRank + 1);
		return (node);
	}

	bool compareValue(const value_type& lhs, const value_type& rhs) const
	{
		return base::compareValue(lhs, rhs);
	}
};

template <class Iterator, class ValueIter, class ValuePointer>
class ft::node::node_iterator
{
private:
	typedef ft::iterator<std::bidirectional_iterator_tag, typename ft::remove_pointer<Iterator>::type> node_iterator_type;
	typedef ft::iterator<std::bidirectional_iterator_tag, typename ft::remove_pointer<ValueIter>::type> iterator_type;

public:
	// for value type
	typedef typename iterator_type::value_type value_type;
	typedef typename iterator_type::pointer pointer;
	typedef typename iterator_type::reference reference;
	typedef typename iterator_type::difference_type difference_type;
	typedef typename iterator_type::iterator_category iterator_category;

	// for node type
	typedef typename node_iterator_type::value_type node_value_type;
	typedef typename node_iterator_type::pointer node_pointer;
	typedef typename node_iterator_type::reference node_reference;
	typedef typename node_iterator_type::difference_type node_difference_type;
	typedef typename node_iterator_type::iterator_category node_iterator_category;

private:
	Iterator base_;  // same with BalanceNode*

public:
	node_iterator()
	{
	}

	template <class P, class Vp>
	node_iterator(const node_iterator<P, Vp,
									  typename ft::enable_if<ft::is_same<Vp, ValuePointer>::value, ValuePointer>::type>& other)
		: base_(reinterpret_cast<Iterator>(other.base()))
	{
	}

	explicit node_iterator(Iterator otherNode)
		: base_(reinterpret_cast<Iterator>(otherNode))
	{
	}

	node_iterator& operator=(const node_iterator& other)
	{
		base_ = other.base_;
		return (*this);
	}

	~node_iterator() {}

	Iterator base(void)
	{
		return base_;
	}
	Iterator base(void) const
	{
		return base_;
	}

	reference operator*() const
	{
		return (base_->getValue());
	}
	reference operator*()
	{
		return (base_->getValue());
	}

	pointer operator->() const
	{
		return &(base_->getValue());
	}
	pointer operator->()
	{
		return &(base_->getValue());
	}

	size_t rank() const
	{
		return base_->getRank();
	}

	Iterator left() const
	{
		return base_->getLeft();
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
