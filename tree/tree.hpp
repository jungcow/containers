#ifndef __FT_BST_H__
#define __FT_BST_H__

#include <iostream>

#include "../utility.hpp" // ft::pair, ft::make_pair
#include "queue.hpp" // 

namespace ft
{
	template <class Node>
	class Tree;
}

/**
 * @class
 *
 * @param Node: Node for Tree
 */
template <class Node>
class ft::Tree
{
private:
	typedef typename Node::BalanceNode BalanceNode;
	typedef typename Node::node_size_type node_size_type;
	typedef typename Node::node_allocator_type node_allocator_type;
	typedef typename Node::node_value_type node_value_type;

public:
	typedef typename Node::value_type value_type;

private:
	BalanceNode* end_node_;
	node_size_type size_;
	node_allocator_type nalloc;

public:
	Tree() : end_node_(NULL), size_(0), nalloc(node_allocator_type())
	{
		end_node_ = createNode();
		end_node_->setRank(Node().calculateRankFrom(end_node_));
	}

	Tree(const Tree& other) : end_node_(NULL), size_(0), nalloc(node_allocator_type())
	{
		end_node_ = createNode();
		end_node_->setRank(Node().calculateRankFrom(end_node_));
		*this = other;
	}

	~Tree()
	{
		deleteAllNodes(end_node_);
	}

	Tree& operator=(const Tree& other)
	{
		if (this->end_node_ == other.end_node_)
			return (*this);
		typedef typename node_allocator_type::template rebind<BalanceNode*>::other pointer_allocator;
		ft::Queue<BalanceNode*, pointer_allocator> srcQueue(other.size(), pointer_allocator());
		ft::Queue<BalanceNode*, pointer_allocator> destQueue(other.size(), pointer_allocator());
		BalanceNode* dest;
		BalanceNode* src;

		end_node_->setLeft(deleteAllNodes(end_node_->getLeft()));
		size_ = other.size();
		src = other.getEndNode()->getLeft();
		if (!src)
			return (*this);
		end_node_->setLeft(nalloc.allocate(1));

		srcQueue.enqueue(src);
		destQueue.enqueue(end_node_->getLeft());
		while (src && !srcQueue.empty())
		{
			src = srcQueue.dequeue();
			dest = destQueue.dequeue();
			nalloc.construct(dest, *src);

			if (src->getLeft())
			{
				dest->setLeft(nalloc.allocate(1));
				destQueue.enqueue(dest->getLeft());
				srcQueue.enqueue(src->getLeft());
			}
			if (src->getRight())
			{
				dest->setRight(nalloc.allocate(1));
				destQueue.enqueue(dest->getRight());
				srcQueue.enqueue(src->getRight());
			}
		}
		end_node_->setRank(Node().calculateRankFrom(end_node_));
		return (*this);
	}

	BalanceNode* getEndNode(void) const
	{
		return end_node_;
	}

	BalanceNode* getFirst(void) const
	{
		BalanceNode* node = this->end_node_;

		while (node->getLeft())
			node = node->getLeft();
		return node;
	}

	BalanceNode* getLast(void) const
	{
		BalanceNode* node = end_node_->getLeft();

		while (node && node->getRight())
			node = node->getRight();
		return node;
	}

	bool empty() const
	{
		return size_ == 0;
	}

	node_size_type size() const
	{
		return size_;
	}

	ft::pair<BalanceNode*, BalanceNode*> find(const value_type& value) const
	{
		BalanceNode* arrivedNode;
		BalanceNode* foundNode;

		foundNode = Node().find(end_node_->getLeft(), value, end_node_, &arrivedNode);
		return ft::make_pair(foundNode, arrivedNode);
	}

	ft::pair<BalanceNode*, bool> insert(const value_type& value)
	{
		BalanceNode* insertedNode;
		bool inserted = false;

		end_node_->setLeft(Node().insert(end_node_->getLeft(), value, &insertedNode, inserted));
		if (inserted)
			size_++;
		setEndNode(end_node_);
		return (ft::make_pair(insertedNode, inserted));
	}

	ft::pair<BalanceNode*, bool> insert(BalanceNode* node, const value_type& value)
	{
		BalanceNode* insertedNode;
		bool inserted = false;

		end_node_->setLeft(Node().insert(node, value, &insertedNode, inserted));
		if (inserted)
			size_++;
		setEndNode(end_node_);
		return (ft::make_pair(insertedNode, inserted));
	}

	bool erase(const value_type& value)
	{
		bool erased = true;

		if (empty())
			return false;
		end_node_->setLeft(Node().erase(end_node_->getLeft(), value, end_node_, erased));
		if (erased)
			size_--;
		setEndNode(end_node_);
		return (erased);
	}

	node_size_type getOrder(const value_type& value) const
	{
		return getOrder(end_node_->getLeft(), value);
	}

	BalanceNode* OS_Select(BalanceNode* node, size_t i) const
	{
		if (i > size_)
			return end_node_;
		if (i < 1 || !node)
			return NULL;

		size_t r;
		if (!node->getLeft())
			r = 1;
		else
			r = node->getLeft()->getRank() + 1;
		if (i == r)
			return node;
		else if (i < r)
			return OS_Select(node->getLeft(), i);
		else
			return OS_Select(node->getRight(), i - r);
	}

	void printByInOrderTraversal() const
	{
		printByInOrderTraversal(end_node_->getLeft());
		std::cout << "\n";
	}

	bool isFollowedAllRules() const
	{
		bool result = true;

		checkBlackNodeCount(end_node_->getLeft(), &result);
		if (result)
			checkTwoRedNodesContinuously(end_node_->getLeft(), &result);
		return result;
	}

private:
	node_size_type getOrder(BalanceNode* node, const value_type& value) const
	{
		return Node().getOrder(node, value);
	}

	void printByInOrderTraversal(BalanceNode* node) const
	{
		if (node == NULL)
			return;
		printByInOrderTraversal(node->getLeft());

		printNode(node);

		printByInOrderTraversal(node->getRight());
	}

	int checkBlackNodeCount(BalanceNode* node, bool* result) const
	{
		int l, r;
		if (node == NULL)
			return 1;
		l = checkBlackNodeCount(node->getLeft(), result);
		r = checkBlackNodeCount(node->getRight(), result);
		if (l != r)
			*result = false;
		return l + (node->getColor());
	}

	void checkTwoRedNodesContinuously(BalanceNode* node, bool* result) const
	{
		if (node == NULL || !node->getLeft() || !node->getRight())
			return;
		checkTwoRedNodesContinuously(node->getLeft(), result);
		checkTwoRedNodesContinuously(node->getRight(), result);
		if (node->getColor() == BalanceNode::Red && node->getLeft()->getColor() == BalanceNode::Red)
			*result = false;
		if (node->getColor() == BalanceNode::Red && node->getRight()->getColor() == BalanceNode::Red)
			*result = false;
	}

	BalanceNode* createNode(const value_type& value = value_type())
	{
		return Node().createNode(value);
	}
	BalanceNode* createNode(const BalanceNode& other)
	{
		return Node().createNode(other);
	}

	BalanceNode* deleteNode(BalanceNode* node)
	{
		return Node().deleteNode(node);
	}

	BalanceNode* deleteAllNodes(BalanceNode* node)
	{
		return Node().deleteAllNodes(node);
	}

	void setEndNode(BalanceNode* node)
	{
		Node().setEndNode(node);
	}

	void printNode(BalanceNode* node) const
	{
		Node().printNode(node);
	}
};

#endif
