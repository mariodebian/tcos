/*sound.h common headers  2006-09-09 14:22:40 mariodebian $
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



#define SOUND_WRAPPER TCOS_PATH"/soundctl.sh "

#define SOUND_GET_CONTROLS    SOUND_WRAPPER " --showcontrols"
#define SOUND_GET_LEVEL       SOUND_WRAPPER " --getlevel "
#define SOUND_SET_LEVEL       SOUND_WRAPPER " --setlevel "

#define SOUND_GET_MUTE        SOUND_WRAPPER " --getmute "
#define SOUND_SET_MUTE        SOUND_WRAPPER " --setmute "
#define SOUND_SET_UNMUTE      SOUND_WRAPPER " --setunmute "


#define SOUND_OK    "ok: sound action done."
#define SOUND_ERROR "error: sound action error."

#define SOUND_READING_ERROR "error: reading sound settings."
#define SOUND_UNKNOW_ERROR "error: unknow option passed."
