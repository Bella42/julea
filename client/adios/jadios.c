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

	//DESIGN: additional parameters needed?s
	// g_autoptr(JBatch) batch = NULL;
	// g_autoptr(JSemantics) semantics = NULL;
	// g_autoptr(JObject) object = NULL;

	// g_autofree gchar* name = NULL;
	// gboolean use_batch = false;
	julea_info->semantics = j_semantics_new (J_SEMANTICS_TEMPLATE_POSIX);

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
/**
 * Put data and the according metadata. There is no separate function for putting only the metadata.
 * @param  name_space   namespace of the variable; defined by io.open("namespace")
 * @param  metadata     metadata struct containing the information to store in kv
 * @param  data_pointer data to be stored in object store
 * @param  batch 		batch to execute the operation in
 * @param  use_batch    pass false when using deferred/asynchronous I/O; true for synchronous I/O
 * @return              return 0 on success
 */
int j_adios_put(char* name_space, Metadata* metadata, void* data_pointer, JBatch* batch, gboolean use_batch)
{
	JBatch *batch_2;
	bson_iter_t *b_iter;
	bson_t* bson_meta_data;
	bson_t* bson_names;
	// bson_t* bson_count_names;

	g_autoptr(JKV) kv_object_metadata = NULL;
	g_autoptr(JKV) kv_object_names = NULL;
	g_autoptr(JObject) data_object = NULL;
	// g_autoptr(JKV) kv_object_count_names = NULL;

	guint64 bytes_written = 0; //nb = bytes written; see benchmark

	gchar *name_metadata_kv;
	gchar *name_names_kv;
	gchar *name_data_object;
	// gchar *count_names_kv;

	batch_2 = j_batch_new(j_batch_get_semantics(batch));

	name_data_object = g_strdup_printf("%s_%s", name_space, metadata->name);
	data_object = j_object_new("adios_data", name_data_object);
	j_object_create(data_object, batch);
	printf("Julea Adios Client: Object create \n");

	float *data = data_pointer;
	for(int i = 0; i <10; i++)
	{
		float test = data[i];
		printf("Data: [%d]= %f\n",i, test );
	}

	j_object_write(data_object, data_pointer, metadata->data_size, 0, &bytes_written, batch);

	if(bytes_written, metadata->data_size)
	{
		printf("Julea Adios Client: Written data for variable '%s' \n", metadata->name);
	}
	else
	{
		printf("WARNING: only %d bytes written instead of %d bytes! \n",bytes_written , metadata->data_size);
	}

	name_metadata_kv = g_strdup_printf("%s_%s", name_space, metadata->name);
	name_names_kv = g_strdup_printf("names_%s", name_space);
	// count_names_kv = g_strdup_printf("adios_count_names_kv_%s", name_space);

	kv_object_metadata = j_kv_new("adios_metadata", name_metadata_kv);
	kv_object_names = j_kv_new("adios_variable_names", name_names_kv);
	// kv_object_count_names = j_kv_new("adios_count_variable_names", count_names_kv);


	bson_meta_data = bson_new();
	bson_names = bson_new();
	// bson_count_names = bson_new();

	j_kv_get(kv_object_names, bson_names, batch_2);
	j_batch_execute(batch_2);

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

	j_kv_put(kv_object_metadata, bson_meta_data, batch);
	j_kv_put(kv_object_names, bson_names, batch);
	// j_kv_put(kv_object_count_names, bson_count_names, batch);

	if(use_batch)
	{
		j_batch_execute(batch); //DESIGN: where should this be? how often?
		printf("Julea Adios Client: Batch execute \n");
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
/**
 * Get all variable names for the passed namespace.
 * @param  name_space  namespace of the variables; defined by io.open("namespace")
 * @param  names       array to store the retrieved names
 * @param  count_names number of names to retrieve
 * @param  semantics   semantics to be used
 * @return             returns 0 on success
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

/**
 * Get the metadata from the kv store the passed variable.
 * The name member of the Metadata struct needs to be set since it is the key!
 * DESIGN: use julea_info struct which contains semantics?
 *
 * @param  name_space namespace of the variable
 * @param  metadata   metadata information struct; needs to be allocated
 * @param  semantics  semantics to be used
 * @return            returns 0 on success
 */
int j_adios_get_metadata(char* name_space, Metadata* metadata, JSemantics* semantics)
{
	JBatch* batch;
	gchar* metadata_kv;
	bson_iter_t* b_iter;

	g_autoptr(JKV) kv_object = NULL;
	batch = j_batch_new(semantics);

	// metadata = g_slice_alloc(sizeof(Metadata*)); //TODO: is this ok?
	metadata_kv = g_strdup_printf("%s_%s", name_space, metadata->name);
	kv_object = j_kv_new("adios_metadata", metadata_kv);
	bson_t* bson_metadata = bson_new();

	j_kv_get(kv_object, bson_metadata, batch);
	bson_iter_init(b_iter, bson_metadata);

	/* probably not very efficient;
	however this function is not supposed to be used when SMD backend is ready */
	while(bson_iter_next(b_iter))
	{
		if(g_strcmp0(bson_iter_key(b_iter),"shape") == 0)
		{
			metadata->shape = bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"start") == 0)
		{
			metadata->start = bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"count") == 0)
		{
			metadata->count = bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"steps_start") == 0)
		{
			metadata->steps_start = bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"steps_count") == 0)
		{
			metadata->steps_count = bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"is_value") == 0)
		{
			metadata->is_value = (bool) bson_iter_bool(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"data_size") == 0)
		{
			metadata->data_size = bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"var_type") == 0)
		{
			metadata->var_type = (int) bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"min_value") == 0)
		{
			if(metadata->var_type == CHAR)
			{
				metadata->min_value.character = bson_iter_int32(b_iter);
			}
			else if(metadata->var_type == INT)
			{
				metadata->min_value.integer = bson_iter_int64(b_iter);
			}
			else if(metadata->var_type == UNSIGNED_LONG_LONG_INT)
			{
				bson_iter_decimal128(b_iter,metadata->min_value.ull_integer);
			}
			else if(metadata->var_type == FLOAT)
			{
				metadata->min_value.real_float = bson_iter_double(b_iter);
			}
			else if(metadata->var_type == DOUBLE)
			{
				metadata->min_value.real_double = bson_iter_double(b_iter);
			}
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"max_value") == 0)
		{
			if(metadata->var_type == CHAR)
			{
				metadata->max_value.character = bson_iter_int32(b_iter);
			}
			else if(metadata->var_type == INT)
			{
				metadata->max_value.integer = bson_iter_int64(b_iter);
			}
			else if(metadata->var_type == UNSIGNED_LONG_LONG_INT)
			{
				bson_iter_decimal128(b_iter,metadata->max_value.ull_integer);
			}
			else if(metadata->var_type == FLOAT)
			{
				metadata->max_value.real_float = (float) bson_iter_double(b_iter);
			}
			else if(metadata->var_type == DOUBLE)
			{
				metadata->max_value.real_double = bson_iter_double(b_iter);
			}
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"curr_value") == 0)
		{
			if(metadata->var_type == CHAR)
			{
				metadata->curr_value.character = bson_iter_int32(b_iter);
			}
			else if(metadata->var_type == INT)
			{
				metadata->curr_value.integer = bson_iter_int64(b_iter);
			}
			else if(metadata->var_type == UNSIGNED_LONG_LONG_INT)
			{
				bson_iter_decimal128(b_iter,metadata->curr_value.ull_integer);
			}
			else if(metadata->var_type == FLOAT)
			{
				metadata->curr_value.real_float = (float) bson_iter_double(b_iter);
			}
			else if(metadata->var_type == DOUBLE)
			{
				metadata->curr_value.real_double = bson_iter_double(b_iter);
			}
		}
		else
		{
			printf("Unknown key '%s' when retrieving metadata for variable %s\n", bson_iter_key(b_iter), metadata->name);
		}
	}
	printf("Julea Adios Client: Get Metadata \n");
	return 0;
}

/**
 * Get the data for the passed variable from object store
 * @param  name_space    namespace of variable; defined in io.open("namespace")
 * @param  variable_name name of variable
 * @param  length        number of bytes to read
 * @param  data_pointer  pointer to return data
 * @param  batch         batch to execute this operation in
 * @param  use_batch     pass false when using deferred/asynchronous I/O; true for synchronous I/O
 * @return               returns 0 on success
 */
int j_adios_get_data(char* name_space, char* variable_name, unsigned int length, void* data_pointer, JBatch* batch, gboolean use_batch)
{
	guint64 bytes_read = 0; //nb = bytes written; see benchmark
	g_autoptr(JObject) data_object = NULL;
	gchar *name_data_object;
	name_data_object = g_strdup_printf("%s_%s", name_space,variable_name);

	data_object = j_object_new("adios_data", name_data_object);
	j_object_read(data_object, data_pointer, length, 0, &bytes_read, batch);
	if(bytes_read, length)
	{
		printf("Julea Adios Client: Read data for variable '%s' \n", variable_name);
	}
	else
	{
		printf("WARNING: only %d bytes read instead of %d bytes! \n",bytes_read, length);
	}

	if(use_batch)
	{
		j_batch_execute(batch); //DESIGN: where should this be? how often?
		printf("Julea Adios Client: Batch execute \n");
	}
	return 0;
}


int j_adios_delete(char* name_space, Metadata* metadata, JBatch* batch)
{
	//j_kv_delete (JKV*, JBatch*);
	//j_adios_get()
	// j_kv_delete(object, batch);
	return 0;
}