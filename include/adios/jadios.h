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

#ifndef JULEA_ADIOS_ADIOS_H
#define JULEA_ADIOS_ADIOS_H

// #if !defined(JULEA_ITEM_H) && !defined(JULEA_ITEM_COMPILATION)
// #error "Only <julea-item.h> can be included directly."
// #endif

#include <glib.h>
#include <complex.h>

#include <julea.h>

G_BEGIN_DECLS

// ordered according to ADIOSTypes.h
enum variable_type{
	COMPOUND,
	UNKNOWN,
	STRING,
	CHAR,
	SIGNED_CHAR,
	UNSIGNED_CHAR,
	SHORT,
	UNSIGNED_SHORT,
	INT,
	UNSIGNED_INT,
	LONG_INT,
	UNSIGNED_LONG_INT,
	LONG_LONG_INT,
	UNSIGNED_LONG_LONG_INT,
	FLOAT,
	DOUBLE,
	LONG_DOUBLE,
	FLOAT_COMPLEX,
	DOUBLE_COMPLEX
};
typedef enum variable_type variable_type;

//to pass, e.g. "float min_value" an actual c type is needed; therefore this union...
union value_type{
	//compound?!
	//unknown?!
	char *string;						//probably dangerous when passing from c++
	char character;
	signed char s_character;
	unsigned char u_character;
	short shorter;
	unsigned short u_shorter;
	int integer;
	unsigned int u_integer;
	long int l_integer;
	unsigned long int ul_integer;
	long long int ll_integer;
	unsigned long long int ull_integer;
	float real_float;
	double real_double;
	long double lreal_double;
	// float complex float_complex;	//inconsistent naming I know; sorry real_float_complex is not great either
	// double complex double_complex;
};
typedef union value_type value_type;

/**
 * Metadata information to be stored in kv store or structured metadata backend.
 * TODO: namespace?!
 *
 */
struct Metadata{
	// char* name_space; //IO.open("namespace") = Unique name for Engine within m_IO
	char* name;
	const unsigned long* shape;
	const unsigned long*  start;
	const unsigned long*  count;

	size_t steps_start;
	size_t steps_count;
	bool constant_shape;
	bool is_value;

	guint data_size;
	variable_type var_type;

	union value_type min_value;
	union value_type max_value;
	union value_type curr_value;

	//operations -> how are these used? necessary for structured backend?
	//std::map<size_t, std::vector<helper::SubStreamBoxInfo>>
    //        StepBlockSubStreamsInfo;
};
typedef struct Metadata Metadata;

struct AttributeMetadata
{
	char* name;
	variable_type attr_type;
	size_t number_elements;
	bool is_single_value;
}

struct JuleaInfo{
	JSemantics* semantics;
	char* name_space; //unique engine name within m_IO
	//DESIGN: should every io op get a new batch?
	//where should this be decided in adios? probably...

};
typedef struct JuleaInfo JuleaInfo;

int j_adios_init(JuleaInfo* julea_info); //DESIGN: param needed?
int j_adios_finish(void);

/* performs data put AND metadata put*/
int j_adios_put(char* name_space, Metadata* metadata, void* data_pointer, JBatch* batch, gboolean use_batch);

/* get data from object store*/
int j_adios_get_data(char* name_space, char* variable_name, unsigned int length, void* data_pointer, JBatch* batch, gboolean use_batch);

/* get metadata from kv store; hopefully soon from SMD backend*/
int j_adios_get_all_var_names_from_kv(char* name_space, char*** names, unsigned int count_names, JSemantics* semantics);
int j_adios_get_metadata_from_kv(char* name_space, char* var_name, Metadata* metadata, JSemantics* semantics);

/* get metadata from SMD backend*/
int j_adios_get_all_var_names(char* name_space, char*** names, JSemantics* semantics);
int j_adios_get_metadata(char* name_space, Metadata* metadata, JSemantics* semantics);

int j_adios_delete_variable(char* name_space, char* var_name, JBatch* batch);

// void j_adios_get_status(..., JBatch*)
G_END_DECLS

#endif
