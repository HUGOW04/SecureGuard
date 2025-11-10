#include <ntifs.h>
#include <ntddk.h>

#define DEVICE_NAME L"\\Device\\SecureGuardKernel"
#define SYMLINK_NAME L"\\??\\SecureGuardKernel"

#define AV_DEVICE_TYPE 0x8000
#define PROCESS_TERMINATE (0x0001)
#define IOCTL_BLACKLIST_PROCESS CTL_CODE(AV_DEVICE_TYPE, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WHITELIST_PROCESS CTL_CODE(AV_DEVICE_TYPE, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_KILL_PROCESS CTL_CODE(AV_DEVICE_TYPE, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)

PDEVICE_OBJECT g_DeviceObject = NULL;

// Forward declarations
void DriverUnload(PDRIVER_OBJECT DriverObject);
VOID ProcessNotifyRoutineEx(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo);
VOID ToLowercase(PUNICODE_STRING str);
NTSTATUS KillProcessByPid(ULONG pid);
NTSTATUS DriverCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverIoControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

VOID ToLowercase(PUNICODE_STRING str)
{
    if (str == NULL || str->Buffer == NULL) return;
    for (USHORT i = 0; i < str->Length / sizeof(WCHAR); i++) {
        str->Buffer[i] = towlower(str->Buffer[i]);
    }
}

NTSTATUS KillProcessByPid(ULONG pid)
{
    NTSTATUS status;
    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES objAttr;
    CLIENT_ID clientId;

    InitializeObjectAttributes(&objAttr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
    clientId.UniqueProcess = (HANDLE)(ULONG_PTR)pid;
    clientId.UniqueThread = NULL;

    status = ZwOpenProcess(&hProcess, PROCESS_TERMINATE, &objAttr, &clientId);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = ZwTerminateProcess(hProcess, 0);
    ZwClose(hProcess);
    return status;
}

VOID ProcessNotifyRoutineEx(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo)
{
    UNREFERENCED_PARAMETER(Process);
    UNREFERENCED_PARAMETER(ProcessId);

    if (CreateInfo != NULL && CreateInfo->ImageFileName != NULL && CreateInfo->ImageFileName->Buffer != NULL) {
        UNICODE_STRING exeName = { 0 };
        UNICODE_STRING fullProcessName = *CreateInfo->ImageFileName;
        PWCHAR buffer = fullProcessName.Buffer;
        USHORT lengthChars = fullProcessName.Length / sizeof(WCHAR);

        // Find last backslash
        USHORT pos = lengthChars;
        while (pos > 0 && buffer[pos - 1] != L'\\') {
            pos--;
        }

        PWCHAR nameStart = (pos < lengthChars) ? &buffer[pos] : buffer;
        USHORT nameLenChars = (USHORT)(lengthChars - pos);

        // Initialize exeName with a temporary buffer (we allocate on non-paged pool)
        SIZE_T allocBytes = (nameLenChars + 1) * sizeof(WCHAR);
        PWCHAR tempBuf = (PWCHAR)ExAllocatePoolWithTag(NonPagedPoolNx, allocBytes, 'blkt');
        if (tempBuf == NULL) {
            return;
        }

        RtlZeroMemory(tempBuf, allocBytes);
        RtlCopyMemory(tempBuf, nameStart, nameLenChars * sizeof(WCHAR));
        tempBuf[nameLenChars] = L'\0';

        RtlInitUnicodeString(&exeName, tempBuf);
        ToLowercase(&exeName);

        ExFreePoolWithTag(tempBuf, 'blkt');
    }
}

NTSTATUS DriverCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DriverIoControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
    ULONG controlCode = stack->Parameters.DeviceIoControl.IoControlCode;
    NTSTATUS status = STATUS_SUCCESS;
    ULONG_PTR information = 0;

    switch (controlCode)
    {

    case IOCTL_KILL_PROCESS:
    {
        ULONG inLen = stack->Parameters.DeviceIoControl.InputBufferLength;
        if (inLen < sizeof(ULONG)) {
            status = STATUS_INVALID_PARAMETER;
            break;
        }

        ULONG pid = *(ULONG*)Irp->AssociatedIrp.SystemBuffer;
        status = KillProcessByPid(pid);
        if (!NT_SUCCESS(status)) {
            DbgPrint("SecureGuard: Failed to kill pid %u : 0x%X\n", pid, status);
        }
        else {
            DbgPrint("SecureGuard: Killed pid %u\n", pid);
        }
        information = 0;
        break;
    }

    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
        KdPrint(("SecureGuard: Invalid device request.\n"));
        break;
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = information;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

void DriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);

    UNICODE_STRING symLink = RTL_CONSTANT_STRING(SYMLINK_NAME);

    // Remove process notify routine
    (VOID)PsSetCreateProcessNotifyRoutineEx(ProcessNotifyRoutineEx, TRUE);

    // Delete device and symbolic link
    if (g_DeviceObject) {
        IoDeleteDevice(g_DeviceObject);
        g_DeviceObject = NULL;
    }

    IoDeleteSymbolicLink(&symLink);
    DbgPrint("SecureGuardKernel unloaded\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    NTSTATUS status = STATUS_SUCCESS;

    DriverObject->DriverUnload = DriverUnload;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverIoControl;

    UNICODE_STRING devName = RTL_CONSTANT_STRING(DEVICE_NAME);
    UNICODE_STRING symLink = RTL_CONSTANT_STRING(SYMLINK_NAME);

    status = IoCreateDevice(
        DriverObject,
        0,
        &devName,
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &g_DeviceObject
    );

    if (!NT_SUCCESS(status)) {
        DbgPrint("SecureGuard: IoCreateDevice failed: 0x%X\n", status);
        return status;
    }

    status = IoCreateSymbolicLink(&symLink, &devName);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(g_DeviceObject);
        DbgPrint("SecureGuard: IoCreateSymbolicLink failed: 0x%X\n", status);
        return status;
    }

    // Register process creation notify routine
    status = PsSetCreateProcessNotifyRoutineEx(ProcessNotifyRoutineEx, FALSE);
    if (!NT_SUCCESS(status)) {
        DbgPrint("SecureGuard: PsSetCreateProcessNotifyRoutineEx failed: 0x%X\n", status);
        // Not fatal for driver load — continue but warn.
    }

    DbgPrint("SecureGuardKernel loaded\n");
    return STATUS_SUCCESS;
}
