VALUE exception = rb_errinfo(); /* 直近の例外を得る */
rb_set_errinfo(Qnil);           /* 直近の例外を消去 */
