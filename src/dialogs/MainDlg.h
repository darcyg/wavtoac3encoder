﻿#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include "configuration\Configuration.h"
#include "controls\MyDialogEx.h"
#include "controls\MyButton.h"
#include "controls\MyStatic.h"
#include "controls\MyEdit.h"
#include "controls\MyComboBoxEdit.h"
#include "controls\MyListCtrl.h"
#include "controls\MyComboBox.h"
#include "controls\MySliderCtrl.h"
#include "libaften\aften.h"
#ifndef DISABLE_AVISYNTH
#include "worker\Avs2Raw.h"
#endif

#define IDC_STATUSBAR 1500
#define IDC_COMBOBOX 1501
#define ID_OPTIONS_MENU_START 4000
#define ID_LANGUAGE_MENU_START 2000
#define ID_LANGUAGE_MENU_MAX 2999

class ItemToMove
{
public:
    int nItem0;
    int nItem1;
public:
    ItemToMove()
    {
    }
    ItemToMove(const ItemToMove &other)
    {
        Copy(other);
    }
    ItemToMove& operator=(const ItemToMove &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~ItemToMove()
    {
    }
public:
    void Copy(const ItemToMove &other)
    {
        this->nItem0 = other.nItem0;
        this->nItem1 = other.nItem1;
    }
};

class CMainDlg : public CMyDialogEx
{
    DECLARE_DYNAMIC(CMainDlg)
public:
    CMainDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_DIALOG_MAIN };
protected:
    HICON m_hIcon;
    HACCEL m_hAccelTable;
public:
    bool bVisible;
    AftenAPI api;
    int nSortColumn;
    bool nSortOrder[2];
    bool bSettingsValueVisible;
    ConfigList m_EngineList;
    EncoderPresetList encPresets;
    CEncoderPreset defaultPreset;
    int nCurrentPreset = 0;
    CString szOutputPath;
    CString szOutputFile;
    bool bMultipleMonoInput;
    bool bDisableAllWarnings;
    bool bSaveConfig;
public:
    CStatusBarCtrl m_StatusBar;
    CMySliderCtrl m_SldBitrate;
    CMyListCtrl m_LstSettings;
    CMyListCtrl m_LstFiles;
    CMyStatic m_StcBitrate;
    CMyStatic m_StcSelected;
    CMyStatic m_StcPreconfigured;
    CMyStatic m_StcQualityBitrate;
    CMyComboBox m_CmbRawSampleFormat;
    CMyComboBox m_CmbValue;
    CMyComboBox m_CmbPresets;
    CMyComboBox m_CmbEngines;
    CMyComboBoxEdit m_EdtCmbPresetName;
    CMyEdit m_EdtRawSamplerate;
    CMyEdit m_EdtRawChannels;
    CMyEdit m_EdtOutPath;
    CMyEdit m_EdtThreads;
    CMyButton m_ChkVbr;
    CMyButton m_BtnEncode;
    CMyButton m_ChkSimdMMX;
    CMyButton m_ChkSimdSSE;
    CMyButton m_ChkSimdSSE2;
    CMyButton m_ChkSimdSSE3;
    CMyButton m_BtnResetCurrent;
    CMyButton m_BtnRemove;
    CMyButton m_BtnAddNew;
    CMyButton m_BtnAddFiles;
    CMyButton m_BtnBrowse;
    CMyButton m_ChkMultipleMonoInput;
    CMyButton m_BtnMuxWizard;
    CMyButton m_BtnEngines;
    CSpinButtonCtrl m_SpnRawSampleRate;
    CSpinButtonCtrl m_SpnRawChannels;
    CSpinButtonCtrl m_SpnThreads;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
    void InitTitle();
    void InitTooltips();
    void InitSettingsList();
    void InitDefaultPreset();
    void InitRawSamleFormatComboBox();
    void InitSettingsListGroups();
    void InitDialogControls();
    void InitLang(bool initLangMenu);
    void InitLangButtons();
    void InitLangStaticText();
    void InitLangFilesList();
    void InitLangFilesListContextMenu(CMenu &m_hMenu);
    void InitLangSettingsList();
    void InitLangMainMenu();
    void InitLangMenu();
    bool LoadFilesList(CString &szFileName);
    bool SaveFilesList(CString &szFileName, int nFormat);
    bool LoadProgramConfig(CString szFileName);
    bool SaveProgramConfig(CString szFileName);
    bool UpdateProgramEngines();
    bool LoadProgramEngines(CString szFileName);
    bool SaveProgramEngines(CString szFileName);
    void LoadAllConfiguration();
    void SaveAllConfiguration();
    CEncoderPreset& GetCurrentPreset();
    void HandleDropFiles(HDROP hDropInfo);
    void SearchFolderForFiles(CString szFile, const bool bRecurse);
    void AddItemToFileList(CString szPath);
    void UpdateBitrateText();
    void UpdateSettingsComboBox(int nItem);
    void ApplyPresetToDlg(CEncoderPreset &Preset);
    void ShowOptionPopup(bool bUseRect);
#ifndef DISABLE_AVISYNTH
    bool GetAvisynthFileInfo(CString szFileName, AvsAudioInfo *pInfoAVS);
#endif
protected:
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnFileAddFiles();
    afx_msg void OnFileAddDirectory();
    afx_msg void OnFileMuxWizard();
    afx_msg void OnFileLoadFilesList();
    afx_msg void OnFileSaveFilesList();
    afx_msg void OnFileLoadPresets();
    afx_msg void OnFileSavePresets();
    afx_msg void OnFileExit();
    afx_msg void OnOptionsDisableAllWarnings();
    afx_msg void OnOptionsSaveConfigurationOnExit();
    afx_msg void OnOptionsLoadConfiguration();
    afx_msg void OnOptionsSaveConfiguration();
    afx_msg void OnLanguageChangeDefault();
    afx_msg void OnLanguageChange(UINT nID);
    afx_msg void OnHelpWebsite();
    afx_msg void OnHelpAbout();
    afx_msg void OnListAddFiles();
    afx_msg void OnListAddDirectory();
    afx_msg void OnListMuxWizard();
    afx_msg void OnListLoadList();
    afx_msg void OnListSavelist();
    afx_msg void OnListMoveUp();
    afx_msg void OnListMoveDown();
    afx_msg void OnListDelFiles();
    afx_msg void OnListClearList();
    afx_msg void OnBnClickedCheckVbr();
    afx_msg void OnBnClickedButtonPresetAdd();
    afx_msg void OnBnClickedButtonPresetDel();
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonEncode();
    afx_msg void OnBnClickedCheckSimdMmx();
    afx_msg void OnBnClickedCheckSimdSse();
    afx_msg void OnBnClickedCheckSimdSse2();
    afx_msg void OnBnClickedCheckSimdSse3();
    afx_msg void OnBnClickedButtonBrowse();
    afx_msg void OnBnClickedCheckMultipleMonoInput();
    afx_msg void OnBnClickedButtonPresetsDefaults();
    afx_msg void OnBnClickedButtonMuxWizard();
    afx_msg void OnBnClickedButtonEngines();
    afx_msg void OnCbnSelchangeComboSetting();
    afx_msg void OnCbnSelchangeComboPresets();
    afx_msg void OnCbnSelchangeComboRawSampleFormat();
    afx_msg void OnCbnSelchangeComboEngines();
    afx_msg void OnLvnItemchangedListSettings(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnKeydownListFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnColumnclickListFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnKeydownListSettings(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRclickListFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRclickListSettings(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkListSettings(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkListFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEditRawSampleRate();
    afx_msg void OnEnChangeEditRawChannels();
    afx_msg void OnEnChangeEditThreads();
    afx_msg void OnEnChangeEditOutputPath();
    afx_msg void OnEnSetfocusEditOutputPath();
    afx_msg void OnEnSetfocusEditRawSampleRate();
    afx_msg void OnEnSetfocusEditRawChannels();
    afx_msg void OnEnSetfocusEditThreads();
    afx_msg void OnEnKillfocusEditOutputPath();
    afx_msg void OnEnKillfocusEditRawSampleRate();
    afx_msg void OnEnKillfocusEditRawChannels();
    afx_msg void OnEnKillfocusEditThreads();
    afx_msg LRESULT EditChangeComboPresets(WPARAM wParam, LPARAM lParam);
};
