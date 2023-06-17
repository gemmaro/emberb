VALUE my_method(int argc, VALUE* argv, VALUE self)
{
	/*
	   次のようなメソッドを定義したいです。
	  
	       def my_method man1, opt1 = true, opt2 = false, *splat, man2, **opts, &blk
	*/

	VALUE man1, man2;
	VALUE opt1, opt2;
	VALUE splat;
	VALUE opts;
	VALUE blk;

	rb_scan_args(argc, argv, "12*1:&", &man1, &opt1, &opt2, &splat, &man2, &opts, &blk);

	/* オプション引数に手動で既定値を設定しなければいけません */
	if (NIL_P(opt1)) opt1 = Qtrue;
	if (NIL_P(opt2)) opt2 = Qfalse;
	/* キーワード引数が全く渡されなければoptsは（{}ではなく）nilになります */
	if (NIL_P(opts)) opts = rb_hash_new();

	/* ... */
}
