#pragma once

namespace uaudio
{
	namespace wave_reader
	{
		class WaveChunkAllocator
		{
		public:
			WaveChunkAllocator() = default;
			WaveChunkAllocator(size_t a_Size);
			WaveChunkAllocator(const WaveChunkAllocator& rhs);

			~WaveChunkAllocator();

			WaveChunkAllocator& operator=(const WaveChunkAllocator& rhs);

			void* Alloc(size_t a_Size);

			void* Start() const
			{
				return m_Start;
			}

			void* End() const
			{
				return m_End;
			}

			size_t Size() const
			{
				return m_Size;
			}

			void* GetBuffer() const
			{
				return m_Buffer;
			}
		private:
			void InitAlloc(size_t a_Size);

			void* m_Start = nullptr;
			void* m_Buffer = nullptr;
			void* m_End;
			size_t m_Size = 0;
		};
	}
}