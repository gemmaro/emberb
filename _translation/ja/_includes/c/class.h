VALUE cFoo;
VALUE cBar;

/* 最上位のクラスFoo < Object */
cFoo = rb_define_class("Foo", rb_cObject);
/* 入れ子のクラスFoo::Bar < Array */
cBar = rb_define_class_under(cFoo, "Bar", rb_cArray);
