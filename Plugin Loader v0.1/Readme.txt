*******************************
 PluginLoader v0.1    by NEG
*******************************
original language is japanese

��outline��
========
�K�v�ȂƂ��Ƀv���O�C�������[�h�ł���悤�ɂ���OllyDbg1.10�p�v���O�C��
It can load any plugin, when you need  
for OllyDbg1.10

��function��
========
1.�ő�100�̃v���O�C�����T�|�[�g
support max 100 plugin
2.�ő�100�̃V���[�g�J�b�g�L�[���T�|�[�g
support max 100 shortcutkey 

��notes��
========
�f�o�b�O�C�x���g���󂯎��v���O�C���̏ꍇ�A�v���Z�X�쐬��Ƀ��[�h����ƃv���Z�X�쐬�C�x���g�Ȃǂ��󂯎��Ȃ�
if you load plugin after process was made, plugin can't receive dbg event

OllyDbg.exe������������v���O�C���̏ꍇ�A�v���O�C�����A�����[�h����ƃA�N�Z�X�ᔽ����������ꍇ������
unload patching OllyDbg.exe plugin, cause access violation

udd�t�@�C����ǂݏ�������v���O�C���̏ꍇ�A�v���Z�X�쐬��Ƀ��[�h�����udd��ǂ߂��A�v���Z�X�I���O�ɃA�����[�h�����udd�Ƀf�[�^��ۑ��ł��Ȃ�
if you load plugin after process was made, plugin can't read udd
if you unload plugin before process was finish, plugin can't save udd


��ini form��
=============
[OllyDump]					;display name of plugin�Byou can use &
FileName=.\Plugin2\OllyDump301.dll		;File name
AutoLoad=0					;Auto = 1  not = 0
Shortcut=0x44,CS,0,PM_DISASM; 0x41,A,63,PM_MAIN	;shortcutkey ( you need ; )

;shortcutkey syntax: [virtual key code],[C(Ctrl)S(Shift)A(Alt)],[menu ID],[window class]
;use ; to separate next shortcutkey

;;window class definition
;PM_MAIN        = 0;   // Main window (NULL)
;PM_DUMP        = 10;  // Any Dump window (t_dump* )
;PM_MODULES     = 11;  // Modules window (t_module* )
;PM_MEMORY      = 12;  // Memory window (t_memory* )
;PM_THREADS     = 13;  // Threads window (t_thread* )
;PM_BREAKPOINTS = 14;  // Breakpoints window (t_bpoint* )
;PM_REFERENCES  = 15;  // References window (t_ref* )
;PM_RTRACE      = 16;  // Run trace window (int* )
;PM_WATCHES     = 17;  // Watches window (1-based index)
;PM_WINDOWS     = 18;  // Windows window (t_window* )
;PM_DISASM      = 31;  // CPU Disassembler (t_dump* )
;PM_CPUDUMP     = 32;  // CPU Dump (t_dump* )
;PM_CPUSTACK    = 33;  // CPU Stack (t_dump* )
;PM_CPUREGS     = 34;  // CPU Registers (t_reg* )

;;example
;0x44,CS,0,PM_DISASM	Ctrl+Shift+D = click plugin menu on disassemble window "0 &Dump debugged process" (only disassemble window)
;0x41,A,63,PM_MAIN	Alt+A = click plugin menu "63 &About" (only window class PM_MAIN)


Excuse me for my bad english    

translated by Ryohma