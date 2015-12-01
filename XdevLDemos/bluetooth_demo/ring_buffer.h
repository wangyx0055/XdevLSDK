/**
	@class ring_buffer
	@brief Simulates a ringbuffer. 
	@author Cengiz Terzibas
	
*/
template<typename T>
class ring_buffer{
public:
	/// The value pair for the buffer.
	struct value_pair{
    value_pair() : time(0.0) {}
		value_pair(double t, T o) : time(t), obj(o) {}
		double 	time;
		T				obj;
	};
	/// Constructor.
	ring_buffer(xdl::xdl_int size) :m_size(size), m_pointer(0),m_buffer_filled(false) {
	}
	void reset(){
		std::fill(m_buffer.begin(), m_buffer.end(), value_pair(0.0, T::zero()));
	}
	/// Increase the internal buffer pointer.
	void increase(){
		if(m_pointer == m_buffer.size() - 1)
			m_pointer = 0;
		else
			m_pointer++;
	}
	/// Decrease the internal buffer pointer.
	void decrease(){
		if(m_pointer == 0)
			m_pointer = m_buffer.size() - 1;
		else
			m_pointer--;
	}
	/// Decrease the a specified buffer pointer.
	unsigned int decrease(unsigned int& pointer){
		if(pointer == 0)
			pointer = m_buffer.size() - 1;
		else
			pointer--;
			
		return pointer;
	}
	/// Put one item into the buffer.
	void put(double time, T obj, int number){
			static int counter = 0;
			static T tmp;
			if(m_dirty_flag){
				
			}
			
			if(number%m_size == 0)
				// Check the time difference between the current put and last put.
				m_buffer[m_pointer] = value_pair(time , obj);
			else if(number > m_size){
				memcpy(tmp,obj,m_size);
				m_buffer[m_pointer] = value_pair(time , tmp);
				m_dirty_flag = true;
				m_last_size = number - m_size;
				return;
			}
			
			// Calculate the first time the max possible delay time with the specified buffer size.
			if(m_buffer_filled == false){

				if(m_pointer == m_buffer.size()-1){
					m_buffer_filled = true;
					std::cout << time << std::endl;
					
				}
			}

	}

	
	/// Return one delayed item from the buffer.
	int get(xdl::xdl_double time, value_pair* val){
		// Only start returning values if the buffer is full.
    if(false == m_buffer_filled)
      return 0;		
		
		unsigned int pointer = m_pointer;
		while(true){
			pointer = decrease(pointer);
			if(pointer == m_pointer)
				return 0;

		}

//		for(int a = 0; a < time_quantum; a++)
//			decrease(pointer);
		*val = m_buffer[pointer];
    return 1;
	}
	
protected:
	xdl::xdl_int m_size;
	xdl::xdl_int m_last_size;
	/// Holds the current pointer to the buffer.
	size_t m_pointer;
	/// Is the buffer filled or not.
  xdl::xdl_bool m_buffer_filled;
	xdl::xdl_bool m_dirty_flag;
	/// Buffer for the data.
	std::vector<value_pair> m_buffer;
	value_pair m_last_value;
};