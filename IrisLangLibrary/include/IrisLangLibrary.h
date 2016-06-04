  // ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� IRISLANGLIBRARY_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// IRISLANGLIBRARY_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef IRISLANGLIBRARY_EXPORTS
#define IRISLANGLIBRARY_API __declspec(dllexport)
#else
#define IRISLANGLIBRARY_API __declspec(dllimport)
#endif

#include "IrisUnil/IrisValue.h"
#include "IrisInterfaces/IIrisClass.h"
#include "IrisInterfaces/IIrisClosureBlock.h"
#include "IrisInterfaces/IIrisContextEnvironment.h"
#include "IrisInterfaces/IIrisInterface.h"
#include "IrisInterfaces/IIrisModule.h"
#include "IrisInterfaces/IIrisObject.h"
#include "IrisInterfaces/IIrisValues.h"

//class IIrisValues;
//class IIrisClass;
//class IIrisModule;
//class IIrisInterface;
//class IIrisContextEnvironment;

typedef IrisValue(*IrisNativeFunction)(IrisValue&, IIrisValues*, IIrisValues*, IIrisContextEnvironment*);
typedef void(*IrisDev_FatalErrorMessageFunction)(char* pMessage);
typedef int(*IrisDev_ExitConditionFunction)();

#define DECLARE_IRISDEV_CLASS_CHECK(klass) IRISLANGLIBRARY_API bool IrisDev_CheckClassIs##klass(const IrisValue& ivValue);

DECLARE_IRISDEV_CLASS_CHECK(Class)
DECLARE_IRISDEV_CLASS_CHECK(Module)
DECLARE_IRISDEV_CLASS_CHECK(Interface)
DECLARE_IRISDEV_CLASS_CHECK(Object)
DECLARE_IRISDEV_CLASS_CHECK(String)
DECLARE_IRISDEV_CLASS_CHECK(UniqueString)
DECLARE_IRISDEV_CLASS_CHECK(Integer)
DECLARE_IRISDEV_CLASS_CHECK(Float)
DECLARE_IRISDEV_CLASS_CHECK(Array)
DECLARE_IRISDEV_CLASS_CHECK(Hash)
DECLARE_IRISDEV_CLASS_CHECK(Range)
DECLARE_IRISDEV_CLASS_CHECK(Block)

IRISLANGLIBRARY_API void* _IrisDev_InnerGetNativePointer(const IrisValue& ivValue);
IRISLANGLIBRARY_API void* _IrisDev_InnerGetNativePointer(IIrisObject* pObject);

#ifndef _IRIS_LIB_DEFINE_
#define _IRIS_LIB_DEFINE_
template<class T>
T IrisDev_GetNativePointer(const IrisValue& ivValue) {
	return static_cast<T>(_IrisDev_InnerGetNativePointer(ivValue));
}

template<class T>
T IrisDev_GetNativePointer(IIrisObject* pObject) { 
	return static_cast<T>(_IrisDev_InnerGetNativePointer(pObject));
}

typedef struct IrisInitializeStruct_tag {
	IrisDev_FatalErrorMessageFunction m_pfFatalErrorMessageFunction;
	IrisDev_ExitConditionFunction m_pfExitConditionFunction;
} IrisInitializeStruct, *PIrisInitializeStruct;
#endif

IRISLANGLIBRARY_API bool IrisDev_CheckClass(const IrisValue& ivValue, const char* szClassName);

IRISLANGLIBRARY_API void IrisDev_GroanIrregularWithString(const char* szIrregularString);

IRISLANGLIBRARY_API int IrisDev_GetInt(const IrisValue& ivValue);
IRISLANGLIBRARY_API double IrisDev_GetFloat(const IrisValue& ivValue);
IRISLANGLIBRARY_API const char* IrisDev_GetString(const IrisValue& ivValue);
IRISLANGLIBRARY_API IrisValue IrisDev_CallMethod(const IrisValue& ivObj, IIrisValues* pParameters, const char* szMethodName);
IRISLANGLIBRARY_API IIrisClass* IrisDev_GetClass(const char* strClassPathName);
IRISLANGLIBRARY_API IIrisModule* IrisDev_GetModule(const char* strClassPathName);
IRISLANGLIBRARY_API IIrisInterface* IrisDev_GetInterface(const char* strClassPathName);

IRISLANGLIBRARY_API bool IrisDev_ObjectIsFixed(const IrisValue& ivObj);
IRISLANGLIBRARY_API IIrisClosureBlock* IrisDev_GetClosureBlock(IIrisContextEnvironment* pContextEnvironment);
IRISLANGLIBRARY_API IrisValue IrisDev_ExcuteClosureBlock(IIrisClosureBlock* pClosureBlock, IIrisValues* pParameters);
IRISLANGLIBRARY_API void IrisDev_ContextEnvironmentSetClosureBlock(IIrisContextEnvironment* pContextEnvironment, IIrisClosureBlock* pBlock);
IRISLANGLIBRARY_API IIrisObject* IrisDev_GetNativeObjectPointer(const IrisValue& ivObj);
IRISLANGLIBRARY_API int IrisDev_GetObjectID(const IrisValue& ivObj);
IRISLANGLIBRARY_API IIrisClass* IrisDev_GetClassOfObject(const IrisValue& ivObj);
IRISLANGLIBRARY_API const char* IrisDev_GetNameOfClass(IIrisClass* pClass);
IRISLANGLIBRARY_API const char* IrisDev_GetNameOfModule(IIrisModule* pModule);
IRISLANGLIBRARY_API const char* IrisDev_GetNameOfInterface(IIrisInterface* pInterface);
IRISLANGLIBRARY_API void IrisDev_MarkObject(const IrisValue& ivObject);
IRISLANGLIBRARY_API void IrisDev_MarkClosureBlock(IIrisClosureBlock* pClosureBlock);

IRISLANGLIBRARY_API const IrisValue& IrisDev_Nil();
IRISLANGLIBRARY_API const IrisValue& IrisDev_False();
IRISLANGLIBRARY_API const IrisValue& IrisDev_True();

IRISLANGLIBRARY_API bool IrisDev_RegistClass(const char* szPath, IIrisClass* pClass);
IRISLANGLIBRARY_API bool IrisDev_RegistModule(const char* szPath, IIrisModule* pModule);
IRISLANGLIBRARY_API bool IrisDev_RegistInterface(const char* szPath, IIrisInterface* pInterface);

//IRISLANGLIBRARY_API bool IrisDev_AddClass(IIrisModule* pModule, IIrisClass* pClass);

IRISLANGLIBRARY_API void IrisDev_AddInstanceMethod(IIrisClass* pClass, const char* szMethodName, IrisNativeFunction pfFunction, size_t nParamCount, bool bIsWithVariableParameter);
IRISLANGLIBRARY_API void IrisDev_AddClassMethod(IIrisClass* pClass, const char* szMethodName, IrisNativeFunction pfFunction, size_t nParamCount, bool bIsWithVariableParameter);

IRISLANGLIBRARY_API void IrisDev_AddInstanceMethod(IIrisModule* pModule, const char* szMethodName, IrisNativeFunction pfFunction, size_t nParamCount, bool bIsWithVariableParameter);
IRISLANGLIBRARY_API void IrisDev_AddClassMethod(IIrisModule* pModule, const char* szMethodName, IrisNativeFunction pfFunction, size_t nParamCount, bool bIsWithVariableParameter);

IRISLANGLIBRARY_API void IrisDev_AddGetter(IIrisClass* pClass, const char* szInstanceVariableName, IrisNativeFunction pfFunction);
IRISLANGLIBRARY_API void IrisDev_AddSetter(IIrisClass* pClass, const char* szInstanceVariableName, IrisNativeFunction pfFunction);

IRISLANGLIBRARY_API void IrisDev_AddConstance(IIrisClass* pClass, const char* szConstanceName, const IrisValue& ivValue);
IRISLANGLIBRARY_API void IrisDev_AddConstance(IIrisModule* pModule, const char* szConstanceName, const IrisValue& ivValue);

IRISLANGLIBRARY_API void IrisDev_AddClassVariable(IIrisClass* pClass, const char* szVariableName, const IrisValue& ivValue);
IRISLANGLIBRARY_API void IrisDev_AddClassVariable(IIrisModule* pModule, const char* szVariableName, const IrisValue& ivValue);

IRISLANGLIBRARY_API void IrisDev_AddModule(IIrisClass* pClass, IIrisModule* pTargetModule);
IRISLANGLIBRARY_API void IrisDev_AddModule(IIrisModule* pModule, IIrisModule* pTargetModule);

IRISLANGLIBRARY_API IrisValue IrisDev_CreateNormalInstance(IIrisClass* pClass, IIrisValues* ivsParams, IIrisContextEnvironment* pContexEnvironment);
IRISLANGLIBRARY_API IrisValue IrisDev_CreateStringInstanceByInstantValue(const char* szString);
IRISLANGLIBRARY_API IrisValue IrisDev_CreateFloatInstanceByInstantValue(double dFloat);
IRISLANGLIBRARY_API IrisValue IrisDev_CreateIntegerInstanceByInstantValue(int nInteger);
IRISLANGLIBRARY_API IrisValue IrisDev_CreateUniqueStringInstanceByUniqueIndex(size_t nIndex);

IRISLANGLIBRARY_API void IrisDev_SetObjectInstanceVariable(IrisValue& ivObj, char* szInstanceVariableName, const IrisValue& ivValue);
IRISLANGLIBRARY_API IrisValue IrisDev_GetObjectInstanceVariable(IrisValue& ivObj, char* szInstanceVariableName);

IRISLANGLIBRARY_API bool IrisDev_IrregularHappened();
IRISLANGLIBRARY_API bool IrisDev_FatalErrorHappened();

IRISLANGLIBRARY_API IIrisValues* IrisDev_CreateIrisValuesList(size_t nSize);
IRISLANGLIBRARY_API void IrisDev_ReleaseIrisValuesList(IIrisValues* pValues);

IRISLANGLIBRARY_API bool IR_Initialize(PIrisInitializeStruct pInitializeStruct);
IRISLANGLIBRARY_API bool IR_Run();
IRISLANGLIBRARY_API bool IR_ShutDown();

IRISLANGLIBRARY_API bool IR_LoadScriptFromPath(char* pScriptFilePath);
IRISLANGLIBRARY_API bool IR_LoadScriptFromVirtualPathAndText(char* pPath, char * pScriptText);

IRISLANGLIBRARY_API bool IR_LoadExtention(char* pExtentionPath);