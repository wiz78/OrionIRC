; $Id: Orion.iss,v 1.28 2006/01/15 12:38:29 wiz Exp $

[Setup]
Compression=lzma/ultra
OutputDir=\\chrome\www\beta\orion\
OutputBaseFilename=OrionSetup
VersionInfoCompany=Simone Tellini
VersionInfoDescription=Orion setup
VersionInfoTextVersion=1.8
VersionInfoVersion=1.8
VersionInfoCopyright=©2003-2006 by Simone Tellini
AppName=Orion
AppVersion=1.8
AppVerName=Orion 1.8
AppCopyright=©2003-2006 by Simone Tellini
AppPublisher=Simone Tellini
AppPublisherURL=http://tellini.info/
AppUpdatesURL=http://tellini.info/software/orion/
AppSupportURL=http://tellini.info/
AppID=Orion
AppMutex=ST_Orion
DefaultDirName={pf}\Orion
AllowNoIcons=true
UninstallDisplayIcon={app}\Orion.exe
UninstallDisplayName=Orion
WizardImageFile=Pics\Setup.bmp
WizardImageBackColor=clBlack
DefaultGroupName=Orion
InfoBeforeFile=Readme.txt
InfoAfterFile=Changes.txt
InternalCompressLevel=ultra
PrivilegesRequired=admin
ShowLanguageDialog=auto
WizardSmallImageFile=compiler:wizmodernsmallimage-IS.bmp
MinVersion=4.1.1998,4.0.1381sp6

[Files]
Source: Orion.exe; DestDir: {app}; Flags: replacesameversion
Source: Readme.txt; DestDir: {app}; Flags: isreadme ignoreversion replacesameversion
Source: Changes.txt; DestDir: {app}; Flags: ignoreversion replacesameversion
;Source: Orion.map; DestDir: {app}
;Source: Catcher.exe; DestDir: {app}
Source: Data\Colors.ini; DestDir: {app}\Data; Flags: onlyifdoesntexist
Source: ..\WizUpdate\WizUpdate.dll; DestDir: {sys}; Flags: sharedfile
Source: pcre.dll; DestDir: {app}; Flags: replacesameversion
;Source: ..\WizSupport\WizSupport.dll; DestDir: {sys}; Flags: sharedfile
;Source: ..\Locale\WizLocale.dll; DestDir: {sys}; Flags: sharedfile
;Source: E:\Borland\CBuilder3\Bin\CP3240MT.DLL; DestDir: {sys}; Flags: sharedfile
;Source: E:\Borland\CBuilder3\Bin\borlndmm.dll; DestDir: {sys}; Flags: sharedfile
Source: OrionScript\Release\OrionScript.dll; DestDir: {app}; Flags: restartreplace
Source: FileProps\Release\FileProps.dll; DestDir: {app}; Flags: restartreplace
Source: PHP_License.txt; DestDir: {app}; Flags: ignoreversion replacesameversion
Source: Zend_License.txt; DestDir: {app}; Flags: ignoreversion replacesameversion

[Registry]
Root: HKCU; Subkey: Software\WizSoftware; ValueType: none; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: Software\WizSoftware\Orion; Flags: uninsdeletekey; ValueType: none

[Icons]
Name: {group}\Orion; Filename: {app}\Orion.exe; WorkingDir: {app}; IconFilename: {app}\Orion.exe; IconIndex: 0
Name: {group}\Orion homepage; Filename: {app}\Orion.url
Name: {userdesktop}\Orion; Filename: {app}\Orion.exe; WorkingDir: {app}; IconFilename: {app}\Orion.exe; Comment: Orion; IconIndex: 0; Tasks: DesktopIcon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\Orion; Filename: {app}\Orion.exe; WorkingDir: {app}; IconFilename: {app}\Orion.exe; Comment: Orion; IconIndex: 0; Tasks: QuickLaunchIcon
Name: {group}\{cm:UninstallProgram,Orion}; Filename: {uninstallexe}
Name: {group}\Readme; Filename: {app}\Readme.txt; Flags: createonlyiffileexists
Name: {group}\Change log; Filename: {app}\Changes.txt; Flags: createonlyiffileexists

[INI]
Filename: {app}\Orion.url; Section: InternetShortcut; Key: URL; String: http://tellini.info/software/orion/

[UninstallDelete]
Type: files; Name: {app}\Orion.url

[_ISTool]
UseAbsolutePaths=false

[Tasks]
Name: DesktopIcon; Description: {cm:CreateDesktopIcon}
Name: QuickLaunchIcon; Description: {cm:CreateQuickLaunchIcon}

[Dirs]
Name: {app}\Downloads
Name: {app}\Data

[Run]
Filename: {app}\Orion.exe; WorkingDir: {app}; Description: {cm:LaunchProgram,Orion}; Flags: shellexec postinstall

[Languages]
Name: English; MessagesFile: compiler:Default.isl
Name: Italian; MessagesFile: compiler:Languages\Italian.isl
Name: German; MessagesFile: compiler:Languages\German.isl
Name: French; MessagesFile: compiler:Languages\French.isl
Name: Russian; MessagesFile: compiler:Languages\Russian.isl
