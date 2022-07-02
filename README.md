# UAudio Wave Reader

UAudio Wave Reader is a small library that allows the user to easily read and save wave files. The library returns a collection of wavechunks that can then be used and manipulated by the user.

## Usage

### OPTIONAL: Wave file size

The user is responsible for the allocation of memory. The user can choose to allocate more memory than necessary or exactly the size of the wave file. The user can find out the exact wave file size by doing something like this:
```cpp
	size_t size = 0;
	uaudio::wave_reader::WaveReader::GetSoundSize("test.wav", size);
```
Result:
```
<WaveReader> Reading wave file: ("test.wav").
<WaveReader> Found "fmt " chunk with size "16".
<WaveReader> Found "data" chunk with size "43966508".
<WaveReader> Opened file successfully: ("test.wav").
<WaveReader> Wave file ("test.wav") has a size of 43966540.
```

### Loading a wave file

To load a wave file, the user can do something like this:
```cpp
	uaudio::wave_reader::ChunkCollection chunkCollection;

	// Use the allocated memory and also pass the size.
	uaudio::wave_reader::WaveInfo waveInfo{malloc(size), size};
	uaudio::wave_reader::WaveReader::LoadSound("test.wav", chunkCollection, waveInfo);
```
Result:
```
<WaveReader> Reading wave file: ("test.wav").
<WaveReader> Found "RIFF" chunk.
<WaveReader> Found "fmt " chunk with size "16".
<WaveReader> Found "data" chunk with size "43966508".
<WaveReader> Opened file successfully: ("test.wav").
```

### Saving a wave file
To save a wave file, the user can do something like this:
```cpp
	// Chunk collection here is an earlier loaded collection of chunks.
	uaudio::wave_reader::WaveReader::SaveSound("new-test.wav", chunkCollection);
```
Result:
```
<WaveReader> Opened file successfully: ("new-test.wav").
```

## Future features
* Chunk filtering (providing an array or vector of chunk ids. Only those will be loaded and other chunks will be ignored)
* Adding new or custom chunks to an existing chunk collection
* Conversion settings (8-bit, 16-bit, 24-bit, 32-bit, mono, stereo, etc)