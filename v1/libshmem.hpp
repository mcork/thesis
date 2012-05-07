/*
 * libshmem.hpp (version 3.0)
 *
 *  This is a shared memory designed for use in the MAVStar '11 project at University of New South Wales. It uses a circular buffer to emulate
 *  synchornisation. The buffer size is recommended to be greater than or equal to 10
 *  This is the intellectual property of Matthew Cork and can only be use by authourised people or future MAVStar teams at UNSW.
 * 
 *
 *  Author: Matt Cork
 *  Created on: Sep 7, 2010  
 *  Last Modified By: Matt Cork March 10,2011
 * 
 */


/*
 * 
 * Error Message: If read is called without write having being called returns -1
 * 
 * NOTE: Doesn't work for large shared memory
 * 
 */

#ifndef LIBSHMEM_HPP_
#define LIBSHMEM_HPP_

#define BUFFER_SIZE 10

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>

template <typename T>
struct shmem_item
{
	T item;
};

template<typename T>
struct shmem_region
{
	shmem_region(std::string filename)
		: buffer_size_(sizeof(shmem_item<T>)*BUFFER_SIZE+sizeof(int)*2)
	{
		path_ = "/dev/shm/" + filename;
		fd_  = open(path_.c_str(),O_CREAT |O_RDWR , 0666);
		ftruncate(fd_,buffer_size_);
		const void* m = mmap(0, buffer_size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);

		read_entry_ = (int*)m;
		write_entry_ = (int*)m + sizeof(int);
		buffer_= (shmem_item<T>*)((int*)m+sizeof(int)*2);

		// check to see if read/write are initialize
		if (*read_entry_ == 0) {
			*write_entry_ = 1;
		}
 
	}

	T* read()
	{
		int error=-1;
		if (*read_entry_ == 0){
			return (T*)error;
		}
		return &buffer_[*read_entry_-1].item;			// returns the item in the buffer
	}

	void write(T item)
	{
		shmem_item<T> myItem;
		myItem.item = item;
		buffer_[*write_entry_-1] = myItem;			// Places into the buffer at point next able to be written
		*read_entry_ = *write_entry_;				// assign read_entry to the new value just written
		*write_entry_ ++;					// increment write entry for next write
		*write_entry_ = (*write_entry_ % BUFFER_SIZE)+1;	// stops from overflowing buffer bounds
	}

	void shutdown()
	{
		//Doesn't do anything
	}

private:
	std::string path_;
	int fd_;
	const size_t buffer_size_;
	shmem_item<T>* buffer_;
	int* read_entry_;
	int* write_entry_;
};


#endif /* LIBSHMEM_HPP_ */
