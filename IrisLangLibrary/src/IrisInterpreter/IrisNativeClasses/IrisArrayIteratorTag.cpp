#include "IrisInterpreter/IrisNativeClasses/IrisArrayIteratorTag.h"



void IrisArrayIteratorTag::Initialize(vector<IrisValue>* pArray)
{
	m_pArray = pArray;
	m_iIter = pArray->begin();
}

void IrisArrayIteratorTag::Next()
{
	++m_iIter;
}

bool IrisArrayIteratorTag::IsEnd()
{
	return m_pArray->end() == m_iIter;
}

const IrisValue IrisArrayIteratorTag::GetValue()
{
	return *m_iIter;
}

const vector<IrisValue>::iterator& IrisArrayIteratorTag::GetIter()
{
	return m_iIter;
}

IrisArrayIteratorTag::IrisArrayIteratorTag()
{
}


IrisArrayIteratorTag::~IrisArrayIteratorTag()
{
}
