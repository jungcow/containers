#ifndef __FT_MAP_H__
#define __FT_MAP_H__

#include <cstddef>     // ptrdiff_t
#include <functional>  // std::less, std::binary_function

#include "algorithm.hpp"  // ft::equal, ft::lexicographical_compare
#include "iterator.hpp"   // ft::reverse_iterator
#include "tree/node_base.hpp"
#include "tree/node_wrapper.hpp"
#include "tree/tree.hpp"
#include "type.hpp"     // TODO: type 출력, 디버깅용
#include "utility.hpp"  // ft::pair

namespace ft
{
	template <class Iterator, class NodePointer, class TreePointer, class VIter, class VPointer>
	class map_iterator;

	template <class Key,
			  class T,
			  class Compare = std::less<Key>,
			  class Alloc = std::allocator<ft::pair<const Key, T> > >
	class map
	{
	private:
		template <class Iterator, class NodePointer, class TreePointer, class VIter, class VPointer>
		friend class ft::map_iterator;

	public:
		/**
		 * Map Inner Type
		 */
		typedef Key key_type;
		typedef T mapped_type;

		typedef Compare key_compare;
		class value_compare;

		typedef Alloc allocator_type;

		typedef typename allocator_type::size_type size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef typename allocator_type::value_type value_type;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef typename allocator_type::const_reference const_reference;

	private:
		/**
		 * Node and Tree type
		 */
		typedef ft::node::NodeBase<pointer, pointer, value_compare, allocator_type> node_base;
		typedef ft::node::NodeBase<const_pointer, pointer, value_compare, allocator_type> const_node_base;

		typedef typename ft::NodeWrapper<node_base>::RBNode node_type;
		typedef typename ft::NodeWrapper<const_node_base>::RBNode const_node_type;

		typedef typename ft::NodeWrapper<node_base>::node_allocator_type node_allocator_type;
		typedef typename ft::NodeWrapper<node_base>::node_size_type node_size_type;
		typedef typename ft::NodeWrapper<node_base>::node_value_type node_value_type;

		typedef typename ft::NodeWrapper<node_base>::BalanceNode balance_node_type;
		typedef typename ft::NodeWrapper<const_node_base>::BalanceNode const_balance_node_type;

		typedef ft::Tree<node_type> map_tree;
		typedef typename node_allocator_type::template rebind<map_tree>::other tree_allocator_type;

		typedef typename node_type::iterator node_iterator;
		typedef typename const_node_type::iterator const_node_iterator;

	public:
		/**
		 * iterator type
		 */
		typedef map_iterator<node_iterator, balance_node_type*, map_tree*, pointer, pointer> iterator;
		typedef map_iterator<const_node_iterator, const_balance_node_type*, map_tree*, const_pointer, pointer> const_iterator;

		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

	private:
		map_tree* data_;
		key_compare compare_key_;
		value_compare compare_value_;
		allocator_type allocator_;
		tree_allocator_type tree_allocator_;

	public:
		explicit map(const key_compare& comp = key_compare(),
					 const allocator_type& alloc = allocator_type())
			: compare_key_(comp),
			  compare_value_(value_compare(compare_key_)),
			  allocator_(alloc),
			  tree_allocator_(tree_allocator_type())
		{
			data_ = tree_allocator_.allocate(1);
			tree_allocator_.construct(data_);
		}

#if 0
		template <class InputIterator>
		map(InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(),
			const allocator_type& alloc = allocator_type())
		{
			for (; first != last; first++)
				data_->insert(*first);
		}

		map(const map& x)
		{
			// level tranverse
		}
#endif
		~map()
		{
			tree_allocator_.destroy(data_);
			tree_allocator_.deallocate(data_, 1);
		}

		iterator begin()
		{
			return iterator(data_->getFirst(), data_);
		}
		const_iterator begin() const
		{
			return const_iterator(iterator(data_->getFirst(), data_));
		}

		void clear()
		{
			erase(begin(), end());
		}

		size_type count(const key_type& k) const
		{
			if (data_->find(ft::make_pair(k, mapped_type())))
				return static_cast<size_type>(true);
			return static_cast<size_type>(false);
		}

		bool empty() const
		{
			return data_->empty();
		}
		iterator end()
		{
			return iterator(data_->getFoot(), data_);
		}
		const_iterator end() const
		{
			return const_iterator(iterator(data_->getFoot(), data_));
		}

		ft::pair<const_iterator, const_iterator> equal_range(const key_type& k) const
		{
			return ft::pair<const_iterator, const_iterator>(this->lower_bound(k), this->upper_bound(k));
		}
		ft::pair<iterator, iterator> equal_range(const key_type& k)
		{
			return ft::pair<iterator, iterator>(this->lower_bound(k), this->upper_bound(k));
		}

		void erase(iterator position)
		{
			data_->erase(*position);
		}

		size_type erase(const key_type& k)
		{
			return static_cast<size_type>(data_->erase(ft::make_pair(k, mapped_type())));
		}
		void erase(iterator first, iterator last)
		{
			--last;
			for (; last != first; last--)
			{
				data_->erase(*last);
			}
		}

		iterator find(const key_type& k)
		{
			balance_node_type* node = data_->find(value_type(k, mapped_type()));

			if (node == NULL)
				return end();
			return iterator(node, data_);
		}
		const_iterator find(const key_type& k) const
		{
			const_balance_node_type* node = data_->find(value_type(k, mapped_type()));

			if (node == NULL)
				return end();
			return const_iterator(node, data_);
		}

		allocator_type get_allocator() const
		{
			return allocator_;
		}

		ft::pair<iterator, bool> insert(const value_type& val)
		{
			bool inserted;

			inserted = data_->insert(val);
			return ft::pair<iterator, bool>(iterator(data_->find(val), data_), inserted);
		}

		iterator insert(iterator position, const value_type& val)
		{
			iterator tmp = position;
			balance_node_type* node;

			if (position == end())
				--position;
			if (compare_value_(*position, val))
			{
				++tmp;
				if (tmp == end() || compare_value_(val, *tmp))
					data_->insert(position.base().base(), val);
				else
					data_->insert(val);
			}
			else if (compare_value_(val, *position))
			{
				if (position == begin())
					data_->insert(position.base().base(), val);
				if (compare_value_(*(--tmp), val))
					data_->insert(position.base().base(), val);
				else
					data_->insert(val);
			}
			else
				return position;
			node = data_->find(val);
			if (!node)
				return end();
			return iterator(node, data_);
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			for (; first != last; first++)
			{
				data_->insert(*first);
			}
		}

		key_compare key_comp() const
		{
			return compare_key_;
		}

		iterator lower_bound(const key_type& k)
		{
			balance_node_type* node = data_->find(value_type(k, mapped_type()));
			if (!node)
				return end();
			return iterator(node, data_);
		}
		const_iterator lower_bound(const key_type& k) const
		{
			const_balance_node_type* node = data_->find(value_type(k, mapped_type()));
			if (!node)
				return end();
			return const_iterator(node, data_);
		}

		size_type max_size() const throw()
		{
			return (std::numeric_limits<node_size_type>::max() /
					std::max(2, static_cast<int>(sizeof(node_value_type))));
		}

		// map& operator=(const map& x)
		// {
		// 	std::allocator<node_size_type> alloc;
		// 	node_size_type* i = alloc.allocate(x.size());
		// 	while ()
		// }

		mapped_type& operator[](const key_type& k)
		{
			return (*((this->insert(make_pair(k, mapped_type()))).first)).second;
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		size_type size() const
		{
			return data_->size();
		}

		void print() const
		{
			data_->printByInOrderTraversal();
		}

		void swap(map& x)
		{
			map_tree* mt;

			mt = data_;
			data_ = x.data_;
			x.data_ = mt;

			allocator_type at = allocator_;
			allocator_ = x.allocator_;
			x.allocator_ = at;

			tree_allocator_type tat = tree_allocator_;
			tree_allocator_ = x.tree_allocator_;
			x.tree_allocator_ = tat;
		}

		iterator upper_bound(const key_type& k)
		{
			balance_node_type* node = data_->find(value_type(k, mapped_type()));
			if (!node)
				return end();
			return ++(iterator(node, data_));
		}
		const_iterator upper_bound(const key_type& k) const
		{
			const_balance_node_type* node = data_->find(value_type(k, mapped_type()));
			if (!node)
				return end();
			return ++(const_iterator(node, data_));
		}

		value_compare value_comp() const
		{
			return (value_compare(compare_key_));
		}
		template <class K, class V, class Comp, class A>
		friend bool operator==(const map<K, V, Comp, A>& lhs,
							   const map<K, V, Comp, A>& rhs);
		template <class K, class V, class Comp, class A>
		friend bool operator!=(const map<K, V, Comp, A>& lhs,
							   const map<K, V, Comp, A>& rhs);
		template <class K, class V, class Comp, class A>
		friend bool operator<(const map<K, V, Comp, A>& lhs,
							  const map<K, V, Comp, A>& rhs);
		template <class K, class V, class Comp, class A>
		friend bool operator<=(const map<K, V, Comp, A>& lhs,
							   const map<K, V, Comp, A>& rhs);
		template <class K, class V, class Comp, class A>
		friend bool operator>(const map<K, V, Comp, A>& lhs,
							  const map<K, V, Comp, A>& rhs);
		template <class K, class V, class Comp, class A>
		friend bool operator>=(const map<K, V, Comp, A>& lhs,
							   const map<K, V, Comp, A>& rhs);

		template <class K, class V, class Comp, class A>
		friend void swap(map<Key, T, Compare, Alloc>& x, map<K, V, Comp, A>& y);
	};

	template <class K, class V, class Comp, class A>
	bool operator==(const map<K, V, Comp, A>& lhs,
					const map<K, V, Comp, A>& rhs)
	{
		return (lhs.size() == rhs.size() &&
				ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}
	template <class K, class V, class Comp, class A>
	bool operator!=(const map<K, V, Comp, A>& lhs,
					const map<K, V, Comp, A>& rhs)
	{
		return !(lhs == rhs);
	}
	template <class K, class V, class Comp, class A>
	bool operator<(const map<K, V, Comp, A>& lhs,
				   const map<K, V, Comp, A>& rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}
	template <class K, class V, class Comp, class A>
	bool operator<=(const map<K, V, Comp, A>& lhs,
					const map<K, V, Comp, A>& rhs)
	{
		return !(rhs < lhs);
	}
	template <class K, class V, class Comp, class A>
	bool operator>(const map<K, V, Comp, A>& lhs,
				   const map<K, V, Comp, A>& rhs)
	{
		return (rhs < lhs);
	}
	template <class K, class V, class Comp, class A>
	bool operator>=(const map<K, V, Comp, A>& lhs,
					const map<K, V, Comp, A>& rhs)
	{
		return !(lhs < rhs);
	}
	template <class K, class V, class Comp, class A>
	void swap(map<K, V, Comp, A>& x, map<K, V, Comp, A>& y)
	{
		x.swap(y);
	}

	/**
	 * @class: value_compare
	 *
	 * @param Iterator: a key to find value
	 * @param T: a value, are paired with Key type
	 * @param Compare: class comparator for compare Keys
	 * @param Alloc: allocator for allocate value type which is ft::pair<Key, T> type
	 */
	template <class Key,
			  class T,
			  class Compare,
			  class Alloc>
	class map<Key, T, Compare, Alloc>::value_compare : public std::binary_function<value_type, value_type, bool>
	{
	private:
		friend class map;
		friend class ft::node::NodeBase<value_type*, value_type*, value_compare, Alloc>;

	public:
		bool operator()(const value_type& lhs, const value_type& rhs) const
		{
			return comp(lhs.first, rhs.first);
		}

	protected:
		Compare comp;
		value_compare(Compare c) : comp(c) {}
		value_compare() : comp(Compare()) {}
	};

	/**
	 * @class map_iterator
	 *
	 * @param Iterator: node iterator
	 * @param VIter: value iterator, here is ft::pair<Key, T>
	 * @param VPointer: value pointer, used to be compatible with const pointer
	 */
	template <class Iterator, class NodePointer, class TreePointer, class VIter, class VPointer>
	class map_iterator
	{
	private:
		typedef typename Iterator::inner_value_type inner_value_type;
		typedef typename Iterator::inner_pointer inner_pointer;
		typedef typename Iterator::inner_reference inner_reference;
		typedef typename Iterator::const_inner_pointer const_inner_pointer;
		typedef typename Iterator::const_inner_reference const_inner_reference;

	public:
		typedef typename Iterator::value_type value_type;
		typedef typename value_type::node_size_type size_type;

		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::iterator_category iterator_category;

	private:
		Iterator base_;  // Iterator: node_iterator
		TreePointer tree_;
		size_type order_;

	public:
		map_iterator() : order_(0)
		{
		}

		template <class Iter, class Np, class Tp, class I>
		map_iterator(const map_iterator<
					 Iter, Np, Tp, I, typename ft::enable_if<ft::is_same<I, VPointer>::value, VPointer>::type>& other)
			: base_(other.base()), tree_(other.tree()), order_(other.order())
		{
		}

		explicit map_iterator(const Iterator& otherIter, TreePointer tree)  // node_iterator로 생성
			: base_(otherIter), tree_(tree)
		{
			order_ = tree_.getOrder(*otherIter);
		}

		explicit map_iterator(NodePointer node, TreePointer tree)  // node_iterator로 생성
			: base_(node), tree_(tree), order_(0)
		{
			order_ = tree_->getOrder(*base_);
		}

		map_iterator& operator=(const map_iterator& other)
		{
			base_ = other.base_;
			tree_ = other.tree_;
			order_ = other.order_;
			return (*this);
		}

		~map_iterator() {}

		Iterator base(void) const
		{
			return base_;
		}
		TreePointer tree(void) const
		{
			return tree_;
		}
		size_type order(void) const
		{
			return order_;
		}

		const_inner_reference operator*() const
		{
			return (*base_);
		}

		inner_reference operator*()
		{
			return (*base_);
		}

		inner_pointer operator->() const
		{
			return (base_.operator->());
		}

		map_iterator& operator++()
		{
			base_ = Iterator(reinterpret_cast<NodePointer>(tree_->OS_Select(tree_->getRoot()->getLeft(), ++order_)));
			return (*this);
		}
		map_iterator operator++(int)
		{
			map_iterator tmp = *this;
			++(*this);
			return (tmp);
		}
		map_iterator& operator--()
		{
			base_ = Iterator(tree_->OS_Select(tree_->getRoot()->getLeft(), --order_));
			return (*this);
		}
		map_iterator operator--(int)
		{
			map_iterator tmp = *this;
			--(*this);
			return (tmp);
		}

		template <class Iter, class Np, class Tp, class Vi, class Vp>
		friend bool operator==(const map_iterator<Iter, Np, Tp, Vi, Vp>& lhs, const map_iterator<Iter, Np, Tp, Vi, Vp>& rhs);
		template <class Iter, class Np, class Tp, class Vi, class Vp>
		friend bool operator!=(const map_iterator<Iter, Np, Tp, Vi, Vp>& lhs, const map_iterator<Iter, Np, Tp, Vi, Vp>& rhs);
	};
	template <class Iter, class Np, class Tp, class Vi, class Vp>
	bool operator==(const map_iterator<Iter, Np, Tp, Vi, Vp>& lhs, const map_iterator<Iter, Np, Tp, Vi, Vp>& rhs)
	{
		return (lhs.base_ == rhs.base_);
	}
	template <class Iter, class Np, class Tp, class Vi, class Vp>
	bool operator!=(const map_iterator<Iter, Np, Tp, Vi, Vp>& lhs, const map_iterator<Iter, Np, Tp, Vi, Vp>& rhs)
	{
		return !(lhs == rhs);
	}
}

#endif
