#include <ruby.h>
#include <gmp.h>
#include <string.h>

/*
   Cのデータを得るためにVALUEを開封することがしょっちゅうになるでしょう。
   そこまで大変ではないですが、面倒になってきます。
   これがあるとVALUEから奥底にあるデータに直通できるようにするものです。
 */
#define UNWRAP(val, data) \
	mpz_t* data;\
	TypedData_Get_Struct(val, mpz_t, &mpz_type, data);

/*
   またGMP::Integer型のオブジェクトのみ受け付けるように大変厳密にしていきます。
   なのでこれを頻繁に検査します。
 */
#define CHECK_MPZ(val) \
	if (CLASS_OF(val) != cInteger)\
		rb_raise(rb_eTypeError, "%+"PRIsVALUE" is not a %"PRIsVALUE, val, cInteger);

/* メソッドで簡単にアクセスできるので、これらを大域的にしているのはいいことです */
VALUE mGMP;
VALUE cInteger;

/* GMP::Integerに梱包されたデータを解放する関数 */
void integer_free(void* data)
{
	/* GMPにより割り当てられたメモリを解放 */
	mpz_clear(*(mpz_t*)data);

	free(data);
}

static const rb_data_type_t mpz_type = {
	.wrap_struct_name = "gmp_mpz",
	.function = {
		.dfree = integer_free,
		/* 恐らく.dsizeを設定すべきなのでしょうがどうmpz_tに書き込めばいいか分かりません…… */
	},
	.flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

/* GMP::Integer.allocate */
VALUE integer_c_alloc(VALUE self)
{
	mpz_t* data = malloc(sizeof(mpz_t));
	/* GMPの初期化 */
	mpz_init(*data);

	return TypedData_Wrap_Struct(self, &mpz_type, data);
}

/* GMP::Integer#initialize
  
   最初の引数を使って内部のmpz_tを設定します
  
   最初の引数が文字列なら2つ目のFixnum引数を文字列を解釈するための基数として与えられます。
   既定の0の基数は基数がStringの前置詞から決定することを意味しています。
 */
VALUE integer_m_initialize(int argc, VALUE* argv, VALUE self)
{
	int base = 0;

	/* 基数のオプション引数を確認 */
	VALUE val;
	VALUE rbase;
	if (rb_scan_args(argc, argv, "11", &val, &rbase) == 2)
	{
		/* 基数は文字列のみが認識されます */
		Check_Type(val, T_STRING);
		Check_Type(rbase, T_FIXNUM);

		base = FIX2INT(rbase);

		/* GMPは特定の基数のみを受け付けます */
		if (!(base >= 2 && base <= 62) && base != 0)
			rb_raise(rb_eRangeError, "base must be 0 or in (2..62)");
	}

	UNWRAP(self, data);

	VALUE str;

	switch (TYPE(val))
	{
		case T_FIXNUM:
			/* 簡単な場合 */
			mpz_set_si(*data, FIX2LONG(val));
			return self;
		case T_BIGNUM:
			/* これが安全に変換する上で一番簡単な方法です */
			str = rb_funcall(val, rb_intern("to_s"), 0);
			base = 10;
			break;
		case T_STRING:
			str = val;
			break;
		case T_DATA:
			/* 別のGMP::Integerを複製 */
			if (CLASS_OF(val) == cInteger)
			{
				UNWRAP(val, other);

				mpz_set(*data, *other);

				return self;
			}
			/* breakは意図的に省かれています */
		default:
			rb_raise(rb_eTypeError, "%+"PRIsVALUE" is not an integer type", val);
			break;
	}

	/* 代入 */
	char* cstr = StringValueCStr(str);
	if (mpz_set_str(*data, cstr, base))
	{
		if (base == 0)
			rb_raise(rb_eArgError, "invalid number: %"PRIsVALUE, val);
		else
			rb_raise(rb_eArgError, "invalid base %d number: %"PRIsVALUE, base, val);
	}

	return self;
}

/* GMP::Integer#to_s
  
   Stringの基数（既定は10）のためにオプションのFixnum引数を受け付けます
 */
VALUE integer_m_to_s(int argc, VALUE* argv, VALUE self)
{
	int base = 10;

	/* 基数のオプション引数を確認 */
	VALUE rbase;
	if (rb_scan_args(argc, argv, "01", &rbase) == 1)
	{
		Check_Type(rbase, T_FIXNUM);

		base = FIX2INT(rbase);

		/* GMPは特定の基数のみを受け付けます */
		if (!(base >= -36 && base <= -2) && !(base >= 2 && base <= 62))
			rb_raise(rb_eRangeError, "base must be in (-36..-2) or (2..62)");
	}

	UNWRAP(self, data);

	/* GMPからCの文字列を得る */
	char* cstr = malloc(mpz_sizeinbase(*data, base) + 2);
	mpz_get_str(cstr, base, *data);

	/* Rubyの文字列を作成 */
	VALUE str = rb_str_new_cstr(cstr);

	/* メモリを解放 */
	free(cstr);

	return str;
}

/* GMP::Integer#to_i */
VALUE integer_m_to_i(VALUE self)
{
	/* 最も安全かつ簡単に変換する方法はto_s.to_iを呼び出すことです */
	return rb_funcall(integer_m_to_s(0, NULL, self), rb_intern("to_i"), 0);
}

/* GMP::Integer#<=> */
VALUE integer_m_spaceship(VALUE self, VALUE x)
{
	CHECK_MPZ(x);

	UNWRAP(self, data);
	UNWRAP(x, other);

	/* 同値なオブジェクトのための早道 */
	if (data == other)
		return INT2FIX(0);

	return INT2FIX(mpz_cmp(*data, *other));
}

/* GMP::Integer#== */
VALUE integer_m_eq(VALUE self, VALUE x)
{
	/* GMP::Integerについては<=>を使用 */
	if (CLASS_OF(x) == cInteger)
		return integer_m_spaceship(self, x) == INT2FIX(0) ? Qtrue : Qfalse;

	return rb_call_super(1, &x);
}

/* GMP::Integer#+ */
VALUE integer_m_add(VALUE self, VALUE x)
{
	CHECK_MPZ(x);

	UNWRAP(self, data);
	UNWRAP(x, other);

	/*
	   結果を補完するために新しいGMP::Integerが必要ですが、
	   実際に`new`メソッドを使う必要は全くありません。
	 */
	VALUE result = integer_c_alloc(cInteger);
	UNWRAP(result, res);

	mpz_add(*res, *data, *other);

	return result;
}
/* 乗算と減算はほぼ同様に定義されるでしょう */

/* GMP::Integer#-@ */
VALUE integer_m_neg(VALUE self)
{
	UNWRAP(self, data);

	/* +メソッド中なので`new`を迂回 */
	VALUE result = integer_c_alloc(cInteger);
	UNWRAP(result, res);

	mpz_neg(*res, *data);

	return result;
}

/* 入口 */
void Init_gmp()
{
	mGMP = rb_define_module("GMP");

	/* GMP::Integerを定義 */
	cInteger = rb_define_class_under(mGMP, "Integer", rb_cObject);
	rb_define_alloc_func(cInteger, integer_c_alloc);
	rb_define_method(cInteger, "initialize", integer_m_initialize, -1);
	rb_define_method(cInteger, "to_s", integer_m_to_s, -1);
	rb_define_method(cInteger, "to_i", integer_m_to_i, 0);
	rb_define_method(cInteger, "<=>", integer_m_spaceship, 1);
	rb_define_method(cInteger, "==", integer_m_eq, 1);
	rb_define_method(cInteger, "+", integer_m_add, 1);
	rb_define_method(cInteger, "-@", integer_m_neg, 0);

	rb_define_alias(cInteger, "inspect", "to_s");
}
