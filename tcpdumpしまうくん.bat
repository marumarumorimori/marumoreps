@echo off
REM for,if内の変数スコープ変更
setlocal ENABLEEXTENSIONS 

REM 設定ファイルパス
set CONFIG_FILE=config.ini

REM 設定ファイルパスが存在するか確認
if not exist %CONFIG_FILE% (
    echo ERROR: %CONFIG_FILE%が存在しません。
    exit /b 1
)

REM config.iniを読み込む
for /f "usebackq tokens=1,* delims==" %%a in ("%CONFIG_FILE%") do (
	set %%a=%%b
	
	REM ttpmacro.exeまでのPATHを取得
	if "TT_PATH" == "%%a" (
		set TTP_PATH=%%b
	)
	
	REM tcpdump_for_server*.ttlまでのPATHを取得
	if "MACRO_PATH" == "%%a" (
		set MACRO_PATH=%%b
	)

	REM tcpdumpを取るinterfaceの名前を取得
	REM server1のinterface
	if "server1" == "%%a" (
		set SERV_IF1=%%b
	)
	
	REM server2のinterface
	if "server2" == "%%a" (
		set SERV_IF2=%%b
	)
	
	REM server3のinterface
	if "server3" == "%%a" (
		set SERV_IF3=%%b
	)

	REM tcpdumpを取るinterfaceの名前を取得
	REM tcpdmp結果をしまうディレクトリまでのパス
	if "STORE_DIR" == "%%a" (
		set STORE_DIR=%%b
	)
	
	REM tcpdmp結果をしまうディレクトリまでのパス
	if "DIR_PATH" == "%%a" (
		set DIR_PATH=%%b
	)
)

REM 確認用ファイルパス出力
echo "-----ファイルパス-----"
echo "ttpmacro.exeまでのPATH            : %TTP_PATH%"
echo "tcpdump_for_server*.ttlまでのPATH : %MACRO_PATH%"
echo "----------------------"

REM 確認用server,eth対応関係表出力
echo "-------interface------"
echo "server1 : %SERV_IF1%"
echo "server2 : %SERV_IF2%"
echo "server3 : %SERV_IF3%"
echo "----------------------"

REM 確認用格納ディレクトリ表出力
echo "-------directory------"
echo "STORE_DIR : %STORE_DIR%"
echo "DIR_PATH  : %DIR_PATH%"
echo "----------------------"


set /p ITEM_NUM="検査項目番号を入力してね！('ω')/~ >"
echo 項目番号は%ITEM_NUM%と入力されました。(硬派な対応)

REM 3端末同時処理のためstart実行
start %TTP_PATH%\ttpmacro.exe %MACRO_PATH%\tcpdump_for_server1.ttl %SERV_IF1% %ITEM_NUM% %STORE_DIR% %DIR_PATH%
start %TTP_PATH%\ttpmacro.exe %MACRO_PATH%\tcpdump_for_server1.ttl %SERV_IF2% %ITEM_NUM% %STORE_DIR% %DIR_PATH%
start %TTP_PATH%\ttpmacro.exe %MACRO_PATH%\tcpdump_for_server1.ttl %SERV_IF3% %ITEM_NUM% %STORE_DIR% %DIR_PATH%


