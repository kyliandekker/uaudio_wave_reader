#include "../../include/WaveReader.h"
#include "../../include/ChunkCollection.h"
#include "../../include/WaveChunks.h"

int main()
{
	uaudio::wave_reader::ChunkCollection chunkCollection;

	const char* path = "C:/test/ttest.wav";
	const char* save_path = "C:/test/ttest_new.wav";

	size_t size = 0;
	uaudio::wave_reader::WaveReader::GetSoundSize(path, size);
	uaudio::wave_reader::WaveInfo waveInfo{malloc(size), size};
	printf("\n");
	uaudio::wave_reader::WaveReader::LoadSound(path, chunkCollection, waveInfo);

	uaudio::wave_reader::FMT_Chunk fmt_chunk;
	chunkCollection.GetChunkFromData(fmt_chunk, uaudio::wave_reader::FMT_CHUNK_ID);

	printf("\n");
	uaudio::wave_reader::WaveReader::SaveSound(save_path, chunkCollection);

	free(chunkCollection.GetData());

	while (true)
	{
		
	}
}