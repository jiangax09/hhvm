/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef incl_HPHP_VM_REG_ALLOC_H_
#define incl_HPHP_VM_REG_ALLOC_H_

#include "hphp/runtime/vm/jit/state-vector.h"
#include "hphp/runtime/vm/jit/ir.h"

namespace HPHP {  namespace JIT {

class IRUnit;

// This value must be consistent with the number of pre-allocated
// bytes for spill locations in __enterTCHelper in translator-x64.cpp.
// Be careful when changing this value.
const size_t NumPreAllocatedSpillLocs = kReservedRSPSpillSpace /
                                        sizeof(uint64_t);

struct RegAllocInfo {
  struct RegMap {
    PhysLoc& operator[](const SSATmp* k) { return m_map[k->id()]; }
    PhysLoc& operator[](const SSATmp& k) { return m_map[k.id()]; }
    const PhysLoc& operator[](const SSATmp* k) const {
      return m_map[k->id()];
    }
    const PhysLoc& operator[](const SSATmp& k) const {
      return m_map[k.id()];
    }
  private:
    mutable smart::flat_map<uint32_t,PhysLoc> m_map;
  };
  explicit RegAllocInfo(const IRUnit& unit) : m_regs(unit, RegMap()) {}
  RegAllocInfo(const RegAllocInfo& other) : m_regs(other.m_regs) {}
  RegAllocInfo(RegAllocInfo&& other) : m_regs(other.m_regs) {}
  RegMap& operator[](const IRInstruction* i) { return m_regs[i]; }
  RegMap& operator[](const IRInstruction& i) { return m_regs[i]; }
  const RegMap& operator[](const IRInstruction* i) const { return m_regs[i]; }
  const RegMap& operator[](const IRInstruction& i) const { return m_regs[i]; }

  RegSet srcRegs(IRInstruction& inst) const {
    auto regs = RegSet();
    auto& map = m_regs[inst];
    for (auto src : inst.srcs()) {
      regs |= map[src].regs();
    }
    return regs;
  }

  RegSet dstRegs(IRInstruction& inst) const {
    auto regs = RegSet();
    if (inst.is(Shuffle)) {
      for (auto const& dest : *inst.extra<Shuffle>()) {
        regs |= dest.regs();
      }
    } else {
      auto& map = m_regs[inst];
      for (auto& dst : inst.dsts()) {
        regs |= map[dst].regs();
      }
    }
    return regs;
  }

private:
  StateVector<IRInstruction,RegMap> m_regs;
};

// Return a valid register if this tmp should be forced into a particular
// register, otherwise return InvalidReg.
PhysReg forceAlloc(SSATmp& t);

/*
 * New register allocator doing extended linear scan
 */
RegAllocInfo allocateRegs(IRUnit&);

/*
 * Returns true if the instruction can store source operand srcIdx to
 * memory as a cell using a 16-byte store.  (implying its okay to
 * clobber TypedValue.m_aux)
 */
bool storesCell(const IRInstruction& inst, uint32_t srcIdx);

/*
 * Return true if this instruction can load a TypedValue using a 16-byte
 * load into a SIMD register.  Note that this function returns
 * false for instructions that load internal meta-data, such as Func*,
 * Class*, etc.
 */
bool loadsCell(Opcode op);

}}

#endif
