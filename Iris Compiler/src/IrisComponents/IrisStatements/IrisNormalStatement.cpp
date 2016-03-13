#include "IrisComponents/IrisStatements/IrisNormalStatement.h"
#include "IrisComponents/IrisExpressions/IrisExpression.h"
#include "IrisCompiler.h"


bool IrisNormalStatement::Generate()
{
	IrisCompiler* pCompiler = IrisCompiler::CurrentCompiler();
	pCompiler->SetLineNumber(m_nLineNumber);
	return m_pExpression->Generate();
}

IrisNormalStatement::IrisNormalStatement(IrisExpression* pExpression) : m_pExpression(pExpression)
{
}


IrisNormalStatement::~IrisNormalStatement()
{
	if (m_pExpression)
		delete m_pExpression;
}
