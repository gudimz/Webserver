#pragma once
#ifndef FT_UTILS_H__
#define FT_UTILS_H__

#define RED   false
#define BLACK true

#include <sstream>
#include <string>
namespace ft
{
    template <class arg, class result>
    struct unary_function
    {
      typedef arg argument_type;   ///< @c argument_type is the type of the
      typedef result result_type;  ///< @c result_type is the return type
    };

    template<typename Arg1, typename Arg2, typename Result>
    struct binary_function
    {
      typedef Arg1            first_argument_type; 
      typedef Arg2            second_argument_type;
      typedef Result          result_type;
    };

    template <class _Pair>
    struct Select1st : public unary_function<_Pair,
                          typename _Pair::first_type>
    {
      typename _Pair::first_type&       operator()(_Pair& x) const { return x.first; }
      const typename _Pair::first_type& operator()(const _Pair& x) const  { return x.first; }
    };

    template <class _Pair>
    struct Select2nd : public unary_function<_Pair,
                          typename _Pair::second_type>
    {
      typename _Pair::second_type&        operator()(_Pair& x) const { return x.second; }
      const typename _Pair::second_type&  operator()(const _Pair& x) const { return x.second; }
    };

    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::difference_type      difference_type;
        typedef          difference_type                distance_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
    };

    template<typename _Tp>
    struct iterator_traits<_Tp*>
    {
      typedef ft::random_access_iterator_tag  iterator_category;
      typedef _Tp                             value_type;
      typedef std::ptrdiff_t                  difference_type;
      typedef _Tp*                            pointer;
      typedef _Tp&                            reference;
    };

    template<typename _Tp>
    struct iterator_traits<const _Tp*>
    {
      typedef ft::random_access_iterator_tag  iterator_category;
      typedef _Tp                             value_type;
      typedef std::ptrdiff_t                  difference_type;
      typedef const _Tp*                      pointer;
      typedef const _Tp&                      reference;
    };

    template <class Category, class T, class Distance = std::ptrdiff_t,
            class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef T         value_type;
        typedef Distance  difference_type;
        typedef Pointer   pointer;
        typedef Reference reference;
        typedef Category  iterator_category;
    };

    template<class It>
    typename ft::iterator_traits<It>::difference_type 
    do_distance(It first, It last, ft::input_iterator_tag)
    {
        typename ft::iterator_traits<It>::difference_type result = 0;
        while (first != last) {
            ++first;
            ++result;
        }
        return result;
    }

    template<class It>
    typename ft::iterator_traits<It>::difference_type 
        do_distance(It first, It last, ft::random_access_iterator_tag)
    {
        return last - first;
    }

    template<class It>
    typename ft::iterator_traits<It>::difference_type 
    distance(It first, It last)
    {
        return ft::do_distance(first, last,
                typename ft::iterator_traits<It>::iterator_category());
    }
 
    template<bool, typename _Tp = void>struct enable_if { };
    template<typename _Tp> struct enable_if<true, _Tp> { typedef _Tp type; };
    
    template<typename _Tp, _Tp v>
    struct integral_constant{
        static const _Tp                        value = v;
        typedef _Tp                             value_type;
        typedef integral_constant<_Tp, v>       type;
        operator value_type() { return value; }
    };
    
    typedef ft::integral_constant<bool, true> true_type;
    typedef ft::integral_constant<bool, false> false_type;
    
    template<typename, typename>
    struct are_same
    {
      enum { value = 0 };
      typedef false_type type;
    };

    template<bool>
    struct truth_type
    { typedef false_type type; };
    
    template<>
    struct truth_type<true>
    { typedef true_type type; };

    template<typename>
    struct is_integral_helper : public false_type { };

    template<typename _Tp>
    struct is_integer
    {
      enum { value = 0 };
      typedef false_type type;
    };
  // Thirteen specializations (yes there are eleven standard integer
  // types; <em>long long</em> and <em>unsigned long long</em> are
  // supported as extensions).  Up to four target-specific int<N>
  // types are supported as well.
  template<>
    struct is_integer<bool>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<char>
    {
      enum { value = 1 };
      typedef true_type type;
    };
    template<>
    struct is_integral_helper<wchar_t>
    {
        const static bool value = true;
    };
  template<>
    struct is_integer<signed char>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<unsigned char>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<short>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<unsigned short>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<int>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<unsigned int>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<long>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<unsigned long>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<long long>
    {
      enum { value = 1 };
      typedef true_type type;
    };
  template<>
    struct is_integer<unsigned long long>
    {
      enum { value = 1 };
      typedef true_type type;
    };
// #define INT_N(TYPE)                         \
//   template<>                                \
//     struct is_integer<TYPE>                \
//     {                                        \
//       enum { value = 1 };                \
//       typedef true_type type;        \
//     };                                        \
//   template<>                                \
//     struct is_integer<unsigned TYPE>        \
//     {                                        \
//       enum { value = 1 };                \
//       typedef true_type type;        \
//     };
    // template<typename Tp>
    // struct is_default_constructible
    // : public is_default_constructible_safe<Tp>::type { };

    template<typename _Tp>
    struct remove_const { typedef _Tp     type; };
    
    template<typename _Tp>
    struct remove_const<_Tp const> { typedef _Tp     type; };
  
  /// remove_volatile
    template<typename _Tp>
    struct remove_volatile { typedef _Tp     type; };

    template<typename _Tp>
    struct remove_volatile<_Tp volatile> { typedef _Tp     type; };
    
    template<typename _Tp>
    struct remove_cv
    {
        typedef typename remove_const<typename remove_volatile<_Tp>::type>::type     type;
    };
    
    template<typename>
      struct is_pointer_helper
      : public false_type { };
    
    template<typename _Tp>
      struct is_pointer_helper<_Tp*>
      : public true_type { };
    /// is_pointer
    
    template<typename _Tp>
      struct is_pointer
      : public is_pointer_helper<typename remove_cv<_Tp>::type>::type
      { };

    // is_integral
    template<typename _Tp>
    struct is_integral : public integral_constant<bool, (is_integral_helper<typename
                        remove_cv<_Tp>::type>::value)> {};
    // template<typename> struct is_integral: ft::false_type {};
    template<> struct is_integral<int>: ft::true_type {};

  template <class InputIterator1, class InputIterator2>
  bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
  {
      while (first1 != last1)
      {
          if (first2 == last2 || *first2 < *first1) 
              return false;
          else if (*first1 < *first2) 
              return true;
          ++first1; ++first2;
      }
      return (first2 != last2);
  }
  template<bool _BoolType>
    struct _equal
    {
      template<typename _II1, typename _II2>
      static bool        equal(_II1 first1, _II1 last1, _II2 first2)
        {
          for (; first1 != last1; ++first1, ++first2)
            if (!(*first1 == *first2))
              return false;
          return true;
        }
    };

    template<typename _II1, typename _II2>
    bool    equal_aux(_II1 first1, _II1 last1, _II2 first2)
    {
      typedef typename ft::iterator_traits<_II1>::value_type _ValueType1;
      typedef typename ft::iterator_traits<_II2>::value_type _ValueType2;
      const bool simple = ((ft::is_integer<_ValueType1>::value
                              || ft::is_pointer<_ValueType1>::value)
                             && ft::is_pointer<_II1>::value
                             && ft::is_pointer<_II2>::value
                             && ft::are_same<_ValueType1, _ValueType2>::value);
      return ft::_equal<simple>::equal(first1, last1, first2);
    }
    template<typename _II1, typename _II2>
    bool
    equal(_II1 first1, _II1 last1, _II2 first2)
    {
      return ft::equal_aux(first1, last1, first2);
    }

//=========================================ReverseIterator===============================================
    template<typename _Iterator>
    class ReverseIterator
    : public iterator<typename iterator_traits<_Iterator>::iterator_category,
                      typename iterator_traits<_Iterator>::value_type,
                      typename iterator_traits<_Iterator>::difference_type,
                      typename iterator_traits<_Iterator>::pointer,
                      typename iterator_traits<_Iterator>::reference>
    {
    protected:
      _Iterator current;
      typedef iterator_traits<_Iterator>                traits_type;
    public:
      typedef _Iterator                                        iterator_type;
      typedef typename traits_type::difference_type        difference_type;
      typedef typename traits_type::pointer                pointer;
      typedef typename traits_type::reference                reference;

      ReverseIterator() : current() { }
      ReverseIterator(iterator_type x) : current(x) { }
      ReverseIterator(const ReverseIterator& x) : current(x.current) { }

      template<typename _Iter>
      ReverseIterator(const ReverseIterator<_Iter>& x) : current(x.base()) { }

      iterator_type
      base() const
      { return current; }

      reference
      operator*() const
      {
        _Iterator tmp = current;
        return *--tmp;
      }

       pointer
      operator->() const
      { return &(operator*()); }

       ReverseIterator&
      operator++()
      {
        --current;
        return *this;
      }

       ReverseIterator
      operator++(int)
      {
        ReverseIterator tmp = *this;
        --current;
        return tmp;
      }

       ReverseIterator&
      operator--()
      {
        ++current;
        return *this;
      }

       ReverseIterator
      operator--(int)
      {
        ReverseIterator tmp = *this;
        ++current;
        return tmp;
      }

       ReverseIterator
      operator+(difference_type n) const
      { return ReverseIterator(current - n); }

       ReverseIterator&
      operator+=(difference_type n)
      {
        current -= n;
        return *this;
      }

       ReverseIterator
      operator-(difference_type n) const
      { return ReverseIterator(current + n); }

       ReverseIterator&
      operator-=(difference_type n)
      {
        current += n;
        return *this;
      }

       reference
      operator[](difference_type n) const
      { return *(*this + n); }
    };

  template<typename _Iterator>
     bool
    operator==(const ReverseIterator<_Iterator>& x,
               const ReverseIterator<_Iterator>& y)
    { return x.base() == y.base(); }
  template<typename _Iterator>
     bool
    operator<(const ReverseIterator<_Iterator>& x,
              const ReverseIterator<_Iterator>& y)
    { return y.base() < x.base(); }
  template<typename _Iterator>
     bool
    operator!=(const ReverseIterator<_Iterator>& x,
               const ReverseIterator<_Iterator>& y)
    { return !(x == y); }
  template<typename _Iterator>
     bool
    operator>(const ReverseIterator<_Iterator>& x,
              const ReverseIterator<_Iterator>& y)
    { return y < x; }
  template<typename _Iterator>
     bool
    operator<=(const ReverseIterator<_Iterator>& x,
               const ReverseIterator<_Iterator>& y)
    { return !(y < x); }
  template<typename _Iterator>
     bool
    operator>=(const ReverseIterator<_Iterator>& x,
               const ReverseIterator<_Iterator>& y)
    { return !(x < y); }
  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 280. Comparison of ReverseIterator to const ReverseIterator.
  template<typename _IteratorL, typename _IteratorR>
     bool
    operator==(const ReverseIterator<_IteratorL>& x,
               const ReverseIterator<_IteratorR>& y)
    { return x.base() == y.base(); }
  template<typename _IteratorL, typename _IteratorR>
     bool
    operator<(const ReverseIterator<_IteratorL>& x,
              const ReverseIterator<_IteratorR>& y)
    { return y.base() < x.base(); }
  template<typename _IteratorL, typename _IteratorR>
     bool
    operator!=(const ReverseIterator<_IteratorL>& x,
               const ReverseIterator<_IteratorR>& y)
    { return !(x == y); }
  template<typename _IteratorL, typename _IteratorR>
     bool
    operator>(const ReverseIterator<_IteratorL>& x,
              const ReverseIterator<_IteratorR>& y)
    { return y < x; }
  template<typename _IteratorL, typename _IteratorR>
     bool
    operator<=(const ReverseIterator<_IteratorL>& x,
               const ReverseIterator<_IteratorR>& y)
    { return !(y < x); }
  template<typename _IteratorL, typename _IteratorR>
     bool
    operator>=(const ReverseIterator<_IteratorL>& x,
               const ReverseIterator<_IteratorR>& y)
    { return !(x < y); }

  template <class Iter1, class Iter2>
	typename ReverseIterator<Iter1>::difference_type
	operator-(const ReverseIterator<Iter1>& x, const ReverseIterator<Iter2>& y) {
		return y.base() - x.base();
	}
	template <class Iter>
	ReverseIterator<Iter> operator+(typename ReverseIterator<Iter>::difference_type n,
									 const ReverseIterator<Iter>& x) {
		return ReverseIterator<Iter>(x.base() - n);
	}

//=================================================pair===============================================

    template<class T1, class T2>
    struct pair
    {
        typedef T1  first_type;    /// @c first_type is the first bound type
        typedef T2  second_type;   /// @c second_type is the second bound type
        first_type  first;                 /// @c first is a copy of the first object
        second_type second;
        pair() : first(), second() { }
        pair(const T1& a, const T2& b) : first(a), second(b) { }
        template<class U1, class U2>
        pair(const pair<U1, U2>& p): first(p.first), second(p.second) { }
        
        pair&   operator=(const pair& p)
        {
            this->first = p.first;
            this->second = p.second;
            return *this;
        }

        template<class U1, class U2>
        pair&   operator=(const pair<U1, U2>& p)
        {
          first = p.first;
          second = p.second;
          return *this;
        }

        bool    operator==(const pair<T1, T2>& y) const
        { return this->first == y.first && this->second == y.second; }

        bool    operator<(const pair<T1, T2>& y) const
        { return this->first < y.first
                || (!(y.first < this->first) && this->second < y.second); }

        bool    operator!=(const pair<T1, T2>& y) const
        { return !(this == y); }

        bool    operator>(const pair<T1, T2>& y) const
        { return y < this; }

        bool    operator<=(const pair<T1, T2>& y) const
        { return !(y < this); }

        bool    operator>=(const pair<T1, T2>& y) const
        { return !(this < y); }
    };

    template<class T1, class T2>
    ft::pair<T1, T2>    make_pair(T1 x, T2 y)
    { return pair<T1, T2>(x, y); }

    template <class T>
    void swap ( T& a, T& b )
    {
        T c(a);
        a = b;
        b = c;
    }
//================================================rb_tree_node_base=====================================
    struct rb_tree_node_base
    {
      typedef rb_tree_node_base*       _base_ptr;
      typedef const rb_tree_node_base* _const_base_ptr;
      bool            color;
      _base_ptr       parent;
      _base_ptr       left;
      _base_ptr       right;
      static _base_ptr            _S_minimum(_base_ptr x)
      {
        while (x->left != 0) x = x->left;
        return x;
      }
      static _const_base_ptr      _S_minimum(_const_base_ptr x)
      {
        while (x->left != 0) x = x->left;
        return x;
      }
      static _base_ptr            _S_maximum(_base_ptr x)
      {
        while (x->right != 0) x = x->right;
        return x;
      }
      static _const_base_ptr      _S_maximum(_const_base_ptr x)
      {
        while (x->right != 0) x = x->right;
        return x;
      }
    };

    static rb_tree_node_base*    local_Rb_tree_decrement(rb_tree_node_base* x) throw ()
    {
      if (x->color == RED
          && x->parent->parent == x)
        x = x->right;
      else if (x->left != 0)
      {
        rb_tree_node_base* y = x->left;
        while (y->right != 0)
          y = y->right;
        x = y;
      }
      else
      {
        rb_tree_node_base* y = x->parent;
        while (x == y->left)
          {
            x = y;
            y = y->parent;
          }
        x = y;
      }
      return x;
    }
    rb_tree_node_base*          rb_tree_decrement(rb_tree_node_base* x) throw ()
    {
      return local_Rb_tree_decrement(x);
    }
    const rb_tree_node_base*   rb_tree_decrement(const rb_tree_node_base* x) throw ()
    {
      return local_Rb_tree_decrement(const_cast<rb_tree_node_base*>(x));
    }

    static rb_tree_node_base*    local_Rb_tree_increment(rb_tree_node_base* x) throw ()
    {
      if (x->right != 0)
      {
        x = x->right;
        while (x->left != 0)
          x = x->left;
      }
      else
      {
        rb_tree_node_base* y = x->parent;
        while (x == y->right)
          {
            x = y;
            y = y->parent;
          }
        if (x->right != y)
          x = y;
      }
      return x;
    }
    rb_tree_node_base*        rb_tree_increment(rb_tree_node_base* x) throw ()
    {
      return local_Rb_tree_increment(x);
    }
    const rb_tree_node_base*  rb_tree_increment(const rb_tree_node_base* x) throw ()
    {
      return local_Rb_tree_increment(const_cast<rb_tree_node_base*>(x));
    }

//=================================================rb_tree_node=========================================
    template<typename Val>
    struct rb_tree_node : public rb_tree_node_base
    {
      typedef rb_tree_node<Val>*  _link_type;
      Val                         value_field;

      rb_tree_node(const Val& v) : rb_tree_node_base(), value_field(v) { }

      Val*           valptr() { return &value_field; }
      const Val*     valptr() const { return &value_field; }
    };
};

#endif