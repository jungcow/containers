#ifndef __FT_QUEUE_H__
#define __FT_QUEUE_H__

namespace ft
{
	template <class T, class Alloc>
	class Queue
	{
	private:
		T* elem_;
		Alloc allocator_;
		size_t length_;
		size_t size_, front_, rear_;

	public:
		Queue(size_t len, Alloc allocator) : length_(len), allocator_(allocator)
		{
			size_ = front_ = rear_ = 0;
			elem_ = allocator_.allocate(length_);
		}
		~Queue()
		{
			allocator_.deallocate(elem_, length_);
		}

		bool empty(void) const
		{
			return (size_ == 0);
		}

		size_t size(void) const
		{
			return size_;
		}

		void enqueue(T newElem)
		{
			rear_ = (rear_ + 1) % length_;
			allocator_.construct(&elem_[rear_], newElem);
			++size_;
		}

		T dequeue(void)
		{
			front_ = (front_ + 1) % length_;
			T item = elem_[front_];

			allocator_.destroy(&elem_[front_]);
			--size_;
			return item;
		}
	};
}

#endif
