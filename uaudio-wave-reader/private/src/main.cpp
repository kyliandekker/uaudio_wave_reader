#include <string>

#include "../../include/WaveReader.h"
#include "../../include/ChunkCollection.h"
#include "../../include/WaveChunks.h"

int main()
{
	const char* path = "C:/test/32.wav";
	const char* save_path = "C:/test/32_new_new.wav";

	std::string_view strings[] =
	{
		"data",
		"fmt ",
	};
	const uaudio::wave_reader::Filter filters{ strings, std::size(strings) };

	size_t size = 0;
	uaudio::wave_reader::WaveReader::FTell(path, size, filters);

	void* allocated_space = malloc(size);
	uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

	uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, filters);

	uaudio::wave_reader::FMT_Chunk fmt_chunk;
	chunkCollection.GetChunkFromData(fmt_chunk, uaudio::wave_reader::FMT_CHUNK_ID);

	uaudio::wave_reader::WaveReader::SaveWave(save_path, chunkCollection);

	free(allocated_space);

	while (true)
	{
		
	}
}