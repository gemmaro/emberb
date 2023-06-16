void some_function()
{
	/* ... */

	VALUE result;

	int state;
	result = rb_protect(dangerous_func, dangerous_arg, &state);

	if (state)
	{
		/* 例外に対処 */
	}
	else
	{
		/* 例外の発生無し */
	}

	/* ... */
}
