/*
*   $Id: actionscript.c,v 1.1 2004/01/03 03:59:19 svoisen Exp $
*
*   Original file copyright (c) 2004, Sean Voisen
*	
*	Modified October 8, 2007 By Mike Fahy (VeryVito) of www.turdhead.com
*		- Added initial AS3 support
*		- Threw in some "TODO" and "NOTE" bits
*
*	Modified October 9, 2007 By Ali Rantakari of hasseg.org:
*		- Added more allowed AS3 attribute keywords (override, final, internal 
* 		  etc...) for classes, getters & setters, variables
*		- Allowed varying versions of "note" and "todo" spellings
*		- Allowed points (.) in package names so that they would display the
* 		  whole package name instead of just the first level
* 		- Added interfaces matching support
* 		- Reformatted some name parameters:
*			- Getters and setters: display either "get" or "set" in front
*			  of the property name
*			- Todos & notes: made the name be the text that comes after the
*			  "todo" or "note" text
*			- Variables: Moved the variable type after the name and separated
*			  them with " : " according to ActionScript syntax
*
*	Modifed January 24, 2010 by David Wilhelm 
*		- removed type tag from end of tag names for better compatibility
*		- removed package tagging
*		- left previous regexes in place in case you want to use them instead
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   This module contains functions for generating tags for ActionScript language
*   files.
*/

/*
*   INCLUDE FILES
*/
#include "general.h"	/* must always come first */
#include "parse.h"

/*
*   FUNCTION DEFINITIONS
* 
*/

static void installActionScriptRegex (const langType language)
{
	// Functions
    /*addTagRegex (language, "^[ \t]*[(private|public|static|protected|internal|final|override)( \t)]*function[ \t]+([A-Za-z0-9_]+)[ \t]*\\(([^\\{]*)", "\\1 (\\2", "f,function,functions,methods", NULL);*/
    addTagRegex (language, "^[ \t]*[(private|public|static|protected|internal|final|override)( \t)]*function[ \t]+([A-Za-z0-9_]+)[ \t]*\\(([^\\{]*)", "\\1", "f,function,functions,methods", NULL);

	// Getters and setters
	/*addTagRegex (language, "^[ \t]*[(public|static|internal|final|override)( \t)]*function[ \t]+(set|get)[ \t]+([A-Za-z0-9_]+)[ \t]*\\(", "\\1 \\2", "p,property,properties", NULL);*/
	addTagRegex (language, "^[ \t]*[(public|static|internal|final|override)( \t)]*function[ \t]+(set|get)[ \t]+([A-Za-z0-9_]+)[ \t]*\\(", "\\1 \\2", "p,property,properties", NULL);

	// Variables
	/*addTagRegex (language, "^[ \t]*[(private|public|static|protected|internal)( \t)]*var[ \t]+([A-Za-z0-9_]+)([ \t]*\\:[ \t]*([A-Za-z0-9_]+))*[ \t]*", "\\3 \\: \\1", "v,variable,variables", NULL);*/
	addTagRegex (language, "^[ \t]*[(private|public|static|protected|internal)( \t)]*var[ \t]+([A-Za-z0-9_]+)([ \t]*\\:[ \t]*([A-Za-z0-9_]+))*[ \t]*", "\\1", "v,variable,variables", NULL);

	// Constants
	/*addTagRegex (language, "^[ \t]*[(private|public|static|protected|internal)( \t)]*const[ \t]+([A-Za-z0-9_]+)([ \t]*\\:[ \t]*([A-Za-z0-9_]+))*[ \t]*", "\\1 : \\3", "v,variable,variables", NULL);*/
	addTagRegex (language, "^[ \t]*[(private|public|static|protected|internal)( \t)]*const[ \t]+([A-Za-z0-9_]+)([ \t]*\\:[ \t]*([A-Za-z0-9_]+))*[ \t]*", "\\1", "v,variable,variables", NULL);

	// Classes
	/*addTagRegex (language, "^[ \t]*[(private|public|static|dynamic|final|internal)( \t)]*class[ \t]+([A-Za-z0-9_]+)[ \t]*([^\\{]*)", "\\1 (\\2)", "c,class,classes", NULL);*/
	addTagRegex (language, "^[ \t]*[(private|public|static|dynamic|final|internal)( \t)]*class[ \t]+([A-Za-z0-9_]+)[ \t]*([^\\{]*)", "\\1", "c,class,classes", NULL);

	// Interfaces
	/*addTagRegex (language, "^[ \t]*[(private|public|static|dynamic|final|internal)( \t)]*interface[ \t]+([A-Za-z0-9_]+)[ \t]*([^\\{]*)", "\\1 (\\2)", "i,interface,interfaces", NULL); */
	addTagRegex (language, "^[ \t]*[(private|public|static|dynamic|final|internal)( \t)]*interface[ \t]+([A-Za-z0-9_]+)[ \t]*([^\\{]*)", "\\1", "i,interface,interfaces", NULL); 

	// Packages
	/*addTagRegex (language, "^[ \t]*[(private|public|static)( \t)]*package[ \t]+([A-Za-z0-9_.]+)[ \t]*", "\\1", "p,package", NULL);*/

	// Notes
	addTagRegex (language, "\\/\\/[ \t]*(NOTE|note|Note)[ \t]*\\:*(.*)", "\\2", "i,{Notes}", NULL);
	
	// Todos
	addTagRegex (language, "\\/\\/[ \t]*(TODO|todo|ToDo|Todo)[ \t]*\\:*(.*)", "\\2", "i,{To do}", NULL);
	
	// Prototypes (Put this in for AS1 compatibility...)
    addTagRegex (language, ".*\\.prototype\\.([A-Za-z0-9 ]+)[ \t]*\\=([ \t]*)function( [ \t]?)*\\(", "\\1", "p,prototype", NULL);
	
}

/* Create parser definition stucture */


extern parserDefinition* ActionScriptParser (void)

{
    static const char *const extensions [] = { "as", NULL };
    parserDefinition *const def = parserNew ("ActionScript");
    def->extensions = extensions;
    def->initialize = installActionScriptRegex;
    def->regex      = TRUE;
    return def;
}

