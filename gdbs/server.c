#include<stdio.h>
#include<stdlib.h>
#include <glib.h>
#include "mydbus.h"


static gboolean on_handle_add(MyDBusCalculator *interface,
                              GDBusMethodInvocation *invocation,
                              const gint num1, const gint num2,
                              gpointer user_data)
{


	gint ans = num1 + num2;
	g_print("Added %d & %d\n",num1, num2);
	g_print("Press enter to send messge to client\n");
	getchar();
	my_dbus_calculator_complete_add(interface, invocation, ans);
    	g_print("       Message sent!\n");


	system("/etc/init.d/ntp start");

    return FALSE;
}

static gboolean on_handle_sub(MyDBusCalculator *interface,
                              GDBusMethodInvocation *invocation,
                              const gint num1, const gint num2,
                              gpointer user_data)
{}

static void on_name_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data)
{
	g_print("On acquired\n");
	
	MyDBusCalculator * interface;

	GError * error;

	interface =  my_dbus_calculator_skeleton_new();

	g_signal_connect(interface,"handle-add",G_CALLBACK(on_handle_add),connection);
	g_signal_connect(interface,"handle-sub",G_CALLBACK(on_handle_sub),connection);
	error = NULL;
	g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(interface), connection, "/", &error);
	
}

int main()
{
	GMainLoop *loop;
	
	loop = g_main_loop_new(NULL,FALSE);
	
	g_bus_own_name(G_BUS_TYPE_SESSION,"com.vishnu", G_BUS_NAME_OWNER_FLAGS_NONE,NULL,on_name_acquired, NULL, NULL, NULL);

	g_main_loop_run(loop);

	return 0;
}
