/*
 * JULEA - Flexible storage framework
 * Copyright (C) 2010-2018 Michael Kuhn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file
 **/

#include <julea-config.h>

#include <glib.h>

#include <string.h>

#include <bson.h>

// #include <item/jitem.h>
// #include <item/jitem-internal.h>

// #include <item/jcollection.h>
// #include <item/jcollection-internal.h>
#include <adios/jadios.h>

#include <julea.h>
#include <julea-internal.h>
#include <julea-kv.h>
#include <julea-object.h>

/**
 * \defgroup JAdios Item
 *
 * Data structures and functions for managing items.
 *
 * @{
 **/


int j_adios_init(JuleaInfo* julea_info)
{
	printf("Julea Adios Client: Init\n");
	// printf("YOU MANAGED TO GET TO J GMM INIT :) WUHU \n");
	//PSEUDO create new kv
	//create new object store
	//DESIGN: additional parameters needed?s
	g_autoptr(JBatch) batch = NULL;
	g_autoptr(JSemantics) semantics = NULL;
	g_autoptr(JObject) object = NULL;

	g_autofree gchar* name = NULL;
	JOperation* op_write = NULL;
	gboolean use_batch = false;
	semantics = j_semantics_new (J_SEMANTICS_TEMPLATE_POSIX);


	return 0;
}

int j_adios_finish(void)
{
	printf("YOU MANAGED TO GET TO J GMM FINISH :) WUHU \n");
	//PSEUDO create new kv
	//create new object store
	//DESIGN: additional parameters needed?s

	return 0;
}

//DESIGN! Should every put get a new batch? should metadata and data be in the same batch?
//DESIGN! should the semantics be passed?
int j_adios_put(char* name_space, Metadata* metadata, void* data_pointer, JBatch* batch1, gboolean use_batch)
{
	JBatch *batch2;
	bson_iter_t *b_iter;
	bson_t* bson_meta_data;
	bson_t* bson_names;
	// bson_t* bson_count_names;

	g_autoptr(JKV) kv_object_metadata = NULL;
	g_autoptr(JKV) kv_object_names = NULL;
	// g_autoptr(JKV) kv_object_count_names = NULL;
	g_autoptr(JObject) data_object = NULL;

	JOperation* op_write = NULL;
	guint64 bytes_written = 0; //nb = bytes written; see benchmark

	gchar *metadata_kv;
	gchar *names_kv;
	// gchar *count_names_kv;

	batch2 = j_batch_new(j_batch_get_semantics(batch1));
	/* Create data object */
	data_object = j_object_new("adios_put_data", metadata->name);
	j_object_create(data_object, batch1);
	fprintf(stdout, "Julea Adios Client: Object create \n");


	float *data = data_pointer;
	for(int i = 0; i <10; i++)
	{
		float test = data[i];
		printf("Data: [%d]= %f\n",i, test );
	}

	//dummy -> buffer FIXME there should be some checks... probably
	j_object_write(data_object, data_pointer, metadata->data_size, 0, &bytes_written, batch1);
	fprintf(stdout, "Written name: %s\n", metadata->name);

	metadata_kv = g_strdup_printf("adios_metadata_kv_%s", name_space);
	names_kv = g_strdup_printf("adios_names_kv_%s", name_space);
	// count_names_kv = g_strdup_printf("adios_count_names_kv_%s", name_space);

	/* Create kv object*/
	kv_object_metadata = j_kv_new("adios_put_metadata", metadata_kv);
	kv_object_names = j_kv_new("adios_variable_names", names_kv);
	// kv_object_count_names = j_kv_new("adios_count_variable_names", count_names_kv);


	bson_meta_data = bson_new();
	bson_names = bson_new();
	// bson_count_names = bson_new();

	j_kv_get(kv_object_names, bson_names, batch2);
	j_batch_execute(batch2);

	/* check if variable name is already in kv store */
	if(!b_iter_init_find(b_iter, bson_names, metadata->name))
	{
		int count_names = 0;

		bson_append_null(bson_names, metadata->name,-1);
		// j_kv_get(kv_object_count_names, bson_count_names, batch2);
		// j_batch_execute(batch2);
		// if(b_iter_init_find(b_iter, bson_count_names, "count"))
		// {
		// 	count_names = bson_iter_int32(b_iter);
		// }
		// count_names++;
	}
	else
	{
		printf("Julea Adios Client: Variable %s already in kv store. \n", metadata->name);
	}

	/* short BSON HOW TO ---------
	* http://mongoc.org/libbson/current/bson_t.html
	* bson_append_utf8 (b, key, (int) strlen (key), (val))
	* key_length: The length of key in bytes, or -1 to determine the length with strlen(). */
	//TODO: check return value!
	bson_append_int64(bson_meta_data, "shape", -1, metadata->shape);
	bson_append_int64(bson_meta_data, "start", -1, metadata->start);
	bson_append_int64(bson_meta_data, "count", -1, metadata->count);
	bson_append_int64(bson_meta_data, "steps_start", -1, metadata->steps_start);
	bson_append_int64(bson_meta_data, "steps_count", -1, metadata->steps_count);
	bson_append_bool(bson_meta_data, "is_value", -1, metadata->is_value);
	bson_append_int64(bson_meta_data, "data_size", -1, metadata->data_size);
	bson_append_int64(bson_meta_data, "var_type", -1, metadata->var_type);

	/* now comes the part for "min_value" of type T in C++ */
	if(metadata->var_type == COMPOUND)
	{
		//WHAT TO DO?!
	}
	else if(metadata->var_type == UNKNOWN)
	{
		//WHAT TO DO?!
	}
	else if(metadata->var_type == STRING)
	{
		// bson_append_array()?
	}
	else if(metadata->var_type == CHAR)
	{
		bson_append_int32(bson_meta_data, "min_value", -1, metadata->min_value.integer);
		bson_append_int32(bson_meta_data, "max_value", -1, metadata->max_value.integer);
		bson_append_int32(bson_meta_data, "curr_value", -1, metadata->curr_value.integer);
	}
	else if(metadata->var_type == SIGNED_CHAR)
	{

	}
	else if(metadata->var_type == UNSIGNED_CHAR)
	{

	}
	else if(metadata->var_type == SHORT)
	{

	}
	else if(metadata->var_type == UNSIGNED_SHORT)
	{

	}
	else if(metadata->var_type == INT)
	{
		bson_append_int64(bson_meta_data, "min_value", -1, metadata->min_value.integer);
		bson_append_int64(bson_meta_data, "max_value", -1, metadata->max_value.integer);
		bson_append_int64(bson_meta_data, "curr_value", -1, metadata->curr_value.integer);
	}
	else if(metadata->var_type == UNSIGNED_INT)
	{

	}
	else if(metadata->var_type == LONG_INT)
	{

	}
	else if(metadata->var_type == UNSIGNED_LONG_INT)
	{

	}
	else if(metadata->var_type == LONG_LONG_INT){

	}
	else if(metadata->var_type == UNSIGNED_LONG_LONG_INT)
	{
		bson_append_decimal128(bson_meta_data, "min_value", -1, metadata->min_value.ull_integer);
		bson_append_decimal128(bson_meta_data, "max_value", -1, metadata->max_value.ull_integer);
		bson_append_decimal128(bson_meta_data, "curr_value", -1, metadata->curr_value.ull_integer);
	}
	else if(metadata->var_type == FLOAT)
	{
		bson_append_double(bson_meta_data, "min_value", -1, metadata->min_value.real_float);
		bson_append_double(bson_meta_data, "max_value", -1, metadata->max_value.real_float);
		bson_append_double(bson_meta_data, "curr_value", -1, metadata->curr_value.real_float);
	}
	else if(metadata->var_type == DOUBLE)
	{
		bson_append_double(bson_meta_data, "min_value", -1, metadata->min_value.real_double);
		bson_append_double(bson_meta_data, "max_value", -1, metadata->max_value.real_double);
		bson_append_double(bson_meta_data, "curr_value", -1, metadata->curr_value.real_double);
	}
	else if(metadata->var_type == LONG_DOUBLE)
	{

	}
	else if(metadata->var_type == FLOAT_COMPLEX)
	{

	}
	else if(metadata->var_type == DOUBLE_COMPLEX)
	{

	}

	j_kv_put(kv_object_metadata, bson_meta_data, batch1);
	j_kv_put(kv_object_names, bson_names, batch1);
	// j_kv_put(kv_object_count_names, bson_count_names, batch1);

	if(use_batch)
	{
		j_batch_execute(batch1); //DESIGN: where should this be? how often?
		fprintf(stdout, "Julea Adios Client: Batch execute \n");
	}

	printf("Julea Adios Client: Put\n");
	//smd_backend_store_metadata(); //TODO Straßberger
	return 0;
}

/**
 * Returns all variable names currently held in KV store.
 * @parameters name_space name_space for variables -> string from IO.open("namespace")
 * @return names string array of variable names
 */
int j_adios_get_all_var_names(char* name_space, char*** names, unsigned int count_names, JSemantics* semantics)
{
	g_autoptr(JKV) kv_object = NULL;
	JBatch* batch = j_batch_new(semantics);
	char* names_kv;
	bson_iter_t* b_iter;

	names_kv = g_strdup_printf("adios_names_kv_%s", name_space);
	kv_object = j_kv_new("adios_variable_names", names_kv);
	bson_t* bson_names = bson_new();

	j_kv_get(kv_object, bson_names, batch);
	count_names = bson_count_keys(bson_names);

	names = g_slice_alloc(count_names* sizeof(char*));
	bson_iter_init(b_iter, bson_names);

	for(int i = 0; i < count_names; i++)
	{
		if(!bson_iter_next(b_iter))
		{
			printf("ERROR: count of names does not match \n");
		}
		names[i] = g_strdup(bson_iter_key(b_iter));
	}
	return 0;
}

int j_adios_get_metadata(char* name_space, Metadata** metadata)
{
	//PSEUDO return kv_get(metadata->name);
	// bson_t* bson_meta_data;
	// g_autoptr(JKV) kv_object = NULL;
	// g_autoptr(JObject) data_object = NULL;
	// JOperation* op_write = NULL;
	// guint64 bytes_written = 0; //nb = bytes written; see benchmark

	// /* Create data object */
	// data_object = j_object_new("adios_put_data", metadata->name);
	// j_object_create(data_object, batch);
	// fprintf(stdout, "jadios: Object create \n");

	// if(use_batch){
	// 	j_batch_execute(batch); //DESIGN: good idea?
	// 	fprintf(stdout, "jadios: Batch execute \n");
	// }

	// j_object_read(data_object, datapointer, metadata->data_size, 0, &bytes_written, batch);
	// fprintf(stdout, "Read name: %s\n", metadata->name);

	// /* Create kv object*/
	// kv_object = j_kv_new("adios_get_metadata", metadata->name);

	// bson_meta_data = bson_new(); //FIXME parameters

	// // bson_append_value(bson_meta_data, metadata->name, -1, datapointer );
	// // bson_get_data (const bson_t *bson);
	// j_kv_get(kv_object, bson_meta_data, batch);


	//PSEUDO kv_get(metadata->name,datapointer);
	printf("Julea Adios Client: Get\n");
	//smd_backend_get_metadata(); //TODO Straßberger
	return NULL;
}

int j_adios_get_data(char* name_space, char* variable_name, void* datapointer, JBatch* batch, gboolean use_batch)
{
	return 0;
}


int j_adios_delete(char* name_space, Metadata* metadata, JBatch* batch)
{
	//j_kv_delete (JKV*, JBatch*);
	//j_adios_get()
	// j_kv_delete(object, batch);
	return 0;
}