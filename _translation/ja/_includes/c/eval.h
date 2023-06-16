int state;
VALUE result;
result = rb_eval_string_protect("puts 'Hello, world!'", &state);

if (state)
{
	/* 例外に対処 */
}
