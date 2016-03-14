#include "IrisInterpreter/IrisStructure/IrisObject.h"
#include "IrisInterpreter/IrisStructure/IrisClass.h"
#include "IrisInterpreter/IrisStructure/IrisModule.h"
#include "IrisInterpreter/IrisNativeClasses/IrisClassBaseTag.h"
#include "IrisInterpreter/IrisNativeClasses/IrisModuleBaseTag.h"
#include "IrisInterpreter.h"
#include "IrisInterpreter/IrisStructure/IrisMethod.h"
#include "IrisUnil/IrisValues.h"
#include "IrisFatalErrorHandler.h"

unsigned int IrisObject::s_nMaxID = 0;

void IrisObject::Mark() {
	m_bIsMaked = true;
	m_pClass->Mark(m_pNativeObject);
	for (auto value : m_mpInstanceMethods) {
		value.second->GetMethodObject()->Mark();
	}
	for (auto value : m_mpInstanceVariables) {
		value.second.GetIrisObject()->Mark();
	}
}

IrisObject::IrisObject() {
	m_nObjectID = ++s_nMaxID;
}

IrisValue IrisObject::CallInstanceFunction(const string& strFunctionName, IIrisContextEnvironment* pContextEnvironment, IIrisValues* ivsValues, CallerSide eSide, unsigned int nLineNumber, int nBelongingFileIndex) {

	// �����Լ���Instance Functions��Ѱ�Ҷ�Ӧ����
	IrisMethod* pMethod = nullptr;
	bool bIsCurClassMethod = false;

	// ���⴦�����������������ģ����������ż̳������ϲ���һֱ��Class��Ϊֹ
	if (m_pClass->GetInternClass()->IsClassClass()) {
		bIsCurClassMethod = false;
		IrisObject* pClassObject = this;
		IrisClass* pClass = static_cast<IrisClassBaseTag*>(pClassObject->m_pNativeObject)->GetClass();

		do {
			pClassObject = static_cast<IrisObject*>(pClass->GetClassObject());

			pMethod = pClassObject->GetInstanceMethod(strFunctionName);

			// ���ݡ���ĵ��������ɼ̳С���ԭ�����е��඼�ܹ�����Class��ĵ��������������������е��඼��Class���ʵ����������е��඼�ܵ���Class���ʵ��������
			// Class��
			if (!pMethod) {
				pMethod = static_cast<IrisObject*>(m_pClass->GetInternClass()->GetClassObject())->GetInstanceMethod(strFunctionName);
			}

			if (!pMethod) {
				bool bDummy = false;
				pMethod = m_pClass->GetInternClass()->GetMethod(strFunctionName, bDummy);
			}

			// ��鸸�������ģ��
			if (!pMethod) {
				auto& hsModules = pClass->GetModules();
				for (auto& module : hsModules) {
					auto pModule = module.second;
					if (pMethod = pModule->GetModuleInstanceMethod(strFunctionName)) {
						break;
					}
				}
			}

			pClass = static_cast<IrisClassBaseTag*>(pClassObject->m_pNativeObject)->GetClass()->GetSuperClass();

		} while (pClass && !pClass->IsClassClass() && !pMethod);
	}
	else if (m_pClass->GetInternClass()->IsModuleClass()) {
		bIsCurClassMethod = false;

		IrisModule* pModule = static_cast<IrisModuleBaseTag*>(m_pNativeObject)->GetModule();

		pMethod = static_cast<IrisObject*>(pModule->GetModuleObject())->GetInstanceMethod(strFunctionName);

		if(!pMethod) {
			auto& hsModules = pModule->GetModules();
			for (auto& module : hsModules) {
				auto pTmpModule = module.second;
				if (pMethod = pTmpModule->GetModuleInstanceMethod(strFunctionName)) {
					break;
				}
			}
		}
	}
	else {
		decltype(m_mpInstanceMethods)::iterator iMethod = m_mpInstanceMethods.find(strFunctionName);
		if (!(iMethod == m_mpInstanceMethods.end())) {
			pMethod = iMethod->second;
			bIsCurClassMethod = true;
		}
	}

	if (!pMethod) {
		pMethod = m_pClass->GetInternClass()->GetMethod(strFunctionName, bIsCurClassMethod);
	}

	IrisValue ivResult;
	if (pMethod) {
		IrisValue ivValue;
		ivValue.SetIrisObject(this);

		// �ڲ�����
		if (eSide == CallerSide::Inside) {
			// �����ڲ�����������
			if (bIsCurClassMethod) {
				ivResult = pMethod->Call(ivValue, static_cast<IrisContextEnvironment*>(pContextEnvironment), static_cast<IrisValues*>(ivsValues), nLineNumber, nBelongingFileIndex);
			}
			// ������ø��෽��
			else {
				// ��ֹ����
				if (pMethod->GetAuthority() == IrisMethod::MethodAuthority::Personal) {
					// **Error**
					IrisFatalErrorHandler::CurrentFatalHandler()->ShowFatalErrorMessage(IrisFatalErrorHandler::FatalErrorType::MethodAuthorityIrregular, nLineNumber, nBelongingFileIndex, "Method of " + strFunctionName + " is PERSONAL and cannot be called from derrived class " + m_pClass->GetInternClass()->GetClassName() + ".");
					ivResult = IrisInterpreter::CurrentInterpreter()->Nil();
				}
				else {
					ivResult = pMethod->Call(ivValue, static_cast<IrisContextEnvironment*>(pContextEnvironment), static_cast<IrisValues*>(ivsValues), nLineNumber, nBelongingFileIndex);
				}
			}
		}
		// �ⲿ����
		else {
			// ֻ�ܵ���EveryOne�ķ���
			// ��ֹ����
			if (pMethod->GetAuthority() != IrisMethod::MethodAuthority::Everyone) {
				// **Error**
				IrisFatalErrorHandler::CurrentFatalHandler()->ShowFatalErrorMessage(IrisFatalErrorHandler::FatalErrorType::MethodAuthorityIrregular, nLineNumber, nBelongingFileIndex,  "Method of " + strFunctionName + " is not EVERYONE and cannot be called outside the class " + m_pClass->GetInternClass()->GetClassName() + " .");
				ivResult = IrisInterpreter::CurrentInterpreter()->Nil();
			}
			else {
				ivResult = pMethod->Call(ivValue, static_cast<IrisContextEnvironment*>(pContextEnvironment), static_cast<IrisValues*>(ivsValues), nLineNumber, nBelongingFileIndex);
			}
		}
	}
	else {
		// **Error**
		IrisFatalErrorHandler::CurrentFatalHandler()->ShowFatalErrorMessage(IrisFatalErrorHandler::FatalErrorType::NoMethodIrregular, nLineNumber, nBelongingFileIndex, "Method of " + strFunctionName + " not found in class " + m_pClass->GetInternClass()->GetClassName() + ".");
		ivResult = IrisInterpreter::CurrentInterpreter()->Nil();
	}

	return ivResult;
}

const IrisValue & IrisObject::GetInstanceValue(const string & strInstanceValueName, bool & bResult) {
	m_iwlInstanceValueWLLock.ReadLock();
	decltype(m_mpInstanceVariables)::iterator iVariable;
	if ((iVariable = m_mpInstanceVariables.find(strInstanceValueName)) == m_mpInstanceVariables.end()) {
		bResult = false;
		m_iwlInstanceValueWLLock.ReadUnlock();
		return IrisInterpreter::CurrentInterpreter()->Nil();
	}
	bResult = true;
	auto& ivResult = iVariable->second;
	m_iwlInstanceValueWLLock.ReadUnlock();
	return ivResult;
}

IrisMethod * IrisObject::GetInstanceMethod(const string & strInstanceMethodName)
{
	m_iwlInstanceMethodWLLock.ReadLock();
	decltype(m_mpInstanceMethods)::iterator iVariable = m_mpInstanceMethods.find(strInstanceMethodName);
	if (iVariable == m_mpInstanceMethods.end()) {
		m_iwlInstanceMethodWLLock.ReadUnlock();
		return nullptr;
	}
	m_iwlInstanceMethodWLLock.ReadUnlock();
	return iVariable->second;
}

void IrisObject::AddInstanceValue(const string & strInstanceValueName, const IrisValue & ivValue) {
	m_iwlInstanceValueWLLock.WriteLock();
	m_mpInstanceVariables.insert(_VariablePair(strInstanceValueName, ivValue));
	m_iwlInstanceValueWLLock.WriteUnlock();
}

void IrisObject::AddSingleInstanceMethod(IrisMethod * pMethod)
{
	m_iwlInstanceMethodWLLock.WriteLock();
	decltype(m_mpInstanceMethods)::iterator iFind = m_mpInstanceMethods.find(pMethod->GetMethodName());

	if (iFind == m_mpInstanceMethods.end()) {
		m_mpInstanceMethods.insert(_MethodPair(pMethod->GetMethodName(), pMethod));
	}
	else {
		m_mpInstanceMethods[pMethod->GetMethodName()] = pMethod;
	}

	m_iwlInstanceMethodWLLock.WriteUnlock();
}

IrisObject::~IrisObject() {
	this->m_pClass->NativeFree(m_pNativeObject);

	for (auto iter : m_mpInstanceMethods) {
		delete iter.second;
	}
}