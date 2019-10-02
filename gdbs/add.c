#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "mydbus.h"


static void
callback_add_async(GObject *proxy,
                   GAsyncResult *res,
                   gpointer user_data)
{
	g_print("callback_add_async called!\n");
	gint retval;
 	GError *error;
	error = NULL;
	
	my_dbus_calculator_call_add_finish(MY_DBUS_CALCULATOR(proxy), &retval, res, &error);

	if (error == NULL)
	{
		g_print("Answer = %d\n", retval);
		exit(0);
	}
	else
	g_print("ERROR!!\n");
}

int main(int argc, char **argv)
{
	MyDBusCalculator * proxy;
	GError *error = NULL;
	gint retval;

	if (argc < 3)
	{
		printf("Usage: ./add num1 num2\n");
		return 0;
	}

	proxy = my_dbus_calculator_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, 0, "com.vishnu", "/", NULL, &error);

	gint a, b;
	a = atoi(argv[1]);
	b = atoi(argv[2]);

	error = NULL;
	my_dbus_calculator_call_add(proxy, a, b, NULL, callback_add_async, &error);

 	GMainLoop *loop;
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_object_unref(proxy);

	return 0;

	
}
