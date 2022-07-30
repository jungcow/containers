#ifndef __FT_SET_H__
#define __FT_SET_H__

#include <climits>
#include <functional>  // std::less
#include <memory>      // std::allocator

#include "__tree__.hpp"
#include "algorithm.hpp"  // ft::equal, ft::lexicographical_compare
#include "iterator.hpp"   // ft::reverse_iterator
#include "utility.hpp"    // ft::pair

namespace ft
{
	template <class Iterator, class TreePointer, class VIter, class VPointer>
	class set_iterator;

	template <class T,                         // set::key_type/value_type
			  class Compare = std::less<T>,    // set::key_compare/value_compare
			  class Alloc = std::allocator<T>  // set::allocator_type
			  >
	class set
	{
	private:
		template <class Iterator, class TreePointer, class VIter, class VPointer>
		friend class ft::set_iterator;

	public:
		/**
		 * Set Inner Type
		 */
		typedef T key_type;
		typedef T value_type;

		typedef Compare key_compare;
		typedef Compare value_compare;

		typedef Alloc allocator_type;

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

		typedef typename ft::node::NodeWrapper<node_base>::RBNode node_type;
		typedef typename ft::node::NodeWrapper<const_node_base>::RBNode const_node_type;

		typedef typename ft::node::NodeWrapper<node_base>::node_allocator_type node_allocator_type;
		typedef typename ft::node::NodeWrapper<node_base>::node_size_type node_size_type;
		typedef typename ft::node::NodeWrapper<node_base>::node_value_type node_value_type;

		typedef typename ft::node::NodeWrapper<node_base>::BalanceNode balance_node_type;
		typedef typename ft::node::NodeWrapper<const_node_base>::BalanceNode const_balance_node_type;

		typedef ft::Tree<node_type> set_tree;
		typedef typename node_allocator_type::template rebind<set_tree>::other tree_allocator_type;

		typedef typename node_type::iterator node_iterator;
		typedef typename const_node_type::iterator const_node_iterator;

	public:
		/**
		 * iterator type
		 */
		typedef set_iterator<node_iterator, set_tree*, pointer, pointer> iterator;
		typedef set_iterator<const_node_iterator, set_tree*, const_pointer, pointer> const_iterator;

		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

		typedef typename allocator_type::size_type size_type;
		typedef typename ft::iterator_traits<iterator>::difference_type difference_type;

	private:
		set_tree* data_;
		key_compare compare_key_;
		value_compare compare_value_;
		allocator_type allocator_;
		tree_allocator_type tree_allocator_;

	public:
		explicit set(const key_compare& comp = key_compare(),
					 const allocator_type& alloc = allocator_type())
			: data_(NULL),
			  compare_key_(comp),
			  compare_value_(value_compare(compare_key_)),
			  allocator_(alloc),
			  tree_allocator_(tree_allocator_type())
		{
			data_ = tree_allocator_.allocate(1);
			tree_allocator_.construct(data_);
		}

		template <class InputIterator>
		set(InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(),
			const allocator_type& alloc = allocator_type())
			: data_(NULL),
			  compare_key_(comp),
			  compare_value_(value_compare(comp)),
			  allocator_(alloc),
			  tree_allocator_(tree_allocator_type())
		{
			data_ = tree_allocator_.allocate(1);
			tree_allocator_.construct(data_);
			for (; first != last; first++)
				data_->insert(*first);
		}

		set(const set& x)
			: data_(NULL),
			  compare_key_(x.compare_key_),
			  compare_value_(value_compare(x.compare_key_)),
			  allocator_(x.allocator_),
			  tree_allocator_(x.tree_allocator_)
		{
			data_ = tree_allocator_.allocate(1);
			tree_allocator_.construct(data_, *x.data_);
		}

		set& operator=(const set& x)
		{
			*data_ = *(x.data_);
			return (*this);
		}

		~set()
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

		size_type count(const value_type& val) const
		{
			if (data_->find(val).first)
				return static_cast<size_type>(true);
			return static_cast<size_type>(false);
		}

		bool empty() const
		{
			return data_->empty();
		}
		iterator end()
		{
			return iterator(data_->getEndNode(), data_);
		}
		const_iterator end() const
		{
			return const_iterator(iterator(data_->getEndNode(), data_));
		}

		ft::pair<iterator, iterator> equal_range(const value_type& val) const
		{
			return ft::pair<iterator, iterator>(this->lower_bound(val), this->upper_bound(val));
		}

		void erase(iterator position)
		{
			data_->erase(*position);
		}
		size_type erase(const value_type& val)
		{
			return static_cast<size_type>(data_->erase(val));
		}
		void erase(iterator first, iterator last)
		{
			while (first != last)
			{
				data_->erase(*first++);
			}
		}

		iterator find(const value_type& val) const
		{
			balance_node_type* node = data_->find(val).first;

			if (node == NULL)
				return iterator(data_->getEndNode(), data_);
			return iterator(node, data_);
		}

		allocator_type get_allocator() const
		{
			return allocator_;
		}

		ft::pair<iterator, bool> insert(const value_type& val)
		{
			ft::pair<balance_node_type*, bool> result;

			result = data_->insert(val);
			return ft::pair<iterator, bool>(iterator(result.first, data_), result.second);
		}

		iterator insert(iterator position, const value_type& val)
		{
			ft::pair<balance_node_type*, bool> result;

			// TODO: hint 최적화
			//  iterator tmp = position;
			//  if (position == end())
			//  	--position;
			//  if (compare_value_(*position, val))
			//  {
			//  	++tmp;
			//  	if (tmp == end() || compare_value_(val, *tmp))
			//  		result = data_->insert(position.base().base(), val);
			//  	else
			//  		result = data_->insert(val);
			//  }
			//  else if (compare_value_(val, *position))
			//  {
			//  	if (position == begin())
			//  		result = data_->insert(position.base().base(), val);
			//  	if (compare_value_(*(--tmp), val))
			//  		result = data_->insert(position.base().base(), val);
			//  	else
			//  		result = data_->insert(val);
			//  }
			//  else
			//  	return position;
			iterator dummy = position;  // position 사용 안함

			result = data_->insert(val);
			return iterator(result.first, data_);
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				data_->insert(*first++);
			}
		}

		key_compare key_comp() const
		{
			return compare_key_;
		}

		iterator lower_bound(const value_type& val) const
		{
			balance_node_type* arrived = data_->find(val).second;
			if (this->compare_value_(arrived->getValue(), val))
				return ++iterator(arrived, data_);
			return iterator(arrived, data_);
		}

		size_type max_size() const throw()
		{
			return (std::numeric_limits<node_size_type>::max() /
					std::max(2, static_cast<int>(sizeof(node_value_type))));
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

		void swap(set& x)
		{
			set_tree* mt;

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

		iterator upper_bound(const value_type& val) const
		{
			balance_node_type* arrived = data_->find(val).second;
			if (this->compare_key_(val, arrived->getValue()))
				return iterator(arrived, data_);
			return ++iterator(arrived, data_);
		}

		value_compare value_comp() const
		{
			return (compare_value_);
		}

		template <class V, class Comp, class A>
		friend bool operator==(const set<V, Comp, A>& lhs,
							   const set<V, Comp, A>& rhs);
		template <class V, class Comp, class A>
		friend bool operator!=(const set<V, Comp, A>& lhs,
							   const set<V, Comp, A>& rhs);
		template <class V, class Comp, class A>
		friend bool operator<(const set<V, Comp, A>& lhs,
							  const set<V, Comp, A>& rhs);
		template <class V, class Comp, class A>
		friend bool operator<=(const set<V, Comp, A>& lhs,
							   const set<V, Comp, A>& rhs);
		template <class V, class Comp, class A>
		friend bool operator>(const set<V, Comp, A>& lhs,
							  const set<V, Comp, A>& rhs);
		template <class V, class Comp, class A>
		friend bool operator>=(const set<V, Comp, A>& lhs,
							   const set<V, Comp, A>& rhs);

		template <class V, class Comp, class A>
		friend void swap(set<V, Comp, A>& x, set<V, Comp, A>& y);

#if DEBUG
	public:
		void print() const
		{
			data_->printByInOrderTraversal();
		}
#endif
	};

	template <class V, class Comp, class A>
	bool operator==(const set<V, Comp, A>& lhs,
					const set<V, Comp, A>& rhs)
	{
		return (lhs.size() == rhs.size() &&
				ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}
	template <class V, class Comp, class A>
	bool operator!=(const set<V, Comp, A>& lhs,
					const set<V, Comp, A>& rhs)
	{
		return !(lhs == rhs);
	}
	template <class V, class Comp, class A>
	bool operator<(const set<V, Comp, A>& lhs,
				   const set<V, Comp, A>& rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}
	template <class V, class Comp, class A>
	bool operator<=(const set<V, Comp, A>& lhs,
					const set<V, Comp, A>& rhs)
	{
		return !(rhs < lhs);
	}
	template <class V, class Comp, class A>
	bool operator>(const set<V, Comp, A>& lhs,
				   const set<V, Comp, A>& rhs)
	{
		return (rhs < lhs);
	}
	template <class V, class Comp, class A>
	bool operator>=(const set<V, Comp, A>& lhs,
					const set<V, Comp, A>& rhs)
	{
		return !(lhs < rhs);
	}
	template <class V, class Comp, class A>
	void swap(set<V, Comp, A>& x, set<V, Comp, A>& y)
	{
		x.swap(y);
	}

	/**
	 * @class set_iterator
	 *
	 * @param Iterator: node iterator
	 * @param VIter: value iterator, here is T*
	 * @param VPointer: value pointer, used to be compatible with const pointer
	 */
	template <class Iterator, class TreePointer, class VIter, class VPointer>
	class set_iterator
	{
	private:
		typedef typename Iterator::node_value_type node_value_type;
		typedef typename Iterator::node_pointer node_pointer;
		typedef typename Iterator::node_reference node_reference;
		typedef typename Iterator::node_difference_type node_difference_type;
		typedef typename Iterator::node_iterator_category node_iterator_category;

		typedef typename node_value_type::node_size_type node_size_type;

	public:
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::iterator_category iterator_category;

	private:
		Iterator base_;  // Iterator: node_iterator
		TreePointer tree_;

	public:
		set_iterator()
		{
		}

		template <class Iter, class Tp, class I>
		set_iterator(const set_iterator<
					 Iter, Tp, I, typename ft::enable_if<ft::is_same<I, VPointer>::value, VPointer>::type>& other)
			: base_(other.base()), tree_(other.tree())
		{
		}

		explicit set_iterator(const Iterator& otherIter, TreePointer tree)  // node_iterator로 생성
			: base_(otherIter), tree_(tree)
		{
		}

		explicit set_iterator(node_pointer node, TreePointer tree)  // node_iterator로 생성
			: base_(node), tree_(tree)
		{
		}

		set_iterator& operator=(const set_iterator& other)
		{
			base_ = other.base_;
			tree_ = other.tree_;
			return (*this);
		}

		~set_iterator() {}

		Iterator base(void) const
		{
			return base_;
		}
		TreePointer tree(void) const
		{
			return tree_;
		}

		const reference operator*() const
		{
			return (*base_);
		}

		const reference operator*()
		{
			return (*base_);
		}

		pointer operator->() const
		{
			return (base_.operator->());
		}

		set_iterator& operator++()
		{
			node_size_type order;

			if (base_.base() == reinterpret_cast<node_pointer>(tree_->getEndNode()))
				order = tree_->size() + 1;
			else
				order = tree_->getOrder(*base_);
			base_ = Iterator(reinterpret_cast<node_pointer>(tree_->OS_Select(tree_->getEndNode(), ++order)));
			return (*this);
		}
		set_iterator operator++(int)
		{
			set_iterator tmp = *this;
			++(*this);
			return (tmp);
		}
		set_iterator& operator--()
		{
			node_size_type order;

			if (base_.base() == reinterpret_cast<node_pointer>(tree_->getEndNode()))
				order = tree_->size() + 1;
			else
				order = tree_->getOrder(*base_);
			base_ = Iterator(reinterpret_cast<node_pointer>(tree_->OS_Select(tree_->getEndNode(), --order)));
			return (*this);
		}
		set_iterator operator--(int)
		{
			set_iterator tmp = *this;
			--(*this);
			return (tmp);
		}

		template <class Iter, class Tp, class Vi, class Vp>
		friend bool operator==(const set_iterator<Iter, Tp, Vi, Vp>& lhs, const set_iterator<Iter, Tp, Vi, Vp>& rhs);
		template <class Iter, class Tp, class Vi, class Vp>
		friend bool operator!=(const set_iterator<Iter, Tp, Vi, Vp>& lhs, const set_iterator<Iter, Tp, Vi, Vp>& rhs);
	};
	template <class Iter, class Tp, class Vi, class Vp>
	bool operator==(const set_iterator<Iter, Tp, Vi, Vp>& lhs, const set_iterator<Iter, Tp, Vi, Vp>& rhs)
	{
		return (lhs.base_ == rhs.base_);
	}
	template <class Iter, class Tp, class Vi, class Vp>
	bool operator!=(const set_iterator<Iter, Tp, Vi, Vp>& lhs, const set_iterator<Iter, Tp, Vi, Vp>& rhs)
	{
		return !(lhs == rhs);
	}
};

#endif