/* ... */

void* api_func(void* api_arg)
{
	/* API関数を呼び出す */
}

void* slow_func(void* slow_arg)
{
	/* ... */

	rb_thread_call_with_gvl(api_func, api_arg);

	/* ... */
}

/* ... */
