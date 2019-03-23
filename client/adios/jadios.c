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

/**
 * \defgroup JAdios Item
 *
 * Data structures and functions for managing items.
 *
 * @{
 **/
int print_float_data(void* data_pointer);
void var_metadata_to_bson(Metadata* metadata, bson_t* bson_meta_data);
void attr_metadata_to_bson(AttributeMetadata* attr_metadata, bson_t* bson_meta_data);

int print_float_data(void* data_pointer)
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
 * Put metadata to passed bson file
 *
 * @param [r] metadata   		metadata to be stored
 * @param [r] bson_meta_data 	bson file of kv store
 */
void
var_metadata_to_bson(Metadata* metadata, bson_t* bson_meta_data)
{
	gchar* key;
	printf("-- JADIOS DEBUG PRINT: bson: metadata->shape %ld\n", metadata->shape_size);
	printf("-- JADIOS DEBUG PRINT: bson: metadata->start %ld\n", metadata->start_size);
	printf("-- JADIOS DEBUG PRINT: bson: metadata->count %ld\n", metadata->count_size);

	assert(bson_append_int64(bson_meta_data, "shape_size", -1, metadata->shape_size));
	for(guint i = 0; i < metadata->shape_size; i++)
	{
		key = g_strdup_printf("shape_%d",i);
		assert(bson_append_int64(bson_meta_data, key, -1, metadata->shape[i]));
	}

	assert(bson_append_int64(bson_meta_data, "start_size", -1, metadata->start_size));
	for(guint i = 0; i < metadata->start_size; i++)
	{
		key = g_strdup_printf("start_%d",i);
		assert(bson_append_int64(bson_meta_data, key, -1, metadata->start[i]));
	}

	assert(bson_append_int64(bson_meta_data, "count_size", -1, metadata->count_size));
	for(guint i = 0; i < metadata->count_size; i++)
	{
		key = g_strdup_printf("count_%d",i);
		assert(bson_append_int64(bson_meta_data, key, -1, metadata->count[i]));
	}

	assert(bson_append_int64(bson_meta_data, "memory_start_size", -1, metadata->memory_start_size));
	for(guint i = 0; i < metadata->memory_start_size; i++)
	{
		key = g_strdup_printf("memory_start_%d",i);
		assert(bson_append_int64(bson_meta_data, key, -1, metadata->memory_start[i]));
	}

	assert(bson_append_int64(bson_meta_data, "memory_count_size", -1, metadata->memory_count_size));
	for(guint i = 0; i < metadata->memory_count_size; i++)
	{
		key = g_strdup_printf("memory_count_%d",i);
		assert(bson_append_int64(bson_meta_data, key, -1, metadata->memory_count[i]));
	}

	assert(bson_append_int64(bson_meta_data, "steps_start", -1, metadata->steps_start));
	assert(bson_append_int64(bson_meta_data, "steps_count", -1, metadata->steps_count));
	assert(bson_append_int64(bson_meta_data, "block_id", -1, metadata->block_id));
	assert(bson_append_int64(bson_meta_data, "index_start", -1, metadata->index_start));
	assert(bson_append_int64(bson_meta_data, "element_size", -1, metadata->element_size));
	assert(bson_append_int64(bson_meta_data, "available_steps_start", -1, metadata->available_steps_start));
	assert(bson_append_int64(bson_meta_data, "available_steps_count", -1, metadata->available_steps_count));

	assert(bson_append_int64(bson_meta_data, "var_type", -1, metadata->var_type));

	assert(bson_append_int64(bson_meta_data, "data_size", -1, metadata->data_size));

	assert(bson_append_bool(bson_meta_data, "is_value", -1, metadata->is_value));
	assert(bson_append_bool(bson_meta_data, "is_single_value", -1, metadata->is_single_value));
	assert(bson_append_bool(bson_meta_data, "is_single_value", -1, metadata->is_single_value));
	assert(bson_append_bool(bson_meta_data, "is_constant_dims", -1, metadata->is_constant_dims));
	assert(bson_append_bool(bson_meta_data, "is_read_as_joined", -1, metadata->is_read_as_joined));
	assert(bson_append_bool(bson_meta_data, "is_read_as_local_value", -1, metadata->is_read_as_local_value));
	assert(bson_append_bool(bson_meta_data, "is_random_access", -1, metadata->is_random_access));
	assert(bson_append_bool(bson_meta_data, "is_first_streaming_step", -1, metadata->is_first_streaming_step));

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
	g_free(key);
}

/**
 * Put attribute metadata to passed bson file
 *
 * @param [r] metadata   		metadata to be stored
 * @param [r] bson_meta_data 	bson file of kv store
 */
void
attr_metadata_to_bson(AttributeMetadata* attr_metadata, bson_t* bson_meta_data)
{
	assert(bson_append_int64(bson_meta_data, "var_type", -1, attr_metadata->attr_type));
	assert(bson_append_int64(bson_meta_data, "number_elements", -1, attr_metadata->number_elements));
	assert(bson_append_bool(bson_meta_data, "is_single_value", -1, attr_metadata->is_single_value));
}

int j_adios_init(JuleaInfo* julea_info)
{
	printf("---* Julea Adios Client: Init\n");

	//TODO:create scheme for db
	// julea_info->semantics = j_semantics_new (J_SEMANTICS_TEMPLATE_POSIX);

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
 * The names kv is needed as the JKV does not provide "get_all_keys". The type is added because it
 * might be nice to just add the variables to m_IO.m_Variables the DataMap which needs the name and the type.
 * Currently every variable needs to be defined when initializing the EngineReader
 *
 * @param [r] name_space   	namespace of the variables = unique name of engine in m_IO
 * @param [r] metadata     	metadata struct containing the information to store in kv
 * @param [r] data_pointer 	data to be stored in object store
 * @param [r] batch 	 	batch to execute the operation in
 * @param [r] use_batch    	pass false when using deferred/asynchronous I/O; true for synchronous I/O
 * @return              	return 0 on success
 */
int j_adios_put_variable(char* name_space, Metadata* metadata, void* data_pointer, JBatch* batch, gboolean use_batch)
{
	JBatch *batch_2;
	// JBatch *batch_3;
	guint64 bytes_written = 0; //nb = bytes written; see benchmark

	bson_iter_t b_iter;
	bson_t* bson_meta_data;
	bson_t* bson_names;

	g_autoptr(JKV) kv_object_metadata = NULL;
	g_autoptr(JKV) kv_object_names = NULL;
	g_autoptr(JObject) data_object = NULL;

	gchar *string_metadata_kv;
	//gchar *string_names_kv;
	gchar *string_data_object;
	// gchar* json;

	batch_2 = j_batch_new(j_batch_get_semantics(batch));

	// print_float_data(data_pointer);

	string_data_object = g_strdup_printf("%s_variables_%s", name_space, metadata->name);
	data_object = j_object_new(string_data_object, metadata->name);

	j_object_create(data_object, batch);
	j_object_write(data_object, data_pointer, metadata->data_size, 0, &bytes_written, batch);

	string_metadata_kv = g_strdup_printf("variables_%s", name_space);
	//string_names_kv = g_strdup_printf("variable_names_%s", name_space);
	kv_object_metadata = j_kv_new(string_metadata_kv, metadata->name);
	kv_object_names = j_kv_new("variable_names", name_space);

	bson_meta_data = bson_new();
	bson_names = bson_new();

	j_kv_get(kv_object_names, bson_names, batch_2);
	j_batch_execute(batch_2);

	/* check if variable name is already in kv store */
	if(!bson_iter_init_find(&b_iter, bson_names, metadata->name))
	{
		printf("Init b_iter successfull \n");
		// bson_append_null(bson_names, metadata->name,-1);
		bson_append_int32(bson_names, metadata->name,-1, metadata->var_type);
	}
	else
	{
		printf("---* Julea Adios Client: Variable %s already in kv store. \n", metadata->name);
	}

	// json = bson_as_canonical_extended_json(bson_names, NULL);
	// g_print("bson_names before put %s\n", json);

	var_metadata_to_bson(metadata, bson_meta_data);

	j_kv_put(kv_object_metadata, bson_meta_data, batch);
	j_kv_put(kv_object_names, bson_names, batch);
	//j_smd_put_metadata(name_space, metadata, batch); //TODO use SMD backend

	if(use_batch)
	{
		j_batch_execute(batch); //DESIGN: where should this be? how often?
		printf("---* Julea Adios Client: Batch execute \n");
	}

	if(bytes_written == metadata->data_size)
	{
		printf("---* Julea Adios Client: Data written for variable '%s' \n", metadata->name);
	}
	else
	{
		printf("WARNING: only %ld bytes written instead of %d bytes! \n",
		 bytes_written , metadata->data_size);
	}
	g_free(string_metadata_kv);
	//g_free(string_names_kv);
	g_free(string_data_object);

	printf("---* Julea Adios Client: Put Variable \n");
	return 0;
}

/**
 * Put attribute
 * TODO
 * @param  name_space    [description]
 * @param  attr_metadata [description]
 * @param  data_pointer  [description]
 * @param  batch         [description]
 * @param  use_batch     [description]
 * @return               [description]
 */
int j_adios_put_attribute(char* name_space, AttributeMetadata* attr_metadata, void* data_pointer, JBatch* batch, gboolean use_batch)
{
	JBatch *batch_2;
	guint64 bytes_written = 0; //nb = bytes written; see benchmark

	bson_iter_t *b_iter = NULL;
	bson_t* bson_meta_data;
	bson_t* bson_names;

	g_autoptr(JKV) kv_object_metadata = NULL;
	g_autoptr(JKV) kv_object_names = NULL;
	g_autoptr(JObject) data_object = NULL;

	gchar *string_metadata_kv;
	// gchar *string_names_kv;
	gchar *string_data_object;

	batch_2 = j_batch_new(j_batch_get_semantics(batch));

	// print_float_data(data_pointer);

	string_data_object = g_strdup_printf("%s_attributes_%s", name_space, attr_metadata->name);
	data_object = j_object_new(string_data_object, attr_metadata->name);
	j_object_create(data_object, batch);
	// printf("---* Julea Adios Client: Object create \n");

	j_object_write(data_object, data_pointer, attr_metadata->data_size, 0, &bytes_written, batch);

	if(bytes_written == attr_metadata->data_size)
	{
		printf("---* Julea Adios Client: Data written for attribute '%s' \n", attr_metadata->name);
	}
	else
	{
		printf("WARNING: only %ld bytes written instead of %d bytes! \n",
		 bytes_written , attr_metadata->data_size);
	}

	string_metadata_kv = g_strdup_printf("attributes_%s", name_space);
	// string_names_kv = g_strdup_printf("attributes_names", name_space);

	kv_object_metadata = j_kv_new(string_metadata_kv, attr_metadata->name);
	kv_object_names = j_kv_new("attribute_names", name_space);

	bson_meta_data = bson_new();
	bson_names = bson_new();

	j_kv_get(kv_object_names, bson_names, batch_2);
	j_batch_execute(batch_2);

	/* check if variable name is already in kv store */
	if(!bson_iter_init_find(b_iter, bson_names, attr_metadata->name))
	{
		// ADIOS requires not only the name but also the type of a variable when initialising the variables
		// bson_append_null(bson_names, metadata->name,-1);
		bson_append_int32(bson_names, attr_metadata->name,-1, attr_metadata->attr_type);
	}
	else
	{
		printf("---* Julea Adios Client: Attribute %s already in kv store. \n", attr_metadata->name);
	}

	attr_metadata_to_bson(attr_metadata, bson_meta_data);
	j_kv_put(kv_object_metadata, bson_meta_data, batch);
	j_kv_put(kv_object_names, bson_names, batch);

	//j_smd_put_metadata(name_space, metadata, batch); //TODO use SMD backend

	if(use_batch)
	{
		j_batch_execute(batch); //DESIGN: where should this be? how often?
		printf("---* Julea Adios Client: Batch execute \n");
	}

	g_free(string_metadata_kv);
	// g_free(string_names_kv);
	g_free(string_data_object);

	printf("---* Julea Adios Client: Put Attribute \n");
	return 0;
}



/**
 * Get all variable names from kv store for the passed namespace.
 *
 * @param [r] name_space  namespace of the variables; defined by io.open("namespace")
 * @param [w] names       array to store the retrieved names
 * @param [w] types       array to store the retrieved variable types
 * @param [w] count_names number of names to retrieve
 * @param [r] semantics   semantics to be used
 * @return             	  returns 0 on success
 */
int j_adios_get_all_var_names_from_kv(char* name_space, char*** names, int** types, unsigned int* count_names, JSemantics* semantics)
{
	//gchar* string_names_kv;
	// gchar* json;
	bson_t* bson_names;
	bson_iter_t b_iter;

	g_autoptr(JKV) kv_object = NULL;

	JBatch* batch = j_batch_new(semantics);

	//string_names_kv = g_strdup_printf("%s_variable_names%s", name_space);
	kv_object = j_kv_new("variable_names", name_space);
	bson_names = bson_new();

	j_kv_get(kv_object, bson_names, batch);
	j_batch_execute(batch);

	// json = bson_as_canonical_extended_json(bson_names, NULL);
	// g_print("bson_names after get %s \n",json);

	*count_names = bson_count_keys(bson_names);

	*names = g_slice_alloc(*count_names * sizeof(char*));
	*types = g_slice_alloc(*count_names * sizeof(int));
	bson_iter_init(&b_iter, bson_names);

	for(unsigned int i = 0; i < *count_names; i++)
	{
		if(!bson_iter_next(&b_iter))
		{
			printf("ERROR: count of names does not match \n");
		}
		(*names)[i] = g_strdup(bson_iter_key(&b_iter));
		(*types)[i] = bson_iter_int32(&b_iter);
		printf("-- JADIOS DEBUG PRINT: get_all_var_names_from_kv DEBUG PRINT: %s\n", (*names)[i]);
		printf("-- JADIOS DEBUG PRINT: types DEBUG PRINT: %d\n", (*types)[i]);
	}

	//g_free(string_names_kv);
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
int j_adios_get_var_metadata_from_kv(char* name_space, char *var_name, Metadata* metadata, JSemantics* semantics)
{
	JBatch* batch;
	gchar* string_metadata_kv;
	gchar* key;
	bson_t* bson_metadata;
	bson_iter_t b_iter;

	g_autoptr(JKV) kv_object = NULL;
	batch = j_batch_new(semantics);

	string_metadata_kv = g_strdup_printf("variables_%s", name_space);
	kv_object = j_kv_new(string_metadata_kv, var_name);
	bson_metadata = bson_new();

	j_kv_get(kv_object, bson_metadata, batch);
	j_batch_execute(batch);

	bson_iter_init(&b_iter, bson_metadata);

	/* probably not very efficient */
	while(bson_iter_next(&b_iter))
	{
		// if(g_strcmp0(bson_iter_key(&b_iter),"shape") == 0)
		// {
		// 	//FIXME: for
		// 	*metadata->shape = bson_iter_int64(&b_iter);
		// }

		if(g_strcmp0(bson_iter_key(&b_iter),"shape_size") == 0)
		{
			metadata->shape_size = bson_iter_int64(&b_iter);
			printf("-- JADIOS DEBUG PRINT: shape_size = %ld \n", metadata->shape_size);

			bson_iter_next(&b_iter);
			if(metadata->shape_size > 0)
			{
				for(guint i = 0; i < metadata->shape_size; i++)
				{
					key = g_strdup_printf("shape_%d",i);
					if(g_strcmp0(bson_iter_key(&b_iter),key) == 0)
					{
						metadata->shape[i] = bson_iter_int64(&b_iter);
					}
					bson_iter_next(&b_iter);
				}
			}
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"start_size") == 0)
		{
			metadata->start_size = bson_iter_int64(&b_iter);
			printf("-- JADIOS DEBUG PRINT: start_size = %ld \n", metadata->start_size);

			bson_iter_next(&b_iter);
			if(metadata->start_size > 0)
			{
				for(guint i = 0; i < metadata->start_size; i++)
				{
					key = g_strdup_printf("start_%d",i);
					if(g_strcmp0(bson_iter_key(&b_iter),key) == 0)
					{
						metadata->start[i] = bson_iter_int64(&b_iter);
					}
					bson_iter_next(&b_iter);
				}
			}
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"count_size") == 0)
		{
			metadata->count_size = bson_iter_int64(&b_iter);
			printf("-- JADIOS DEBUG PRINT: count_size = %ld \n", metadata->count_size);

			bson_iter_next(&b_iter);
			if(metadata->count_size > 0)
			{
				for(guint i = 0; i < metadata->count_size; i++)
				{
					key = g_strdup_printf("count_%d",i);
					if(g_strcmp0(bson_iter_key(&b_iter),key) == 0)
					{
						metadata->count[i] = bson_iter_int64(&b_iter);
						printf("-- JADIOS DEBUG PRINT: count[%d] = %ld \n",i, metadata->count[i]);
					}
					bson_iter_next(&b_iter);
				}
			}
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"memory_start_size") == 0)
		{
			metadata->memory_start_size = bson_iter_int64(&b_iter);

			if(metadata->memory_start_size > 0)
			{
				for(guint i = 0; i < metadata->memory_start_size; i++)
				{
					key = g_strdup_printf("memory_start_%d",i);
					if(g_strcmp0(bson_iter_key(&b_iter),key) == 0)
					{
						metadata->memory_start[i] = bson_iter_int64(&b_iter);
					}
					bson_iter_next(&b_iter);
				}
			}
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"memory_count_size") == 0)
		{
			metadata->memory_count_size = bson_iter_int64(&b_iter);

			if(metadata->memory_count_size > 0)
			{
				for(guint i = 0; i < metadata->memory_count_size; i++)
				{
					key = g_strdup_printf("memory_count_%d",i);
					if(g_strcmp0(bson_iter_key(&b_iter),key) == 0)
					{
						metadata->memory_count[i] = bson_iter_int64(&b_iter);
					}
					bson_iter_next(&b_iter);
				}
			}
		}
		/* unsigned long */
		else if(g_strcmp0(bson_iter_key(&b_iter),"steps_start") == 0)
		{
			metadata->steps_start = bson_iter_int64(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"steps_count") == 0)
		{
			metadata->steps_count = bson_iter_int64(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"block_id") == 0)
		{
			metadata->block_id = bson_iter_int64(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"index_start") == 0)
		{
			metadata->index_start = bson_iter_int64(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"element_size") == 0)
		{
			metadata->element_size = bson_iter_int64(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"available_steps_start") == 0)
		{
			metadata->available_steps_start = bson_iter_int64(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"available_steps_count") == 0)
		{
			metadata->available_steps_count = bson_iter_int64(&b_iter);
		}
		/* variable_type */
		else if(g_strcmp0(bson_iter_key(&b_iter),"var_type") == 0)
		{
			metadata->var_type = (int) bson_iter_int64(&b_iter);
		}
		/* unsigned int */
		else if(g_strcmp0(bson_iter_key(&b_iter),"data_size") == 0)
		{
			metadata->data_size = bson_iter_int64(&b_iter);
		}
		/* boolean */
		else if(g_strcmp0(bson_iter_key(&b_iter),"is_value") == 0)
		{
			metadata->is_value = (bool) bson_iter_bool(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"is_single_value") == 0)
		{
			metadata->is_single_value = (bool) bson_iter_bool(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"is_constant_dims") == 0)
		{
			metadata->is_constant_dims = (bool) bson_iter_bool(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"is_read_as_joined") == 0)
		{
			metadata->is_read_as_joined = (bool) bson_iter_bool(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"is_read_as_local_value") == 0)
		{
			metadata->is_read_as_local_value = (bool) bson_iter_bool(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"is_random_access") == 0)
		{
			metadata->is_random_access = (bool) bson_iter_bool(&b_iter);
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"is_first_streaming_step") == 0)
		{
			metadata->is_first_streaming_step = (bool) bson_iter_bool(&b_iter);
		}
		/* value_type*/
		else if(g_strcmp0(bson_iter_key(&b_iter),"min_value") == 0)
		{
			if(metadata->var_type == CHAR)
			{
				metadata->min_value.character = bson_iter_int32(&b_iter);
			}
			else if(metadata->var_type == INT)
			{
				metadata->min_value.integer = bson_iter_int64(&b_iter);
			}
			else if(metadata->var_type == UNSIGNED_LONG_LONG_INT)
			{
				bson_iter_decimal128(&b_iter, (bson_decimal128_t*) metadata->min_value.ull_integer);
			}
			else if(metadata->var_type == FLOAT)
			{
				metadata->min_value.real_float = bson_iter_double(&b_iter);
			}
			else if(metadata->var_type == DOUBLE)
			{
				metadata->min_value.real_double = bson_iter_double(&b_iter);
			}
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"max_value") == 0)
		{
			if(metadata->var_type == CHAR)
			{
				metadata->max_value.character = bson_iter_int32(&b_iter);
			}
			else if(metadata->var_type == INT)
			{
				metadata->max_value.integer = bson_iter_int64(&b_iter);
			}
			else if(metadata->var_type == UNSIGNED_LONG_LONG_INT)
			{
				bson_iter_decimal128(&b_iter,(bson_decimal128_t*) metadata->max_value.ull_integer);
			}
			else if(metadata->var_type == FLOAT)
			{
				metadata->max_value.real_float = (float) bson_iter_double(&b_iter);
			}
			else if(metadata->var_type == DOUBLE)
			{
				metadata->max_value.real_double = bson_iter_double(&b_iter);
			}
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"curr_value") == 0)
		{
			if(metadata->var_type == CHAR)
			{
				metadata->curr_value.character = bson_iter_int32(&b_iter);
			}
			else if(metadata->var_type == INT)
			{
				metadata->curr_value.integer = bson_iter_int64(&b_iter);
			}
			else if(metadata->var_type == UNSIGNED_LONG_LONG_INT)
			{
				bson_iter_decimal128(&b_iter,(bson_decimal128_t*) metadata->curr_value.ull_integer);
			}
			else if(metadata->var_type == FLOAT)
			{
				metadata->curr_value.real_float = (float) bson_iter_double(&b_iter);
			}
			else if(metadata->var_type == DOUBLE)
			{
				metadata->curr_value.real_double = bson_iter_double(&b_iter);
			}
		}
		else if(g_strcmp0(bson_iter_key(&b_iter),"is_constant_dims") == 0)
		{
			metadata->is_constant_dims = (bool) bson_iter_bool(&b_iter);
		}
		else
		{
			printf("Unknown key '%s' when retrieving metadata for variable %s\n", bson_iter_key(&b_iter), metadata->name);
		}
	}

	g_free(string_metadata_kv);
	g_free(key);
	printf("---* Julea Adios Client: Get Variable Metadata \n");
	return 0;
}

/** see get_all_var_names_from kv for param description */
int j_adios_get_all_attr_names_from_kv(char* name_space, char*** names, int** types, unsigned int count_names, JSemantics* semantics)
{
	// gchar* string_names_kv;
	bson_t* bson_names;
	bson_iter_t* b_iter = NULL;

	g_autoptr(JKV) kv_object = NULL;

	JBatch* batch = j_batch_new(semantics);

	// string_names_kv = g_strdup_printf("attributes_names", name_space);
	kv_object = j_kv_new("attribute_names", name_space);
	bson_names = bson_new();

	j_kv_get(kv_object, bson_names, batch);
	count_names = bson_count_keys(bson_names);

	*names = g_slice_alloc(count_names* sizeof(char*));
	*types = g_slice_alloc(count_names* sizeof(int));
	bson_iter_init(b_iter, bson_names);

	for(unsigned int i = 0; i < count_names; i++)
	{
		if(!bson_iter_next(b_iter))
		{
			printf("ERROR: count of names does not match \n");
		}
		(*names)[i] = g_strdup(bson_iter_key(b_iter));
		(*types)[i] = bson_iter_int32(b_iter);
	}

	// g_free(string_names_kv);
	return 0;
}

/** see get_all_metadata_from kv for param description */
int j_adios_get_attr_metadata_from_kv(char* name_space, char* attr_name, AttributeMetadata* attr_metadata, JSemantics* semantics)
{
	JBatch* batch;
	gchar* string_metadata_kv;
	bson_t* bson_metadata;
	bson_iter_t* b_iter = NULL;

	g_autoptr(JKV) kv_object = NULL;
	batch = j_batch_new(semantics);

	string_metadata_kv = g_strdup_printf("attributes_%s", name_space);
	kv_object = j_kv_new(string_metadata_kv, attr_name);
	bson_metadata = bson_new();

	j_kv_get(kv_object, bson_metadata, batch);
	bson_iter_init(b_iter, bson_metadata);

	/* probably not very efficient */
	while(bson_iter_next(b_iter))
	{
		if(g_strcmp0(bson_iter_key(b_iter),"attr_type") == 0)
		{
			attr_metadata->attr_type = bson_iter_int32(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"number_elements") == 0)
		{
			attr_metadata->number_elements = bson_iter_int64(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"is_single_value") == 0)
		{
			attr_metadata->is_single_value = (bool) bson_iter_bool(b_iter);
		}
		else if(g_strcmp0(bson_iter_key(b_iter),"data_size") == 0)
		{
			attr_metadata->data_size = bson_iter_int64(b_iter);
		}
		else
		{
			printf("Unknown key '%s' when retrieving metadata for attribute %s\n", bson_iter_key(b_iter), attr_metadata->name);
		}
	}

	g_free(string_metadata_kv);
	printf("---* Julea Adios Client: Get Attribute Metadata \n");
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
int j_adios_get_all_var_names(char* name_space, char*** names, JSemantics* semantics)
{
	JBatch* batch;
	gchar* smd_name;
	//JSMD smd_object = NULL;
	batch = j_batch_new(semantics);

	smd_name = g_strdup_printf("%s_%s", "metadata-", name_space);
	// smd_object = j_smd_new("adios_metadata", metadata_kv);

	// j_smd_get_all_var_names(smd_object, names batch);
	//TODO: j_smd_get_all_var_names(name_space,names);
	printf("-- JADIOS DEBUG PRINT: Namespace %s variable name %s \n",name_space, &names[0][0][0] );
	// printf("---* Julea Adios Client: get_all_var_names (from smd) is not yet supported \n");

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
int j_adios_get_metadata(char* name_space, Metadata* metadata, JSemantics* semantics)
{
	JBatch* batch;
	gchar* smd_name;
	//JSMD smd_object = NULL;
	batch = j_batch_new(semantics);

	smd_name = g_strdup_printf("%s_%s", "metadata-", name_space);
	// smd_object = j_smd_new("adios_metadata", metadata_kv);
	//TODO: j_smd_get_metadata(name_space, metadata);

	printf("-- JADIOS DEBUG PRINT: Namespace %s, variable \n", metadata->name);
	printf("---* Julea Adios Client: Get Metadata from SMD \n");
	// printf("---* Julea Adios Client: get_metadata (from smd) is not yet supported \n");
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
int j_adios_get_var_data(char* name_space, char* variable_name, unsigned int length, void* data_pointer, JBatch* batch, gboolean use_batch)
{
	gchar *string_data_object;
	guint64 bytes_read = 0; //nb = bytes written; see benchmark
	g_autoptr(JObject) data_object = NULL;

	printf("-- JADIOS DEBUG PRINT:  get var data: length: %d\n", length);
	length = 40;

	string_data_object = g_strdup_printf("%s_variables_%s", name_space,variable_name);
	data_object = j_object_new(string_data_object, variable_name);

	j_object_read(data_object, data_pointer, length, 0, &bytes_read, batch);

	if(use_batch)
	{
		j_batch_execute(batch); //DESIGN: where should this be? how often?
		printf("---* Julea Adios Client: Batch execute \n");
	}

	if(bytes_read == length)
	{
		printf("---* Julea Adios Client: Read data for variable '%s' \n", variable_name);
	}
	else
	{
		printf("WARNING: only %ld bytes read instead of %d bytes! \n",bytes_read, length);
	}

	// print_float_data(data_pointer);
	free(string_data_object);
	return 0;
}

/**
 * Get the data for the passed attribute from object store.
 *
 * @param [r] name_space    namespace of variable; defined in io.open("namespace")
 * @param [r] variable_name name of variable
 * @param [r] length        number of bytes to read
 * @param [w] data_pointer  pointer to return data
 * @param [r] batch         batch to execute this operation in
 * @param [r] use_batch     pass false when using deferred/asynchronous I/O; true for synchronous I/O
 * @return               	returns 0 on success
 */
int j_adios_get_attr_data(char* name_space, char* attribute_name, unsigned int length, void* data_pointer, JBatch* batch, gboolean use_batch)
{
	guint64 bytes_read = 0; //nb = bytes written; see benchmark
	g_autoptr(JObject) data_object = NULL;
	gchar *string_data_object;
	string_data_object = g_strdup_printf("%s_attributes_%s", name_space,attribute_name);

	data_object = j_object_new(string_data_object, attribute_name);
	j_object_read(data_object, data_pointer, length, 0, &bytes_read, batch);
	if(bytes_read == length)
	{
		printf("---* Julea Adios Client: Read data for variable '%s' \n", attribute_name);
	}
	else
	{
		printf("WARNING: only %ld bytes read instead of %d bytes! \n",bytes_read, length);
	}

	if(use_batch)
	{
		j_batch_execute(batch); //DESIGN: where should this be? how often?
		printf("---* Julea Adios Client: Batch execute \n");
	}

	free(string_data_object);
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
int j_adios_delete_variable(char* name_space, char* var_name, JBatch* batch)
{
	g_autoptr(JKV) kv_object_metadata = NULL;
	g_autoptr(JKV) kv_object_names = NULL;
	g_autoptr(JObject) data_object = NULL;

	gchar *string_metadata_kv;
	// gchar *string_names_kv;
	gchar *string_data_object;

	string_data_object = g_strdup_printf("%s_variables_%s", name_space, var_name);
	string_metadata_kv = g_strdup_printf("variables_%s", name_space);
	// string_names_kv = g_strdup_printf("names_%s", name_space);

	data_object = j_object_new(string_data_object, var_name);
	kv_object_metadata = j_kv_new(string_metadata_kv, var_name);
	kv_object_names = j_kv_new("variable_names", name_space);

	j_object_delete(data_object, batch);
	j_kv_delete (kv_object_metadata, batch);
	j_kv_delete (kv_object_names, batch);

	j_batch_execute(batch);

	g_free(string_metadata_kv);
	// g_free(string_names_kv);
	g_free(string_data_object);

	printf("---* Julea Adios Client: Delete variable %s \n", var_name);
	return 0;
}

/**
 * Delete attribute and according metadata.
 *
 * @param [r] name_space [description]
 * @param [r] metadata   [description]
 * @param [r] batch      [description]
 * @return            [description]
 */
int j_adios_delete_attribute(char* name_space, char* var_name, JBatch* batch)
{
	g_autoptr(JKV) kv_object_metadata = NULL;
	g_autoptr(JKV) kv_object_names = NULL;
	g_autoptr(JObject) data_object = NULL;

	gchar *string_metadata_kv;
	// gchar *string_names_kv;
	gchar *string_data_object;

	string_data_object = g_strdup_printf("%s_attributes_%s", name_space, var_name);
	string_metadata_kv = g_strdup_printf("attributes_%s", name_space);
	// string_names_kv = g_strdup_printf("names_%s", name_space);

	data_object = j_object_new(string_data_object, var_name);
	kv_object_metadata = j_kv_new(string_metadata_kv, var_name);
	kv_object_names = j_kv_new("attribute_names", name_space);

	j_object_delete(data_object, batch);
	j_kv_delete (kv_object_metadata, batch);
	j_kv_delete (kv_object_names, batch);

	j_batch_execute(batch);

	g_free(string_metadata_kv);
	// g_free(string_names_kv);
	g_free(string_data_object);

	printf("---* Julea Adios Client: Delete attribute %s \n", var_name);
	return 0;
}