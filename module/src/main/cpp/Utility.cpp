//
// Created by Reveny on 1/6/2025.
//
#include <Headers/Utility.hpp>

#include <KittyMemory.h>

size_t Utility::ProcessVMRead(pid_t pid, void *local, void *remote, size_t size) {
    struct iovec local_iov = {local, size};
    struct iovec remote_iov = {remote, size};

    return process_vm_readv(pid, &local_iov, 1, &remote_iov, 1, 0);
}

size_t Utility::ProcessVMWrite(pid_t pid, void *local, void *remote, size_t size) {
    struct iovec local_iov = {local, size};
    struct iovec remote_iov = {remote, size};

    return process_vm_writev(pid, &local_iov, 1, &remote_iov, 1, 0);
}

bool Utility::PVM(void *address, void *buffer, size_t size, bool write) {
    if (write) {
        return ProcessVMWrite(getpid(), buffer, address, size) == size;
    } else {
        return ProcessVMRead(getpid(), buffer, address, size) == size;
    }
}

uintptr_t Utility::GetBaseAddress(const char *libraryName) {
    KittyMemory::ProcMap maps = KittyMemory::getLibraryMap(libraryName);
    return reinterpret_cast<uintptr_t>(maps.startAddr);
}

uintptr_t Utility::GetAbsoluteAddress(const char *libraryName, uintptr_t relativeAddr) {
    uintptr_t baseAddr = GetBaseAddress(libraryName);
    return baseAddr + relativeAddr;
}

bool Utility::IsLibraryLoaded(const char *libraryName) {
    return GetBaseAddress(libraryName) != 0;
}

uintptr_t Utility::String2Offset(const char *c) {
    if (c[0] == '0' && c[1] == 'x') {
        return strtoul(c, nullptr, 16);
    } else {
        return strtoul(c, nullptr, 10);
    }
}