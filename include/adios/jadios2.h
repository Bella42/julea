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

#ifndef JULEA_ADIOS2_ADIOS2_H
#define JULEA_ADIOS2_ADIOS2_H

#include <glib.h>
#include <complex.h>

#include <julea.h>

G_BEGIN_DECLS

// struct JuleaInfo{
// 	JSemantics* semantics;
// 	char* name_space;
// };
// typedef struct JuleaInfo JuleaInfo;

// void j_adios_init(JuleaInfo* julea_info); //DESIGN: param needed?
// void j_adios_finish(void);

// /* performs data put AND metadata put*/
// void j_adios_put_variable(char* name_space, Metadata* metadata, void* data_pointer, JBatch* batch, gboolean use_batch);
// void j_adios_put_attribute(char* name_space, AttributeMetadata* attr_metadata, void* data_pointer, JBatch* batch, gboolean use_batch);

// /* get data from object store*/
// void j_adios_get_var_data(char* name_space, char* variable_name, unsigned int length, void* data_pointer, JBatch* batch, gboolean use_batch);
// void j_adios_get_attr_data(char* name_space, char* attribute_name, unsigned int length, void* data_pointer, JBatch* batch, gboolean use_batch);

// /* get metadata from kv store; hopefully soon from SMD backend*/
// void j_adios_get_all_var_names_from_kv(char* name_space, char*** names, int** types, unsigned int* count_names, JSemantics* semantics);
// void j_adios_get_var_metadata_from_kv(char* name_space, char* var_name, Metadata* metadata, JSemantics* semantics);

// /* get attributes from kv store; hopefully soon from SMD backend */
// void j_adios_get_all_attr_names_from_kv(char* name_space, char*** names, int** types, unsigned int count_names, JSemantics* semantics);
// void j_adios_get_attr_metadata_from_kv(char* name_space, char* var_name, AttributeMetadata* attr_metadata, JSemantics* semantics);

// void j_adios_delete_variable(char* name_space, char* var_name, JBatch* batch);
// void j_adios_delete_attribute(char* name_space, char* attr_name, JBatch* batch);

G_END_DECLS

#endif
