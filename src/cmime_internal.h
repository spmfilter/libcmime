/* libcmime - A C mime library
 * Copyright (C) 2011 Axel Steiner <ast@treibsand.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CMIME_INTERNAL_H
#define _CMIME_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define HEADER_CONTENT_TYPE_PATTERN "Content-Type: %s%s"
#define HEADER_CONTENT_DISPOSITION_PATTERN "Content-Disposition: %s%s"
#define HEADER_CONTENT_TRANSFER_ENCODING_PATTERN "Content-Transfer-Encoding: %s%s"

#define MIMETYPE_DEFAULT "application/octet-stream"
#define MIMETYPE_TEXT_PLAIN "text/plain"

#define LINE_LENGTH 72

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_INTERNAL_H */
