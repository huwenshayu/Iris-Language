#include "IrisInterpreter/IrisNativeClasses/IrisModuleBaseTag.h"
#include "IrisInterpreter/IrisStructure/IrisModule.h"


IrisModuleBaseTag::IrisModuleBaseTag(IrisModule* pModule) : m_pModule(pModule)
{
}

void IrisModuleBaseTag::SetModule(IrisModule* pModule) {
	m_pModule = pModule;
}

const string& IrisModuleBaseTag::GetModuleName() {
	return m_pModule->GetModuleName().GetSTLString();
}

IrisModule* IrisModuleBaseTag::GetModule() {
	return m_pModule;
}

IrisModuleBaseTag::~IrisModuleBaseTag()
{
}
