#include <linux/unistd.h>

int main(void)
{
	syscall(333);

	return 0;
}
