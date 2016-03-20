
#ifndef __WSL_CONCURRENT_CIRCLE_QUEUE_H__
#define __WSL_CONCURRENT_CIRCLE_QUEUE_H__

#include <stdint.h>
#include "atomicity.h"
#include  "pthread_util.h"

namespace wsl
{
namespace ccq_detail
{
	union helper
	{
		struct
		{
			uint32_t	n1;
			uint32_t	n2;
		};
		uint64_t		n64;

		explicit helper(const volatile void * p)
		{
			n64 = *((volatile uint64_t *) p);
		}
	};
}

//enum EPopIfResult { ePopIfSucc, ePopIfEmpty, ePopIfNotMatch };

template < typename T >
class concurrent_circle_queue
{
public:
	typedef uint32_t		 			size_type;
	typedef T							value_type;
	typedef value_type	*				pointer;
	typedef const value_type *			const_pointer;
	typedef value_type&					reference;
	typedef const value_type&			const_reference;
private:
	typedef std::vector<value_type>		vec_t;
	typedef ccq_detail::helper			cq_helper;

	volatile uint32_t	_head;
	volatile uint32_t	_tail;
	volatile uint32_t	_push_tail;
	volatile uint32_t	_push_count;
	vec_t				_v;

	concurrent_circle_queue(const concurrent_circle_queue&);
	concurrent_circle_queue& operator=(const concurrent_circle_queue&);
public:
	explicit concurrent_circle_queue(size_type max_item_count)
		: _head(0)
		, _tail(0)
		, _push_tail(0)
		, _push_count(0)
		, _v(max_item_count)
	{}

	bool empty() const			{ return size() == 0; }
	size_type size() const
	{
		check_fp();
		cq_helper h(&_head);
		return h.n2 - h.n1;
	}
	size_type max_size() const	{ return _v.size(); }

	void clear()
	{
		value_type v;
		while(pop(v));
	}

	template < typename Op >
	void clear(Op op)
	{
		value_type v;
		while(pop(v))
			op(v);
	}

	bool push(const value_type& v)
	{
		for (; ;) {
			check_fp();

			uint32_t pt = _push_tail;
			if (pt - _head >= _v.size())
				return false;
			uint32_t pt_new = pt + 1;
			if (atomicity::compare_and_swap(&_push_tail, pt, pt_new)) {
				_v[pt % _v.size()] = v;
				atomicity::fetch_and_add(&_push_count, (uint32_t) 1);
				atomicity::compare_and_swap(&_tail, pt, pt_new); // 若失败，则延后到下次check_fp时后移_tail
				return true;
			}
		}
	}
//*
	template < typename RI >
	bool push_all(RI first, RI last)
	{
		size_type n = std::distance(first, last);
		for (; ;) {
			check_fp();

			uint32_t pt = _push_tail;
			uint32_t pt_new = pt + n;
			if (pt_new - _head > _v.size())
				return false;
			if (atomicity::compare_and_swap(&_push_tail, pt, pt_new)) {
				uint32_t xb = pt % _v.size(), xe = pt_new % _v.size();
				if (xb < xe)
					std::copy(first, last, _v.begin() + xb);
				else {
					RI middle = first;
					std::advance(middle, _v.size() - xb);
					std::copy(first, middle, _v.begin() + xb);
					std::copy(middle, last, _v.begin());
				}
				atomicity::fetch_and_add(&_push_count, n);
				atomicity::compare_and_swap(&_tail, pt, pt_new); // 若失败，则延后到下次check_fp时后移_tail
				return true;
			}
		}
	}
//*/
	bool pop(value_type& v)
	{
		for (; ;) {
			check_fp();

			cq_helper h(&_head);
			if (h.n1 == h.n2)
				return false;
			uint32_t new_h = h.n1 + 1;
			v = _v[h.n1 % _v.size()];
			if (atomicity::compare_and_swap(&_head, h.n1, new_h))
				return true;
		}
	}
/*
	template < typename Pred >
	EPopIfResult pop_if(value_type& v, Pred pred)
	{
		for (; ;) {
			check_fp();

			cq_helper h(&_head);
			if (h.n1 == h.n2)
				return ePopIfEmpty;
			uint32_t new_h = h.n1 + 1;
			v = _v[h.n1 % _v.size()];
			if (!pred(v))
				return ePopIfNotMatch;
			if (atomicity::compare_and_swap(&_head, h.n1, new_h))
				return ePopIfSucc;
		}
	}
*/
	bool peek(value_type& v)
	{
		for (; ;) {
			check_fp();

			cq_helper h(&_head);
			if (h.n1 == h.n2)
				return false;
			v = _v[h.n1 % _v.size()];
			if (atomicity::compare_and_swap(&_head, h.n1, h.n1))
				return true;
		}
	}
private:
	/*
	void check_fp_b() const 
	{
		// 检查是否需要后移_tail
		cq_helper h(&_push_tail);
		uint32_t pt = h.n1;
		// 由于_for_push的head比tail先增加，因此当且仅当head == tail时，_for_push之前的push操作都已经完成
		if (pt == h.n2) {
			for (; ;) {
				uint32_t t = _tail;
				if (pt <= t)
					return;
				if (atomicity::compare_and_swap((volatile uint32_t *) &_tail, t, pt)) {
					syscall(__NR_futex, &_tail, FUTEX_WAKE, pt - t, NULL, NULL);
					return;
				}
			}
		}
	}
	*/
	void check_fp() const 
	{
		// 检查是否需要后移_tail
		cq_helper h(&_push_tail);
		uint32_t pt = h.n1;
		// 由于_for_push的head比tail先增加，因此当且仅当head == tail时，_for_push之前的push操作都已经完成
		if (pt == h.n2) {
			for (; ;) {
				uint32_t t = _tail;
				if (pt <= t)
					return;
				if (atomicity::compare_and_swap((volatile uint32_t *) &_tail, t, pt))
					return;
			}
		}
	}
};
}

#endif
