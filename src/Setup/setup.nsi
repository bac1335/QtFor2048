;?; 该脚本使用 HM VNISEdit 脚本编辑器向导产生

; 安装程序初始定义常量
!define PRODUCT_NAME "2048Game"
!define CHINESE_NAME "2048游戏"
!define PRODUCT_VERSION "1.0.0"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${PRODUCT_NAME}.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_DRIVER_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\DemoForge Mirage Driver for TightVNC_is1"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
;自己添加
!define PRODUCT_REINSTALL_KEY "Software\MicroRecording\MicroRecording-installer"
!define PRODUCT_AUTORUN_KEY "Software\Microsoft\Windows\CurrentVersion\Run"
!define PRODUCT_VERSION_KEY "Software\MicroRecording\MicroRecording"
;设置窗口置顶
;!define MUI_PAGE_CUSTOMFUNCTION_SHOW Page_Show
SetCompressor lzma

!include "x64.nsh"

; ------ MUI 现代界面定义 (1.67 版本以上兼容) ------
!include "MUI.nsh"

; MUI 预定义常量
;!define MUI_ABORTWARNING
!define MUI_ICON "setup.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; 语言选择窗口常量设置
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 许可协议页面
;!insertmacro MUI_PAGE_LICENSE "licence.txt"
; 安装目录选择页面
!insertmacro MUI_PAGE_DIRECTORY
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
; 安装完成页面
!insertmacro MUI_PAGE_FINISH

; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES

; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "SimpChinese"

; 安装预释放文件
!insertmacro MUI_RESERVEFILE_LANGDLL
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; ------ MUI 现代界面定义结束 ------
LangString CURLAN ${LANG_ENGLISH} "en"
LangString CURLAN ${LANG_SIMPCHINESE} "中文"

LangString ProgLinkName ${LANG_SIMPCHINESE} ${CHINESE_NAME}
LangString ProgLinkName ${LANG_ENGLISH} "${PRODUCT_NAME}"

LangString UninstallBeforeInstall ${LANG_SIMPCHINESE} "检测到系统已经安装了 $(ProgLinkName)，$\r$\n卸载之前的版本以继续?"
LangString UninstallBeforeInstall ${LANG_ENGLISH} "${PRODUCT_NAME} already installed $\r$\n we must uninstall it before continuing the setup. continue?"


LangString RuningTip ${LANG_SIMPCHINESE} "检测到 $(ProgLinkName) 正在运行, 请先退出程序然后再尝试!"
LangString RuningTip ${LANG_ENGLISH} "Setup has detected ${PRODUCT_NAME} is running, please exit the program and try again!"

LangString UninstallSuccessTip ${LANG_SIMPCHINESE} "已经成功卸载 $(^Name)."
LangString UninstallSuccessTip ${LANG_ENGLISH} "$(^Name) has been successfully removed from your computer."

LangString UninstallConfirmTip ${LANG_SIMPCHINESE} "确定卸载 $(^Name) 及其所有组件?"
LangString UninstallConfirmTip ${LANG_ENGLISH} "You really want to completely remove the $(^Name) , and all of its components?"


Name "${PRODUCT_NAME}${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
ShowInstDetails show
ShowUnInstDetails show

;添加所有需要的文件
Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite try

  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(ProgLinkName).lnk" "$INSTDIR\${PRODUCT_NAME}.exe"
  CreateShortCut "$DESKTOP\$(ProgLinkName).lnk" "$INSTDIR\${PRODUCT_NAME}.exe"
  File /r ".\Setup\*"

  IfFileExists "$SYSDIR\msvcp120.dll" +2 0
  nsExec::Exec /TIMEOUT=3600000 "$INSTDIR\runtime\vcredist_x86_2013.exe /q"

SectionEnd

Section -AdditionalIcons
	SetShellVarContext all
  ;WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  ;CreateShortCut "$SMPROGRAMS\MicroRecording\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
  ; 防火墙规则添加收
  ;ExecDos::exec 'netsh advfirewall firewall add rule name="${PRODUCT_NAME}" dir=in program="$INSTDIR\${PRODUCT_NAME}.exe" action=allow'
  ;ExecDos::exec 'netsh advfirewall firewall add rule name="${PRODUCT_NAME}" dir=out program="$INSTDIR\${PRODUCT_NAME}.exe" action=allow'
  
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\${PRODUCT_NAME}.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\${PRODUCT_NAME}.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  ;WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_NAME}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_VERSION_KEY}" "CurrVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_VERSION_KEY}" "" "$INSTDIR"
  
SectionEnd

;运行库判定操作系统位数进行安装
;Section
;  ${If} ${RunningX64}
;   //电脑为64为操作系统时，调用相应的外部安装程序
;   ExecWait '"$INSTDIR\runtime\vc_redist2015.x64.exe" /quiet'
;   ExecWait '"$INSTDIR\runtime\vcredist2010_x64.exe" /quiet'
;  ${Else}
;   ExecWait '"$INSTDIR\runtime\vc_redist2015.x86.exe" /quiet'
;   ExecWait '"$INSTDIR\runtime\vcredist2010_x86.exe" /quiet'
;  ${EndIf}
;SectionEnd

;
;以下是抄的老版本里面的写法
LangString SecCoreDesp ${LANG_SIMPCHINESE} "${CHINESE_NAME}"
LangString SecCoreDesp ${LANG_ENGLISH} "${PRODUCT_NAME}"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SEC01} $(SecCoreDesp)
!insertmacro MUI_FUNCTION_DESCRIPTION_END


#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

Function .onInit

	
  
   Pop $R0
   IntCmp $R0 1 0 no_run
   MessageBox MB_ICONSTOP $(RuningTip)
   Quit
   no_run:

  Push $0
  Push $1
  ReadRegStr $0 HKLM ${PRODUCT_UNINST_KEY} "UninstallString"
  StrCmp $0 "" onInit.End
  IfFileExists $0 0 onInit.End
  MessageBox MB_YESNO|MB_ICONQUESTION $(UninstallBeforeInstall) /SD IDYES IDNO onInit.GoAbort
  system::call 'Kernel32::GetModuleFileName(i 0,t .r1,i 256)i'
  WriteRegStr HKLM ${PRODUCT_REINSTALL_KEY} "" $1
  
  StrCpy $0 '"$0" /S'
	ExecWait $0
	Goto onInit.GoAbort
	
onInit.GoAbort:
    Abort
	onInit.End:
  Pop $1
  Pop $0

  !insertmacro MUI_LANGDLL_DISPLAY ;语言选择画面
;WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\ffmpeg\ffmpeg.exe" "RUNASADMIN"
;WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\HYEvaluate.exe" "RUNASADMIN"
;WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers" "$INSTDIR\HyScreenRecord.exe" "RUNASADMIN"

FunctionEnd



/******************************
 *  以下是安装程序的卸载部分  *
 ******************************/


Section Uninstall

  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  RMDir /r "$INSTDIR"
  
    ;防火墙规则删除
  ExecDos::exec 'netsh advfirewall firewall delete rule name="${PRODUCT_NAME}"'
  
  ;删除开始菜单和桌面的快捷方式
  SetShellVarContext all

  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
  ;Delete "$SMPROGRAMS\${PRODUCT_NAME}\Website.lnk"
  Delete "$DESKTOP\$(ProgLinkName).lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\$(ProgLinkName).lnk"

  RMDir /r "$SMPROGRAMS\${PRODUCT_NAME}"
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  
  DeleteRegValue ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_AUTORUN_KEY}" "${PRODUCT_NAME}"
  DeleteRegKey HKLM "${PRODUCT_VERSION_KEY}"
  
  SetAutoClose true
SectionEnd

#-- 根据 NSIS 脚本编辑规则，所有 Function 区段必须放置在 Section 区段之后编写，以避免安装程序出现未可预知的问题。--#

Function un.onInit

   FindProcDLL::FindProc "${PRODUCT_NAME}.exe"
   Pop $R0
   IntCmp $R0 1 0 no_run
   MessageBox MB_ICONSTOP $(RuningTip)
   Quit
   no_run:
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "您确实要完全移除 $(^Name) ，及其所有的组件？" IDYES +2
  Abort
  
  KillProcDLL::KillProc "MicroClassRecorder.exe"
  sleep 1000
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) 已成功地从您的计算机移除。"
  Push $0
  ReadRegStr $0 HKLM ${PRODUCT_REINSTALL_KEY} ""
  StrCmp $0 "" un.onUnEnd
  ReadRegStr $1 HKLM ${PRODUCT_REINSTALL_KEY} "params"
  StrCpy $0 '"$0" $1'
  Exec $0
  un.onUnEnd:
  DeleteRegKey HKLM ${PRODUCT_REINSTALL_KEY}
  Pop $0
FunctionEnd

;Function Page_Show
;StrCpy $0 $HWNDPARENT
;     System::Call "user32::SetWindowPos(i r0, i -1,i 0,i 0,i 0,i 0,i 3)"
;BringToFront

;FunctionEnd

/* Section "VCRedist2010" SecVC2010
  SetOutPath "$INSTDIR\runtime"
    SetOverwrite on
  SetCompress off
  File "runtime\dfmirage-setup-2.0.301.exe"
	Exec "$INSTDIR\runtime\dfmirage-setup-2.0.301.exe /q # silent install"
SectionEnd */
