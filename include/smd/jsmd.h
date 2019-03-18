/*
 * JULEA - Flexible storage framework
 * Copyright (C) 2018 Michael Straßberger
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

#ifndef JULEA_SMD_SMD_H
#define JULEA_SMD_SMD_H

#if !defined(JULEA_SMD_H) && !defined(JULEA_SMD_COMPILATION)
#error "Only <julea-smd.h> can be included directly."
#endif

#include <glib.h>

#include <julea.h>

G_BEGIN_DECLS

#define JSMD_REGISTER_TYPE(enum,string) enum,
typedef enum JSMD_TYPES{
#include <smd/jsmd-types.h>
} JSMD_TYPES;

#undef JSMD_REGISTER_TYPE

#define JSMD_REGISTER_TYPE(enum,string) string,
const gchar *JSMD_TYPES_STRING[] = {
  #include <smd/jsmd-types.h>
  NULL
};

#undef JSMD_REGISTER_TYPE

struct JSMD;

typedef struct JSMD JSMD;

int64_t j_smd_type_string2int(const gchar*);
gchar*  j_smd_type_int2string(int64_t);

void j_smd_apply_scheme(const gchar*, const bson_t*, JBatch*);
void j_smd_get_scheme(const gchar*, const bson_t*, JBatch*);

void j_smd_insert(const gchar*, const gchar*, const bson_t*, JBatch*);
void j_smd_get(const gchar*, const gchar*, const bson_t*, JBatch*);
void j_smd_update(const gchar*, const gchar*, const bson_t*, JBatch*);
void j_smd_delete(const gchar*, const gchar*, const bson_t*, JBatch*);

G_END_DECLS

#endif