#include <coco.h>

#include "basfuncs.h"

// Color Basic interpretation table as found at $0120.
//
typedef struct CmdInterpTable
{
    byte numReservedWords;
    char *reservedWordsLookupTable;
    void **cmdJumpTable;  // array of routine addresses
    byte numSecFuncs;
    char *secFuncLookupTable;
    void **secFuncJumpTable;  // array of routine addresses
} CmdInterpTable;


// New Color Basic tables.
// If the program is loaded at $2800, these tables will still be intact
// at around $3xxx after main() quits.
//
static char newReservedWordsLookupTable[0xAB1A - 0xAA66];
static void *newCmdJumpTable[(0xABAF - 0xAB67) / 2];


// Advances the interpreter's read pointer (CHARAD) past any blanks,
// then returns the character at CHARAD.
//
asm char
skipBlanksInStatement(void)
{
    asm
    {
        ldx     <$A6        ; CHARAD
@loop
        ldb     ,x+
        cmpb    #' '
        beq     @loop
        ; char to return left in B
@endOfCmd
        leax    -1,x
        stx     <$A6        ; update read pointer
    }
}


// Advance the interpreter's read pointer (CHARAD) by one.
//
asm void
skipChar(void)
{
    asm
    {
        inc     <$A7        ; low byte of CHARAD
        bne     @done
        inc     <$A6        ; high byte of CHARAD
@done
    }
}


// Returns TRUE if the expression is a string, FALSE if it is a number.
// If it is a string, call getTopString().
// If it is a number, use FPA0 (at $004F), e.g., by calling convertFPA0ToSignedInt().
//
asm BOOL
evaluateBasicExpression(void)
{
    asm
    {
        pshs    u,y         ; protect CMOC registers against Basic
        jsr     $B156       ; evaluate expression (numerical or string)
        puls    y,u         ; this restores U, which allows refs to C local variables
        ldb     <$06        ; get expression type (VALTYP)
        andb    #1
    }
}


void
getTopString(char **stringStart, byte *stringLength)
{
    asm
    {
        pshs    u,y
        jsr     $B654       ; point X to start of evaluated string, get length in B
        puls    y,u
        stx     [:stringStart]
        stb     [:stringLength]
    }
}


// Converts floating point accumulator #0 into a signed 16-bit value.
//
asm int
convertFPA0ToSignedInt(BOOL roundToNearestInteger)
{
    asm
    {
        pshs    u,y
        tst     7,s         ; roundToNearestInteger
        beq     @noRounding
        jsr     $B9B4       ; add .5 to FPA0
@noRounding
        jsr     $BCC8       ; denormalize FPA0 mantissa
        ldd     <$52        ; read low 16 bits of mantissa; return it in D
        puls    y,u
    }
}


asm BOOL
getVariableReference(void **varptr)
{
    asm
    {
        pshs    u,y
        jsr     $B357           ; get var desc address in X (creates var if inexistent); advances CHARAD
        puls    y,u
        stx     [2,s]           ; X points after the 2-byte variable name; store X in *varptr
        ldb     <$06            ; VALTYP: 0 = numerical, $FF = string
        andb    #1              ; convert to 0 (FALSE) or 1 (TRUE) to better accommodate C; return B
    }
}


void
assignStringToVariable(void *varptr, const char *str, byte strLength)
{
    asm
    {
        ldx     :str
        ldb     :strLength
        pshs    u,y
        stx     <$62        ; RESSGN: start of source string
        stx     <$58        ; STRDES+2: in temp descriptor
        abx                 ; computer end of source string
        stx     <$64        ; COEFPT gets end of source string
        stb     <$56        ; STRDES: length in temp descriptor
        jsr     $B545       ; go reserve space in string space, copy source string there, etc.
        puls    y,u
        // Address of string value descriptor is now in FPA0+2, i.e., $52.
        // This is required by the call to $AFA4.

        ldx     <$50
        ldx     <$52

        pshs    b,a
        ldd     ,x
        ldd     2,x
        ldb     4,x
        puls    a,b

        // Move the string whose descriptor's address is at $0052 into the string space
        // and assign that string to the variable whose descriptor address is in VARDES.
        // (This is part of the LET command, so we are simulating LET X$="STUFF".)
        ldx     :varptr     ; address of descriptor of string variable that gets assigned
        stx     <$3B        ; VARDES
        pshs    u,y
        jsr     $AFA4       ; end of the LET command
        puls    y,u
    }
}


// Trashes FPA0.
//
asm __norts__ void
assignSignedIntegerToVariable(void *varptr, int value)
{
    asm
    {
        ldd     4,s         ; value
        ldx     2,s         ; varptr: packed floating point where to store value as a float
        pshs    u,y,x
        jsr     $B4F4       ; GIVABF: convert signed integer in D to FPA0
        puls    x           ; varptr
        jsr     $BC35       ; pack FPA0 and save it in address pointed to by X
        puls    y,u,pc
    }
}


BOOL
isHighBitKeywordEqualToString(const char *keywordBegin, const char *lastKeywordChar, const char *str)
{
    for ( ; keywordBegin != lastKeywordChar; ++keywordBegin, ++str)
        if (*keywordBegin != *str)
            return FALSE;
    return (*lastKeywordChar & 0x7F) == *str;  // compare last chars
}


// Return the index of the found command in the given table, or 255 if not found.
//
byte
replaceReservedWordInLookupTable(char *table, size_t tableLength,
                                 const char *oldCommandName, const char *newCommandName)
{
    byte cmdIndex = 0;
    for (char *wordBegin = table; wordBegin < table + tableLength; ++cmdIndex)
    {
        char *wordEnd = wordBegin;
        while ((*wordEnd & 0x80) == 0)
            ++wordEnd;
        if (isHighBitKeywordEqualToString(wordBegin, wordEnd, oldCommandName))  // if found 'commandName'
        {
            memcpy(wordBegin, newCommandName, wordEnd + 1 - wordBegin);
            *wordEnd |= 0x80;  // set bit 7 of last char of new command name
            return cmdIndex;  // success
        }
        wordBegin = wordEnd + 1;
    }
    return 255; // oldCommandName not found
}


BOOL
replaceColorBasicCommand(const char *oldCommandName, const char *newCommandName, void (*newCommand)(void))
{
    if (strlen(oldCommandName) != strlen(newCommandName))
        return FALSE;

    // Get the Color Basic command interpretation table.
    //
    CmdInterpTable *cbTable = (CmdInterpTable *) 0x0120;
    
    // Copy the Color Basic token table to RAM, then point CB to it.
    // This is a string of characters that contains a concatenation of several Basic keywords:
    // FOR, GO, REM, etc, with bit 7 of the last character of each keyword set to 1.
    //
    memcpy(newReservedWordsLookupTable,
           cbTable->reservedWordsLookupTable,
           sizeof(newReservedWordsLookupTable));
    cbTable->reservedWordsLookupTable = newReservedWordsLookupTable;

    // Change the name of the command.
    //
    byte cmdIndex = replaceReservedWordInLookupTable(
                        cbTable->reservedWordsLookupTable, sizeof(newReservedWordsLookupTable),
                        oldCommandName, newCommandName);
    if (cmdIndex == 255)  // not found
        return FALSE;

    // Copy the Color Basic jump table to RAM, then point CB to it, then change
    // the entry of the old command to point to the C function that implements
    // the new command.
    //
    memcpy(newCmdJumpTable, cbTable->cmdJumpTable, sizeof(newCmdJumpTable));
    cbTable->cmdJumpTable = newCmdJumpTable;

    cbTable->cmdJumpTable[cmdIndex] = (void *) newCommand;

    return TRUE;
}
