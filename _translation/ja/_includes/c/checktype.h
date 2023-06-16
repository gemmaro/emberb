RB_TYPE_P(obj, T_STRING);  /* objが文字列ならtrueを返す */
Check_Type(obj, T_STRING); /* あるいはobjが文字列でなければTypeErrorを投げる */
