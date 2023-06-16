rb_require("foo");

/* もしくはRubyの文字列を使用（最初の引数は無視されます） */
rb_f_require(Qnil, rb_str_new_cstr("foo"));
