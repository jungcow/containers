#ifndef __FT_BST_H__
#define __FT_BST_H__

#include <iostream>

#include "../type.hpp"  //TODO: 지우기
#include "./queue.hpp"

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
class ft::Tree : public Node
{
public:
	typedef typename Node::value_type value_type;
	typedef typename Node::node_size_type node_size_type;
	typedef typename Node::BalanceNode BalanceNode;
	typedef typename Node::node_allocator_type node_allocator_type;
	typedef typename Node::node_value_type node_value_type;

private:
	BalanceNode* root_;
	BalanceNode* foot_;
	node_size_type size_;
	node_allocator_type nalloc;

public:
	Tree() : root_(NULL), foot_(NULL), size_(0), nalloc(node_allocator_type())
	{
		root_ = createNode();
		foot_ = createNode();
		foot_->setRank(size_ + 1);
		root_->setRight(foot_);
	}

	Tree(const Tree& other) : nalloc(node_allocator_type())
	{
		*this = other;
	}

	~Tree()
	{
		deleteAllNodes(root_);
	}

	Tree& operator=(const Tree& other)
	{
		typedef typename node_allocator_type::template rebind<BalanceNode*>::other pointer_allocator;
		ft::Queue<BalanceNode*, pointer_allocator> srcQueue(other.size(), pointer_allocator());
		ft::Queue<BalanceNode*, pointer_allocator> destQueue(other.size(), pointer_allocator());
		BalanceNode* dest;
		BalanceNode* src;

		deleteAllNodes(root_);
		src = other.getRoot();
		root_ = nalloc.allocate(1);

		srcQueue.enqueue(src);
		destQueue.enqueue(root_);
		while (!srcQueue.empty())
		{
			src = srcQueue.dequeue();
			dest = destQueue.dequeue();
			nalloc.construct(dest, *src);
			dest->setLeft(NULL);
			dest->setRight(NULL);

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
		size_ = other.size();
		foot_ = root_->getRight();
		return (*this);
	}

	BalanceNode* getRoot(void) const
	{
		return root_;
	}
	BalanceNode* getFoot(void) const
	{
		return foot_;
	}

	BalanceNode* getFirst(void) const
	{
		BalanceNode* node = root_->getLeft();

		if (!node)
			return foot_;
		while (node->getLeft())
			node = node->getLeft();
		return node;
	}

	BalanceNode* getLast(void) const
	{
		BalanceNode* node = root_->getLeft();

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

	BalanceNode* find(const value_type& value) const
	{
		return (Node::find(root_->getLeft(), value));
	}

	bool insert(const value_type& value)
	{
		bool inserted = false;

		root_->setLeft(Node::insert(root_->getLeft(), value, inserted));
		if (inserted)
			size_++;
		setRootNode(root_->getLeft());
		return (inserted);
	}

	bool insert(BalanceNode* node, const value_type& value)
	{
		bool inserted = false;

		root_->setLeft(Node::insert(node, value, inserted));
		if (inserted)
			size_++;
		setRootNode(root_->getLeft());
		return (inserted);
	}

	bool erase(const value_type& value)
	{
		bool erased = true;

		if (empty())
			return false;
		root_->setLeft(Node::erase(root_->getLeft(), value, root_, erased));
		if (erased)
			size_--;
		setRootNode(root_->getLeft());
		return (erased);
	}

	node_size_type getOrder(const value_type& value) const
	{
		return getOrder(root_->getLeft(), value);
	}

	BalanceNode* OS_Select(BalanceNode* node, size_t i) const
	{
		if (i == size_ + 1)
			return foot_;
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
		printByInOrderTraversal(root_->getLeft());
		std::cout << "\n";
	}

	bool isFollowedAllRules() const
	{
		bool result = true;

		checkBlackNodeCount(root_->getLeft(), &result);
		if (result)
			checkTwoRedNodesContinuously(root_->getLeft(), &result);
		return result;
	}

private:
	node_size_type getOrder(BalanceNode* node, const value_type& value) const
	{
		return Node::getOrder(node, value);
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
		return Node::createNode(value);
	}
	BalanceNode* createNode(const BalanceNode& other)
	{
		return Node::createNode(other);
	}

	void deleteNode(BalanceNode* node)
	{
		Node::deleteNode(node);
	}

	void deleteAllNodes(BalanceNode* node)
	{
		Node::deleteAllNodes(node);
	}

	void setRootNode(BalanceNode* node)
	{
		Node::setRootNode(node);
	}

	void printNode(BalanceNode* node) const
	{
		Node::printNode(node);
	}
};

#endif
