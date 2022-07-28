#ifndef __FT_RB_BALANCE_H__
#define __FT_RB_BALANCE_H__

#include "node_wrapper.hpp"

template <class N>
class ft::NodeWrapper<N>::RBBalance : public TNode<RBBalance>::type
{
private:
	typedef RBBalance Node;

public:
	enum Color
	{
		Red,
		Black,
		DoubleBlack
	};

	/**
	 * Base Type
	 */
	typedef typename TNode<RBBalance>::type base_node;
	typedef typename base_node::allocator_type allocator_type;

	typedef typename allocator_type::value_type value_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;

public:
	/**
	 * RB Node Type
	 */
	typedef typename base_node::node_allocator_type node_allocator_type;
	typedef typename node_allocator_type::value_type node_value_type;
	typedef typename node_allocator_type::pointer node_pointer;
	typedef typename node_allocator_type::reference node_reference;
	typedef typename node_allocator_type::const_pointer node_const_pointer;
	typedef typename node_allocator_type::const_reference node_const_reference;

	typedef typename node_allocator_type::size_type node_size_type;
	typedef typename node_allocator_type::difference_type node_difference_type;

private:
	Color color_;
	bool Nil_;
	node_allocator_type rb_node_allocator_;

public:
	RBBalance()
		: base_node(),
		  color_(Red),
		  Nil_(false),
		  rb_node_allocator_(node_allocator_type())
	{
	}

	RBBalance(const value_type& value, const Color& color = Red)
		: base_node(value),
		  color_(color),
		  Nil_(false),
		  rb_node_allocator_(node_allocator_type())
	{
	}

	RBBalance(const RBBalance& other)
		: base_node(other),
		  color_(other.getColor()),
		  Nil_(other.getNil()),
		  rb_node_allocator_(node_allocator_type())
	{
	}

	~RBBalance()
	{
	}

	Color getColor(void) const
	{
		return (color_);
	}

	bool getNil(void) const
	{
		return (Nil_);
	}

	bool isNil(Node* node) const
	{
		if (node == NULL || node->Nil_ == false)
			return false;
		return true;
	}

	void setColor(const Color& color)
	{
		color_ = color;
	}

	void setNil(bool nil)
	{
		Nil_ = nil;
	}

	void setRootNode(Node* endNode)
	{
		if (!endNode || !endNode->getLeft())
			return;
		if (isNil(endNode->getLeft()))
		{
			deleteAllNodes(endNode->getLeft());
			endNode->setLeft(NULL);
		}
		if (!isBlack(endNode->getLeft()))
			endNode->getLeft()->setColor(Black);
		endNode->setRank(this->calculateRankFrom(endNode));
	}

	void printNode(Node* node) const
	{
		std::cout << node->getValue().first << "(" << node->getRank() << ", ";
		if (node->getColor() == Red)
			std::cout << "R";
		else if (node->getColor() == Black)
			std::cout << "B";
		else if (node->getColor() == DoubleBlack)
			std::cout << "DB";
		std::cout << ")" << '-';
	}

	void flipColor(Node* node)
	{
		if (isRed(node))
			node->setColor(Black);
		else if (isBlack(node))
			node->setColor(Red);
	}

	void splitNode(Node* node)
	{
		if (node->getLeft())
			flipColor(node->getLeft());
		if (node->getRight())
			flipColor(node->getRight());
		flipColor(node);
	}

	bool isRed(Node* node) const
	{
		if (node == NULL || node->getColor() != Red)
			return false;
		return true;
	}

	bool isBlack(Node* node) const
	{
		if (node == NULL || node->getColor() == Black)
			return true;
		return false;
	}

	bool isDoubleBlack(Node* node) const
	{
		if (isRed(node) || isBlack(node))
			return false;
		return true;
	}

	bool isLeftChild(Node* node, Node* parent) const
	{
		if (parent->getLeft() == node)
			return true;
		return false;
	}

	bool hasTwoChildren(Node* parent) const
	{
		if (parent == NULL || (!parent->getLeft() || !parent->getRight()))
			return false;
		return true;
	}

	Node* insert(Node* node, const value_type& value, bool& inserted)
	{
		if (node == NULL)
		{
			inserted = true;
			return (createNode(value));
		}

		if (isRed(node->getLeft()) && isRed(node->getRight()))
			splitNode(node);
		if (this->compareValue(node->getValue(), value))
			node->setRight(insert(node->getRight(), value, inserted));
		else if (this->compareValue(value, node->getValue()))
			node->setLeft(insert(node->getLeft(), value, inserted));
		node->setRank(this->calculateRankFrom(node));
		return balance(node);
	}

	Node* erase(Node* node, const value_type& value, Node* parent, bool& erased)
	{
		if (node == NULL)
		{
			erased = false;
			return NULL;
		}

		if (this->compareValue(node->getValue(), value))
			node->setRight(erase(node->getRight(), value, node, erased));
		else if (this->compareValue(value, node->getValue()))
			node->setLeft(erase(node->getLeft(), value, node, erased));
		else
			return eraseImpl(node, parent);
		node = fixup(node);
		node->setRank(this->calculateRankFrom(node));
		return node;
	}

	Node* eraseImpl(Node* node, Node* parent)
	{
		if (!hasTwoChildren(node))
			return eraseImplOneOrNoChild(node, parent);
		return eraseImplTwoChildren(node, parent);
	}

	Node* postEraseImpl(Node* node, const value_type& value, Node* parent)
	{
		if ((!this->compareValue(node->getValue(), value) &&
			 !this->compareValue(value, node->getValue())) ||
			isNil(node))
		{
			return node;
		}
		node->setLeft(postEraseImpl(node->getLeft(), value, parent));
		node = fixup(node);
		node->setRank(this->calculateRankFrom(node));
		return node;
	}

	Node* fixup(Node* node)
	{
		Node* toFixChild = NULL;

		if (isDoubleBlack(node->getLeft()))
			toFixChild = node->getLeft();
		else if (isDoubleBlack(node->getRight()))
			toFixChild = node->getRight();
		if (toFixChild)
		{
			if (canTransfer(toFixChild, node))
				return (transferNode(toFixChild, node));
			return (fusionNode(toFixChild, node));
		}

		if (isNil(node->getLeft()))
		{
			deleteNode(node->getLeft());
			node->setLeft(NULL);
		}
		else if (isNil(node->getRight()))
		{
			deleteNode(node->getRight());
			node->setRight(NULL);
		}
		return node;
	}

	Node* balance(Node* node)
	{
		Node* tmp = node;

		if (isRed(node->getLeft()))
		{
			if (isRed(node->getLeft()->getRight()))
				node->setLeft(rotateLeft(node->getLeft()));  // LR
			if (isRed(node->getLeft()->getLeft()))
				tmp = rotateRight(node);  // LL
		}
		else if (isRed(node->getRight()))
		{
			if (isRed(node->getRight()->getLeft()))
				node->setRight(rotateRight(node->getRight()));  // RL
			if (isRed(node->getRight()->getRight()))
				tmp = rotateLeft(node);  // RR
		}
		return tmp;
	}

	Node* rotateLeft(Node* node)
	{
		Node* tmp = node->getRight();

		node->setRight(tmp->getLeft());

		tmp->setLeft(node);
		tmp->setColor(node->getColor());
		tmp->setRank(node->getRank());

		node->setRank(this->calculateRankFrom(node));
		node->setColor(Red);
		return tmp;
	}

	Node* rotateRight(Node* node)
	{
		Node* tmp = node->getLeft();

		node->setLeft(tmp->getRight());

		tmp->setRight(node);
		tmp->setColor(node->getColor());
		tmp->setRank(node->getRank());

		node->setRank(this->calculateRankFrom(node));
		node->setColor(Red);
		return tmp;
	}

	bool canTransfer(Node* node, Node* parent)
	{
		if (isCase4(node, parent) || isCase5(node, parent))
			return true;
		return false;
	}

	Node* transferNode(Node* node, Node* parent)
	{
		if (isCase4(node, parent))
			doCase4(node, parent);
		return doCase5(node, parent);
	}

	Node* fusionNode(Node* node, Node* parent)
	{
		Node* sibling = getSiblingNode(node, parent);

		if (isCase2(node, parent))
			return doCase2(node, parent, sibling);

		if (isCase1(node, parent))
			return doCase1(node, parent, sibling);
		return doCase3(node, parent, sibling);
	}

	bool isLeafNode(Node* node) const
	{
		if (!node->getLeft() && !node->getRight())
			return true;
		return false;
	}

	Node* getSiblingNode(Node* node, Node* parent) const
	{
		Node* sibling;

		if (isLeftChild(node, parent))
			sibling = parent->getRight();
		else
			sibling = parent->getLeft();
		return sibling;
	}

	Node* getNearNephewNode(Node* sibling, Node* parent) const
	{
		if (isLeftChild(sibling, parent))
			return sibling->getRight();
		return sibling->getLeft();
	}
	Node* getFarNephewNode(Node* sibling, Node* parent) const
	{
		if (isLeftChild(sibling, parent))
			return sibling->getLeft();
		return sibling->getRight();
	}

	Node* getMinParentNodeFrom(Node* node) const
	{
		if (!node->getLeft())
			return NULL;  // 노드 바로 아래가 minNode일 경우
		if (node->getLeft()->getLeft() == NULL)
			return node;
		return (getMinParentNodeFrom(node->getLeft()));
	}

	Node* getMinChildNodeFrom(Node* node) const
	{
		if (!node->getLeft())
			return node->getRight();
		return (getMinChildNodeFrom(node->getLeft()));
	}

	Node* createNode(const value_type& value = value_type())
	{
		Node* newNode = rb_node_allocator_.allocate(1);
		rb_node_allocator_.construct(newNode, node_value_type(value));
		return newNode;
	}
	Node* createNode(const Node& other)
	{
		Node* newNode = rb_node_allocator_.allocate(1);
		rb_node_allocator_.construct(newNode, node_value_type(other));
		return newNode;
	}

	void deleteNode(Node* node)
	{
		rb_node_allocator_.destroy(node);
		rb_node_allocator_.deallocate(node, 1);
	}

	void deleteAllNodes(Node* node)
	{
		if (node == NULL)
			return;
		deleteAllNodes(node->getRight());
		deleteAllNodes(node->getLeft());
		rb_node_allocator_.destroy(node);
		rb_node_allocator_.deallocate(node, 1);
	}

protected:
	/**
	 * @description: map range constructor에서 최적화 할 때 사용됨.
	 * 정렬된 데이터를 map으로 옮길 때, 어디에 넣을지에 대한 결정을 미리 할 수 있기에
	 * 전체 삽입 시간을 O(N)에 맞출 수 있다.
	 */
	Node* insert(Node* node, const value_type& value, const Color& color)  // node know where itself insert into
	{
		if (node == NULL)
		{
			Node* newNode = createNode(value);
			newNode->setColor(color);
			return (newNode);
		}
		if (this->compareValue(node->getValue(), value))
			node->setRight(insert(node->getRight(), value, color));
		else if (this->compareValue(value, node->getValue()))
			node->setLeft(insert(node->getLeft(), value, color));
		node->setRank(this->calculateRankFrom(node));
		return node;
	}

private:
	Node* createNilNode(void)
	{
		Node* newNode = rb_node_allocator_.allocate(1);
		rb_node_allocator_.construct(newNode, node_value_type(value_type()));
		newNode->setNil(true);
		newNode->setColor(Black);
		return newNode;
	}

	/**
	 * @param myChild: my child
	 * @param me: parent of myChild
	 * @param otherChild: to be my child soon
	 */
	void changeMyChild(Node* myChild, Node* me, Node* otherChild)
	{
		if (isLeftChild(myChild, me))
			me->setLeft(otherChild);
		else
			me->setRight(otherChild);
	}

	Node* eraseImplOneOrNoChild(Node* node, Node* parent)
	{
		Node* toDel;
		Node* toDelChild;
		Node* toDelParent;

		// set toDel, toDelChild, toDelParent
		toDel = node;
		toDelChild = toDel->getLeft() ? toDel->getLeft() : toDel->getRight();
		if (!toDelChild)
			toDelChild = createNilNode();
		toDelParent = parent;

		// disconnect toDel node
		changeMyChild(toDel, toDelParent, toDelChild);

		// set toDelChild's color
		if (!isRed(toDel) && !isRed(toDelChild))
			toDelChild->setColor(DoubleBlack);
		else if (isRed(toDel) && !isRed(toDelChild))
			toDelChild->setColor(Black);
		else if (!isRed(toDel) && isRed(toDelChild))
			toDelChild->setColor(Black);

		// free node
		deleteNode(toDel);
		return (toDelChild);
	}

	Node* eraseImplTwoChildren(Node* node, Node* parent)
	{
		Node* toDel;
		Node* toDelChild;
		Node* toDelParent;

		// set toDel, toDelChild, toDelParent
		toDel = this->getMinNodeFrom(node->getRight());
		toDelChild = getMinChildNodeFrom(node->getRight());
		if (!toDelChild)
			toDelChild = createNilNode();
		toDelParent = node;
		if (toDel != node->getRight())
			toDelParent = getMinParentNodeFrom(node->getRight());
		changeMyChild(toDel, toDelParent, toDelChild);

		// set toDelChild's color
		if (!isRed(toDel) && !isRed(toDelChild))
			toDelChild->setColor(DoubleBlack);
		else if (isRed(toDel) && !isRed(toDelChild))
			toDelChild->setColor(Black);
		else if (!isRed(toDel) && isRed(toDelChild))
			toDelChild->setColor(Black);

		// disconnect toDel node
		toDel->setLeft(node->getLeft());
		toDel->setRight(node->getRight());
		toDel->setColor(node->getColor());
		changeMyChild(node, parent, toDel);

		// free node
		deleteNode(node);
		node = toDel;

		// fix
		node->setRight(postEraseImpl(node->getRight(), toDelChild->getValue(), node));
		node = fixup(node);
		node->setRank(this->calculateRankFrom(node));
		return node;
	}

	bool isCase1(Node* node, Node* parent)
	{
		if (isRed(getSiblingNode(node, parent)))
			return true;
		return false;
	}

	bool isCase2(Node* node, Node* parent)
	{
		Node* sibling = getSiblingNode(node, parent);
		if (!isRed(parent) &&
			!isRed(sibling) &&
			!isRed(getNearNephewNode(sibling, parent)) &&
			!isRed(getFarNephewNode(sibling, parent)))
			return true;
		return false;
	}

	bool isCase3(Node* node, Node* parent)
	{
		Node* sibling = getSiblingNode(node, parent);
		if (isRed(parent) &&
			!isRed(sibling) &&
			!isRed(getNearNephewNode(sibling, parent)) &&
			!isRed(getFarNephewNode(sibling, parent)))
			return true;
		return false;
	}

	bool isCase4(Node* node, Node* parent) const
	{
		Node* sibling = getSiblingNode(node, parent);
		if (isRed(getNearNephewNode(sibling, parent)) &&
			!isRed(getFarNephewNode(sibling, parent)))
			return true;
		return false;
	}

	bool isCase5(Node* node, Node* parent) const
	{
		if (isRed(getFarNephewNode(getSiblingNode(node, parent), parent)))
			return true;
		return false;
	}

	Node* doCase1(Node* node, Node* parent, Node* sibling)
	{
		Node* grandParent;
		bool isLeft;

		isLeft = isLeftChild(node, parent);
		if (isLeft)
			grandParent = rotateLeft(parent);
		else
			grandParent = rotateRight(parent);
		sibling = getSiblingNode(node, parent);
		if (canTransfer(node, parent))
			parent = transferNode(node, parent);
		else
			parent = doCase3(node, parent, sibling);
		if (isLeft)
			grandParent->setLeft(parent);
		else
			grandParent->setRight(parent);
		return grandParent;
	}

	Node* doCase2(Node* node, Node* parent, Node* sibling)
	{
		parent->setColor(DoubleBlack);
		sibling->setColor(Red);
		node->setColor(Black);
		if (isNil(node))
		{
			if (isLeftChild(node, parent))
				parent->setLeft(NULL);
			else
				parent->setRight(NULL);
			deleteNode(node);
		}
		return parent;
	}

	Node* doCase3(Node* node, Node* parent, Node* sibling)
	{
		parent->setColor(Black);
		sibling->setColor(Red);
		node->setColor(Black);
		if (isNil(node))
		{
			deleteNode(node);
			if (isLeftChild(node, parent))
				parent->setLeft(NULL);
			else
				parent->setRight(NULL);
			parent->setRank(this->calculateRankFrom(parent));
		}
		return parent;
	}

	Node* doCase4(Node* node, Node* parent)
	{
		Node* sibling = getSiblingNode(node, parent);

		if (isLeftChild(node, parent))
		{
			sibling = rotateRight(sibling);
			parent->setRight(sibling);
		}
		else
		{
			sibling = rotateLeft(sibling);
			parent->setLeft(sibling);
		}
		return sibling;
	}

	Node* doCase5(Node* node, Node* parent)
	{
		Node* grandParent;
		bool isLeft;

		isLeft = isLeftChild(node, parent);
		if (isLeft)
			grandParent = rotateLeft(parent);
		else
			grandParent = rotateRight(parent);
		parent->setColor(Black);
		getSiblingNode(parent, grandParent)->setColor(Black);
		node->setColor(Black);  // double black에서 black으로 완화시키기
		if (isNil(node))
		{
			deleteNode(node);
			if (isLeft)
				parent->setLeft(NULL);
			else
				parent->setRight(NULL);
			parent->setRank(this->calculateRankFrom(parent));
		}
		grandParent->setRank(this->calculateRankFrom(grandParent));
		return grandParent;
	}
};

#endif
