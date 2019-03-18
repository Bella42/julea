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

JSMD_REGISTER_TYPE(JSMD_TYPE_INVALID_BSON , "ERROR: type of bson value must be of STRING or INTEGER")
JSMD_REGISTER_TYPE(JSMD_TYPE_UNKNOWN, "ERROR: unknown type in bson value")
JSMD_REGISTER_TYPE(JSMD_TYPE_INTEGER_KEY , "integer_key")
JSMD_REGISTER_TYPE(JSMD_TYPE_TEXT_KEY , "text_key")
JSMD_REGISTER_TYPE(JSMD_TYPE_DATE_KEY , "date_key")
JSMD_REGISTER_TYPE(JSMD_TYPE_INTEGER , "integer")
JSMD_REGISTER_TYPE(JSMD_TYPE_TEXT , "text")
JSMD_REGISTER_TYPE(JSMD_TYPE_DATE , "date")

/* ADIOS Variable Types
 * String specifying types according to members of union value_type for consistency.
 * The unknown type of ADIOS can probably be mapped to the JSMD_TYPE_UNKNOWN.
 * The compound type of ADIOS is not yet realised. So it is not clear what it actually
 * is or will be.  */
// JSMD_REGISTER_TYPE(JSMD_TYPE_COMPOUND, "compound")
// JSMD_REGISTER_TYPE(JSMD_TYPE_UNKNOWN, "")
JSMD_REGISTER_TYPE(JSMD_TYPE_STRING, "string")
JSMD_REGISTER_TYPE(JSMD_TYPE_CHAR, "character")
JSMD_REGISTER_TYPE(JSMD_TYPE_SIGNED_CHAR, "s_character")
JSMD_REGISTER_TYPE(JSMD_TYPE_UNSIGNED_CHAR, "u_character")
JSMD_REGISTER_TYPE(JSMD_TYPE_SHORT, "shorter")
JSMD_REGISTER_TYPE(JSMD_TYPE_UNSIGNED_SHORT, "u_shorter")
JSMD_REGISTER_TYPE(JSMD_TYPE_INT, "integer")
JSMD_REGISTER_TYPE(JSMD_TYPE_UNSIGNED_INT, "u_integer")
JSMD_REGISTER_TYPE(JSMD_TYPE_LONG_INT, "l_integer")
JSMD_REGISTER_TYPE(JSMD_TYPE_UNSIGNED_LONG_INT, "ul_integer")
JSMD_REGISTER_TYPE(JSMD_TYPE_LONG_LONG_INT, "ll_integer")
JSMD_REGISTER_TYPE(JSMD_TYPE_UNSIGNED_LONG_LONG_INT, "ull_integer")
JSMD_REGISTER_TYPE(JSMD_TYPE_FLOAT, "real_float")
JSMD_REGISTER_TYPE(JSMD_TYPE_DOUBLE, "real_double")
JSMD_REGISTER_TYPE(JSMD_TYPE_LONG_DOUBLE, "lreal_double")
JSMD_REGISTER_TYPE(JSMD_TYPE_FLOAT_COMPLEX, "float_complex")
JSMD_REGISTER_TYPE(JSMD_TYPE_DOUBLE_COMPLEX, "double_complex")

//probably useful
JSMD_REGISTER_TYPE(JSMD_TYPE_BOOLEAN, "boolean")
// DESIGN: needed?
//see enum variable_type in jadios.h
JSMD_REGISTER_TYPE(JSMD_TYPE_VARIABLE_TYPE, "variable_type")
JSMD_REGISTER_TYPE(JSMD_TYPE_VALUE_TYPE, "value_type")