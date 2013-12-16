/*-------------------------------------------------------------------------\
| File: UNCOPYABLE.H														|
| Desc: A base class with private copy constructor and copy assignment		|
|		operator - use as a base to prevent a class from being copied.		|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef UNCOPYABLE_H
#define UNCOPYABLE_H

class Uncopyable
{
public:
	Uncopyable() {};
	~Uncopyable() {};
private:
	Uncopyable(const Uncopyable&) {};
	Uncopyable& operator=(const Uncopyable&) {};
};

#endif // UNCOPYABLE_H