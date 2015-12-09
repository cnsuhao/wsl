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
		template<typename FUN,typename PARA>
		class fun_call1:public runnable
		{
			FUN m_fun;
			const PARA m_para;
		public:
			fun_call1(FUN f,PARA p):m_fun(f),m_para(p){}
			virtual void run(){m_fun(m_para);}
		};

		template<typename FUN,typename PARA1,typename PARA2>
		class fun_call2:public runnable
		{
			FUN m_fun;
			const PARA1 m_para1;
			const PARA2 m_para2;
		public:
			fun_call2(FUN f,PARA1 p1,PARA2 p2):m_fun(f),m_para1(p1),m_para2(p2){}
			virtual void run(){m_fun(m_para1,m_para2);}
		};


		template < typename FUN, typename PARA1, typename PARA2, typename PARA3 >
		class fun_call3 : public runnable
		{
			FUN		m_fun;
			const PARA1	m_para1;
			const PARA2	m_para2;
			const PARA3	m_para3;
		public:
			fun_call3(FUN f, PARA1 para1, PARA2 para2, PARA3 para3)
				: m_fun(f), m_para1(para1), m_para2(para2), m_para3(para3)
			{}
			virtual void run() { m_fun(m_para1, m_para2, m_para3); }
		};

		template < typename FUN, typename PARA1, typename PARA2, typename PARA3, typename PARA4 >
		class fun_call4 : public runnable
		{
			FUN		m_fun;
			const PARA1	m_para1;
			const PARA2	m_para2;
			const PARA3	m_para3;
			const PARA4	m_para4;
		public:
			fun_call4(FUN f, PARA1 para1, PARA2 para2, PARA3 para3, PARA4 para4)
				: m_fun(f), m_para1(para1), m_para2(para2), m_para3(para3), m_para4(para4)
			{}
			virtual void run() { m_fun(m_para1, m_para2, m_para3, m_para4); }
		};

		template < typename FUN, typename PARA1, typename PARA2, typename PARA3, typename PARA4, typename PARA5 >
		class fun_call5 : public runnable
		{
			FUN		m_fun;
			const PARA1	m_para1;
			const PARA2	m_para2;
			const PARA3	m_para3;
			const PARA4	m_para4;
			const PARA5	m_para5;
		public:
			fun_call5(FUN f, PARA1 para1, PARA2 para2, PARA3 para3, PARA4 para4, PARA5 para5)
				: m_fun(f), m_para1(para1), m_para2(para2), m_para3(para3), m_para4(para4), m_para5(para5)
			{}
			virtual void run() { m_fun(m_para1, m_para2, m_para3, m_para4, m_para5); }
		};

		template < typename FUN, typename PARA1, typename PARA2, typename PARA3, typename PARA4, typename PARA5, typename PARA6 >
		class fun_call6 : public runnable
		{
			FUN		m_fun;
			const PARA1	m_para1;
			const PARA2	m_para2;
			const PARA3	m_para3;
			const PARA4	m_para4;
			const PARA5	m_para5;
			const PARA6	m_para6;
		public:
			fun_call6(FUN f, PARA1 para1, PARA2 para2, PARA3 para3, PARA4 para4, PARA5 para5, PARA6 para6)
				: m_fun(f), m_para1(para1), m_para2(para2), m_para3(para3), m_para4(para4), m_para5(para5), m_para6(para6)
			{}
			virtual void run() { m_fun(m_para1, m_para2, m_para3, m_para4, m_para5, m_para6); }
		};

	}
}

#endif