#include "WaveReader.h"

int main()
{
	std::string_view chunk_filters[] =
	{
		"data",
		"fmt "
	};

	const uaudio::wave_reader::Filter filters{ chunk_filters, std::size(chunk_filters) };

	const char* path = "./resources/Atmoscrash.wav";
	size_t size = 0;
	uaudio::wave_reader::WaveReader::FTell(path, size, filters);

	void* allocated_space = malloc(size);
	uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

	// Use the allocated memory and also pass the size.
	uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, filters);
}
