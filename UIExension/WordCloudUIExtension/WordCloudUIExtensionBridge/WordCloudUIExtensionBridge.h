/////////////////////////////////////////////////////////////////////

#include "..\..\..\..\ToDoList_Dev\Interfaces\IUIExtension.h"

#include <vcclr.h>

/////////////////////////////////////////////////////////////////////

// This class is exported from WordCloudUIExtensionBridge.dll
class CWordCloudUIExtensionBridge : public IUIExtension
{
public:
	CWordCloudUIExtensionBridge();

	void Release(); // releases the interface

	LPCTSTR GetMenuText() const;
	HICON GetIcon() const;
	LPCWSTR GetTypeID() const; // caller must copy result only

	IUIExtensionWindow* CreateExtWindow(UINT nCtrlID, DWORD nStyle, 
		long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);
	void SetLocalizer(ITransText* pTT);

	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey);

protected:
	HICON m_hIcon;
	ITransText* m_pTT;
};

class CWordCloudUIExtensionBridgeWindow : public IUIExtensionWindow
{
public:
	CWordCloudUIExtensionBridgeWindow(ITransText* pTT);

	void Release(); // releases the interface
	BOOL Create(UINT nCtrlID, DWORD nStyle, 
		long nLeft, long nTop, long nWidth, long nHeight, HWND hwndParent);

	HICON GetIcon() const;
	LPCWSTR GetMenuText() const; // caller must copy result only
	LPCWSTR GetTypeID() const; // caller must copy result only

	bool SelectTask(DWORD dwTaskID);
	bool SelectTasks(const DWORD* pdwTaskIDs, int nTaskCount);
	bool SupportsTaskSelection() const { return false; }

	void UpdateTasks(const ITaskList* pTasks, IUI_UPDATETYPE nUpdate, const IUI_ATTRIBUTE* pAttributes, int nNumAttributes);
	bool WantEditUpdate(IUI_ATTRIBUTE nAttribute) const;
	bool WantSortUpdate(IUI_ATTRIBUTE nAttribute) const;
	bool PrepareNewTask(ITaskList* pTask) const;

	bool ProcessMessage(MSG* pMsg);
	bool DoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra);
	bool CanDoAppCommand(IUI_APPCOMMAND nCmd, DWORD dwExtra) const;

	bool GetLabelEditRect(LPRECT pEdit); // screen coordinates
	IUI_HITTEST HitTest(const POINT& ptScreen) const;

	void SetUITheme(const UITHEME* pTheme);
	void SetReadOnly(bool bReadOnly);
	HWND GetHwnd() const;

	void SavePreferences(IPreferences* pPrefs, LPCWSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCWSTR szKey, bool bAppOnly);

protected:
	gcroot<WordCloudUIExtension::WordCloudUIExtensionCore^> m_wnd;
	ITransText* m_pTT;
};

DLL_DECLSPEC int GetInterfaceVersion()
{
	return IUIEXTENSION_VERSION;
}

DLL_DECLSPEC IUIExtension* CreateUIExtensionInterface()
{
	return new CWordCloudUIExtensionBridge();
}

