#include "../../include/WaveReader.h"
#include "../../include/WaveFile.h"

int main()
{
	uaudio::wave_reader::WaveFile waveFile;
	uaudio::wave_reader::WaveReader::LoadSound("C:/test/ttest.wav", waveFile);

	while (true)
	{
		
	}
}