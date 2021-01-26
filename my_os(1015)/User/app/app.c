#include "module_config.h"
#include "timer.h"


LINK_NODE *Timer;


/*p_flash_register*/
static p_module_register p_flash_register;

void module_register_con_add(p_module_register p)
{
	p_flash_register = p;
}

void module_register_one_test(void)
{
	module_register_init(module_register_con_add);
	p_flash_register->init();
}

/*p_module*/
static p_module_register p_module;

void module_register_two_test(void)
{
	p_module = module_init();
	p_module->init();
}


void app_heat_heart_timer_callback(void *p)
{

}

void app_timer_test(void)
{
	system_timer_creat(0,0,app_heat_heart_timer_callback);
	system_timer_start(0,APP_TIMER_MODE_REPEATED,10,NULL);
}

int main()
{	
//	module_register_one_test();
//	
//	module_register_two_test();
	
	system_timer_init();

	app_timer_test();
	while(1)
	{
		system_timer_run();
	}
}
