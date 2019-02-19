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


typedef struct JAdiosGetData JAdiosGetData;

//DESIGN! Should every put get a new batch? should metadata and data be in the same batch?
//DESIGN! should the semantics be passed?
int j_adios_put(Metadata* metadata, void* datapointer, JBatch* batch, JSemantics* semantics, gboolean use_batch){

	bson_t* kv_data;
	g_autoptr(JKV) kv_object = NULL;
	g_autoptr(JObject) data_object = NULL;
	JOperation* op_write = NULL;
	guint64 bytes_written = 0; //nb = bytes written; see benchmark

	/* Create data object */
	data_object = j_object_new("adios_put_data", metadata->name);
	j_object_create(data_object, batch);
	fprintf(stdout, "jadios: Object create \n");

	if(use_batch){
		j_batch_execute(batch); //DESIGN: good idea?
		fprintf(stdout, "jadios: Batch execute \n");
	}

	//dummy -> buffer FIXME there should be some checks... probably
	j_object_write(data_object, datapointer, metadata->data_size, 0, &bytes_written, batch);
	fprintf(stdout, "Written name: %s\n", metadata->name);

	/* Create kv object*/
	kv_object = j_kv_new("adios_put_metadata", metadata->name);

	kv_data = bson_new(); //FIXME parameters

	/* short BSON HOW TO ---------
	* http://mongoc.org/libbson/current/bson_t.html
	* bson_append_value (b, key, (int) strlen (key), (val))
	* key_length: The length of key in bytes, or -1 to determine the length with strlen(). */
	// bson_append_value(kv_data, metadata->name, (int) strlen (metadata->name), datapointer );
	bson_append_value(kv_data, metadata->name, -1, datapointer );
	j_kv_put(kv_object, kv_data, batch);


	//j_kv_unref(); //FIXME should be called somewhere

	//PSEUDO kv_put(metadata->name,datapointer);
	printf("jadios: Put\n");
	//smd_backend_store_metadata(); //TODO Straßberger
	return 0;
}

int j_adios_get(Metadata* metadata, void* datapointer, JBatch* batch, JSemantics* semantics, gboolean use_batch){
	//PSEUDO return kv_get(metadata->name);
	// bson_t* kv_data;
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

	// kv_data = bson_new(); //FIXME parameters

	// // bson_append_value(kv_data, metadata->name, -1, datapointer );
	// // bson_get_data (const bson_t *bson);
	// j_kv_get(kv_object, kv_data, batch);


	//PSEUDO kv_get(metadata->name,datapointer);
	printf("jadios: Get\n");
	//smd_backend_get_metadata(); //TODO Straßberger
	return NULL;
}

int j_adios_init(JuleaInfo* julea_info){
	printf("Global Metadata Manager: Get\n");
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

int j_adios_finish(void){
	printf("YOU MANAGED TO GET TO J GMM FINISH :) WUHU \n");
	//PSEUDO create new kv
	//create new object store
	//DESIGN: additional parameters needed?s

	return 0;
}

int j_adios_delete(Metadata* metadata, JBatch* batch){
	//j_kv_delete (JKV*, JBatch*);
	//j_adios_get()
	// j_kv_delete(object, batch);
	return 0;
}