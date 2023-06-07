#ifndef MedianFilter_h
#define MedianFilter_h
#include "Arduino.h" 
class MedianFilter {
public:
	MedianFilter(int buffer_size);
	void addValue(int value);
    int getMedian();
    int getLatestValue();
    int* getBuffer();
    int getBufferSize();
    void dumpBuffer();

private:
	int _buffer_size;
    int* _buffer;
};
#endif