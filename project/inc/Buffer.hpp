#pragma once

#include <cstdlib>

namespace ft
{
	class Buffer
	{
		private:
			static const size_t DEFAULT_CAPACITY = 2048;

			char	*_buf;
			size_t	_size;
			size_t	_capacity;
		public:
			Buffer();
			Buffer(char *buf, size_t size);
			Buffer &operator=(const Buffer & rhs);
			~Buffer();

			void append(const char *buf, size_t size);
			void append(const Buffer & rhs);

			const char	*getBuffer() const;
			size_t	size() const;
	};
}
