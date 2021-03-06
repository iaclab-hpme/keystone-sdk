//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "Memory.hpp"

namespace Keystone {

void
PhysicalEnclaveMemory::init(
    KeystoneDevice* dev, uintptr_t phys_addr, size_t min_pages, uintptr_t utm_phys_addr) {
  pDevice = dev;
  // TODO(dayeol): need to set actual EPM size
  epmSize       = PAGE_SIZE * min_pages;
  rootPageTable = allocMem(PAGE_SIZE);
  epmFreeList   = phys_addr + PAGE_SIZE;
  startAddr     = phys_addr;
  utmStartAddr  = utm_phys_addr;
}

uintptr_t
PhysicalEnclaveMemory::allocUtm(size_t size) {
  uintptr_t ret = pDevice->initUTM(size);
  utmFreeList   = ret;
  untrustedSize = size;
  utmPhysAddr   = ret;
  return ret;
}

uintptr_t
PhysicalEnclaveMemory::allocMem(size_t size) {
  uintptr_t ret;

  assert(pDevice);

  ret = reinterpret_cast<uintptr_t>(pDevice->map(0, PAGE_SIZE));
  return ret;
}

uintptr_t
PhysicalEnclaveMemory::readMem(uintptr_t src, size_t size) {
  uintptr_t ret;

  assert(pDevice);

  ret = reinterpret_cast<uintptr_t>(pDevice->map(src - startAddr, size));
  return ret;
}

void
PhysicalEnclaveMemory::writeMem(uintptr_t src, uintptr_t dst, size_t size, bool isUtm) {
  assert(pDevice);
  uintptr_t startAddr = isUtm ? utmStartAddr : this->startAddr;
  void* va_dst = pDevice->map(dst - startAddr, size);
  // printf("[DEBUG] dst=0x%lx, startAddr=0x%lx, addr = 0x%lx, va_dst=0x%lx\n",dst, startAddr, dst-startAddr, va_dst);
  memcpy(va_dst, reinterpret_cast<void*>(src), size);
}

}  // namespace Keystone
