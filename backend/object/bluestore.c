/*
 * JULEA - Flexible storage framework
 * Copyright (C) 2010-2020 Michael Kuhn
 * Copyright (C) 2020 Johannes Coym
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

#include <julea-config.h>

#include <glib.h>
#include <gmodule.h>

#include <julea.h>

#include <julea_bluestore.h>

struct JBackendData
{
	gchar* path;
	void* store;
	void* coll;
};

typedef struct JBackendData JBackendData;

struct JBackendObject
{
	gchar* path;
	void* obj;
};

typedef struct JBackendObject JBackendObject;

static gboolean
backend_create(gpointer backend_data, gchar const* namespace, gchar const* path, gpointer* backend_object)
{
	gchar* full_path;
	JBackendData* bd;
	JBackendObject* bo;

	bd = backend_data;
	bo = g_slice_new(JBackendObject);

	full_path = g_build_filename(namespace, path, NULL);

	j_trace_file_begin(full_path, J_TRACE_FILE_CREATE);
	bo->obj = julea_bluestore_create(bd->store, bd->coll, full_path);
	j_trace_file_end(full_path, J_TRACE_FILE_CREATE, 0, 0);

	bo->path = full_path;

	*backend_object = bo;

	return TRUE;
}

static gboolean
backend_open(gpointer backend_data, gchar const* namespace, gchar const* path, gpointer* backend_object)
{
	gchar* full_path;
	JBackendObject* bo;
	(void)backend_data;

	bo = g_slice_new(JBackendObject);

	full_path = g_build_filename(namespace, path, NULL);

	j_trace_file_begin(full_path, J_TRACE_FILE_OPEN);
	bo->obj = julea_bluestore_open(full_path);
	j_trace_file_end(full_path, J_TRACE_FILE_CREATE, 0, 0);

	bo->path = full_path;

	*backend_object = bo;

	return TRUE;
}

static gboolean
backend_delete(gpointer backend_data, gpointer backend_object)
{
	JBackendData* bd;
	JBackendObject* bo;

	bd = backend_data;
	bo = backend_object;

	j_trace_file_begin(bo->path, J_TRACE_FILE_DELETE);
	julea_bluestore_delete(bd->store, bd->coll, bo->obj);
	j_trace_file_end(bo->path, J_TRACE_FILE_DELETE, 0, 0);

	g_slice_free(JBackendObject, bo);

	return TRUE;
}

static gboolean
backend_close(gpointer backend_data, gpointer backend_object)
{
	JBackendObject* bo;

	bo = backend_object;
	(void)backend_data;

	j_trace_file_begin(bo->path, J_TRACE_FILE_CLOSE);
	j_trace_file_end(bo->path, J_TRACE_FILE_CLOSE, 0, 0);

	g_slice_free(JBackendObject, bo);

	return TRUE;
}

static gboolean
backend_status(gpointer backend_data, gpointer backend_object, gint64* modification_time, guint64* size)
{
	struct stat buf;
	JBackendData* bd;
	JBackendObject* bo;
	gboolean ret = TRUE;

	bd = backend_data;
	bo = backend_object;

	if (modification_time != NULL || size != NULL)
	{
		j_trace_file_begin(bo->path, J_TRACE_FILE_STATUS);
		ret = julea_bluestore_status(bd->store, bd->coll, bo->obj, &buf);
		j_trace_file_end(bo->path, J_TRACE_FILE_STATUS, 0, 0);

		if (ret && modification_time != NULL)
		{
			*modification_time = buf.st_mtime * G_USEC_PER_SEC;

#ifdef HAVE_STMTIM_TVNSEC
			*modification_time += buf.st_mtim.tv_nsec / 1000;
#endif
		}

		if (ret && size != NULL)
		{
			*size = buf.st_size;
		}
	}

	return ret;
}

static gboolean
backend_sync(gpointer backend_data, gpointer backend_object)
{
	JBackendData* bd;
	JBackendObject* bo;

	bd = backend_data;
	bo = backend_object;

	j_trace_file_begin(bo->path, J_TRACE_FILE_SYNC);
	julea_bluestore_fsync(bd->coll);
	j_trace_file_end(bo->path, J_TRACE_FILE_SYNC, 0, 0);

	return TRUE;
}

static gboolean
backend_read(gpointer backend_data, gpointer backend_object, gpointer buffer, guint64 length, guint64 offset, guint64* bytes_read)
{
	gsize br = 0;
	JBackendData* bd;
	JBackendObject* bo;

	bd = backend_data;
	bo = backend_object;

	j_trace_file_begin(bo->path, J_TRACE_FILE_READ);
	br = julea_bluestore_read(bd->store, bd->coll, bo->obj, offset, (char**)&buffer, length);
	j_trace_file_end(bo->path, J_TRACE_FILE_READ, length, offset);

	if (bytes_read != NULL)
	{
		*bytes_read = br;
	}

	return (br == length);
}

static gboolean
backend_write(gpointer backend_data, gpointer backend_object, gconstpointer buffer, guint64 length, guint64 offset, guint64* bytes_written)
{
	gsize bw = 0;
	JBackendData* bd;
	JBackendObject* bo;

	bd = backend_data;
	bo = backend_object;

	j_trace_file_begin(bo->path, J_TRACE_FILE_WRITE);
	bw = julea_bluestore_write(bd->store, bd->coll, bo->obj, offset, (const char*)buffer, length);
	j_trace_file_end(bo->path, J_TRACE_FILE_WRITE, length, offset);

	if (bytes_written != NULL)
	{
		*bytes_written = bw;
	}

	return (bw == length);
}

static gboolean
backend_init(gchar const* path, gpointer* backend_data)
{
	JBackendData* bd;
	gchar* mkfs_path;

	bd = g_slice_new(JBackendData);
	bd->path = g_strdup(path);
	mkfs_path = g_build_filename(path, "/mkfs_done", NULL);

	bd->store = julea_bluestore_init(path);

	if (access(mkfs_path, F_OK) == 0)
	{
		julea_bluestore_mount(bd->store);
		bd->coll = julea_bluestore_open_collection(bd->store);
	}
	else
	{
		julea_bluestore_mkfs(bd->store);
		julea_bluestore_mount(bd->store);
		bd->coll = julea_bluestore_create_collection(bd->store);
	}

	*backend_data = bd;

	return TRUE;
}

static void
backend_fini(gpointer backend_data)
{
	JBackendData* bd = backend_data;

	julea_bluestore_umount(bd->store, bd->coll);

	g_free(bd->path);
	g_slice_free(JBackendData, bd);
}

static JBackend bluestore_backend = {
	.type = J_BACKEND_TYPE_OBJECT,
	.component = J_BACKEND_COMPONENT_CLIENT | J_BACKEND_COMPONENT_SERVER,
	.object = {
		.backend_init = backend_init,
		.backend_fini = backend_fini,
		.backend_create = backend_create,
		.backend_delete = backend_delete,
		.backend_open = backend_open,
		.backend_close = backend_close,
		.backend_status = backend_status,
		.backend_sync = backend_sync,
		.backend_read = backend_read,
		.backend_write = backend_write }
};

G_MODULE_EXPORT
JBackend*
backend_info(void)
{
	return &bluestore_backend;
}