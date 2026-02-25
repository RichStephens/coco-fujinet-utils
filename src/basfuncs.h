#ifndef BASFUNCS_H
#define BASFUNCS_H

#include <cmoc.h>

/* 
 * Advances CHARAD past spaces and returns the next character.
 */
char skipBlanksInStatement(void);

/*
 * Advances CHARAD by one character.
 */
void skipChar(void);

/*
 * Evaluates the BASIC expression at CHARAD.
 * Returns TRUE if string, FALSE if numeric.
 */
BOOL evaluateBasicExpression(void);

/*
 * Retrieves the evaluated string (after evaluateBasicExpression()).
 */
void getTopString(char **stringStart, byte *stringLength);

/*
 * Converts FPA0 to signed 16-bit integer.
 * If roundToNearestInteger is TRUE, adds 0.5 before conversion.
 */
int convertFPA0ToSignedInt(BOOL roundToNearestInteger);

/*
 * Gets a variable reference from BASIC source.
 * Returns TRUE if string variable, FALSE if numeric.
 */
BOOL getVariableReference(void **varptr);

/*
 * Assigns a C string to a BASIC string variable.
 */
void assignStringToVariable(void *varptr,
                            const char *str,
                            byte strLength);

/*
 * Assigns a signed integer to a BASIC numeric variable.
 * Trashes FPA0.
 */
void assignSignedIntegerToVariable(void *varptr, int value);

/*
 * Replaces a Color BASIC command with a new name and implementation.
 */
BOOL replaceColorBasicCommand(const char *oldCommandName,
                              const char *newCommandName,
                              void (*newCommand)(void));

#endif