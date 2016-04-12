/*
 * Touhou Vorbis Compressor
 *
 * ----
 *
 * Shared file functions.
 */

#include <string.h>

char* MatchFileSub(const char* str, const char* sub)
{
	// Remove the directory part from the filename to avoid false positives
	// if someone happens to store the game in a directory that matches the substring..
	int FNPos;
	// Get last backslash
	for(FNPos = strlen(str); FNPos > 0; FNPos--)	if(str[FNPos] == '\\')	break;

	return strstr(&str[FNPos], sub);
}
