; stagehand.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install stagehand.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "Stagehand"

; The file to write
OutFile "StagehandInstaller.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Stagehand

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Stagehand" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Stagehand (required)"

  SectionIn RO
    
      ; Set output path to the installation directory.
        SetOutPath $INSTDIR
	  
	    ; Put file there
	      File /R "output\*"
	        
		  ; Write the installation path into the registry
		    WriteRegStr HKLM SOFTWARE\Stagehand "Install_Dir" "$INSTDIR"
		      
		        ; Write the uninstall keys for Windows
			  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Stagehand" "DisplayName" "NSIS Stagehand"
			    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Stagehand" "UninstallString" '"$INSTDIR\uninstall.exe"'
			      WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Stagehand" "NoModify" 1
			        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Stagehand" "NoRepair" 1
				  WriteUninstaller "uninstall.exe"
				    
				    SectionEnd

				    ; Optional section (can be disabled by the user)
				    Section "Start Menu Shortcuts"

				      CreateDirectory "$SMPROGRAMS\Stagehand"
				        CreateShortCut "$SMPROGRAMS\Stagehand\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
					  CreateShortCut "$SMPROGRAMS\Stagehand\Stagehand.lnk" "$INSTDIR\stagehand.bat" "" "$INSTDIR\stagehand.nsi" 0 SW_SHOWMINIMIZED
					    
					    SectionEnd

					    ;--------------------------------

					    ; Uninstaller

					    Section "Uninstall"
					      
					        ; Remove registry keys
						  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Stagehand"
						    DeleteRegKey HKLM SOFTWARE\NSIS_Stagehand

						      ; Remove files and uninstaller
						        Delete $INSTDIR\stagehand.nsi
							  Delete $INSTDIR\uninstall.exe

							    ; Remove shortcuts, if any
							      Delete "$SMPROGRAMS\Stagehand\*.*"

							        ; Remove directories used
								  RMDir "$SMPROGRAMS\Stagehand"
								    RMDir "$INSTDIR"

								    SectionEnd

