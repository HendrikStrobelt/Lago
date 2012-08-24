#ifndef DUMPING_H
#define DUMPING_H

#include <fstream>
#include <vector>

using namespace std;

/*
 * inline header because
 * without the "export" keyword the template
 * functions can't be defined in a cpp file
 *
 */

namespace dump {

	//WRITE

	inline void w(ofstream* dataStream, int data) {
		dataStream->write((const char*) &data, sizeof(int));
	}

	inline void w(ofstream* dataStream, float data) {
		dataStream->write((const char*) &data, sizeof(float));
	}

	inline void w(ofstream* dataStream, bool data) {
		dataStream->write((const char*) &data, sizeof(bool));
	}

	inline void w(ofstream* dataStream, char* data, int size) {
		dataStream->write(data, size);
	}

	template <class T>
	inline void w(ofstream* dataStream, vector<T>* data) {
		int size = data->size();
		w(dataStream, size);
		dataStream->write((const char*) &data->at(0), sizeof(T) * size);
	}

	template <class T>
	inline void w(ofstream* dataStream, vector<vector<T> *>* data) {
		int size = data->size();
		w(dataStream, size);
		for (int i = 0; i < size; i++) {
			w(dataStream, data->at(i));
		}
	}

	template <class T>
	inline void w(ofstream* dataStream, int size, T* data) {
		w(dataStream, size);
		dataStream->write((const char*) data, sizeof(T) * size);
	}


	//READ

	inline void r(ifstream* dataStream, int* data) {
		dataStream->read((char*) data, sizeof(int));
	}

	inline void r(ifstream* dataStream, float* data) {
		dataStream->read((char*) data, sizeof(float));
	}

	inline void r(ifstream* dataStream, bool* data) {
		dataStream->read((char*) data, sizeof(bool));
	}

	inline void r(ifstream* dataStream, char* data, int size) {
		dataStream->read(data, size);
	}

	template <class T>
	inline void r(ifstream* dataStream, vector<T>* data) {
		data->clear();
		int size;
		r(dataStream, &size);
		data->resize(size);
		dataStream->read((char*) &data->at(0), sizeof(T) * size);
	}

	template <class T>
	inline void r(ifstream* dataStream, vector<vector<T> *>* data) {
		data->clear();		
		int size;
		r(dataStream, &size);
		for (int i = 0; i < size; i++) {
			vector<T>* v = new vector<T>();
			r(dataStream, v);
			data->push_back(v);
		}
	}

	template <class T>
	inline void r(ifstream* dataStream, int* size, T** data) {
		r(dataStream, size);
		*data = new T[*size];
		dataStream->read((char*) *data, sizeof(T) * (*size));
	}

}

#endif