/*
 * JULEA - Flexible storage framework
 * Copyright (C) 2019-2020 Michael Kuhn
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

#include <julea.h>
#include <julea-object.h>
#include <julea-kv.h>
#include <julea-db.h>

#include <locale.h>
#include <stdio.h>


//TODO: this example currently does not check whether this object already exists
int object_store_write()
{
	//g_autoptr declares a pointer variable with automatic cleanup
	g_autoptr(JBatch) batch = NULL; 		// a batch in which the operations are executed
	g_autoptr(JObject) object = NULL;		// an object to be stored

	gboolean is_executed = false;			// batch status
	guint64 bytes_written = 0;				// number of bytes actually written
	gchar const* data = "Hello World!"; 	// our example object data is this string
	gchar const* name_space = "object-namespace";
	gchar const* name = "object-1";
	
	// in C a string needs to be terminated by a null character '\0' which needs space.
	// therefore the data_size is the length of the string (strlen) plus 1.
	guint64 data_size = strlen(data) + 1;	// size of the object

	// Explicitly enable UTF-8 since functions such as g_format_size might return UTF-8 characters.
	setlocale(LC_ALL, "C.UTF-8");

	// creates a new batch with the default semantics
	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_DEFAULT);

	// returns a new object handler
	object = j_object_new(name_space, name);
	
	//creates the actual object
	j_object_create(object, batch);

	// Writes data for object. Afterwards bytes_written contains the 
	// number of bytes that were actually written
	j_object_write(object, data, data_size, 0, &bytes_written, batch);
	
	// Executes the object creation and the object write in one batch.
	// returns false if an error occurred 
	is_executed = j_batch_execute(batch);

	if (!is_executed)
	{
		printf("An error occurred when writing the object! \n");
	}
	else
	{
		// check whether everything was written as this might not be the case
		if (bytes_written == data_size)
		{
			printf("Writing was successful :-) \n");
		}
		else
		{
			printf("Only %ld bytes written instead of %ld\n", bytes_written, data_size);
		}
	}

	return 0;
}

int object_store_read()
{
	return 0;
}

int key_value_store_write()
{
	return 0;
}

int key_value_store_read()
{
	return 0;
}

int database_write()
{
	return 0;
}

int database_read()
{
	return 0;
}

int
main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	// Explicitly enable UTF-8 since functions such as g_format_size might return UTF-8 characters.
	setlocale(LC_ALL, "C.UTF-8");

	object_store_write();
	object_store_read();

	key_value_store_write();
	key_value_store_read();

	database_write();
	database_read();

	return 0;
}
