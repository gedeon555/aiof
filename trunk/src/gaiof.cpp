#include <gtk/gtk.h>
#include <glade/glade.h>
#include <stdio.h>
#include <vector>
#include "recognition_engine.h"
#include "interface.h"
#include "typedefs.h"

using namespace std;

GladeXML *xml;

void on_main_learn_button_clicked (GtkWidget * widget)
{
	GtkWidget *learn_dialog;
	GtkWidget *to_clear;
	GtkTreeModel *list;

	learn_dialog = glade_xml_get_widget (xml, "learn_dialog");

	to_clear = glade_xml_get_widget (xml, "learn_dialog_class_name_entry");
	gtk_entry_set_text (GTK_ENTRY (to_clear), "");

	to_clear = glade_xml_get_widget (xml, "learn_dialog_save_button_label");
	gtk_label_set_text (GTK_LABEL (to_clear), "");

	to_clear = glade_xml_get_widget (xml, "learn_dialog_samples_treeview");
	list = gtk_tree_view_get_model (GTK_TREE_VIEW (to_clear));
	gtk_list_store_clear (GTK_LIST_STORE (list));

	gtk_widget_show (learn_dialog);
}

void on_learn_dialog_samples_add_button_clicked (GtkWidget * widget)
{
	GtkWidget *dialog;
	GtkTreeModel *list;
	GtkTreeIter iter;
	GSList   *filenames;
	int       i;

	dialog = glade_xml_get_widget (xml, "add_files_dialog");
	gtk_file_chooser_unselect_all (GTK_FILE_CHOOSER (dialog));

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		filenames = gtk_file_chooser_get_filenames (GTK_FILE_CHOOSER (dialog));

		for (i = 0; i < g_slist_length (filenames); ++i)
		{
			list = gtk_tree_view_get_model (GTK_TREE_VIEW (widget));

			gtk_list_store_append (GTK_LIST_STORE (list), &iter);
			gtk_list_store_set (GTK_LIST_STORE (list), &iter,
								0, g_slist_nth_data (filenames, i), -1);

			g_free (g_slist_nth_data (filenames, i));
		}

		g_slist_free (filenames);
	}

	gtk_widget_hide (dialog);
}

void on_learn_dialog_samples_remove_button_clicked (GtkWidget * widget)
{
	GtkTreeModel *list;
	GtkTreeIter iter;
	GtkTreePath *path;
	GtkTreeViewColumn *column;

	list = gtk_tree_view_get_model (GTK_TREE_VIEW (widget));
	gtk_tree_view_get_cursor (GTK_TREE_VIEW (widget), &path, &column);

	if (path != NULL)
	{
		gtk_tree_model_get_iter (list, &iter, path);
		gtk_list_store_remove (GTK_LIST_STORE (list), &iter);

		gtk_tree_path_free (path);
	}
}

void on_learn_dialog_save_button_clicked (GtkWidget * widget)
{
	GtkWidget *dialog;
	GtkWidget *label;
	gchar    *filename;

	dialog = glade_xml_get_widget (xml, "save_file_dialog");
	label = glade_xml_get_widget (xml, "learn_dialog_save_button_label");

	gtk_file_chooser_unselect_all (GTK_FILE_CHOOSER (dialog));

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		gtk_label_set_text (GTK_LABEL (label), filename);
	}

	gtk_widget_hide (dialog);
}

void on_learn_dialog_learn_button_clicked (GtkWidget * widget)
{
	GtkWidget *progress_dialog;
    GtkWidget *learn_dialog;
    GtkWidget *finish_button;
	GtkWidget *label;
    GtkWidget *samples_treeview;
    GtkWidget *database_treeview;
    GtkWidget *class_name_entry;
    GtkWidget *save_file_dialog;
    GtkTreeModel *list;
    GtkTreeIter iter;

    vector < string > samples;
    gboolean valid;
    string class_name;
    string file_name;
    gchar *value;

	progress_dialog = glade_xml_get_widget (xml, "progress_dialog");
    learn_dialog = glade_xml_get_widget (xml, "learn_dialog");
    finish_button = glade_xml_get_widget (xml, "progress_dialog_finish");
	label = glade_xml_get_widget (xml, "progress_dialog_label");
    samples_treeview = glade_xml_get_widget (xml, "learn_dialog_samples_treeview");
    database_treeview = glade_xml_get_widget (xml, "database_dialog_treeview");
    class_name_entry = glade_xml_get_widget (xml, "learn_dialog_class_name_entry");
    save_file_dialog = glade_xml_get_widget (xml, "save_file_dialog");

    gtk_widget_set_sensitive(finish_button, FALSE);

    list = gtk_tree_view_get_model(GTK_TREE_VIEW (samples_treeview));

    valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL (list), &iter);
    class_name = gtk_entry_get_text (GTK_ENTRY (class_name_entry));
    file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (save_file_dialog));

    while (valid)
    {
        gtk_tree_model_get (GTK_TREE_MODEL (list), &iter, 0, &value, -1);
        samples.push_back (value);
        g_free (value);
        valid = gtk_tree_model_iter_next (GTK_TREE_MODEL (list), &iter);
    }

    gtk_widget_hide(learn_dialog);
    gtk_widget_show(progress_dialog);

    gtk_main_iteration();

    Recognition_engine::get_engine()->learn(samples, class_name);
    Recognition_engine::get_engine()->save(class_name, file_name);

    list = gtk_tree_view_get_model(GTK_TREE_VIEW (database_treeview));
    gtk_list_store_append (GTK_LIST_STORE (list), &iter);
	gtk_list_store_set (GTK_LIST_STORE (list), &iter, 0, class_name.c_str(), -1);
  
    gtk_widget_set_sensitive(finish_button, TRUE);
}

void on_main_open_button_clicked (GtkWidget * widget)
{
	GtkWidget *dialog;
    GtkWidget *treeview;
    GtkWidget *database;
	GtkTreeModel *list;
	GtkTreeIter iter;
	GSList   *filenames;
    gchar *filename;
    string class_name;
	int       i;

	dialog = glade_xml_get_widget (xml, "open_dialog");
    treeview = glade_xml_get_widget (xml, "database_dialog_treeview");
    database = glade_xml_get_widget (xml, "database_dialog");

	gtk_file_chooser_unselect_all (GTK_FILE_CHOOSER (dialog));

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
      filenames = gtk_file_chooser_get_filenames (GTK_FILE_CHOOSER (dialog));

      for (i = 0; i < g_slist_length (filenames); ++i)
		{
            filename = (gchar*) g_slist_nth_data (filenames, i);
            class_name = Recognition_engine::get_engine()->load(filename);
			list = gtk_tree_view_get_model (GTK_TREE_VIEW (treeview));

			gtk_list_store_append (GTK_LIST_STORE (list), &iter);
			gtk_list_store_set (GTK_LIST_STORE (list), &iter,
								0, class_name.c_str(), -1);

			g_free (g_slist_nth_data (filenames, i));
		}

		g_slist_free (filenames);
    }

    gtk_widget_hide(dialog);
    gtk_widget_show(database);
}

void on_main_recognize_button_clicked (GtkWidget * widget)
{
    GtkWidget *dialog;

	dialog = glade_xml_get_widget (xml, "recognize_dialog");
	gtk_widget_show (dialog);
}

void on_recognize_dialog_add_button_clicked(GtkWidget * widget)
{
    GtkWidget *dialog;
	GtkTreeModel *list;
	GtkTreeIter iter;
	GSList   *filenames;
	int       i;

	dialog = glade_xml_get_widget (xml, "add_files_dialog");
	gtk_file_chooser_unselect_all (GTK_FILE_CHOOSER (dialog));

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		filenames = gtk_file_chooser_get_filenames (GTK_FILE_CHOOSER (dialog));

		for (i = 0; i < g_slist_length (filenames); ++i)
		{
			list = gtk_tree_view_get_model (GTK_TREE_VIEW (widget));

			gtk_list_store_append (GTK_LIST_STORE (list), &iter);
			gtk_list_store_set (GTK_LIST_STORE (list), &iter,
								0, g_slist_nth_data (filenames, i), -1);

			g_free (g_slist_nth_data (filenames, i));
		}

		g_slist_free (filenames);
	}

	gtk_widget_hide (dialog);
}

void on_recognize_dialog_remove_button_clicked (GtkWidget * widget)
{
	GtkTreeModel *list;
	GtkTreeIter iter;
	GtkTreePath *path;
	GtkTreeViewColumn *column;

	list = gtk_tree_view_get_model (GTK_TREE_VIEW (widget));
	gtk_tree_view_get_cursor (GTK_TREE_VIEW (widget), &path, &column);

	if (path != NULL)
	{
		gtk_tree_model_get_iter (list, &iter, path);
		gtk_list_store_remove (GTK_LIST_STORE (list), &iter);

		gtk_tree_path_free (path);
	}
}

void on_recognize_dialog_recognize_button_clicked(GtkWidget * widget)
{
    GtkWidget *dialog;
    GtkWidget *progress_dialog;
    GtkWidget *recognize_dialog;
    GtkWidget *treeview;
    GtkWidget *finish_button;
	GtkListStore *list;
	GtkTreeViewColumn *column;
    vector < string > classes;
    vector < string > files;
    vector < string >::iterator it, jt;
    t_results results;
    gboolean valid;
    GtkTreeIter iter;
    gchar *value;
    ostringstream oss;
    GValue str = {0, };
    GType *types;
    int i, n;
  
    dialog = glade_xml_get_widget (xml, "recognize_results_dialog");
    progress_dialog = glade_xml_get_widget (xml, "progress_dialog");
    recognize_dialog = glade_xml_get_widget (xml, "recognize_dialog");
    treeview = glade_xml_get_widget (xml, "database_dialog_treeview");
    list = GTK_LIST_STORE (gtk_tree_view_get_model(GTK_TREE_VIEW (treeview)));
    valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL (list), &iter);
    finish_button = glade_xml_get_widget (xml, "progress_dialog_finish");

    while (valid)
    {
        gtk_tree_model_get (GTK_TREE_MODEL (list), &iter, 0, &value, -1);
        classes.push_back (value);
        g_free (value);
        valid = gtk_tree_model_iter_next (GTK_TREE_MODEL (list), &iter);
    }
  
    treeview = glade_xml_get_widget (xml, "recognize_dialog_treeview");
    list = GTK_LIST_STORE (gtk_tree_view_get_model(GTK_TREE_VIEW (treeview)));
    valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL (list), &iter);

    gtk_widget_hide(recognize_dialog);
    gtk_widget_show(dialog);
    gtk_widget_show(progress_dialog);

    for (n = 0; valid; ++n)
        valid = gtk_tree_model_iter_next (GTK_TREE_MODEL (list), &iter);

    valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL (list), &iter);

    for (i = 0; valid; ++i)
    {
        gtk_tree_model_get (GTK_TREE_MODEL (list), &iter, 0, &value, -1);
        results[value] = Recognition_engine::get_engine()->recognize(value);
        files.push_back(value);
        g_free (value);
        valid = gtk_tree_model_iter_next (GTK_TREE_MODEL (list), &iter);
        progress_bar_set_total_value(i / (float)(n));
    }
    progress_bar_set_total_value(1);

    types = (GType *) malloc((classes.size() + 1) * sizeof (GType));
    types[0] = G_TYPE_STRING;

    for (i = 0; i < classes.size(); ++i)
        types[i + 1] = G_TYPE_STRING;

    treeview = glade_xml_get_widget (xml, "recognize_results_dialog_treeview");

	list = gtk_list_store_newv (classes.size() + 1, types);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), GTK_TREE_MODEL (list));

    column = gtk_tree_view_column_new_with_attributes ("Processed files", gtk_cell_renderer_text_new (), "text", 0, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);
  
    for (it = classes.begin(), i = 1; it != classes.end(); ++it, ++i)
    {
        column = gtk_tree_view_column_new_with_attributes (it->c_str(), gtk_cell_renderer_text_new (), "text", i, NULL);
        gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);
    }

    g_value_init(&str, G_TYPE_STRING);
    for (it = files.begin(); it != files.end(); ++it)
    {
      g_value_set_string(&str, it->c_str());
      gtk_list_store_append (GTK_LIST_STORE (list), &iter);
      gtk_list_store_set_value (GTK_LIST_STORE (list), &iter, 0, &str);

      for (jt = classes.begin(), i = 1; jt != classes.end(); ++jt, ++i)
      {
        oss.str("");
        oss << round(results[*it][*jt]) << " %";
        g_value_set_string(&str, oss.str().c_str());
        gtk_list_store_set_value (GTK_LIST_STORE (list), &iter, i, &str);
      }
    }

  gtk_widget_set_sensitive(finish_button, TRUE);
}

int gaiof_main (int argc, char **argv)
{
	GtkWidget *widget;
	GtkWidget *treeview;
	GtkListStore *list;
	GtkTreeViewColumn *column;

	gtk_init (&argc, &argv);
	xml = glade_xml_new ("src/aiof.glade", NULL, NULL);

	/* get a widget (useful if you want to change something) */
	widget = glade_xml_get_widget (xml, "main_window");

	/* connect signal handlers */
    glade_xml_signal_connect(xml, "on_main_learn_button_clicked", G_CALLBACK (on_main_learn_button_clicked));
    glade_xml_signal_connect(xml, "on_learn_dialog_samples_add_button_clicked", G_CALLBACK (on_learn_dialog_samples_add_button_clicked));
    glade_xml_signal_connect(xml, "on_learn_dialog_samples_remove_button_clicked", G_CALLBACK (on_learn_dialog_samples_remove_button_clicked));
    glade_xml_signal_connect(xml, "on_learn_dialog_learn_button_clicked", G_CALLBACK (on_learn_dialog_learn_button_clicked));
    glade_xml_signal_connect(xml, "on_learn_dialog_save_button_clicked", G_CALLBACK (on_learn_dialog_save_button_clicked));

    glade_xml_signal_connect(xml, "on_main_open_button_clicked", G_CALLBACK (on_main_open_button_clicked));
  
    glade_xml_signal_connect(xml, "on_main_recognize_button_clicked", G_CALLBACK (on_main_recognize_button_clicked));
    glade_xml_signal_connect(xml, "on_recognize_dialog_add_button_clicked", G_CALLBACK (on_recognize_dialog_add_button_clicked));
    glade_xml_signal_connect(xml, "on_recognize_dialog_remove_button_clicked", G_CALLBACK (on_recognize_dialog_remove_button_clicked));
    glade_xml_signal_connect(xml, "on_recognize_dialog_recognize_button_clicked", G_CALLBACK (on_recognize_dialog_recognize_button_clicked));
  
    glade_xml_signal_connect(xml, "gtk_main_quit", G_CALLBACK (gtk_main_quit));
    glade_xml_signal_connect(xml, "gtk_widget_hide", G_CALLBACK (gtk_widget_hide));
    glade_xml_signal_connect(xml, "gtk_widget_show", G_CALLBACK (gtk_widget_show));

    /* initialize the samples treeview with a model */
	treeview = glade_xml_get_widget (xml, "learn_dialog_samples_treeview");

	list = gtk_list_store_new (1, G_TYPE_STRING);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), GTK_TREE_MODEL (list));

	column = gtk_tree_view_column_new_with_attributes ("Files to learn", gtk_cell_renderer_text_new (), "text", 0, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

    /* initialize the database treeview with a model */
	treeview = glade_xml_get_widget (xml, "database_dialog_treeview");

	list = gtk_list_store_new (1, G_TYPE_STRING);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), GTK_TREE_MODEL (list));

	column = gtk_tree_view_column_new_with_attributes ("Classes learned", gtk_cell_renderer_text_new (), "text", 0, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

    /* initialize the recognize treeview with a model */
	treeview = glade_xml_get_widget (xml, "recognize_dialog_treeview");

	list = gtk_list_store_new (1, G_TYPE_STRING);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), GTK_TREE_MODEL (list));

	column = gtk_tree_view_column_new_with_attributes ("Files to process", gtk_cell_renderer_text_new (), "text", 0, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);

	gtk_widget_show (widget);

	gtk_main ();

	return 0;
}

void progress_bar_set_local_value(float value)
{
  GtkWidget *progress_bar;
  
  progress_bar = glade_xml_get_widget (xml, "progress_dialog_local_progressbar");

  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR (progress_bar), value);

  gtk_main_iteration();
}

void progress_bar_set_total_value(float value)
{
  GtkWidget *progress_bar;
  
  progress_bar = glade_xml_get_widget (xml, "progress_dialog_total_progressbar");

  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR (progress_bar), value);

  gtk_main_iteration();
}

void progress_bar_set_current_text(string text)
{
  GtkWidget *progress_bar;
  
  progress_bar = glade_xml_get_widget (xml, "progress_dialog_local_progressbar");
  
  gtk_progress_bar_set_text(GTK_PROGRESS_BAR (progress_bar), text.c_str());
  
  gtk_main_iteration();
}
