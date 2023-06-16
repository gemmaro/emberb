FIXNUM_P(obj);        /* RB_TYPE_P(obj, T_FIXNUM)と同様 */
RB_FLOAT_TYPE_P(obj); /* RB_TYPE_P(obj, T_FLOAT)と同様 */
SYMBOL_P(obj);        /* RB_TYPE_P(obj, T_SYMBOL)と同様 */
NIL_P(obj);           /* RB_TYPE_P(obj, T_NIL)と同様 */
RTEST(obj);           /* objが「真値」のときtrueを返す。つまりnilでもfalseでもありません */
