VALUE my_block(VALUE block_arg, VALUE data, int argc, VALUE* argv)
{
	/* block_argは最初のyieldされる値です */
	/* dataはrb_block_callに渡される最後の引数です */
	/* 複数の値がyieldされる場合、アクセスするにはargc/argvを使います */
}

void some_func()
{
	/* ... */

	VALUE obj;
	VALUE result;

	result = rb_block_call(obj, rb_intern("each"), 0, NULL, my_block, Qnil);

	/* ... */
}
