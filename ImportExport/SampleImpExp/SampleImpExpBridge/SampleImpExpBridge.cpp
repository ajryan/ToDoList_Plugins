// ExporterBridge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SampleImpExpBridge.h"

#include <unknwn.h>
#include <tchar.h>
#include <msclr\auto_gcroot.h>

#include "..\..\..\..\ToDoList_Dev\Interfaces\ITasklist.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\ITransText.h"
#include "..\..\..\..\ToDoList_Dev\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#	using <..\..\..\Debug\SampleImpExpCore.dll>
#	using <..\..\..\Debug\PluginHelpers.dll> as_friend
#else
#	using <..\..\..\Release\SampleImpExpCore.dll>
#	using <..\..\..\Release\PluginHelpers.dll> as_friend
#endif

using namespace SampleImpExp;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

// This is the constructor of a class that has been exported.
// see ExporterBridge.h for the class definition
CSampleImpExpBridge::CSampleImpExpBridge()
{
	return;
}

void CSampleImpExpBridge::Release()
{
	delete this;
}

void CSampleImpExpBridge::SetLocalizer(ITransText* /*pTT*/)
{
	// TODO
}

HICON CSampleImpExpBridge::GetIcon(void) const
{
	// TODO
	return NULL;
}

LPCTSTR CSampleImpExpBridge::GetMenuText() const
{
	return L"Sample";
}

LPCTSTR CSampleImpExpBridge::GetFileFilter() const
{
	return L"smp";
}

LPCTSTR CSampleImpExpBridge::GetFileExtension() const
{
	return L"smp";
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool CSampleImpExpBridge::Export(const ITaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	// call into out sibling C# module to do the actual work
	msclr::auto_gcroot<SampleImpExpCore^> expCore = gcnew SampleImpExpCore();
	msclr::auto_gcroot<Preferences^> prefs = gcnew Preferences(pPrefs);
	msclr::auto_gcroot<TaskList^> srcTasks = gcnew TaskList(pSrcTaskFile);
	
	// do the export
	return expCore->Export(srcTasks.get(), gcnew String(szDestFilePath), (bSilent != FALSE), prefs.get(), gcnew String(szKey));
}

bool CSampleImpExpBridge::Export(const IMultiTaskList* pSrcTaskFile, LPCTSTR szDestFilePath, bool bSilent, IPreferences* pPrefs, LPCTSTR szKey)
{
	// TODO
	return false;
}
