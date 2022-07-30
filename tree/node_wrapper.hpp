#ifndef __FT_TREE_H__
#define __FT_TREE_H__

#include "node.hpp" // ft::node::Node

namespace ft
{
	namespace node
	{
		template <class N>
		struct NodeWrapper
		{
			// class AVLBalance;
			// class LLRBBalance;

		private:
			template <class Balance>
			struct TNode
			{
				typedef ft::node::Node<N, Balance> type;
			};

			class RBBalance;

		public:
			typedef typename TNode<RBBalance>::type RBNode;
			typedef typename RBNode::node_allocator_type node_allocator_type;
			// typedef typename TNode<AVLBalance>::type AVLNode;

			typedef typename node_allocator_type::value_type node_value_type;
			typedef typename node_allocator_type::size_type node_size_type;
			typedef typename node_allocator_type::pointer node_pointer;
			typedef typename node_allocator_type::reference node_reference;
			typedef typename node_allocator_type::const_pointer node_const_pointer;
			typedef typename node_allocator_type::const_reference node_const_reference;

			typedef RBBalance BalanceNode;
		};
	}
}

#endif
