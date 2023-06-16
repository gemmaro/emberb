VALUE rescue_func(VALUE obj1)
{
	/* 例外に対処 */
	return obj2;
}

void some_function()
{
	/* ... */

	VALUE result;

	/* TypeErrorとRangeErrorをレスキューする */
	result = rb_rescue2(dangerous_func, dangerous_arg, rescue_func, rescue_arg, rb_eTypeError, rb_eRangeError, 0);

	/* ... */
}
