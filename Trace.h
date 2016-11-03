#pragma once

#include <sstream>
#include <string>

template<const wchar_t * FunctionName>
class Trace
{
protected:
	static int _depth;

	LARGE_INTEGER start_time;
	std::wstring m_message;
	int m_id;

public:
	Trace(std::wstring s);
	Trace(std::wstring s, int id);
	~Trace();

	void WriteMessage(std::wstring &s);
	void Write(std::wstringstream &ss);
};

#define TRACE() \
	Trace trace(__FUNCTIONW__);

#define TRACEWITHMESSAGE(message) \
	Trace trace((message));


#define TRACEWRITE(s) \
	trace.WriteMessage(std::wstring(s) );

int Trace::_depth = 1;

template<const wchar_t * FunctionName>
Trace<FunctionName>::Trace(std::wstring s)
{
	m_id = -1;

	std::wstringstream ss;
	ss.str(L"");

	for(int i = 0; i < _depth; ++i) ss << ".";
	ss << L"Entering: " << m_message;

	Write(ss); 

	_depth++;

	QueryPerformanceCounter(&start_time);
}

template<const wchar_t * FunctionName>
Trace<FunctionName>::Trace(std::wstring s, int id)
{
	std::wstringstream ss;
	ss.str(L"");

	ss << id << L":" << s;

	m_message = std::wstring(ss.str());
	m_id = id;

	ss.str(L"");
	for(int i = 0; i < _depth; ++i) ss << ".";
	ss << L"Entering: " << m_message;

	Write(ss); 

	_depth++;

	QueryPerformanceCounter(&start_time);
}

template<const wchar_t * FunctionName>
Trace<FunctionName>::~Trace(void)
{
	LARGE_INTEGER end_time;
	LARGE_INTEGER freq;

	QueryPerformanceCounter(&end_time);
	double diff = (double)(end_time.QuadPart - start_time.QuadPart);

	QueryPerformanceFrequency(&freq);
	double time = (diff / (double)freq.QuadPart) * 1000.0;

	--_depth;

	std::wstringstream ss;
	ss.str(L"");

	for(int i = 0; i < _depth; ++i) ss << ".";
	ss << L"Exiting: " << m_message << L" (" << time << L" ms)";

	Write(ss);
}

template<const wchar_t * FunctionName>
void Trace<FunctionName>::Write(std::wstringstream &ss)
{
	ss << std::endl;

	std::wstring s = ss.str();

	OutputDebugString(s.c_str()); 
}

template<const wchar_t * FunctionName>
void Trace<FunctionName>::WriteMessage(std::wstring &s)
{
	std::wstringstream ss;
	ss.str(L"");

//	for(int i = 0; i < _depth; ++i) ss << ".";

	ss << FunctionName << L": " << s;

	Write(ss); 
}