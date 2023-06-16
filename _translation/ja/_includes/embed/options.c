#include <ruby.h>

int main(int argc, char* argv[])
{
	ruby_init();

	char* options[] = { "-v", "-eputs 'Hello, world!'" };
	void* node = ruby_options(2, options);

	int state;
	if (ruby_executable_node(node, &state))
	{
		state = ruby_exec_node(node);
	}

	if (state)
	{
		/* 例外に対処します。ないとは思いますが */
	}

	return ruby_cleanup(state);
}
