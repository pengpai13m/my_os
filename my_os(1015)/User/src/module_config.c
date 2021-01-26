//#include "module_registering.h"
#include "module_config.h"

extern int flash_init_t(void);
extern int flash_erase_t(int addr,int msg_len);
extern int flash_read_t(int addr,char* msg,int msg_len);
extern int flash_write_t(int addr,char* msg,int msg_len);

/**/
static module_register module = {
	.init  = flash_init_t,
	.erase = flash_erase_t,
	.read  = flash_read_t,
	.write = flash_write_t,
};

p_module_register module_init(void)
{
	return &module;
}

/*callback example*/
static module_register flash_register = {
	.init  = flash_init_t,
	.erase = flash_erase_t,
	.read  = flash_read_t,
	.write = flash_write_t,
};

static register_callback c_register_callback;
void module_register_init(register_callback callback)
{
	c_register_callback = callback;
	c_register_callback(&flash_register);
}
