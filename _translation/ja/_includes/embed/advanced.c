#include <ruby.h>

int main(int argc, char* argv[])
{
	if (ruby_setup())
	{
		/* Rubyなしのコードを走らせる */
	}
	else
	{
		/* Rubyはここに入る */

		ruby_finalize(); /* XXX 絶対にここで例外をrescueすること */
	}

	return 0;
}
