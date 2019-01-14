@echo off
REM for,if���̕ϐ��X�R�[�v�ύX
setlocal ENABLEEXTENSIONS 

REM �ݒ�t�@�C���p�X
set CONFIG_FILE=config.ini

REM �ݒ�t�@�C���p�X�����݂��邩�m�F
if not exist %CONFIG_FILE% (
    echo ERROR: %CONFIG_FILE%�����݂��܂���B
    exit /b 1
)

REM config.ini��ǂݍ���
for /f "usebackq tokens=1,* delims==" %%a in ("%CONFIG_FILE%") do (
	set %%a=%%b
	
	REM ttpmacro.exe�܂ł�PATH���擾
	if "TT_PATH" == "%%a" (
		set TTP_PATH=%%b
	)
	
	REM tcpdump_for_server*.ttl�܂ł�PATH���擾
	if "MACRO_PATH" == "%%a" (
		set MACRO_PATH=%%b
	)

	REM tcpdump�����interface�̖��O���擾
	REM server1��interface
	if "server1" == "%%a" (
		set SERV_IF1=%%b
	)
	
	REM server2��interface
	if "server2" == "%%a" (
		set SERV_IF2=%%b
	)
	
	REM server3��interface
	if "server3" == "%%a" (
		set SERV_IF3=%%b
	)

	REM tcpdump�����interface�̖��O���擾
	REM tcpdmp���ʂ����܂��f�B���N�g���܂ł̃p�X
	if "STORE_DIR" == "%%a" (
		set STORE_DIR=%%b
	)
	
	REM tcpdmp���ʂ����܂��f�B���N�g���܂ł̃p�X
	if "DIR_PATH" == "%%a" (
		set DIR_PATH=%%b
	)
)

REM �m�F�p�t�@�C���p�X�o��
echo "-----�t�@�C���p�X-----"
echo "ttpmacro.exe�܂ł�PATH            : %TTP_PATH%"
echo "tcpdump_for_server*.ttl�܂ł�PATH : %MACRO_PATH%"
echo "----------------------"

REM �m�F�pserver,eth�Ή��֌W�\�o��
echo "-------interface------"
echo "server1 : %SERV_IF1%"
echo "server2 : %SERV_IF2%"
echo "server3 : %SERV_IF3%"
echo "----------------------"

REM �m�F�p�i�[�f�B���N�g���\�o��
echo "-------directory------"
echo "STORE_DIR : %STORE_DIR%"
echo "DIR_PATH  : %DIR_PATH%"
echo "----------------------"


set /p ITEM_NUM="�������ڔԍ�����͂��ĂˁI('��')/~ >"
echo ���ڔԍ���%ITEM_NUM%�Ɠ��͂���܂����B(�d�h�ȑΉ�)

REM 3�[�����������̂���start���s
start %TTP_PATH%\ttpmacro.exe %MACRO_PATH%\tcpdump_for_server1.ttl %SERV_IF1% %ITEM_NUM% %STORE_DIR% %DIR_PATH%
start %TTP_PATH%\ttpmacro.exe %MACRO_PATH%\tcpdump_for_server1.ttl %SERV_IF2% %ITEM_NUM% %STORE_DIR% %DIR_PATH%
start %TTP_PATH%\ttpmacro.exe %MACRO_PATH%\tcpdump_for_server1.ttl %SERV_IF3% %ITEM_NUM% %STORE_DIR% %DIR_PATH%


