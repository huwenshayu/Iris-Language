#include "IrisInterpreter/IrisNativeClasses/IrisClassBase.h"


IrisValue IrisClassBase::InitializeFunction(IrisValue & ivObj, IIrisValues * ivsValues, IIrisValues * ivsVariableValues, IIrisContextEnvironment * pContextEnvironment) {
	return ivObj;
}

IrisValue IrisClassBase::GetClassName(IrisValue & ivObj, IIrisValues * ivsValues, IIrisValues * ivsVariableValues, IIrisContextEnvironment * pContextEnvironment) {
	// ��ȡ��ָ��
	IrisClassBaseTag* pClass = IrisDevUtil::GetNativePointer<IrisClassBaseTag*>(ivObj);
	const string& strClassName = pClass->GetThisClassName();
	//����String
	return IrisDevUtil::CreateInstanceByInstantValue(strClassName.c_str());
}

IrisValue IrisClassBase::New(IrisValue & ivObj, IIrisValues * ivsValues, IIrisValues * ivsVariableValues, IIrisContextEnvironment * pContextEnvironment) {
	IrisClassBaseTag* pClass = IrisDevUtil::GetNativePointer<IrisClassBaseTag*>(ivObj);
	return IrisDevUtil::CreateInstance(pClass->GetClass()->GetExternClass(), ivsVariableValues, pContextEnvironment);
}

IrisClassBase::IrisClassBase()
{
}


IrisClassBase::~IrisClassBase()
{
}
