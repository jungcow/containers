#ifndef __FT_TYPE_TRAITS_H__
#define __FT_TYPE_TRAITS_H__

/**
 * enable_if
 * is_same
 * is_integral
 * is_arithmetic
 * is_base_of
 * is_pointer
 * is_array
 * is_trivial_destructible_junior
 * is_pair
 * remove_const
 * remove_pointer
 */

#include <cstdint>

namespace ft
{
	/**
	 * enable_if
	 */
	template <bool Cond, typename T = void>
	struct enable_if
	{
	};

	template <typename T>
	struct enable_if<true, T>
	{
		typedef T type;
	};

	template <class>
	struct check_type
	{
		typedef void type;
	};

	/**
	 * is_same
	 */
	template <typename T, typename U>
	struct is_same
	{
		static const bool value = false;
	};

	template <typename T>
	struct is_same<T, T>
	{
		static const bool value = true;
	};

	/**
	 * is_integral
	 */
	template <typename T>
	struct is_integral
	{
		static const bool value = ft::is_same<T, bool>::value ||
								  ft::is_same<T, char>::value ||
								  ft::is_same<T, char16_t>::value ||
								  ft::is_same<T, char32_t>::value ||
								  ft::is_same<T, wchar_t>::value ||
								  ft::is_same<T, signed char>::value ||
								  ft::is_same<T, short int>::value ||
								  ft::is_same<T, int>::value ||
								  ft::is_same<T, long int>::value ||
								  //   ft::is_same<T, long long int>::value ||
								  ft::is_same<T, unsigned char>::value ||
								  ft::is_same<T, unsigned short int>::value ||
								  ft::is_same<T, unsigned int>::value ||
								  ft::is_same<T, unsigned long int>::value;
		//   ft::is_same<T, unsigned long long int>::value;
	};

	/**
	 * is_arithmetic
	 */
	template <typename T>
	struct is_arithmetic
	{
		typedef T type;
		static const bool value = ft::is_integral<T>::value ||
								  ft::is_same<T, float>::value ||
								  ft::is_same<T, double>::value ||
								  ft::is_same<T, long double>::value;
	};

	/**
	 * is_base_of
	 */
	template <class Base, class Derived>
	struct is_base_of
	{
		struct two
		{
			char c[2];
		};
		static two test_base(Base*);
		static char test_base(...);

		static const bool value = sizeof(test_base(static_cast<Derived*>(0))) == 2;
	};

	/**
	 * is_pointer
	 */
	template <typename T>
	struct is_pointer
	{
		static const bool value = false;
	};
	template <typename T>
	struct is_pointer<T*>
	{
		static const bool value = true;
	};

	template <typename T>
	struct is_array
	{
		static const bool value = false;
	};
	template <typename T>
	struct is_array<T[]>
	{
		static const bool value = true;
	};

	template <typename T>
	struct is_trivial_destructible_junior
	{
		static const bool value = ft::is_arithmetic<T>::value ||
								  ft::is_pointer<T>::value ||
								  ft::is_array<T>::value;
	};

	template <class T>
	struct is_pair
	{
	private:
		struct two
		{
			char c[2];
		};

		template <class U>
		static two test(...);

		template <class U>
		static char test(typename ft::check_type<typename U::first_type>::type* = 0,
						 typename ft::check_type<typename U::second_type>::type* = 0);

	public:
		static const bool value = (sizeof(test<T>(0, 0)) == 1);
	};

	template <class T>
	struct remove_const
	{
		typedef T type;
	};

	template <class T>
	struct remove_const<T const>
	{
		typedef T type;
	};

	template <class T>
	struct remove_pointer
	{
		typedef T type;
	};

	template <class T>
	struct remove_pointer<T*>
	{
		typedef T type;
	};
	template <class T>
	struct remove_pointer<T* const>
	{
		typedef T type;
	};
	template <class T>
	struct remove_pointer<T* volatile>
	{
		typedef T type;
	};
	template <class T>
	struct remove_pointer<T* const volatile>
	{
		typedef T type;
	};

}

#endif
