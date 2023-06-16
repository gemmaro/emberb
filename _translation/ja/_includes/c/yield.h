VALUE result;

/* 値をyieldします。何もyieldしないならQundefを使います */
result = rb_yield(Qundef);
/* 複数の値をyield */
result = rb_yield_values(3, Qtrue, Qfalse, Qnil);
/* Rubyの配列をスプラットしてyield */
result = rb_yield_splat(ary);
