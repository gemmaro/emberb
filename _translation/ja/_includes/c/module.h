VALUE mFoo;
VALUE mBar;

/* 最上位のモジュールFoo */
mFoo = rb_define_module("Foo");
/* 入れ子のモジュールFoo::Bar */
mBar = rb_define_module_under(mFoo, "Bar");
