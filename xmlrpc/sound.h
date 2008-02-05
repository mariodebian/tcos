/*
* sound.h part of tcosxmlrpc
*   => common headers of sound.c
* Copyright (C) 2006,2007,2008  mariodebian at gmail
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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
