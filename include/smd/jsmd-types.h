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