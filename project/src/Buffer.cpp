#include "../inc/Buffer.hpp"

ft::Buffer::Buffer(): _buf(NULL), _size(0) , _capacity(DEFAULT_CAPACITY) {}

ft::Buffer::Buffer(char *buf, size_t size):_size(size), _capacity(DEFAULT_CAPACITY)
{
	if (size > DEFAULT_CAPACITY)
		_capacity = size;
	_buf = new char[_capacity];
	for (size_t i = 0; i < _size; i++)
		_buf[i] = buf[i];
}

ft::Buffer & ft::Buffer::operator=(const Buffer & rhs)
{
	if (this->_buf != NULL)
		delete [] this->_buf;
	this->_buf = new char[rhs._capacity];
	for (size_t i = 0; i < rhs._size; i++)
	{
		this->_buf[i] = rhs._buf[i];
	}
	this->_size = rhs._size;
	this->_capacity = rhs._capacity;
	return *this;
}

ft::Buffer::~Buffer()
{
	delete [] _buf;
}

void ft::Buffer::append(const Buffer & rhs)
{
	this->append(rhs.getBuffer(), rhs.size());
}

void ft::Buffer::append(const char *buf, size_t size)
{
	if (size + _size > _capacity)
	{
		char *tmp = _buf;
		_buf = new char[size + _size];
		for (size_t i = 0; i < _size; i++)
			_buf[i] = tmp[i];
		_capacity = size + _size;
		delete [] tmp;
	}
	for (size_t i = 0; i < size; i++)
		_buf[_size + i] = buf[i];
	_size += size;
}

const char *ft::Buffer::getBuffer() const
{
	return _buf;
}

size_t ft::Buffer::size() const
{
	return _size;
}
