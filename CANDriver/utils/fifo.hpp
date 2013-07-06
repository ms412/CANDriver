/*
 * fifo.hpp
 *
 * Created: 06.07.2013 10:31:07
 *  Author: Administrator
 */ 


#ifndef FIFO_H_
#define FIFO_H_

template <class FifoType, int BufferSize=32>

class Fifo
{
	public:
	//Constructor
	Fifo(): m_put(0), m_get(0) {}

	bool isEmpty() {return (m_put == m_get);}
	bool isFull(){
		int temp = m_put;
		Inc(temp);
		return temp == m_get;
	}

	bool Get(FifoType &Slot){
		if (isEmpty())
		return false;
		Slot = m_Slots[m_get];
		Inc(m_get);
		return true;
	}

	bool Put(const FifoType &Slot){
		if (isFull())
		return false;
		m_Slots[m_put] = Slot;
		Inc(m_put);
		return true;
	}
	
	uint8_t Usage(void){
		return m_put;
	}

	private:
	FifoType m_Slots[BufferSize+1];
	int m_put, m_get;
	void Inc (int & Pos) {Pos = (Pos+1)%(BufferSize+1);}

};





#endif /* FIFO_H_ */