#include "IrisComponents/IrisStatements/IrisContinueStatement.h"
#include "IrisUnil/IrisIdentifier.h"
#include "IrisInstructorMaker.h"
#include "IrisCompiler.h"
#include "IrisFatalErrorHandler.h"

bool IrisContinueStatement::Generate()
{
	IrisCompiler* pCompiler = IrisCompiler::CurrentCompiler();
	IrisInstructorMaker* pMaker = IrisInstructorMaker::CurrentInstructor();
	pCompiler->SetLineNumber(m_nLineNumber);

	if (!pCompiler->UpperWithLoop()) {
		IrisFatalErrorHandler::CurrentFatalHandler()->ShowFatalErrorMessage(IrisFatalErrorHandler::FatalErrorType::ContinueIrregular, m_nLineNumber, pCompiler->GetCurrentFileIndex(), "Continue Statement CAN ONLY be used in loop if statement/for statement.");
		return false;
	}

	pMaker->ctn();

	return true;
}

IrisContinueStatement::IrisContinueStatement(IrisIdentifier* pLabel) : m_pLabel(pLabel)
{
}


IrisContinueStatement::~IrisContinueStatement()
{
	if (m_pLabel)
		delete m_pLabel;
}
