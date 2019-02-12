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

#include <julea.h>

G_BEGIN_DECLS

struct Metadata{
	char* name;
	const unsigned long* shape;
	const unsigned long*  start;
	const unsigned long*  count;

	size_t steps_start;
	size_t steps_count;
	bool constant_shape;
	bool is_value;
	const bool debug_mode;

	const void *data_pointer;
	guint data_size;
	// char *min_value;
	// float min_value_real;
	// float min_value_complex;
	// float max_value_real;
	// float max_value_complex;
	// float curr_value_real;
	// float curr_value_complex;
	//variable type //DESIGN: needed to know what type max, min and current are
	//max value
	//min value
	//current value
	//is value
	//operations -> how are these used? necessary for structured backend?
	//std::map<size_t, std::vector<helper::SubStreamBoxInfo>>
    //        StepBlockSubStreamsInfo;
};
typedef struct Metadata Metadata;

struct JuleaInfo{
	JSemantics* semantics;
	//FIXME: should every io op get a new batch?
	//where should this be decided in adios? probably...

};
typedef struct JuleaInfo JuleaInfo;

int j_adios_init(JuleaInfo* julea_info); //FIXME: param needed?
int j_adios_finish(void);

//FIXME use_batch -> aggregate data or not
int j_adios_put(Metadata* metadata, void* datapointer, JBatch* batch, JSemantics* semantics, gboolean use_batch);
int j_adios_get(Metadata* metadata, void* datapointer, JBatch* batch, JSemantics* semantics, gboolean use_batch);
int j_adios_delete(Metadata* metadata, JBatch* batch);

// void j_adios_get_status(..., JBatch*)


G_END_DECLS

#endif
