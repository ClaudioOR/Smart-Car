# Smart-Car

Der Framework ist auf OpenGL-Basis aufgebaut, daher hat das Projekt bereits einige Dateien. Die eigentliche Logik befindet sich im Ordner 
'Game Logic'. In der Datei 'Game_Logic.cpp' existiert die 'Game_Logic::compute(..)'-Methode, welche zu jedem Zeitschritt aufgerufen wird.
Saemtliche Logik wird also dort implementiert. Bereits implementiert ist eine Kollisionserkennung mit dem Rand und den Hindernissen, sowie
eine 'Reset()'-Methode welche das Auto zum Initalzustand zurueckfuehrt.


Projekt installieren und ausfuehren:
------------------------------------

-> Zum Ausfuehren MS Visual Studio 2015 Community herunterladen: https://www.visualstudio.com/de-de/downloads/download-visual-studio-vs.aspx
-> Dann Repo klonen oder herunterladen und die Datei 'Smart Car.sln' im Ordner 'Smart Car' ausfuehren
-> Projekt mit 'Strg + F5' builden und starten
=> Sollte es nicht auf anhieb funktionieren, dann schreib mich an ;)

Bedienung (bisher): 
-------------------
-> Taste '2' um das Auto von Hand zu bedienen
	- Tasten 'w', 'a', 's' und 'd' um das Auto zu bewegen
-> Taste '1' um das Auto dem Computer zu ueberlassen (Bisher noch keine Aktion implementiert)