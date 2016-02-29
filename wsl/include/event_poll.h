/************************************************************************/
/* filename:			 event_loop.h                                                                  */
/* function:			  event�¼�����                    */
/* author :			wangchenglin                                                                  */
/* creattime :       2016/02/29															*/
/* company :		at home                                                                               */
/************************************************************************/

#ifndef _WSL_EVENT_POLL_H_
#define _WSL_EVENT_POLL_H_
namespace wsl
{
	class EventLoop;
	//poll interface
	class IEventPoll{
	public:
		IEventPoll(EventLoop *loop):
		  pLoop(loop){}
		  virtual ~IEventPoll(){}
	public:
		virtual int 		add(int fd, int mask) = 0;
		virtual int 		del(int fd, int mask) = 0;
		virtual int 		poll(struct timeval *tvp = NULL) = 0;
		virtual const char*	name() = 0;
		virtual void		resize(){}
	protected:
		EventLoop*			pLoop;
	};


	//epoll
#ifdef HAVE_EPOLL
	class EventEPoll:
		public IEventPoll
	{
	public:
		EventEPoll(EventLoop *loop);
		~EventEPoll();
	public:
		int 				add(int fd, int mask);
		int 				del(int fd, int mask);
		int 				poll(struct timeval *tvp = NULL);
		const char*			name(){ return "epoll";}
		void				resize();
	private:
		int					epfd;
		struct epoll_event*	events;
	};
#endif

	//kqueue
#ifdef HAVE_KQUEUE
	class EventKqueue:
		public IEventPoll
	{
	public:
		EventKqueue(EventLoop *loop);
		~EventKqueue();
	public:
		int 				add(int fd, int mask);
		int 				del(int fd, int mask);
		int 				poll(struct timeval *tvp = NULL);
		const char*			name(){ return "kqueue";}
		void				resize();
	private:
		int					kqfd;
		struct kevent*		events;
	};
#endif

	//select
#ifdef HAVE_SELECT
	class EventSelect:
		public IEventPoll
	{
	public:
		EventSelect(EventLoop *loop);
		~EventSelect();
	public:
		int 				add(int fd, int mask);
		int 				del(int fd, int mask);
		int 				poll(struct timeval *tvp = NULL);
		const char*			name(){ return "select";}
	private:
		fd_set				rfds;
		fd_set				wfds;
		fd_set				_rfds;
		fd_set				_wfds;
	};
#endif
}
#endif