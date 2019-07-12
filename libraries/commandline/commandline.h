#ifndef Commandline_h
#define Commandline_h

#include <Arduino.h>

/// commandline buffer size
#define CMDLINE_MAX_CMD_LENGTH		30

class Commandline
{
  public:
	// DATE TYPES
	typedef void (*CmdlineFuncPtrType) ( void);
	typedef void (*tConsole_sendString) ( char *BufferPtr);

	typedef struct tag_tCmdlineMenu
	{
	  char Command[CMDLINE_MAX_CMD_LENGTH];
	  CmdlineFuncPtrType Function;
	} tCmdlineMenu;

	// PUBLIC Variables
	tCmdlineMenu *CmdlineMenuItem;
	unsigned char CmdlineMenuAnzahl;
	char CmdlineMenuPrompt[20];

	// CONSTRUCTORS
	Commandline( tConsole_sendString ConsoleSendString);

    // PUBLIC METHODS
	void cmdlineDecodeInput (char c);
	void cmdlinePrintPrompt (void);
	void cmdlineRepaint (void);
	void Console_sendByte( char cByte);

	char *cmdlineGetArgStr (unsigned char argnum);
	long cmdlineGetArgInt (unsigned char argnum);
	long cmdlineGetArgHex (unsigned char argnum);
	double cmdlineGetArgDouble (unsigned char argnum);
	tConsole_sendString Console_sendString;

	// functions
	void vt100Init (void);
	void vt100ClearScreen (void);
	void vt100EraseToLineEnd (void);
	void vt100SetAttr (unsigned char attr);
	void vt100SetCursorMode (unsigned char visible);
	void vt100SetCursorPos (unsigned char line, unsigned char col);
	void vt100MoveCursorRight (void);
	void vt100MoveCursorLeft (void);

  private:
	// PRIVATE TYPEDEV
	// History commands
	typedef enum Tag_tCMDLINE_HISTORY
	{
	  CMDLINE_HISTORY_SAVE,
	  CMDLINE_HISTORYREV,
	  CMDLINE_HISTORY_NEXT
	} tCMDLINE_HISTORY;

	// PRIVATE Functions
	void cmdlinePrintError (void);
	void cmdlineNormalChar (char c);
	void cmdlineBS_deleteBack (void);
	void cmdlineDEL_deleteForword (void);
	void cmdlineDoHistory (tCMDLINE_HISTORY action);
	void cmdlineCR_Enter (void);
	void cmdlineTABcompletion (void);
	void cmdlineCursorLeft (void);
	void cmdlineCursorRight (void);
	void cmdlineCursorStart (void);
	void cmdlineCursorEnde (void);
};

#endif
