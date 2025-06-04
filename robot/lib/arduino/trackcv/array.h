template <typename T, int MAX_SIZE> 
class Array {
public:
	Array(): max_size(MAX_SIZE){
		size = 0;
		// max_size = MAX_SIZE;
	}
	size_t size;
	const int max_size;
	T operator[] (int idx) {
		if(idx < size) {
			return data[idx];
		}
		return (T)0;
	}
	T data[MAX_SIZE];
};