// Touhou Vorbis Compressor
// ------------------------
// fake.h - Shared data faking functions
// ------------------------
// "©" Nmlgc, 2011

#ifndef THVORBIS_FAKE_H
#define THVORBIS_FAKE_H

int Fake_WAVEFORMATEX(char* Buf, DWORD Read, OggVorbis_File* vf);
#ifdef _INC_MMSYSTEM
int Fake_Descend(__inout LPMMCKINFO pmmcki, int iChunkSeq, OggVorbis_File* vf);
#endif /* _INC_MMSYSTEM */

#endif /* THVORBIS_FAKE_H */
