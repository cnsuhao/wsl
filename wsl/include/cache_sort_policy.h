
#ifndef __WSL_CACHE_SORT_POLICY_H__
#define __WSL_CACHE_SORT_POLICY_H__

#include "meta.h"

namespace wsl
{
struct null_policy_type
{
	typedef null_policy_type			policy_type1;
	typedef null_policy_type			policy_type2;
};
struct sort_by_create_time
{
	typedef sort_by_create_time			policy_type1;
	typedef null_policy_type			policy_type2;
}; 
struct sort_by_last_access_time
{
	typedef sort_by_last_access_time	policy_type1;
	typedef null_policy_type			policy_type2;
};

/** policy list.
 *  wrap two policy, it can be sort_by_create_time, sort_by_last_access_time or <sort_by_create_time, sort_by_last_access_time>
 */
template < typename _Policy1, typename _Policy2 = null_policy_type >
struct policy_list
{
	enum { flag = is_same_type<_Policy2, sort_by_create_time>::value };

	// make sure sort_by_create_time is first when it exist
	typedef	typename type_selector<flag, _Policy2, _Policy1>::Type	policy_type1;
	typedef	typename type_selector<flag, _Policy1, _Policy2>::Type	policy_type2;
	//typedef	_Policy1	policy_type1;
	//typedef	_Policy2	policy_type2;

	//WBL_STATIC_ASSERT((!is_same_type<policy_type1, null_policy_type>::value));
	//WBL_STATIC_ASSERT((!is_same_type<policy_type1, policy_type2>::value));
};

template < typename _PolicyList, typename _Policy >
struct is_contain_policy
{
	enum 
	{
		value = is_same_type<_Policy, typename _PolicyList::policy_type1>::value 
			|| is_same_type<_Policy, typename _PolicyList::policy_type2>::value 
	};
};
}

#endif
