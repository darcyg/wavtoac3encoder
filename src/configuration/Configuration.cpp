﻿#pragma once

#include "StdAfx.h"
#include "Configuration.h"
#include "utilities\Utilities.h"
#include "utilities\MyFile.h"

CConfiguration::CConfiguration()
{
}

CConfiguration::~CConfiguration()
{
}

bool CConfiguration::LoadConfig(CString &szFileName, ConfigList_t &cl)
{
    try
    {
        CMyFile fp;
        if (fp.FOpen(szFileName, false) == false)
            return false;

        // clear list
        cl.RemoveAll();

        TCHAR Buffer;
        const ULONGLONG nLength = fp.FSize();
        CString szBuffer = _T("");

        while (fp.FRead(Buffer) == true)
        {
            if ((Buffer != '\r') && (Buffer != '\n'))
                szBuffer += Buffer;

            // we have full line if there is end of line mark or end of file
            if ((Buffer == '\n') || (fp.FPos() == nLength))
            {
                szBuffer += _T("\0");

                int nPos = szBuffer.Find('=', 0);
                if (nPos != -1)
                {
                    ConfigEntry ce;
                    ce.szKey = szBuffer.Mid(0, nPos);
                    ce.szValue = szBuffer.Mid(nPos + 1, szBuffer.GetLength() - 1);
                    cl.Insert(ce);
                }

                szBuffer = _T("");
            }
        }

        fp.FClose();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool CConfiguration::SaveConfig(CString &szFileName, ConfigList_t &cl)
{
    int nSize = cl.Count();
    try
    {
        CMyFile fp;
        if (fp.FOpen(szFileName, true) == false)
            return false;

        for (int i = 0; i < nSize; i++)
        {
            CString szBuffer;
            auto& ce = cl.Get(i);

            // format and save key/data pair
            szBuffer.Format(_T("%s=%s\r\n"), ce.szKey, ce.szValue);
            fp.FWriteString(szBuffer.GetBuffer(), szBuffer.GetLength());
            szBuffer.ReleaseBuffer();
        }

        fp.FClose();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void CConfiguration::SearchFolderForLang(CString szPath, const bool bRecurse, LangList_t& m_LangLst)
{
    try
    {
        WIN32_FIND_DATA w32FileData;
        HANDLE hSearch = NULL;
        BOOL fFinished = FALSE;
        TCHAR cTempBuf[(MAX_PATH * 2) + 1];

        ZeroMemory(&w32FileData, sizeof(WIN32_FIND_DATA));
        ZeroMemory(cTempBuf, MAX_PATH * 2);

        // remove '\' or '/' from end of search path
        szPath.TrimRight(_T("\\"));
        szPath.TrimRight(_T("/"));

        wsprintf(cTempBuf, _T("%s\\*.*\0"), szPath);

        hSearch = FindFirstFile(cTempBuf, &w32FileData);
        if (hSearch == INVALID_HANDLE_VALUE)
            return;

        while (fFinished == FALSE)
        {
            if (w32FileData.cFileName[0] != '.' &&
                !(w32FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                CString szTempBuf;
                szTempBuf.Format(_T("%s\\%s\0"), szPath, w32FileData.cFileName);

                // apply filter and add only .txt files
                CString szExt = ::PathFindExtension(szTempBuf);
                szExt.MakeLower();
                szExt.Remove('.');

                // add only files with proper file extensions
                if (szExt.CompareNoCase(_T("txt")) == 0)
                {
                    Lang lang;

                    if (this->LoadLang(szTempBuf, lang.lm) == true)
                    {
                        lang.szFileName = szTempBuf;
                        lang.szEnglishName = lang.lm.Get(0x00000001);
                        lang.szTargetName = lang.lm.Get(0x00000002);

                        m_LangLst.Insert(lang);
                    }
                }
            }

            if (w32FileData.cFileName[0] != '.' &&
                w32FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                wsprintf(cTempBuf, _T("%s\\%s\0"), szPath, w32FileData.cFileName);

                // recurse subdirs
                if (bRecurse == true)
                    SearchFolderForLang(cTempBuf, true, m_LangLst);
            }

            if (FindNextFile(hSearch, &w32FileData) == FALSE)
            {
                if (GetLastError() == ERROR_NO_MORE_FILES)
                    fFinished = TRUE;
                else
                    return;
            }
        }

        if (FindClose(hSearch) == FALSE)
            return;
    }
    catch (...)
    {
        MessageBox(NULL,
            HaveLangStrings() ? GetLangString(0x0020702A) : _T("Error while searching for files!"),
            HaveLangStrings() ? GetLangString(0x00207010) : _T("Error"),
            MB_OK | MB_ICONERROR);
    }
}

bool CConfiguration::LoadLang(CString &szFileName, LangMap_t& lm)
{
    try
    {
        CMyFile fp;
        if (fp.FOpen(szFileName, false) == false)
            return false;

        // clear list
        lm.RemoveAll();

        TCHAR Buffer;
        const ULONGLONG nLength = fp.FSize();
        CString szBuffer = _T("");

        int key;
        CString szKey = _T("");
        CString szValue = _T("");

        while (fp.FRead(Buffer) == true)
        {
            if ((Buffer != '\r') && (Buffer != '\n'))
                szBuffer += Buffer;

            // we have full line if there is end of line mark or end of file
            if ((Buffer == '\n') || (fp.FPos() == nLength))
            {
                szBuffer += _T("\0");

                int nPos = szBuffer.Find('=', 0);
                if (nPos != -1)
                {
                    szKey = szBuffer.Mid(0, nPos);
                    szValue = szBuffer.Mid(nPos + 1, szBuffer.GetLength() - 1);

                    szValue.Replace(_T("\\n"), _T("\n"));
                    szValue.Replace(_T("\\t"), _T("\t"));

                    _stscanf(szKey, _T("%x"), &key);

                    lm.Set(key, szValue);
                }

                szBuffer = _T("");
            }
        }

        fp.FClose();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool CConfiguration::LoadLangConfig(CString &szFileName)
{
    try
    {
        CMyFile fp;
        if (fp.FOpen(szFileName, false) == false)
            return false;

        ULONGLONG  nRead = 0, nLength = fp.FSize();
        if (nLength == 0)
        {
            fp.FClose();
            return true;
        }

        TCHAR Buffer;
        CString szBuffer = _T("");
        int nFileCounter = 0;

        while (fp.FRead(Buffer) == true)
        {
            if ((Buffer != '\r') && (Buffer != '\n'))
                szBuffer += Buffer;

            if (Buffer == '\n' || nRead == nLength - (fp.FMode() == 1 ? 1 : sizeof(TCHAR)))
            {
                szBuffer += _T("\0");

                // terminate reading if max of input files is reached
                if (nFileCounter >= 1)
                {
                    fp.FClose();
                    return true;
                }

                // remove leading and trailing quotes (used for *.mux file format)
                szBuffer.TrimLeft('"');
                szBuffer.TrimRight('"');

                m_szLangFileName = szBuffer;

                // update file counter
                nFileCounter++;

                // reset buffer
                szBuffer = _T("");
            }

            nRead++;
        }

        fp.FClose();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool CConfiguration::SaveLangConfig(CString &szFileName)
{
    try
    {
        CMyFile fp;

        if (fp.FOpen(szFileName, true) == false)
            return false;

        CString szBuffer;

        szBuffer.Format(_T("%s\r\n"), m_szLangFileName);
        fp.FWriteString(szBuffer.GetBuffer(), szBuffer.GetLength());
        szBuffer.ReleaseBuffer();

        fp.FClose();
    }
    catch (...)
    {
        return false;
    }

    return true;
}

void CConfiguration::LoadLangStrings()
{
    CString szLangPath;
    if (m_bIsPortable == true)
    {
        szLangPath = GetExeFilePath() + _T("lang");
    }
    else
    {
        szLangPath = GetSettingsFilePath(_T(""), CString(DEFAULT_CONFIG_DIRECTORY) + _T("\\lang"));
    }

    SearchFolderForLang(szLangPath, false, m_LangLst);

    if (m_LangLst.Count() > 0)
    {
        bool haveLang = false;
        for (int i = 0; i < m_LangLst.Count(); i++)
        {
            auto& lang = m_LangLst.Get(i);
            if (lang.szFileName.Compare(m_szLangFileName) == 0)
            {
                m_nLangId = i;
                m_bHaveLang = TRUE;
                m_Lang = &lang.lm;
                haveLang = true;
                break;
            }
        }

        if (haveLang == false)
        {
            auto& lang = m_LangLst.Get(0);
            m_nLangId = 0;
            m_bHaveLang = TRUE;
            m_Lang = &lang.lm;
            m_szLangFileName = lang.szFileName;
        }
    }
    else
    {
        m_nLangId = -1;
        m_bHaveLang = FALSE;
    }
}

BOOL CConfiguration::HaveLangStrings()
{
    return m_bHaveLang;
}

CString CConfiguration::GetLangString(int id)
{
    CString szValue = _T("");
    if (m_Lang != NULL)
    {
        m_Lang->TryGet(id, szValue);
    }
    return szValue;
}