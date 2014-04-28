/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#include "MiscPch.h"
#include "PathLib.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   include <io.h>
#   include <windows.h>
#   include <ShlObj.h>
#else
#   include <unistd.h>
#endif

namespace Demi
{
    DiString DiPathLib::msAppFile;

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32

    const DiString& _GetAppFileName()
    {
        static DiString appFile;

        char moduleFile[MAX_PATH];
        GetModuleFileNameA(0, moduleFile, MAX_PATH);
        appFile = moduleFile;

        return appFile;
    }

#define _SLASH "\\"

    static bool FileDialogShared(bool bSave, const void* wndHandle, const DiString& dialogTitle,
        const DiString& defaultPath, const DiString& defaultFile, const DiString& fileTypes, uint32 flags, DiVector<DiString>& outFiles)
    {
        DiString defFile = defaultFile;
        DiString defPath = defaultPath;
        defFile.Replace("/", "\\");
        defPath.Replace("/", "\\");

        const int MAX_FILENAME_STR = 65536;
        const int MAX_FILETYPES_STR = 4096;

        CHAR Filename[MAX_FILENAME_STR];
        strcpy(Filename, defFile.c_str());

        CHAR Pathname[MAX_FILENAME_STR];
        strcpy(Pathname, defPath.c_str());

        CHAR FileTypeStr[MAX_FILETYPES_STR];
        CHAR* FileTypesPtr = NULL;
        const int32 FileTypesLen = fileTypes.length();

        DiVector<DiString> CleanExtensionList;

        DiVector<DiString> UnformattedExtensions = fileTypes.Tokenize("|");
        for (size_t ExtensionIndex = 1; ExtensionIndex < UnformattedExtensions.size(); ExtensionIndex += 2)
        {
            const DiString& Extension = UnformattedExtensions[ExtensionIndex];
            if (Extension != "*.*")
            {
                SizeT WildCardIndex = Extension.find("*");
                CleanExtensionList.push_back(WildCardIndex != DiString::npos ? Extension.GetFileExtension() : Extension);
            }
        }

        if (FileTypesLen > 0 && FileTypesLen - 1 < MAX_FILETYPES_STR)
        {
            FileTypesPtr = FileTypeStr;
            strcpy(FileTypeStr, fileTypes.c_str());

            CHAR* Pos = FileTypeStr;
            while (Pos[0] != 0)
            {
                if (Pos[0] == '|')
                    Pos[0] = 0;
                Pos++;
            }

            FileTypeStr[FileTypesLen] = 0;
            FileTypeStr[FileTypesLen + 1] = 0;
        }

        OPENFILENAME ofn;
        ::memset(&ofn, 0, sizeof(OPENFILENAME));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = (HWND)wndHandle;
        ofn.lpstrFilter = FileTypesPtr;
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = Filename;
        ofn.nMaxFile = MAX_FILENAME_STR;
        ofn.lpstrInitialDir = Pathname;
        ofn.lpstrTitle = dialogTitle.c_str();

        if (FileTypesLen > 0)
            ofn.lpstrDefExt = &FileTypeStr[0];

        ofn.Flags = OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER;

        if (bSave)
            ofn.Flags |= OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOVALIDATE;
        else
            ofn.Flags |= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (flags & DiPathLib::MULTIPLE_SELECTION)
            ofn.Flags |= OFN_ALLOWMULTISELECT;

        bool bSuccess;
        if (bSave)
            bSuccess = !!::GetSaveFileName(&ofn);
        else
            bSuccess = !!::GetOpenFileName(&ofn);

        if (bSuccess)
        {
            DiPathLib::ResetCurrentDir();

            if (flags & DiPathLib::MULTIPLE_SELECTION)
            {
                // When selecting multiple files, the returned string is a NULL delimited list
                // where the first element is the directory and all remaining elements are filenames.
                // There is an extra NULL character to indicate the end of the list.
                DiString DirectoryOrSingleFileName = Filename;
                CHAR* Pos = Filename + DirectoryOrSingleFileName.length() + 1;

                outFiles.clear();
                if (Pos[0] == 0)
                {
                    // One item selected. There was an extra trailing NULL character.
                    outFiles.push_back(DirectoryOrSingleFileName);
                }
                else
                {
                    // Multiple items selected. Keep adding filenames until two NULL characters.
                    DiString SelectedFile;
                    do
                    {
                        SelectedFile = DiString(Pos);
                        outFiles.push_back(SelectedFile);
                        Pos += SelectedFile.length() + 1;
                    } while (Pos[0] != 0);
                }
            }
            else
            {
                outFiles.clear();
                outFiles.push_back(Filename);
            }
        }
        else
        {
            uint32 error = ::CommDlgExtendedError();
            if (error != ERROR_SUCCESS)
            {
                DI_WARNING("Cannot get the results from open file dialog");
            }
        }

        return bSuccess;
    }

    static ::INT CALLBACK OpenDirCallback(HWND hwnd, ::UINT uMsg, LPARAM lParam, LPARAM lpData)
    {
        // Set the path to the start path upon initialization.
        switch (uMsg)
        {
        case BFFM_INITIALIZED:
            if (lpData)
            {
                SendMessage(hwnd, BFFM_SETSELECTION, true, lpData);
            }
            break;
        }

        return 0;
    }

    bool DiPathLib::OpenDirectoryDialog(const void* wndHandle, const DiString& title, const DiString& defaultPath, DiString& outFolderName)
    {
        BROWSEINFO bi;
        ZeroMemory(&bi, sizeof(BROWSEINFO));

        TCHAR FolderName[MAX_PATH];
        ZeroMemory(FolderName, sizeof(TCHAR)* MAX_PATH);

        DiString PathToSelect = defaultPath;
        PathToSelect.Replace("/", "\\");

        bi.hwndOwner = (HWND)wndHandle;
        bi.pszDisplayName = FolderName;
        bi.lpszTitle = title.c_str();
        bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS | BIF_SHAREABLE;
        bi.lpfn = OpenDirCallback;
        bi.lParam = (LPARAM)(PathToSelect.c_str());
        bool bSuccess = false;
        LPCITEMIDLIST Folder = ::SHBrowseForFolder(&bi);
        if (Folder)
        {
            bSuccess = (::SHGetPathFromIDList(Folder, FolderName) ? true : false);
            if (bSuccess)
            {
                outFolderName = FolderName;
                //FPaths::NormalizeFilename(OutFolderName);
            }
            else
            {
                DI_WARNING("Failed to launch the open directory dialog");
            }
        }
        else
        {
            DI_WARNING("Failed to launch the open directory dialog");
        }

        return bSuccess;
    }

    void DiPathLib::ResetCurrentDir()
    {
        DiString path = DiPathLib::GetApplicationPath();
        ::SetCurrentDirectory(path.c_str());
    }

#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX || DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   include <mach-o/dyld.h>
    
    const DiString& _GetAppFileName()
    {
        static DiString ret;
        uint32_t pathNameSize = 1024;
        char pathName[1024];
        
        if (!_NSGetExecutablePath(pathName, &pathNameSize))
        {
            char real[PATH_MAX];
            
            if (realpath(pathName, real) != NULL)
            {
                pathNameSize = strlen(real);
                strncpy(pathName, real, pathNameSize);
            }
            
            ret = pathName;
        }
        
        
        
        return ret;
    }

#define _SLASH "/"

#endif

    const DiString& DiPathLib::GetApplicationFileName()
    {
        if (msAppFile.empty())
            msAppFile = _GetAppFileName();
        return msAppFile;
    }

    const DiString& DiPathLib::GetApplicationPath()
    {
        static DiString path;

        const DiString& appFile = GetApplicationFileName();
        size_t pos = appFile.rfind(_SLASH);
        path = appFile.substr(0, pos) + _SLASH;
        return path;
    }

    bool DiPathLib::FileExisted(const DiString& file)
    {
        return (access(file.c_str(), 0) != -1);
    }

    bool DiPathLib::OpenFileDialog(const void* wndHandle, const DiString& title,
        const DiString& defaultPath, const DiString& defaultFile, const DiString& fileTypes, uint32 flags, DiVector<DiString>& outFiles)
    {
        return FileDialogShared(false, wndHandle, title, defaultPath, defaultFile, fileTypes, flags, outFiles);
    }

    bool DiPathLib::SaveFileDialog(const void* wndHandle, const DiString& title, const DiString& defaultPath,
        const DiString& defaultFile, const DiString& fileTypes, uint32 flags, DiVector<DiString>& outFiles)
    {
        return FileDialogShared(true, wndHandle, title, defaultPath, defaultFile, fileTypes, flags, outFiles);
    }

}