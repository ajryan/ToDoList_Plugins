// PluginHelpers.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "pluginhelpers.h"
#include "TaskList.h"
#include "UIExtension.h"
#include "ColorUtil.h"

#include "..\..\ToDoList_Dev\Interfaces\ITasklist.h"

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Abstractspoon::Tdl::PluginHelpers;

////////////////////////////////////////////////////////////////////////////////////////////////

TaskList::TaskList(ITaskList* pTaskList) 
	: 
m_pTaskList(GetITLInterface<ITaskList15>(pTaskList, IID_TASKLIST14)), 
	m_pConstTaskList(nullptr) 
{
} 

TaskList::TaskList(const ITaskList* pTaskList) 
	: 
m_pTaskList(nullptr), 
	m_pConstTaskList(GetITLInterface<ITaskList15>(pTaskList, IID_TASKLIST14)) 
{
} 

// private constructor
TaskList::TaskList() : m_pTaskList(nullptr), m_pConstTaskList(nullptr)
{

}

bool TaskList::IsValid()
{
	return (m_pConstTaskList || m_pTaskList);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// GETTERS

#define GETVAL(fn, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn() : (m_pTaskList ? m_pTaskList->fn() : errret))

#define GETSTR(fn) \
	gcnew String(m_pConstTaskList ? m_pConstTaskList->fn() : (m_pTaskList ? m_pTaskList->fn() : L""))

#define GETVAL_ARG(fn, arg, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(arg) : (m_pTaskList ? m_pTaskList->fn(arg) : errret))

#define GETSTR_ARG(fn, arg) \
	gcnew String(m_pConstTaskList ? m_pConstTaskList->fn(arg) : (m_pTaskList ? m_pTaskList->fn(arg) : L""))

// -------------------------------------------------------------

String^ TaskList::GetReportTitle()
{
	return GETSTR(GetReportTitle);
}

String^ TaskList::GetReportDate()
{
	return GETSTR(GetReportDate);
}

String^ TaskList::GetMetaData(String^ sKey)
{
	return GETSTR_ARG(GetMetaData, MS(sKey));
}

UInt32 TaskList::GetCustomAttributeCount()
{
	return GETVAL(GetCustomAttributeCount, 0);
}

String^ TaskList::GetCustomAttributeLabel(int nIndex)
{
	return GETSTR_ARG(GetCustomAttributeLabel, nIndex);
}

String^ TaskList::GetCustomAttributeID(int nIndex)
{
	return GETSTR_ARG(GetCustomAttributeID, nIndex);
}

UInt32  TaskList::GetCustomAttributeType(int nIndex)
{
	return GETVAL_ARG(GetCustomAttributeType, nIndex, 0);
}

String^ TaskList::GetCustomAttributeListData(int nIndex)
{
	return GETSTR_ARG(GetCustomAttributeListData, nIndex);
}

Boolean TaskList::IsCustomAttributeEnabled(int nIndex)
{
	return GETVAL_ARG(IsCustomAttributeEnabled, nIndex, false);
}

String^ TaskList::GetCustomAttributeValue(int nIndex, String^ sItem)
{
	LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetCustomAttributeValue(nIndex, MS(sItem)) : 
		(m_pTaskList ? m_pTaskList->GetCustomAttributeValue(nIndex, MS(sItem)) : L""));

return gcnew String(szValue);
}

UInt32 TaskList::GetTaskCount()
{
	return GETVAL(GetTaskCount, 0);
}

Task^ TaskList::FindTask(UInt32 dwTaskID)
{
	HTASKITEM hTask = GETVAL_ARG(FindTask, dwTaskID, NULL);

	return gcnew Task((m_pConstTaskList ? m_pConstTaskList : m_pTaskList), hTask);
}

Task^ TaskList::GetFirstTask()
{
	if (m_pConstTaskList)
		return gcnew Task(m_pConstTaskList, m_pConstTaskList->GetFirstTask(nullptr));

	// else
	return gcnew Task(m_pTaskList, m_pTaskList->GetFirstTask(nullptr));
}

// ---------------------------------------------------------

#define GETTASKVAL(fn, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(m_hTask) : (m_pTaskList ? m_pTaskList->fn(m_hTask) : errret))

#define GETTASKDATE(fn, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(m_hTask, date) : (m_pTaskList ? m_pTaskList->fn(m_hTask, date) : errret))

#define GETTASKSTR(fn) \
	gcnew String(GETTASKVAL(fn, L""))

#define GETTASKVAL_ARG(fn, arg, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(m_hTask, arg) : (m_pTaskList ? m_pTaskList->fn(m_hTask, arg) : errret))

#define GETTASKSTR_ARG(fn, arg) \
	gcnew String(GETTASKVAL_ARG(fn, arg, L""))

#define GETTASKDATE_ARG(fn, arg, errret) \
	(m_pConstTaskList ? m_pConstTaskList->fn(m_hTask, arg, date) : (m_pTaskList ? m_pTaskList->fn(m_hTask, arg, date) : errret))

// ---------------------------------------------------------

Task::Task()
	: 
m_pTaskList(nullptr), 
	m_pConstTaskList(nullptr), 
	m_hTask(nullptr)
{

}

Task::Task(ITaskList* pTaskList, HTASKITEM hTask) 
	: 
m_pTaskList(GetITLInterface<ITaskList15>(pTaskList, IID_TASKLIST14)), 
	m_pConstTaskList(nullptr), 
	m_hTask(hTask)
{

}

Task::Task(const ITaskList* pTaskList, HTASKITEM hTask)
	: 
m_pTaskList(nullptr), 
	m_pConstTaskList(GetITLInterface<ITaskList15>(pTaskList, IID_TASKLIST14)), 
	m_hTask(hTask)
{

}

Task::Task(const Task^ task)
{
	m_pTaskList = task->m_pTaskList;
	m_pConstTaskList = task->m_pConstTaskList;
	m_hTask = task->m_hTask;
}

bool Task::IsValid()
{
	return ((m_pConstTaskList || m_pTaskList) && (m_hTask != nullptr));
}

Task^ Task::GetFirstSubtask()
{
	if (m_pConstTaskList)
		return gcnew Task(m_pConstTaskList, m_pConstTaskList->GetFirstTask(m_hTask));

	// else
	return gcnew Task(m_pTaskList, m_pTaskList->GetFirstTask(m_hTask));
}

Task^ Task::GetNextTask()
{
	if (m_pConstTaskList)
		return gcnew Task(m_pConstTaskList, m_pConstTaskList->GetNextTask(m_hTask));

	// else
	return gcnew Task(m_pTaskList, m_pTaskList->GetNextTask(m_hTask));
}

Task^ Task::GetParentTask()
{
	if (m_pConstTaskList)
		return gcnew Task(m_pConstTaskList, m_pConstTaskList->GetTaskParent(m_hTask));

	// else
	return gcnew Task(m_pTaskList, m_pTaskList->GetTaskParent(m_hTask));
}

String^ Task::GetTitle()
{
	return GETTASKSTR(GetTaskTitle);
}

String^ Task::GetComments()
{
	return GETTASKSTR(GetTaskComments);
}

String^ Task::GetAllocatedBy()
{
	return GETTASKSTR(GetTaskAllocatedBy);
}

String^ Task::GetStatus()
{
	return GETTASKSTR(GetTaskStatus);
}

String^ Task::GetWebColor()
{
	return GETTASKSTR(GetTaskWebColor);
}

String^ Task::GetPriorityWebColor()
{
	return GETTASKSTR(GetTaskPriorityWebColor);
}

String^ Task::GetVersion()
{
	return GETTASKSTR(GetTaskVersion);
}

String^ Task::GetExternalID()
{
	return GETTASKSTR(GetTaskExternalID);
}

String^ Task::GetCreatedBy()
{
	return GETTASKSTR(GetTaskCreatedBy);
}

String^ Task::GetPositionString()
{
	return GETTASKSTR(GetTaskPositionString);
}

String^ Task::GetIcon()
{
	return GETTASKSTR(GetTaskIcon);
}

UInt32 Task::GetID()
{
	return GETTASKVAL(GetTaskID, 0);
}

UInt32 Task::GetColor()
{
	return GETTASKVAL(GetTaskColor, 0);
}

System::Drawing::Color Task::GetTextDrawingColor()
{
	return ColorUtil::GetDrawingColor(GETTASKVAL(GetTaskTextColor, 0));
}

System::Windows::Media::Color Task::GetTextMediaColor()
{
	return ColorUtil::GetMediaColor(GETTASKVAL(GetTaskTextColor, 0));
}

System::Drawing::Color Task::GetPriorityDrawingColor()
{
	return ColorUtil::GetDrawingColor(GETTASKVAL(GetTaskPriorityColor, 0));
}

System::Windows::Media::Color Task::GetPriorityMediaColor()
{
	return ColorUtil::GetMediaColor(GETTASKVAL(GetTaskPriorityColor, 0));
}

UInt32 Task::GetPosition()
{
	return GETTASKVAL(GetTaskPosition, 0);
}

UInt32 Task::GetPriority()
{
	return GETTASKVAL_ARG(GetTaskPriority, FALSE, 0);
}

UInt32 Task::GetRisk()
{
	return GETTASKVAL_ARG(GetTaskRisk, FALSE, 0);
}

List<String^>^ Task::GetAllocatedTo()
{
	System::Collections::Generic::List<String^>^ items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskAllocatedToCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskAllocatedTo, nIndex));

	return items;
}

List<String^>^ Task::GetCategory()
{
	System::Collections::Generic::List<String^>^ items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskCategoryCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskCategory, nIndex));

	return items;
}

List<String^>^ Task::GetTag()
{
	System::Collections::Generic::List<String^>^ items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskTagCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskTag, nIndex));

	return items;
}

List<String^>^ Task::GetDependency()
{
	System::Collections::Generic::List<String^>^ items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskDependencyCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskDependency, nIndex));

	return items;
}

List<String^>^ Task::GetFileReference()
{
	System::Collections::Generic::List<String^>^ items = gcnew System::Collections::Generic::List<String^>;
	int numItems = GETTASKVAL(GetTaskFileLinkCount, 0);

	for (int nIndex = 0; nIndex < numItems; nIndex++)
		items->Add(GETTASKSTR_ARG(GetTaskFileLink, nIndex));

	return items;
}

Byte Task::GetPercentDone()
{
	return GETTASKVAL_ARG(GetTaskPercentDone, FALSE, 0);
}

double Task::GetCost()
{
	return GETTASKVAL_ARG(GetTaskCost, FALSE, 0);
}

DateTime Task::GetLastModified()
{
	__int64 date = 0;
	GETTASKDATE(GetTaskLastModified64, 0);

	return Map(date);
}

DateTime Task::GetDoneDate()
{
	__int64 date = 0;
	GETTASKDATE(GetTaskDoneDate64, 0);

	return Map(date);
}

DateTime Task::GetDueDate()
{
	__int64 date = 0;
	GETTASKDATE_ARG(GetTaskDueDate64, FALSE, 0);

	return Map(date);
}

DateTime Task::GetStartDate()
{
	__int64 date = 0;
	GETTASKDATE_ARG(GetTaskStartDate64, FALSE, 0);

	return Map(date);
}

DateTime Task::GetCreationDate()
{
	__int64 date = 0;
	GETTASKDATE(GetTaskCreationDate64, 0);

	return Map(date);
}

String^ Task::GetDoneDateString()
{
	return GETTASKSTR(GetTaskDoneDateString);
}

String^ Task::GetDueDateString()
{
	return GETTASKSTR_ARG(GetTaskDueDateString, FALSE);
}

String^ Task::GetStartDateString()
{
	return GETTASKSTR_ARG(GetTaskStartDateString, FALSE);
}

String^ Task::GetCreationDateString()
{
	return GETTASKSTR(GetTaskCreationDateString);
}

Boolean Task::IsDone()
{
	return GETTASKVAL(IsTaskDone, false);
}

Boolean Task::IsDue()
{
	return GETTASKVAL(IsTaskDue, false);
}

Boolean Task::IsGoodAsDone()
{
	return GETTASKVAL(IsTaskGoodAsDone, false);
}

Boolean Task::IsFlagged()
{
	return GETTASKVAL(IsTaskFlagged, false);
}

Boolean Task::IsParent()
{
	return GETTASKVAL(IsTaskParent, false);
}

// ---------------------------------------------------------

double Task::GetTimeEstimate(TimeUnits% cUnits)
{
	TDC_UNITS nUnits = TDCU_NULL;

	double dTime = (m_pConstTaskList ? m_pConstTaskList->GetTaskTimeEstimate(m_hTask, nUnits, FALSE) :
		(m_pTaskList ? m_pTaskList->GetTaskTimeEstimate(m_hTask, nUnits, FALSE) : 0.0));

	cUnits = Map(nUnits);
	return dTime;
}

double Task::GetTimeSpent(TimeUnits% cUnits)
{
	TDC_UNITS nUnits = TDCU_NULL;

	double dTime = (m_pConstTaskList ? m_pConstTaskList->GetTaskTimeSpent(m_hTask, nUnits, FALSE) :
		(m_pTaskList ? m_pTaskList->GetTaskTimeSpent(m_hTask, nUnits, FALSE) : 0.0));

	cUnits = Map(nUnits);
	return dTime;
}

Boolean Task::GetRecurrence()
{
	// TODO
	return false;
}

Boolean Task::HasAttribute(String^ sAttrib)
{
	return (m_pConstTaskList ? m_pConstTaskList->TaskHasAttribute(m_hTask, MS(sAttrib)) : 
		(m_pTaskList ? m_pTaskList->TaskHasAttribute(m_hTask, MS(sAttrib)) : false));
}

String^ Task::GetAttribute(String^ sAttrib)
{
	LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetTaskAttribute(m_hTask, MS(sAttrib)) : 
		(m_pTaskList ? m_pTaskList->GetTaskAttribute(m_hTask, MS(sAttrib)) : L""));

return gcnew String(szValue);
}

String^ Task::GetCustomAttributeData(String^ sID)
{
	LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetTaskCustomAttributeData(m_hTask, MS(sID)) : 
		(m_pTaskList ? m_pTaskList->GetTaskCustomAttributeData(m_hTask, MS(sID)) : L""));

return gcnew String(szValue);
}

String^ Task::GetMetaData(String^ sKey)
{
	LPCWSTR szValue = (m_pConstTaskList ? m_pConstTaskList->GetTaskMetaData(m_hTask, MS(sKey)) : 
		(m_pTaskList ? m_pTaskList->GetTaskMetaData(m_hTask, MS(sKey)) : L""));

return gcnew String(szValue);
}

// TODO

////////////////////////////////////////////////////////////////////////////////////////////////
// SETTERS

Task^ TaskList::NewTask(String^ sTitle)
{
	HTASKITEM hTask = (m_pTaskList ? m_pTaskList->NewTask(MS(sTitle), nullptr, 0) : nullptr);

	return gcnew Task(m_pTaskList, hTask);
}

Boolean TaskList::AddCustomAttribute(String^ sID, String^ sLabel, String^ sColumn)
{
	return (m_pTaskList ? m_pTaskList->AddCustomAttribute(MS(sID), MS(sLabel), MS(sColumn)) : false);
}

Boolean TaskList::SetMetaData(String^ sKey, String^ sValue)
{
	return (m_pTaskList ? m_pTaskList->SetMetaData(MS(sKey), MS(sValue)) : false);
}

Boolean TaskList::ClearMetaData(String^ sKey)
{
	return (m_pTaskList ? m_pTaskList->ClearMetaData(MS(sKey)) : false);
}

// ---------------------------------------------------------

#define SETTASKVAL(fn, val) \
	(m_pTaskList ? m_pTaskList->fn(m_hTask, val) : false)

#define SETTASKSTR(fn, str) \
	(m_pTaskList ? m_pTaskList->fn(m_hTask, MS(str)) : false)

// ---------------------------------------------------------

Task^ Task::NewSubtask(String^ sTitle)
{
	HTASKITEM hTask = (m_pTaskList ? m_pTaskList->NewTask(MS(sTitle), nullptr, 0) : nullptr);

	return gcnew Task(m_pTaskList, hTask);
}

Boolean Task::SetTitle(String^ sTitle)
{
	return SETTASKSTR(SetTaskTitle, sTitle);
}

Boolean Task::SetComments(String^ sComments)
{
	return SETTASKSTR(SetTaskComments, sComments);
}

Boolean Task::SetAllocatedBy(String^ sAllocBy)
{
	return SETTASKSTR(SetTaskAllocatedBy, sAllocBy);
}

Boolean Task::SetStatus(String^ sStatus)
{
	return SETTASKSTR(SetTaskStatus, sStatus);
}

Boolean Task::SetVersion(String^ sVersion)
{
	return SETTASKSTR(SetTaskVersion, sVersion);
}

Boolean Task::SetExternalID(String^ sExternalID)
{
	return SETTASKSTR(SetTaskExternalID, sExternalID);
}

Boolean Task::SetCreatedBy(String^ sCreatedBy)
{
	return SETTASKSTR(SetTaskCreatedBy, sCreatedBy);
}

Boolean Task::SetPosition(String^ sPosition)
{
	return SETTASKSTR(SetTaskPosition, sPosition);
}

Boolean Task::SetIcon(String^ sIcon)
{
	return SETTASKSTR(SetTaskIcon, sIcon);
}

Boolean Task::AddAllocatedTo(String^ sAllocTo)
{
	return SETTASKSTR(AddTaskAllocatedTo, sAllocTo);
}

Boolean Task::AddCategory(String^ sCategory)
{
	return SETTASKSTR(AddTaskCategory, sCategory);
}

Boolean Task::AddTag(String^ sTag)
{
	return SETTASKSTR(AddTaskTag, sTag);
}

Boolean Task::AddDependency(String^ sDependency)
{
	return SETTASKSTR(AddTaskDependency, sDependency);
}

Boolean Task::AddFileReference(String^ sFileLink)
{
	return SETTASKSTR(AddTaskFileLink, sFileLink);
}

Boolean Task::SetColor(UINT32 color)
{
	return SETTASKVAL(SetTaskColor, color);
}

Boolean Task::SetPriority(Byte nPriority)
{
	return SETTASKVAL(SetTaskPriority, nPriority);
}

Boolean Task::SetRisk(Byte Risk)
{
	return SETTASKVAL(SetTaskRisk, Risk);
}

Boolean Task::SetPercentDone(Byte nPercent)
{
	return SETTASKVAL(SetTaskPercentDone, nPercent);
}

Boolean Task::SetCost(double dCost)
{
	return SETTASKVAL(SetTaskCost, dCost);
}

Boolean Task::SetFlag(Boolean bFlag)
{
	return SETTASKVAL(SetTaskFlag, bFlag);
}

Boolean Task::SetLastModified(DateTime^ dtLastMod)
{
	return SETTASKVAL(SetTaskLastModified, Map(dtLastMod));
}

Boolean Task::SetDoneDate(DateTime^ dtCompletion)
{
	return SETTASKVAL(SetTaskDoneDate, Map(dtCompletion));
}

Boolean Task::SetDueDate(DateTime^ dtDue)
{
	return SETTASKVAL(SetTaskDueDate, Map(dtDue));
}

Boolean Task::SetStartDate(DateTime^ dtStart)
{
	return SETTASKVAL(SetTaskStartDate, Map(dtStart));
}

Boolean Task::SetCreationDate(DateTime^ dtCreation)
{
	return SETTASKVAL(SetTaskCreationDate, Map(dtCreation));
}

Boolean Task::SetTimeEstimate(double dTime, TimeUnits cUnits)
{
	return (m_pTaskList ? m_pTaskList->SetTaskTimeEstimate(m_hTask, dTime, Map(cUnits)) : false);
}

Boolean Task::SetTimeSpent(double dTime, TimeUnits cUnits)
{
	return (m_pTaskList ? m_pTaskList->SetTaskTimeSpent(m_hTask, dTime, Map(cUnits)) : false);
}

Boolean Task::SetCustomAttributeData(String^ sID, String^ sValue)
{
	return (m_pTaskList ? m_pTaskList->SetTaskCustomAttributeData(m_hTask, MS(sID), MS(sValue)) : false);
}

Boolean Task::ClearCustomAttributeData(String^ sID)
{
	return (m_pTaskList ? m_pTaskList->ClearTaskCustomAttributeData(m_hTask, MS(sID)) : false);
}

Boolean Task::SetMetaData(String^ sKey, String^ sValue)
{
	return (m_pTaskList ? m_pTaskList->SetTaskMetaData(m_hTask, MS(sKey), MS(sValue)) : false);
}

Boolean Task::ClearMetaData(String^ sKey)
{
	return (m_pTaskList ? m_pTaskList->ClearTaskMetaData(m_hTask, MS(sKey)) : false);
}

Task::TimeUnits Task::Map(TDC_UNITS units)
{
	switch (units)
	{
	case TDCU_MINS:		return TimeUnits::Minutes;
	case TDCU_HOURS:	return TimeUnits::Hours;
	case TDCU_DAYS:		return TimeUnits::Days;
	case TDCU_WEEKDAYS:	return TimeUnits::Weekdays;
	case TDCU_WEEKS:	return TimeUnits::Weeks;
	case TDCU_MONTHS:	return TimeUnits::Months;
	case TDCU_YEARS:	return TimeUnits::Years;
	}

	// default
	return TimeUnits::Unknown;
}

TDC_UNITS Task::Map(TimeUnits units)
{
	switch (units)
	{
	case TimeUnits::Minutes:	return TDCU_MINS;
	case TimeUnits::Hours:		return TDCU_HOURS;
	case TimeUnits::Days:		return TDCU_DAYS;
	case TimeUnits::Weekdays:	return TDCU_WEEKDAYS;
	case TimeUnits::Weeks:		return TDCU_WEEKS;
	case TimeUnits::Months:		return TDCU_MONTHS;
	case TimeUnits::Years:		return TDCU_YEARS;
	}

	// default
	return TDCU_NULL;
}

DateTime Task::Map(Int64 tDate)
{
	DateTime^ date = gcnew DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind::Utc);

	return date->AddSeconds(static_cast<double>(tDate)).ToLocalTime();
}

Int64 Task::Map(DateTime^ date)
{
	DateTime utc = date->ToUniversalTime();
	DateTime^ epoch = gcnew DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind::Utc);

	return static_cast<Int64>((utc - *epoch).TotalSeconds);
}

////////////////////////////////////////////////////////////////////////////////////////////////
