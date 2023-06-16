VALUE constant;

/* 定義されていなければconst_missingフックを呼ぶ */
constant = rb_const_get_at(rb_mMath, rb_intern("PI_ISH"));

/* 定義されていなければ継承の連鎖を遡る。それでも見付からなければconst_missingを呼ぶ */
constant = rb_const_get(rb_mMath, rb_intern("PI_ISH"));

/* 前と同じですが、定数がObject由来（トップレベル）に落ち着いたときに警告を印字 */
constant = rb_const_get_from(rb_mMath, rb_intern("PI_ISH"));
