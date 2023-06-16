VALUE ensure_func(VALUE obj1)
{
	/* dangerous_funcの後に常に走るもの */
	return obj2;
}

/* 例外をレスキューできるようrb_ensureで梱包 */
VALUE begin_func(VALUE dangerous_arg)
{
	return rb_ensure(dangerous_func, dangerous_arg, ensure_func, Qnil);
}

VALUE rescue_func(VALUE obj1)
{
	/* 例外に対処 */
	return obj2;
}

void some_function()
{
	/* ... */

	VALUE result = rb_rescue(begin_func, dangerous_arg, rescue_func, rescue_arg);

	/* ... */
}
