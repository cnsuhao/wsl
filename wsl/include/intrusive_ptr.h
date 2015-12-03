#ifndef   _WSL_INTRUSIVE_PTR_H_
#define  _WSL_INTRUSIVE_PTR_H_


#include <iosfwd>
#include <meta.h>

namespace wsl
{
	template<class T>
	class intrusive_ptr
	{
	private:
		typedef intrusive_ptr this_type;
		T* p_;
	public:

		typedef T element_type;
		typedef T value_type;

		intrusive_ptr():p_(NULL){}

		intrusive_ptr(T* p,bool add_ref=true)
			:p_(p)
		{
			if(p_ != 0 && add_ref)
				intrusive_ptr_add_ref(p_);
		}
		template<class U>
		intrusive_ptr( intrusive_ptr<U> const & rhs, typename enable_if<is_convertible<U *, T *>, void ***>::type = 0)
			: p_( rhs.get() )
		{
			if( p_ != 0 )
				intrusive_ptr_add_ref( p_ );
		}
		intrusive_ptr(intrusive_ptr const& rhs)
			:p_(rhs.p_)
		{
			if (p_!=0)
			{
				intrusive_ptr_add_ref( p_ );
			}
		}
		~intrusive_ptr()
		{
			if (p_ !=0)
			{
				intrusive_ptr_release(p_);
			}
		}
		void swap(intrusive_ptr& rhs)
		{
			T* tmp = p_;
			p_ = rhs.p_;
			rhs.p_ = tmp;
		}

		template<class U>
		intrusive_ptr& operator =(intrusive_ptr<U> const& rhs)
		{
			this_type(rhs).swap(*this);
			return *this;
		}
		intrusive_ptr& operator=(intrusive_ptr const & rhs)
		{
			this_type(rhs).swap(*this);
			return *this;
		}
		intrusive_ptr & operator=(T * rhs)
		{
			this_type(rhs).swap(*this);
			return *this;
		}

		void reset()
		{
			this_type().swap(*this);
		}

		void reset(T* rhs)
		{
			this_type(rhs).swap(*this);
		}
	
		T* get()const
		{
			return p_;
		}
		T& operator*() const
		{
			return *p_;
		}

		T* operator->()const
		{
			return p_;
		}
		typedef T* (this_type::*unspecified_bool_type)()const;

		operator unspecified_bool_type() const // never throws
		{
			return p_ == 0 ? 0: &this_type::get;
		}

		// operator! is a Borland-specific workaround
		bool operator! () const
		{
			return p_ == 0;
		}
		
	};


	template<class T, class U>
	inline bool operator==(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b)
	{
		return a.get() == b.get();
	}

	template<class T, class U>
	inline bool operator!=(intrusive_ptr<T> const & a, intrusive_ptr<U> const & b)
	{
		return a.get() != b.get();
	}

	template<class T, class U>
	inline bool operator==(intrusive_ptr<T> const & a, U * b)
	{
		return a.get() == b;
	}

	template<class T, class U>
	inline bool operator!=(intrusive_ptr<T> const & a, U * b)
	{
		return a.get() != b;
	}

	template<class T, class U>
	inline bool operator==(T * a, intrusive_ptr<U> const & b)
	{
		return a == b.get();
	}

	template<class T, class U>
	inline bool operator!=(T * a, intrusive_ptr<U> const & b)
	{
		return a != b.get();
	}

	template<class T>
	inline bool operator<(intrusive_ptr<T> const & a, intrusive_ptr<T> const & b)
	{
		return std::less<T *>()(a.get(), b.get());
	}

	template<class T>
	inline bool operator>(intrusive_ptr<T> const & a, intrusive_ptr<T> const & b)
	{
		return b < a;
	}

	template<class T>
	inline bool operator>=(intrusive_ptr<T> const & a, intrusive_ptr<T> const & b)
	{
		return !(a < b);
	}

	template<class T>
	inline bool operator<=(intrusive_ptr<T> const & a, intrusive_ptr<T> const & b)
	{
		return !(b < a);
	}

	template<class T>
	inline void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs)
	{
		lhs.swap(rhs);
	}

	// mem_fn support

	template<class T>
	inline T * get_pointer(intrusive_ptr<T> const & p)
	{
		return p.get();
	}

	template<class T, class U>
	inline intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const & p)
	{
		return static_cast<T *>(p.get());
	}

	template<class T, class U>
	inline intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const & p)
	{
		return const_cast<T *>(p.get());
	}

	template<class T, class U>
	inline intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const & p)
	{
		return dynamic_cast<T *>(p.get());
	}

	template<class E, class T, class Y>
	inline std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, intrusive_ptr<Y> const & p)
	{
		os << p.get();
		return os;
	}


}


#endif