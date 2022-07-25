#ifndef __FT_BST_H__
#define __FT_BST_H__

#include <iostream>  // TODO: 지우기
#include <memory>    // TODO: 지우기 -> default argument로 적은 std 지우기

#include "node.hpp"

namespace ft
{
	namespace tree
	{
		template <class Node, class Comp, class Alloc>
		class BSTree;
	}
}

template <class Node, class Comp, class Alloc>
class ft::tree::BSTree
{
private:
	// typedef ft::tree::Node<P1, P2, Alloc> Node;

	typedef typename Node::value_type value_type;
	typedef typename Node::pointer pointer;
	typedef typename Node::const_pointer const_pointer;
	typedef typename Node::reference reference;
	typedef typename Node::const_reference const_reference;
	typedef typename Node::size_type size_type;

	typedef Alloc allocator_type;
	typedef typename allocator_type::template rebind<Node>::other node_allocator_type;

	typedef typename node_allocator_type::size_type node_size_type;
	typedef typename node_allocator_type::value_type node_value_type;

private:
	Node* root_;
	node_size_type size_;
	node_allocator_type allocator_;
	Comp compareValue;

public:
	BSTree(const Comp& compare = Comp(), const node_allocator_type& alloc = node_allocator_type())
		: root_(NULL), size_(0), allocator_(alloc), compareValue(compare)
	{
		root_ = createNode();
	}

	/**
	 * TODO:
	 * 복사 생성자
	 * 대입 연산자?
	 */

	~BSTree()
	{
		deleteAllNodes(root_->getLeft());
		deleteNode(root_);
	}

public:
	bool empty() const
	{
		return size_ == 0;
	}

	node_size_type size() const
	{
		return size_;
	}

	Node* find(const value_type& value) const
	{
		return (find(root_->getLeft(), value));
	}

	Node* insert(const value_type& value)
	{
		Node* result = insert(root_->getLeft(), value);
		if (!result)
			return NULL;
		size_++;
		root_->setLeft(result);
		return (root_);
	}

	node_size_type erase(const value_type& value)
	{
		if (empty())
			return 0;

		if (!erase(root_->getLeft(), value))
			return 0;
		size_--;
		return 1;
	}

	void printByInOrderTraversal() const
	{
		printByInOrderTraversal(root_->getLeft());
		std::cout << "\n";
	}

private:
	void printByInOrderTraversal(Node* node) const
	{
		if (node == NULL)
			return;
		printByInOrderTraversal(node->getLeft());
		std::cout << node->getValue().first << "(" << node->getRank() << ")" << '-';
		printByInOrderTraversal(node->getRight());
	}
	Node* find(Node* node, const value_type& value) const
	{
		if (node == NULL)
			return NULL;

		if (compareValue(node->getValue(), value))
			return find(node->getRight(), value);
		else if (compareValue(value, node->getValue()))
			return find(node->getLeft(), value);
		return node;
	}

	Node* insert(Node* node, const value_type& value)
	{
		if (node == NULL)
			return (createNode(value));

		if (compareValue(node->getValue(), value))
			node->setRight(insert(node->getRight(), value));
		else if (compareValue(value, node->getValue()))
			node->setLeft(insert(node->getLeft(), value));
		node->setRank(calculateRankFrom(node));
		return node;
	}

	Node* erase(Node* node, const value_type& value)
	{
		if (node == NULL)
			return NULL;

		if (compareValue(node->getValue(), value))
			node->setRight(erase(node->getRight(), value));
		else if (compareValue(value, node->getValue()))
			node->setLeft(erase(node->getLeft(), value));
		else
		{
			Node* tmp;
			if (node->getLeft() == NULL)
			{
				tmp = node->getRight();
				deleteNode(node);
				return tmp;
			}
			else if (node->getRight() == NULL)
			{
				tmp = node->getLeft();
				deleteNode(node);
				return tmp;
			}
			tmp = node;
			node = getMinNodeFrom(node->getRight());
			node->setRight(eraseMinNodeFrom(tmp->getRight()));
			node->setLeft(tmp->getLeft());
			deleteNode(tmp);
		}
		node->setRank(calculateRankFrom(node));
		return node;
	}

	Node* getMinNodeFrom(Node* node)
	{
		if (node->getLeft() == NULL)
			return node;
		return (getMinNodeFrom(node->getLeft()));
	}

	Node* eraseMinNodeFrom(Node* node)
	{
		if (node->getLeft() == NULL)
		{
			Node* tmp = node->getRight();
			return tmp;
		}
		node->setLeft(eraseMinNodeFrom(node->getLeft()));
		node->setRank(calculateRankFrom(node));
		return node;
	}

private:
	Node* createNode(const value_type& value = value_type())
	{
		Node* newNode = allocator_.allocate(1);
		allocator_.construct(newNode, node_value_type(value));  // TODO: 확인해보기
		return newNode;
	}

	void deleteNode(Node* node)
	{
		allocator_.destroy(node);
		allocator_.deallocate(node, 1);
	}

	void deleteAllNodes(Node* node)
	{
		if (node == NULL)
			return;
		deleteAllNodes(node->getRight());
		deleteAllNodes(node->getLeft());
		allocator_.destroy(node);
		allocator_.deallocate(node, 1);
	}

	size_type calculateRankFrom(Node* node)
	{
		size_type lRank = 0;
		size_type rRank = 0;
		if (node->getLeft())
			lRank = node->getLeft()->getRank();
		if (node->getRight())
			rRank = node->getRight()->getRank();
		return (lRank + rRank + 1);
	}
};

#endif
