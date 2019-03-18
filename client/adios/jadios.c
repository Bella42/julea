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

#include <assert.h>
#include <bson.h>
#include <glib.h>
#include <string.h>

// #include <item/jitem.h>
// #include <item/jitem-internal.h>

// #include <item/jcollection.h>
// #include <item/jcollection-internal.h>
#include <adios/jadios.h>

#include <julea.h>
#include <julea-internal.h>
#include <julea-kv.h>
#include <julea-object.h>
#include <julea-smd.h>

/**
 * \defgroup JAdios Item
 *
 * Data structures and functions for managing items.
 *
 * @{
 **/
int print_float_data(void* data_pointer);
void metadata_to_bson(Metadata* metadata, bson_t* bson_meta_data);
void create_scheme_for_metadata_struct(bson_t* scheme);

int
print_float_data(void* data_pointer)
{
	float *data = data_pointer;
	for(int i = 0; i <10; i++)
	{
		float test = data[i];
		printf("Data: [%d]= %f\n",i, (double) test );
	}
	return 0;
}

/**
 * Create scheme for smd backend in a bson file.
 * The scheme is according to the current version of the metadata struct (18.03.2019).
 *
 * @param [w] scheme 	 bson file to store the scheme in
 * @return       		 returns 0 on success
 */
void
create_scheme_for_metadata_struct(bson_t* scheme)
{
	//TODO: check return value
	// int err = -1;

	assert(bson_append_int32(scheme, "name_space", -1, JSMD_TYPE_UNSIGNED_LONG_INT));
	assert(bson_append_int32(scheme, "name", -1, JSMD_TYPE_UNSIGNED_LONG_INT));

	assert(bson_append_int32(scheme, "shape", -1, JSMD_TYPE_UNSIGNED_LONG_INT));
	assert(bson_append_int32(scheme, "start", -1, JSMD_TYPE_UNSIGNED_LONG_INT));
	assert(bson_append_int32(scheme, "count", -1, JSMD_TYPE_UNSIGNED_LONG_INT));

	assert(bson_append_int32(scheme, "steps_start", -1, JSMD_TYPE_UNSIGNED_INT));
	assert(bson_append_int32(scheme, "steps_count", -1, JSMD_TYPE_UNSIGNED_INT));

	assert(bson_append_int32(scheme, "constant_shape", -1, JSMD_TYPE_BOOLEAN));
	assert(bson_append_int32(scheme, "is_value", -1, JSMD_TYPE_BOOLEAN));

	assert(bson_append_int32(scheme, "data_size", -1, JSMD_TYPE_UNSIGNED_INT));
	assert(bson_append_int32(scheme, "variable_type", -1, JSMD_TYPE_INT)); //DESIGN: which solution?
	assert(bson_append_int32(scheme, "variable_type", -1, JSMD_TYPE_VARIABLE_TYPE));

	assert(bson_append_int32(scheme, "min_value", -1, JSMD_TYPE_VALUE_TYPE));
	assert(bson_append_int32(scheme, "max_value", -1, JSMD_TYPE_VALUE_TYPE));
	assert(bson_append_int32(scheme, "curr_value", -1, JSMD_TYPE_VALUE_TYPE));
}


/**
 * Put metadata to passed bson file
 *
 * @param [r] metadata   		metadata to be stored
 * @param [r] bson_meta_data 	bson file of kv store
 */
void
metadata_to_bson(Metadata* metadata, bson_t* bson_meta_data)
{
	/* short BSON HOW TO ---------
	* http://mongoc.org/libbson/current/bson_t.html
	* bson_append_utf8 (b, key, (int) strlen (key), (val))
	* key_length: The length of key in bytes, or -1 to determine the length with strlen(). */
	//TODO: check return value!
	assert(bson_append_int64(bson_meta_data, "shape", -1, (unsigned long) metadata->shape));
	assert(bson_append_int64(bson_meta_data, "start", -1, (unsigned long) metadata->start));
	assert(bson_append_int64(bson_meta_data, "count", -1, (unsigned long) metadata->count));
	assert(bson_append_int64(bson_meta_data, "steps_start", -1, metadata->steps_start));
	assert(bson_append_int64(bson_meta_data, "steps_count", -1, metadata->steps_count));
	assert(bson_append_bool(bson_meta_data, "constant_shape", -1, metadata->constant_shape));
	assert(bson_append_bool(bson_meta_data, "is_value", -1, metadata->is_value));
	assert(bson_append_int64(bson_meta_data, "data_size", -1, metadata->data_size));
	assert(bson_append_int64(bson_meta_data, "var_type", -1, metadata->var_type));

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
		assert(bson_append_int32(bson_meta_data, "min_value", -1, metadata->min_value.integer));
		assert(bson_append_int32(bson_meta_data, "max_value", -1, metadata->max_value.integer));
		assert(bson_append_int32(bson_meta_data, "curr_value", -1, metadata->curr_value.integer));
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
		assert(bson_append_int64(bson_meta_data, "min_value", -1, metadata->min_value.integer));
		assert(bson_append_int64(bson_meta_data, "max_value", -1, metadata->max_value.integer));
		assert(bson_append_int64(bson_meta_data, "curr_value", -1, metadata->curr_value.integer));
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
		assert(bson_append_decimal128(bson_meta_data, "min_value", -1,
			(void*) metadata->min_value.ull_integer));
		assert(bson_append_decimal128(bson_meta_data, "max_value", -1,
			(void*) metadata->max_value.ull_integer));
		assert(bson_append_decimal128(bson_meta_data, "curr_value", -1,
			(void*) metadata->curr_value.ull_integer));
	}
	else if(metadata->var_type == FLOAT)
	{
		assert(bson_append_double(bson_meta_data, "min_value", -1, metadata->min_value.real_float));
		assert(bson_append_double(bson_meta_data, "max_value", -1, metadata->max_value.real_float));
		assert(bson_append_double(bson_meta_data, "curr_value", -1, metadata->curr_value.real_float));
	}
	else if(metadata->var_type == DOUBLE)
	{
		assert(bson_append_double(bson_meta_data, "min_value", -1, metadata->min_value.real_double));
		assert(bson_append_double(bson_meta_data, "max_value", -1, metadata->max_value.real_double));
		assert(bson_append_double(bson_meta_data, "curr_value", -1, metadata->curr_value.real_double));
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
}

/**
 * Initialise ADIOS client.
 * @param [r] julea_info
 * @return            [description]
 */
int
j_adios_init(JuleaInfo* julea_info)
{
	bson_t* scheme;
	JBatch* batch;

	scheme = bson_new();
	batch = j_batch_new(julea_info->semantics);

	create_scheme_for_metadata_struct(scheme);
	j_smd_apply_scheme(julea_info->name_space, scheme, batch);
	j_batch_execute(batch);

	printf("Julea Adios Client: Init\n");

	return 0;
}

/**
 * Finalise ADIOS client
 */
int
j_adios_finish(void)
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
 *
 * @param [r] name_space   	namespace of the variables = unique name of engine in m_IO
 * @param [r] metadata     	metadata struct containing the information to store in kv
 * @param [r] data_pointer 	data to be stored in object store
 * @param [r] batch 	 	batch to execute the operation in
 * @param [r] use_batch    	pass false when using deferred/asynchronous I/O; true for synchronous I/O
 * @return              	return 0 on success
 */
int
j_adios_put(char* name_space, Metadata* metadata, void* data_pointer, JBatch* batch, gboolean use_batch)
{
	JBatch *batch_2;
	guint64 bytes_written = 0; //nb = bytes written; see benchmark

	bson_iter_t *b_iter = NULL;
	bson_t* bson_metadata;
	bson_t* bson_names;

	g_autoptr(JKV) kv_object_metadata = NULL;
	g_autoptr(JKV) kv_object_names = NULL;
	g_autoptr(JObject) data_object = NULL;

	gchar *name_metadata_kv;
	gchar *name_names_kv;
	gchar *name_data_object;

	batch_2 = j_batch_new(j_batch_get_semantics(batch));

	// print_float_data(data_pointer);

	name_data_object = g_strdup_printf("%s_%s", name_space, metadata->name);
	data_object = j_object_new("adios_data", name_data_object);
	j_object_create(data_object, batch);
	// printf("Julea Adios Client: Object create \n");

	j_object_write(data_object, data_pointer, metadata->data_size, 0, &bytes_written, batch);

	if(bytes_written == metadata->data_size)
	{
		printf("Julea Adios Client: Data written for variable '%s' \n", metadata->name);
	}
	else
	{
		printf("WARNING: only %ld bytes written instead of %d bytes! \n",
		 bytes_written , metadata->data_size);
	}

	name_metadata_kv = g_strdup_printf("%s_%s", name_space, metadata->name);
	name_names_kv = g_strdup_printf("names_%s", name_space);

	kv_object_metadata = j_kv_new("adios_metadata", name_metadata_kv);
	kv_object_names = j_kv_new("adios_variable_names", name_names_kv);

	bson_metadata = bson_new();
	bson_names = bson_new();

	j_kv_get(kv_object_names, bson_names, batch_2);
	j_batch_execute(batch_2);

	/* check if variable name is already in kv store */
	if(!bson_iter_init_find(b_iter, bson_names, metadata->name))
	{
		assert(bson_append_null(bson_names, metadata->name,-1));
	}
	else
	{
		printf("Julea Adios Client: Variable %s already in kv store. \n", metadata->name);
	}

	/* using kv store */
	metadata_to_bson(metadata, bson_metadata);
	j_kv_put(kv_object_metadata, bson_metadata, batch);
	j_kv_put(kv_object_names, bson_names, batch);

	/* using smd backend */
	metadata_to_bson(metadata, bson_metadata);
	j_smd_insert(name_space, metadata->name, bson_metadata, batch); //TODO use SMD backend

	if(use_batch)
	{
		j_batch_execute(batch); //DESIGN: where should this be? how often?
		printf("Julea Adios Client: Batch execute \n");
	}

	g_free(name_metadata_kv);
	g_free(name_names_kv);
	g_free(name_data_object);

	printf("Julea Adios Client: Put\n");
	return 0;
}


/**
 * Get all variable names from kv store for the passed namespace.
 *
 * @param [r] name_space  namespace of the variables; defined by io.open("namespace")
 * @param [w] names       array to store the retrieved names
 * @param [w] count_names number of names to retrieve
 * @param [r] semantics   semantics to be used
 * @return             	  returns 0 on success
 */
int
j_adios_get_all_var_names_from_kv(char* name_space, char*** names, unsigned int count_names, JSemantics* semantics)
{
	gchar* names_kv;
	bson_t* bson_names;
	bson_iter_t* b_iter = NULL;

	g_autoptr(JKV) kv_object = NULL;

	JBatch* batch = j_batch_new(semantics);

	names_kv = g_strdup_printf("adios_names_kv_%s", name_space);
	kv_object = j_kv_new("adios_variable_names", names_kv);
	bson_names = bson_new();

	j_kv_get(kv_object, bson_names, batch);
	count_names = bson_count_keys(bson_names);

	names = g_slice_alloc(count_names* sizeof(char*));
	bson_iter_init(b_iter, bson_names);

	for(unsigned int i = 0; i < count_names; i++)
	{
		if(!bson_iter_next(b_iter))
		{
			printf("ERROR: count of names does not match \n");
		}
		names[i] = g_strdup(bson_iter_key(b_iter));
	}

	g_free(names_kv);
	return 0;
}

/**
 * Get the metadata from the kv store for the passed variable.
 *
 * @param [r] name_space 	namespace of the variable = unique engine name in m_IO
 * @param [r] var_name 		namespace of the variable = unique engine name in m_IO
 * @param [w] metadata   	metadata information struct; needs to be allocated
 * @param [r] semantics  	semantics to be used
 * @return            		returns 0 on success
 */
int
j_adios_get_metadata_from_kv(char* name_space, char *var_name, Metadata* metadata, JSemantics* semantics)
{
	JBatch* batch;
	gchar* metadata_kv;
	bson_t* bson_metadata;
	bson_iter_t* b_iter = NULL;

	g_autoptr(JKV) kv_object = NULL;
	batch = j_batch_new(semantics);

	metadata_kv = g_strdup_printf("%s_%s", name_space, var_name);
	kv_object = j_kv_new("adios_metadata", metadata_kv);
	bson_metadata = bson_new();

	j_kv_get(kv_object, bson_metadata, batch);
	bson_iter_init(b_iter, bson_metadata);

	/* probably not very efficient */
	while(bson_iter_next(b_iter))
	{
		if(g_strcmp0(bson_iter_key(b_iter),"shape") == 0)
		{
			metadata->shape = (unsigned long*) bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"start") == 0)
		{
			metadata->start = (unsigned long*) bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"count") == 0)
		{
			metadata->count = (unsigned long*) bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"steps_start") == 0)
		{
			metadata->steps_start = bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"steps_count") == 0)
		{
			metadata->steps_count = bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"constant_shape") == 0)
		{
			metadata->constant_shape = (bool) bson_iter_bool(b_iter);
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
				bson_iter_decimal128(b_iter, (bson_decimal128_t*) metadata->min_value.ull_integer);
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
				bson_iter_decimal128(b_iter,(bson_decimal128_t*) metadata->max_value.ull_integer);
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
				bson_iter_decimal128(b_iter,(bson_decimal128_t*) metadata->curr_value.ull_integer);
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

	g_free(metadata_kv);
	printf("Julea Adios Client: Get Metadata \n");
	return 0;
}

/**
 * Get all variables for the passed namespace.
 *
 * @param [r] name_space namespace for the variables = the unique engine name in m_IO
 * @param [w] names      Array of variable names
 * @param [r] semantics  semantics to be used
 * @return            	 returns 0 on success
 */
int
j_adios_get_all_var_names(char* name_space, char*** names, JSemantics* semantics)
{
	JBatch* batch;
	gchar* smd_name;
	//JSMD smd_object = NULL;
	batch = j_batch_new(semantics);

	smd_name = g_strdup_printf("%s_%s", "metadata-", name_space);
	// smd_object = j_smd_new("adios_metadata", metadata_kv);

	// j_smd_get_all_var_names(smd_object, names batch);
	//TODO: j_smd_get_all_var_names(name_space,names);
	printf("Namespace %s variable name %s \n",name_space, &names[0][0][0] );
	// printf("Julea Adios Client: get_all_var_names (from smd) is not yet supported \n");

	j_batch_execute(batch);
	g_free(smd_name);
	return 0;
}


/**
 * Get metadata for the given name_space.
 *
 * @param [r] name_space namespace for the variables = the unique engine name in m_IO
 * @param [w] metadata   metadata information struct; needs to be allocated
 * @param [r] semantics  semantics to be used
 * @return            	 returns 0 on success
 */
int
j_adios_get_metadata(char* name_space, Metadata* metadata, JSemantics* semantics)
{
	JBatch* batch;
	gchar* smd_name;
	//JSMD smd_object = NULL;
	batch = j_batch_new(semantics);

	smd_name = g_strdup_printf("%s_%s", "metadata-", name_space);
	// smd_object = j_smd_new("adios_metadata", metadata_kv);
	//TODO: j_smd_get_metadata(name_space, metadata);

	printf("Namespace %s, variable \n", metadata->name);
	printf("Julea Adios Client: Get Metadata from SMD \n");
	// printf("Julea Adios Client: get_metadata (from smd) is not yet supported \n");
	j_batch_execute(batch);
	g_free(smd_name);
	return 0;
}


/**
 * Get the data for the passed variable from object store.
 *
 * @param [r] name_space    namespace of variable; defined in io.open("namespace")
 * @param [r] variable_name name of variable
 * @param [r] length        number of bytes to read
 * @param [w] data_pointer  pointer to return data
 * @param [r] batch         batch to execute this operation in
 * @param [r] use_batch     pass false when using deferred/asynchronous I/O; true for synchronous I/O
 * @return               	returns 0 on success
 */
int
j_adios_get_data(char* name_space, char* variable_name, unsigned int length, void* data_pointer, JBatch* batch, gboolean use_batch)
{
	guint64 bytes_read = 0; //nb = bytes written; see benchmark
	g_autoptr(JObject) data_object = NULL;
	gchar *name_data_object;
	name_data_object = g_strdup_printf("%s_%s", name_space,variable_name);

	data_object = j_object_new("adios_data", name_data_object);
	j_object_read(data_object, data_pointer, length, 0, &bytes_read, batch);
	if(bytes_read == length)
	{
		printf("Julea Adios Client: Read data for variable '%s' \n", variable_name);
	}
	else
	{
		printf("WARNING: only %ld bytes read instead of %d bytes! \n",bytes_read, length);
	}

	if(use_batch)
	{
		j_batch_execute(batch); //DESIGN: where should this be? how often?
		printf("Julea Adios Client: Batch execute \n");
	}
	return 0;
}

/**
 * Delete variable and according metadata.
 *
 * @param [r] name_space [description]
 * @param [r] metadata   [description]
 * @param [r] batch      [description]
 * @return            [description]
 */
int
j_adios_delete_variable(char* name_space, char* var_name, JBatch* batch)
{
	g_autoptr(JKV) kv_object_metadata = NULL;
	g_autoptr(JKV) kv_object_names = NULL;
	g_autoptr(JObject) data_object = NULL;

	gchar *name_metadata_kv;
	gchar *name_names_kv;
	gchar *name_data_object;

	name_data_object = g_strdup_printf("%s_%s", name_space, var_name);
	name_metadata_kv = g_strdup_printf("%s_%s", name_space, var_name);
	name_names_kv = g_strdup_printf("names_%s", name_space);

	data_object = j_object_new("adios_data", name_data_object);
	kv_object_metadata = j_kv_new("adios_metadata", name_metadata_kv);
	kv_object_names = j_kv_new("adios_variable_names", name_names_kv);

	j_object_delete(data_object, batch);
	j_kv_delete (kv_object_metadata, batch);
	j_kv_delete (kv_object_names, batch);

	j_batch_execute(batch);

	g_free(name_metadata_kv);
	g_free(name_names_kv);
	g_free(name_data_object);

	printf("Julea Adios Client: Delete variable %s \n", var_name);
	return 0;
}