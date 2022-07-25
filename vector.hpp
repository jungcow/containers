#ifndef __FT_VECTOR_H__
#define __FT_VECTOR_H__

#include <algorithm>  // std::min, std::max
#include <exception>  // bad_alloc, length_error, out_of_range
#include <iostream>   //temporary include TODO: 지우기
#include <iterator>   // std::distance
#include <memory>     // allocator

#include "algorithm.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"

namespace ft
{
	template <class Iter, class Pointer>
	class vector_iterator;

	template <typename T, class Alloc = std::allocator<T> >
	class vector
	{
	public:
		typedef Alloc allocator_type;

	public:
		typedef typename allocator_type::value_type value_type;
		typedef typename allocator_type::size_type size_type;

		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef value_type* pointer;
		typedef const value_type* const_pointer;

		typedef ft::vector_iterator<pointer, pointer> iterator;
		typedef ft::vector_iterator<const_pointer, pointer> const_iterator;

		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

	private:
		allocator_type allocator_;
		pointer data_;
		size_type size_;
		size_type capacity_;

	public:
		explicit vector(const allocator_type& alloc = allocator_type())
			: allocator_(alloc), data_(0), size_(0), capacity_(1)
		{
			if (capacity_ > max_size())
				throw std::bad_alloc();
			data_ = allocator_.allocate(capacity_);
		}

		explicit vector(size_type n, const value_type& val = value_type(),
						const allocator_type& alloc = allocator_type())
			: allocator_(alloc), data_(0), size_(n), capacity_(n + 1)
		{
			if (n > max_size())
				throw std::bad_alloc();
			data_ = allocator_.allocate(capacity_);
			constructRange(0, size_, val);
		}

		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
			   typename ft::check_type<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0)
			: allocator_(alloc), data_(0), size_(0), capacity_(0)
		{
			if (ft::is_same<std::input_iterator_tag,
							typename ft::iterator_traits<InputIterator>::iterator_category>::value)
			{
				size_ = 0;
				capacity_ = 1;
				data_ = allocator_.allocate(capacity_);
				for (; first != last; first++)
					push_back(*first);
				return;
			}
			size_type n = ft::distance(first, last);
			if (n > max_size())
				throw std::bad_alloc();
			size_ = n;
			capacity_ = n + 1;
			data_ = allocator_.allocate(capacity_);
			for (size_type i = 0; i < n; i++)
				allocator_.construct(&data_[i], *first++);
		}
		vector(const vector& x)
			: allocator_(x.allocator_), data_(0), size_(x.size_), capacity_(x.capacity_)
		{
			if (capacity_ > max_size())
				throw std::bad_alloc();
			data_ = allocator_.allocate(capacity_);
			for (size_type i = 0; i < size_; i++)
				allocator_.construct(&data_[i], x.data_[i]);
		}
		~vector()
		{
			// TODO: Trivial destructible 에 대해서 최적화 하기
			clear();
			allocator_.deallocate(data_, capacity_);
			size_ = 0;
			capacity_ = 0;
		}

	public:
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
					typename ft::check_type<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0)
		{
			if (ft::is_same<std::input_iterator_tag,
							typename ft::iterator_traits<InputIterator>::iterator_category>::value)
			{
				clear();
				for (; first != last; first++)
					push_back(*first);
				return;
			}
			size_type n = 0;
			n = ft::distance(first, last);
			if (capacity_ < n)
				reserve(n);
			clear();
			allocator_.deallocate(data_, capacity_);
			data_ = allocator_.allocate(capacity_, 0);
			for (size_type i = 0; i < n; i++)
				allocator_.construct(&data_[i], *first++);
			size_ = n;
		}

		void assign(size_type n, const value_type& val)
		{
			if (capacity_ < n)
				reserve(n);
			clear();
			allocator_.deallocate(data_, capacity_);
			data_ = allocator_.allocate(capacity_, 0);
			constructRange(0, n, val);
			size_ = n;
		}

		reference at(size_type n)
		{
			if (size_ <= n)
				throw std::out_of_range("at: out of range");
			return (data_[n]);
		}
		const_reference at(size_type n) const
		{
			if (size_ <= n)
				throw std::out_of_range("at: out of range");
			return (data_[n]);
		}

		reference back()
		{
			return (data_[size_ - 1]);
		}
		const_reference back() const
		{
			return (data_[size_ - 1]);
		}

		iterator begin()
		{
			return iterator(&data_[0]);
		}
		const_iterator begin() const
		{
			return const_iterator(&data_[0]);
		}

		size_type capacity() const
		{
			return (capacity_);
		}

		void clear()
		{
			if (!ft::is_trivial_destructible_junior<value_type>::value)
			{
				for (size_type i = 0; i < size_; i++)
					allocator_.destroy(&data_[i]);
			}
			size_ = 0;
		}

		bool empty() const
		{
			return (size_ == 0);
		}

		iterator end()
		{
			return iterator(&data_[size_]);
		}
		const_iterator end() const
		{
			return const_iterator(&data_[size_]);
		}

		iterator erase(iterator position)
		{
			iterator ret = position;
			moveFrontByInterval(position, end(), 1);
			destroyRange(size_ - 1, size_);
			size_--;
			return ret;
		}

		iterator erase(iterator first, iterator last)
		{
			size_type n = last - first;
			iterator finish = end();

			moveFrontByInterval(first, finish, n);
			destroyRange(first + (finish - last), finish);
			size_ -= n;
			return first;
		}

		reference front()
		{
			return (data_[0]);
		}
		const_reference front() const
		{
			return (data_[0]);
		}

		allocator_type get_allocator() const
		{
			return (allocator_type(allocator_));
		}

		iterator insert(iterator position, const value_type& val)
		{
			size_type pos = getOffsetFromEndTo(position);

			if (size_ + 1 >= capacity_)
				reserve(capacity_ * 2);
			constructRange(size_, size_ + 1);
			moveBackByInterval(end(), end() - pos, 1);
			constructRange(size_ - pos, size_ - pos + 1, val);
			size_++;
			return iterator(&data_[(size_ - 1) - pos]);
		}

		void insert(iterator position, size_type n, const value_type& val)
		{
			size_type pos = getOffsetFromEndTo(position);

			if (size_ + n + 1 >= capacity_)
				reserve(std::max((size_ + n + 1), (2 * capacity_)));
			constructRange(size_, size_ + n);
			moveBackByInterval(end() + n - 1, end() - pos, n);
			constructRange(size_ - pos, size_ - pos + n, val);
			size_ += n;
		}

		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,
					typename ft::check_type<typename ft::iterator_traits<InputIterator>::iterator_category>::type* = 0)
		{
			if (ft::is_same<std::input_iterator_tag,
							typename ft::iterator_traits<InputIterator>::iterator_category>::value)
			{
				vector tmp = *this;
				iterator start = begin();
				iterator finish = end();

				for (; start != position; start++)
					tmp.push_back(*start);
				for (; first != last; first++, start++)
					tmp.push_back(*first);
				for (; start != finish; start++)
					tmp.push_back(*start);
				clear();
				allocator_.deallocate(data_, capacity_);
				*this = tmp;
				return;
			}
			size_type pos = getOffsetFromEndTo(position);
			size_type n = ft::distance(first, last);

			if (size_ + n >= capacity_)
				reserve(std::max((size_ + n + 1), (2 * capacity_)));
			constructRange(size_, size_ + 1);
			moveBackByInterval(end() + n - 1, end() - pos, n);
			for (size_type i = 0; i < n; i++)
				allocator_.construct(&data_[size_ - pos + i], *first++);
			size_ += n;
		}

		size_type max_size() const throw()
		{
			// return (allocator_.max_size()); // why not?
			return (std::numeric_limits<size_type>::max() / std::max(2, static_cast<int>(sizeof(value_type))));
		}

		vector& operator=(const vector& x)
		{
			size_type i;

			if (capacity_ < x.size_)
				reserve(x.capacity_);
			/**
			 * value_type에서 operator=가 오버로딩 안되어있으면 ub
			 * https://m.cplusplus.com/reference/vector/vector/operator=/
			 */
			for (i = 0; i < size_ && i < x.size_; i++)
				data_[i] = x.data_[i];
			for (; i < x.size_; i++)
				allocator_.construct(&data_[i], x.data_[i]);
			if (!ft::is_trivial_destructible_junior<value_type>::value)
				destroyRange(x.size_, size_);
			size_ = x.size_;
			return (*this);
		}

		reference operator[](size_type n)  // does not check bound
		{
			return (data_[n]);
		}
		const_reference operator[](size_type n) const
		{
			return (data_[n]);
		}

		void pop_back()
		{
			if (empty())
				return;
			if (!ft::is_trivial_destructible_junior<value_type>::value)
				allocator_.destroy(&data_[size_ - 1]);
			size_--;
		}

		void push_back(const value_type& val)
		{
			if (size_ + 1 >= capacity_)
				reserve(capacity_ * 2);
			allocator_.construct(&data_[size_++], val);
		}

		reverse_iterator rbegin() throw()
		{
			return (reverse_iterator(end()));
		}
		const_reverse_iterator rbegin() const throw()
		{
			return (const_reverse_iterator(end()));
		}

		reverse_iterator rend() throw()
		{
			return (reverse_iterator(begin()));
		}
		const_reverse_iterator rend() const throw()
		{
			return (const_reverse_iterator(begin()));
		}

		void reserve(size_type n)
		{
			pointer tmp;
			size_type i;

			if (n > max_size())
				throw(std::length_error("reserve"));
			if (capacity_ >= n)
				return;
			tmp = allocator_.allocate(n);
			if (!ft::is_trivial_destructible_junior<value_type>::value)
			{
				for (i = 0; i < size_; i++)
				{
					allocator_.construct(&tmp[i], data_[i]);
					allocator_.destroy(&data_[i]);
				}
			}
			else
			{
				for (i = 0; i < size_; i++)
					allocator_.construct(&tmp[i], data_[i]);
			}
			allocator_.deallocate(data_, capacity_);
			data_ = tmp;
			capacity_ = n;
		}

		void resize(size_type n, value_type val = value_type())  // throw(std::bad_alloc, std::length_error)
		{
			// TODO: optimization using checking is type trivial destructible
			if (n > max_size())
				throw std::length_error("resize");
			if (size_ > n)
			{
				if (!ft::is_trivial_destructible_junior<value_type>::value)
					for (size_type i = n; i < size_; i++)
						allocator_.destroy(&data_[i]);
			}
			else if (size_ < n && n < capacity_)
			{
				for (size_type i = size_; i < n; i++)
					allocator_.construct(&data_[i], val);
			}
			else if (capacity_ <= n)
			{
				reserve(n + 1);
				for (size_type i = size_; i < n; i++)
					allocator_.construct(&data_[i], val);
			}
			size_ = n;
		}

		size_type size() const throw()
		{
			return (size_);
		}

		void swap(vector& x)
		{
			pointer d = data_;
			data_ = x.data_;
			x.data_ = d;

			size_type n = size_;
			size_ = x.size_;
			x.size_ = n;

			size_type c = capacity_;
			capacity_ = x.capacity_;
			x.capacity_ = c;

			allocator_type tmp = get_allocator();
			allocator_ = x.get_allocator();
			x.allocator_ = tmp;
		}

	private:
		size_type getOffsetFromEndTo(iterator position)
		{
			size_type pos = 0;
			for (size_type i = size_; i >= 0; i--)
			{
				if (iterator(&data_[i]) == position)
					break;
				pos++;
			}
			return pos;
		}
		void constructRange(size_type from, size_type to, value_type val = value_type())
		{
			for (size_type i = from; i < to; i++)
				allocator_.construct(&data_[i], val);
		}
		void destroyRange(size_type from, size_type to)
		{
			if (!ft::is_trivial_destructible_junior<value_type>::value)
				for (size_type i = from; i < to; i++)
					allocator_.destroy(&data_[i]);
		}
		void destroyRange(iterator from, iterator to)
		{
			if (!ft::is_trivial_destructible_junior<value_type>::value)
				for (iterator i = from; i < to; i++)
					allocator_.destroy(&(*i));
		}
		void moveBackByInterval(iterator start, iterator finish, size_type interval)
		{
			for (; start - (interval - 1) != finish; start--)
			{
				allocator_.construct(&(*start), (*(start - interval)));
				allocator_.destroy(&(*(start - interval)));
			}
		}
		void moveFrontByInterval(iterator start, iterator finish, size_type interval)
		{
			for (; start + interval != finish; start++)
			{
				allocator_.destroy(&(*start));
				allocator_.construct(&(*start), (*(start + interval)));
			}
		}
	};
	template <class T, class Alloc>
	bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}
	template <class T, class Alloc>
	bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}
	template <class T, class Alloc>
	bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}
	template <class T, class Alloc>
	bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}
	template <class T, class Alloc>
	bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (rhs < lhs);
	}
	template <class T, class Alloc>
	bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}
	template <class T, class Alloc>
	void swap(vector<T, Alloc>& x, vector<T, Alloc>& y)
	{
		x.swap(y);
	}

	template <class Iterator, class Pointer>
	class vector_iterator
	{
	public:
		typedef ft::iterator<std::random_access_iterator_tag, typename ft::remove_pointer<Iterator>::type> iterType;
		typedef typename iterType::value_type value_type;
		typedef typename iterType::pointer pointer;
		typedef typename iterType::reference reference;
		typedef typename iterType::difference_type difference_type;
		typedef typename iterType::iterator_category iterator_category;

	private:
		Iterator base_;

	public:
		vector_iterator() {}

		/**
		 * Copy Constructor
		 * ft::is_same의 두번째 템플릿 인자는 복사 생성에 이용하는 iterator의 type을 의미한다.
		 * i) const_iterator = iterator (O)
		 * ii) iterator = const_iterator (X)
		 * 따라서 들어오는 템플릿 인자에 대해선 const 포인터인지 non-const 포인터인지를 그대로 받고,
		 * 현재 받는 템플릿 인자의 두번째 인자인 Pointer 타입과 비교한다.
		 * 즉, const_iterator는 <const int*, int*> 타입이고 iterator는 <int*, int*> 타입이기 때문에
		 * const_iterator = iterator의 과정은 다음과 같다. -> is_same<int *, int*>
		 * iterator = const_iterator의 과정은 다음과 같다. -> is_same<const int*, int*>
		 */
		template <class P /*pointer를 받는다. */>
		vector_iterator(const vector_iterator<P, typename ft::enable_if<
													 ft::is_same<P, Pointer>::value,
													 Pointer>::type>& other)
			: base_(other.base())
		{
		}

		explicit vector_iterator(const Iterator& otherIter)
			: base_(otherIter) {}

		/**
		 * can be assigned by same type
		 * 복사 생성자 처럼 호환 가능한 것들을 인자로 받을 필욘 없음
		 */
		vector_iterator& operator=(const vector_iterator& other)
		{
			base_ = other.base_;
			return (*this);
		}

		/**
		 * pointer를 보관만 하기 때문에 destructor의 본문이 없음
		 */
		~vector_iterator() {}

		/**
		 * lvalue로도 되어야 하고 rvalue로도 되어야 하므로
		 * reference를 반환하므로 lvalue가 될 수도 있게 된다.
		 */
		reference operator*() const
		{
			return (*base_);
		}

		/**
		 * iterator가 가리키는 값의 멤버 함수 및 멤버 변수를 자신의 것인 마냥 접근할 수 있다.
		 */
		pointer operator->() const
		{
			return &(*base_);
		}
		reference& operator[](int n) const
		{
			return base_[n];
		}
		Iterator base(void) const
		{
			return base_;
		}

		vector_iterator& operator++()
		{
			++base_;
			return (*this);
		}
		vector_iterator operator++(int)
		{
			vector_iterator tmp = *this;
			++(*this);
			return (tmp);
		}
		vector_iterator& operator--()
		{
			--base_;
			return (*this);
		}
		vector_iterator operator--(int)
		{
			vector_iterator tmp = *this;
			--(*this);
			return (tmp);
		}

		vector_iterator operator+(int n) const
		{
			vector_iterator tmp = *this;

			tmp.base_ += n;
			return (tmp);
		}
		vector_iterator operator-(int n) const
		{
			vector_iterator tmp = *this;

			tmp.base_ -= n;
			return (tmp);
		}
		difference_type operator-(const vector_iterator& other) const
		{
			return (base_ - other.base());
		}

		vector_iterator operator+=(int n)
		{
			*this = *this + n;
			return (*this);
		}
		vector_iterator operator-=(int n)
		{
			*this = *this - n;
			return (*this);
		}
		/**
		 * 두 iterator가 같다 => 두 iterator 값이 동일한 sequence를 반복한다(순회한다)
		 */
		bool operator==(const vector_iterator& other) const
		{
			return (base_ == other.base_);
		}
		bool operator!=(const vector_iterator& other) const
		{
			return !(base_ == other.base_);
		}

		bool operator<(const vector_iterator& other) const
		{
			return (base_ < other.base());
		}
		bool operator>(const vector_iterator& other) const
		{
			return (base_ > other.base());
		}
		bool operator<=(const vector_iterator& other) const
		{
			return (base_ <= other.base());
		}
		bool operator>=(const vector_iterator& other) const
		{
			return (base_ >= other.base());
		}

		template <class Iter, class P>
		friend vector_iterator<Iter, P> operator+(int n, const vector_iterator<Iter, P>& other);
	};

	template <class Iter, class P>
	vector_iterator<Iter, P> operator+(int n, const vector_iterator<Iter, P>& other)
	{
		return (other + n);
	}
}

#endif
