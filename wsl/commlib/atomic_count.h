#ifndef __WSL_DETAIL_ATOMIC_COUNT_GCC_X86_H__
#define __WSL_DETAIL_ATOMIC_COUNT_GCC_X86_H__

//  atomic_count for g++ on 486+/AMD64

/*
type __sync_fetch_and_add (type *ptr, type value);
type __sync_fetch_and_sub (type *ptr, type value);
type __sync_fetch_and_or (type *ptr, type value);
type __sync_fetch_and_and (type *ptr, type value);
type __sync_fetch_and_xor (type *ptr, type value);
type __sync_fetch_and_nand (type *ptr, type value);
type __sync_add_and_fetch (type *ptr, type value);
type __sync_sub_and_fetch (type *ptr, type value);
type __sync_or_and_fetch (type *ptr, type value);
type __sync_and_and_fetch (type *ptr, type value);
type __sync_xor_and_fetch (type *ptr, type value);
type __sync_nand_and_fetch (type *ptr, type value);
*/
namespace wsl
{
	class atomic_count
	{
	public:

		explicit atomic_count( long v = 0 ) : value_( static_cast< int >( v ) ) {}

		void operator++()
		{
			__asm__ 
				(
				"lock\n\t"
				"incl %0":
			"+m"( value_ ): // output (%0)
			: // inputs
			"cc" // clobbers
				);
		}

		long operator--()
		{
			return atomic_exchange_and_add( &value_, -1 ) - 1;
		}

		operator long() const
		{
			return atomic_exchange_and_add( &value_, 0 );
		}

	private:

		atomic_count(atomic_count const &);
		atomic_count & operator=(atomic_count const &);

		mutable int value_;

	private:

		static int atomic_exchange_and_add( int * pw, int dv )
		{
			// int r = *pw;
			// *pw += dv;
			// return r;

			int r;

			__asm__ __volatile__
				(
				"lock\n\t"
				"xadd %1, %0":
			"+m"( *pw ), "=r"( r ): // outputs (%0, %1)
			"1"( dv ): // inputs (%2 == %1)
			"memory", "cc" // clobbers
				);

			return r;
		}
	};
} 

#endif
