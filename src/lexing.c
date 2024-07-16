

/*
Important for lexing:
	- parenthesis for priority				()
	- single quotes (no interpretation)		''
	- double quotes (only $-interpretation)	""
	- dollar sign (environment variables)	$
	- exit status of foreground pipeline	$?
	- output to next input -> pipes 		|
	- redirect input 						<
	- redirect output 						>
	- read input until delimiter 			<<
	- redirect output in append mode 		>>
	- AND-execution							&&
	- OR-execution							||
	- wildcards in curr. working directory	*

Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon).
*/

/*
GET ENVIRONMENTS: env / printenv
GET VALUE OF VAR: " printenv varname "
EXPORT GLOBAL VAR: " export varname=value "
EXPORT LOCAL VAR: " varname =value " (or " set varname =value "
CREATE NEW ENV VAR: " export varname=value "
REMOVE ENV VAR: " unset varname "
*/

/*
Fehler in der Ausgabe umleiten:
Find everything in root (/) ending in .log, where errors (2) sent to /dev/null
find / -name *.log 2> /dev/null 
*/