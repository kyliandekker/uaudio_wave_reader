#include "ChunkHeader.h"

namespace uaudio
{
	namespace wave_reader
	{
		ChunkHeader::ChunkHeader(ChunkHeader * a_DataBuffer)
		{
			memcpy(chunk_id, a_DataBuffer->chunk_id, CHUNK_ID_SIZE);
			chunkSize = a_DataBuffer->chunkSize;
		}
	}
}