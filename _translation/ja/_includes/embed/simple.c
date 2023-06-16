#include <ruby.h>

int main(int argc, char* argv[])
{
	/* VMを構築 */
	ruby_init();

	/* Rubyはここに入る */

	/* VMを解体 */
	return ruby_cleanup(0);
}
