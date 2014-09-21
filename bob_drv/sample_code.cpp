/**----------------------------------------------------------------------------
 * sample_code.cpp
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh,Yonghwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 2014:8:22 16:32 created
**---------------------------------------------------------------------------*/
#include "DriverHeaders.h"
#include "sample_code.h"


void queue_sample();
void stack_sample();


void  plookaside_initialize(__out PPAGED_LOOKASIDE_LIST lookaside, __in ULONG buffer_size, __in ULONG pool_tag);
void* plookaside_allocate(__in const PPAGED_LOOKASIDE_LIST lookaside);
void  plookaside_free(__in const PPAGED_LOOKASIDE_LIST lookaside, __in void* p);
void  plookaside_finalize(__in PPAGED_LOOKASIDE_LIST lookaside);


/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void sample_linked_list()
{
	queue_sample();
	stack_sample();

	
}


/**
 * @brief	pool 할당/해제

			 kd> !process 0 0 winlogon.exe
			 PROCESS 85fa2020  SessionId: 0  Cid: 0284    Peb: 7ffd9000  ParentCid: 017c
			 DirBase: 0d000060  ObjectTable: e101a0d0  HandleCount: 468.
			 Image: winlogon.exe

			 kd> !pool 85fa2020  
			 Pool page 85fa2020 region is Nonpaged pool
			 *85fa2000 size:  280 previous size:    0  (Allocated) *Proc (Protected)
			 Pooltag Proc : Process objects, Binary : nt!ps

**/
void sample_kernel_heap()
{
	PVOID p = ExAllocatePoolWithTag(PagedPool, 128, ' GAT');
	if (NULL == p) return;
	
	log_info "pool allocated = 0x%p", p log_end	
	__asm int 3

	ExFreePoolWithTag(p, ' GAT');
}


/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void sample_lookaside_list()
{
	PAGED_LOOKASIDE_LIST paged_llist = {0};
	plookaside_initialize(&paged_llist, 128, ' LSL');

	for (int i  = 0; i < 100; ++i)
	{
		char* p = (char*)plookaside_allocate(&paged_llist);
		if (NULL != p)
		{
			if (!NT_SUCCESS(RtlStringCbPrintfA(p, 128, "%s_%d", __FUNCTION__, i)))
			{
				// ...
				log_err "oops! " log_end
			}

			// TODO - add p to list...
	
			log_info "%s", p log_end
			plookaside_free(&paged_llist, p);
		}
	}

	// TODO - print & free & delete 
	



	// TODO - nonpaged lookaside list
	plookaside_finalize(&paged_llist);
}

/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void sample_file_write()
{
	HANDLE file_handle = NULL;
	OBJECT_ATTRIBUTES oa;
	IO_STATUS_BLOCK iostatus;
	UNICODE_STRING file_path;


	// http://msdn.microsoft.com/en-us/library/windows/hardware/ff565384(v=vs.85).aspx

	RtlInitUnicodeString(&file_path, L"\\??\\c:\\dbg\\kfile_io.test");
	// or
	//RtlInitUnicodeString(&file_path, L"\\DosDevices\\c:\\dbg\\kfile_io.test");

	InitializeObjectAttributes(
		&oa,
		&file_path, 
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 
		NULL, 
		NULL
		);
	
	
	NTSTATUS status = ZwCreateFile(
						&file_handle,
						FILE_APPEND_DATA,
						&oa,
						&iostatus,
						NULL,
						FILE_ATTRIBUTE_NORMAL,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						FILE_OPEN_IF,
						FILE_SYNCHRONOUS_IO_NONALERT,
						NULL,
						0
						);
	if (!NT_SUCCESS(status) )
	{
		log_err "ZwCreateFile() failed. status = 0x%08x", status log_end
		return;
	}

	do 
	{
		wchar_t log_buf[128];
		for (int i = 0; i < 24; ++i)
		{
			status = RtlStringCbPrintfW(log_buf, sizeof(log_buf), L"%s_%d \n", __FUNCTION__, i);
			if (!NT_SUCCESS(status))
			{
				log_err "oops! never reach here!" log_end
				break;
			}


			status = ZwWriteFile(
						file_handle, 
						NULL,
						NULL,
						NULL,
						&iostatus,
						log_buf, 
						sizeof(log_buf),
						NULL,
						NULL
						);
			if (!NT_SUCCESS(status))
			{
				log_err "ZwWriteFile() failed. status = 0x%08x", status log_end
				break;
			}
		}	


	} while(false);


	ZwClose(file_handle);
}


/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void sample_file_read()
{
	// TODO
}


/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void sample_reg_io()
{
	// TODO
}

/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void sample_thread()
{
	// TODO
}

/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void sample_workitem()
{
	// TODO
}


/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
			 kd> dl nt!PsActiveProcessHead
			 8055c158  863b78b8 8607e2b0 00000001 ee836a6c
			 863b78b8  860631b0 8055c158 00000000 00000000
			 860631b0  85eab640 863b78b8 00000280 000017dc
			 85eab640  85e24e28 860631b0 00001a20 000142bc
			 85e24e28  85eab3c0 85eab640 0000a1a8 000199fc
			 85eab3c0  8616b3e0 85e24e28 00001998 00008d94
			 8616b3e0  86083718 85eab3c0 000020d8 00012634
			 86083718  861eedc0 8616b3e0 000007d0 00008a84
			 861eedc0  8610b968 86083718 000019e0 00013b8c
			 8610b968  862ee5e0 861eedc0 000034d8 000131bc
			 862ee5e0  85e61bb0 8610b968 00006f88 00027cd4
			 85e61bb0  85e60a10 862ee5e0 00000ce8 0000e08c
			 85e60a10  860080a8 85e61bb0 00001a90 00012d04
			 860080a8  860dc840 85e60a10 00001880 000161ac
			 860dc840  86065778 860080a8 00002508 0001dadc
			 86065778  86195458 860dc840 00000d20 00010b64
			 86195458  85e54e28 86065778 00001518 00019ebc
			 85e54e28  86111a08 86195458 00000e88 0000f5a4
			 86111a08  8610e798 85e54e28 00000be0 0000ec2c
			 8610e798  85fe98a8 86111a08 000022d8 00021be4
			 85fe98a8  86096938 8610e798 00000f78 000125bc

			 ntdll!_EPROCESS
			 +0x000 Pcb              : _KPROCESS
			 +0x06c ProcessLock      : _EX_PUSH_LOCK
			 +0x070 CreateTime       : _LARGE_INTEGER 0x00040001`00000000
			 +0x078 ExitTime         : _LARGE_INTEGER 0x863b7934`00000000
			 +0x080 RundownProtect   : _EX_RUNDOWN_REF
			 +0x084 UniqueProcessId  : (null) 
			 +0x088 ActiveProcessLinks : _LIST_ENTRY [ 0x0 - 0x0 ]

			 kd> dt _eprocess (0x863b78b8 - 0x88)
			 ntdll!_EPROCESS
			 +0x000 Pcb              : _KPROCESS
				...
			 +0x084 UniqueProcessId  : 0x00000004 Void
			 +0x088 ActiveProcessLinks : _LIST_ENTRY [ 0x860631b0 - 0x8055c158 ]
				...
			 +0x174 ImageFileName    : [16]  "System"
				...
			 +0x190 ThreadListHead   : _LIST_ENTRY [ 0x863b77e4 - 0x85fe0f6c ]



 * @code		
 * @endcode	
 * @return	
**/
void enum_process_threads()
{
	//DWORD_PTR active_list_head = 
}

/**
 * @brief	implement queue by double linked list
 * @param	
 * @see		
 * @remarks	http://www.osronline.com/article.cfm?article=499
 * @code		
 * @endcode	
 * @return	
**/
typedef struct list_item_rec
{
	LIST_ENTRY link;

	UINT32	v1;
	UINT32	v2;

}*plist_item_rec;

#define LIST_ITEM_POOL_TAG	' TSL'

plist_item_rec alloc_list_item(__in UINT32 v1, __in UINT32 v2)
{
	plist_item_rec p = (plist_item_rec) ExAllocatePoolWithTag(PagedPool, sizeof(list_item_rec), LIST_ITEM_POOL_TAG);
	if (NULL == p) return NULL;

	p->v1 = v1;
	p->v2 = v2;
	return p;
}

void free_list_item(__in plist_item_rec& p)
{
	ASSERTMSG("input param is null", NULL != p);
	ExFreePoolWithTag(p, LIST_ITEM_POOL_TAG);
	p = NULL;
}


void queue_sample()
{
	LIST_ENTRY  list_head = {0};
	InitializeListHead(&list_head);

	// add
	//
	for (int i = 0; i < 8; i ++)
	{
		plist_item_rec li = alloc_list_item(i, i+1);
		if (NULL == li)
		{
			log_err "alloc_list_item() failed, insufficient resource?!" log_end
			return;
		}

		InsertTailList(&list_head, &li->link);
	}

	UINT32			idx = 0;
	PLIST_ENTRY		le = NULL;
	plist_item_rec	li = NULL;

	// iterate by insert order
	if (TRUE != IsListEmpty(&list_head))
	{
		le = list_head.Flink;

		do
		{
			li = CONTAINING_RECORD(le, list_item_rec, link);			
			log_info "[%u] v1 = %u, v2 = %u", idx++, li->v1, li->v2 log_end

			le = le->Flink;
		} while (le != &list_head);
	}

	// free
	while (TRUE != IsListEmpty(&list_head))
	{
		le = RemoveHeadList(&list_head);
		li = CONTAINING_RECORD(le, list_item_rec, link);		
		free_list_item(li);
		ASSERT(NULL == li);
	}

	ASSERTMSG("list is not empty", TRUE == IsListEmpty(&list_head));
}



/**
 * @brief	implement stack by double linked list
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void stack_sample()
{
	// TODO - implement this

}







/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void plookaside_initialize(__out PPAGED_LOOKASIDE_LIST lookaside, __in ULONG buffer_size, __in ULONG pool_tag)
{
	ASSERTMSG("null", NULL != lookaside);
	if (NULL == lookaside) return;

	ExInitializePagedLookasideList(lookaside, NULL, NULL, 0, buffer_size, pool_tag, 0);
}

void* plookaside_allocate(__in const PPAGED_LOOKASIDE_LIST lookaside)
{
	void* p = ExAllocateFromPagedLookasideList(lookaside);
	if (NULL == p) return NULL;
	
	return p;
}

void plookaside_free(__in const PPAGED_LOOKASIDE_LIST lookaside, __in void* p)
{
	if (NULL != p)
	{
		ExFreeToPagedLookasideList(lookaside, p);
	}
}

void  plookaside_finalize(__in PPAGED_LOOKASIDE_LIST lookaside)
{
	if (NULL != lookaside)
	{
		ExDeletePagedLookasideList(lookaside);		
	}
}