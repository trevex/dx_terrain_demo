/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef MATERIALMANAGER_H_
#define MATERIALMANAGER_H_

class CMaterialManager
{
public:
	CMaterialManager(void);
	~CMaterialManager(void);

	static CMaterialManager* getInstance(void);

	int get(void);
	void set(int mat);
protected:
private:
	int m_iCounter;
	int m_iMat;

	static CMaterialManager* m_Instance;
};

#endif