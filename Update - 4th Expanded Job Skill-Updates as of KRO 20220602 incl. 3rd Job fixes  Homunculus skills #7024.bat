@ECHO OFF
TITLE Update Server.
ECHO Getting the latest Git from rAthena Master PR. 4th Expanded Job Skill-Updates as of KRO 20220602 incl. 3rd Job fixes  Homunculus skills #7024
Echo Please wait ...
git pull https://github.com/datawulf/rathena.git skillupd
Echo ---------------------------------------------------
timeout /t 1
cls
msg * "Update complete."
exit
