#ifndef _WSL_RUNNABLE_H_
#define _WSL_RUNNABLE_H_


#include "meta.h"
#include "ref.h"

namespace wsl
{
	struct runnable
	{
		virtual void run()=0;
		virtual ~runnable(){};
	};

	namespace runnable_detail
	{
		template<typename FUN>
		class fun_call:public runnable
		{
			FUN m_fun;
		public:
			fun_call(FUN f):m_fun(f){}
			virtual void run(){m_fun();}
		};
	}
}

#endif