//
// Created by Reveny on 1/6/2025.
//
#pragma once

#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cinttypes>
#include <sys/uio.h>

#if defined(__arm__)
#define process_vm_readv_syscall 376
#define process_vm_writev_syscall 377
#elif defined(__aarch64__)
#define process_vm_readv_syscall 270
#define process_vm_writev_syscall 271
#elif defined(__i386__)
#define process_vm_readv_syscall 347
#define process_vm_writev_syscall 348
#else
#define process_vm_readv_syscall 310
#define process_vm_writev_syscall 311
#endif

namespace Utility
{
    size_t ProcessVMRead(pid_t pid, void *local, void *remote, size_t size);
    size_t ProcessVMWrite(pid_t pid, void *local, void *remote, size_t size);
    bool PVM(void *address, void *buffer, size_t size, bool write = false);

    uintptr_t GetBaseAddress(const char *name);
    uintptr_t GetAbsoluteAddress(const char *libraryName, uintptr_t relativeAddr);

    bool IsLibraryLoaded(const char *libraryName);
    uintptr_t String2Offset(const char *c);
}