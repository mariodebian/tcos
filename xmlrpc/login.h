/*login.h common headers  2006-09-09 14:22:40 mariodebian $
#
# This file is part of tcosxmlrpc.
#
# tcosxmlrpc is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# tcosxmlrpc is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with tcosxmlrpc; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.
*/

#define NO_ALLOWED "error: access denied"

#ifndef BSIZE
#define BSIZE 500
#endif

#define LOGIN_OK_MSG      "ok"
#define LOGIN_NOUSER_MSG  "error: User not allowed"
#define LOGIN_NOPASS_MSG  "error: Password not valid"
#define LOGIN_NOPERMS_MSG "error: cant read shadow"
#define LOGIN_EXPIRED_MSG "error: user expired"
#define LOGIN_ERROR       "error: unknow error"


