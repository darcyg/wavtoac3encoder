﻿#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include "version.h"
#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS
#define _AFX_PORTABLE
#define _FORCENAMELESSUNION
#define _AFX_NO_DAO_SUPPORT
#define _AFX_NO_DHTML_SUPPORT
#define _AFX_NO_OLEDB_SUPPORT
#define _AFX_NO_RICHEDIT_SUPPORT

#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif

#include <afxcontrolbars.h>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#ifndef _UNICODE
#include <direct.h>
#endif
#include <shlwapi.h>

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <thread>

#include "controls\MyButton.h"
#include "controls\MyComboBox.h"
#include "controls\MyComboBoxEdit.h"
#include "controls\MyDialogEx.h"
#include "controls\MyEdit.h"
#include "controls\MyHyperlink.h"
#include "controls\MyListCtrl.h"
#include "controls\MyProgressCtrl.h"
#include "controls\MySliderCtrl.h"
#include "controls\MySpinButtonCtrl.h"
#include "controls\MyStatic.h"
#include "controls\MyStatusBarCtrl.h"

#include "utilities\Log.h"
#include "utilities\FileLog.h"
#include "utilities\StringHelper.h"
#include "utilities\Utilities.h"

#include "configuration\Configuration.h"

#include "avs\src\Avs2Raw\Avs2Raw.h"
#include "worker\Worker.h"
