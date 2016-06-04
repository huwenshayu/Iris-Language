#include "IrisInterpreter/IrisNativeClasses/IrisInterfaceBaseTag.h"
#include "IrisInterpreter/IrisStructure/IrisInterface.h"


IrisInterfaceBaseTag::IrisInterfaceBaseTag(IrisInterface* pInterface) : m_pInterface(pInterface)
{
}

const string& IrisInterfaceBaseTag::GetInterfaceName() {
	return m_pInterface->GetInterfaceName().GetSTLString();
}

void IrisInterfaceBaseTag::SetInterface(IrisInterface* pInterface) {
	m_pInterface = pInterface;
}

IrisInterface* IrisInterfaceBaseTag::GetInterface() {
	return m_pInterface;
}

IrisInterfaceBaseTag::~IrisInterfaceBaseTag()
{
}
