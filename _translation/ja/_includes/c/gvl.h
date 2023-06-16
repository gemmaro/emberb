#include <ruby/thread.h>

/* ... */

void unblocking_func(void* arg)
{
	/* どうにかしてslow_funcに早めに返るよう伝える */
}

VALUE my_method(VALUE self)
{
	/* ... */

	rb_thread_call_without_gvl(slow_func, slow_arg, unblocking_func, unblocking_arg);

	/* ... */
}
