#ifndef _WSL_TASK_INTERFFACE_H_
#define  _WSL_TASK_INTERFFACE_H_

/*! \class Task
    \brief A Task base class
 *
 *  A Task base class works as a base class of command pattern
 *  For more information about command pattern, please visit:
 *  http://en.wikipedia.org/wiki/Command_pattern
 */
class TaskIf
{
public:

  /**
   * virtual destructor for inheritance
   */
  virtual ~TaskIf() { /* empty */}

  /**
   * Override this function to implement your specific logic
   * @param thread_context  thead's specific context
   * @param app_context    application specific context
   */
  virtual void execute(
    TaskThread* thread_context,
    void*       app_context) = 0;

  virtual void run(__VA_ARGS__)=0;

  /**
   * Release relative resource of this task
   * It's the User's responsibility to release
   * the resource
   */
  virtual void release() = 0;
  
  /**
   * 
   * @return task info 
   */
  virtual std::string to_string(){ return "";};

protected:

  TaskIf() { /* empty */ }

};

#endif