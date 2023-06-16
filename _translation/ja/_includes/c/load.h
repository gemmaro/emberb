VALUE script = rb_str_new_cstr("./foo.rb");

rb_load(script, 0);

/* あるいはrb_eval_string_protect()のように例外を制御 */
int state;
rb_load_protect(script, 0, &state);

if (state)
{
	/* 例外があった */
}
