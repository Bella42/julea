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

// struct JAdiosGetData
// {
// 	JCollection* collection;
// 	JAdios** item;
// };

typedef struct JAdiosGetData JAdiosGetData;

//DESIGN! Should every put get a new batch? should metadata and data be in the same batch?
//DESIGN! should the semantics be passed?
int j_gmm_put(Metadata* metadata, void* datapointer, JBatch* batch, JSemantics* semantics, gboolean use_batch){

	// bson_t* kv_data;
	// g_autoptr(JKV) kv_object = NULL;
	// g_autoptr(JObject) data_object = NULL;
	// JOperation* op_write = NULL;
	// guint64 nb = 0; //FIXME: what exactly does this do?

	// /* Create data object */
	// data_object = j_object_new("gmm_put_data", metadata->name);
	// j_object_create(data_object, batch);
	// fprintf(stdout, "Object create \n");

	// if(use_batch){
	// 	j_batch_execute(batch); //DESIGN: good idea?
	// 	fprintf(stdout, "Batch execute \n");
	// }


	// //dummy -> buffer FIXME there should be some checks... probably
	// //nb? FIXME
	// j_object_write(data_object, datapointer, metadata->data_size, 0, &nb, batch);
	// fprintf(stdout, "Written name: %s\n", metadata->name);

	// /* Create kv object*/
	// kv_object = j_kv_new("gmm_put_metadata", metadata->name);

	// kv_data = bson_new(); //FIXME parameters
	// bson_append_value(kv_data, metadata->name, (int) strlen (metadata->name), datapointer );
	// j_kv_put(kv_object, kv_data, batch);




	// //PSEUDO kv_put(metadata->name,datapointer);
	// printf("Global Metadata Manager: Put\n");
	// //md_backend_store_metadata(); //TODO Straßberger
	return 0;
}

void* j_gmm_get(Metadata* metadata){
	//PSEUDO return kv_get(metadata->name);
	(void) metadata;
	return NULL;
}

int j_gmm_init(JuleaInfo* julea_info){
	// printf("Global Metadata Manager: Get\n");
	// // printf("YOU MANAGED TO GET TO J GMM INIT :) WUHU \n");
	// //PSEUDO create new kv
	// //create new object store
	// //DESIGN: additional parameters needed?s
	// g_autoptr(JBatch) batch = NULL;
	// g_autoptr(JSemantics) semantics = NULL;
	// g_autoptr(JObject) object = NULL;

	// g_autofree gchar* name = NULL;
	// JOperation* op_write = NULL;
	// gboolean use_batch = false;
	// semantics = j_semantics_new (J_SEMANTICS_TEMPLATE_POSIX);
	return 0;
}

int j_gmm_finish(void){
	printf("YOU MANAGED TO GET TO J GMM FINISH :) WUHU \n");
	//PSEUDO create new kv
	//create new object store
	//DESIGN: additional parameters needed?s

	return 0;
}
