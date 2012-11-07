
#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <l4/re/env.h>
#include <l4/sys/ipc.h>
#include <l4/sys/vcon.h>


static int manual_syscall_print(const char *str) {

	l4re_env_t *env;
	l4_msg_regs_t *mr;
	l4_msgtag_t tag, ret;
	size_t length;


	if(str == NULL)
		return -1;

	length = strlen(str) + 1;
	
	env = l4re_env();
	mr = l4_utcb_mr();

	mr->mr[0] = L4_VCON_WRITE_OP;
	mr->mr[1] = length;
	memcpy(&(mr->mr[2]), str, length);

	tag = l4_msgtag(L4_PROTO_LOG, (length/sizeof(mr->mr[2]) + (length%sizeof(mr->mr[2]) ? 1 : 0)) + 2, 0, 0);

	// syscall
	ret = l4_ipc_send(env->log, l4_utcb(), tag, L4_IPC_NEVER);

	return l4_msgtag_has_error(ret) ? -1 : 0;
}

int main(void) {

	for (;;) {
		//puts("Hello World!");
		manual_syscall_print("Hello World!\n");
		sleep(1);
	}
}
