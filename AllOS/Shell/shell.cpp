#include "../Utils/Typedefs.h"
#include "../Drivers/VGA_Text.h"
#include "../Misc/colors.h"
#include "../Utils/string.h"
#include "shellFunctions.h"
#include "../Memory/mem.h"
#include "../Utils/Conversions.h"
#include "../Drivers/Keyboard.h"
/*

///////////////////////////////////// To add: command buffer, scroll navigation


FLOPPY:
    floppy list     [detect drives]
    

FILE SYSTEM: [...]

POWER MANAGEMENT: 
    shutdown
    reboot

MEMORY:
    usedmem
    
MISC:
    echo #string
    oldcmd
    
VARIABLES:
    let #name = #value
    


(+ load old modes)
*/

int CommandCursor;
extern int CursorPos;
extern int curMode;

extern int curColor;

const char* currentTask;

void load_shell(){

	// ColLine(0, BAR_COLOR);
	// ColLine(24, BAR_COLOR);
	// ClrLine(24);
	// SetCursorPos(0,0);
	// kprint("AllOS shell - [");
    // kprint("task: ");
    // kprint(currentTask);
    // kprint(" | ");
    // kprint("dir: -");
    // kprint("]\n");
    // kprint("Work in progress, ");
    // kprintCol("press F1 to use the old CMD mode.\n\n", ERROR_COLOR);
	// CommandCursor = CursorPos;
	// SetCursorPosRaw(1920);
    int backgroundCol = 0x0;
    int defCol = 0xF;
    curColor = backgroundCol;
    // curColor = e;
    currentTask = "init";
	curMode = 10;
    kprintCol("AllOS shell - ", defCol, false);
    kprintCol("[", 0x5, false);
    kprintCol("task: ", defCol, false);
    kprintCol(currentTask, 0xA, false);
    kprintCol("]\n", 0x5, false);
    kprintCol(">", defCol, false); 
    CommandCursor = CursorPos;
    // kprintCol(toString(CommandCursor, 10), defCol, false);
    // kprintChar('\n', false);
    // SetCursorPosRaw(CommandCursor);
	return;
    
}

void refreshShell(){
    int bakcgroundCol = 0x0;
    int defCol = 0xF;
    ClrLine(0);
    ClrLine(1);
	SetCursorPos(0,0);
    kprintCol("AllOS shell - ", defCol, false);
    kprintCol("[", 0x5, false);
    kprintCol("task: ", defCol, false);
    kprintCol(currentTask, 0xA, false);
    kprintCol("]\n", 0x5, false);
    kprintCol(">", defCol, false); 
    CommandCursor = CursorPos; 
	return;
}


char CommandBuffer[128];
void findCommand(int t_CursorPos){
    int commandLength = t_CursorPos - 81;
    uint16_t* commandStart = 0xb8000 + (81 * 2);
    // kprintChar('\n', false);
    // kprintCol(toString(commandLength, 10), 13, false);
    // kprintChar('\n', false);
    // kprintCol(toString(commandStart, 10), 13, false);
    // kprintChar('\n', false);
    for(uint16_t i = 0; i <= commandLength; i++){
        CommandBuffer[i] = *(commandStart + i);
        // kprintCharCol(*(commandStart + i * 2), 0xF, false);
    }
}


// bool CheckCMD(void(*f)(const char*), const char* command){
//     bool res = false;
//     memrec();
//     if(StringStartsWith(CommandBuffer, command)) {
//         f(strDecapitate(CommandBuffer, strLen(command)));
//         res = true;
//     }
//     memunrec();
//     return res;
// }

char cmds[256][128] = {
    "help\0",
    "clear\0",
    "echo\0",
    "floppy\0",
    "usedmem\0"
};

typedef void(*cmdsFuncPoint)(const char*);
cmdsFuncPoint cmdsFunc[256] = {
    helpCMD
};




void CheckCMD(){
    int commandLength = 1;
    int possibleCmd_nr = 0;
    int a = 0;
    while(bool keeprunning = true){
        if(CommandBuffer[a] != ' '){
            keeprunning = false;
        }else{
            commandLength++;
        }
        a++;
    }
    char command[commandLength];
    for(int i = 0; i < commandLength; i++){
        command[i] = CommandBuffer[i];
    }
    for(int i = 0; i < 256; i++){
        if(StringsEqu(&command[0], &cmds[i][0])){
            possibleCmd_nr = i;
        }
    }
    // typedef void myFunc = ((void (*)(const char*))cmdsFunc[possibleCmd_nr]);
    // (cmdsFunc[possibleCmd_nr])((const char*)&CommandBuffer[commandLength]);
    kprintCharCol(toString(possibleCmd_nr, 10), 0xF, false);
    (*cmdsFunc[possibleCmd_nr])(&CommandBuffer[commandLength]);

}

void parseCommand(){
    findCommand(CursorPos);
    
    // if(CheckCMD(helpCMD, "help")){;}
    // else if (CheckCMD(kprint, "echo ")){currentTask = "echo";}
    // else if (CheckCMD(printUsedMem, "usedmem")){;}
    // else if (CheckCMD(floppyCMD, "floppy ")){;}
    // else if (CheckCMD(floppyCMD, "floppy")){;}      // get rid of redundancy asap
    // else if (CheckCMD(clearCMD, "clear")){;}
    // else {kprintChar('\n', false); printError(CommandBuffer); printError(" is not a command");}
    kprintChar('\n', false);
    CheckCMD();
    kprint("its about uwu");
    CommandCursor = CursorPos;
    // SetCursorPosRaw(1920);
    refreshShell();
}
