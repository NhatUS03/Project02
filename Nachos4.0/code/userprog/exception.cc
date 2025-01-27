// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which) {
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
		case NoException:
			return;
		case PageFaultException:
			DEBUG(dbgAddr, "Page Fault Exception, No valid translation found\n");
			SysHalt();
			break;
		case ReadOnlyException:
			DEBUG(dbgAddr, "Read Only Exception, Write attempted to page marked \"read-only\"\n");
			SysHalt();
			break;
		case BusErrorException:
			DEBUG(dbgAddr, "Bus Error Exception, Translation resulted in an invalid physical address\n");
			SysHalt();
			break;
		case AddressErrorException:
			DEBUG(dbgAddr, "Address Error Exception, Unaligned reference or one that was beyond the end of the address space\n");
			SysHalt();
			break;
		case OverflowException:
			DEBUG(dbgAddr, "Overflow Exception, Integer overflow in add or sub\n");
			SysHalt();
			break;
		case IllegalInstrException:
			DEBUG(dbgAddr, "Illegal Instruction Exception, Unimplemented or reserved instr\n");
			SysHalt();
			break;
		case NumExceptionTypes:
			DEBUG(dbgAddr, "Num Exception Types\n");
			SysHalt();
			break;
			
        case SyscallException:
            switch (type) {
                case SC_Halt:
                    DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

                    SysHalt();

                    ASSERTNOTREACHED();
                    break;

                case SC_Create: {
                    int result;

                    DEBUG(dbgSys, "\n SC_Create call ...");
                    result = SysCreate((int) kernel->machine->ReadRegister(4));

                    DEBUG(dbgSys, "SysCreate returning with " << result << "\n");
                    kernel->machine->WriteRegister(2, (int) result);

                    IncreasePC();

                    return;
                    ASSERTNOTREACHED();
                    break;
                }

                case SC_Open: {
                    int result;

                    DEBUG(dbgSys, "\n SC_Open call ...");
                    result = SysOpen((int) kernel->machine->ReadRegister(4), (int) kernel->machine->ReadRegister(5));

                    DEBUG(dbgSys, "SysOpen returning with " << result << "\n");
                    kernel->machine->WriteRegister(2, (int) result);

                    IncreasePC();

                    return;
                    ASSERTNOTREACHED();
                    break;
                }
                case SC_Close: {
                    int result;

                    DEBUG(dbgSys, "\n SC_Close call ...");
                    result = SysClose((int) kernel->machine->ReadRegister(4));

			DEBUG(dbgSys, "SysClose returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_Read:
		{
			int result;

			DEBUG(dbgSys, "\n SC_Read call ...");
			result = SysRead((int)kernel->machine->ReadRegister(4), (int)kernel->machine->ReadRegister(5), (int)kernel->machine->ReadRegister(6));

			DEBUG(dbgSys, "SysRead returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_Write:
		{
			int result;

			DEBUG(dbgSys, "\n SC_Write call ...");
			result = SysWrite((int)kernel->machine->ReadRegister(4), (int)kernel->machine->ReadRegister(5), (int)kernel->machine->ReadRegister(6));

			DEBUG(dbgSys, "SysWrite returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_Remove:
		{
			int result;

			DEBUG(dbgSys, "\n SC_Remove call ...");
			result = SysRemove((int)kernel->machine->ReadRegister(4));

			DEBUG(dbgSys, "SysRemove returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_SocketTCP:
		{
			int result;

			DEBUG(dbgSys, "\n SC_SocketTCP call ...");
			result = SysSocketTCP();

			DEBUG(dbgSys, "SysSocketTCP returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_Connect:
		{
			int result;

			DEBUG(dbgSys, "\n SC_Connect call ...");
			result = SysConnect((int)kernel->machine->ReadRegister(4), (int)kernel->machine->ReadRegister(5),
								(int)kernel->machine->ReadRegister(6));

			DEBUG(dbgSys, "SysConnect returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();

			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_Seek:
		{
			int result;
			DEBUG(dbgSys, "\n SC_Seek call ...");
			result = SysSeek((int)kernel->machine->ReadRegister(4), (int)kernel->machine->ReadRegister(5));
			DEBUG(dbgSys, "SysSeek returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_Send:
		{
			int result;
			DEBUG(dbgSys, "\n SC_Send call ...");
			result = SysSend((int)kernel->machine->ReadRegister(4), (int)kernel->machine->ReadRegister(5),
							 (int)kernel->machine->ReadRegister(6));
			DEBUG(dbgSys, "SysSend returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_Receive:
		{
			int result;
			DEBUG(dbgSys, "\n SC_Receive call ...");
			result = SysReceive((int)kernel->machine->ReadRegister(4), (int)kernel->machine->ReadRegister(5),
								(int)kernel->machine->ReadRegister(6));
			DEBUG(dbgSys, "SysReceive returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);
			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		case SC_CloseSocket:
		{
			int result;
			DEBUG(dbgSys, "\n SC_CloseSocket call ...");
			result = SysCloseSocket((int)kernel->machine->ReadRegister(4));
			DEBUG(dbgSys, "SysCloseSocket returning with " << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);
			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}
		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
}
