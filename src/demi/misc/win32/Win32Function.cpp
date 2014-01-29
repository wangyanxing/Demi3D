
#include "MiscPch.h"
#include "Win32/Win32Function.h"
#include <signal.h>
#include <windows.h>

#pragma warning(disable:4200)

namespace Demi 
{
#define IDD_DIALOG_ASSERT               101
#define IDC_ASSERT_IGNORE               1001
#define IDC_ASSERT_IGNORE_ALL           1002
#define IDC_ASSERT_BREAK                1003
#define IDC_ASSERT_ABORT                1004
#define IDC_EDIT_ASSERT_VALUE           1005
#define IDC_EDIT_ASSERT_FILE            1006
#define IDC_EDIT_ASSERT_LINE            1007

#define ASSERT_DLG_CAPTION      L"Assert Error - Demi Engine"
#define ASSERT_DLG_FONT         L"MS Shell Dlg"
#define ASSERT_BTN_IGNORE       L"Ignore"
#define ASSERT_BTN_IGNORE_ALL   L"Ignore All"
#define ASSERT_BTN_BREAK        L"Break"
#define ASSERT_BTN_ABORT        L"Abort"
#define ASSERT_BTN_VALUE        L"Value"
#define ASSERT_BTN_LINE         L"Line"
#define ASSERT_BTN_FILE         L"File"

    template<int captionSize>
    struct DialogItem
    {
        DWORD   style;
        DWORD   dwExtendedStyle;
        short   x;
        short   y;
        short   cx;
        short   cy;
        WORD    id;
        WORD    ch;
        WORD    c;
        WCHAR   caption[captionSize];
        WORD    dummy;
    };

    struct AssertDlgData
    {
        DLGTEMPLATE dlgTemplate;
        WORD        menu;
        WORD        wndClass;
        WCHAR       caption[sizeof(ASSERT_DLG_CAPTION)/2];
        WORD        pointSize;
        WCHAR       font[sizeof(ASSERT_DLG_FONT)/2];

        DialogItem<sizeof(ASSERT_BTN_IGNORE)/2>    item0;
        DialogItem<sizeof(ASSERT_BTN_IGNORE_ALL)/2>item1;
        DialogItem<sizeof(ASSERT_BTN_BREAK)/2>     item2;
        DialogItem<sizeof(ASSERT_BTN_ABORT)/2>     item3;
        DialogItem<sizeof(ASSERT_BTN_VALUE)/2>     item4;
        DialogItem<sizeof(ASSERT_BTN_LINE)/2>      item5;
        DialogItem<sizeof(ASSERT_BTN_FILE)/2>      item6;
        DialogItem<sizeof(L"")/2>      item7;
        DialogItem<sizeof(L"")/2>      item8;
        DialogItem<sizeof(L"")/2>      item9;
    };

    static AssertDlgData s_DialogRC =
    {
        {DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU,0,10,0,0,293,109},0,0,ASSERT_DLG_CAPTION,8,ASSERT_DLG_FONT,

        {BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0, 157,84, 50, 14, IDC_ASSERT_IGNORE,     0xFFFF,0x0080,ASSERT_BTN_IGNORE,    0},
        {BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0, 213,84, 50, 14, IDC_ASSERT_IGNORE_ALL, 0xFFFF,0x0080,ASSERT_BTN_IGNORE_ALL,0},
        {BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0, 97 ,84, 50, 14, IDC_ASSERT_BREAK,         0xFFFF,0x0080,ASSERT_BTN_BREAK,     0},
        {BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 0, 33 ,84, 50, 14, IDC_ASSERT_ABORT,         0xFFFF,0x0080,ASSERT_BTN_ABORT,     0},
        {WS_CHILD|WS_VISIBLE,0,18,19,18,8,0,0xFFFF,0x0082,ASSERT_BTN_VALUE, 0},
        {WS_CHILD|WS_VISIBLE,0,18,39,12,8,0,0xFFFF,0x0082,ASSERT_BTN_FILE,  0},
        {WS_CHILD|WS_VISIBLE,0,18,59,14,8,0,0xFFFF,0x0082,ASSERT_BTN_LINE,  0},
        {WS_BORDER|ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_CHILD|WS_VISIBLE,0,39,16,231,15,    IDC_EDIT_ASSERT_VALUE,    0xFFFF,0x0081,L"",0},
        {WS_BORDER|ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_CHILD|WS_VISIBLE,0,39,37,231,15,    IDC_EDIT_ASSERT_FILE,    0xFFFF,0x0081,L"",0},
        {WS_BORDER|ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_CHILD|WS_VISIBLE,0,39,58,231,15,    IDC_EDIT_ASSERT_LINE,    0xFFFF,0x0081,L"",0},
    };

    void DiPlatformFunc::ErrorDlg(const char* msg)
    {
        #ifdef _DEBUG
        OutputDebugString(msg);
        #endif

        MessageBoxA(NULL, msg, "Demi Engine Error", MB_OK|MB_APPLMODAL|MB_SETFOREGROUND|MB_ICONERROR);
        abort();
    }

    struct AssertDesc
    {
        const char* message;
        const char* file;
        int         line;

        enum
        {
            ASSERT_ABORT,
            ASSERT_IGNORE,
            ASSERT_IGNORE_ALL,
            ASSERT_BREAK,
        } result;
    };

    static AssertDesc* currentAssertDesc = NULL;

    static BOOL CALLBACK AssertDlgProc(HWND hDlg, uint32 iMsg, WPARAM wParam, LPARAM lParam)
    {
        HWND hwndDlgItem;
        DiString lineStr;

        switch (iMsg)
        {
        case WM_INITDIALOG:

            int x, y, screenWidth, screenHeight;
            RECT rcDlg;
            GetWindowRect(hDlg, &rcDlg);
            screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
            screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

            x = (screenWidth / 2) - ((rcDlg.right - rcDlg.left) / 2);
            y = (screenHeight / 2) - ((rcDlg.bottom - rcDlg.top) / 2);

            MoveWindow(hDlg, x, y, (rcDlg.right - rcDlg.left),
                (rcDlg.bottom - rcDlg.top), TRUE);

            hwndDlgItem = GetDlgItem(hDlg, IDC_EDIT_ASSERT_VALUE);
            SetWindowTextA(hwndDlgItem, currentAssertDesc->message);

            hwndDlgItem = GetDlgItem(hDlg, IDC_EDIT_ASSERT_FILE);
            SetWindowTextA(hwndDlgItem, currentAssertDesc->file);

            hwndDlgItem = GetDlgItem(hDlg, IDC_EDIT_ASSERT_LINE);
            lineStr.SetInt(currentAssertDesc->line);
            SetWindowTextA(hwndDlgItem, lineStr.c_str());

            return TRUE;

        case WM_COMMAND:
            int xx = LOWORD(wParam);
            switch (xx)
            {
            case IDC_ASSERT_IGNORE:
                currentAssertDesc->result = AssertDesc::ASSERT_IGNORE;
                EndDialog(hDlg, TRUE);
                return TRUE;

            case IDC_ASSERT_IGNORE_ALL:
                currentAssertDesc->result = AssertDesc::ASSERT_IGNORE_ALL;
                EndDialog(hDlg, TRUE);
                return TRUE;

            case IDC_ASSERT_BREAK:
                currentAssertDesc->result = AssertDesc::ASSERT_BREAK;
                EndDialog(hDlg, TRUE);
                return TRUE;

            case IDC_ASSERT_ABORT:
                currentAssertDesc->result = AssertDesc::ASSERT_ABORT;
                EndDialog(hDlg, TRUE);
                return TRUE;

            case IDC_EDIT_ASSERT_VALUE:
            case IDC_EDIT_ASSERT_FILE:
            case IDC_EDIT_ASSERT_LINE:
                return TRUE;

            default:
                currentAssertDesc->result = AssertDesc::ASSERT_IGNORE;
                EndDialog(hDlg, TRUE);
                return TRUE;
            }
        }

        return FALSE;
    }

    bool DiPlatformFunc::AssertDlg(const char* msg, const char* file, int line, bool& ignore)
    {
        static bool ignoreAll = false;

        if (!ignoreAll)
        {
            AssertDesc desc;
            desc.message = msg;
            desc.file = file;
            desc.line = line;

            currentAssertDesc = &desc;

            DialogBoxIndirectParamA(GetModuleHandle(NULL),(DLGTEMPLATE *) &s_DialogRC,GetDesktopWindow(),AssertDlgProc,(LPARAM) NULL);
            DWORD err = GetLastError();

            switch (desc.result)
            {
            case AssertDesc::ASSERT_IGNORE:
                ignore = true;
                return false;
                break;
            case AssertDesc::ASSERT_IGNORE_ALL:
                ignoreAll = true;
                return false;
                break;
            case AssertDesc::ASSERT_BREAK:
                return true;
            case AssertDesc::ASSERT_ABORT:
                raise(SIGABRT);
                exit(-1);
            }
        }
        return true;
    }
}