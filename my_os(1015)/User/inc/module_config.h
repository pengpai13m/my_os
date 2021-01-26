#ifndef _MODULE_CONFIG_H
#define _MODULE_CONFIG_H

typedef struct{
	int (*init)(void);
	int (*erase)(int addr,int msg_len);
	int (*read)(int addr,char* msg,int msg_len);
	int (*write)(int addr,char* msg,int msg_len);
}module_register, *p_module_register;

typedef void (*register_callback) (p_module_register p);

void module_register_init(register_callback callback);

p_module_register module_init(void);

#endif
