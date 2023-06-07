#include "Arduino.h”
#include "MedianFilter.h"
MedianFilter::MedianFilter(int buffer_size) {
	_buffer_size = buffer_size;
    _buffer = (int*) malloc(sizeof(int) * _buffer_size);
    for (int i = 0; i < _buffer_size; i++) {
    	_buffer[i] = 0;
    }
}
void MedianFilter::addValue(int value) {
    for (int i = 0; i < _buffer_size - 1; i++) {
    	_buffer[i] = _buffer[i + 1];
    }
    _buffer[_buffer_size - 1] = value;
}
int getMedian() {
    //creates a copy of the buffer
    int* sorted_buffer = (int*) malloc(sizeof(int) * _buffer_size);
    for (int i = 0; i < _buffer_size; i++) {
    	sorted_buffer[i] = _buffer[i];
    }
    //sorts buffer
    for (int i = 0; i < _buffer_size; i++) {
    	for (int j = i + 1; j < _buffer_size; j++) {
        	if (sorted_buffer[j] < sorted_buffer[i]) {
            	int tmp = sorted_buffer[i];
                sorted_buffer[i] = sorted_buffer[j];
                sorted_buffer[j] = tmp;
            }
        }
    }
    //gets median
    int median = sorted_buffer[_buffer_size / 2];
    //frees memory
    free(sorted_buffer);
    return median;
}
int* getBuffer() {
    return _buffer;
}
int getBufferSize() {
    return _buffer_size;
}
void dumpBuffer() {
    for (int i = 0; i < _buffer_size; i++) {
    	_buffer[i] = 0;
    }
}
int getLatestValue() {
    return _buffer[_buffer_size - 1];
}