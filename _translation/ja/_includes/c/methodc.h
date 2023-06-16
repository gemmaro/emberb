/* 通常は必須の引数（selfを除いて16以内） */
VALUE my_method(VALUE self, VALUE arg1, VALUE arg2)
{
	/* ... */
}

/* あるいは全ての引数をRubyのArrayに丸呑みする */
VALUE my_method(VALUE self, VALUE args)
{
	/* ... */
}

/* あるいは全ての引数をCの配列として渡す */
VALUE my_method(int argc, VALUE* argv, VALUE self)
{
	/* ... */
}
