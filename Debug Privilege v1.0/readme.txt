�ϴ����� Disable �� OD �� DebugPrivilege Ȩ��, ���ܻ�������.
������ѧѧд OD �Ĳ��, �������������, ֻ�޸ı����Խ��̵� DebugPrivilege Ȩ��.

����ο��� IsDebugPresent �� ASM ����, ��C ʵ��, ����������.

1. �ֶ� Disable �ӽ��� DebugPrivlege
2. �ֶ� Enable  �ӽ��� DebugPrivlege
3. ������غ� Restart ʱ��ѡ���Զ� Disable DebugPrivlege

������ VC6 �±���ͨ��, ��һ�����⻨���Һó�ʱ��Ž��,
OD �ֲ����ᵽ�� DllEntryPoint() ������ͨ��, �����ǲ�ִ��.
���˿� OD �Դ��� VC ���� ������������, ���� DllEntryPoint().

�����Ҹĳ� DllMain() ����ȷ, ����ǲ��� OD Pulgin ��һ��С����?





Because OD enable DebugPrivilege itself, 
so debugee created by OD also has DebugPrivilege.
Some packer will detect OD by checking debugee's DebugPrivilege.

This plugin can control debugee's DebugPrivilege.

1. Disable
    
   Manual disable debugee's DebugPrivilege.

2. Enable
   
   Manual enable debugee's DebugPrivilege.

3. Option
   You can set auto disable debugee's DebugPrivilege when open or restart.

   Thank 2oo3SV for your IsDebugPresent src code.