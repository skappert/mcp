echo Starting DIP Browser
set PATH=%CD%\lib;%PATH%
echo Starting DIP Browser 5.4.4
javaw -Djava.library.path=%CD%\lib -jar %CD%\tools\dip-browser-5.4.4.jar
