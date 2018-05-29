/*
    Qalculate (GTK+ UI)

    Copyright (C) 2003-2007, 2008, 2016  Hanna Knutsson (hanna_k@fmgirl.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <unistd.h>

#include "support.h"
#include "interface.h"
#include "callbacks.h"
#include "main.h"

MathStructure *mstruct, *matrix_mstruct, *parsed_mstruct, *parsed_tostruct, *displayed_mstruct;
bool prev_result_approx;
string *parsed_to_str;
KnownVariable *vans[5];
GtkWidget *functions_window;
string selected_function_category;
MathFunction *selected_function;
GtkWidget *variables_window;
string selected_variable_category;
Variable *selected_variable;
string result_text, parsed_text;
GtkWidget *units_window;
string selected_unit_category;
string selected_unit_selector_category;
Unit *selected_unit, *selected_to_unit;
bool load_global_defs, fetch_exchange_rates_at_startup, first_time, showing_first_time_message;
cairo_surface_t *surface_result;
GdkPixbuf *pixbuf_result;
extern bool b_busy, b_busy_command, b_busy_result, b_busy_expression;
extern vector<string> recent_functions_pre;
extern vector<string> recent_variables_pre;
extern vector<string> recent_units_pre;
extern GtkWidget *expression;
extern PrintOptions printops;

GtkBuilder *main_builder, *argumentrules_builder, *csvimport_builder, *csvexport_builder, *setbase_builder, *datasetedit_builder, *datasets_builder, *decimals_builder;
GtkBuilder *functionedit_builder, *functions_builder, *matrixedit_builder, *matrix_builder, *namesedit_builder, *nbases_builder, *plot_builder, *precision_builder;
GtkBuilder *preferences_builder, *unit_builder, *unitedit_builder, *units_builder, *unknownedit_builder, *variableedit_builder, *variables_builder;
GtkBuilder *periodictable_builder;

FILE *view_pipe_r, *view_pipe_w, *command_pipe_r, *command_pipe_w;
pthread_t view_thread, command_thread;
pthread_attr_t view_thread_attr, command_thread_attr;
bool command_thread_started;
string calc_arg;

bool do_timeout, check_expression_position;
gint expression_position;

static GOptionEntry options[] = {
	{"new-instance", 'n', 0, G_OPTION_ARG_NONE, NULL, N_("Start a new instance of the application"), NULL},
	{G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_STRING_ARRAY, NULL, N_("Expression to calculate"), N_("EXPRESSION")},
	{NULL}
};

void create_application(GtkApplication *app) {

	gtk_window_set_default_icon_from_file(PACKAGE_DATA_DIR "/pixmaps/qalculate.png", NULL);
	
	b_busy = false; 
	b_busy_result = false;
	b_busy_expression = false;
	b_busy_command = false;

	main_builder = NULL; argumentrules_builder = NULL; 
	csvimport_builder = NULL; datasetedit_builder = NULL; datasets_builder = NULL; decimals_builder = NULL; functionedit_builder = NULL; 
	functions_builder = NULL; matrixedit_builder = NULL; matrix_builder = NULL; namesedit_builder = NULL; nbases_builder = NULL; plot_builder = NULL; 
	precision_builder = NULL; preferences_builder = NULL; unit_builder = NULL; 
	unitedit_builder = NULL; units_builder = NULL; unknownedit_builder = NULL; variableedit_builder = NULL; 
	variables_builder = NULL;	csvexport_builder = NULL; setbase_builder = NULL; periodictable_builder = NULL;

	//create the almighty Calculator object
	new Calculator();
	
	CALCULATOR->setExchangeRatesWarningEnabled(false);
	
	//load application specific preferences
	load_preferences();

	mstruct = new MathStructure();
	displayed_mstruct = new MathStructure();
	parsed_mstruct = new MathStructure();
	parsed_tostruct = new MathStructure();
	parsed_tostruct->setUndefined();
	matrix_mstruct = new MathStructure();
	prev_result_approx = false;
	parsed_to_str = new string;

	bool canplot = CALCULATOR->canPlot();
	bool canfetch = CALCULATOR->canFetch();

	//create main window
	create_main_window();

	g_application_set_default(G_APPLICATION(app));
	gtk_window_set_application(GTK_WINDOW(gtk_builder_get_object(main_builder, "main_window")), app);

	while(gtk_events_pending()) gtk_main_iteration();

	showing_first_time_message = first_time;

	if(!calc_arg.empty()) {
		gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(main_builder, "expression")), calc_arg.c_str());
	} else if(first_time) {		
		PangoLayout *layout = gtk_widget_create_pango_layout(GTK_WIDGET(gtk_builder_get_object(main_builder, "resultview")), NULL);
		GdkRGBA rgba;
#if GDK_MAJOR_VERSION > 3 || GDK_MINOR_VERSION >= 22
		GdkDrawingContext *gdc = gdk_window_begin_draw_frame(gtk_widget_get_window(GTK_WIDGET(gtk_builder_get_object(main_builder, "resultview"))), cairo_region_create());
		cairo_t *cr = gdk_drawing_context_get_cairo_context(gdc);
#else
		cairo_t *cr = gdk_cairo_create(gtk_widget_get_window(GTK_WIDGET(gtk_builder_get_object(main_builder, "resultview"))));
#endif
		pango_layout_set_markup(layout, _("<span font=\"10\">Type a mathematical expression above, e.g. \"5 + 2 / 3\",\nand press the enter key.</span>"), -1);
		gtk_style_context_get_color(gtk_widget_get_style_context(GTK_WIDGET(gtk_builder_get_object(main_builder, "resultview"))), gtk_widget_get_state_flags(GTK_WIDGET(gtk_builder_get_object(main_builder, "resultview"))), &rgba);
		cairo_move_to(cr, 6, 0);
		gdk_cairo_set_source_rgba(cr, &rgba);
		pango_cairo_show_layout(cr, layout);
		g_object_unref(layout);
#if GDK_MAJOR_VERSION > 3 || GDK_MINOR_VERSION >= 22
		gdk_window_end_draw_frame(gtk_widget_get_window(GTK_WIDGET(gtk_builder_get_object(main_builder, "resultview"))), gdc);
#else
		cairo_destroy(cr);
#endif
	}

	while(gtk_events_pending()) gtk_main_iteration();	

	//exchange rates
	
	if(fetch_exchange_rates_at_startup && canfetch) {
		fetch_exchange_rates(5);
		while(gtk_events_pending()) gtk_main_iteration();
		CALCULATOR->loadExchangeRates();
	} else if(!CALCULATOR->loadExchangeRates() && first_time && canfetch) {
		GtkWidget *edialog = gtk_message_dialog_new(GTK_WINDOW(gtk_builder_get_object(main_builder, "main_window")), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, _("You need to download exchange rates to be able to convert between different currencies. You can later get current exchange rates by selecting “Update Exchange Rates” under the File menu.\n\nDo you want to fetch exchange rates now from the Internet?"));
		int question_answer = gtk_dialog_run(GTK_DIALOG(edialog));
		gtk_widget_destroy(edialog);
		while(gtk_events_pending()) gtk_main_iteration();
		if(question_answer == GTK_RESPONSE_YES) {
			fetch_exchange_rates(5);
			while(gtk_events_pending()) gtk_main_iteration();
			CALCULATOR->loadExchangeRates();
		}
	}

	string ans_str = _("ans");
	vans[0] = (KnownVariable*) CALCULATOR->addVariable(new KnownVariable(CALCULATOR->temporaryCategory(), ans_str, m_undefined, _("Last Answer"), false));
	vans[0]->addName(_("answer"));
	vans[0]->addName(ans_str + "1");
	vans[1] = (KnownVariable*) CALCULATOR->addVariable(new KnownVariable(CALCULATOR->temporaryCategory(), ans_str + "2", m_undefined, _("Answer 2"), false));
	vans[2] = (KnownVariable*) CALCULATOR->addVariable(new KnownVariable(CALCULATOR->temporaryCategory(), ans_str + "3", m_undefined, _("Answer 3"), false));
	vans[3] = (KnownVariable*) CALCULATOR->addVariable(new KnownVariable(CALCULATOR->temporaryCategory(), ans_str + "4", m_undefined, _("Answer 4"), false));
	vans[4] = (KnownVariable*) CALCULATOR->addVariable(new KnownVariable(CALCULATOR->temporaryCategory(), ans_str + "5", m_undefined, _("Answer 5"), false));

	//load global definitions
	if(load_global_defs && !CALCULATOR->loadGlobalDefinitions()) {
		g_print(_("Failed to load global definitions!\n"));
	}

	//load local definitions
	CALCULATOR->loadLocalDefinitions();

	//reset
	functions_window = NULL;
	selected_function_category = _("All");
	selected_function = NULL;
	variables_window = NULL;
	selected_variable_category = _("All");
	selected_variable = NULL;
	units_window = NULL;
	selected_unit_category = _("All");
	selected_unit = NULL;
	selected_to_unit = NULL;
	result_text = "0";
	parsed_text = "0";
	surface_result = NULL;
	pixbuf_result = NULL;

	//check for calculation errros regularly
	do_timeout = true;
	g_timeout_add(100, on_display_errors_timeout, NULL);
	
	check_expression_position = true;
	expression_position = 1;
	g_timeout_add(50, on_check_expression_position_timeout, NULL);
	
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (main_builder, "menu_item_plot_functions")), canplot);
	gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object (main_builder, "menu_item_fetch_exchange_rates")), canfetch);

	//create dynamic menus
	generate_units_tree_struct();

	generate_functions_tree_struct();
	generate_variables_tree_struct();
	create_fmenu();
	create_vmenu();	
	create_umenu();
	//create_pmenu();	
	create_umenu2();
	create_pmenu2();

	update_unit_selector_tree();

	for(int i = ((int) recent_functions_pre.size()) - 1; i >= 0; i--) {
		function_inserted(CALCULATOR->getActiveFunction(recent_functions_pre[i]));
	}
	for(int i = ((int) recent_variables_pre.size()) - 1; i >= 0; i--) {
		variable_inserted(CALCULATOR->getActiveVariable(recent_variables_pre[i]));
	}
	for(int i = ((int) recent_units_pre.size()) - 1; i >= 0; i--) {
		Unit *u = CALCULATOR->getActiveUnit(recent_units_pre[i]);
		if(!u) u = CALCULATOR->getCompositeUnit(recent_units_pre[i]);
		unit_inserted(u);
	}

	update_completion();
	
	int pipe_wr[] = {0, 0};
	pipe(pipe_wr);
	view_pipe_r = fdopen(pipe_wr[0], "r");
	view_pipe_w = fdopen(pipe_wr[1], "w");
	pthread_attr_init(&view_thread_attr);
	pthread_create(&view_thread, &view_thread_attr, view_proc, view_pipe_r);
	
	int pipe_wr2[] = {0, 0};
	pipe(pipe_wr2);
	command_pipe_r = fdopen(pipe_wr2[0], "r");
	command_pipe_w = fdopen(pipe_wr2[1], "w");
	pthread_attr_init(&command_thread_attr);
	command_thread_started = false;
	
	if(!calc_arg.empty()) {
		execute_expression();
	} else if(!first_time) {
		int base = printops.base;
		printops.base = 10;
		setResult(NULL, false, false, false);
		printops.base = base;
	}

	gchar *gstr = g_build_filename(g_get_home_dir(), ".qalculate", "accelmap", NULL);
	gtk_accel_map_load(gstr);
	g_free(gstr);
}

static void qalculate_activate(GtkApplication *app) {

	GList *list;

	list = gtk_application_get_windows (app);
	
	if(list) {
		gtk_window_present(GTK_WINDOW(list->data));
		return;
	}
	create_application(app);
	
}

static gint qalculate_handle_local_options(GtkApplication *app, GVariantDict *options_dict) {
	gboolean b = false;
	g_variant_dict_lookup(options_dict, "new-instance", "b", &b);
	if(b) {
		g_application_set_flags(G_APPLICATION(app), G_APPLICATION_NON_UNIQUE);
	}
	return -1;
}

static gint qalculate_command_line(GtkApplication *app, GApplicationCommandLine *cmd_line) {
	GVariantDict *options_dict = g_application_command_line_get_options_dict(cmd_line);
	gchar **remaining = NULL;
	g_variant_dict_lookup(options_dict, G_OPTION_REMAINING, "^as", &remaining);
	calc_arg = "";
	for(int i = 0; remaining && i < (int) g_strv_length(remaining); i++) {
		if(i > 1) {
			calc_arg += " ";
		}
		if(strlen(remaining[i]) >= 2 && ((remaining[i][0] == '\"' && remaining[i][strlen(remaining[i]) - 1] == '\"') || (remaining[i][0] == '\'' && remaining[i][strlen(remaining[i]) - 1] == '\''))) {
			calc_arg += remaining[i] + 1;
			calc_arg.erase(calc_arg.length() - 1);
		} else {
			calc_arg += remaining[i];
		}
	}
	if(main_builder) {
		gtk_window_present(GTK_WINDOW(gtk_builder_get_object(main_builder, "main_window")));
		if(!calc_arg.empty()) {
			gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(main_builder, "expression")), calc_arg.c_str());
			execute_expression();
		}
	} else {
		create_application(app);
	}
	return 0;
}


int main (int argc, char *argv[]) {
	
	GtkApplication *app;
	gint status;
	
#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	setlocale(LC_ALL, "");

	app = gtk_application_new("org.gtk.qalculate", G_APPLICATION_HANDLES_COMMAND_LINE);
	g_application_add_main_option_entries(G_APPLICATION(app), options);
	g_signal_connect(app, "activate", G_CALLBACK(qalculate_activate), NULL);
	g_signal_connect(app, "handle_local_options", G_CALLBACK(qalculate_handle_local_options), NULL);
	g_signal_connect(app, "command_line", G_CALLBACK(qalculate_command_line), NULL);

	status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);

	return status;
	
}



