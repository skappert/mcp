Dim objShell, strSystemPath, strUserPath, rCode, strNewPath
Set objShell = CreateObject("wscript.shell")
strSystemPath    = objShell.RegRead("HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment\Path")
strUserPath      = objShell.Environment.item("PATH")
strUserClasspath = objShell.Environment.item("CLASSPATH")
For each arg in WScript.Arguments  
	pathExtend = arg & "windows\lib;" & arg
	classpathExtend = arg & "windows\lib\dip.jar;."
	rCode = objShell.RegWrite("HKCU\Environment\Path", strUserPath & ";" & pathExtend, "REG_SZ")
	rCode = objShell.RegWrite("HKCU\Environment\CLASSPATH", strUserPath & ";" & classpathExtend, "REG_SZ")
Next
WSCRIPT.QUIT(0)
