#include <ruby/thread.h>

void* slow_func(void* slow_arg)
{
	/* APIを全く使わない低速なコード */
}

VALUE my_method(VALUE self)
{
	/* 引数の譲渡、API関連など */

	rb_thread_call_without_gvl(slow_func, slow_arg, NULL, NULL);

	/* さらにAPI関連のもの。slow_funcの結果をVALUEに変えることが多い */
}
