 #ifndef foosimplehfoo
 #define foosimplehfoo

 /***
   This file is part of PulseAudio.

   Copyright 2004-2006 Lennart Poettering
   Copyright 2006 Pierre Ossman <ossman@cendio.se> for Cendio AB

   PulseAudio is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation; either version 2.1 of the License,
   or (at your option) any later version.

   PulseAudio is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with PulseAudio; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA.
 ***/

 #include <sys/types.h>

 #include <pulse/sample.h>
 #include <pulse/channelmap.h>
 #include <pulse/def.h>
 #include <pulse/cdecl.h>
 #include <pulse/version.h>

 PA_C_DECL_BEGIN

 typedef struct pa_simple pa_simple;

 pa_simple* pa_simple_new(
     const char *server,
     const char *name,
     pa_stream_direction_t dir,          
     const char *dev,
     const char *stream_name,
     const pa_sample_spec *ss,
     const pa_channel_map *map,
     const pa_buffer_attr *attr,
     int *error
     );

 void pa_simple_free(pa_simple *s);

 int pa_simple_write(pa_simple *s, const void*data, size_t bytes, int *error);

 int pa_simple_drain(pa_simple *s, int *error);

 int pa_simple_read(pa_simple *s, void*data, size_t bytes, int *error);

 pa_usec_t pa_simple_get_latency(pa_simple *s, int *error);

 int pa_simple_flush(pa_simple *s, int *error);

 PA_C_DECL_END

 #endif
