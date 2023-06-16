VALUE global;

VALUE global_getter(ID id)
{
	/* 何らかのVALUEを返す。多くの場合大域変数に基づく */
}

void global_setter(VALUE val, ID id)
{
	/* 大域変数を設定する。多くの場合valに基づく */
}

void some_func()
{
	/* ... */

	/* まず大域変数を初期化すること */

	/* $wはRubyで自由に変えられます */
	rb_define_variable("$w", &global);

	/* Rubyで新しい値を$xに代入するとNameErrorが投げられます */
	rb_define_readonly_variable("$x", &global);

	/* 特定の関数を通じてですが、$yはRubyで自由に変えられます */
	rb_define_hooked_variable("$y", &global, global_getter, global_setter);

	/* 前と同じですが対応するVALUEがありませんよ */
	rb_define_virtual_variable("$z", global_getter, global_setter)

	/* ... */
}
