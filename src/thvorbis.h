/*
 * Touhou Vorbis Compressor
 *
 * ----
 *
 * Shared declarations.
 * (DLL hooking adopted from jisakujien's "noflashmyon" source.)
 */

#ifndef THVORBIS_THVORBIS_H
#define THVORBIS_THVORBIS_H

#include <vorbis/vorbisfile.h>

#define MIN(a, b)		        ((a) < (b) ? (a) : (b))				// Minimum
#define MAX(a, b)               ((a) > (b) ? (a) : (b))				// Maximum
#define BETWEEN(a, b, c)    	((a) > (b) && (a) < (c) ? 1 : 0)	// Checks if a is between b and c
#define BETWEEN_EQUAL(a, b, c)	((a) >= (b) && (a) <= (c) ? 1 : 0)	// Checks if a is between b and c or is equals b/c 

// Globals
// -------
extern OggVorbis_File gl_VF;
// -------

size_t w32_read(void* dst, size_t elmsize, size_t count, HANDLE file);
int w32_seek(HANDLE file, ogg_int64_t bytes, int org);
int w32_close(HANDLE file);
long w32_tell(HANDLE file);

static ov_callbacks OV_CALLBACKS_WIN32;

#endif /* THVORBIS_THVORBIS_H */
