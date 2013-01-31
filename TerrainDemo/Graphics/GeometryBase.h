/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef GEOMETRYBASE_H_
#define GEOMETRYBASE_H_

template <class G>
class TGeometry
{
public:
	void prepare(void) 
	{
		static_cast<G*>(this)->prepare();
	}
	void* getInfo(void)
	{
		return static_cast<void*>(static_cast<G*>(this)->getInfo());
	}
	void checkUV(bool n)
	{
		static_cast<G*>(this)->checkUV(n);
	}
protected:
private:
};

#endif