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
*			  them with " : " according to JavaScript syntax
*
*	Modifed January 24, 2010 by David Wilhelm 
*		- removed type tag from end of tag names for better compatibility
*		- removed package tagging
*		- left previous regexes in place in case you want to use them instead
*
*	Modifed July 20, 2010 by David Wilhelm 
*		- adapted for javascript, including Ext.js syntax
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   This module contains functions for generating tags for JavaScript language
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

static void installJavaScriptRegex (const langType language)
{
	// Old skool Functions
	// function foo (blarg) {...}
    addTagRegex (language, "^[ \t]*function[ \t]+([A-Za-z0-9_$]+)[ \t]*\\(([^\\{]*)", "\\1", "f,function,functions,methods", NULL);
	
	// variables which have function values
	// var f00 = function (blarg) {
	addTagRegex (language, "^[ \t]*var[ \t]+([A-Za-z0-9_$]+)[ \t]*\\=[ \t]+function[ \t]*\\(([^\\{]*)", "\\1", "f,function,functions,methods", NULL);


	// Methods of the form 
	// 	foo : function (blarg) {
	addTagRegex (language, "^[ \t]*([A-Za-z0-9_$]*)[ \t]*\\:[ \t]*function[ \t]*\\(([^\\{]*)", "\\1", "f,function,functions,methods", NULL);

	// properties of the form 
	// this will duplicate methods -- need to filter them out in a post processing stage :/
	// 	foo : bar
	addTagRegex (language, "^[ \t]*([A-Za-z0-9_$]*)[ \t]*\\:[ \t]*", "\\1", "p,property,properties", NULL);

	// Variables
	// var foo;
	addTagRegex (language, "^[ \t]*var[ \t]+([A-Za-z0-9_$]+)[ \t]*[,;]+", "\\1", "v,variable,variables", NULL);
	
	// Variables
	// var foo = blah  .. this will duplicate variables which have functions as values
	addTagRegex (language, "^[ \t]*var[ \t]+([A-Za-z0-9_$]+)[ \t]*\\=[ \t]*", "\\1", "v,variable,variables", NULL);
	
	//multiple variables
	// var foo[=quux],bar;
	addTagRegex (language, "^[ \t]*var[ \t]+([A-Za-z0-9_$]+)[ \t]*([^,]*)\\,[ \t]*([A-Za-z0-9_$]+)", "\\3", "v,variable,variables", NULL);

	// Classes
	// using @class metadata
	addTagRegex (language, "^[ \t]*\\*[ \t]*\\@class[ \t]*([A-Za-z0-9_$]+)", "\\1", "c,class,classes", NULL);

	// Notes
	addTagRegex (language, "\\/\\/[ \t]*(NOTE|note|Note)[ \t]*\\:*(.*)", "\\2", "i,{Notes}", NULL);
	
	// Todos
	addTagRegex (language, "\\/\\/[ \t]*(TODO|todo|ToDo|Todo)[ \t]*\\:*(.*)", "\\2", "i,{To do}", NULL);
	
	// Prototypes (Put this in for AS1 compatibility...)
    addTagRegex (language, ".*\\.prototype\\.([A-Za-z0-9 ]+)[ \t]*\\=([ \t]*)function( [ \t]?)*\\(", "\\1", "p,prototype", NULL);
	
}

/* Create parser definition stucture */


extern parserDefinition* JavaScriptParser (void)

{
    static const char *const extensions [] = { "js", NULL };
    parserDefinition *const def = parserNew ("JavaScript");
    def->extensions = extensions;
    def->initialize = installJavaScriptRegex;
    def->regex      = TRUE;
    return def;
}

