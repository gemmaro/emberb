/* 通常 */
rb_define_method(klass, "my_method", my_method, argc);
/* または（Kernelでメソッドを定義することによる）最上位のdefと同様 */
rb_define_global_function("my_method", my_method, argc);

/* もしくはアクセス制御付き */
rb_define_private_method(klass, "my_method", my_method, argc);
rb_define_protected_method(klass, "my_method", my_method, argc);

/* もしくはシングルトンクラス中で */
rb_define_singleton_method(object, "my_method", my_method, argc);
