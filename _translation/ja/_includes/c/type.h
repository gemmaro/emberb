VALUE obj;

switch (TYPE(obj))
{
	case T_NIL:
		/* NilClassを扱う */
		break;
	case T_FIXNUM:
		/* Fixnumを扱う */
		break;
	case T_STRING:
		/* 文字列を扱う */
		break;
	/* ... */
}
