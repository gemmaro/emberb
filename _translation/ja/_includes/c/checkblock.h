/* ブロックがなければLocalJumpErrorを投げる */
rb_need_block();

/* あるいはより柔軟な手法 */
if (rb_block_given_p())
{
	/* ブロックがあるときに走るコード */
}
