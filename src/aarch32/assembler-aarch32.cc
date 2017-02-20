// Copyright 2015, VIXL authors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of ARM Limited nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

extern "C" {
#include <stdint.h>
}

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "utils-vixl.h"
#include "aarch32/constants-aarch32.h"
#include "aarch32/instructions-aarch32.h"
#include "aarch32/operands-aarch32.h"
#include "aarch32/assembler-aarch32.h"

namespace vixl {
namespace aarch32 {

void Assembler::EmitT32_16(uint16_t instr) {
  VIXL_ASSERT(buffer_.Is16bitAligned());
  buffer_.Emit16(instr);
}


void Assembler::EmitT32_32(uint32_t instr) {
  VIXL_ASSERT(buffer_.Is16bitAligned());
  buffer_.Emit16(static_cast<uint16_t>(instr >> 16));
  buffer_.Emit16(static_cast<uint16_t>(instr & 0xffff));
}


void Assembler::EmitA32(uint32_t instr) {
  VIXL_ASSERT(buffer_.Is32bitAligned());
  buffer_.Emit32(instr);
}


#ifdef VIXL_DEBUG
void Assembler::PerformCheckIT(Condition condition) {
  if (it_mask_ == 0) {
    VIXL_ASSERT(IsUsingA32() || condition.Is(al));
  } else {
    VIXL_ASSERT(condition.Is(first_condition_));
    first_condition_ =
        Condition((first_condition_.GetCondition() & 0xe) | (it_mask_ >> 3));
    // For A32, AdavanceIT() is not called by the assembler. We must call it
    // in order to check that IT instructions are used consistently with
    // the following conditional instructions.
    if (IsUsingA32()) AdvanceIT();
  }
}
#endif


void Assembler::BindHelper(Label* label) {
  VIXL_ASSERT(!label->IsBound());
  label->Bind(GetCursorOffset());

  for (Label::ForwardRefList::iterator ref = label->GetFirstForwardRef();
       ref != label->GetEndForwardRef();
       ref++) {
    EncodeLabelFor(*ref, label);
  }
  if (label->IsInVeneerPool()) {
    label->GetVeneerPoolManager()->RemoveLabel(label);
  }
}


uint32_t Assembler::Link(uint32_t instr,
                         Label* label,
                         const Label::LabelEmitOperator& op) {
  label->SetReferenced();
  if (label->IsBound()) {
    return op.Encode(instr,
                     GetCursorOffset() + GetArchitectureStatePCOffset(),
                     label);
  }
  label->AddForwardRef(GetCursorOffset(), GetInstructionSetInUse(), op);
  return instr;
}


void Assembler::EncodeLabelFor(const Label::ForwardReference& forward,
                               Label* label) {
  const uint32_t location = forward.GetLocation();
  const uint32_t from = location + forward.GetStatePCOffset();
  const Label::LabelEmitOperator& encoder = forward.GetEmitOperator();
  if (forward.IsUsingT32()) {
    uint16_t* instr_ptr = buffer_.GetOffsetAddress<uint16_t*>(location);
    if (Is16BitEncoding(instr_ptr[0])) {
      // The Encode methods always deals with uint32_t types so we need
      // to explicitely cast it.
      uint32_t instr = static_cast<uint32_t>(instr_ptr[0]);
      instr = encoder.Encode(instr, from, label);
      // The Encode method should not ever set the top 16 bits.
      VIXL_ASSERT((instr & ~0xffff) == 0);
      instr_ptr[0] = static_cast<uint16_t>(instr);
    } else {
      uint32_t instr =
          instr_ptr[1] | (static_cast<uint32_t>(instr_ptr[0]) << 16);
      instr = encoder.Encode(instr, from, label);
      instr_ptr[0] = static_cast<uint16_t>(instr >> 16);
      instr_ptr[1] = static_cast<uint16_t>(instr);
    }
  } else {
    uint32_t* instr_ptr = buffer_.GetOffsetAddress<uint32_t*>(location);
    instr_ptr[0] = encoder.Encode(instr_ptr[0], from, label);
  }
}


// Start of generated code.
class Dt_L_imm6_1 : public EncodingValue {
  uint32_t type_;

 public:
  explicit Dt_L_imm6_1(DataType dt);
  uint32_t GetTypeEncodingValue() const { return type_; }
};

Dt_L_imm6_1::Dt_L_imm6_1(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      type_ = 0x0;
      SetEncodingValue(0x1);
      break;
    case U8:
      type_ = 0x1;
      SetEncodingValue(0x1);
      break;
    case S16:
      type_ = 0x0;
      SetEncodingValue(0x2);
      break;
    case U16:
      type_ = 0x1;
      SetEncodingValue(0x2);
      break;
    case S32:
      type_ = 0x0;
      SetEncodingValue(0x4);
      break;
    case U32:
      type_ = 0x1;
      SetEncodingValue(0x4);
      break;
    case S64:
      type_ = 0x0;
      SetEncodingValue(0x8);
      break;
    case U64:
      type_ = 0x1;
      SetEncodingValue(0x8);
      break;
    default:
      VIXL_UNREACHABLE();
      type_ = 0x0;
      break;
  }
}

class Dt_L_imm6_2 : public EncodingValue {
  uint32_t type_;

 public:
  explicit Dt_L_imm6_2(DataType dt);
  uint32_t GetTypeEncodingValue() const { return type_; }
};

Dt_L_imm6_2::Dt_L_imm6_2(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      type_ = 0x1;
      SetEncodingValue(0x1);
      break;
    case S16:
      type_ = 0x1;
      SetEncodingValue(0x2);
      break;
    case S32:
      type_ = 0x1;
      SetEncodingValue(0x4);
      break;
    case S64:
      type_ = 0x1;
      SetEncodingValue(0x8);
      break;
    default:
      VIXL_UNREACHABLE();
      type_ = 0x0;
      break;
  }
}

class Dt_L_imm6_3 : public EncodingValue {
 public:
  explicit Dt_L_imm6_3(DataType dt);
};

Dt_L_imm6_3::Dt_L_imm6_3(DataType dt) {
  switch (dt.GetValue()) {
    case I8:
      SetEncodingValue(0x1);
      break;
    case I16:
      SetEncodingValue(0x2);
      break;
    case I32:
      SetEncodingValue(0x4);
      break;
    case I64:
      SetEncodingValue(0x8);
      break;
    default:
      break;
  }
}

class Dt_L_imm6_4 : public EncodingValue {
 public:
  explicit Dt_L_imm6_4(DataType dt);
};

Dt_L_imm6_4::Dt_L_imm6_4(DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8:
      SetEncodingValue(0x1);
      break;
    case Untyped16:
      SetEncodingValue(0x2);
      break;
    case Untyped32:
      SetEncodingValue(0x4);
      break;
    case Untyped64:
      SetEncodingValue(0x8);
      break;
    default:
      break;
  }
}

class Dt_imm6_1 : public EncodingValue {
  uint32_t type_;

 public:
  explicit Dt_imm6_1(DataType dt);
  uint32_t GetTypeEncodingValue() const { return type_; }
};

Dt_imm6_1::Dt_imm6_1(DataType dt) {
  switch (dt.GetValue()) {
    case S16:
      type_ = 0x0;
      SetEncodingValue(0x1);
      break;
    case U16:
      type_ = 0x1;
      SetEncodingValue(0x1);
      break;
    case S32:
      type_ = 0x0;
      SetEncodingValue(0x2);
      break;
    case U32:
      type_ = 0x1;
      SetEncodingValue(0x2);
      break;
    case S64:
      type_ = 0x0;
      SetEncodingValue(0x4);
      break;
    case U64:
      type_ = 0x1;
      SetEncodingValue(0x4);
      break;
    default:
      VIXL_UNREACHABLE();
      type_ = 0x0;
      break;
  }
}

class Dt_imm6_2 : public EncodingValue {
  uint32_t type_;

 public:
  explicit Dt_imm6_2(DataType dt);
  uint32_t GetTypeEncodingValue() const { return type_; }
};

Dt_imm6_2::Dt_imm6_2(DataType dt) {
  switch (dt.GetValue()) {
    case S16:
      type_ = 0x1;
      SetEncodingValue(0x1);
      break;
    case S32:
      type_ = 0x1;
      SetEncodingValue(0x2);
      break;
    case S64:
      type_ = 0x1;
      SetEncodingValue(0x4);
      break;
    default:
      VIXL_UNREACHABLE();
      type_ = 0x0;
      break;
  }
}

class Dt_imm6_3 : public EncodingValue {
 public:
  explicit Dt_imm6_3(DataType dt);
};

Dt_imm6_3::Dt_imm6_3(DataType dt) {
  switch (dt.GetValue()) {
    case I16:
      SetEncodingValue(0x1);
      break;
    case I32:
      SetEncodingValue(0x2);
      break;
    case I64:
      SetEncodingValue(0x4);
      break;
    default:
      break;
  }
}

class Dt_imm6_4 : public EncodingValue {
  uint32_t type_;

 public:
  explicit Dt_imm6_4(DataType dt);
  uint32_t GetTypeEncodingValue() const { return type_; }
};

Dt_imm6_4::Dt_imm6_4(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      type_ = 0x0;
      SetEncodingValue(0x1);
      break;
    case U8:
      type_ = 0x1;
      SetEncodingValue(0x1);
      break;
    case S16:
      type_ = 0x0;
      SetEncodingValue(0x2);
      break;
    case U16:
      type_ = 0x1;
      SetEncodingValue(0x2);
      break;
    case S32:
      type_ = 0x0;
      SetEncodingValue(0x4);
      break;
    case U32:
      type_ = 0x1;
      SetEncodingValue(0x4);
      break;
    default:
      VIXL_UNREACHABLE();
      type_ = 0x0;
      break;
  }
}

class Dt_op_U_size_1 : public EncodingValue {
 public:
  explicit Dt_op_U_size_1(DataType dt);
};

Dt_op_U_size_1::Dt_op_U_size_1(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      SetEncodingValue(0x0);
      break;
    case S16:
      SetEncodingValue(0x1);
      break;
    case S32:
      SetEncodingValue(0x2);
      break;
    case U8:
      SetEncodingValue(0x4);
      break;
    case U16:
      SetEncodingValue(0x5);
      break;
    case U32:
      SetEncodingValue(0x6);
      break;
    case P8:
      SetEncodingValue(0x8);
      break;
    case P64:
      SetEncodingValue(0xa);
      break;
    default:
      break;
  }
}

class Dt_op_size_1 : public EncodingValue {
 public:
  explicit Dt_op_size_1(DataType dt);
};

Dt_op_size_1::Dt_op_size_1(DataType dt) {
  switch (dt.GetValue()) {
    case I8:
      SetEncodingValue(0x0);
      break;
    case I16:
      SetEncodingValue(0x1);
      break;
    case I32:
      SetEncodingValue(0x2);
      break;
    case P8:
      SetEncodingValue(0x4);
      break;
    default:
      break;
  }
}

class Dt_op_size_2 : public EncodingValue {
 public:
  explicit Dt_op_size_2(DataType dt);
};

Dt_op_size_2::Dt_op_size_2(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      SetEncodingValue(0x0);
      break;
    case S16:
      SetEncodingValue(0x1);
      break;
    case S32:
      SetEncodingValue(0x2);
      break;
    case U8:
      SetEncodingValue(0x4);
      break;
    case U16:
      SetEncodingValue(0x5);
      break;
    case U32:
      SetEncodingValue(0x6);
      break;
    default:
      break;
  }
}

class Dt_op_size_3 : public EncodingValue {
 public:
  explicit Dt_op_size_3(DataType dt);
};

Dt_op_size_3::Dt_op_size_3(DataType dt) {
  switch (dt.GetValue()) {
    case S16:
      SetEncodingValue(0x0);
      break;
    case S32:
      SetEncodingValue(0x1);
      break;
    case S64:
      SetEncodingValue(0x2);
      break;
    case U16:
      SetEncodingValue(0x4);
      break;
    case U32:
      SetEncodingValue(0x5);
      break;
    case U64:
      SetEncodingValue(0x6);
      break;
    default:
      break;
  }
}

class Dt_U_imm3H_1 : public EncodingValue {
 public:
  explicit Dt_U_imm3H_1(DataType dt);
};

Dt_U_imm3H_1::Dt_U_imm3H_1(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      SetEncodingValue(0x1);
      break;
    case S16:
      SetEncodingValue(0x2);
      break;
    case S32:
      SetEncodingValue(0x4);
      break;
    case U8:
      SetEncodingValue(0x9);
      break;
    case U16:
      SetEncodingValue(0xa);
      break;
    case U32:
      SetEncodingValue(0xc);
      break;
    default:
      break;
  }
}

class Dt_U_opc1_opc2_1 : public EncodingValue {
 public:
  explicit Dt_U_opc1_opc2_1(DataType dt, const DRegisterLane& lane);
};

Dt_U_opc1_opc2_1::Dt_U_opc1_opc2_1(DataType dt, const DRegisterLane& lane) {
  switch (dt.GetValue()) {
    case S8:
      if ((lane.GetLane() & 7) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x8 | lane.GetLane());
      break;
    case S16:
      if ((lane.GetLane() & 3) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x1 | (lane.GetLane() << 1));
      break;
    case U8:
      if ((lane.GetLane() & 7) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x18 | lane.GetLane());
      break;
    case U16:
      if ((lane.GetLane() & 3) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x11 | (lane.GetLane() << 1));
      break;
    case Untyped32:
      if ((lane.GetLane() & 1) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x0 | (lane.GetLane() << 2));
      break;
    case kDataTypeValueNone:
      if ((lane.GetLane() & 1) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x0 | (lane.GetLane() << 2));
      break;
    default:
      break;
  }
}

class Dt_opc1_opc2_1 : public EncodingValue {
 public:
  explicit Dt_opc1_opc2_1(DataType dt, const DRegisterLane& lane);
};

Dt_opc1_opc2_1::Dt_opc1_opc2_1(DataType dt, const DRegisterLane& lane) {
  switch (dt.GetValue()) {
    case Untyped8:
      if ((lane.GetLane() & 7) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x8 | lane.GetLane());
      break;
    case Untyped16:
      if ((lane.GetLane() & 3) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x1 | (lane.GetLane() << 1));
      break;
    case Untyped32:
      if ((lane.GetLane() & 1) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x0 | (lane.GetLane() << 2));
      break;
    case kDataTypeValueNone:
      if ((lane.GetLane() & 1) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x0 | (lane.GetLane() << 2));
      break;
    default:
      break;
  }
}

class Dt_imm4_1 : public EncodingValue {
 public:
  explicit Dt_imm4_1(DataType dt, const DRegisterLane& lane);
};

Dt_imm4_1::Dt_imm4_1(DataType dt, const DRegisterLane& lane) {
  switch (dt.GetValue()) {
    case Untyped8:
      if ((lane.GetLane() & 7) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x1 | (lane.GetLane() << 1));
      break;
    case Untyped16:
      if ((lane.GetLane() & 3) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x2 | (lane.GetLane() << 2));
      break;
    case Untyped32:
      if ((lane.GetLane() & 1) != lane.GetLane()) {
        return;
      }
      SetEncodingValue(0x4 | (lane.GetLane() << 3));
      break;
    default:
      break;
  }
}

class Dt_B_E_1 : public EncodingValue {
 public:
  explicit Dt_B_E_1(DataType dt);
};

Dt_B_E_1::Dt_B_E_1(DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8:
      SetEncodingValue(0x2);
      break;
    case Untyped16:
      SetEncodingValue(0x1);
      break;
    case Untyped32:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Dt_op_1 : public EncodingValue {
 public:
  Dt_op_1(DataType dt1, DataType dt2);
};

Dt_op_1::Dt_op_1(DataType dt1, DataType dt2) {
  if ((dt1.GetValue() == F32) && (dt2.GetValue() == S32)) {
    SetEncodingValue(0x0);
    return;
  }
  if ((dt1.GetValue() == F32) && (dt2.GetValue() == U32)) {
    SetEncodingValue(0x1);
    return;
  }
  if ((dt1.GetValue() == S32) && (dt2.GetValue() == F32)) {
    SetEncodingValue(0x2);
    return;
  }
  if ((dt1.GetValue() == U32) && (dt2.GetValue() == F32)) {
    SetEncodingValue(0x3);
    return;
  }
}

class Dt_op_2 : public EncodingValue {
 public:
  explicit Dt_op_2(DataType dt);
};

Dt_op_2::Dt_op_2(DataType dt) {
  switch (dt.GetValue()) {
    case U32:
      SetEncodingValue(0x0);
      break;
    case S32:
      SetEncodingValue(0x1);
      break;
    default:
      break;
  }
}

class Dt_op_3 : public EncodingValue {
 public:
  explicit Dt_op_3(DataType dt);
};

Dt_op_3::Dt_op_3(DataType dt) {
  switch (dt.GetValue()) {
    case S32:
      SetEncodingValue(0x0);
      break;
    case U32:
      SetEncodingValue(0x1);
      break;
    default:
      break;
  }
}

class Dt_U_sx_1 : public EncodingValue {
 public:
  explicit Dt_U_sx_1(DataType dt);
};

Dt_U_sx_1::Dt_U_sx_1(DataType dt) {
  switch (dt.GetValue()) {
    case S16:
      SetEncodingValue(0x0);
      break;
    case S32:
      SetEncodingValue(0x1);
      break;
    case U16:
      SetEncodingValue(0x2);
      break;
    case U32:
      SetEncodingValue(0x3);
      break;
    default:
      break;
  }
}

class Dt_op_U_1 : public EncodingValue {
 public:
  Dt_op_U_1(DataType dt1, DataType dt2);
};

Dt_op_U_1::Dt_op_U_1(DataType dt1, DataType dt2) {
  if ((dt1.GetValue() == F32) && (dt2.GetValue() == S32)) {
    SetEncodingValue(0x0);
    return;
  }
  if ((dt1.GetValue() == F32) && (dt2.GetValue() == U32)) {
    SetEncodingValue(0x1);
    return;
  }
  if ((dt1.GetValue() == S32) && (dt2.GetValue() == F32)) {
    SetEncodingValue(0x2);
    return;
  }
  if ((dt1.GetValue() == U32) && (dt2.GetValue() == F32)) {
    SetEncodingValue(0x3);
    return;
  }
}

class Dt_sz_1 : public EncodingValue {
 public:
  explicit Dt_sz_1(DataType dt);
};

Dt_sz_1::Dt_sz_1(DataType dt) {
  switch (dt.GetValue()) {
    case F32:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Dt_F_size_1 : public EncodingValue {
 public:
  explicit Dt_F_size_1(DataType dt);
};

Dt_F_size_1::Dt_F_size_1(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      SetEncodingValue(0x0);
      break;
    case S16:
      SetEncodingValue(0x1);
      break;
    case S32:
      SetEncodingValue(0x2);
      break;
    case F32:
      SetEncodingValue(0x6);
      break;
    default:
      break;
  }
}

class Dt_F_size_2 : public EncodingValue {
 public:
  explicit Dt_F_size_2(DataType dt);
};

Dt_F_size_2::Dt_F_size_2(DataType dt) {
  switch (dt.GetValue()) {
    case I8:
      SetEncodingValue(0x0);
      break;
    case I16:
      SetEncodingValue(0x1);
      break;
    case I32:
      SetEncodingValue(0x2);
      break;
    case F32:
      SetEncodingValue(0x6);
      break;
    default:
      break;
  }
}

class Dt_F_size_3 : public EncodingValue {
 public:
  explicit Dt_F_size_3(DataType dt);
};

Dt_F_size_3::Dt_F_size_3(DataType dt) {
  switch (dt.GetValue()) {
    case I16:
      SetEncodingValue(0x1);
      break;
    case I32:
      SetEncodingValue(0x2);
      break;
    case F32:
      SetEncodingValue(0x6);
      break;
    default:
      break;
  }
}

class Dt_F_size_4 : public EncodingValue {
 public:
  explicit Dt_F_size_4(DataType dt);
};

Dt_F_size_4::Dt_F_size_4(DataType dt) {
  switch (dt.GetValue()) {
    case U32:
      SetEncodingValue(0x2);
      break;
    case F32:
      SetEncodingValue(0x6);
      break;
    default:
      break;
  }
}

class Dt_U_size_1 : public EncodingValue {
 public:
  explicit Dt_U_size_1(DataType dt);
};

Dt_U_size_1::Dt_U_size_1(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      SetEncodingValue(0x0);
      break;
    case S16:
      SetEncodingValue(0x1);
      break;
    case S32:
      SetEncodingValue(0x2);
      break;
    case U8:
      SetEncodingValue(0x4);
      break;
    case U16:
      SetEncodingValue(0x5);
      break;
    case U32:
      SetEncodingValue(0x6);
      break;
    default:
      break;
  }
}

class Dt_U_size_2 : public EncodingValue {
 public:
  explicit Dt_U_size_2(DataType dt);
};

Dt_U_size_2::Dt_U_size_2(DataType dt) {
  switch (dt.GetValue()) {
    case S16:
      SetEncodingValue(0x1);
      break;
    case S32:
      SetEncodingValue(0x2);
      break;
    case U16:
      SetEncodingValue(0x5);
      break;
    case U32:
      SetEncodingValue(0x6);
      break;
    default:
      break;
  }
}

class Dt_U_size_3 : public EncodingValue {
 public:
  explicit Dt_U_size_3(DataType dt);
};

Dt_U_size_3::Dt_U_size_3(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      SetEncodingValue(0x0);
      break;
    case S16:
      SetEncodingValue(0x1);
      break;
    case S32:
      SetEncodingValue(0x2);
      break;
    case S64:
      SetEncodingValue(0x3);
      break;
    case U8:
      SetEncodingValue(0x4);
      break;
    case U16:
      SetEncodingValue(0x5);
      break;
    case U32:
      SetEncodingValue(0x6);
      break;
    case U64:
      SetEncodingValue(0x7);
      break;
    default:
      break;
  }
}

class Dt_size_1 : public EncodingValue {
 public:
  explicit Dt_size_1(DataType dt);
};

Dt_size_1::Dt_size_1(DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Dt_size_2 : public EncodingValue {
 public:
  explicit Dt_size_2(DataType dt);
};

Dt_size_2::Dt_size_2(DataType dt) {
  switch (dt.GetValue()) {
    case I8:
      SetEncodingValue(0x0);
      break;
    case I16:
      SetEncodingValue(0x1);
      break;
    case I32:
      SetEncodingValue(0x2);
      break;
    case I64:
      SetEncodingValue(0x3);
      break;
    default:
      break;
  }
}

class Dt_size_3 : public EncodingValue {
 public:
  explicit Dt_size_3(DataType dt);
};

Dt_size_3::Dt_size_3(DataType dt) {
  switch (dt.GetValue()) {
    case I16:
      SetEncodingValue(0x0);
      break;
    case I32:
      SetEncodingValue(0x1);
      break;
    case I64:
      SetEncodingValue(0x2);
      break;
    default:
      break;
  }
}

class Dt_size_4 : public EncodingValue {
 public:
  explicit Dt_size_4(DataType dt);
};

Dt_size_4::Dt_size_4(DataType dt) {
  switch (dt.GetValue()) {
    case I8:
      SetEncodingValue(0x0);
      break;
    case I16:
      SetEncodingValue(0x1);
      break;
    case I32:
      SetEncodingValue(0x2);
      break;
    default:
      break;
  }
}

class Dt_size_5 : public EncodingValue {
 public:
  explicit Dt_size_5(DataType dt);
};

Dt_size_5::Dt_size_5(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      SetEncodingValue(0x0);
      break;
    case S16:
      SetEncodingValue(0x1);
      break;
    case S32:
      SetEncodingValue(0x2);
      break;
    default:
      break;
  }
}

class Dt_size_6 : public EncodingValue {
 public:
  explicit Dt_size_6(DataType dt);
};

Dt_size_6::Dt_size_6(DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8:
      SetEncodingValue(0x0);
      break;
    case Untyped16:
      SetEncodingValue(0x1);
      break;
    case Untyped32:
      SetEncodingValue(0x2);
      break;
    case Untyped64:
      SetEncodingValue(0x3);
      break;
    default:
      break;
  }
}

class Dt_size_7 : public EncodingValue {
 public:
  explicit Dt_size_7(DataType dt);
};

Dt_size_7::Dt_size_7(DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8:
      SetEncodingValue(0x0);
      break;
    case Untyped16:
      SetEncodingValue(0x1);
      break;
    case Untyped32:
      SetEncodingValue(0x2);
      break;
    default:
      break;
  }
}

class Dt_size_8 : public EncodingValue {
 public:
  Dt_size_8(DataType dt, Alignment align);
};

Dt_size_8::Dt_size_8(DataType dt, Alignment align) {
  switch (dt.GetValue()) {
    case Untyped8:
      SetEncodingValue(0x0);
      break;
    case Untyped16:
      SetEncodingValue(0x1);
      break;
    case Untyped32:
      if (align.Is(k64BitAlign) || align.Is(kNoAlignment)) {
        SetEncodingValue(0x2);
      } else if (align.Is(k128BitAlign)) {
        SetEncodingValue(0x3);
      }
      break;
    default:
      break;
  }
}

class Dt_size_9 : public EncodingValue {
  uint32_t type_;

 public:
  explicit Dt_size_9(DataType dt);
  uint32_t GetTypeEncodingValue() const { return type_; }
};

Dt_size_9::Dt_size_9(DataType dt) {
  switch (dt.GetValue()) {
    case I16:
      type_ = 0x0;
      SetEncodingValue(0x1);
      break;
    case I32:
      type_ = 0x0;
      SetEncodingValue(0x2);
      break;
    case F32:
      type_ = 0x1;
      SetEncodingValue(0x2);
      break;
    default:
      VIXL_UNREACHABLE();
      type_ = 0x0;
      break;
  }
}

class Dt_size_10 : public EncodingValue {
 public:
  explicit Dt_size_10(DataType dt);
};

Dt_size_10::Dt_size_10(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
    case U8:
    case I8:
      SetEncodingValue(0x0);
      break;
    case S16:
    case U16:
    case I16:
      SetEncodingValue(0x1);
      break;
    case S32:
    case U32:
    case I32:
      SetEncodingValue(0x2);
      break;
    default:
      break;
  }
}

class Dt_size_11 : public EncodingValue {
  uint32_t type_;

 public:
  explicit Dt_size_11(DataType dt);
  uint32_t GetTypeEncodingValue() const { return type_; }
};

Dt_size_11::Dt_size_11(DataType dt) {
  switch (dt.GetValue()) {
    case S16:
      type_ = 0x0;
      SetEncodingValue(0x1);
      break;
    case U16:
      type_ = 0x1;
      SetEncodingValue(0x1);
      break;
    case S32:
      type_ = 0x0;
      SetEncodingValue(0x2);
      break;
    case U32:
      type_ = 0x1;
      SetEncodingValue(0x2);
      break;
    default:
      VIXL_UNREACHABLE();
      type_ = 0x0;
      break;
  }
}

class Dt_size_12 : public EncodingValue {
  uint32_t type_;

 public:
  explicit Dt_size_12(DataType dt);
  uint32_t GetTypeEncodingValue() const { return type_; }
};

Dt_size_12::Dt_size_12(DataType dt) {
  switch (dt.GetValue()) {
    case S8:
      type_ = 0x0;
      SetEncodingValue(0x0);
      break;
    case U8:
      type_ = 0x1;
      SetEncodingValue(0x0);
      break;
    case S16:
      type_ = 0x0;
      SetEncodingValue(0x1);
      break;
    case U16:
      type_ = 0x1;
      SetEncodingValue(0x1);
      break;
    case S32:
      type_ = 0x0;
      SetEncodingValue(0x2);
      break;
    case U32:
      type_ = 0x1;
      SetEncodingValue(0x2);
      break;
    default:
      VIXL_UNREACHABLE();
      type_ = 0x0;
      break;
  }
}

class Dt_size_13 : public EncodingValue {
 public:
  explicit Dt_size_13(DataType dt);
};

Dt_size_13::Dt_size_13(DataType dt) {
  switch (dt.GetValue()) {
    case S16:
      SetEncodingValue(0x1);
      break;
    case S32:
      SetEncodingValue(0x2);
      break;
    default:
      break;
  }
}

class Dt_size_14 : public EncodingValue {
 public:
  explicit Dt_size_14(DataType dt);
};

Dt_size_14::Dt_size_14(DataType dt) {
  switch (dt.GetValue()) {
    case S16:
      SetEncodingValue(0x0);
      break;
    case S32:
      SetEncodingValue(0x1);
      break;
    case S64:
      SetEncodingValue(0x2);
      break;
    default:
      break;
  }
}

class Dt_size_15 : public EncodingValue {
 public:
  explicit Dt_size_15(DataType dt);
};

Dt_size_15::Dt_size_15(DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8:
      SetEncodingValue(0x0);
      break;
    case Untyped16:
      SetEncodingValue(0x1);
      break;
    default:
      break;
  }
}

class Dt_size_16 : public EncodingValue {
 public:
  explicit Dt_size_16(DataType dt);
};

Dt_size_16::Dt_size_16(DataType dt) {
  switch (dt.GetValue()) {
    case I8:
      SetEncodingValue(0x0);
      break;
    case I16:
      SetEncodingValue(0x1);
      break;
    case I32:
      SetEncodingValue(0x2);
      break;
    default:
      break;
  }
}

class Index_1 : public EncodingValue {
 public:
  Index_1(const NeonRegisterList& nreglist, DataType dt);
};

Index_1::Index_1(const NeonRegisterList& nreglist, DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8: {
      if ((nreglist.GetTransferLane() & 7) != nreglist.GetTransferLane()) {
        return;
      }
      uint32_t value = nreglist.GetTransferLane() << 1;
      if (!nreglist.IsSingleSpaced()) return;
      SetEncodingValue(value);
      break;
    }
    case Untyped16: {
      if ((nreglist.GetTransferLane() & 3) != nreglist.GetTransferLane()) {
        return;
      }
      uint32_t value = nreglist.GetTransferLane() << 2;
      if (nreglist.IsDoubleSpaced()) value |= 2;
      SetEncodingValue(value);
      break;
    }
    case Untyped32: {
      if ((nreglist.GetTransferLane() & 1) != nreglist.GetTransferLane()) {
        return;
      }
      uint32_t value = nreglist.GetTransferLane() << 3;
      if (nreglist.IsDoubleSpaced()) value |= 4;
      SetEncodingValue(value);
      break;
    }
    default:
      break;
  }
}

class Align_index_align_1 : public EncodingValue {
 public:
  Align_index_align_1(Alignment align,
                      const NeonRegisterList& nreglist,
                      DataType dt);
};

Align_index_align_1::Align_index_align_1(Alignment align,
                                         const NeonRegisterList& nreglist,
                                         DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8: {
      uint32_t value;
      if (align.GetType() == kNoAlignment) {
        value = 0;
      } else {
        return;
      }
      if ((nreglist.GetTransferLane() & 7) != nreglist.GetTransferLane()) {
        return;
      }
      value |= nreglist.GetTransferLane() << 1;
      SetEncodingValue(value);
      break;
    }
    case Untyped16: {
      uint32_t value;
      if (align.GetType() == k16BitAlign) {
        value = 1;
      } else if (align.GetType() == kNoAlignment) {
        value = 0;
      } else {
        return;
      }
      if ((nreglist.GetTransferLane() & 3) != nreglist.GetTransferLane()) {
        return;
      }
      value |= nreglist.GetTransferLane() << 2;
      SetEncodingValue(value);
      break;
    }
    case Untyped32: {
      uint32_t value;
      if (align.GetType() == k32BitAlign) {
        value = 3;
      } else if (align.GetType() == kNoAlignment) {
        value = 0;
      } else {
        return;
      }
      if ((nreglist.GetTransferLane() & 1) != nreglist.GetTransferLane()) {
        return;
      }
      value |= nreglist.GetTransferLane() << 3;
      SetEncodingValue(value);
      break;
    }
    default:
      break;
  }
}

class Align_index_align_2 : public EncodingValue {
 public:
  Align_index_align_2(Alignment align,
                      const NeonRegisterList& nreglist,
                      DataType dt);
};

Align_index_align_2::Align_index_align_2(Alignment align,
                                         const NeonRegisterList& nreglist,
                                         DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8: {
      uint32_t value;
      if (align.GetType() == k16BitAlign) {
        value = 1;
      } else if (align.GetType() == kNoAlignment) {
        value = 0;
      } else {
        return;
      }
      if ((nreglist.GetTransferLane() & 7) != nreglist.GetTransferLane()) {
        return;
      }
      value |= nreglist.GetTransferLane() << 1;
      if (!nreglist.IsSingleSpaced()) return;
      SetEncodingValue(value);
      break;
    }
    case Untyped16: {
      uint32_t value;
      if (align.GetType() == k32BitAlign) {
        value = 1;
      } else if (align.GetType() == kNoAlignment) {
        value = 0;
      } else {
        return;
      }
      if ((nreglist.GetTransferLane() & 3) != nreglist.GetTransferLane()) {
        return;
      }
      value |= nreglist.GetTransferLane() << 2;
      if (nreglist.IsDoubleSpaced()) value |= 2;
      SetEncodingValue(value);
      break;
    }
    case Untyped32: {
      uint32_t value;
      if (align.GetType() == k64BitAlign) {
        value = 1;
      } else if (align.GetType() == kNoAlignment) {
        value = 0;
      } else {
        return;
      }
      if ((nreglist.GetTransferLane() & 1) != nreglist.GetTransferLane()) {
        return;
      }
      value |= nreglist.GetTransferLane() << 3;
      if (nreglist.IsDoubleSpaced()) value |= 4;
      SetEncodingValue(value);
      break;
    }
    default:
      break;
  }
}

class Align_index_align_3 : public EncodingValue {
 public:
  Align_index_align_3(Alignment align,
                      const NeonRegisterList& nreglist,
                      DataType dt);
};

Align_index_align_3::Align_index_align_3(Alignment align,
                                         const NeonRegisterList& nreglist,
                                         DataType dt) {
  switch (dt.GetValue()) {
    case Untyped8: {
      uint32_t value;
      if (align.GetType() == k32BitAlign) {
        value = 1;
      } else if (align.GetType() == kNoAlignment) {
        value = 0;
      } else {
        return;
      }
      if ((nreglist.GetTransferLane() & 7) != nreglist.GetTransferLane()) {
        return;
      }
      value |= nreglist.GetTransferLane() << 1;
      if (!nreglist.IsSingleSpaced()) return;
      SetEncodingValue(value);
      break;
    }
    case Untyped16: {
      uint32_t value;
      if (align.GetType() == k64BitAlign) {
        value = 1;
      } else if (align.GetType() == kNoAlignment) {
        value = 0;
      } else {
        return;
      }
      if ((nreglist.GetTransferLane() & 3) != nreglist.GetTransferLane()) {
        return;
      }
      value |= nreglist.GetTransferLane() << 2;
      if (nreglist.IsDoubleSpaced()) value |= 2;
      SetEncodingValue(value);
      break;
    }
    case Untyped32: {
      uint32_t value;
      if (align.GetType() == k64BitAlign) {
        value = 1;
      } else if (align.GetType() == k128BitAlign) {
        value = 2;
      } else if (align.GetType() == kNoAlignment) {
        value = 0;
      } else {
        return;
      }
      if ((nreglist.GetTransferLane() & 1) != nreglist.GetTransferLane()) {
        return;
      }
      value |= nreglist.GetTransferLane() << 3;
      if (nreglist.IsDoubleSpaced()) value |= 4;
      SetEncodingValue(value);
      break;
    }
    default:
      break;
  }
}

class Align_a_1 : public EncodingValue {
 public:
  Align_a_1(Alignment align, DataType dt);
};

Align_a_1::Align_a_1(Alignment align, DataType dt) {
  switch (align.GetType()) {
    case k16BitAlign:
      if (dt.Is(Untyped16)) SetEncodingValue(0x1);
      break;
    case k32BitAlign:
      if (dt.Is(Untyped32)) SetEncodingValue(0x1);
      break;
    case kNoAlignment:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Align_a_2 : public EncodingValue {
 public:
  Align_a_2(Alignment align, DataType dt);
};

Align_a_2::Align_a_2(Alignment align, DataType dt) {
  switch (align.GetType()) {
    case k16BitAlign:
      if (dt.Is(Untyped8)) SetEncodingValue(0x1);
      break;
    case k32BitAlign:
      if (dt.Is(Untyped16)) SetEncodingValue(0x1);
      break;
    case k64BitAlign:
      if (dt.Is(Untyped32)) SetEncodingValue(0x1);
      break;
    case kNoAlignment:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Align_a_3 : public EncodingValue {
 public:
  Align_a_3(Alignment align, DataType dt);
};

Align_a_3::Align_a_3(Alignment align, DataType dt) {
  switch (align.GetType()) {
    case k32BitAlign:
      if (dt.Is(Untyped8)) SetEncodingValue(0x1);
      break;
    case k64BitAlign:
      if (dt.Is(Untyped16))
        SetEncodingValue(0x1);
      else if (dt.Is(Untyped32))
        SetEncodingValue(0x1);
      break;
    case k128BitAlign:
      if (dt.Is(Untyped32)) SetEncodingValue(0x1);
      break;
    case kNoAlignment:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Align_align_1 : public EncodingValue {
 public:
  Align_align_1(Alignment align, const NeonRegisterList& nreglist);
};

Align_align_1::Align_align_1(Alignment align,
                             const NeonRegisterList& nreglist) {
  switch (align.GetType()) {
    case k64BitAlign:
      SetEncodingValue(0x1);
      break;
    case k128BitAlign:
      if ((nreglist.GetLength() == 2) || (nreglist.GetLength() == 4))
        SetEncodingValue(0x2);
      break;
    case k256BitAlign:
      if ((nreglist.GetLength() == 2) || (nreglist.GetLength() == 4))
        SetEncodingValue(0x3);
      break;
    case kNoAlignment:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Align_align_2 : public EncodingValue {
 public:
  Align_align_2(Alignment align, const NeonRegisterList& nreglist);
};

Align_align_2::Align_align_2(Alignment align,
                             const NeonRegisterList& nreglist) {
  switch (align.GetType()) {
    case k64BitAlign:
      SetEncodingValue(0x1);
      break;
    case k128BitAlign:
      SetEncodingValue(0x2);
      break;
    case k256BitAlign:
      if ((nreglist.GetLength() == 4)) SetEncodingValue(0x3);
      break;
    case kNoAlignment:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Align_align_3 : public EncodingValue {
 public:
  explicit Align_align_3(Alignment align);
};

Align_align_3::Align_align_3(Alignment align) {
  switch (align.GetType()) {
    case k64BitAlign:
      SetEncodingValue(0x1);
      break;
    case kNoAlignment:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Align_align_4 : public EncodingValue {
 public:
  explicit Align_align_4(Alignment align);
};

Align_align_4::Align_align_4(Alignment align) {
  switch (align.GetType()) {
    case k64BitAlign:
      SetEncodingValue(0x1);
      break;
    case k128BitAlign:
      SetEncodingValue(0x2);
      break;
    case k256BitAlign:
      SetEncodingValue(0x3);
      break;
    case kNoAlignment:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}

class Align_align_5 : public EncodingValue {
 public:
  Align_align_5(Alignment align, const NeonRegisterList& nreglist);
};

Align_align_5::Align_align_5(Alignment align,
                             const NeonRegisterList& nreglist) {
  switch (align.GetType()) {
    case k64BitAlign:
      SetEncodingValue(0x1);
      break;
    case k128BitAlign:
      if ((nreglist.GetLength() == 2) || (nreglist.GetLength() == 4))
        SetEncodingValue(0x2);
      break;
    case k256BitAlign:
      if ((nreglist.GetLength() == 4)) SetEncodingValue(0x3);
      break;
    case kNoAlignment:
      SetEncodingValue(0x0);
      break;
    default:
      break;
  }
}


void Assembler::adc(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // ADC{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf1400000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // ADC{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02a00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // ADC<c>{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (InITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4140 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // ADC{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeb400000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ADC{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00a00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // ADC{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00a00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kAdc, &Assembler::adc, cond, size, rd, rn, operand);
}

void Assembler::adcs(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // ADCS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf1500000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // ADCS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02b00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // ADCS{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (OutsideITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4140 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // ADCS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeb500000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ADCS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00b00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // ADCS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00b00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kAdcs, &Assembler::adcs, cond, size, rd, rn, operand);
}

void Assembler::add(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // ADD{<c>}{<q>} <Rd>, PC, #<imm8> ; T1
      if (!size.IsWide() && rd.IsLow() && rn.Is(pc) && (imm <= 1020) &&
          ((imm % 4) == 0)) {
        uint32_t imm_ = imm >> 2;
        EmitT32_16(0xa000 | (rd.GetCode() << 8) | imm_);
        AdvanceIT();
        return;
      }
      // ADD<c>{<q>} <Rd>, <Rn>, #<imm3> ; T1
      if (InITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
          (imm <= 7)) {
        EmitT32_16(0x1c00 | rd.GetCode() | (rn.GetCode() << 3) | (imm << 6));
        AdvanceIT();
        return;
      }
      // ADD<c>{<q>} {<Rdn>}, <Rdn>, #<imm8> ; T2
      if (InITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
          (imm <= 255)) {
        EmitT32_16(0x3000 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
      // ADD{<c>}{<q>} <Rd>, SP, #<imm8> ; T1
      if (!size.IsWide() && rd.IsLow() && rn.Is(sp) && (imm <= 1020) &&
          ((imm % 4) == 0)) {
        uint32_t imm_ = imm >> 2;
        EmitT32_16(0xa800 | (rd.GetCode() << 8) | imm_);
        AdvanceIT();
        return;
      }
      // ADD{<c>}{<q>} {SP}, SP, #<imm7> ; T2
      if (!size.IsWide() && rd.Is(sp) && rn.Is(sp) && (imm <= 508) &&
          ((imm % 4) == 0)) {
        uint32_t imm_ = imm >> 2;
        EmitT32_16(0xb000 | imm_);
        AdvanceIT();
        return;
      }
      // ADD{<c>}{<q>} <Rd>, PC, #<imm12> ; T3
      if (!size.IsNarrow() && rn.Is(pc) && (imm <= 4095) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf20f0000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // ADD{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T3
      if (!size.IsNarrow() && immediate_t32.IsValid() && !rn.Is(sp) &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf1000000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // ADD{<c>}{<q>} {<Rd>}, <Rn>, #<imm12> ; T4
      if (!size.IsNarrow() && (imm <= 4095) && ((rn.GetCode() & 0xd) != 0xd) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2000000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (imm & 0xff) | ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // ADD{<c>}{<q>} {<Rd>}, SP, #<const> ; T3
      if (!size.IsNarrow() && rn.Is(sp) && immediate_t32.IsValid() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf10d0000U | (rd.GetCode() << 8) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // ADD{<c>}{<q>} {<Rd>}, SP, #<imm12> ; T4
      if (!size.IsNarrow() && rn.Is(sp) && (imm <= 4095) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf20d0000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // ADD{<c>}{<q>} <Rd>, PC, #<const> ; A1
      if (rn.Is(pc) && immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x028f0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
      // ADD{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever() &&
          ((rn.GetCode() & 0xd) != 0xd)) {
        EmitA32(0x02800000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
      // ADD{<c>}{<q>} {<Rd>}, SP, #<const> ; A1
      if (rn.Is(sp) && immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x028d0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // ADD<c>{<q>} <Rd>, <Rn>, <Rm> ; T1
        if (InITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x1800 | rd.GetCode() | (rn.GetCode() << 3) |
                     (rm.GetCode() << 6));
          AdvanceIT();
          return;
        }
        // ADD{<c>}{<q>} {<Rdn>}, <Rdn>, <Rm> ; T2
        if (!size.IsWide() && rd.Is(rn) && !rm.Is(sp) &&
            (((!rd.IsPC() || OutsideITBlockAndAlOrLast(cond)) &&
              (!rd.IsPC() || !rm.IsPC())) ||
             AllowUnpredictable())) {
          EmitT32_16(0x4400 | (rd.GetCode() & 0x7) |
                     ((rd.GetCode() & 0x8) << 4) | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
        // ADD{<c>}{<q>} {<Rdm>}, SP, <Rdm> ; T1
        if (!size.IsWide() && rd.Is(rm) && rn.Is(sp) &&
            ((!rd.IsPC() || OutsideITBlockAndAlOrLast(cond)) ||
             AllowUnpredictable())) {
          EmitT32_16(0x4468 | (rd.GetCode() & 0x7) |
                     ((rd.GetCode() & 0x8) << 4));
          AdvanceIT();
          return;
        }
        // ADD{<c>}{<q>} {SP}, SP, <Rm> ; T2
        if (!size.IsWide() && rd.Is(sp) && rn.Is(sp) && !rm.Is(sp)) {
          EmitT32_16(0x4485 | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // ADD{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T3
      if (!size.IsNarrow() && shift.IsValidAmount(amount) && !rn.Is(sp) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeb000000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
      // ADD{<c>}{<q>} {<Rd>}, SP, <Rm> {, <shift> #<amount> } ; T3
      if (!size.IsNarrow() && rn.Is(sp) && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeb0d0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ADD{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever() && !rn.Is(sp)) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00800000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
      // ADD{<c>}{<q>} {<Rd>}, SP, <Rm> {, <shift> #<amount> } ; A1
      if (rn.Is(sp) && shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x008d0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // ADD{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00800010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kAdd, &Assembler::add, cond, size, rd, rn, operand);
}

void Assembler::add(Condition cond, Register rd, const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // ADD<c>{<q>} <Rdn>, #<imm8> ; T2
      if (InITBlock() && rd.IsLow() && (imm <= 255)) {
        EmitT32_16(0x3000 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rm = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // ADD<c>{<q>} <Rdn>, <Rm> ; T2
      if (InITBlock() && !rm.Is(sp) &&
          (((!rd.IsPC() || OutsideITBlockAndAlOrLast(cond)) &&
            (!rd.IsPC() || !rm.IsPC())) ||
           AllowUnpredictable())) {
        EmitT32_16(0x4400 | (rd.GetCode() & 0x7) | ((rd.GetCode() & 0x8) << 4) |
                   (rm.GetCode() << 3));
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kAdd, &Assembler::add, cond, rd, operand);
}

void Assembler::adds(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // ADDS{<q>} <Rd>, <Rn>, #<imm3> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
          (imm <= 7)) {
        EmitT32_16(0x1c00 | rd.GetCode() | (rn.GetCode() << 3) | (imm << 6));
        AdvanceIT();
        return;
      }
      // ADDS{<q>} {<Rdn>}, <Rdn>, #<imm8> ; T2
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
          (imm <= 255)) {
        EmitT32_16(0x3000 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
      // ADDS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T3
      if (!size.IsNarrow() && immediate_t32.IsValid() && !rn.Is(sp) &&
          !rd.Is(pc) && (!rn.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf1100000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // ADDS{<c>}{<q>} {<Rd>}, SP, #<const> ; T3
      if (!size.IsNarrow() && rn.Is(sp) && immediate_t32.IsValid() &&
          !rd.Is(pc)) {
        EmitT32_32(0xf11d0000U | (rd.GetCode() << 8) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // ADDS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever() && !rn.Is(sp)) {
        EmitA32(0x02900000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
      // ADDS{<c>}{<q>} {<Rd>}, SP, #<const> ; A1
      if (rn.Is(sp) && immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x029d0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // ADDS{<q>} {<Rd>}, <Rn>, <Rm> ; T1
        if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x1800 | rd.GetCode() | (rn.GetCode() << 3) |
                     (rm.GetCode() << 6));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // ADDS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T3
      if (!size.IsNarrow() && shift.IsValidAmount(amount) && !rn.Is(sp) &&
          !rd.Is(pc) && ((!rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeb100000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
      // ADDS{<c>}{<q>} {<Rd>}, SP, <Rm> {, <shift> #<amount> } ; T3
      if (!size.IsNarrow() && rn.Is(sp) && shift.IsValidAmount(amount) &&
          !rd.Is(pc) && (!rm.IsPC() || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeb1d0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ADDS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever() && !rn.Is(sp)) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00900000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
      // ADDS{<c>}{<q>} {<Rd>}, SP, <Rm> {, <shift> #<amount> } ; A1
      if (rn.Is(sp) && shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x009d0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // ADDS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00900010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kAdds, &Assembler::adds, cond, size, rd, rn, operand);
}

void Assembler::adds(Register rd, const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // ADDS{<q>} <Rdn>, #<imm8> ; T2
      if (OutsideITBlock() && rd.IsLow() && (imm <= 255)) {
        EmitT32_16(0x3000 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kAdds, &Assembler::adds, rd, operand);
}

void Assembler::addw(Condition cond,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // ADDW{<c>}{<q>} <Rd>, PC, #<imm12> ; T3
      if (rn.Is(pc) && (imm <= 4095) && (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf20f0000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // ADDW{<c>}{<q>} {<Rd>}, <Rn>, #<imm12> ; T4
      if ((imm <= 4095) && ((rn.GetCode() & 0xd) != 0xd) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2000000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (imm & 0xff) | ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // ADDW{<c>}{<q>} {<Rd>}, SP, #<imm12> ; T4
      if (rn.Is(sp) && (imm <= 4095) && (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf20d0000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kAddw, &Assembler::addw, cond, rd, rn, operand);
}

void Assembler::adr(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    int32_t neg_offset = -offset;
    // ADR{<c>}{<q>} <Rd>, <label> ; T1
    if (!size.IsWide() && rd.IsLow() &&
        ((label->IsBound() && (offset >= 0) && (offset <= 1020) &&
          ((offset & 0x3) == 0)) ||
         (!label->IsBound() && size.IsNarrow()))) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(0, 1020) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= 0) && (offset <= 1020) &&
                      ((offset & 0x3) == 0));
          const int32_t target = offset >> 2;
          return instr | (target & 0xff);
        }
      } immop;
      EmitT32_16(Link(0xa000 | (rd.GetCode() << 8), label, immop));
      AdvanceIT();
      return;
    }
    // ADR{<c>}{<q>} <Rd>, <label> ; T2
    if (!size.IsNarrow() && label->IsBound() && (neg_offset > 0) &&
        (neg_offset <= 4095) && (!rd.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xf2af0000U | (rd.GetCode() << 8) | (neg_offset & 0xff) |
                 ((neg_offset & 0x700) << 4) | ((neg_offset & 0x800) << 15));
      AdvanceIT();
      return;
    }
    // ADR{<c>}{<q>} <Rd>, <label> ; T3
    if (!size.IsNarrow() &&
        (!label->IsBound() || ((offset >= 0) && (offset <= 4095))) &&
        (!rd.IsPC() || AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(0, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          int32_t target;
          if ((offset >= 0) && (offset <= 4095)) {
            target = offset;
          } else {
            target = -offset;
            VIXL_ASSERT((target >= 0) && (target <= 4095));
            // Emit the T2 encoding.
            instr |= 0x00a00000;
          }
          return instr | (target & 0xff) | ((target & 0x700) << 4) |
                 ((target & 0x800) << 15);
        }
      } immop;
      EmitT32_32(Link(0xf20f0000U | (rd.GetCode() << 8), label, immop));
      AdvanceIT();
      return;
    }
  } else {
    ImmediateA32 positive_immediate_a32(offset);
    ImmediateA32 negative_immediate_a32(-offset);
    // ADR{<c>}{<q>} <Rd>, <label> ; A1
    if ((!label->IsBound() || positive_immediate_a32.IsValid()) &&
        cond.IsNotNever()) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(0, 255) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          int32_t target;
          ImmediateA32 positive_immediate_a32(offset);
          if (positive_immediate_a32.IsValid()) {
            target = positive_immediate_a32.GetEncodingValue();
          } else {
            ImmediateA32 negative_immediate_a32(-offset);
            VIXL_ASSERT(negative_immediate_a32.IsValid());
            // Emit the A2 encoding.
            target = negative_immediate_a32.GetEncodingValue();
            instr = (instr & ~0x00f00000) | 0x00400000;
          }
          return instr | (target & 0xfff);
        }
      } immop;
      EmitA32(
          Link(0x028f0000U | (cond.GetCondition() << 28) | (rd.GetCode() << 12),
               label,
               immop));
      return;
    }
    // ADR{<c>}{<q>} <Rd>, <label> ; A2
    if (label->IsBound() && negative_immediate_a32.IsValid() &&
        cond.IsNotNever()) {
      EmitA32(0x024f0000U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              negative_immediate_a32.GetEncodingValue());
      return;
    }
  }
  Delegate(kAdr, &Assembler::adr, cond, size, rd, label);
}

void Assembler::and_(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // AND{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf0000000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // AND{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02000000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // AND<c>{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (InITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4000 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // AND{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea000000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // AND{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00000000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // AND{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00000010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kAnd, &Assembler::and_, cond, size, rd, rn, operand);
}

void Assembler::ands(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // ANDS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() && !rd.Is(pc) &&
          (!rn.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf0100000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // ANDS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02100000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // ANDS{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (OutsideITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4000 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // ANDS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) && !rd.Is(pc) &&
          ((!rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea100000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ANDS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00100000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // ANDS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00100010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kAnds, &Assembler::ands, cond, size, rd, rn, operand);
}

void Assembler::asr(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rm,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // ASR<c>{<q>} {<Rd>}, <Rm>, #<imm> ; T2
      if (InITBlock() && !size.IsWide() && rd.IsLow() && rm.IsLow() &&
          (imm >= 1) && (imm <= 32)) {
        uint32_t amount_ = imm % 32;
        EmitT32_16(0x1000 | rd.GetCode() | (rm.GetCode() << 3) |
                   (amount_ << 6));
        AdvanceIT();
        return;
      }
      // ASR{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; T3
      if (!size.IsNarrow() && (imm >= 1) && (imm <= 32) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = imm % 32;
        EmitT32_32(0xea4f0020U | (rd.GetCode() << 8) | rm.GetCode() |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ASR{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; A1
      if ((imm >= 1) && (imm <= 32) && cond.IsNotNever()) {
        uint32_t amount_ = imm % 32;
        EmitA32(0x01a00040U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rs = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // ASR<c>{<q>} {<Rdm>}, <Rdm>, <Rs> ; T1
      if (InITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          rs.IsLow()) {
        EmitT32_16(0x4100 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // ASR{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa40f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // ASR{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01a00050U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kAsr, &Assembler::asr, cond, size, rd, rm, operand);
}

void Assembler::asrs(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rm,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // ASRS{<q>} {<Rd>}, <Rm>, #<imm> ; T2
      if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && rm.IsLow() &&
          (imm >= 1) && (imm <= 32)) {
        uint32_t amount_ = imm % 32;
        EmitT32_16(0x1000 | rd.GetCode() | (rm.GetCode() << 3) |
                   (amount_ << 6));
        AdvanceIT();
        return;
      }
      // ASRS{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; T3
      if (!size.IsNarrow() && (imm >= 1) && (imm <= 32) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = imm % 32;
        EmitT32_32(0xea5f0020U | (rd.GetCode() << 8) | rm.GetCode() |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ASRS{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; A1
      if ((imm >= 1) && (imm <= 32) && cond.IsNotNever()) {
        uint32_t amount_ = imm % 32;
        EmitA32(0x01b00040U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rs = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // ASRS{<q>} {<Rdm>}, <Rdm>, <Rs> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          rs.IsLow()) {
        EmitT32_16(0x4100 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // ASRS{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa50f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // ASRS{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01b00050U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kAsrs, &Assembler::asrs, cond, size, rd, rm, operand);
}

void Assembler::b(Condition cond, EncodingSize size, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                (GetCursorOffset() + GetArchitectureStatePCOffset())
          : 0;
  if (IsUsingT32()) {
    // B<c>{<q>} <label> ; T1
    if (OutsideITBlock() && !size.IsWide() &&
        ((label->IsBound() && (offset >= -256) && (offset <= 254) &&
          ((offset & 0x1) == 0)) ||
         (!label->IsBound() && size.IsNarrow())) &&
        !cond.Is(al) && cond.IsNotNever()) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-256, 254) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - pc;
          VIXL_ASSERT((offset >= -256) && (offset <= 254) &&
                      ((offset & 0x1) == 0));
          const int32_t target = offset >> 1;
          return instr | (target & 0xff);
        }
      } immop;
      EmitT32_16(Link(0xd000 | (cond.GetCondition() << 8), label, immop));
      AdvanceIT();
      return;
    }
    // B{<c>}{<q>} <label> ; T2
    if (OutsideITBlockAndAlOrLast(cond) && !size.IsWide() &&
        ((label->IsBound() && (offset >= -2048) && (offset <= 2046) &&
          ((offset & 0x1) == 0)) ||
         (!label->IsBound() && size.IsNarrow()))) {
      CheckIT(cond);
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-2048, 2046) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - pc;
          VIXL_ASSERT((offset >= -2048) && (offset <= 2046) &&
                      ((offset & 0x1) == 0));
          const int32_t target = offset >> 1;
          return instr | (target & 0x7ff);
        }
      } immop;
      EmitT32_16(Link(0xe000, label, immop));
      AdvanceIT();
      return;
    }
    // B<c>{<q>} <label> ; T3
    if (OutsideITBlock() && !size.IsNarrow() &&
        ((label->IsBound() && (offset >= -1048576) && (offset <= 1048574) &&
          ((offset & 0x1) == 0)) ||
         !label->IsBound()) &&
        !cond.Is(al) && cond.IsNotNever()) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-1048576, 1048574) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - pc;
          VIXL_ASSERT((offset >= -1048576) && (offset <= 1048574) &&
                      ((offset & 0x1) == 0));
          const int32_t target = offset >> 1;
          return instr | (target & 0x7ff) | ((target & 0x1f800) << 5) |
                 ((target & 0x20000) >> 4) | ((target & 0x40000) >> 7) |
                 ((target & 0x80000) << 7);
        }
      } immop;
      EmitT32_32(Link(0xf0008000U | (cond.GetCondition() << 22), label, immop));
      AdvanceIT();
      return;
    }
    // B{<c>}{<q>} <label> ; T4
    if (OutsideITBlockAndAlOrLast(cond) && !size.IsNarrow() &&
        ((label->IsBound() && (offset >= -16777216) && (offset <= 16777214) &&
          ((offset & 0x1) == 0)) ||
         !label->IsBound())) {
      CheckIT(cond);
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-16777216, 16777214) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - pc;
          VIXL_ASSERT((offset >= -16777216) && (offset <= 16777214) &&
                      ((offset & 0x1) == 0));
          int32_t target = offset >> 1;
          uint32_t S = target & (1 << 23);
          target ^= ((S >> 1) | (S >> 2)) ^ (3 << 21);
          return instr | (target & 0x7ff) | ((target & 0x1ff800) << 5) |
                 ((target & 0x200000) >> 10) | ((target & 0x400000) >> 9) |
                 ((target & 0x800000) << 3);
        }
      } immop;
      EmitT32_32(Link(0xf0009000U, label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // B{<c>}{<q>} <label> ; A1
    if (((label->IsBound() && (offset >= -33554432) && (offset <= 33554428) &&
          ((offset & 0x3) == 0)) ||
         !label->IsBound()) &&
        cond.IsNotNever()) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-33554432, 33554428) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - pc;
          VIXL_ASSERT((offset >= -33554432) && (offset <= 33554428) &&
                      ((offset & 0x3) == 0));
          const int32_t target = offset >> 2;
          return instr | (target & 0xffffff);
        }
      } immop;
      EmitA32(Link(0x0a000000U | (cond.GetCondition() << 28), label, immop));
      return;
    }
  }
  Delegate(kB, &Assembler::b, cond, size, label);
}

void Assembler::bfc(Condition cond, Register rd, uint32_t lsb, uint32_t width) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // BFC{<c>}{<q>} <Rd>, #<lsb>, #<width> ; T1
    if ((lsb <= 31) && (((width >= 1) && (width <= 32 - lsb) && !rd.IsPC()) ||
                        AllowUnpredictable())) {
      uint32_t msb = lsb + width - 1;
      EmitT32_32(0xf36f0000U | (rd.GetCode() << 8) | ((lsb & 0x3) << 6) |
                 ((lsb & 0x1c) << 10) | msb);
      AdvanceIT();
      return;
    }
  } else {
    // BFC{<c>}{<q>} <Rd>, #<lsb>, #<width> ; A1
    if ((lsb <= 31) && cond.IsNotNever() &&
        (((width >= 1) && (width <= 32 - lsb) && !rd.IsPC()) ||
         AllowUnpredictable())) {
      uint32_t msb = lsb + width - 1;
      EmitA32(0x07c0001fU | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (lsb << 7) | (msb << 16));
      return;
    }
  }
  Delegate(kBfc, &Assembler::bfc, cond, rd, lsb, width);
}

void Assembler::bfi(
    Condition cond, Register rd, Register rn, uint32_t lsb, uint32_t width) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // BFI{<c>}{<q>} <Rd>, <Rn>, #<lsb>, #<width> ; T1
    if ((lsb <= 31) && !rn.Is(pc) &&
        (((width >= 1) && (width <= 32 - lsb) && !rd.IsPC()) ||
         AllowUnpredictable())) {
      uint32_t msb = lsb + width - 1;
      EmitT32_32(0xf3600000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 ((lsb & 0x3) << 6) | ((lsb & 0x1c) << 10) | msb);
      AdvanceIT();
      return;
    }
  } else {
    // BFI{<c>}{<q>} <Rd>, <Rn>, #<lsb>, #<width> ; A1
    if ((lsb <= 31) && cond.IsNotNever() && !rn.Is(pc) &&
        (((width >= 1) && (width <= 32 - lsb) && !rd.IsPC()) ||
         AllowUnpredictable())) {
      uint32_t msb = lsb + width - 1;
      EmitA32(0x07c00010U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rn.GetCode() | (lsb << 7) | (msb << 16));
      return;
    }
  }
  Delegate(kBfi, &Assembler::bfi, cond, rd, rn, lsb, width);
}

void Assembler::bic(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // BIC{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf0200000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // BIC{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03c00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // BIC<c>{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (InITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4380 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // BIC{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea200000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // BIC{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01c00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // BIC{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x01c00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kBic, &Assembler::bic, cond, size, rd, rn, operand);
}

void Assembler::bics(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // BICS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf0300000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // BICS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03d00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // BICS{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (OutsideITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4380 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // BICS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea300000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // BICS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01d00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // BICS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x01d00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kBics, &Assembler::bics, cond, size, rd, rn, operand);
}

void Assembler::bkpt(Condition cond, uint32_t imm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // BKPT{<q>} {#}<imm> ; T1
    if ((imm <= 255)) {
      EmitT32_16(0xbe00 | imm);
      AdvanceIT();
      return;
    }
  } else {
    // BKPT{<q>} {#}<imm> ; A1
    if ((imm <= 65535) && (cond.Is(al) || AllowUnpredictable())) {
      EmitA32(0x01200070U | (cond.GetCondition() << 28) | (imm & 0xf) |
              ((imm & 0xfff0) << 4));
      return;
    }
  }
  Delegate(kBkpt, &Assembler::bkpt, cond, imm);
}

void Assembler::bl(Condition cond, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                (GetCursorOffset() + GetArchitectureStatePCOffset())
          : 0;
  if (IsUsingT32()) {
    // BL{<c>}{<q>} <label> ; T1
    if (((label->IsBound() && (offset >= -16777216) && (offset <= 16777214) &&
          ((offset & 0x1) == 0)) ||
         !label->IsBound()) &&
        (OutsideITBlockAndAlOrLast(cond) || AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-16777216, 16777214) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - pc;
          VIXL_ASSERT((offset >= -16777216) && (offset <= 16777214) &&
                      ((offset & 0x1) == 0));
          int32_t target = offset >> 1;
          uint32_t S = target & (1 << 23);
          target ^= ((S >> 1) | (S >> 2)) ^ (3 << 21);
          return instr | (target & 0x7ff) | ((target & 0x1ff800) << 5) |
                 ((target & 0x200000) >> 10) | ((target & 0x400000) >> 9) |
                 ((target & 0x800000) << 3);
        }
      } immop;
      EmitT32_32(Link(0xf000d000U, label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // BL{<c>}{<q>} <label> ; A1
    if (((label->IsBound() && (offset >= -33554432) && (offset <= 33554428) &&
          ((offset & 0x3) == 0)) ||
         !label->IsBound()) &&
        cond.IsNotNever()) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-33554432, 33554428) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - pc;
          VIXL_ASSERT((offset >= -33554432) && (offset <= 33554428) &&
                      ((offset & 0x3) == 0));
          const int32_t target = offset >> 2;
          return instr | (target & 0xffffff);
        }
      } immop;
      EmitA32(Link(0x0b000000U | (cond.GetCondition() << 28), label, immop));
      return;
    }
  }
  Delegate(kBl, &Assembler::bl, cond, label);
}

void Assembler::blx(Condition cond, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // BLX{<c>}{<q>} <label> ; T2
    if (((label->IsBound() && (offset >= -16777216) && (offset <= 16777212) &&
          ((offset & 0x3) == 0)) ||
         !label->IsBound()) &&
        (OutsideITBlockAndAlOrLast(cond) || AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-16777216, 16777212) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -16777216) && (offset <= 16777212) &&
                      ((offset & 0x3) == 0));
          int32_t target = offset >> 2;
          uint32_t S = target & (1 << 22);
          target ^= ((S >> 1) | (S >> 2)) ^ (3 << 20);
          return instr | ((target & 0x3ff) << 1) | ((target & 0xffc00) << 6) |
                 ((target & 0x100000) >> 9) | ((target & 0x200000) >> 8) |
                 ((target & 0x400000) << 4);
        }
      } immop;
      EmitT32_32(Link(0xf000c000U, label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // BLX{<c>}{<q>} <label> ; A2
    if (((label->IsBound() && (offset >= -33554432) && (offset <= 33554430) &&
          ((offset & 0x1) == 0)) ||
         !label->IsBound())) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        static class EmitOp : public Label::LabelEmitOperator {
         public:
          EmitOp() : Label::LabelEmitOperator(-33554432, 33554430) {}
          virtual uint32_t Encode(uint32_t instr,
                                  Label::Offset pc,
                                  const Label* label) const VIXL_OVERRIDE {
            Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
            VIXL_ASSERT((offset >= -33554432) && (offset <= 33554430) &&
                        ((offset & 0x1) == 0));
            const int32_t target = offset >> 1;
            return instr | ((target & 0x1) << 24) | ((target & 0x1fffffe) >> 1);
          }
        } immop;
        EmitA32(Link(0xfa000000U, label, immop));
        return;
      }
    }
  }
  Delegate(kBlx, &Assembler::blx, cond, label);
}

void Assembler::blx(Condition cond, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // BLX{<c>}{<q>} <Rm> ; T1
    if (((!rm.IsPC() && OutsideITBlockAndAlOrLast(cond)) ||
         AllowUnpredictable())) {
      EmitT32_16(0x4780 | (rm.GetCode() << 3));
      AdvanceIT();
      return;
    }
  } else {
    // BLX{<c>}{<q>} <Rm> ; A1
    if (cond.IsNotNever() && (!rm.IsPC() || AllowUnpredictable())) {
      EmitA32(0x012fff30U | (cond.GetCondition() << 28) | rm.GetCode());
      return;
    }
  }
  Delegate(kBlx, &Assembler::blx, cond, rm);
}

void Assembler::bx(Condition cond, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // BX{<c>}{<q>} <Rm> ; T1
    if ((OutsideITBlockAndAlOrLast(cond) || AllowUnpredictable())) {
      EmitT32_16(0x4700 | (rm.GetCode() << 3));
      AdvanceIT();
      return;
    }
  } else {
    // BX{<c>}{<q>} <Rm> ; A1
    if (cond.IsNotNever()) {
      EmitA32(0x012fff10U | (cond.GetCondition() << 28) | rm.GetCode());
      return;
    }
  }
  Delegate(kBx, &Assembler::bx, cond, rm);
}

void Assembler::bxj(Condition cond, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // BXJ{<c>}{<q>} <Rm> ; T1
    if (((!rm.IsPC() && OutsideITBlockAndAlOrLast(cond)) ||
         AllowUnpredictable())) {
      EmitT32_32(0xf3c08f00U | (rm.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // BXJ{<c>}{<q>} <Rm> ; A1
    if (cond.IsNotNever() && (!rm.IsPC() || AllowUnpredictable())) {
      EmitA32(0x012fff20U | (cond.GetCondition() << 28) | rm.GetCode());
      return;
    }
  }
  Delegate(kBxj, &Assembler::bxj, cond, rm);
}

void Assembler::cbnz(Register rn, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                (GetCursorOffset() + GetArchitectureStatePCOffset())
          : 0;
  if (IsUsingT32()) {
    // CBNZ{<q>} <Rn>, <label> ; T1
    if (rn.IsLow() && ((label->IsBound() && (offset >= 0) && (offset <= 126) &&
                        ((offset & 0x1) == 0)) ||
                       !label->IsBound())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(0, 126) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - pc;
          VIXL_ASSERT((offset >= 0) && (offset <= 126) &&
                      ((offset & 0x1) == 0));
          const int32_t target = offset >> 1;
          return instr | ((target & 0x1f) << 3) | ((target & 0x20) << 4);
        }
      } immop;
      EmitT32_16(Link(0xb900 | rn.GetCode(), label, immop));
      AdvanceIT();
      return;
    }
  }
  Delegate(kCbnz, &Assembler::cbnz, rn, label);
}

void Assembler::cbz(Register rn, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                (GetCursorOffset() + GetArchitectureStatePCOffset())
          : 0;
  if (IsUsingT32()) {
    // CBZ{<q>} <Rn>, <label> ; T1
    if (rn.IsLow() && ((label->IsBound() && (offset >= 0) && (offset <= 126) &&
                        ((offset & 0x1) == 0)) ||
                       !label->IsBound())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(0, 126) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - pc;
          VIXL_ASSERT((offset >= 0) && (offset <= 126) &&
                      ((offset & 0x1) == 0));
          const int32_t target = offset >> 1;
          return instr | ((target & 0x1f) << 3) | ((target & 0x20) << 4);
        }
      } immop;
      EmitT32_16(Link(0xb100 | rn.GetCode(), label, immop));
      AdvanceIT();
      return;
    }
  }
  Delegate(kCbz, &Assembler::cbz, rn, label);
}

void Assembler::clrex(Condition cond) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // CLREX{<c>}{<q>} ; T1
    EmitT32_32(0xf3bf8f2fU);
    AdvanceIT();
    return;
  } else {
    // CLREX{<c>}{<q>} ; A1
    if (cond.Is(al)) {
      EmitA32(0xf57ff01fU);
      return;
    }
  }
  Delegate(kClrex, &Assembler::clrex, cond);
}

void Assembler::clz(Condition cond, Register rd, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // CLZ{<c>}{<q>} <Rd>, <Rm> ; T1
    if (((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfab0f080U | (rd.GetCode() << 8) | rm.GetCode() |
                 (rm.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // CLZ{<c>}{<q>} <Rd>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x016f0f10U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode());
      return;
    }
  }
  Delegate(kClz, &Assembler::clz, cond, rd, rm);
}

void Assembler::cmn(Condition cond,
                    EncodingSize size,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // CMN{<c>}{<q>} <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf1100f00U | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // CMN{<c>}{<q>} <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03700000U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // CMN{<c>}{<q>} <Rn>, <Rm> ; T1
        if (!size.IsWide() && rn.IsLow() && rm.IsLow()) {
          EmitT32_16(0x42c0 | rn.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // CMN{<c>}{<q>} <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeb100f00U | (rn.GetCode() << 16) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // CMN{<c>}{<q>} <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01700000U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // CMN{<c>}{<q>} <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rn.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01700010U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | rm.GetCode() | (shift.GetType() << 5) |
                (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kCmn, &Assembler::cmn, cond, size, rn, operand);
}

void Assembler::cmp(Condition cond,
                    EncodingSize size,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // CMP{<c>}{<q>} <Rn>, #<imm8> ; T1
      if (!size.IsWide() && rn.IsLow() && (imm <= 255)) {
        EmitT32_16(0x2800 | (rn.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
      // CMP{<c>}{<q>} <Rn>, #<const> ; T2
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf1b00f00U | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // CMP{<c>}{<q>} <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03500000U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // CMP{<c>}{<q>} <Rn>, <Rm> ; T1
        if (!size.IsWide() && rn.IsLow() && rm.IsLow()) {
          EmitT32_16(0x4280 | rn.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
        // CMP{<c>}{<q>} <Rn>, <Rm> ; T2
        if (!size.IsWide() &&
            ((!rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
          EmitT32_16(0x4500 | (rn.GetCode() & 0x7) |
                     ((rn.GetCode() & 0x8) << 4) | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // CMP{<c>}{<q>} <Rn>, <Rm>, <shift> #<amount> ; T3
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xebb00f00U | (rn.GetCode() << 16) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // CMP{<c>}{<q>} <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01500000U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // CMP{<c>}{<q>} <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rn.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01500010U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | rm.GetCode() | (shift.GetType() << 5) |
                (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kCmp, &Assembler::cmp, cond, size, rn, operand);
}

void Assembler::crc32b(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // CRC32B{<q>} <Rd>, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfac0f080U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // CRC32B{<q>} <Rd>, <Rn>, <Rm> ; A1
    if ((cond.Is(al) || AllowUnpredictable()) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01000040U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kCrc32b, &Assembler::crc32b, cond, rd, rn, rm);
}

void Assembler::crc32cb(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // CRC32CB{<q>} <Rd>, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfad0f080U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // CRC32CB{<q>} <Rd>, <Rn>, <Rm> ; A1
    if ((cond.Is(al) || AllowUnpredictable()) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01000240U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kCrc32cb, &Assembler::crc32cb, cond, rd, rn, rm);
}

void Assembler::crc32ch(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // CRC32CH{<q>} <Rd>, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfad0f090U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // CRC32CH{<q>} <Rd>, <Rn>, <Rm> ; A1
    if ((cond.Is(al) || AllowUnpredictable()) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01200240U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kCrc32ch, &Assembler::crc32ch, cond, rd, rn, rm);
}

void Assembler::crc32cw(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // CRC32CW{<q>} <Rd>, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfad0f0a0U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // CRC32CW{<q>} <Rd>, <Rn>, <Rm> ; A1
    if ((cond.Is(al) || AllowUnpredictable()) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01400240U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kCrc32cw, &Assembler::crc32cw, cond, rd, rn, rm);
}

void Assembler::crc32h(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // CRC32H{<q>} <Rd>, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfac0f090U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // CRC32H{<q>} <Rd>, <Rn>, <Rm> ; A1
    if ((cond.Is(al) || AllowUnpredictable()) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01200040U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kCrc32h, &Assembler::crc32h, cond, rd, rn, rm);
}

void Assembler::crc32w(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // CRC32W{<q>} <Rd>, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfac0f0a0U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // CRC32W{<q>} <Rd>, <Rn>, <Rm> ; A1
    if ((cond.Is(al) || AllowUnpredictable()) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01400040U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kCrc32w, &Assembler::crc32w, cond, rd, rn, rm);
}

void Assembler::dmb(Condition cond, MemoryBarrier option) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // DMB{<c>}{<q>} {<option>} ; T1
    EmitT32_32(0xf3bf8f50U | option.GetType());
    AdvanceIT();
    return;
  } else {
    // DMB{<c>}{<q>} {<option>} ; A1
    if (cond.Is(al)) {
      EmitA32(0xf57ff050U | option.GetType());
      return;
    }
  }
  Delegate(kDmb, &Assembler::dmb, cond, option);
}

void Assembler::dsb(Condition cond, MemoryBarrier option) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // DSB{<c>}{<q>} {<option>} ; T1
    EmitT32_32(0xf3bf8f40U | option.GetType());
    AdvanceIT();
    return;
  } else {
    // DSB{<c>}{<q>} {<option>} ; A1
    if (cond.Is(al)) {
      EmitA32(0xf57ff040U | option.GetType());
      return;
    }
  }
  Delegate(kDsb, &Assembler::dsb, cond, option);
}

void Assembler::eor(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // EOR{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf0800000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // EOR{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02200000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // EOR<c>{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (InITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4040 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // EOR{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea800000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // EOR{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00200000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // EOR{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00200010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kEor, &Assembler::eor, cond, size, rd, rn, operand);
}

void Assembler::eors(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // EORS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() && !rd.Is(pc) &&
          (!rn.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf0900000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // EORS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02300000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // EORS{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (OutsideITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4040 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // EORS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) && !rd.Is(pc) &&
          ((!rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea900000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // EORS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00300000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // EORS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00300010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kEors, &Assembler::eors, cond, size, rd, rn, operand);
}

void Assembler::fldmdbx(Condition cond,
                        Register rn,
                        WriteBack write_back,
                        DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // FLDMDBX{<c>}{<q>} <Rn>!, <dreglist> ; T1
    if (write_back.DoesWriteBack() &&
        (((dreglist.GetLength() <= 16) &&
          (dreglist.GetLastDRegister().GetCode() < 16)) ||
         AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xed300b01U | (rn.GetCode() << 16) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // FLDMDBX{<c>}{<q>} <Rn>!, <dreglist> ; A1
    if (write_back.DoesWriteBack() && cond.IsNotNever() &&
        (((dreglist.GetLength() <= 16) &&
          (dreglist.GetLastDRegister().GetCode() < 16)) ||
         AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0d300b01U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              dreg.Encode(22, 12) | (len & 0xff));
      return;
    }
  }
  Delegate(kFldmdbx, &Assembler::fldmdbx, cond, rn, write_back, dreglist);
}

void Assembler::fldmiax(Condition cond,
                        Register rn,
                        WriteBack write_back,
                        DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // FLDMIAX{<c>}{<q>} <Rn>{!}, <dreglist> ; T1
    if ((((dreglist.GetLength() <= 16) &&
          (dreglist.GetLastDRegister().GetCode() < 16)) ||
         AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xec900b01U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // FLDMIAX{<c>}{<q>} <Rn>{!}, <dreglist> ; A1
    if (cond.IsNotNever() && (((dreglist.GetLength() <= 16) &&
                               (dreglist.GetLastDRegister().GetCode() < 16)) ||
                              AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0c900b01U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kFldmiax, &Assembler::fldmiax, cond, rn, write_back, dreglist);
}

void Assembler::fstmdbx(Condition cond,
                        Register rn,
                        WriteBack write_back,
                        DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // FSTMDBX{<c>}{<q>} <Rn>!, <dreglist> ; T1
    if (write_back.DoesWriteBack() &&
        (((dreglist.GetLength() <= 16) &&
          (dreglist.GetLastDRegister().GetCode() < 16)) ||
         AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xed200b01U | (rn.GetCode() << 16) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // FSTMDBX{<c>}{<q>} <Rn>!, <dreglist> ; A1
    if (write_back.DoesWriteBack() && cond.IsNotNever() &&
        (((dreglist.GetLength() <= 16) &&
          (dreglist.GetLastDRegister().GetCode() < 16)) ||
         AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0d200b01U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              dreg.Encode(22, 12) | (len & 0xff));
      return;
    }
  }
  Delegate(kFstmdbx, &Assembler::fstmdbx, cond, rn, write_back, dreglist);
}

void Assembler::fstmiax(Condition cond,
                        Register rn,
                        WriteBack write_back,
                        DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // FSTMIAX{<c>}{<q>} <Rn>{!}, <dreglist> ; T1
    if ((((dreglist.GetLength() <= 16) &&
          (dreglist.GetLastDRegister().GetCode() < 16)) ||
         AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xec800b01U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // FSTMIAX{<c>}{<q>} <Rn>{!}, <dreglist> ; A1
    if (cond.IsNotNever() && (((dreglist.GetLength() <= 16) &&
                               (dreglist.GetLastDRegister().GetCode() < 16)) ||
                              AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0c800b01U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kFstmiax, &Assembler::fstmiax, cond, rn, write_back, dreglist);
}

void Assembler::hlt(Condition cond, uint32_t imm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // HLT{<q>} {#}<imm> ; T1
    if ((imm <= 63)) {
      EmitT32_16(0xba80 | imm);
      AdvanceIT();
      return;
    }
  } else {
    // HLT{<q>} {#}<imm> ; A1
    if ((imm <= 65535) && (cond.Is(al) || AllowUnpredictable())) {
      EmitA32(0x01000070U | (cond.GetCondition() << 28) | (imm & 0xf) |
              ((imm & 0xfff0) << 4));
      return;
    }
  }
  Delegate(kHlt, &Assembler::hlt, cond, imm);
}

void Assembler::hvc(Condition cond, uint32_t imm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // HVC{<q>} {#}<imm16> ; T1
    if ((imm <= 65535)) {
      EmitT32_32(0xf7e08000U | (imm & 0xfff) | ((imm & 0xf000) << 4));
      AdvanceIT();
      return;
    }
  } else {
    // HVC{<q>} {#}<imm16> ; A1
    if ((imm <= 65535) && (cond.Is(al) || AllowUnpredictable())) {
      EmitA32(0x01400070U | (cond.GetCondition() << 28) | (imm & 0xf) |
              ((imm & 0xfff0) << 4));
      return;
    }
  }
  Delegate(kHvc, &Assembler::hvc, cond, imm);
}

void Assembler::isb(Condition cond, MemoryBarrier option) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // ISB{<c>}{<q>} {<option>} ; T1
    EmitT32_32(0xf3bf8f60U | option.GetType());
    AdvanceIT();
    return;
  } else {
    // ISB{<c>}{<q>} {<option>} ; A1
    if (cond.Is(al)) {
      EmitA32(0xf57ff060U | option.GetType());
      return;
    }
  }
  Delegate(kIsb, &Assembler::isb, cond, option);
}

void Assembler::it(Condition cond, uint16_t mask) {
  VIXL_ASSERT(AllowAssembler());
  CheckNotIT();
  if (mask != 0) {
    if ((cond.GetCondition() & 0x1) != 0) {
      if ((mask & 0x1) != 0) {
        mask ^= 0xE;
      } else if ((mask & 0x2) != 0) {
        mask ^= 0xC;
      } else if ((mask & 0x4) != 0) {
        mask ^= 0x8;
      }
    }
    if (IsUsingT32()) EmitT32_16(0xbf00 | (cond.GetCondition() << 4) | mask);
    SetIT(cond, mask);
    return;
  }
  DelegateIt(cond, mask);
}

void Assembler::lda(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDA{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d00fafU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDA{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01900c9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLda, &Assembler::lda, cond, rt, operand);
}

void Assembler::ldab(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDAB{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d00f8fU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDAB{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01d00c9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdab, &Assembler::ldab, cond, rt, operand);
}

void Assembler::ldaex(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDAEX{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d00fefU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDAEX{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01900e9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdaex, &Assembler::ldaex, cond, rt, operand);
}

void Assembler::ldaexb(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDAEXB{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d00fcfU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDAEXB{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01d00e9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdaexb, &Assembler::ldaexb, cond, rt, operand);
}

void Assembler::ldaexd(Condition cond,
                       Register rt,
                       Register rt2,
                       const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDAEXD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rt2.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d000ffU | (rt.GetCode() << 12) | (rt2.GetCode() << 8) |
                   (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDAEXD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>] ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsOffset() && cond.IsNotNever() &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC() && !rn.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x01b00e9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdaexd, &Assembler::ldaexd, cond, rt, rt2, operand);
}

void Assembler::ldaexh(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDAEXH{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d00fdfU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDAEXH{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01f00e9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdaexh, &Assembler::ldaexh, cond, rt, operand);
}

void Assembler::ldah(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDAH{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d00f9fU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDAH{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01f00c9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdah, &Assembler::ldah, cond, rt, operand);
}

void Assembler::ldm(Condition cond,
                    EncodingSize size,
                    Register rn,
                    WriteBack write_back,
                    RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // LDM{<c>}{<q>} <Rn>{!}, <registers> ; T1
    if (!size.IsWide() && rn.IsLow() &&
        (((registers.GetList() & (1 << rn.GetCode())) == 0) ==
         write_back.DoesWriteBack()) &&
        ((registers.GetList() & ~0xff) == 0)) {
      EmitT32_16(0xc800 | (rn.GetCode() << 8) |
                 GetRegisterListEncoding(registers, 0, 8));
      AdvanceIT();
      return;
    }
    // LDM{<c>}{<q>} SP!, <registers> ; T1
    if (!size.IsWide() && rn.Is(sp) && write_back.DoesWriteBack() &&
        ((registers.GetList() & ~0x80ff) == 0)) {
      EmitT32_16(0xbc00 | (GetRegisterListEncoding(registers, 15, 1) << 8) |
                 GetRegisterListEncoding(registers, 0, 8));
      AdvanceIT();
      return;
    }
    // LDM{<c>}{<q>} <Rn>{!}, <registers> ; T2
    if (!size.IsNarrow() && ((registers.GetList() & ~0xdfff) == 0) &&
        (!rn.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe8900000U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) |
                 (GetRegisterListEncoding(registers, 15, 1) << 15) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // LDM{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x08900000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kLdm, &Assembler::ldm, cond, size, rn, write_back, registers);
}

void Assembler::ldmda(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // LDMDA{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x08100000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kLdmda, &Assembler::ldmda, cond, rn, write_back, registers);
}

void Assembler::ldmdb(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // LDMDB{<c>}{<q>} <Rn>{!}, <registers> ; T1
    if (((registers.GetList() & ~0xdfff) == 0) &&
        (!rn.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe9100000U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) |
                 (GetRegisterListEncoding(registers, 15, 1) << 15) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // LDMDB{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x09100000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kLdmdb, &Assembler::ldmdb, cond, rn, write_back, registers);
}

void Assembler::ldmea(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // LDMEA{<c>}{<q>} <Rn>{!}, <registers> ; T1
    if (((registers.GetList() & ~0xdfff) == 0) &&
        (!rn.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe9100000U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) |
                 (GetRegisterListEncoding(registers, 15, 1) << 15) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // LDMEA{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x09100000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kLdmea, &Assembler::ldmea, cond, rn, write_back, registers);
}

void Assembler::ldmed(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // LDMED{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x09900000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kLdmed, &Assembler::ldmed, cond, rn, write_back, registers);
}

void Assembler::ldmfa(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // LDMFA{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x08100000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kLdmfa, &Assembler::ldmfa, cond, rn, write_back, registers);
}

void Assembler::ldmfd(Condition cond,
                      EncodingSize size,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // LDMFD{<c>}{<q>} <Rn>{!}, <registers> ; T1
    if (!size.IsWide() && rn.IsLow() &&
        (((registers.GetList() & (1 << rn.GetCode())) == 0) ==
         write_back.DoesWriteBack()) &&
        ((registers.GetList() & ~0xff) == 0)) {
      EmitT32_16(0xc800 | (rn.GetCode() << 8) |
                 GetRegisterListEncoding(registers, 0, 8));
      AdvanceIT();
      return;
    }
    // LDMFD{<c>}{<q>} <Rn>{!}, <registers> ; T2
    if (!size.IsNarrow() && ((registers.GetList() & ~0xdfff) == 0) &&
        (!rn.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe8900000U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) |
                 (GetRegisterListEncoding(registers, 15, 1) << 15) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // LDMFD{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x08900000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kLdmfd, &Assembler::ldmfd, cond, size, rn, write_back, registers);
}

void Assembler::ldmib(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // LDMIB{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x09900000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kLdmib, &Assembler::ldmib, cond, rn, write_back, registers);
}

void Assembler::ldr(Condition cond,
                    EncodingSize size,
                    Register rt,
                    const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // LDR{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm>}] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && (offset >= 0) &&
          (offset <= 124) && ((offset % 4) == 0) && operand.IsOffset()) {
        int32_t offset_ = offset >> 2;
        EmitT32_16(0x6800 | rt.GetCode() | (rn.GetCode() << 3) |
                   ((offset_ & 0x1f) << 6));
        AdvanceIT();
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [SP{, #{+}<imm>}] ; T2
      if (!size.IsWide() && rt.IsLow() && (offset >= 0) && (offset <= 1020) &&
          ((offset % 4) == 0) && rn.Is(sp) && operand.IsOffset()) {
        int32_t offset_ = offset >> 2;
        EmitT32_16(0x9800 | (rt.GetCode() << 8) | (offset_ & 0xff));
        AdvanceIT();
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm_1>}] ; T3
      if (!size.IsNarrow() && (offset >= 0) && (offset <= 4095) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() || OutsideITBlockAndAlOrLast(cond)) ||
           AllowUnpredictable())) {
        EmitT32_32(0xf8d00000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [<Rn>{, #-<imm_2>}] ; T4
      if (!size.IsNarrow() && (-offset >= 0) && (-offset <= 255) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() || OutsideITBlockAndAlOrLast(cond)) ||
           AllowUnpredictable())) {
        EmitT32_32(0xf8500c00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (-offset & 0xff));
        AdvanceIT();
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_2> ; T4
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() || OutsideITBlockAndAlOrLast(cond)) ||
           AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8500900U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}]! ; T4
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() || OutsideITBlockAndAlOrLast(cond)) ||
           AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8500d00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm>] ; T2
      if (!size.IsNarrow() && (offset >= -4095) && (offset <= 4095) &&
          rn.Is(pc) && operand.IsOffset() &&
          ((!rt.IsPC() || OutsideITBlockAndAlOrLast(cond)) ||
           AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf85f0000U | (rt.GetCode() << 12) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // LDR{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}] ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsOffset() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x05100000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_3> ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsPostIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x04100000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}]! ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsPreIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x05300000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm_1>] ; A1
      if ((offset >= -4095) && (offset <= 4095) && rn.Is(pc) &&
          operand.IsOffset() && cond.IsNotNever()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x051f0000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | offset_ | (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingT32()) {
      // LDR{<c>}{<q>} <Rt>, [<Rn>, #{+}<Rm>] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && rm.IsLow() &&
          sign.IsPlus() && operand.IsOffset()) {
        EmitT32_16(0x5800 | rt.GetCode() | (rn.GetCode() << 3) |
                   (rm.GetCode() << 6));
        AdvanceIT();
        return;
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // LDR{<c>}{<q>} <Rt>, [<Rn>, {+}<Rm>{, LSL #<imm>}] ; T2
      if (!size.IsNarrow() && sign.IsPlus() && shift.IsLSL() && (amount <= 3) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rm.IsPC() && (!rt.IsPC() || OutsideITBlockAndAlOrLast(cond))) ||
           AllowUnpredictable())) {
        EmitT32_32(0xf8500000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount << 4));
        AdvanceIT();
        return;
      }
    } else {
      // LDR{<c>}{<q>} <Rt>, [<Rn>, {+/-}<Rm>{, <shift>}] ; A1
      if (operand.IsShiftValid() && operand.IsOffset() && cond.IsNotNever() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x07100000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [<Rn>], {+/-}<Rm>{, <shift>} ; A1
      if (operand.IsShiftValid() && operand.IsPostIndex() &&
          cond.IsNotNever() && (!rm.IsPC() || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x06100000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
      // LDR{<c>}{<q>} <Rt>, [<Rn>, {+/-}<Rm>{, <shift>}]! ; A1
      if (operand.IsShiftValid() && operand.IsPreIndex() && cond.IsNotNever() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x07300000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
    }
  }
  Delegate(kLdr, &Assembler::ldr, cond, size, rt, operand);
}

void Assembler::ldr(Condition cond,
                    EncodingSize size,
                    Register rt,
                    Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // LDR{<c>}{<q>} <Rt>, <label> ; T1
    if (!size.IsWide() && rt.IsLow() &&
        ((label->IsBound() && (offset >= 0) && (offset <= 1020) &&
          ((offset & 0x3) == 0)) ||
         (!label->IsBound() && size.IsNarrow()))) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(0, 1020) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= 0) && (offset <= 1020) &&
                      ((offset & 0x3) == 0));
          const int32_t target = offset >> 2;
          return instr | (target & 0xff);
        }
      } immop;
      EmitT32_16(Link(0x4800 | (rt.GetCode() << 8), label, immop));
      AdvanceIT();
      return;
    }
    // LDR{<c>}{<q>} <Rt>, <label> ; T2
    if (!size.IsNarrow() &&
        ((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound()) &&
        ((!rt.IsPC() || OutsideITBlockAndAlOrLast(cond)) ||
         AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 12);
          return instr | (target & 0xfff) | ((target & 0x1000) << 11);
        }
      } immop;
      EmitT32_32(Link(0xf85f0000U | (rt.GetCode() << 12), label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // LDR{<c>}{<q>} <Rt>, <label> ; A1
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound()) &&
        cond.IsNotNever()) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 12);
          return instr | (target & 0xfff) | ((target & 0x1000) << 11);
        }
      } immop;
      EmitA32(
          Link(0x051f0000U | (cond.GetCondition() << 28) | (rt.GetCode() << 12),
               label,
               immop));
      return;
    }
  }
  Delegate(kLdr, &Assembler::ldr, cond, size, rt, label);
}

void Assembler::ldrb(Condition cond,
                     EncodingSize size,
                     Register rt,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // LDRB{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm>}] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && (offset >= 0) &&
          (offset <= 31) && operand.IsOffset()) {
        EmitT32_16(0x7800 | rt.GetCode() | (rn.GetCode() << 3) |
                   ((offset & 0x1f) << 6));
        AdvanceIT();
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm_1>}] ; T2
      if (!size.IsNarrow() && (offset >= 0) && (offset <= 4095) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc)) {
        EmitT32_32(0xf8900000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [<Rn>{, #-<imm_2>}] ; T3
      if (!size.IsNarrow() && (-offset >= 0) && (-offset <= 255) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc)) {
        EmitT32_32(0xf8100c00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (-offset & 0xff));
        AdvanceIT();
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_2> ; T3
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8100900U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}]! ; T3
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8100d00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm>] ; T1
      if (!size.IsNarrow() && (offset >= -4095) && (offset <= 4095) &&
          rn.Is(pc) && operand.IsOffset() && !rt.Is(pc)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf81f0000U | (rt.GetCode() << 12) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // LDRB{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}] ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsOffset() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x05500000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_3> ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsPostIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x04500000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}]! ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsPreIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x05700000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm_1>] ; A1
      if ((offset >= -4095) && (offset <= 4095) && rn.Is(pc) &&
          operand.IsOffset() && cond.IsNotNever() &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x055f0000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | offset_ | (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingT32()) {
      // LDRB{<c>}{<q>} <Rt>, [<Rn>, #{+}<Rm>] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && rm.IsLow() &&
          sign.IsPlus() && operand.IsOffset()) {
        EmitT32_16(0x5c00 | rt.GetCode() | (rn.GetCode() << 3) |
                   (rm.GetCode() << 6));
        AdvanceIT();
        return;
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // LDRB{<c>}{<q>} <Rt>, [<Rn>, {+}<Rm>{, LSL #<imm>}] ; T2
      if (!size.IsNarrow() && sign.IsPlus() && shift.IsLSL() && (amount <= 3) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc) &&
          (!rm.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf8100000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount << 4));
        AdvanceIT();
        return;
      }
    } else {
      // LDRB{<c>}{<q>} <Rt>, [<Rn>, {+/-}<Rm>{, <shift>}] ; A1
      if (operand.IsShiftValid() && operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x07500000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [<Rn>], {+/-}<Rm>{, <shift>} ; A1
      if (operand.IsShiftValid() && operand.IsPostIndex() &&
          cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x06500000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
      // LDRB{<c>}{<q>} <Rt>, [<Rn>, {+/-}<Rm>{, <shift>}]! ; A1
      if (operand.IsShiftValid() && operand.IsPreIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x07700000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
    }
  }
  Delegate(kLdrb, &Assembler::ldrb, cond, size, rt, operand);
}

void Assembler::ldrb(Condition cond, Register rt, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // LDRB{<c>}{<q>} <Rt>, <label> ; T1
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound()) &&
        !rt.Is(pc)) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 12);
          return instr | (target & 0xfff) | ((target & 0x1000) << 11);
        }
      } immop;
      EmitT32_32(Link(0xf81f0000U | (rt.GetCode() << 12), label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // LDRB{<c>}{<q>} <Rt>, <label> ; A1
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound()) &&
        cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 12);
          return instr | (target & 0xfff) | ((target & 0x1000) << 11);
        }
      } immop;
      EmitA32(
          Link(0x055f0000U | (cond.GetCondition() << 28) | (rt.GetCode() << 12),
               label,
               immop));
      return;
    }
  }
  Delegate(kLdrb, &Assembler::ldrb, cond, rt, label);
}

void Assembler::ldrd(Condition cond,
                     Register rt,
                     Register rt2,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>{, #{+/-}<imm>}] ; T1
      if ((offset >= -1020) && (offset <= 1020) && ((offset % 4) == 0) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xe9500000U | (rt.GetCode() << 12) | (rt2.GetCode() << 8) |
                   (rn.GetCode() << 16) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>], #{+/-}<imm> ; T1
      if ((offset >= -1020) && (offset <= 1020) && ((offset % 4) == 0) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xe8700000U | (rt.GetCode() << 12) | (rt2.GetCode() << 8) |
                   (rn.GetCode() << 16) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>{, #{+/-}<imm>}]! ; T1
      if ((offset >= -1020) && (offset <= 1020) && ((offset % 4) == 0) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xe9700000U | (rt.GetCode() << 12) | (rt2.GetCode() << 8) |
                   (rn.GetCode() << 16) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [PC, #<_plusminus_><imm>] ; T1
      if ((offset >= -255) && (offset <= 255) && rn.Is(pc) &&
          operand.IsOffset() &&
          ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xe95f0000U | (rt.GetCode() << 12) | (rt2.GetCode() << 8) |
                   offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>{, #{+/-}<imm_1>}] ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          (offset >= -255) && (offset <= 255) && operand.IsOffset() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x014000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>], #{+/-}<imm_1> ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          (offset >= -255) && (offset <= 255) && operand.IsPostIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x004000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>{, #{+/-}<imm_1>}]! ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          (offset >= -255) && (offset <= 255) && operand.IsPreIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x016000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [PC, #<_plusminus_><imm_1>] ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          (offset >= -255) && (offset <= 255) && rn.Is(pc) &&
          operand.IsOffset() && cond.IsNotNever() &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x014f00d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingA32()) {
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>, #{+/-}<Rm>] ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsOffset() && cond.IsNotNever() &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC() && !rm.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x010000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>], #{+/-}<Rm> ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsPostIndex() && cond.IsNotNever() &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC() && !rm.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x000000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // LDRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>, #{+/-}<Rm>]! ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsPreIndex() && cond.IsNotNever() &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC() && !rm.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x012000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
    }
  }
  Delegate(kLdrd, &Assembler::ldrd, cond, rt, rt2, operand);
}

void Assembler::ldrd(Condition cond, Register rt, Register rt2, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // LDRD{<c>}{<q>} <Rt>, <Rt2>, <label> ; T1
    if (((label->IsBound() && (offset >= -1020) && (offset <= 1020) &&
          ((offset & 0x3) == 0)) ||
         !label->IsBound()) &&
        ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-1020, 1020) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -1020) && (offset <= 1020) &&
                      ((offset & 0x3) == 0));
          int32_t target = offset >> 2;
          uint32_t U = (target >= 0);
          target = abs(target) | (U << 8);
          return instr | (target & 0xff) | ((target & 0x100) << 15);
        }
      } immop;
      EmitT32_32(Link(0xe95f0000U | (rt.GetCode() << 12) | (rt2.GetCode() << 8),
                      label,
                      immop));
      AdvanceIT();
      return;
    }
  } else {
    // LDRD{<c>}{<q>} <Rt>, <Rt2>, <label> ; A1
    if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
        ((label->IsBound() && (offset >= -255) && (offset <= 255)) ||
         !label->IsBound()) &&
        cond.IsNotNever() &&
        ((((rt.GetCode() & 1) == 0) && !rt2.IsPC()) || AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-255, 255) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -255) && (offset <= 255));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 8);
          return instr | (target & 0xf) | ((target & 0xf0) << 4) |
                 ((target & 0x100) << 15);
        }
      } immop;
      EmitA32(
          Link(0x014f00d0U | (cond.GetCondition() << 28) | (rt.GetCode() << 12),
               label,
               immop));
      return;
    }
  }
  Delegate(kLdrd, &Assembler::ldrd, cond, rt, rt2, label);
}

void Assembler::ldrex(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // LDREX{<c>}{<q>} <Rt>, [<Rn>{, #<imm>}] ; T1
      if ((offset >= 0) && (offset <= 1020) && ((offset % 4) == 0) &&
          operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        int32_t offset_ = offset >> 2;
        EmitT32_32(0xe8500f00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (offset_ & 0xff));
        AdvanceIT();
        return;
      }
    } else {
      // LDREX{<c>}{<q>} <Rt>, [<Rn>{, #<imm_1>}] ; A1
      if ((offset == 0) && operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01900f9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdrex, &Assembler::ldrex, cond, rt, operand);
}

void Assembler::ldrexb(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDREXB{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d00f4fU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDREXB{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01d00f9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdrexb, &Assembler::ldrexb, cond, rt, operand);
}

void Assembler::ldrexd(Condition cond,
                       Register rt,
                       Register rt2,
                       const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDREXD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rt2.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d0007fU | (rt.GetCode() << 12) | (rt2.GetCode() << 8) |
                   (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDREXD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>] ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsOffset() && cond.IsNotNever() &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC() && !rn.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x01b00f9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdrexd, &Assembler::ldrexd, cond, rt, rt2, operand);
}

void Assembler::ldrexh(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LDREXH{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8d00f5fU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // LDREXH{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01f00f9fU | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kLdrexh, &Assembler::ldrexh, cond, rt, operand);
}

void Assembler::ldrh(Condition cond,
                     EncodingSize size,
                     Register rt,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // LDRH{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm>}] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && (offset >= 0) &&
          (offset <= 62) && ((offset % 2) == 0) && operand.IsOffset()) {
        int32_t offset_ = offset >> 1;
        EmitT32_16(0x8800 | rt.GetCode() | (rn.GetCode() << 3) |
                   ((offset_ & 0x1f) << 6));
        AdvanceIT();
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm_1>}] ; T2
      if (!size.IsNarrow() && (offset >= 0) && (offset <= 4095) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc)) {
        EmitT32_32(0xf8b00000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [<Rn>{, #-<imm_2>}] ; T3
      if (!size.IsNarrow() && (-offset >= 0) && (-offset <= 255) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc)) {
        EmitT32_32(0xf8300c00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (-offset & 0xff));
        AdvanceIT();
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_2> ; T3
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8300900U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}]! ; T3
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8300d00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm>] ; T1
      if (!size.IsNarrow() && (offset >= -4095) && (offset <= 4095) &&
          rn.Is(pc) && operand.IsOffset() && !rt.Is(pc)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf83f0000U | (rt.GetCode() << 12) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // LDRH{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}] ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsOffset() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x015000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_3> ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsPostIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x005000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}]! ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsPreIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x017000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm_1>] ; A1
      if ((offset >= -255) && (offset <= 255) && rn.Is(pc) &&
          operand.IsOffset() && cond.IsNotNever() &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x015f00b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingT32()) {
      // LDRH{<c>}{<q>} <Rt>, [<Rn>, #{+}<Rm>] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && rm.IsLow() &&
          sign.IsPlus() && operand.IsOffset()) {
        EmitT32_16(0x5a00 | rt.GetCode() | (rn.GetCode() << 3) |
                   (rm.GetCode() << 6));
        AdvanceIT();
        return;
      }
    } else {
      // LDRH{<c>}{<q>} <Rt>, [<Rn>, #{+/-}<Rm>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x011000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<Rm> ; A1
      if (operand.IsPostIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x001000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // LDRH{<c>}{<q>} <Rt>, [<Rn>, #{+/-}<Rm>]! ; A1
      if (operand.IsPreIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x013000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // LDRH{<c>}{<q>} <Rt>, [<Rn>, {+}<Rm>{, LSL #<imm>}] ; T2
      if (!size.IsNarrow() && sign.IsPlus() && shift.IsLSL() && (amount <= 3) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc) &&
          (!rm.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf8300000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount << 4));
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kLdrh, &Assembler::ldrh, cond, size, rt, operand);
}

void Assembler::ldrh(Condition cond, Register rt, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // LDRH{<c>}{<q>} <Rt>, <label> ; T1
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound()) &&
        !rt.Is(pc)) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 12);
          return instr | (target & 0xfff) | ((target & 0x1000) << 11);
        }
      } immop;
      EmitT32_32(Link(0xf83f0000U | (rt.GetCode() << 12), label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // LDRH{<c>}{<q>} <Rt>, <label> ; A1
    if (((label->IsBound() && (offset >= -255) && (offset <= 255)) ||
         !label->IsBound()) &&
        cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-255, 255) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -255) && (offset <= 255));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 8);
          return instr | (target & 0xf) | ((target & 0xf0) << 4) |
                 ((target & 0x100) << 15);
        }
      } immop;
      EmitA32(
          Link(0x015f00b0U | (cond.GetCondition() << 28) | (rt.GetCode() << 12),
               label,
               immop));
      return;
    }
  }
  Delegate(kLdrh, &Assembler::ldrh, cond, rt, label);
}

void Assembler::ldrsb(Condition cond,
                      EncodingSize size,
                      Register rt,
                      const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm>}] ; T1
      if (!size.IsNarrow() && (offset >= 0) && (offset <= 4095) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc)) {
        EmitT32_32(0xf9900000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>{, #-<imm_1>}] ; T2
      if (!size.IsNarrow() && (-offset >= 0) && (-offset <= 255) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc)) {
        EmitT32_32(0xf9100c00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (-offset & 0xff));
        AdvanceIT();
        return;
      }
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_1> ; T2
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf9100900U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_1>}]! ; T2
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf9100d00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDRSB{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm>] ; T1
      if (!size.IsNarrow() && (offset >= -4095) && (offset <= 4095) &&
          rn.Is(pc) && operand.IsOffset() && !rt.Is(pc)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf91f0000U | (rt.GetCode() << 12) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}] ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsOffset() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x015000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_2> ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsPostIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x005000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}]! ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsPreIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x017000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRSB{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm_1>] ; A1
      if ((offset >= -255) && (offset <= 255) && rn.Is(pc) &&
          operand.IsOffset() && cond.IsNotNever() &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x015f00d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingT32()) {
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>, #{+}<Rm>] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && rm.IsLow() &&
          sign.IsPlus() && operand.IsOffset()) {
        EmitT32_16(0x5600 | rt.GetCode() | (rn.GetCode() << 3) |
                   (rm.GetCode() << 6));
        AdvanceIT();
        return;
      }
    } else {
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>, #{+/-}<Rm>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x011000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<Rm> ; A1
      if (operand.IsPostIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x001000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>, #{+/-}<Rm>]! ; A1
      if (operand.IsPreIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x013000d0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // LDRSB{<c>}{<q>} <Rt>, [<Rn>, {+}<Rm>{, LSL #<imm>}] ; T2
      if (!size.IsNarrow() && sign.IsPlus() && shift.IsLSL() && (amount <= 3) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc) &&
          (!rm.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf9100000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount << 4));
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kLdrsb, &Assembler::ldrsb, cond, size, rt, operand);
}

void Assembler::ldrsb(Condition cond, Register rt, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // LDRSB{<c>}{<q>} <Rt>, <label> ; T1
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound()) &&
        !rt.Is(pc)) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 12);
          return instr | (target & 0xfff) | ((target & 0x1000) << 11);
        }
      } immop;
      EmitT32_32(Link(0xf91f0000U | (rt.GetCode() << 12), label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // LDRSB{<c>}{<q>} <Rt>, <label> ; A1
    if (((label->IsBound() && (offset >= -255) && (offset <= 255)) ||
         !label->IsBound()) &&
        cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-255, 255) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -255) && (offset <= 255));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 8);
          return instr | (target & 0xf) | ((target & 0xf0) << 4) |
                 ((target & 0x100) << 15);
        }
      } immop;
      EmitA32(
          Link(0x015f00d0U | (cond.GetCondition() << 28) | (rt.GetCode() << 12),
               label,
               immop));
      return;
    }
  }
  Delegate(kLdrsb, &Assembler::ldrsb, cond, rt, label);
}

void Assembler::ldrsh(Condition cond,
                      EncodingSize size,
                      Register rt,
                      const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm>}] ; T1
      if (!size.IsNarrow() && (offset >= 0) && (offset <= 4095) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc)) {
        EmitT32_32(0xf9b00000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>{, #-<imm_1>}] ; T2
      if (!size.IsNarrow() && (-offset >= 0) && (-offset <= 255) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc)) {
        EmitT32_32(0xf9300c00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (-offset & 0xff));
        AdvanceIT();
        return;
      }
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_1> ; T2
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf9300900U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_1>}]! ; T2
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf9300d00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // LDRSH{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm>] ; T1
      if (!size.IsNarrow() && (offset >= -4095) && (offset <= 4095) &&
          rn.Is(pc) && operand.IsOffset() && !rt.Is(pc)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf93f0000U | (rt.GetCode() << 12) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}] ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsOffset() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x015000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_2> ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsPostIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x005000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}]! ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsPreIndex() &&
          cond.IsNotNever() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x017000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // LDRSH{<c>}{<q>} <Rt>, [PC, #<_plusminus_><imm_1>] ; A1
      if ((offset >= -255) && (offset <= 255) && rn.Is(pc) &&
          operand.IsOffset() && cond.IsNotNever() &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x015f00f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingT32()) {
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>, #{+}<Rm>] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && rm.IsLow() &&
          sign.IsPlus() && operand.IsOffset()) {
        EmitT32_16(0x5e00 | rt.GetCode() | (rn.GetCode() << 3) |
                   (rm.GetCode() << 6));
        AdvanceIT();
        return;
      }
    } else {
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>, #{+/-}<Rm>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x011000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<Rm> ; A1
      if (operand.IsPostIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x001000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>, #{+/-}<Rm>]! ; A1
      if (operand.IsPreIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x013000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // LDRSH{<c>}{<q>} <Rt>, [<Rn>, {+}<Rm>{, LSL #<imm>}] ; T2
      if (!size.IsNarrow() && sign.IsPlus() && shift.IsLSL() && (amount <= 3) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) && !rt.Is(pc) &&
          (!rm.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf9300000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount << 4));
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kLdrsh, &Assembler::ldrsh, cond, size, rt, operand);
}

void Assembler::ldrsh(Condition cond, Register rt, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // LDRSH{<c>}{<q>} <Rt>, <label> ; T1
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound()) &&
        !rt.Is(pc)) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 12);
          return instr | (target & 0xfff) | ((target & 0x1000) << 11);
        }
      } immop;
      EmitT32_32(Link(0xf93f0000U | (rt.GetCode() << 12), label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // LDRSH{<c>}{<q>} <Rt>, <label> ; A1
    if (((label->IsBound() && (offset >= -255) && (offset <= 255)) ||
         !label->IsBound()) &&
        cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-255, 255) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -255) && (offset <= 255));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 8);
          return instr | (target & 0xf) | ((target & 0xf0) << 4) |
                 ((target & 0x100) << 15);
        }
      } immop;
      EmitA32(
          Link(0x015f00f0U | (cond.GetCondition() << 28) | (rt.GetCode() << 12),
               label,
               immop));
      return;
    }
  }
  Delegate(kLdrsh, &Assembler::ldrsh, cond, rt, label);
}

void Assembler::lsl(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rm,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // LSL<c>{<q>} {<Rd>}, <Rm>, #<imm> ; T2
      if (InITBlock() && !size.IsWide() && rd.IsLow() && rm.IsLow() &&
          (imm >= 1) && (imm <= 31)) {
        EmitT32_16(0x0000 | rd.GetCode() | (rm.GetCode() << 3) | (imm << 6));
        AdvanceIT();
        return;
      }
      // LSL{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; T3
      if (!size.IsNarrow() && (imm >= 1) && (imm <= 31) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xea4f0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   ((imm & 0x3) << 6) | ((imm & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // LSL{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; A1
      if ((imm >= 1) && (imm <= 31) && cond.IsNotNever()) {
        EmitA32(0x01a00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (imm << 7));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rs = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LSL<c>{<q>} {<Rdm>}, <Rdm>, <Rs> ; T1
      if (InITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          rs.IsLow()) {
        EmitT32_16(0x4080 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // LSL{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa00f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // LSL{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01a00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kLsl, &Assembler::lsl, cond, size, rd, rm, operand);
}

void Assembler::lsls(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rm,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // LSLS{<q>} {<Rd>}, <Rm>, #<imm> ; T2
      if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && rm.IsLow() &&
          (imm >= 1) && (imm <= 31)) {
        EmitT32_16(0x0000 | rd.GetCode() | (rm.GetCode() << 3) | (imm << 6));
        AdvanceIT();
        return;
      }
      // LSLS{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; T3
      if (!size.IsNarrow() && (imm >= 1) && (imm <= 31) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xea5f0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   ((imm & 0x3) << 6) | ((imm & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // LSLS{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; A1
      if ((imm >= 1) && (imm <= 31) && cond.IsNotNever()) {
        EmitA32(0x01b00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (imm << 7));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rs = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LSLS{<q>} {<Rdm>}, <Rdm>, <Rs> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          rs.IsLow()) {
        EmitT32_16(0x4080 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // LSLS{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa10f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // LSLS{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01b00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kLsls, &Assembler::lsls, cond, size, rd, rm, operand);
}

void Assembler::lsr(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rm,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // LSR<c>{<q>} {<Rd>}, <Rm>, #<imm> ; T2
      if (InITBlock() && !size.IsWide() && rd.IsLow() && rm.IsLow() &&
          (imm >= 1) && (imm <= 32)) {
        uint32_t amount_ = imm % 32;
        EmitT32_16(0x0800 | rd.GetCode() | (rm.GetCode() << 3) |
                   (amount_ << 6));
        AdvanceIT();
        return;
      }
      // LSR{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; T3
      if (!size.IsNarrow() && (imm >= 1) && (imm <= 32) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = imm % 32;
        EmitT32_32(0xea4f0010U | (rd.GetCode() << 8) | rm.GetCode() |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // LSR{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; A1
      if ((imm >= 1) && (imm <= 32) && cond.IsNotNever()) {
        uint32_t amount_ = imm % 32;
        EmitA32(0x01a00020U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rs = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LSR<c>{<q>} {<Rdm>}, <Rdm>, <Rs> ; T1
      if (InITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          rs.IsLow()) {
        EmitT32_16(0x40c0 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // LSR{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa20f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // LSR{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01a00030U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kLsr, &Assembler::lsr, cond, size, rd, rm, operand);
}

void Assembler::lsrs(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rm,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // LSRS{<q>} {<Rd>}, <Rm>, #<imm> ; T2
      if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && rm.IsLow() &&
          (imm >= 1) && (imm <= 32)) {
        uint32_t amount_ = imm % 32;
        EmitT32_16(0x0800 | rd.GetCode() | (rm.GetCode() << 3) |
                   (amount_ << 6));
        AdvanceIT();
        return;
      }
      // LSRS{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; T3
      if (!size.IsNarrow() && (imm >= 1) && (imm <= 32) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = imm % 32;
        EmitT32_32(0xea5f0010U | (rd.GetCode() << 8) | rm.GetCode() |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // LSRS{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; A1
      if ((imm >= 1) && (imm <= 32) && cond.IsNotNever()) {
        uint32_t amount_ = imm % 32;
        EmitA32(0x01b00020U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rs = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // LSRS{<q>} {<Rdm>}, <Rdm>, <Rs> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          rs.IsLow()) {
        EmitT32_16(0x40c0 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // LSRS{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa30f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // LSRS{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01b00030U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kLsrs, &Assembler::lsrs, cond, size, rd, rm, operand);
}

void Assembler::mla(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // MLA{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb000000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // MLA{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00200090U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kMla, &Assembler::mla, cond, rd, rn, rm, ra);
}

void Assembler::mlas(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // MLAS{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00300090U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kMlas, &Assembler::mlas, cond, rd, rn, rm, ra);
}

void Assembler::mls(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // MLS{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfb000010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // MLS{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00600090U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kMls, &Assembler::mls, cond, rd, rn, rm, ra);
}

void Assembler::mov(Condition cond,
                    EncodingSize size,
                    Register rd,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // MOV{<c>}{<q>} <Rd>, <Rm> ; T1
        if (!size.IsWide() &&
            ((!rd.IsPC() || OutsideITBlockAndAlOrLast(cond)) ||
             AllowUnpredictable())) {
          EmitT32_16(0x4600 | (rd.GetCode() & 0x7) |
                     ((rd.GetCode() & 0x8) << 4) | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // MOV<c>{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; T2
      if (InITBlock() && !size.IsWide() && rd.IsLow() &&
          shift.IsValidAmount(amount) && rm.IsLow() &&
          (shift.Is(LSL) || shift.Is(LSR) || shift.Is(ASR)) &&
          ((!shift.Is(LSL) || (amount != 0)) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_16(0x0000 | rd.GetCode() | (rm.GetCode() << 3) |
                   (operand.GetTypeEncodingValue() << 11) | (amount_ << 6));
        AdvanceIT();
        return;
      }
      // MOV{<c>}{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; T3
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea4f0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // MOV{<c>}{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01a00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingT32()) {
      // MOV<c>{<q>} <Rdm>, <Rdm>, ASR <Rs> ; T1
      if (InITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          shift.IsASR() && rs.IsLow()) {
        EmitT32_16(0x4100 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // MOV<c>{<q>} <Rdm>, <Rdm>, LSL <Rs> ; T1
      if (InITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          shift.IsLSL() && rs.IsLow()) {
        EmitT32_16(0x4080 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // MOV<c>{<q>} <Rdm>, <Rdm>, LSR <Rs> ; T1
      if (InITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          shift.IsLSR() && rs.IsLow()) {
        EmitT32_16(0x40c0 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // MOV<c>{<q>} <Rdm>, <Rdm>, ROR <Rs> ; T1
      if (InITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          shift.IsROR() && rs.IsLow()) {
        EmitT32_16(0x41c0 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // MOV{<c>}{<q>} <Rd>, <Rm>, <shift> <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa00f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   (shift.GetType() << 21) | rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // MOV{<c>}{<q>} <Rd>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01a00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (shift.GetType() << 5) |
                (rs.GetCode() << 8));
        return;
      }
    }
  }
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // MOV<c>{<q>} <Rd>, #<imm8> ; T1
      if (InITBlock() && !size.IsWide() && rd.IsLow() && (imm <= 255)) {
        EmitT32_16(0x2000 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
      // MOV{<c>}{<q>} <Rd>, #<const> ; T2
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf04f0000U | (rd.GetCode() << 8) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // MOV{<c>}{<q>} <Rd>, #<imm16> ; T3
      if (!size.IsNarrow() && (imm <= 65535) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2400000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15) |
                   ((imm & 0xf000) << 4));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // MOV{<c>}{<q>} <Rd>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03a00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
      // MOV{<c>}{<q>} <Rd>, #<imm16> ; A2
      if ((imm <= 65535) && cond.IsNotNever() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitA32(0x03000000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (imm & 0xfff) | ((imm & 0xf000) << 4));
        return;
      }
    }
  }
  Delegate(kMov, &Assembler::mov, cond, size, rd, operand);
}

void Assembler::movs(Condition cond,
                     EncodingSize size,
                     Register rd,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // MOVS{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; T2
      if (OutsideITBlock() && !size.IsWide() && rd.IsLow() &&
          shift.IsValidAmount(amount) && rm.IsLow() &&
          (shift.Is(LSL) || shift.Is(LSR) || shift.Is(ASR))) {
        uint32_t amount_ = amount % 32;
        EmitT32_16(0x0000 | rd.GetCode() | (rm.GetCode() << 3) |
                   (operand.GetTypeEncodingValue() << 11) | (amount_ << 6));
        AdvanceIT();
        return;
      }
      // MOVS{<c>}{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; T3
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea5f0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // MOVS{<c>}{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01b00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingT32()) {
      // MOVS{<q>} <Rdm>, <Rdm>, ASR <Rs> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          shift.IsASR() && rs.IsLow()) {
        EmitT32_16(0x4100 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // MOVS{<q>} <Rdm>, <Rdm>, LSL <Rs> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          shift.IsLSL() && rs.IsLow()) {
        EmitT32_16(0x4080 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // MOVS{<q>} <Rdm>, <Rdm>, LSR <Rs> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          shift.IsLSR() && rs.IsLow()) {
        EmitT32_16(0x40c0 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // MOVS{<q>} <Rdm>, <Rdm>, ROR <Rs> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          shift.IsROR() && rs.IsLow()) {
        EmitT32_16(0x41c0 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // MOVS{<c>}{<q>} <Rd>, <Rm>, <shift> <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa10f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   (shift.GetType() << 21) | rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // MOVS{<c>}{<q>} <Rd>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01b00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (shift.GetType() << 5) |
                (rs.GetCode() << 8));
        return;
      }
    }
  }
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // MOVS{<q>} <Rd>, #<imm8> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && (imm <= 255)) {
        EmitT32_16(0x2000 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
      // MOVS{<c>}{<q>} <Rd>, #<const> ; T2
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf05f0000U | (rd.GetCode() << 8) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // MOVS{<c>}{<q>} <Rd>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03b00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  Delegate(kMovs, &Assembler::movs, cond, size, rd, operand);
}

void Assembler::movt(Condition cond, Register rd, const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // MOVT{<c>}{<q>} <Rd>, #<imm16> ; T1
      if ((imm <= 65535) && (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2c00000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15) |
                   ((imm & 0xf000) << 4));
        AdvanceIT();
        return;
      }
    } else {
      // MOVT{<c>}{<q>} <Rd>, #<imm16> ; A1
      if ((imm <= 65535) && cond.IsNotNever() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitA32(0x03400000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (imm & 0xfff) | ((imm & 0xf000) << 4));
        return;
      }
    }
  }
  Delegate(kMovt, &Assembler::movt, cond, rd, operand);
}

void Assembler::movw(Condition cond, Register rd, const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // MOVW{<c>}{<q>} <Rd>, #<imm16> ; T3
      if ((imm <= 65535) && (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2400000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15) |
                   ((imm & 0xf000) << 4));
        AdvanceIT();
        return;
      }
    } else {
      // MOVW{<c>}{<q>} <Rd>, #<imm16> ; A2
      if ((imm <= 65535) && cond.IsNotNever() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitA32(0x03000000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (imm & 0xfff) | ((imm & 0xf000) << 4));
        return;
      }
    }
  }
  Delegate(kMovw, &Assembler::movw, cond, rd, operand);
}

void Assembler::mrs(Condition cond, Register rd, SpecialRegister spec_reg) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // MRS{<c>}{<q>} <Rd>, <spec_reg> ; T1
    if ((!rd.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xf3ef8000U | (rd.GetCode() << 8) | (spec_reg.GetReg() << 20));
      AdvanceIT();
      return;
    }
  } else {
    // MRS{<c>}{<q>} <Rd>, <spec_reg> ; A1
    if (cond.IsNotNever() && (!rd.IsPC() || AllowUnpredictable())) {
      EmitA32(0x010f0000U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (spec_reg.GetReg() << 22));
      return;
    }
  }
  Delegate(kMrs, &Assembler::mrs, cond, rd, spec_reg);
}

void Assembler::msr(Condition cond,
                    MaskedSpecialRegister spec_reg,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingA32()) {
      ImmediateA32 immediate_a32(imm);
      // MSR{<c>}{<q>} <spec_reg>, #<imm> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x0320f000U | (cond.GetCondition() << 28) |
                ((spec_reg.GetReg() & 0xf) << 16) |
                ((spec_reg.GetReg() & 0x10) << 18) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // MSR{<c>}{<q>} <spec_reg>, <Rn> ; T1
      if ((!rn.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf3808000U | ((spec_reg.GetReg() & 0xf) << 8) |
                   ((spec_reg.GetReg() & 0x10) << 16) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // MSR{<c>}{<q>} <spec_reg>, <Rn> ; A1
      if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
        EmitA32(0x0120f000U | (cond.GetCondition() << 28) |
                ((spec_reg.GetReg() & 0xf) << 16) |
                ((spec_reg.GetReg() & 0x10) << 18) | rn.GetCode());
        return;
      }
    }
  }
  Delegate(kMsr, &Assembler::msr, cond, spec_reg, operand);
}

void Assembler::mul(
    Condition cond, EncodingSize size, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // MUL<c>{<q>} <Rdm>, <Rn>, {<Rdm>} ; T1
    if (InITBlock() && !size.IsWide() && rd.Is(rm) && rn.IsLow() &&
        rm.IsLow()) {
      EmitT32_16(0x4340 | rd.GetCode() | (rn.GetCode() << 3));
      AdvanceIT();
      return;
    }
    // MUL{<c>}{<q>} <Rd>, <Rn>, {<Rm>} ; T2
    if (!size.IsNarrow() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb00f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // MUL{<c>}{<q>} <Rd>, <Rn>, {<Rm>} ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x00000090U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kMul, &Assembler::mul, cond, size, rd, rn, rm);
}

void Assembler::muls(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // MULS{<q>} <Rdm>, <Rn>, {<Rdm>} ; T1
    if (OutsideITBlock() && rd.Is(rm) && rn.IsLow() && rm.IsLow()) {
      EmitT32_16(0x4340 | rd.GetCode() | (rn.GetCode() << 3));
      AdvanceIT();
      return;
    }
  } else {
    // MULS{<c>}{<q>} <Rd>, <Rn>, {<Rm>} ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x00100090U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kMuls, &Assembler::muls, cond, rd, rn, rm);
}

void Assembler::mvn(Condition cond,
                    EncodingSize size,
                    Register rd,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // MVN{<c>}{<q>} <Rd>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf06f0000U | (rd.GetCode() << 8) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // MVN{<c>}{<q>} <Rd>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03e00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // MVN<c>{<q>} <Rd>, <Rm> ; T1
        if (InITBlock() && !size.IsWide() && rd.IsLow() && rm.IsLow()) {
          EmitT32_16(0x43c0 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // MVN{<c>}{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea6f0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // MVN{<c>}{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01e00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // MVN{<c>}{<q>} <Rd>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01e00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (shift.GetType() << 5) |
                (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kMvn, &Assembler::mvn, cond, size, rd, operand);
}

void Assembler::mvns(Condition cond,
                     EncodingSize size,
                     Register rd,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // MVNS{<c>}{<q>} <Rd>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf07f0000U | (rd.GetCode() << 8) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // MVNS{<c>}{<q>} <Rd>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03f00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // MVNS{<q>} <Rd>, <Rm> ; T1
        if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && rm.IsLow()) {
          EmitT32_16(0x43c0 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // MVNS{<c>}{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea7f0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // MVNS{<c>}{<q>} <Rd>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01f00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // MVNS{<c>}{<q>} <Rd>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01f00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (shift.GetType() << 5) |
                (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kMvns, &Assembler::mvns, cond, size, rd, operand);
}

void Assembler::nop(Condition cond, EncodingSize size) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // NOP{<c>}{<q>} ; T1
    if (!size.IsWide()) {
      EmitT32_16(0xbf00);
      AdvanceIT();
      return;
    }
    // NOP{<c>}.W ; T2
    if (!size.IsNarrow()) {
      EmitT32_32(0xf3af8000U);
      AdvanceIT();
      return;
    }
  } else {
    // NOP{<c>}{<q>} ; A1
    if (cond.IsNotNever()) {
      EmitA32(0x0320f000U | (cond.GetCondition() << 28));
      return;
    }
  }
  Delegate(kNop, &Assembler::nop, cond, size);
}

void Assembler::orn(Condition cond,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // ORN{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (immediate_t32.IsValid() && !rn.Is(pc) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf0600000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // ORN{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T1
      if (shift.IsValidAmount(amount) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea600000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kOrn, &Assembler::orn, cond, rd, rn, operand);
}

void Assembler::orns(Condition cond,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // ORNS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (immediate_t32.IsValid() && !rn.Is(pc) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf0700000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // ORNS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T1
      if (shift.IsValidAmount(amount) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea700000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kOrns, &Assembler::orns, cond, rd, rn, operand);
}

void Assembler::orr(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // ORR{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() && !rn.Is(pc) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf0400000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // ORR{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03800000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // ORR<c>{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (InITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4300 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // ORR{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea400000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ORR{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01800000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // ORR{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x01800010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kOrr, &Assembler::orr, cond, size, rd, rn, operand);
}

void Assembler::orrs(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // ORRS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() && !rn.Is(pc) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf0500000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // ORRS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03900000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // ORRS{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (OutsideITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4300 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // ORRS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea500000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ORRS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01900000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // ORRS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x01900010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kOrrs, &Assembler::orrs, cond, size, rd, rn, operand);
}

void Assembler::pkhbt(Condition cond,
                      Register rd,
                      Register rn,
                      const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // PKHBT{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, LSL #<imm> } ; T1
      if (shift.IsLSL() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xeac00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | ((amount & 0x3) << 6) |
                   ((amount & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // PKHBT{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, LSL #<imm> } ; A1
      if (shift.IsLSL() && shift.IsValidAmount(amount) && cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x06800010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (amount << 7));
        return;
      }
    }
  }
  Delegate(kPkhbt, &Assembler::pkhbt, cond, rd, rn, operand);
}

void Assembler::pkhtb(Condition cond,
                      Register rd,
                      Register rn,
                      const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // PKHTB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ASR #<imm> } ; T1
      if ((shift.IsASR() || (amount == 0)) && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeac00020U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | ((amount_ & 0x3) << 6) |
                   ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // PKHTB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ASR #<imm> } ; A1
      if ((shift.IsASR() || (amount == 0)) && shift.IsValidAmount(amount) &&
          cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x06800050U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (amount_ << 7));
        return;
      }
    }
  }
  Delegate(kPkhtb, &Assembler::pkhtb, cond, rd, rn, operand);
}

void Assembler::pld(Condition cond, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // PLD{<c>}{<q>} <label> ; T1
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 12);
          return instr | (target & 0xfff) | ((target & 0x1000) << 11);
        }
      } immop;
      EmitT32_32(Link(0xf81ff000U, label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // PLD{<c>}{<q>} <label> ; A1
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound())) {
      if (cond.Is(al)) {
        static class EmitOp : public Label::LabelEmitOperator {
         public:
          EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
          virtual uint32_t Encode(uint32_t instr,
                                  Label::Offset pc,
                                  const Label* label) const VIXL_OVERRIDE {
            Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
            VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
            uint32_t U = (offset >= 0);
            int32_t target = abs(offset) | (U << 12);
            return instr | (target & 0xfff) | ((target & 0x1000) << 11);
          }
        } immop;
        EmitA32(Link(0xf55ff000U, label, immop));
        return;
      }
    }
  }
  Delegate(kPld, &Assembler::pld, cond, label);
}

void Assembler::pld(Condition cond, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // PLD{<c>}{<q>} [PC, #<_plusminus_><imm>] ; T1
      if ((offset >= -4095) && (offset <= 4095) && rn.Is(pc) &&
          operand.IsOffset()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf81ff000U | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // PLD{<c>}{<q>} [PC, #<_plusminus_><imm_1>] ; A1
      if ((offset >= -4095) && (offset <= 4095) && rn.Is(pc) &&
          operand.IsOffset()) {
        if (cond.Is(al)) {
          uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
          uint32_t offset_ = abs(offset);
          EmitA32(0xf55ff000U | offset_ | (sign << 23));
          return;
        }
      }
    }
  }
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // PLD{<c>}{<q>} [<Rn>{, #{+}<imm>}] ; T1
      if ((offset >= 0) && (offset <= 4095) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        EmitT32_32(0xf890f000U | (rn.GetCode() << 16) | (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // PLD{<c>}{<q>} [<Rn>{, #-<imm_1>}] ; T2
      if ((-offset >= 0) && (-offset <= 255) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        EmitT32_32(0xf810fc00U | (rn.GetCode() << 16) | (-offset & 0xff));
        AdvanceIT();
        return;
      }
    } else {
      // PLD{<c>}{<q>} [<Rn>{, #{+/-}<imm_2>}] ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        if (cond.Is(al)) {
          uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
          uint32_t offset_ = abs(offset);
          EmitA32(0xf550f000U | (rn.GetCode() << 16) | offset_ | (sign << 23));
          return;
        }
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // PLD{<c>}{<q>} [<Rn>, {+}<Rm>{, LSL #<amount>}] ; T1
      if (sign.IsPlus() && shift.IsLSL() && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf) &&
          (!rm.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf810f000U | (rn.GetCode() << 16) | rm.GetCode() |
                   (amount << 4));
        AdvanceIT();
        return;
      }
    } else {
      // PLD{<c>}{<q>} [<Rn>, {+/-}<Rm>{, <shift> #<amount_1>}] ; A1
      if (!shift.IsRRX() && shift.IsValidAmount(amount) && operand.IsOffset() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          uint32_t sign_ = sign.IsPlus() ? 1 : 0;
          uint32_t amount_ = amount % 32;
          EmitA32(0xf750f000U | (rn.GetCode() << 16) | rm.GetCode() |
                  (sign_ << 23) | (shift.GetType() << 5) | (amount_ << 7));
          return;
        }
      }
      // PLD{<c>}{<q>} [<Rn>, {+/-}<Rm>, RRX] ; A1
      if (shift.IsRRX() && operand.IsOffset() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          uint32_t sign_ = sign.IsPlus() ? 1 : 0;
          EmitA32(0xf750f060U | (rn.GetCode() << 16) | rm.GetCode() |
                  (sign_ << 23));
          return;
        }
      }
    }
  }
  Delegate(kPld, &Assembler::pld, cond, operand);
}

void Assembler::pldw(Condition cond, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // PLDW{<c>}{<q>} [<Rn>{, #{+}<imm>}] ; T1
      if ((offset >= 0) && (offset <= 4095) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        EmitT32_32(0xf8b0f000U | (rn.GetCode() << 16) | (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // PLDW{<c>}{<q>} [<Rn>{, #-<imm_1>}] ; T2
      if ((-offset >= 0) && (-offset <= 255) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        EmitT32_32(0xf830fc00U | (rn.GetCode() << 16) | (-offset & 0xff));
        AdvanceIT();
        return;
      }
    } else {
      // PLDW{<c>}{<q>} [<Rn>{, #{+/-}<imm_2>}] ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        if (cond.Is(al)) {
          uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
          uint32_t offset_ = abs(offset);
          EmitA32(0xf510f000U | (rn.GetCode() << 16) | offset_ | (sign << 23));
          return;
        }
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // PLDW{<c>}{<q>} [<Rn>, {+}<Rm>{, LSL #<amount>}] ; T1
      if (sign.IsPlus() && shift.IsLSL() && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf) &&
          (!rm.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf830f000U | (rn.GetCode() << 16) | rm.GetCode() |
                   (amount << 4));
        AdvanceIT();
        return;
      }
    } else {
      // PLDW{<c>}{<q>} [<Rn>, {+/-}<Rm>{, <shift> #<amount_1>}] ; A1
      if (!shift.IsRRX() && shift.IsValidAmount(amount) && operand.IsOffset() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          uint32_t sign_ = sign.IsPlus() ? 1 : 0;
          uint32_t amount_ = amount % 32;
          EmitA32(0xf710f000U | (rn.GetCode() << 16) | rm.GetCode() |
                  (sign_ << 23) | (shift.GetType() << 5) | (amount_ << 7));
          return;
        }
      }
      // PLDW{<c>}{<q>} [<Rn>, {+/-}<Rm>, RRX] ; A1
      if (shift.IsRRX() && operand.IsOffset() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          uint32_t sign_ = sign.IsPlus() ? 1 : 0;
          EmitA32(0xf710f060U | (rn.GetCode() << 16) | rm.GetCode() |
                  (sign_ << 23));
          return;
        }
      }
    }
  }
  Delegate(kPldw, &Assembler::pldw, cond, operand);
}

void Assembler::pli(Condition cond, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // PLI{<c>}{<q>} [<Rn>{, #{+}<imm>}] ; T1
      if ((offset >= 0) && (offset <= 4095) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        EmitT32_32(0xf990f000U | (rn.GetCode() << 16) | (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // PLI{<c>}{<q>} [<Rn>{, #-<imm_1>}] ; T2
      if ((-offset >= 0) && (-offset <= 255) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        EmitT32_32(0xf910fc00U | (rn.GetCode() << 16) | (-offset & 0xff));
        AdvanceIT();
        return;
      }
    } else {
      // PLI{<c>}{<q>} [<Rn>{, #{+/-}<imm_3>}] ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        if (cond.Is(al)) {
          uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
          uint32_t offset_ = abs(offset);
          EmitA32(0xf450f000U | (rn.GetCode() << 16) | offset_ | (sign << 23));
          return;
        }
      }
    }
  }
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // PLI{<c>}{<q>} [PC, #<_plusminus_><imm_2>] ; T3
      if ((offset >= -4095) && (offset <= 4095) && rn.Is(pc) &&
          operand.IsOffset()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf91ff000U | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // PLI{<c>}{<q>} [PC, #<_plusminus_><imm_3>] ; A1
      if ((offset >= -4095) && (offset <= 4095) && rn.Is(pc) &&
          operand.IsOffset()) {
        if (cond.Is(al)) {
          uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
          uint32_t offset_ = abs(offset);
          EmitA32(0xf45ff000U | offset_ | (sign << 23));
          return;
        }
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // PLI{<c>}{<q>} [<Rn>, {+}<Rm>{, LSL #<amount>}] ; T1
      if (sign.IsPlus() && shift.IsLSL() && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf) &&
          (!rm.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf910f000U | (rn.GetCode() << 16) | rm.GetCode() |
                   (amount << 4));
        AdvanceIT();
        return;
      }
    } else {
      // PLI{<c>}{<q>} [<Rn>, {+/-}<Rm>, RRX] ; A1
      if (shift.IsRRX() && operand.IsOffset() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          uint32_t sign_ = sign.IsPlus() ? 1 : 0;
          EmitA32(0xf650f060U | (rn.GetCode() << 16) | rm.GetCode() |
                  (sign_ << 23));
          return;
        }
      }
      // PLI{<c>}{<q>} [<Rn>, {+/-}<Rm>{, <shift> #<amount_1>}] ; A1
      if (!shift.IsRRX() && shift.IsValidAmount(amount) && operand.IsOffset() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          uint32_t sign_ = sign.IsPlus() ? 1 : 0;
          uint32_t amount_ = amount % 32;
          EmitA32(0xf650f000U | (rn.GetCode() << 16) | rm.GetCode() |
                  (sign_ << 23) | (shift.GetType() << 5) | (amount_ << 7));
          return;
        }
      }
    }
  }
  Delegate(kPli, &Assembler::pli, cond, operand);
}

void Assembler::pli(Condition cond, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // PLI{<c>}{<q>} <label> ; T3
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
          uint32_t U = (offset >= 0);
          int32_t target = abs(offset) | (U << 12);
          return instr | (target & 0xfff) | ((target & 0x1000) << 11);
        }
      } immop;
      EmitT32_32(Link(0xf91ff000U, label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // PLI{<c>}{<q>} <label> ; A1
    if (((label->IsBound() && (offset >= -4095) && (offset <= 4095)) ||
         !label->IsBound())) {
      if (cond.Is(al)) {
        static class EmitOp : public Label::LabelEmitOperator {
         public:
          EmitOp() : Label::LabelEmitOperator(-4095, 4095) {}
          virtual uint32_t Encode(uint32_t instr,
                                  Label::Offset pc,
                                  const Label* label) const VIXL_OVERRIDE {
            Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
            VIXL_ASSERT((offset >= -4095) && (offset <= 4095));
            uint32_t U = (offset >= 0);
            int32_t target = abs(offset) | (U << 12);
            return instr | (target & 0xfff) | ((target & 0x1000) << 11);
          }
        } immop;
        EmitA32(Link(0xf45ff000U, label, immop));
        return;
      }
    }
  }
  Delegate(kPli, &Assembler::pli, cond, label);
}

void Assembler::pop(Condition cond, EncodingSize size, RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // POP{<c>}{<q>} <registers> ; T1
    if (!size.IsWide() && ((registers.GetList() & ~0x80ff) == 0)) {
      EmitT32_16(0xbc00 | (GetRegisterListEncoding(registers, 15, 1) << 8) |
                 GetRegisterListEncoding(registers, 0, 8));
      AdvanceIT();
      return;
    }
    // POP{<c>}{<q>} <registers> ; T2
    if (!size.IsNarrow() && ((registers.GetList() & ~0xdfff) == 0)) {
      EmitT32_32(0xe8bd0000U |
                 (GetRegisterListEncoding(registers, 15, 1) << 15) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // POP{<c>}{<q>} <registers> ; A1
    if (cond.IsNotNever()) {
      EmitA32(0x08bd0000U | (cond.GetCondition() << 28) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kPop, &Assembler::pop, cond, size, registers);
}

void Assembler::pop(Condition cond, EncodingSize size, Register rt) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // POP{<c>}{<q>} <single_register_list> ; T4
    if (!size.IsNarrow() && ((!rt.IsPC() || OutsideITBlockAndAlOrLast(cond)) ||
                             AllowUnpredictable())) {
      EmitT32_32(0xf85d0b04U | (rt.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // POP{<c>}{<q>} <single_register_list> ; A1
    if (cond.IsNotNever()) {
      EmitA32(0x049d0004U | (cond.GetCondition() << 28) | (rt.GetCode() << 12));
      return;
    }
  }
  Delegate(kPop, &Assembler::pop, cond, size, rt);
}

void Assembler::push(Condition cond,
                     EncodingSize size,
                     RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // PUSH{<c>}{<q>} <registers> ; T1
    if (!size.IsWide() && ((registers.GetList() & ~0x40ff) == 0)) {
      EmitT32_16(0xb400 | (GetRegisterListEncoding(registers, 14, 1) << 8) |
                 GetRegisterListEncoding(registers, 0, 8));
      AdvanceIT();
      return;
    }
    // PUSH{<c>}{<q>} <registers> ; T1
    if (!size.IsNarrow() && ((registers.GetList() & ~0x5fff) == 0)) {
      EmitT32_32(0xe92d0000U |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // PUSH{<c>}{<q>} <registers> ; A1
    if (cond.IsNotNever()) {
      EmitA32(0x092d0000U | (cond.GetCondition() << 28) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kPush, &Assembler::push, cond, size, registers);
}

void Assembler::push(Condition cond, EncodingSize size, Register rt) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // PUSH{<c>}{<q>} <single_register_list> ; T4
    if (!size.IsNarrow() && (!rt.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xf84d0d04U | (rt.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // PUSH{<c>}{<q>} <single_register_list> ; A1
    if (cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
      EmitA32(0x052d0004U | (cond.GetCondition() << 28) | (rt.GetCode() << 12));
      return;
    }
  }
  Delegate(kPush, &Assembler::push, cond, size, rt);
}

void Assembler::qadd(Condition cond, Register rd, Register rm, Register rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QADD{<c>}{<q>} {<Rd>}, <Rm>, <Rn> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f080U | (rd.GetCode() << 8) | rm.GetCode() |
                 (rn.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // QADD{<c>}{<q>} {<Rd>}, <Rm>, <Rn> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01000050U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode() | (rn.GetCode() << 16));
      return;
    }
  }
  Delegate(kQadd, &Assembler::qadd, cond, rd, rm, rn);
}

void Assembler::qadd16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // QADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06200f10U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kQadd16, &Assembler::qadd16, cond, rd, rn, rm);
}

void Assembler::qadd8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // QADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06200f90U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kQadd8, &Assembler::qadd8, cond, rd, rn, rm);
}

void Assembler::qasx(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfaa0f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // QASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06200f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kQasx, &Assembler::qasx, cond, rd, rn, rm);
}

void Assembler::qdadd(Condition cond, Register rd, Register rm, Register rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QDADD{<c>}{<q>} {<Rd>}, <Rm>, <Rn> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f090U | (rd.GetCode() << 8) | rm.GetCode() |
                 (rn.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // QDADD{<c>}{<q>} {<Rd>}, <Rm>, <Rn> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01400050U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode() | (rn.GetCode() << 16));
      return;
    }
  }
  Delegate(kQdadd, &Assembler::qdadd, cond, rd, rm, rn);
}

void Assembler::qdsub(Condition cond, Register rd, Register rm, Register rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QDSUB{<c>}{<q>} {<Rd>}, <Rm>, <Rn> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f0b0U | (rd.GetCode() << 8) | rm.GetCode() |
                 (rn.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // QDSUB{<c>}{<q>} {<Rd>}, <Rm>, <Rn> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01600050U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode() | (rn.GetCode() << 16));
      return;
    }
  }
  Delegate(kQdsub, &Assembler::qdsub, cond, rd, rm, rn);
}

void Assembler::qsax(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfae0f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // QSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06200f50U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kQsax, &Assembler::qsax, cond, rd, rn, rm);
}

void Assembler::qsub(Condition cond, Register rd, Register rm, Register rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QSUB{<c>}{<q>} {<Rd>}, <Rm>, <Rn> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f0a0U | (rd.GetCode() << 8) | rm.GetCode() |
                 (rn.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // QSUB{<c>}{<q>} {<Rd>}, <Rm>, <Rn> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01200050U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode() | (rn.GetCode() << 16));
      return;
    }
  }
  Delegate(kQsub, &Assembler::qsub, cond, rd, rm, rn);
}

void Assembler::qsub16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfad0f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // QSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06200f70U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kQsub16, &Assembler::qsub16, cond, rd, rn, rm);
}

void Assembler::qsub8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // QSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfac0f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // QSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06200ff0U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kQsub8, &Assembler::qsub8, cond, rd, rn, rm);
}

void Assembler::rbit(Condition cond, Register rd, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // RBIT{<c>}{<q>} <Rd>, <Rm> ; T1
    if (((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f0a0U | (rd.GetCode() << 8) | rm.GetCode() |
                 (rm.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // RBIT{<c>}{<q>} <Rd>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06ff0f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode());
      return;
    }
  }
  Delegate(kRbit, &Assembler::rbit, cond, rd, rm);
}

void Assembler::rev(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // REV{<c>}{<q>} <Rd>, <Rm> ; T1
    if (!size.IsWide() && rd.IsLow() && rm.IsLow()) {
      EmitT32_16(0xba00 | rd.GetCode() | (rm.GetCode() << 3));
      AdvanceIT();
      return;
    }
    // REV{<c>}{<q>} <Rd>, <Rm> ; T2
    if (!size.IsNarrow() &&
        ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f080U | (rd.GetCode() << 8) | rm.GetCode() |
                 (rm.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // REV{<c>}{<q>} <Rd>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06bf0f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode());
      return;
    }
  }
  Delegate(kRev, &Assembler::rev, cond, size, rd, rm);
}

void Assembler::rev16(Condition cond,
                      EncodingSize size,
                      Register rd,
                      Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // REV16{<c>}{<q>} <Rd>, <Rm> ; T1
    if (!size.IsWide() && rd.IsLow() && rm.IsLow()) {
      EmitT32_16(0xba40 | rd.GetCode() | (rm.GetCode() << 3));
      AdvanceIT();
      return;
    }
    // REV16{<c>}{<q>} <Rd>, <Rm> ; T2
    if (!size.IsNarrow() &&
        ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f090U | (rd.GetCode() << 8) | rm.GetCode() |
                 (rm.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // REV16{<c>}{<q>} <Rd>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06bf0fb0U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode());
      return;
    }
  }
  Delegate(kRev16, &Assembler::rev16, cond, size, rd, rm);
}

void Assembler::revsh(Condition cond,
                      EncodingSize size,
                      Register rd,
                      Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // REVSH{<c>}{<q>} <Rd>, <Rm> ; T1
    if (!size.IsWide() && rd.IsLow() && rm.IsLow()) {
      EmitT32_16(0xbac0 | rd.GetCode() | (rm.GetCode() << 3));
      AdvanceIT();
      return;
    }
    // REVSH{<c>}{<q>} <Rd>, <Rm> ; T2
    if (!size.IsNarrow() &&
        ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f0b0U | (rd.GetCode() << 8) | rm.GetCode() |
                 (rm.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // REVSH{<c>}{<q>} <Rd>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06ff0fb0U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode());
      return;
    }
  }
  Delegate(kRevsh, &Assembler::revsh, cond, size, rd, rm);
}

void Assembler::ror(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rm,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // ROR{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; T3
      if (!size.IsNarrow() && (imm >= 1) && (imm <= 31) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xea4f0030U | (rd.GetCode() << 8) | rm.GetCode() |
                   ((imm & 0x3) << 6) | ((imm & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // ROR{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; A1
      if ((imm >= 1) && (imm <= 31) && cond.IsNotNever()) {
        EmitA32(0x01a00060U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (imm << 7));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rs = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // ROR<c>{<q>} {<Rdm>}, <Rdm>, <Rs> ; T1
      if (InITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          rs.IsLow()) {
        EmitT32_16(0x41c0 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // ROR{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa60f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // ROR{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01a00070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kRor, &Assembler::ror, cond, size, rd, rm, operand);
}

void Assembler::rors(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rm,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // RORS{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; T3
      if (!size.IsNarrow() && (imm >= 1) && (imm <= 31) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xea5f0030U | (rd.GetCode() << 8) | rm.GetCode() |
                   ((imm & 0x3) << 6) | ((imm & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // RORS{<c>}{<q>} {<Rd>}, <Rm>, #<imm> ; A1
      if ((imm >= 1) && (imm <= 31) && cond.IsNotNever()) {
        EmitA32(0x01b00060U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (imm << 7));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rs = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // RORS{<q>} {<Rdm>}, <Rdm>, <Rs> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rm) && rm.IsLow() &&
          rs.IsLow()) {
        EmitT32_16(0x41c0 | rd.GetCode() | (rs.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // RORS{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; T2
      if (!size.IsNarrow() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xfa70f000U | (rd.GetCode() << 8) | (rm.GetCode() << 16) |
                   rs.GetCode());
        AdvanceIT();
        return;
      }
    } else {
      // RORS{<c>}{<q>} {<Rd>}, <Rm>, <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01b00070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kRors, &Assembler::rors, cond, size, rd, rm, operand);
}

void Assembler::rrx(Condition cond, Register rd, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // RRX{<c>}{<q>} {<Rd>}, <Rm> ; T3
    if (((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xea4f0030U | (rd.GetCode() << 8) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // RRX{<c>}{<q>} {<Rd>}, <Rm> ; A1
    if (cond.IsNotNever()) {
      EmitA32(0x01a00060U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode());
      return;
    }
  }
  Delegate(kRrx, &Assembler::rrx, cond, rd, rm);
}

void Assembler::rrxs(Condition cond, Register rd, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // RRXS{<c>}{<q>} {<Rd>}, <Rm> ; T3
    if (((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xea5f0030U | (rd.GetCode() << 8) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // RRXS{<c>}{<q>} {<Rd>}, <Rm> ; A1
    if (cond.IsNotNever()) {
      EmitA32(0x01b00060U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rm.GetCode());
      return;
    }
  }
  Delegate(kRrxs, &Assembler::rrxs, cond, rd, rm);
}

void Assembler::rsb(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // RSB<c>{<q>} {<Rd>}, <Rn>, #0 ; T1
      if (InITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
          (imm == 0)) {
        EmitT32_16(0x4240 | rd.GetCode() | (rn.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // RSB{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T2
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf1c00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // RSB{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02600000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // RSB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T1
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xebc00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // RSB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00600000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // RSB{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00600010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kRsb, &Assembler::rsb, cond, size, rd, rn, operand);
}

void Assembler::rsbs(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // RSBS{<q>} {<Rd>}, <Rn>, #0 ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
          (imm == 0)) {
        EmitT32_16(0x4240 | rd.GetCode() | (rn.GetCode() << 3));
        AdvanceIT();
        return;
      }
      // RSBS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T2
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf1d00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // RSBS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02700000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // RSBS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T1
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xebd00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // RSBS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00700000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // RSBS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00700010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kRsbs, &Assembler::rsbs, cond, size, rd, rn, operand);
}

void Assembler::rsc(Condition cond,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingA32()) {
      ImmediateA32 immediate_a32(imm);
      // RSC{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02e00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingA32()) {
      // RSC{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00e00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // RSC{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00e00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kRsc, &Assembler::rsc, cond, rd, rn, operand);
}

void Assembler::rscs(Condition cond,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingA32()) {
      ImmediateA32 immediate_a32(imm);
      // RSCS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02f00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingA32()) {
      // RSCS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00f00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // RSCS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00f00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kRscs, &Assembler::rscs, cond, rd, rn, operand);
}

void Assembler::sadd16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06100f10U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kSadd16, &Assembler::sadd16, cond, rd, rn, rm);
}

void Assembler::sadd8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06100f90U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kSadd8, &Assembler::sadd8, cond, rd, rn, rm);
}

void Assembler::sasx(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfaa0f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06100f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kSasx, &Assembler::sasx, cond, rd, rn, rm);
}

void Assembler::sbc(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // SBC{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf1600000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // SBC{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02c00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // SBC<c>{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (InITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4180 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SBC{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeb600000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // SBC{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00c00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // SBC{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00c00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kSbc, &Assembler::sbc, cond, size, rd, rn, operand);
}

void Assembler::sbcs(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // SBCS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf1700000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // SBCS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x02d00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // SBCS{<q>} {<Rdn>}, <Rdn>, <Rm> ; T1
        if (OutsideITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x4180 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SBCS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeb700000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // SBCS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00d00000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // SBCS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00d00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kSbcs, &Assembler::sbcs, cond, size, rd, rn, operand);
}

void Assembler::sbfx(
    Condition cond, Register rd, Register rn, uint32_t lsb, uint32_t width) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SBFX{<c>}{<q>} <Rd>, <Rn>, #<lsb>, #<width> ; T1
    if ((lsb <= 31) &&
        (((width >= 1) && (width <= 32 - lsb) && !rd.IsPC() && !rn.IsPC()) ||
         AllowUnpredictable())) {
      uint32_t widthm1 = width - 1;
      EmitT32_32(0xf3400000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 ((lsb & 0x3) << 6) | ((lsb & 0x1c) << 10) | widthm1);
      AdvanceIT();
      return;
    }
  } else {
    // SBFX{<c>}{<q>} <Rd>, <Rn>, #<lsb>, #<width> ; A1
    if ((lsb <= 31) && cond.IsNotNever() &&
        (((width >= 1) && (width <= 32 - lsb) && !rd.IsPC() && !rn.IsPC()) ||
         AllowUnpredictable())) {
      uint32_t widthm1 = width - 1;
      EmitA32(0x07a00050U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rn.GetCode() | (lsb << 7) | (widthm1 << 16));
      return;
    }
  }
  Delegate(kSbfx, &Assembler::sbfx, cond, rd, rn, lsb, width);
}

void Assembler::sdiv(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SDIV{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb90f0f0U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SDIV{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0710f010U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSdiv, &Assembler::sdiv, cond, rd, rn, rm);
}

void Assembler::sel(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SEL{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfaa0f080U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SEL{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06800fb0U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kSel, &Assembler::sel, cond, rd, rn, rm);
}

void Assembler::shadd16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SHADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f020U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SHADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06300f10U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kShadd16, &Assembler::shadd16, cond, rd, rn, rm);
}

void Assembler::shadd8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SHADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f020U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SHADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06300f90U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kShadd8, &Assembler::shadd8, cond, rd, rn, rm);
}

void Assembler::shasx(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SHASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfaa0f020U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SHASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06300f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kShasx, &Assembler::shasx, cond, rd, rn, rm);
}

void Assembler::shsax(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SHSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfae0f020U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SHSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06300f50U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kShsax, &Assembler::shsax, cond, rd, rn, rm);
}

void Assembler::shsub16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SHSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfad0f020U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SHSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06300f70U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kShsub16, &Assembler::shsub16, cond, rd, rn, rm);
}

void Assembler::shsub8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SHSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfac0f020U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SHSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06300ff0U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kShsub8, &Assembler::shsub8, cond, rd, rn, rm);
}

void Assembler::smlabb(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLABB{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb100000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLABB{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x01000080U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmlabb, &Assembler::smlabb, cond, rd, rn, rm, ra);
}

void Assembler::smlabt(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLABT{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb100010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLABT{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x010000c0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmlabt, &Assembler::smlabt, cond, rd, rn, rm, ra);
}

void Assembler::smlad(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLAD{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb200000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLAD{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() && !ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x07000010U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmlad, &Assembler::smlad, cond, rd, rn, rm, ra);
}

void Assembler::smladx(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLADX{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb200010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLADX{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() && !ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x07000030U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmladx, &Assembler::smladx, cond, rd, rn, rm, ra);
}

void Assembler::smlal(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLAL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbc00000U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMLAL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00e00090U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlal, &Assembler::smlal, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smlalbb(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLALBB{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbc00080U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMLALBB{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x01400080U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlalbb, &Assembler::smlalbb, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smlalbt(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLALBT{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbc00090U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMLALBT{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x014000c0U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlalbt, &Assembler::smlalbt, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smlald(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLALD{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbc000c0U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMLALD{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x07400010U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlald, &Assembler::smlald, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smlaldx(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLALDX{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbc000d0U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMLALDX{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x07400030U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlaldx, &Assembler::smlaldx, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smlals(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // SMLALS{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00f00090U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlals, &Assembler::smlals, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smlaltb(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLALTB{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbc000a0U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMLALTB{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x014000a0U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlaltb, &Assembler::smlaltb, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smlaltt(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLALTT{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbc000b0U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMLALTT{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x014000e0U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlaltt, &Assembler::smlaltt, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smlatb(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLATB{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb100020U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLATB{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x010000a0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmlatb, &Assembler::smlatb, cond, rd, rn, rm, ra);
}

void Assembler::smlatt(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLATT{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb100030U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLATT{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x010000e0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmlatt, &Assembler::smlatt, cond, rd, rn, rm, ra);
}

void Assembler::smlawb(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLAWB{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb300000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLAWB{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x01200080U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmlawb, &Assembler::smlawb, cond, rd, rn, rm, ra);
}

void Assembler::smlawt(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLAWT{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb300010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLAWT{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x012000c0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmlawt, &Assembler::smlawt, cond, rd, rn, rm, ra);
}

void Assembler::smlsd(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLSD{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb400000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLSD{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() && !ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x07000050U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmlsd, &Assembler::smlsd, cond, rd, rn, rm, ra);
}

void Assembler::smlsdx(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLSDX{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb400010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMLSDX{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() && !ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x07000070U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmlsdx, &Assembler::smlsdx, cond, rd, rn, rm, ra);
}

void Assembler::smlsld(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLSLD{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbd000c0U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMLSLD{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x07400050U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlsld, &Assembler::smlsld, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smlsldx(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMLSLDX{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbd000d0U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMLSLDX{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x07400070U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmlsldx, &Assembler::smlsldx, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smmla(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMMLA{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb500000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMMLA{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() && !ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x07500010U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmmla, &Assembler::smmla, cond, rd, rn, rm, ra);
}

void Assembler::smmlar(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMMLAR{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb500010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMMLAR{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() && !ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x07500030U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmmlar, &Assembler::smmlar, cond, rd, rn, rm, ra);
}

void Assembler::smmls(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMMLS{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfb600000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMMLS{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x075000d0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmmls, &Assembler::smmls, cond, rd, rn, rm, ra);
}

void Assembler::smmlsr(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMMLSR{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfb600010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // SMMLSR{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !ra.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x075000f0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kSmmlsr, &Assembler::smmlsr, cond, rd, rn, rm, ra);
}

void Assembler::smmul(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMMUL{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb50f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMMUL{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0750f010U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmmul, &Assembler::smmul, cond, rd, rn, rm);
}

void Assembler::smmulr(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMMULR{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb50f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMMULR{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0750f030U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmmulr, &Assembler::smmulr, cond, rd, rn, rm);
}

void Assembler::smuad(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMUAD{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb20f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMUAD{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0700f010U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmuad, &Assembler::smuad, cond, rd, rn, rm);
}

void Assembler::smuadx(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMUADX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb20f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMUADX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0700f030U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmuadx, &Assembler::smuadx, cond, rd, rn, rm);
}

void Assembler::smulbb(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMULBB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb10f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMULBB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x01600080U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmulbb, &Assembler::smulbb, cond, rd, rn, rm);
}

void Assembler::smulbt(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMULBT{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb10f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMULBT{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x016000c0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmulbt, &Assembler::smulbt, cond, rd, rn, rm);
}

void Assembler::smull(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMULL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfb800000U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMULL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00c00090U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmull, &Assembler::smull, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smulls(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // SMULLS{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00d00090U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmulls, &Assembler::smulls, cond, rdlo, rdhi, rn, rm);
}

void Assembler::smultb(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMULTB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb10f020U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMULTB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x016000a0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmultb, &Assembler::smultb, cond, rd, rn, rm);
}

void Assembler::smultt(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMULTT{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb10f030U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMULTT{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x016000e0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmultt, &Assembler::smultt, cond, rd, rn, rm);
}

void Assembler::smulwb(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMULWB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb30f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMULWB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x012000a0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmulwb, &Assembler::smulwb, cond, rd, rn, rm);
}

void Assembler::smulwt(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMULWT{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb30f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMULWT{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x012000e0U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmulwt, &Assembler::smulwt, cond, rd, rn, rm);
}

void Assembler::smusd(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMUSD{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb40f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMUSD{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0700f050U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmusd, &Assembler::smusd, cond, rd, rn, rm);
}

void Assembler::smusdx(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SMUSDX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb40f010U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SMUSDX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0700f070U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kSmusdx, &Assembler::smusdx, cond, rd, rn, rm);
}

void Assembler::ssat(Condition cond,
                     Register rd,
                     uint32_t imm,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SSAT{<c>}{<q>} <Rd>, #<imm>, <Rn>, ASR #<amount> ; T1
      if ((imm >= 1) && (imm <= 32) && shift.IsASR() && (amount >= 1) &&
          (amount <= 31) &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        uint32_t imm_ = imm - 1;
        EmitT32_32(0xf3200000U | (rd.GetCode() << 8) | imm_ |
                   (rn.GetCode() << 16) | ((amount & 0x3) << 6) |
                   ((amount & 0x1c) << 10));
        AdvanceIT();
        return;
      }
      // SSAT{<c>}{<q>} <Rd>, #<imm>, <Rn> {, LSL #<amount> } ; T1
      if ((imm >= 1) && (imm <= 32) && shift.IsLSL() && (amount <= 31) &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        uint32_t imm_ = imm - 1;
        EmitT32_32(0xf3000000U | (rd.GetCode() << 8) | imm_ |
                   (rn.GetCode() << 16) | ((amount & 0x3) << 6) |
                   ((amount & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // SSAT{<c>}{<q>} <Rd>, #<imm>, <Rn>, ASR #<amount> ; A1
      if ((imm >= 1) && (imm <= 32) && shift.IsASR() && (amount >= 1) &&
          (amount <= 32) && cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        uint32_t imm_ = imm - 1;
        uint32_t amount_ = amount % 32;
        EmitA32(0x06a00050U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (imm_ << 16) | rn.GetCode() |
                (amount_ << 7));
        return;
      }
      // SSAT{<c>}{<q>} <Rd>, #<imm>, <Rn> {, LSL #<amount> } ; A1
      if ((imm >= 1) && (imm <= 32) && shift.IsLSL() && (amount <= 31) &&
          cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        uint32_t imm_ = imm - 1;
        EmitA32(0x06a00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (imm_ << 16) | rn.GetCode() |
                (amount << 7));
        return;
      }
    }
  }
  Delegate(kSsat, &Assembler::ssat, cond, rd, imm, operand);
}

void Assembler::ssat16(Condition cond, Register rd, uint32_t imm, Register rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SSAT16{<c>}{<q>} <Rd>, #<imm>, <Rn> ; T1
    if ((imm >= 1) && (imm <= 16) &&
        ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
      uint32_t imm_ = imm - 1;
      EmitT32_32(0xf3200000U | (rd.GetCode() << 8) | imm_ |
                 (rn.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // SSAT16{<c>}{<q>} <Rd>, #<imm>, <Rn> ; A1
    if ((imm >= 1) && (imm <= 16) && cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
      uint32_t imm_ = imm - 1;
      EmitA32(0x06a00f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (imm_ << 16) | rn.GetCode());
      return;
    }
  }
  Delegate(kSsat16, &Assembler::ssat16, cond, rd, imm, rn);
}

void Assembler::ssax(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfae0f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06100f50U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kSsax, &Assembler::ssax, cond, rd, rn, rm);
}

void Assembler::ssub16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfad0f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06100f70U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kSsub16, &Assembler::ssub16, cond, rd, rn, rm);
}

void Assembler::ssub8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfac0f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // SSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06100ff0U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kSsub8, &Assembler::ssub8, cond, rd, rn, rm);
}

void Assembler::stl(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STL{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8c00fafU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STL{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x0180fc90U | (cond.GetCondition() << 28) | rt.GetCode() |
                (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStl, &Assembler::stl, cond, rt, operand);
}

void Assembler::stlb(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STLB{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8c00f8fU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STLB{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01c0fc90U | (cond.GetCondition() << 28) | rt.GetCode() |
                (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStlb, &Assembler::stlb, cond, rt, operand);
}

void Assembler::stlex(Condition cond,
                      Register rd,
                      Register rt,
                      const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STLEX{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8c00fe0U | rd.GetCode() | (rt.GetCode() << 12) |
                   (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STLEX{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01800e90U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rt.GetCode() | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStlex, &Assembler::stlex, cond, rd, rt, operand);
}

void Assembler::stlexb(Condition cond,
                       Register rd,
                       Register rt,
                       const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STLEXB{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8c00fc0U | rd.GetCode() | (rt.GetCode() << 12) |
                   (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STLEXB{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01c00e90U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rt.GetCode() | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStlexb, &Assembler::stlexb, cond, rd, rt, operand);
}

void Assembler::stlexd(Condition cond,
                       Register rd,
                       Register rt,
                       Register rt2,
                       const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STLEXD{<c>}{<q>} <Rd>, <Rt>, <Rt2>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rd.IsPC() && !rt.IsPC() && !rt2.IsPC() && !rn.IsPC()) ||
           AllowUnpredictable())) {
        EmitT32_32(0xe8c000f0U | rd.GetCode() | (rt.GetCode() << 12) |
                   (rt2.GetCode() << 8) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STLEXD{<c>}{<q>} <Rd>, <Rt>, <Rt2>, [<Rn>] ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsOffset() && cond.IsNotNever() &&
          ((!rd.IsPC() && ((rt.GetCode() & 1) == 0) && !rt2.IsPC() &&
            !rn.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x01a00e90U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rt.GetCode() | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStlexd, &Assembler::stlexd, cond, rd, rt, rt2, operand);
}

void Assembler::stlexh(Condition cond,
                       Register rd,
                       Register rt,
                       const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STLEXH{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8c00fd0U | rd.GetCode() | (rt.GetCode() << 12) |
                   (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STLEXH{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01e00e90U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rt.GetCode() | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStlexh, &Assembler::stlexh, cond, rd, rt, operand);
}

void Assembler::stlh(Condition cond, Register rt, const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STLH{<c>}{<q>} <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8c00f9fU | (rt.GetCode() << 12) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STLH{<c>}{<q>} <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01e0fc90U | (cond.GetCondition() << 28) | rt.GetCode() |
                (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStlh, &Assembler::stlh, cond, rt, operand);
}

void Assembler::stm(Condition cond,
                    EncodingSize size,
                    Register rn,
                    WriteBack write_back,
                    RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // STM{<c>}{<q>} <Rn>!, <registers> ; T1
    if (!size.IsWide() && rn.IsLow() && write_back.DoesWriteBack() &&
        ((registers.GetList() & ~0xff) == 0)) {
      EmitT32_16(0xc000 | (rn.GetCode() << 8) |
                 GetRegisterListEncoding(registers, 0, 8));
      AdvanceIT();
      return;
    }
    // STM{<c>}{<q>} <Rn>{!}, <registers> ; T2
    if (!size.IsNarrow() && ((registers.GetList() & ~0x5fff) == 0) &&
        (!rn.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe8800000U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // STM{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x08800000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kStm, &Assembler::stm, cond, size, rn, write_back, registers);
}

void Assembler::stmda(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // STMDA{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x08000000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kStmda, &Assembler::stmda, cond, rn, write_back, registers);
}

void Assembler::stmdb(Condition cond,
                      EncodingSize size,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // STMDB{<c>}{<q>} SP!, <registers> ; T1
    if (!size.IsWide() && rn.Is(sp) && write_back.DoesWriteBack() &&
        ((registers.GetList() & ~0x40ff) == 0)) {
      EmitT32_16(0xb400 | (GetRegisterListEncoding(registers, 14, 1) << 8) |
                 GetRegisterListEncoding(registers, 0, 8));
      AdvanceIT();
      return;
    }
    // STMDB{<c>}{<q>} <Rn>{!}, <registers> ; T1
    if (!size.IsNarrow() && ((registers.GetList() & ~0x5fff) == 0) &&
        (!rn.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe9000000U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // STMDB{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x09000000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kStmdb, &Assembler::stmdb, cond, size, rn, write_back, registers);
}

void Assembler::stmea(Condition cond,
                      EncodingSize size,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // STMEA{<c>}{<q>} <Rn>!, <registers> ; T1
    if (!size.IsWide() && rn.IsLow() && write_back.DoesWriteBack() &&
        ((registers.GetList() & ~0xff) == 0)) {
      EmitT32_16(0xc000 | (rn.GetCode() << 8) |
                 GetRegisterListEncoding(registers, 0, 8));
      AdvanceIT();
      return;
    }
    // STMEA{<c>}.W <Rn>{!}, <registers> ; T2
    if (!size.IsNarrow() && ((registers.GetList() & ~0x5fff) == 0) &&
        (!rn.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe8800000U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
    // STMEA{<c>}{<q>} <Rn>{!}, <registers> ; T2
    if (!size.IsNarrow() && ((registers.GetList() & ~0x5fff) == 0) &&
        (!rn.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe8800000U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // STMEA{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x08800000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kStmea, &Assembler::stmea, cond, size, rn, write_back, registers);
}

void Assembler::stmed(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // STMED{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x08000000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kStmed, &Assembler::stmed, cond, rn, write_back, registers);
}

void Assembler::stmfa(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // STMFA{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x09800000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kStmfa, &Assembler::stmfa, cond, rn, write_back, registers);
}

void Assembler::stmfd(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // STMFD{<c>}{<q>} <Rn>{!}, <registers> ; T1
    if (((registers.GetList() & ~0x5fff) == 0) &&
        (!rn.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe9000000U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) |
                 (GetRegisterListEncoding(registers, 14, 1) << 14) |
                 GetRegisterListEncoding(registers, 0, 13));
      AdvanceIT();
      return;
    }
  } else {
    // STMFD{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x09000000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kStmfd, &Assembler::stmfd, cond, rn, write_back, registers);
}

void Assembler::stmib(Condition cond,
                      Register rn,
                      WriteBack write_back,
                      RegisterList registers) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // STMIB{<c>}{<q>} <Rn>{!}, <registers> ; A1
    if (cond.IsNotNever() && (!rn.IsPC() || AllowUnpredictable())) {
      EmitA32(0x09800000U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) |
              GetRegisterListEncoding(registers, 0, 16));
      return;
    }
  }
  Delegate(kStmib, &Assembler::stmib, cond, rn, write_back, registers);
}

void Assembler::str(Condition cond,
                    EncodingSize size,
                    Register rt,
                    const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // STR{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm>}] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && (offset >= 0) &&
          (offset <= 124) && ((offset % 4) == 0) && operand.IsOffset()) {
        int32_t offset_ = offset >> 2;
        EmitT32_16(0x6000 | rt.GetCode() | (rn.GetCode() << 3) |
                   ((offset_ & 0x1f) << 6));
        AdvanceIT();
        return;
      }
      // STR{<c>}{<q>} <Rt>, [SP{, #{+}<imm>}] ; T2
      if (!size.IsWide() && rt.IsLow() && (offset >= 0) && (offset <= 1020) &&
          ((offset % 4) == 0) && rn.Is(sp) && operand.IsOffset()) {
        int32_t offset_ = offset >> 2;
        EmitT32_16(0x9000 | (rt.GetCode() << 8) | (offset_ & 0xff));
        AdvanceIT();
        return;
      }
      // STR{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm_1>}] ; T3
      if (!size.IsNarrow() && (offset >= 0) && (offset <= 4095) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf8c00000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // STR{<c>}{<q>} <Rt>, [<Rn>{, #-<imm_2>}] ; T4
      if (!size.IsNarrow() && (-offset >= 0) && (-offset <= 255) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf8400c00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (-offset & 0xff));
        AdvanceIT();
        return;
      }
      // STR{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_2> ; T4
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8400900U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // STR{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}]! ; T4
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8400d00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
    } else {
      // STR{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}] ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsOffset() &&
          cond.IsNotNever()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x05000000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // STR{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_3> ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsPostIndex() &&
          cond.IsNotNever()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x04000000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // STR{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}]! ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsPreIndex() &&
          cond.IsNotNever()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x05200000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingT32()) {
      // STR{<c>}{<q>} <Rt>, [<Rn>, #{+}<Rm>] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && rm.IsLow() &&
          sign.IsPlus() && operand.IsOffset()) {
        EmitT32_16(0x5000 | rt.GetCode() | (rn.GetCode() << 3) |
                   (rm.GetCode() << 6));
        AdvanceIT();
        return;
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // STR{<c>}{<q>} <Rt>, [<Rn>, {+}<Rm>{, LSL #<imm>}] ; T2
      if (!size.IsNarrow() && sign.IsPlus() && shift.IsLSL() && (amount <= 3) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf8400000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount << 4));
        AdvanceIT();
        return;
      }
    } else {
      // STR{<c>}{<q>} <Rt>, [<Rn>, {+/-}<Rm>{, <shift>}] ; A1
      if (operand.IsShiftValid() && operand.IsOffset() && cond.IsNotNever() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x07000000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
      // STR{<c>}{<q>} <Rt>, [<Rn>], {+/-}<Rm>{, <shift>} ; A1
      if (operand.IsShiftValid() && operand.IsPostIndex() &&
          cond.IsNotNever() && (!rm.IsPC() || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x06000000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
      // STR{<c>}{<q>} <Rt>, [<Rn>, {+/-}<Rm>{, <shift>}]! ; A1
      if (operand.IsShiftValid() && operand.IsPreIndex() && cond.IsNotNever() &&
          (!rm.IsPC() || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x07200000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
    }
  }
  Delegate(kStr, &Assembler::str, cond, size, rt, operand);
}

void Assembler::strb(Condition cond,
                     EncodingSize size,
                     Register rt,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // STRB{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm>}] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && (offset >= 0) &&
          (offset <= 31) && operand.IsOffset()) {
        EmitT32_16(0x7000 | rt.GetCode() | (rn.GetCode() << 3) |
                   ((offset & 0x1f) << 6));
        AdvanceIT();
        return;
      }
      // STRB{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm_1>}] ; T2
      if (!size.IsNarrow() && (offset >= 0) && (offset <= 4095) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf8800000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // STRB{<c>}{<q>} <Rt>, [<Rn>{, #-<imm_2>}] ; T3
      if (!size.IsNarrow() && (-offset >= 0) && (-offset <= 255) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf8000c00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (-offset & 0xff));
        AdvanceIT();
        return;
      }
      // STRB{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_2> ; T3
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8000900U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // STRB{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}]! ; T3
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8000d00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
    } else {
      // STRB{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}] ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsOffset() &&
          cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x05400000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // STRB{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_3> ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsPostIndex() &&
          cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x04400000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
      // STRB{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}]! ; A1
      if ((offset >= -4095) && (offset <= 4095) && operand.IsPreIndex() &&
          cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x05600000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | offset_ |
                (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingT32()) {
      // STRB{<c>}{<q>} <Rt>, [<Rn>, #{+}<Rm>] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && rm.IsLow() &&
          sign.IsPlus() && operand.IsOffset()) {
        EmitT32_16(0x5400 | rt.GetCode() | (rn.GetCode() << 3) |
                   (rm.GetCode() << 6));
        AdvanceIT();
        return;
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // STRB{<c>}{<q>} <Rt>, [<Rn>, {+}<Rm>{, LSL #<imm>}] ; T2
      if (!size.IsNarrow() && sign.IsPlus() && shift.IsLSL() && (amount <= 3) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf8000000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount << 4));
        AdvanceIT();
        return;
      }
    } else {
      // STRB{<c>}{<q>} <Rt>, [<Rn>, {+/-}<Rm>{, <shift>}] ; A1
      if (operand.IsShiftValid() && operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x07400000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
      // STRB{<c>}{<q>} <Rt>, [<Rn>], {+/-}<Rm>{, <shift>} ; A1
      if (operand.IsShiftValid() && operand.IsPostIndex() &&
          cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x06400000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
      // STRB{<c>}{<q>} <Rt>, [<Rn>, {+/-}<Rm>{, <shift>}]! ; A1
      if (operand.IsShiftValid() && operand.IsPreIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        uint32_t shift_ = TypeEncodingValue(shift);
        uint32_t imm_and_type_ = (((amount % 32) << 2) | shift_);
        EmitA32(0x07600000U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23) | ((imm_and_type_ & 0x7f) << 5));
        return;
      }
    }
  }
  Delegate(kStrb, &Assembler::strb, cond, size, rt, operand);
}

void Assembler::strd(Condition cond,
                     Register rt,
                     Register rt2,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // STRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>{, #{+/-}<imm>}] ; T1
      if ((offset >= -1020) && (offset <= 1020) && ((offset % 4) == 0) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rn.IsPC() && !rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xe9400000U | (rt.GetCode() << 12) | (rt2.GetCode() << 8) |
                   (rn.GetCode() << 16) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
      // STRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>], #{+/-}<imm> ; T1
      if ((offset >= -1020) && (offset <= 1020) && ((offset % 4) == 0) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rn.IsPC() && !rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xe8600000U | (rt.GetCode() << 12) | (rt2.GetCode() << 8) |
                   (rn.GetCode() << 16) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
      // STRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>{, #{+/-}<imm>}]! ; T1
      if ((offset >= -1020) && (offset <= 1020) && ((offset % 4) == 0) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rn.IsPC() && !rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xe9600000U | (rt.GetCode() << 12) | (rt2.GetCode() << 8) |
                   (rn.GetCode() << 16) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // STRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>{, #{+/-}<imm_1>}] ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          (offset >= -255) && (offset <= 255) && operand.IsOffset() &&
          cond.IsNotNever() && ((((rt.GetCode() & 1) == 0) && !rt2.IsPC()) ||
                                AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x014000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // STRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>], #{+/-}<imm_1> ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          (offset >= -255) && (offset <= 255) && operand.IsPostIndex() &&
          cond.IsNotNever() && ((((rt.GetCode() & 1) == 0) && !rt2.IsPC()) ||
                                AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x004000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // STRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>{, #{+/-}<imm_1>}]! ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          (offset >= -255) && (offset <= 255) && operand.IsPreIndex() &&
          cond.IsNotNever() && ((((rt.GetCode() & 1) == 0) && !rt2.IsPC()) ||
                                AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x016000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingA32()) {
      // STRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>, #{+/-}<Rm>] ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsOffset() && cond.IsNotNever() &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC() && !rm.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x010000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // STRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>], #{+/-}<Rm> ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsPostIndex() && cond.IsNotNever() &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC() && !rm.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x000000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // STRD{<c>}{<q>} <Rt>, <Rt2>, [<Rn>, #{+/-}<Rm>]! ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsPreIndex() && cond.IsNotNever() &&
          ((((rt.GetCode() & 1) == 0) && !rt2.IsPC() && !rm.IsPC()) ||
           AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x012000f0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
    }
  }
  Delegate(kStrd, &Assembler::strd, cond, rt, rt2, operand);
}

void Assembler::strex(Condition cond,
                      Register rd,
                      Register rt,
                      const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // STREX{<c>}{<q>} <Rd>, <Rt>, [<Rn>{, #<imm>}] ; T1
      if ((offset >= 0) && (offset <= 1020) && ((offset % 4) == 0) &&
          operand.IsOffset() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        int32_t offset_ = offset >> 2;
        EmitT32_32(0xe8400000U | (rd.GetCode() << 8) | (rt.GetCode() << 12) |
                   (rn.GetCode() << 16) | (offset_ & 0xff));
        AdvanceIT();
        return;
      }
    } else {
      // STREX{<c>}{<q>} <Rd>, <Rt>, [<Rn>{, #<imm_1>}] ; A1
      if ((offset == 0) && operand.IsOffset() && cond.IsNotNever() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01800f90U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rt.GetCode() | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStrex, &Assembler::strex, cond, rd, rt, operand);
}

void Assembler::strexb(Condition cond,
                       Register rd,
                       Register rt,
                       const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STREXB{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8c00f40U | rd.GetCode() | (rt.GetCode() << 12) |
                   (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STREXB{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01c00f90U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rt.GetCode() | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStrexb, &Assembler::strexb, cond, rd, rt, operand);
}

void Assembler::strexd(Condition cond,
                       Register rd,
                       Register rt,
                       Register rt2,
                       const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STREXD{<c>}{<q>} <Rd>, <Rt>, <Rt2>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rd.IsPC() && !rt.IsPC() && !rt2.IsPC() && !rn.IsPC()) ||
           AllowUnpredictable())) {
        EmitT32_32(0xe8c00070U | rd.GetCode() | (rt.GetCode() << 12) |
                   (rt2.GetCode() << 8) | (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STREXD{<c>}{<q>} <Rd>, <Rt>, <Rt2>, [<Rn>] ; A1
      if ((((rt.GetCode() + 1) % kNumberOfRegisters) == rt2.GetCode()) &&
          operand.IsOffset() && cond.IsNotNever() &&
          ((!rd.IsPC() && ((rt.GetCode() & 1) == 0) && !rt2.IsPC() &&
            !rn.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x01a00f90U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rt.GetCode() | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStrexd, &Assembler::strexd, cond, rd, rt, rt2, operand);
}

void Assembler::strexh(Condition cond,
                       Register rd,
                       Register rt,
                       const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    if (IsUsingT32()) {
      // STREXH{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; T1
      if (operand.IsOffset() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xe8c00f50U | rd.GetCode() | (rt.GetCode() << 12) |
                   (rn.GetCode() << 16));
        AdvanceIT();
        return;
      }
    } else {
      // STREXH{<c>}{<q>} <Rd>, <Rt>, [<Rn>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rd.IsPC() && !rt.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01e00f90U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rt.GetCode() | (rn.GetCode() << 16));
        return;
      }
    }
  }
  Delegate(kStrexh, &Assembler::strexh, cond, rd, rt, operand);
}

void Assembler::strh(Condition cond,
                     EncodingSize size,
                     Register rt,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // STRH{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm>}] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && (offset >= 0) &&
          (offset <= 62) && ((offset % 2) == 0) && operand.IsOffset()) {
        int32_t offset_ = offset >> 1;
        EmitT32_16(0x8000 | rt.GetCode() | (rn.GetCode() << 3) |
                   ((offset_ & 0x1f) << 6));
        AdvanceIT();
        return;
      }
      // STRH{<c>}{<q>} <Rt>, [<Rn>{, #{+}<imm_1>}] ; T2
      if (!size.IsNarrow() && (offset >= 0) && (offset <= 4095) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf8a00000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (offset & 0xfff));
        AdvanceIT();
        return;
      }
      // STRH{<c>}{<q>} <Rt>, [<Rn>{, #-<imm_2>}] ; T3
      if (!size.IsNarrow() && (-offset >= 0) && (-offset <= 255) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf8200c00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   (-offset & 0xff));
        AdvanceIT();
        return;
      }
      // STRH{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_2> ; T3
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPostIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8200900U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
      // STRH{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_2>}]! ; T3
      if (!size.IsNarrow() && (offset >= -255) && (offset <= 255) &&
          operand.IsPreIndex() && ((rn.GetCode() & 0xf) != 0xf) &&
          (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitT32_32(0xf8200d00U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 9));
        AdvanceIT();
        return;
      }
    } else {
      // STRH{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}] ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsOffset() &&
          cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x014000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // STRH{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<imm_3> ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsPostIndex() &&
          cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x004000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
      // STRH{<c>}{<q>} <Rt>, [<Rn>{, #{+/-}<imm_3>}]! ; A1
      if ((offset >= -255) && (offset <= 255) && operand.IsPreIndex() &&
          cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset);
        EmitA32(0x016000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | (offset_ & 0xf) |
                ((offset_ & 0xf0) << 4) | (sign << 23));
        return;
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    if (IsUsingT32()) {
      // STRH{<c>}{<q>} <Rt>, [<Rn>, #{+}<Rm>] ; T1
      if (!size.IsWide() && rt.IsLow() && rn.IsLow() && rm.IsLow() &&
          sign.IsPlus() && operand.IsOffset()) {
        EmitT32_16(0x5200 | rt.GetCode() | (rn.GetCode() << 3) |
                   (rm.GetCode() << 6));
        AdvanceIT();
        return;
      }
    } else {
      // STRH{<c>}{<q>} <Rt>, [<Rn>, #{+/-}<Rm>] ; A1
      if (operand.IsOffset() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x010000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // STRH{<c>}{<q>} <Rt>, [<Rn>], #{+/-}<Rm> ; A1
      if (operand.IsPostIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x000000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
      // STRH{<c>}{<q>} <Rt>, [<Rn>, #{+/-}<Rm>]! ; A1
      if (operand.IsPreIndex() && cond.IsNotNever() &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t sign_ = sign.IsPlus() ? 1 : 0;
        EmitA32(0x012000b0U | (cond.GetCondition() << 28) |
                (rt.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (sign_ << 23));
        return;
      }
    }
  }
  if (operand.IsShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // STRH{<c>}{<q>} <Rt>, [<Rn>, {+}<Rm>{, LSL #<imm>}] ; T2
      if (!size.IsNarrow() && sign.IsPlus() && shift.IsLSL() && (amount <= 3) &&
          operand.IsOffset() && ((rn.GetCode() & 0xf) != 0xf) &&
          ((!rt.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf8200000U | (rt.GetCode() << 12) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount << 4));
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kStrh, &Assembler::strh, cond, size, rt, operand);
}

void Assembler::sub(Condition cond,
                    EncodingSize size,
                    Register rd,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // SUB<c>{<q>} <Rd>, <Rn>, #<imm3> ; T1
      if (InITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
          (imm <= 7)) {
        EmitT32_16(0x1e00 | rd.GetCode() | (rn.GetCode() << 3) | (imm << 6));
        AdvanceIT();
        return;
      }
      // SUB<c>{<q>} {<Rdn>}, <Rdn>, #<imm8> ; T2
      if (InITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
          (imm <= 255)) {
        EmitT32_16(0x3800 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
      // SUB{<c>}{<q>} {SP}, SP, #<imm7> ; T1
      if (!size.IsWide() && rd.Is(sp) && rn.Is(sp) && (imm <= 508) &&
          ((imm % 4) == 0)) {
        uint32_t imm_ = imm >> 2;
        EmitT32_16(0xb080 | imm_);
        AdvanceIT();
        return;
      }
      // SUB{<c>}{<q>} <Rd>, PC, #<imm12> ; T2
      if (!size.IsNarrow() && rn.Is(pc) && (imm <= 4095) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2af0000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // SUB{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T3
      if (!size.IsNarrow() && immediate_t32.IsValid() && !rn.Is(sp) &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf1a00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // SUB{<c>}{<q>} {<Rd>}, <Rn>, #<imm12> ; T4
      if (!size.IsNarrow() && (imm <= 4095) && ((rn.GetCode() & 0xd) != 0xd) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2a00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (imm & 0xff) | ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // SUB{<c>}{<q>} {<Rd>}, SP, #<const> ; T2
      if (!size.IsNarrow() && rn.Is(sp) && immediate_t32.IsValid() &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf1ad0000U | (rd.GetCode() << 8) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // SUB{<c>}{<q>} {<Rd>}, SP, #<imm12> ; T3
      if (!size.IsNarrow() && rn.Is(sp) && (imm <= 4095) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2ad0000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // SUB{<c>}{<q>} <Rd>, PC, #<const> ; A2
      if (rn.Is(pc) && immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x024f0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
      // SUB{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever() &&
          ((rn.GetCode() & 0xd) != 0xd)) {
        EmitA32(0x02400000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
      // SUB{<c>}{<q>} {<Rd>}, SP, #<const> ; A1
      if (rn.Is(sp) && immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x024d0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // SUB<c>{<q>} <Rd>, <Rn>, <Rm> ; T1
        if (InITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x1a00 | rd.GetCode() | (rn.GetCode() << 3) |
                     (rm.GetCode() << 6));
          AdvanceIT();
          return;
        }
        // SUB{<c>} {<Rd>}, SP, <Rm> ; T1
        if (rn.Is(sp) && ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
          EmitT32_32(0xebad0000U | (rd.GetCode() << 8) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SUB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) && !rn.Is(sp) &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xeba00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
      // SUB{<c>}{<q>} {<Rd>}, SP, <Rm> {, <shift> #<amount> } ; T1
      if (!size.IsNarrow() && rn.Is(sp) && shift.IsValidAmount(amount) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xebad0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // SUB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever() && !rn.Is(sp)) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00400000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
      // SUB{<c>}{<q>} {<Rd>}, SP, <Rm> {, <shift> #<amount> } ; A1
      if (rn.Is(sp) && shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x004d0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // SUB{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00400010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kSub, &Assembler::sub, cond, size, rd, rn, operand);
}

void Assembler::sub(Condition cond, Register rd, const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // SUB<c>{<q>} <Rdn>, #<imm8> ; T2
      if (InITBlock() && rd.IsLow() && (imm <= 255)) {
        EmitT32_16(0x3800 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kSub, &Assembler::sub, cond, rd, operand);
}

void Assembler::subs(Condition cond,
                     EncodingSize size,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // SUBS{<q>} <Rd>, <Rn>, #<imm3> ; T1
      if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
          (imm <= 7)) {
        EmitT32_16(0x1e00 | rd.GetCode() | (rn.GetCode() << 3) | (imm << 6));
        AdvanceIT();
        return;
      }
      // SUBS{<q>} {<Rdn>}, <Rdn>, #<imm8> ; T2
      if (OutsideITBlock() && !size.IsWide() && rd.Is(rn) && rn.IsLow() &&
          (imm <= 255)) {
        EmitT32_16(0x3800 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
      // SUBS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; T3
      if (!size.IsNarrow() && immediate_t32.IsValid() && !rn.Is(sp) &&
          !rd.Is(pc) && (!rn.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf1b00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // SUBS{<c>}{<q>} PC, LR, #<imm8> ; T5
      if (!size.IsNarrow() && rd.Is(pc) && rn.Is(lr) && (imm <= 255) &&
          (OutsideITBlockAndAlOrLast(cond) || AllowUnpredictable())) {
        EmitT32_32(0xf3de8f00U | imm);
        AdvanceIT();
        return;
      }
      // SUBS{<c>}{<q>} {<Rd>}, SP, #<const> ; T2
      if (!size.IsNarrow() && rn.Is(sp) && immediate_t32.IsValid() &&
          !rd.Is(pc)) {
        EmitT32_32(0xf1bd0000U | (rd.GetCode() << 8) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // SUBS{<c>}{<q>} {<Rd>}, <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever() && !rn.Is(sp)) {
        EmitA32(0x02500000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) |
                immediate_a32.GetEncodingValue());
        return;
      }
      // SUBS{<c>}{<q>} {<Rd>}, SP, #<const> ; A1
      if (rn.Is(sp) && immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x025d0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // SUBS{<q>} {<Rd>}, <Rn>, <Rm> ; T1
        if (OutsideITBlock() && !size.IsWide() && rd.IsLow() && rn.IsLow() &&
            rm.IsLow()) {
          EmitT32_16(0x1a00 | rd.GetCode() | (rn.GetCode() << 3) |
                     (rm.GetCode() << 6));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SUBS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) && !rn.Is(sp) &&
          !rd.Is(pc) && ((!rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xebb00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
      // SUBS{<c>}{<q>} {<Rd>}, SP, <Rm> {, <shift> #<amount> } ; T1
      if (!size.IsNarrow() && rn.Is(sp) && shift.IsValidAmount(amount) &&
          !rd.Is(pc) && (!rm.IsPC() || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xebbd0000U | (rd.GetCode() << 8) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // SUBS{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever() && !rn.Is(sp)) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x00500000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
      // SUBS{<c>}{<q>} {<Rd>}, SP, <Rm> {, <shift> #<amount> } ; A1
      if (rn.Is(sp) && shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x005d0000U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // SUBS{<c>}{<q>} {<Rd>}, <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC() && !rs.IsPC()) ||
           AllowUnpredictable())) {
        EmitA32(0x00500010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (shift.GetType() << 5) | (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kSubs, &Assembler::subs, cond, size, rd, rn, operand);
}

void Assembler::subs(Register rd, const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // SUBS{<q>} <Rdn>, #<imm8> ; T2
      if (OutsideITBlock() && rd.IsLow() && (imm <= 255)) {
        EmitT32_16(0x3800 | (rd.GetCode() << 8) | imm);
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kSubs, &Assembler::subs, rd, operand);
}

void Assembler::subw(Condition cond,
                     Register rd,
                     Register rn,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      // SUBW{<c>}{<q>} {<Rd>}, <Rn>, #<imm12> ; T4
      if ((imm <= 4095) && ((rn.GetCode() & 0xd) != 0xd) &&
          (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2a00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   (imm & 0xff) | ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
      // SUBW{<c>}{<q>} {<Rd>}, SP, #<imm12> ; T3
      if (rn.Is(sp) && (imm <= 4095) && (!rd.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf2ad0000U | (rd.GetCode() << 8) | (imm & 0xff) |
                   ((imm & 0x700) << 4) | ((imm & 0x800) << 15));
        AdvanceIT();
        return;
      }
    }
  }
  Delegate(kSubw, &Assembler::subw, cond, rd, rn, operand);
}

void Assembler::svc(Condition cond, uint32_t imm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // SVC{<c>}{<q>} {#}<imm> ; T1
    if ((imm <= 255)) {
      EmitT32_16(0xdf00 | imm);
      AdvanceIT();
      return;
    }
  } else {
    // SVC{<c>}{<q>} {#}<imm> ; A1
    if ((imm <= 16777215) && cond.IsNotNever()) {
      EmitA32(0x0f000000U | (cond.GetCondition() << 28) | imm);
      return;
    }
  }
  Delegate(kSvc, &Assembler::svc, cond, imm);
}

void Assembler::sxtab(Condition cond,
                      Register rd,
                      Register rn,
                      const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SXTAB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; T1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa40f080U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // SXTAB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06a00070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kSxtab, &Assembler::sxtab, cond, rd, rn, operand);
}

void Assembler::sxtab16(Condition cond,
                        Register rd,
                        Register rn,
                        const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SXTAB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; T1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa20f080U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // SXTAB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06800070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kSxtab16, &Assembler::sxtab16, cond, rd, rn, operand);
}

void Assembler::sxtah(Condition cond,
                      Register rd,
                      Register rn,
                      const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SXTAH{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; T1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa00f080U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // SXTAH{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06b00070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kSxtah, &Assembler::sxtah, cond, rd, rn, operand);
}

void Assembler::sxtb(Condition cond,
                     EncodingSize size,
                     Register rd,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // SXTB{<c>}{<q>} {<Rd>}, <Rm> ; T1
        if (!size.IsWide() && rd.IsLow() && rm.IsLow()) {
          EmitT32_16(0xb240 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SXTB{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; T2
      if (!size.IsNarrow() && (shift.IsROR() || (amount == 0)) &&
          (amount <= 24) && ((amount % 8) == 0) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa4ff080U | (rd.GetCode() << 8) | rm.GetCode() |
                   (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // SXTB{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06af0070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kSxtb, &Assembler::sxtb, cond, size, rd, operand);
}

void Assembler::sxtb16(Condition cond, Register rd, const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SXTB16{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; T1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa2ff080U | (rd.GetCode() << 8) | rm.GetCode() |
                   (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // SXTB16{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x068f0070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kSxtb16, &Assembler::sxtb16, cond, rd, operand);
}

void Assembler::sxth(Condition cond,
                     EncodingSize size,
                     Register rd,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // SXTH{<c>}{<q>} {<Rd>}, <Rm> ; T1
        if (!size.IsWide() && rd.IsLow() && rm.IsLow()) {
          EmitT32_16(0xb200 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // SXTH{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; T2
      if (!size.IsNarrow() && (shift.IsROR() || (amount == 0)) &&
          (amount <= 24) && ((amount % 8) == 0) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa0ff080U | (rd.GetCode() << 8) | rm.GetCode() |
                   (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // SXTH{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06bf0070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kSxth, &Assembler::sxth, cond, size, rd, operand);
}

void Assembler::tbb(Condition cond, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // TBB{<c>}{<q>} [<Rn>, <Rm>] ; T1
    if (OutsideITBlockAndAlOrLast(cond) &&
        (!rm.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe8d0f000U | (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  }
  Delegate(kTbb, &Assembler::tbb, cond, rn, rm);
}

void Assembler::tbh(Condition cond, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // TBH{<c>}{<q>} [<Rn>, <Rm>, LSL #1] ; T1
    if (OutsideITBlockAndAlOrLast(cond) &&
        (!rm.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xe8d0f010U | (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  }
  Delegate(kTbh, &Assembler::tbh, cond, rn, rm);
}

void Assembler::teq(Condition cond, Register rn, const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // TEQ{<c>}{<q>} <Rn>, #<const> ; T1
      if (immediate_t32.IsValid() && (!rn.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf0900f00U | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // TEQ{<c>}{<q>} <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03300000U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // TEQ{<c>}{<q>} <Rn>, <Rm> {, <shift> #<amount> } ; T1
      if (shift.IsValidAmount(amount) &&
          ((!rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea900f00U | (rn.GetCode() << 16) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // TEQ{<c>}{<q>} <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01300000U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // TEQ{<c>}{<q>} <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rn.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01300010U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | rm.GetCode() | (shift.GetType() << 5) |
                (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kTeq, &Assembler::teq, cond, rn, operand);
}

void Assembler::tst(Condition cond,
                    EncodingSize size,
                    Register rn,
                    const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    uint32_t imm = operand.GetImmediate();
    if (IsUsingT32()) {
      ImmediateT32 immediate_t32(imm);
      // TST{<c>}{<q>} <Rn>, #<const> ; T1
      if (!size.IsNarrow() && immediate_t32.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        EmitT32_32(0xf0100f00U | (rn.GetCode() << 16) |
                   (immediate_t32.GetEncodingValue() & 0xff) |
                   ((immediate_t32.GetEncodingValue() & 0x700) << 4) |
                   ((immediate_t32.GetEncodingValue() & 0x800) << 15));
        AdvanceIT();
        return;
      }
    } else {
      ImmediateA32 immediate_a32(imm);
      // TST{<c>}{<q>} <Rn>, #<const> ; A1
      if (immediate_a32.IsValid() && cond.IsNotNever()) {
        EmitA32(0x03100000U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | immediate_a32.GetEncodingValue());
        return;
      }
    }
  }
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // TST{<c>}{<q>} <Rn>, <Rm> ; T1
        if (!size.IsWide() && rn.IsLow() && rm.IsLow()) {
          EmitT32_16(0x4200 | rn.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // TST{<c>}{<q>} <Rn>, <Rm> {, <shift> #<amount> } ; T2
      if (!size.IsNarrow() && shift.IsValidAmount(amount) &&
          ((!rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitT32_32(0xea100f00U | (rn.GetCode() << 16) | rm.GetCode() |
                   (operand.GetTypeEncodingValue() << 4) |
                   ((amount_ & 0x3) << 6) | ((amount_ & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // TST{<c>}{<q>} <Rn>, <Rm> {, <shift> #<amount> } ; A1
      if (shift.IsValidAmount(amount) && cond.IsNotNever()) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x01100000U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | rm.GetCode() |
                (operand.GetTypeEncodingValue() << 5) | (amount_ << 7));
        return;
      }
    }
  }
  if (operand.IsRegisterShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    Register rs = operand.GetShiftRegister();
    if (IsUsingA32()) {
      // TST{<c>}{<q>} <Rn>, <Rm>, <shift> <Rs> ; A1
      if (cond.IsNotNever() &&
          ((!rn.IsPC() && !rm.IsPC() && !rs.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x01100010U | (cond.GetCondition() << 28) |
                (rn.GetCode() << 16) | rm.GetCode() | (shift.GetType() << 5) |
                (rs.GetCode() << 8));
        return;
      }
    }
  }
  Delegate(kTst, &Assembler::tst, cond, size, rn, operand);
}

void Assembler::uadd16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f040U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06500f10U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUadd16, &Assembler::uadd16, cond, rd, rn, rm);
}

void Assembler::uadd8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f040U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06500f90U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUadd8, &Assembler::uadd8, cond, rd, rn, rm);
}

void Assembler::uasx(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfaa0f040U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06500f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUasx, &Assembler::uasx, cond, rd, rn, rm);
}

void Assembler::ubfx(
    Condition cond, Register rd, Register rn, uint32_t lsb, uint32_t width) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UBFX{<c>}{<q>} <Rd>, <Rn>, #<lsb>, #<width> ; T1
    if ((lsb <= 31) &&
        (((width >= 1) && (width <= 32 - lsb) && !rd.IsPC() && !rn.IsPC()) ||
         AllowUnpredictable())) {
      uint32_t widthm1 = width - 1;
      EmitT32_32(0xf3c00000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 ((lsb & 0x3) << 6) | ((lsb & 0x1c) << 10) | widthm1);
      AdvanceIT();
      return;
    }
  } else {
    // UBFX{<c>}{<q>} <Rd>, <Rn>, #<lsb>, #<width> ; A1
    if ((lsb <= 31) && cond.IsNotNever() &&
        (((width >= 1) && (width <= 32 - lsb) && !rd.IsPC() && !rn.IsPC()) ||
         AllowUnpredictable())) {
      uint32_t widthm1 = width - 1;
      EmitA32(0x07e00050U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              rn.GetCode() | (lsb << 7) | (widthm1 << 16));
      return;
    }
  }
  Delegate(kUbfx, &Assembler::ubfx, cond, rd, rn, lsb, width);
}

void Assembler::udf(Condition cond, EncodingSize size, uint32_t imm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UDF{<c>}{<q>} {#}<imm> ; T1
    if (!size.IsWide() && (imm <= 255)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_16(0xde00 | imm);
        AdvanceIT();
        return;
      }
    }
    // UDF{<c>}{<q>} {#}<imm> ; T2
    if (!size.IsNarrow() && (imm <= 65535)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xf7f0a000U | (imm & 0xfff) | ((imm & 0xf000) << 4));
        AdvanceIT();
        return;
      }
    }
  } else {
    // UDF{<c>}{<q>} {#}<imm> ; A1
    if ((imm <= 65535)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitA32(0xe7f000f0U | (imm & 0xf) | ((imm & 0xfff0) << 4));
        return;
      }
    }
  }
  Delegate(kUdf, &Assembler::udf, cond, size, imm);
}

void Assembler::udiv(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UDIV{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfbb0f0f0U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UDIV{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0730f010U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kUdiv, &Assembler::udiv, cond, rd, rn, rm);
}

void Assembler::uhadd16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UHADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f060U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UHADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06700f10U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUhadd16, &Assembler::uhadd16, cond, rd, rn, rm);
}

void Assembler::uhadd8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UHADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f060U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UHADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06700f90U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUhadd8, &Assembler::uhadd8, cond, rd, rn, rm);
}

void Assembler::uhasx(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UHASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfaa0f060U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UHASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06700f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUhasx, &Assembler::uhasx, cond, rd, rn, rm);
}

void Assembler::uhsax(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UHSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfae0f060U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UHSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06700f50U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUhsax, &Assembler::uhsax, cond, rd, rn, rm);
}

void Assembler::uhsub16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UHSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfad0f060U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UHSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06700f70U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUhsub16, &Assembler::uhsub16, cond, rd, rn, rm);
}

void Assembler::uhsub8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UHSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfac0f060U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UHSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06700ff0U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUhsub8, &Assembler::uhsub8, cond, rd, rn, rm);
}

void Assembler::umaal(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UMAAL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbe00060U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UMAAL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00400090U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kUmaal, &Assembler::umaal, cond, rdlo, rdhi, rn, rm);
}

void Assembler::umlal(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UMLAL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfbe00000U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UMLAL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00a00090U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kUmlal, &Assembler::umlal, cond, rdlo, rdhi, rn, rm);
}

void Assembler::umlals(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // UMLALS{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00b00090U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kUmlals, &Assembler::umlals, cond, rdlo, rdhi, rn, rm);
}

void Assembler::umull(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UMULL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; T1
    if (((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitT32_32(0xfba00000U | (rdlo.GetCode() << 12) | (rdhi.GetCode() << 8) |
                 (rn.GetCode() << 16) | rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UMULL{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00800090U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kUmull, &Assembler::umull, cond, rdlo, rdhi, rn, rm);
}

void Assembler::umulls(
    Condition cond, Register rdlo, Register rdhi, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingA32()) {
    // UMULLS{<c>}{<q>} <Rd>, <Rd>, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rdlo.IsPC() && !rdhi.IsPC() && !rn.IsPC() && !rm.IsPC()) ||
         AllowUnpredictable())) {
      EmitA32(0x00900090U | (cond.GetCondition() << 28) |
              (rdlo.GetCode() << 12) | (rdhi.GetCode() << 16) | rn.GetCode() |
              (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kUmulls, &Assembler::umulls, cond, rdlo, rdhi, rn, rm);
}

void Assembler::uqadd16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UQADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa90f050U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UQADD16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06600f10U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUqadd16, &Assembler::uqadd16, cond, rd, rn, rm);
}

void Assembler::uqadd8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UQADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfa80f050U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UQADD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06600f90U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUqadd8, &Assembler::uqadd8, cond, rd, rn, rm);
}

void Assembler::uqasx(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UQASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfaa0f050U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UQASX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06600f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUqasx, &Assembler::uqasx, cond, rd, rn, rm);
}

void Assembler::uqsax(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UQSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfae0f050U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UQSAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06600f50U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUqsax, &Assembler::uqsax, cond, rd, rn, rm);
}

void Assembler::uqsub16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UQSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfad0f050U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UQSUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06600f70U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUqsub16, &Assembler::uqsub16, cond, rd, rn, rm);
}

void Assembler::uqsub8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // UQSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfac0f050U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // UQSUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06600ff0U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUqsub8, &Assembler::uqsub8, cond, rd, rn, rm);
}

void Assembler::usad8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // USAD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb70f000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // USAD8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0780f010U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8));
      return;
    }
  }
  Delegate(kUsad8, &Assembler::usad8, cond, rd, rn, rm);
}

void Assembler::usada8(
    Condition cond, Register rd, Register rn, Register rm, Register ra) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // USADA8{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; T1
    if (!ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfb700000U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode() | (ra.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // USADA8{<c>}{<q>} <Rd>, <Rn>, <Rm>, <Ra> ; A1
    if (cond.IsNotNever() && !ra.Is(pc) &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x07800010U | (cond.GetCondition() << 28) | (rd.GetCode() << 16) |
              rn.GetCode() | (rm.GetCode() << 8) | (ra.GetCode() << 12));
      return;
    }
  }
  Delegate(kUsada8, &Assembler::usada8, cond, rd, rn, rm, ra);
}

void Assembler::usat(Condition cond,
                     Register rd,
                     uint32_t imm,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rn = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // USAT{<c>}{<q>} <Rd>, #<imm>, <Rn>, ASR #<amount> ; T1
      if ((imm <= 31) && shift.IsASR() && (amount >= 1) && (amount <= 31) &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf3a00000U | (rd.GetCode() << 8) | imm |
                   (rn.GetCode() << 16) | ((amount & 0x3) << 6) |
                   ((amount & 0x1c) << 10));
        AdvanceIT();
        return;
      }
      // USAT{<c>}{<q>} <Rd>, #<imm>, <Rn> {, LSL #<amount> } ; T1
      if ((imm <= 31) && shift.IsLSL() && (amount <= 31) &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitT32_32(0xf3800000U | (rd.GetCode() << 8) | imm |
                   (rn.GetCode() << 16) | ((amount & 0x3) << 6) |
                   ((amount & 0x1c) << 10));
        AdvanceIT();
        return;
      }
    } else {
      // USAT{<c>}{<q>} <Rd>, #<imm>, <Rn>, ASR #<amount> ; A1
      if ((imm <= 31) && shift.IsASR() && (amount >= 1) && (amount <= 32) &&
          cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount % 32;
        EmitA32(0x06e00050U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (imm << 16) | rn.GetCode() |
                (amount_ << 7));
        return;
      }
      // USAT{<c>}{<q>} <Rd>, #<imm>, <Rn> {, LSL #<amount> } ; A1
      if ((imm <= 31) && shift.IsLSL() && (amount <= 31) && cond.IsNotNever() &&
          ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
        EmitA32(0x06e00010U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (imm << 16) | rn.GetCode() |
                (amount << 7));
        return;
      }
    }
  }
  Delegate(kUsat, &Assembler::usat, cond, rd, imm, operand);
}

void Assembler::usat16(Condition cond, Register rd, uint32_t imm, Register rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // USAT16{<c>}{<q>} <Rd>, #<imm>, <Rn> ; T1
    if ((imm <= 15) && ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xf3a00000U | (rd.GetCode() << 8) | imm |
                 (rn.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // USAT16{<c>}{<q>} <Rd>, #<imm>, <Rn> ; A1
    if ((imm <= 15) && cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06e00f30U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (imm << 16) | rn.GetCode());
      return;
    }
  }
  Delegate(kUsat16, &Assembler::usat16, cond, rd, imm, rn);
}

void Assembler::usax(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // USAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfae0f040U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // USAX{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06500f50U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUsax, &Assembler::usax, cond, rd, rn, rm);
}

void Assembler::usub16(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // USUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfad0f040U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // USUB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06500f70U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUsub16, &Assembler::usub16, cond, rd, rn, rm);
}

void Assembler::usub8(Condition cond, Register rd, Register rn, Register rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // USUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; T1
    if (((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xfac0f040U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                 rm.GetCode());
      AdvanceIT();
      return;
    }
  } else {
    // USUB8{<c>}{<q>} {<Rd>}, <Rn>, <Rm> ; A1
    if (cond.IsNotNever() &&
        ((!rd.IsPC() && !rn.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x06500ff0U | (cond.GetCondition() << 28) | (rd.GetCode() << 12) |
              (rn.GetCode() << 16) | rm.GetCode());
      return;
    }
  }
  Delegate(kUsub8, &Assembler::usub8, cond, rd, rn, rm);
}

void Assembler::uxtab(Condition cond,
                      Register rd,
                      Register rn,
                      const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // UXTAB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; T1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa50f080U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // UXTAB{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06e00070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kUxtab, &Assembler::uxtab, cond, rd, rn, operand);
}

void Assembler::uxtab16(Condition cond,
                        Register rd,
                        Register rn,
                        const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // UXTAB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; T1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa30f080U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // UXTAB16{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06c00070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kUxtab16, &Assembler::uxtab16, cond, rd, rn, operand);
}

void Assembler::uxtah(Condition cond,
                      Register rd,
                      Register rn,
                      const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // UXTAH{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; T1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa10f080U | (rd.GetCode() << 8) | (rn.GetCode() << 16) |
                   rm.GetCode() | (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // UXTAH{<c>}{<q>} {<Rd>}, <Rn>, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() && !rn.Is(pc) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06f00070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | (rn.GetCode() << 16) | rm.GetCode() |
                (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kUxtah, &Assembler::uxtah, cond, rd, rn, operand);
}

void Assembler::uxtb(Condition cond,
                     EncodingSize size,
                     Register rd,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // UXTB{<c>}{<q>} {<Rd>}, <Rm> ; T1
        if (!size.IsWide() && rd.IsLow() && rm.IsLow()) {
          EmitT32_16(0xb2c0 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // UXTB{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; T2
      if (!size.IsNarrow() && (shift.IsROR() || (amount == 0)) &&
          (amount <= 24) && ((amount % 8) == 0) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa5ff080U | (rd.GetCode() << 8) | rm.GetCode() |
                   (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // UXTB{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06ef0070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kUxtb, &Assembler::uxtb, cond, size, rd, operand);
}

void Assembler::uxtb16(Condition cond, Register rd, const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // UXTB16{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; T1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa3ff080U | (rd.GetCode() << 8) | rm.GetCode() |
                   (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // UXTB16{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06cf0070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kUxtb16, &Assembler::uxtb16, cond, rd, operand);
}

void Assembler::uxth(Condition cond,
                     EncodingSize size,
                     Register rd,
                     const Operand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateShiftedRegister()) {
    Register rm = operand.GetBaseRegister();
    if (operand.IsPlainRegister()) {
      if (IsUsingT32()) {
        // UXTH{<c>}{<q>} {<Rd>}, <Rm> ; T1
        if (!size.IsWide() && rd.IsLow() && rm.IsLow()) {
          EmitT32_16(0xb280 | rd.GetCode() | (rm.GetCode() << 3));
          AdvanceIT();
          return;
        }
      }
    }
    Shift shift = operand.GetShift();
    uint32_t amount = operand.GetShiftAmount();
    if (IsUsingT32()) {
      // UXTH{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; T2
      if (!size.IsNarrow() && (shift.IsROR() || (amount == 0)) &&
          (amount <= 24) && ((amount % 8) == 0) &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitT32_32(0xfa1ff080U | (rd.GetCode() << 8) | rm.GetCode() |
                   (amount_ << 4));
        AdvanceIT();
        return;
      }
    } else {
      // UXTH{<c>}{<q>} {<Rd>}, <Rm> {, ROR #<amount> } ; A1
      if ((shift.IsROR() || (amount == 0)) && (amount <= 24) &&
          ((amount % 8) == 0) && cond.IsNotNever() &&
          ((!rd.IsPC() && !rm.IsPC()) || AllowUnpredictable())) {
        uint32_t amount_ = amount / 8;
        EmitA32(0x06ff0070U | (cond.GetCondition() << 28) |
                (rd.GetCode() << 12) | rm.GetCode() | (amount_ << 10));
        return;
      }
    }
  }
  Delegate(kUxth, &Assembler::uxth, cond, size, rd, operand);
}

void Assembler::vaba(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VABA{<c>}{<q>}.<dt> <Dd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000710U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VABA{<c>}{<q>}.<dt> <Dd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000710U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVaba, &Assembler::vaba, cond, dt, rd, rn, rm);
}

void Assembler::vaba(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VABA{<c>}{<q>}.<dt> <Qd>, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000750U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VABA{<c>}{<q>}.<dt> <Qd>, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000750U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVaba, &Assembler::vaba, cond, dt, rd, rn, rm);
}

void Assembler::vabal(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VABAL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800500U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VABAL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800500U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVabal, &Assembler::vabal, cond, dt, rd, rn, rm);
}

void Assembler::vabd(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VABD{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200d00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VABD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000700U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VABD{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200d00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VABD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000700U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVabd, &Assembler::vabd, cond, dt, rd, rn, rm);
}

void Assembler::vabd(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VABD{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200d40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VABD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000740U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VABD{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200d40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VABD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000740U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVabd, &Assembler::vabd, cond, dt, rd, rn, rm);
}

void Assembler::vabdl(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VABDL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800700U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VABDL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800700U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVabdl, &Assembler::vabdl, cond, dt, rd, rn, rm);
}

void Assembler::vabs(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VABS{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb10300U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VABS{<c>}{<q>}.F64 <Dd>, <Dm> ; T2
    if (dt.Is(F64)) {
      EmitT32_32(0xeeb00bc0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VABS{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b10300U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
    // VABS{<c>}{<q>}.F64 <Dd>, <Dm> ; A2
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb00bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVabs, &Assembler::vabs, cond, dt, rd, rm);
}

void Assembler::vabs(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VABS{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb10340U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VABS{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b10340U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVabs, &Assembler::vabs, cond, dt, rd, rm);
}

void Assembler::vabs(Condition cond, DataType dt, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VABS{<c>}{<q>}.F32 <Sd>, <Sm> ; T2
    if (dt.Is(F32)) {
      EmitT32_32(0xeeb00ac0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VABS{<c>}{<q>}.F32 <Sd>, <Sm> ; A2
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb00ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVabs, &Assembler::vabs, cond, dt, rd, rm);
}

void Assembler::vacge(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VACGE{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000e10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VACGE{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000e10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVacge, &Assembler::vacge, cond, dt, rd, rn, rm);
}

void Assembler::vacge(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VACGE{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000e50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VACGE{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000e50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVacge, &Assembler::vacge, cond, dt, rd, rn, rm);
}

void Assembler::vacgt(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VACGT{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200e10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VACGT{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200e10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVacgt, &Assembler::vacgt, cond, dt, rd, rn, rm);
}

void Assembler::vacgt(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VACGT{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200e50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VACGT{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200e50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVacgt, &Assembler::vacgt, cond, dt, rd, rn, rm);
}

void Assembler::vacle(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VACLE{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000e10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VACLE{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000e10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVacle, &Assembler::vacle, cond, dt, rd, rn, rm);
}

void Assembler::vacle(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VACLE{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000e50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VACLE{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000e50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVacle, &Assembler::vacle, cond, dt, rd, rn, rm);
}

void Assembler::vaclt(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VACLT{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200e10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VACLT{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200e10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVaclt, &Assembler::vaclt, cond, dt, rd, rn, rm);
}

void Assembler::vaclt(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VACLT{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200e50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VACLT{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200e50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVaclt, &Assembler::vaclt, cond, dt, rd, rn, rm);
}

void Assembler::vadd(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_2 encoded_dt(dt);
  if (IsUsingT32()) {
    // VADD{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000d00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VADD{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; T2
    if (dt.Is(F64)) {
      EmitT32_32(0xee300b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000800U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VADD{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000d00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VADD{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; A2
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e300b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
    // VADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000800U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVadd, &Assembler::vadd, cond, dt, rd, rn, rm);
}

void Assembler::vadd(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_2 encoded_dt(dt);
  if (IsUsingT32()) {
    // VADD{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000d40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VADD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000840U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VADD{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000d40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VADD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000840U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVadd, &Assembler::vadd, cond, dt, rd, rn, rm);
}

void Assembler::vadd(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VADD{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; T2
    if (dt.Is(F32)) {
      EmitT32_32(0xee300a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VADD{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; A2
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e300a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVadd, &Assembler::vadd, cond, dt, rd, rn, rm);
}

void Assembler::vaddhn(
    Condition cond, DataType dt, DRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VADDHN{<c>}{<q>}.<dt> <Dd>, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid() && (dt.Is(I16) || dt.Is(I32) || dt.Is(I64))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800400U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VADDHN{<c>}{<q>}.<dt> <Dd>, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid() && (dt.Is(I16) || dt.Is(I32) || dt.Is(I64))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800400U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVaddhn, &Assembler::vaddhn, cond, dt, rd, rn, rm);
}

void Assembler::vaddl(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VADDL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800000U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VADDL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800000U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVaddl, &Assembler::vaddl, cond, dt, rd, rn, rm);
}

void Assembler::vaddw(
    Condition cond, DataType dt, QRegister rd, QRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VADDW{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800100U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VADDW{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800100U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVaddw, &Assembler::vaddw, cond, dt, rd, rn, rm);
}

void Assembler::vand(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rn,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVand encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VAND{<c>}{<q>}.<dt> {<Ddn>}, <Ddn>, #<imm> ; T1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800030U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VAND{<c>}{<q>}.<dt> {<Ddn>}, <Ddn>, #<imm> ; A1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al)) {
          EmitA32(0xf2800030U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  if (operand.IsRegister()) {
    DRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VAND{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VAND{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf2000110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVand, &Assembler::vand, cond, dt, rd, rn, operand);
}

void Assembler::vand(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rn,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVand encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VAND{<c>}{<q>}.<dt> {<Qdn>}, <Qdn>, #<imm> ; T1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800070U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VAND{<c>}{<q>}.<dt> {<Qdn>}, <Qdn>, #<imm> ; A1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al)) {
          EmitA32(0xf2800070U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  if (operand.IsRegister()) {
    QRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VAND{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VAND{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf2000150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVand, &Assembler::vand, cond, dt, rd, rn, operand);
}

void Assembler::vbic(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rn,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVbic encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VBIC{<c>}{<q>}.<dt> {<Ddn>}, <Ddn>, #<imm> ; T1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800030U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VBIC{<c>}{<q>}.<dt> {<Ddn>}, <Ddn>, #<imm> ; A1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al)) {
          EmitA32(0xf2800030U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  if (operand.IsRegister()) {
    DRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VBIC{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef100110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VBIC{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf2100110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVbic, &Assembler::vbic, cond, dt, rd, rn, operand);
}

void Assembler::vbic(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rn,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVbic encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VBIC{<c>}{<q>}.<dt> {<Qdn>}, <Qdn>, #<imm> ; T1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800070U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VBIC{<c>}{<q>}.<dt> {<Qdn>}, <Qdn>, #<imm> ; A1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al)) {
          EmitA32(0xf2800070U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  if (operand.IsRegister()) {
    QRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VBIC{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef100150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VBIC{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf2100150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVbic, &Assembler::vbic, cond, dt, rd, rn, operand);
}

void Assembler::vbif(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VBIF{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xff300110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VBIF{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3300110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVbif, &Assembler::vbif, cond, dt, rd, rn, rm);
}

void Assembler::vbif(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VBIF{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xff300150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VBIF{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3300150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVbif, &Assembler::vbif, cond, dt, rd, rn, rm);
}

void Assembler::vbit(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VBIT{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xff200110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VBIT{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3200110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVbit, &Assembler::vbit, cond, dt, rd, rn, rm);
}

void Assembler::vbit(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VBIT{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xff200150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VBIT{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3200150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVbit, &Assembler::vbit, cond, dt, rd, rn, rm);
}

void Assembler::vbsl(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VBSL{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xff100110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VBSL{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3100110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVbsl, &Assembler::vbsl, cond, dt, rd, rn, rm);
}

void Assembler::vbsl(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VBSL{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xff100150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VBSL{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3100150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVbsl, &Assembler::vbsl, cond, dt, rd, rn, rm);
}

void Assembler::vceq(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_2 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCEQ{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb10100U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCEQ{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b10100U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVceq, &Assembler::vceq, cond, dt, rd, rm, operand);
}

void Assembler::vceq(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_2 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCEQ{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb10140U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCEQ{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b10140U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVceq, &Assembler::vceq, cond, dt, rd, rm, operand);
}

void Assembler::vceq(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_4 encoded_dt(dt);
  Dt_sz_1 encoded_dt_2(dt);
  if (IsUsingT32()) {
    // VCEQ{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000810U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VCEQ{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T2
    if (encoded_dt_2.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000e00U | (encoded_dt_2.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCEQ{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3000810U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
    // VCEQ{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A2
    if (encoded_dt_2.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000e00U | (encoded_dt_2.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVceq, &Assembler::vceq, cond, dt, rd, rn, rm);
}

void Assembler::vceq(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_4 encoded_dt(dt);
  Dt_sz_1 encoded_dt_2(dt);
  if (IsUsingT32()) {
    // VCEQ{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000850U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VCEQ{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T2
    if (encoded_dt_2.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000e40U | (encoded_dt_2.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCEQ{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3000850U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
    // VCEQ{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A2
    if (encoded_dt_2.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000e40U | (encoded_dt_2.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVceq, &Assembler::vceq, cond, dt, rd, rn, rm);
}

void Assembler::vcge(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCGE{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb10080U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCGE{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b10080U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVcge, &Assembler::vcge, cond, dt, rd, rm, operand);
}

void Assembler::vcge(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCGE{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb100c0U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCGE{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b100c0U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVcge, &Assembler::vcge, cond, dt, rd, rm, operand);
}

void Assembler::vcge(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCGE{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000310U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VCGE{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T2
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000e00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCGE{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000310U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
    // VCGE{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A2
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000e00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcge, &Assembler::vcge, cond, dt, rd, rn, rm);
}

void Assembler::vcge(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCGE{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000350U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VCGE{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T2
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000e40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCGE{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000350U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
    // VCGE{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A2
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000e40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcge, &Assembler::vcge, cond, dt, rd, rn, rm);
}

void Assembler::vcgt(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCGT{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb10000U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCGT{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b10000U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVcgt, &Assembler::vcgt, cond, dt, rd, rm, operand);
}

void Assembler::vcgt(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCGT{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb10040U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCGT{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b10040U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVcgt, &Assembler::vcgt, cond, dt, rd, rm, operand);
}

void Assembler::vcgt(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCGT{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000300U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VCGT{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T2
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200e00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCGT{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000300U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
    // VCGT{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A2
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200e00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcgt, &Assembler::vcgt, cond, dt, rd, rn, rm);
}

void Assembler::vcgt(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCGT{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000340U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VCGT{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T2
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200e40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCGT{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000340U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
    // VCGT{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A2
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200e40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcgt, &Assembler::vcgt, cond, dt, rd, rn, rm);
}

void Assembler::vcle(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCLE{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb10180U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCLE{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b10180U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVcle, &Assembler::vcle, cond, dt, rd, rm, operand);
}

void Assembler::vcle(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCLE{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb101c0U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCLE{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b101c0U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVcle, &Assembler::vcle, cond, dt, rd, rm, operand);
}

void Assembler::vcle(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCLE{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000310U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(5, 0) | rm.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
    // VCLE{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T2
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000e00U | rd.Encode(22, 12) | rn.Encode(5, 0) |
                   rm.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCLE{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000310U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(5, 0) | rm.Encode(7, 16));
        return;
      }
    }
    // VCLE{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A2
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000e00U | rd.Encode(22, 12) | rn.Encode(5, 0) |
                rm.Encode(7, 16));
        return;
      }
    }
  }
  Delegate(kVcle, &Assembler::vcle, cond, dt, rd, rn, rm);
}

void Assembler::vcle(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCLE{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000350U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(5, 0) | rm.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
    // VCLE{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T2
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000e40U | rd.Encode(22, 12) | rn.Encode(5, 0) |
                   rm.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCLE{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000350U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(5, 0) | rm.Encode(7, 16));
        return;
      }
    }
    // VCLE{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A2
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000e40U | rd.Encode(22, 12) | rn.Encode(5, 0) |
                rm.Encode(7, 16));
        return;
      }
    }
  }
  Delegate(kVcle, &Assembler::vcle, cond, dt, rd, rn, rm);
}

void Assembler::vcls(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_5 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCLS{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00400U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCLS{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00400U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcls, &Assembler::vcls, cond, dt, rd, rm);
}

void Assembler::vcls(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_5 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCLS{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00440U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCLS{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00440U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcls, &Assembler::vcls, cond, dt, rd, rm);
}

void Assembler::vclt(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCLT{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb10200U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCLT{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b10200U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVclt, &Assembler::vclt, cond, dt, rd, rm, operand);
}

void Assembler::vclt(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_F_size_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VCLT{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb10240U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VCLT{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b10240U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVclt, &Assembler::vclt, cond, dt, rd, rm, operand);
}

void Assembler::vclt(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCLT{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000300U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(5, 0) | rm.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
    // VCLT{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T2
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200e00U | rd.Encode(22, 12) | rn.Encode(5, 0) |
                   rm.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCLT{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000300U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(5, 0) | rm.Encode(7, 16));
        return;
      }
    }
    // VCLT{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A2
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200e00U | rd.Encode(22, 12) | rn.Encode(5, 0) |
                rm.Encode(7, 16));
        return;
      }
    }
  }
  Delegate(kVclt, &Assembler::vclt, cond, dt, rd, rn, rm);
}

void Assembler::vclt(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCLT{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000340U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(5, 0) | rm.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
    // VCLT{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T2
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200e40U | rd.Encode(22, 12) | rn.Encode(5, 0) |
                   rm.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCLT{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000340U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(5, 0) | rm.Encode(7, 16));
        return;
      }
    }
    // VCLT{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A2
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200e40U | rd.Encode(22, 12) | rn.Encode(5, 0) |
                rm.Encode(7, 16));
        return;
      }
    }
  }
  Delegate(kVclt, &Assembler::vclt, cond, dt, rd, rn, rm);
}

void Assembler::vclz(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_4 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCLZ{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00480U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCLZ{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00480U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVclz, &Assembler::vclz, cond, dt, rd, rm);
}

void Assembler::vclz(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_4 encoded_dt(dt);
  if (IsUsingT32()) {
    // VCLZ{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb004c0U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCLZ{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b004c0U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVclz, &Assembler::vclz, cond, dt, rd, rm);
}

void Assembler::vcmp(Condition cond, DataType dt, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCMP{<c>}{<q>}.F32 <Sd>, <Sm> ; T1
    if (dt.Is(F32)) {
      EmitT32_32(0xeeb40a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCMP{<c>}{<q>}.F32 <Sd>, <Sm> ; A1
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb40a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcmp, &Assembler::vcmp, cond, dt, rd, rm);
}

void Assembler::vcmp(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCMP{<c>}{<q>}.F64 <Dd>, <Dm> ; T1
    if (dt.Is(F64)) {
      EmitT32_32(0xeeb40b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCMP{<c>}{<q>}.F64 <Dd>, <Dm> ; A1
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb40b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcmp, &Assembler::vcmp, cond, dt, rd, rm);
}

void Assembler::vcmp(Condition cond, DataType dt, SRegister rd, double imm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCMP{<c>}{<q>}.F32 <Sd>, #0.0 ; T2
    if (dt.Is(F32) && (imm == 0.0)) {
      EmitT32_32(0xeeb50a40U | rd.Encode(22, 12));
      AdvanceIT();
      return;
    }
  } else {
    // VCMP{<c>}{<q>}.F32 <Sd>, #0.0 ; A2
    if (dt.Is(F32) && (imm == 0.0) && cond.IsNotNever()) {
      EmitA32(0x0eb50a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12));
      return;
    }
  }
  Delegate(kVcmp, &Assembler::vcmp, cond, dt, rd, imm);
}

void Assembler::vcmp(Condition cond, DataType dt, DRegister rd, double imm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCMP{<c>}{<q>}.F64 <Dd>, #0.0 ; T2
    if (dt.Is(F64) && (imm == 0.0)) {
      EmitT32_32(0xeeb50b40U | rd.Encode(22, 12));
      AdvanceIT();
      return;
    }
  } else {
    // VCMP{<c>}{<q>}.F64 <Dd>, #0.0 ; A2
    if (dt.Is(F64) && (imm == 0.0) && cond.IsNotNever()) {
      EmitA32(0x0eb50b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12));
      return;
    }
  }
  Delegate(kVcmp, &Assembler::vcmp, cond, dt, rd, imm);
}

void Assembler::vcmpe(Condition cond, DataType dt, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCMPE{<c>}{<q>}.F32 <Sd>, <Sm> ; T1
    if (dt.Is(F32)) {
      EmitT32_32(0xeeb40ac0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCMPE{<c>}{<q>}.F32 <Sd>, <Sm> ; A1
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb40ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcmpe, &Assembler::vcmpe, cond, dt, rd, rm);
}

void Assembler::vcmpe(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCMPE{<c>}{<q>}.F64 <Dd>, <Dm> ; T1
    if (dt.Is(F64)) {
      EmitT32_32(0xeeb40bc0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCMPE{<c>}{<q>}.F64 <Dd>, <Dm> ; A1
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb40bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcmpe, &Assembler::vcmpe, cond, dt, rd, rm);
}

void Assembler::vcmpe(Condition cond, DataType dt, SRegister rd, double imm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCMPE{<c>}{<q>}.F32 <Sd>, #0.0 ; T2
    if (dt.Is(F32) && (imm == 0.0)) {
      EmitT32_32(0xeeb50ac0U | rd.Encode(22, 12));
      AdvanceIT();
      return;
    }
  } else {
    // VCMPE{<c>}{<q>}.F32 <Sd>, #0.0 ; A2
    if (dt.Is(F32) && (imm == 0.0) && cond.IsNotNever()) {
      EmitA32(0x0eb50ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12));
      return;
    }
  }
  Delegate(kVcmpe, &Assembler::vcmpe, cond, dt, rd, imm);
}

void Assembler::vcmpe(Condition cond, DataType dt, DRegister rd, double imm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCMPE{<c>}{<q>}.F64 <Dd>, #0.0 ; T2
    if (dt.Is(F64) && (imm == 0.0)) {
      EmitT32_32(0xeeb50bc0U | rd.Encode(22, 12));
      AdvanceIT();
      return;
    }
  } else {
    // VCMPE{<c>}{<q>}.F64 <Dd>, #0.0 ; A2
    if (dt.Is(F64) && (imm == 0.0) && cond.IsNotNever()) {
      EmitA32(0x0eb50bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12));
      return;
    }
  }
  Delegate(kVcmpe, &Assembler::vcmpe, cond, dt, rd, imm);
}

void Assembler::vcnt(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCNT{<c>}{<q>}.8 <Dd>, <Dm> ; T1
    if (dt.Is(Untyped8)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00500U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCNT{<c>}{<q>}.8 <Dd>, <Dm> ; A1
    if (dt.Is(Untyped8)) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00500U | rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcnt, &Assembler::vcnt, cond, dt, rd, rm);
}

void Assembler::vcnt(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCNT{<c>}{<q>}.8 <Qd>, <Qm> ; T1
    if (dt.Is(Untyped8)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00540U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCNT{<c>}{<q>}.8 <Qd>, <Qm> ; A1
    if (dt.Is(Untyped8)) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00540U | rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcnt, &Assembler::vcnt, cond, dt, rd, rm);
}

void Assembler::vcvt(
    Condition cond, DataType dt1, DataType dt2, DRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_2 encoded_dt(dt2);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.F64.F32 <Dd>, <Sm> ; T1
    if (dt1.Is(F64) && dt2.Is(F32)) {
      EmitT32_32(0xeeb70ac0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VCVT{<c>}{<q>}.F64.<dt> <Dd>, <Sm> ; T1
    if (dt1.Is(F64) && encoded_dt.IsValid()) {
      EmitT32_32(0xeeb80b40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVT{<c>}{<q>}.F64.F32 <Dd>, <Sm> ; A1
    if (dt1.Is(F64) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb70ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
    // VCVT{<c>}{<q>}.F64.<dt> <Dd>, <Sm> ; A1
    if (dt1.Is(F64) && encoded_dt.IsValid() && cond.IsNotNever()) {
      EmitA32(0x0eb80b40U | (cond.GetCondition() << 28) |
              (encoded_dt.GetEncodingValue() << 7) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvt(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.F32.F64 <Sd>, <Dm> ; T1
    if (dt1.Is(F32) && dt2.Is(F64)) {
      EmitT32_32(0xeeb70bc0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VCVT{<c>}{<q>}.U32.F64 <Sd>, <Dm> ; T1
    if (dt1.Is(U32) && dt2.Is(F64)) {
      EmitT32_32(0xeebc0bc0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VCVT{<c>}{<q>}.S32.F64 <Sd>, <Dm> ; T1
    if (dt1.Is(S32) && dt2.Is(F64)) {
      EmitT32_32(0xeebd0bc0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVT{<c>}{<q>}.F32.F64 <Sd>, <Dm> ; A1
    if (dt1.Is(F32) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb70bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
    // VCVT{<c>}{<q>}.U32.F64 <Sd>, <Dm> ; A1
    if (dt1.Is(U32) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0ebc0bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
    // VCVT{<c>}{<q>}.S32.F64 <Sd>, <Dm> ; A1
    if (dt1.Is(S32) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0ebd0bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvt(Condition cond,
                     DataType dt1,
                     DataType dt2,
                     DRegister rd,
                     DRegister rm,
                     int32_t fbits) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_U_1 encoded_dt(dt1, dt2);
  Dt_U_sx_1 encoded_dt_2(dt2);
  Dt_U_sx_1 encoded_dt_3(dt1);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.<dt>.<dt> <Dd>, <Dm>, #<fbits> ; T1
    if (encoded_dt.IsValid() && (fbits >= 1) && (fbits <= 32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        uint32_t fbits_ = 64 - fbits;
        EmitT32_32(0xef800e10U | ((encoded_dt.GetEncodingValue() & 0x1) << 28) |
                   ((encoded_dt.GetEncodingValue() & 0x2) << 7) |
                   rd.Encode(22, 12) | rm.Encode(5, 0) | (fbits_ << 16));
        AdvanceIT();
        return;
      }
    }
    // VCVT{<c>}{<q>}.F64.<dt> <Ddm>, <Ddm>, #<fbits> ; T1
    if (dt1.Is(F64) && encoded_dt_2.IsValid() && rd.Is(rm) &&
        (((dt2.Is(S16) || dt2.Is(U16)) && (fbits <= 16)) ||
         ((dt2.Is(S32) || dt2.Is(U32)) && (fbits >= 1) && (fbits <= 32)))) {
      unsigned offset = 32;
      if (dt2.Is(S16) || dt2.Is(U16)) {
        offset = 16;
      }
      uint32_t fbits_ = offset - fbits;
      EmitT32_32(0xeeba0b40U | ((encoded_dt_2.GetEncodingValue() & 0x1) << 7) |
                 ((encoded_dt_2.GetEncodingValue() & 0x2) << 15) |
                 rd.Encode(22, 12) | ((fbits_ & 0x1) << 5) |
                 ((fbits_ & 0x1e) >> 1));
      AdvanceIT();
      return;
    }
    // VCVT{<c>}{<q>}.<dt>.F64 <Ddm>, <Ddm>, #<fbits> ; T1
    if (encoded_dt_3.IsValid() && dt2.Is(F64) && rd.Is(rm) &&
        (((dt1.Is(S16) || dt1.Is(U16)) && (fbits <= 16)) ||
         ((dt1.Is(S32) || dt1.Is(U32)) && (fbits >= 1) && (fbits <= 32)))) {
      unsigned offset = 32;
      if (dt1.Is(S16) || dt1.Is(U16)) {
        offset = 16;
      }
      uint32_t fbits_ = offset - fbits;
      EmitT32_32(0xeebe0b40U | ((encoded_dt_3.GetEncodingValue() & 0x1) << 7) |
                 ((encoded_dt_3.GetEncodingValue() & 0x2) << 15) |
                 rd.Encode(22, 12) | ((fbits_ & 0x1) << 5) |
                 ((fbits_ & 0x1e) >> 1));
      AdvanceIT();
      return;
    }
  } else {
    // VCVT{<c>}{<q>}.<dt>.<dt> <Dd>, <Dm>, #<fbits> ; A1
    if (encoded_dt.IsValid() && (fbits >= 1) && (fbits <= 32)) {
      if (cond.Is(al)) {
        uint32_t fbits_ = 64 - fbits;
        EmitA32(0xf2800e10U | ((encoded_dt.GetEncodingValue() & 0x1) << 24) |
                ((encoded_dt.GetEncodingValue() & 0x2) << 7) |
                rd.Encode(22, 12) | rm.Encode(5, 0) | (fbits_ << 16));
        return;
      }
    }
    // VCVT{<c>}{<q>}.F64.<dt> <Ddm>, <Ddm>, #<fbits> ; A1
    if (dt1.Is(F64) && encoded_dt_2.IsValid() && rd.Is(rm) &&
        (((dt2.Is(S16) || dt2.Is(U16)) && (fbits <= 16)) ||
         ((dt2.Is(S32) || dt2.Is(U32)) && (fbits >= 1) && (fbits <= 32))) &&
        cond.IsNotNever()) {
      unsigned offset = 32;
      if (dt2.Is(S16) || dt2.Is(U16)) {
        offset = 16;
      }
      uint32_t fbits_ = offset - fbits;
      EmitA32(0x0eba0b40U | (cond.GetCondition() << 28) |
              ((encoded_dt_2.GetEncodingValue() & 0x1) << 7) |
              ((encoded_dt_2.GetEncodingValue() & 0x2) << 15) |
              rd.Encode(22, 12) | ((fbits_ & 0x1) << 5) |
              ((fbits_ & 0x1e) >> 1));
      return;
    }
    // VCVT{<c>}{<q>}.<dt>.F64 <Ddm>, <Ddm>, #<fbits> ; A1
    if (encoded_dt_3.IsValid() && dt2.Is(F64) && rd.Is(rm) &&
        (((dt1.Is(S16) || dt1.Is(U16)) && (fbits <= 16)) ||
         ((dt1.Is(S32) || dt1.Is(U32)) && (fbits >= 1) && (fbits <= 32))) &&
        cond.IsNotNever()) {
      unsigned offset = 32;
      if (dt1.Is(S16) || dt1.Is(U16)) {
        offset = 16;
      }
      uint32_t fbits_ = offset - fbits;
      EmitA32(0x0ebe0b40U | (cond.GetCondition() << 28) |
              ((encoded_dt_3.GetEncodingValue() & 0x1) << 7) |
              ((encoded_dt_3.GetEncodingValue() & 0x2) << 15) |
              rd.Encode(22, 12) | ((fbits_ & 0x1) << 5) |
              ((fbits_ & 0x1e) >> 1));
      return;
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm, fbits);
}

void Assembler::vcvt(Condition cond,
                     DataType dt1,
                     DataType dt2,
                     QRegister rd,
                     QRegister rm,
                     int32_t fbits) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_U_1 encoded_dt(dt1, dt2);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.<dt>.<dt> <Qd>, <Qm>, #<fbits> ; T1
    if (encoded_dt.IsValid() && (fbits >= 1) && (fbits <= 32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        uint32_t fbits_ = 64 - fbits;
        EmitT32_32(0xef800e50U | ((encoded_dt.GetEncodingValue() & 0x1) << 28) |
                   ((encoded_dt.GetEncodingValue() & 0x2) << 7) |
                   rd.Encode(22, 12) | rm.Encode(5, 0) | (fbits_ << 16));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCVT{<c>}{<q>}.<dt>.<dt> <Qd>, <Qm>, #<fbits> ; A1
    if (encoded_dt.IsValid() && (fbits >= 1) && (fbits <= 32)) {
      if (cond.Is(al)) {
        uint32_t fbits_ = 64 - fbits;
        EmitA32(0xf2800e50U | ((encoded_dt.GetEncodingValue() & 0x1) << 24) |
                ((encoded_dt.GetEncodingValue() & 0x2) << 7) |
                rd.Encode(22, 12) | rm.Encode(5, 0) | (fbits_ << 16));
        return;
      }
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm, fbits);
}

void Assembler::vcvt(Condition cond,
                     DataType dt1,
                     DataType dt2,
                     SRegister rd,
                     SRegister rm,
                     int32_t fbits) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_sx_1 encoded_dt(dt2);
  Dt_U_sx_1 encoded_dt_2(dt1);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.F32.<dt> <Sdm>, <Sdm>, #<fbits> ; T1
    if (dt1.Is(F32) && encoded_dt.IsValid() && rd.Is(rm) &&
        (((dt2.Is(S16) || dt2.Is(U16)) && (fbits <= 16)) ||
         ((dt2.Is(S32) || dt2.Is(U32)) && (fbits >= 1) && (fbits <= 32)))) {
      unsigned offset = 32;
      if (dt2.Is(S16) || dt2.Is(U16)) {
        offset = 16;
      }
      uint32_t fbits_ = offset - fbits;
      EmitT32_32(0xeeba0a40U | ((encoded_dt.GetEncodingValue() & 0x1) << 7) |
                 ((encoded_dt.GetEncodingValue() & 0x2) << 15) |
                 rd.Encode(22, 12) | ((fbits_ & 0x1) << 5) |
                 ((fbits_ & 0x1e) >> 1));
      AdvanceIT();
      return;
    }
    // VCVT{<c>}{<q>}.<dt>.F32 <Sdm>, <Sdm>, #<fbits> ; T1
    if (encoded_dt_2.IsValid() && dt2.Is(F32) && rd.Is(rm) &&
        (((dt1.Is(S16) || dt1.Is(U16)) && (fbits <= 16)) ||
         ((dt1.Is(S32) || dt1.Is(U32)) && (fbits >= 1) && (fbits <= 32)))) {
      unsigned offset = 32;
      if (dt1.Is(S16) || dt1.Is(U16)) {
        offset = 16;
      }
      uint32_t fbits_ = offset - fbits;
      EmitT32_32(0xeebe0a40U | ((encoded_dt_2.GetEncodingValue() & 0x1) << 7) |
                 ((encoded_dt_2.GetEncodingValue() & 0x2) << 15) |
                 rd.Encode(22, 12) | ((fbits_ & 0x1) << 5) |
                 ((fbits_ & 0x1e) >> 1));
      AdvanceIT();
      return;
    }
  } else {
    // VCVT{<c>}{<q>}.F32.<dt> <Sdm>, <Sdm>, #<fbits> ; A1
    if (dt1.Is(F32) && encoded_dt.IsValid() && rd.Is(rm) &&
        (((dt2.Is(S16) || dt2.Is(U16)) && (fbits <= 16)) ||
         ((dt2.Is(S32) || dt2.Is(U32)) && (fbits >= 1) && (fbits <= 32))) &&
        cond.IsNotNever()) {
      unsigned offset = 32;
      if (dt2.Is(S16) || dt2.Is(U16)) {
        offset = 16;
      }
      uint32_t fbits_ = offset - fbits;
      EmitA32(0x0eba0a40U | (cond.GetCondition() << 28) |
              ((encoded_dt.GetEncodingValue() & 0x1) << 7) |
              ((encoded_dt.GetEncodingValue() & 0x2) << 15) |
              rd.Encode(22, 12) | ((fbits_ & 0x1) << 5) |
              ((fbits_ & 0x1e) >> 1));
      return;
    }
    // VCVT{<c>}{<q>}.<dt>.F32 <Sdm>, <Sdm>, #<fbits> ; A1
    if (encoded_dt_2.IsValid() && dt2.Is(F32) && rd.Is(rm) &&
        (((dt1.Is(S16) || dt1.Is(U16)) && (fbits <= 16)) ||
         ((dt1.Is(S32) || dt1.Is(U32)) && (fbits >= 1) && (fbits <= 32))) &&
        cond.IsNotNever()) {
      unsigned offset = 32;
      if (dt1.Is(S16) || dt1.Is(U16)) {
        offset = 16;
      }
      uint32_t fbits_ = offset - fbits;
      EmitA32(0x0ebe0a40U | (cond.GetCondition() << 28) |
              ((encoded_dt_2.GetEncodingValue() & 0x1) << 7) |
              ((encoded_dt_2.GetEncodingValue() & 0x2) << 15) |
              rd.Encode(22, 12) | ((fbits_ & 0x1) << 5) |
              ((fbits_ & 0x1e) >> 1));
      return;
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm, fbits);
}

void Assembler::vcvt(
    Condition cond, DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_1 encoded_dt(dt1, dt2);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.<dt>.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffbb0600U | (encoded_dt.GetEncodingValue() << 7) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCVT{<c>}{<q>}.<dt>.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3bb0600U | (encoded_dt.GetEncodingValue() << 7) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvt(
    Condition cond, DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_1 encoded_dt(dt1, dt2);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.<dt>.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffbb0640U | (encoded_dt.GetEncodingValue() << 7) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCVT{<c>}{<q>}.<dt>.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3bb0640U | (encoded_dt.GetEncodingValue() << 7) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvt(
    Condition cond, DataType dt1, DataType dt2, DRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.F16.F32 <Dd>, <Qm> ; T1
    if (dt1.Is(F16) && dt2.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb60600U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCVT{<c>}{<q>}.F16.F32 <Dd>, <Qm> ; A1
    if (dt1.Is(F16) && dt2.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3b60600U | rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvt(
    Condition cond, DataType dt1, DataType dt2, QRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.F32.F16 <Qd>, <Dm> ; T1
    if (dt1.Is(F32) && dt2.Is(F16)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb60700U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VCVT{<c>}{<q>}.F32.F16 <Qd>, <Dm> ; A1
    if (dt1.Is(F32) && dt2.Is(F16)) {
      if (cond.Is(al)) {
        EmitA32(0xf3b60700U | rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvt(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_2 encoded_dt(dt2);
  if (IsUsingT32()) {
    // VCVT{<c>}{<q>}.U32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(U32) && dt2.Is(F32)) {
      EmitT32_32(0xeebc0ac0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VCVT{<c>}{<q>}.S32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(S32) && dt2.Is(F32)) {
      EmitT32_32(0xeebd0ac0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VCVT{<c>}{<q>}.F32.<dt> <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && encoded_dt.IsValid()) {
      EmitT32_32(0xeeb80a40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVT{<c>}{<q>}.U32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(U32) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0ebc0ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
    // VCVT{<c>}{<q>}.S32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(S32) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0ebd0ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
    // VCVT{<c>}{<q>}.F32.<dt> <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && encoded_dt.IsValid() && cond.IsNotNever()) {
      EmitA32(0x0eb80a40U | (cond.GetCondition() << 28) |
              (encoded_dt.GetEncodingValue() << 7) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvt, &Assembler::vcvt, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvta(DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_3 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTA{<q>}.<dt>.F32 <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xffbb0000U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTA{<q>}.<dt>.F32 <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xf3bb0000U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvta, &Assembler::vcvta, dt1, dt2, rd, rm);
}

void Assembler::vcvta(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_3 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTA{<q>}.<dt>.F32 <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xffbb0040U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTA{<q>}.<dt>.F32 <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xf3bb0040U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvta, &Assembler::vcvta, dt1, dt2, rd, rm);
}

void Assembler::vcvta(DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_2 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTA{<q>}.<dt>.F32 <Sd>, <Sm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xfebc0a40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTA{<q>}.<dt>.F32 <Sd>, <Sm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xfebc0a40U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvta, &Assembler::vcvta, dt1, dt2, rd, rm);
}

void Assembler::vcvta(DataType dt1, DataType dt2, SRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_2 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTA{<q>}.<dt>.F64 <Sd>, <Dm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F64)) {
      EmitT32_32(0xfebc0b40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTA{<q>}.<dt>.F64 <Sd>, <Dm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F64)) {
      EmitA32(0xfebc0b40U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvta, &Assembler::vcvta, dt1, dt2, rd, rm);
}

void Assembler::vcvtb(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVTB{<c>}{<q>}.F32.F16 <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && dt2.Is(F16)) {
      EmitT32_32(0xeeb20a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VCVTB{<c>}{<q>}.F16.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(F16) && dt2.Is(F32)) {
      EmitT32_32(0xeeb30a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTB{<c>}{<q>}.F32.F16 <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && dt2.Is(F16) && cond.IsNotNever()) {
      EmitA32(0x0eb20a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
    // VCVTB{<c>}{<q>}.F16.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(F16) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb30a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtb, &Assembler::vcvtb, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvtb(
    Condition cond, DataType dt1, DataType dt2, DRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVTB{<c>}{<q>}.F64.F16 <Dd>, <Sm> ; T1
    if (dt1.Is(F64) && dt2.Is(F16)) {
      EmitT32_32(0xeeb20b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTB{<c>}{<q>}.F64.F16 <Dd>, <Sm> ; A1
    if (dt1.Is(F64) && dt2.Is(F16) && cond.IsNotNever()) {
      EmitA32(0x0eb20b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtb, &Assembler::vcvtb, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvtb(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVTB{<c>}{<q>}.F16.F64 <Sd>, <Dm> ; T1
    if (dt1.Is(F16) && dt2.Is(F64)) {
      EmitT32_32(0xeeb30b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTB{<c>}{<q>}.F16.F64 <Sd>, <Dm> ; A1
    if (dt1.Is(F16) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb30b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtb, &Assembler::vcvtb, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvtm(DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_3 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTM{<q>}.<dt>.F32 <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xffbb0300U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTM{<q>}.<dt>.F32 <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xf3bb0300U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtm, &Assembler::vcvtm, dt1, dt2, rd, rm);
}

void Assembler::vcvtm(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_3 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTM{<q>}.<dt>.F32 <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xffbb0340U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTM{<q>}.<dt>.F32 <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xf3bb0340U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtm, &Assembler::vcvtm, dt1, dt2, rd, rm);
}

void Assembler::vcvtm(DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_2 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTM{<q>}.<dt>.F32 <Sd>, <Sm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xfebf0a40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTM{<q>}.<dt>.F32 <Sd>, <Sm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xfebf0a40U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtm, &Assembler::vcvtm, dt1, dt2, rd, rm);
}

void Assembler::vcvtm(DataType dt1, DataType dt2, SRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_2 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTM{<q>}.<dt>.F64 <Sd>, <Dm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F64)) {
      EmitT32_32(0xfebf0b40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTM{<q>}.<dt>.F64 <Sd>, <Dm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F64)) {
      EmitA32(0xfebf0b40U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtm, &Assembler::vcvtm, dt1, dt2, rd, rm);
}

void Assembler::vcvtn(DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_3 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTN{<q>}.<dt>.F32 <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xffbb0100U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTN{<q>}.<dt>.F32 <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xf3bb0100U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtn, &Assembler::vcvtn, dt1, dt2, rd, rm);
}

void Assembler::vcvtn(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_3 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTN{<q>}.<dt>.F32 <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xffbb0140U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTN{<q>}.<dt>.F32 <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xf3bb0140U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtn, &Assembler::vcvtn, dt1, dt2, rd, rm);
}

void Assembler::vcvtn(DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_2 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTN{<q>}.<dt>.F32 <Sd>, <Sm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xfebd0a40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTN{<q>}.<dt>.F32 <Sd>, <Sm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xfebd0a40U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtn, &Assembler::vcvtn, dt1, dt2, rd, rm);
}

void Assembler::vcvtn(DataType dt1, DataType dt2, SRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_2 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTN{<q>}.<dt>.F64 <Sd>, <Dm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F64)) {
      EmitT32_32(0xfebd0b40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTN{<q>}.<dt>.F64 <Sd>, <Dm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F64)) {
      EmitA32(0xfebd0b40U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtn, &Assembler::vcvtn, dt1, dt2, rd, rm);
}

void Assembler::vcvtp(DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_3 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTP{<q>}.<dt>.F32 <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xffbb0200U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTP{<q>}.<dt>.F32 <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xf3bb0200U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtp, &Assembler::vcvtp, dt1, dt2, rd, rm);
}

void Assembler::vcvtp(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_3 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTP{<q>}.<dt>.F32 <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xffbb0240U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTP{<q>}.<dt>.F32 <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xf3bb0240U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtp, &Assembler::vcvtp, dt1, dt2, rd, rm);
}

void Assembler::vcvtp(DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_2 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTP{<q>}.<dt>.F32 <Sd>, <Sm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitT32_32(0xfebe0a40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTP{<q>}.<dt>.F32 <Sd>, <Sm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F32)) {
      EmitA32(0xfebe0a40U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtp, &Assembler::vcvtp, dt1, dt2, rd, rm);
}

void Assembler::vcvtp(DataType dt1, DataType dt2, SRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  Dt_op_2 encoded_dt(dt1);
  if (IsUsingT32()) {
    // VCVTP{<q>}.<dt>.F64 <Sd>, <Dm> ; T1
    if (encoded_dt.IsValid() && dt2.Is(F64)) {
      EmitT32_32(0xfebe0b40U | (encoded_dt.GetEncodingValue() << 7) |
                 rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTP{<q>}.<dt>.F64 <Sd>, <Dm> ; A1
    if (encoded_dt.IsValid() && dt2.Is(F64)) {
      EmitA32(0xfebe0b40U | (encoded_dt.GetEncodingValue() << 7) |
              rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtp, &Assembler::vcvtp, dt1, dt2, rd, rm);
}

void Assembler::vcvtr(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVTR{<c>}{<q>}.U32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(U32) && dt2.Is(F32)) {
      EmitT32_32(0xeebc0a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VCVTR{<c>}{<q>}.S32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(S32) && dt2.Is(F32)) {
      EmitT32_32(0xeebd0a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTR{<c>}{<q>}.U32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(U32) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0ebc0a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
    // VCVTR{<c>}{<q>}.S32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(S32) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0ebd0a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtr, &Assembler::vcvtr, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvtr(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVTR{<c>}{<q>}.U32.F64 <Sd>, <Dm> ; T1
    if (dt1.Is(U32) && dt2.Is(F64)) {
      EmitT32_32(0xeebc0b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VCVTR{<c>}{<q>}.S32.F64 <Sd>, <Dm> ; T1
    if (dt1.Is(S32) && dt2.Is(F64)) {
      EmitT32_32(0xeebd0b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTR{<c>}{<q>}.U32.F64 <Sd>, <Dm> ; A1
    if (dt1.Is(U32) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0ebc0b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
    // VCVTR{<c>}{<q>}.S32.F64 <Sd>, <Dm> ; A1
    if (dt1.Is(S32) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0ebd0b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtr, &Assembler::vcvtr, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvtt(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVTT{<c>}{<q>}.F32.F16 <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && dt2.Is(F16)) {
      EmitT32_32(0xeeb20ac0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VCVTT{<c>}{<q>}.F16.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(F16) && dt2.Is(F32)) {
      EmitT32_32(0xeeb30ac0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTT{<c>}{<q>}.F32.F16 <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && dt2.Is(F16) && cond.IsNotNever()) {
      EmitA32(0x0eb20ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
    // VCVTT{<c>}{<q>}.F16.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(F16) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb30ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtt, &Assembler::vcvtt, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvtt(
    Condition cond, DataType dt1, DataType dt2, DRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVTT{<c>}{<q>}.F64.F16 <Dd>, <Sm> ; T1
    if (dt1.Is(F64) && dt2.Is(F16)) {
      EmitT32_32(0xeeb20bc0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTT{<c>}{<q>}.F64.F16 <Dd>, <Sm> ; A1
    if (dt1.Is(F64) && dt2.Is(F16) && cond.IsNotNever()) {
      EmitA32(0x0eb20bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtt, &Assembler::vcvtt, cond, dt1, dt2, rd, rm);
}

void Assembler::vcvtt(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VCVTT{<c>}{<q>}.F16.F64 <Sd>, <Dm> ; T1
    if (dt1.Is(F16) && dt2.Is(F64)) {
      EmitT32_32(0xeeb30bc0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VCVTT{<c>}{<q>}.F16.F64 <Sd>, <Dm> ; A1
    if (dt1.Is(F16) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb30bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVcvtt, &Assembler::vcvtt, cond, dt1, dt2, rd, rm);
}

void Assembler::vdiv(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VDIV{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; T1
    if (dt.Is(F32)) {
      EmitT32_32(0xee800a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VDIV{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; A1
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e800a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVdiv, &Assembler::vdiv, cond, dt, rd, rn, rm);
}

void Assembler::vdiv(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VDIV{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F64)) {
      EmitT32_32(0xee800b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VDIV{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e800b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVdiv, &Assembler::vdiv, cond, dt, rd, rn, rm);
}

void Assembler::vdup(Condition cond, DataType dt, QRegister rd, Register rt) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_B_E_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VDUP{<c>}{<q>}.<dt> <Qd>, <Rt> ; T1
    if (encoded_dt.IsValid() && (!rt.IsPC() || AllowUnpredictable())) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xeea00b10U | ((encoded_dt.GetEncodingValue() & 0x1) << 5) |
                   ((encoded_dt.GetEncodingValue() & 0x2) << 21) |
                   rd.Encode(7, 16) | (rt.GetCode() << 12));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VDUP{<c>}{<q>}.<dt> <Qd>, <Rt> ; A1
    if (encoded_dt.IsValid() && cond.IsNotNever() &&
        (!rt.IsPC() || AllowUnpredictable())) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitA32(0x0ea00b10U | (cond.GetCondition() << 28) |
                ((encoded_dt.GetEncodingValue() & 0x1) << 5) |
                ((encoded_dt.GetEncodingValue() & 0x2) << 21) |
                rd.Encode(7, 16) | (rt.GetCode() << 12));
        return;
      }
    }
  }
  Delegate(kVdup, &Assembler::vdup, cond, dt, rd, rt);
}

void Assembler::vdup(Condition cond, DataType dt, DRegister rd, Register rt) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_B_E_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VDUP{<c>}{<q>}.<dt> <Dd>, <Rt> ; T1
    if (encoded_dt.IsValid() && (!rt.IsPC() || AllowUnpredictable())) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xee800b10U | ((encoded_dt.GetEncodingValue() & 0x1) << 5) |
                   ((encoded_dt.GetEncodingValue() & 0x2) << 21) |
                   rd.Encode(7, 16) | (rt.GetCode() << 12));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VDUP{<c>}{<q>}.<dt> <Dd>, <Rt> ; A1
    if (encoded_dt.IsValid() && cond.IsNotNever() &&
        (!rt.IsPC() || AllowUnpredictable())) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitA32(0x0e800b10U | (cond.GetCondition() << 28) |
                ((encoded_dt.GetEncodingValue() & 0x1) << 5) |
                ((encoded_dt.GetEncodingValue() & 0x2) << 21) |
                rd.Encode(7, 16) | (rt.GetCode() << 12));
        return;
      }
    }
  }
  Delegate(kVdup, &Assembler::vdup, cond, dt, rd, rt);
}

void Assembler::vdup(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_imm4_1 encoded_dt(dt, rm);
  if (IsUsingT32()) {
    // VDUP{<c>}{<q>}.<dt> <Dd>, <Dm[x]> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00c00U | (encoded_dt.GetEncodingValue() << 16) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VDUP{<c>}{<q>}.<dt> <Dd>, <Dm[x]> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00c00U | (encoded_dt.GetEncodingValue() << 16) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVdup, &Assembler::vdup, cond, dt, rd, rm);
}

void Assembler::vdup(Condition cond,
                     DataType dt,
                     QRegister rd,
                     DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_imm4_1 encoded_dt(dt, rm);
  if (IsUsingT32()) {
    // VDUP{<c>}{<q>}.<dt> <Qd>, <Dm[x]> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00c40U | (encoded_dt.GetEncodingValue() << 16) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VDUP{<c>}{<q>}.<dt> <Qd>, <Dm[x]> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00c40U | (encoded_dt.GetEncodingValue() << 16) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVdup, &Assembler::vdup, cond, dt, rd, rm);
}

void Assembler::veor(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VEOR{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xff000110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VEOR{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3000110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVeor, &Assembler::veor, cond, dt, rd, rn, rm);
}

void Assembler::veor(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VEOR{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xff000150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VEOR{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3000150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVeor, &Assembler::veor, cond, dt, rd, rn, rm);
}

void Assembler::vext(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rn,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      if (IsUsingT32()) {
        // VEXT{<c>}{<q>}.8 {<Dd>}, <Dn>, <Dm>, #<imm> ; T1
        if (dt.Is(Untyped8) && (imm <= 7)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xefb00000U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                       rm.Encode(5, 0) | (imm << 8));
            AdvanceIT();
            return;
          }
        }
        // VEXT{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm>, #<imm> ; T1
        if ((dt.Is(Untyped16) || dt.Is(Untyped32)) &&
            (imm <= (128 / dt.GetSize()) - 1) && ((imm % dt.GetSize()) == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm4 = imm / dt.GetSize();
            EmitT32_32(0xefb00000U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                       rm.Encode(5, 0) | (imm4 << 8));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VEXT{<c>}{<q>}.8 {<Dd>}, <Dn>, <Dm>, #<imm> ; A1
        if (dt.Is(Untyped8) && (imm <= 7)) {
          if (cond.Is(al)) {
            EmitA32(0xf2b00000U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                    rm.Encode(5, 0) | (imm << 8));
            return;
          }
        }
        // VEXT{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm>, #<imm> ; A1
        if ((dt.Is(Untyped16) || dt.Is(Untyped32)) &&
            (imm <= (128 / dt.GetSize()) - 1) && ((imm % dt.GetSize()) == 0)) {
          if (cond.Is(al)) {
            uint32_t imm4 = imm / dt.GetSize();
            EmitA32(0xf2b00000U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                    rm.Encode(5, 0) | (imm4 << 8));
            return;
          }
        }
      }
    }
  }
  Delegate(kVext, &Assembler::vext, cond, dt, rd, rn, rm, operand);
}

void Assembler::vext(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rn,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      if (IsUsingT32()) {
        // VEXT{<c>}{<q>}.8 {<Qd>}, <Qn>, <Qm>, #<imm> ; T1
        if (dt.Is(Untyped8) && (imm <= 15)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xefb00040U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                       rm.Encode(5, 0) | (imm << 8));
            AdvanceIT();
            return;
          }
        }
        // VEXT{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm>, #<imm> ; T1
        if ((dt.Is(Untyped16) || dt.Is(Untyped32) || dt.Is(Untyped64)) &&
            (imm <= (64 / dt.GetSize()) - 1) && ((imm % dt.GetSize()) == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm4 = imm / dt.GetSize();
            EmitT32_32(0xefb00040U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                       rm.Encode(5, 0) | (imm4 << 8));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VEXT{<c>}{<q>}.8 {<Qd>}, <Qn>, <Qm>, #<imm> ; A1
        if (dt.Is(Untyped8) && (imm <= 15)) {
          if (cond.Is(al)) {
            EmitA32(0xf2b00040U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                    rm.Encode(5, 0) | (imm << 8));
            return;
          }
        }
        // VEXT{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm>, #<imm> ; A1
        if ((dt.Is(Untyped16) || dt.Is(Untyped32) || dt.Is(Untyped64)) &&
            (imm <= (64 / dt.GetSize()) - 1) && ((imm % dt.GetSize()) == 0)) {
          if (cond.Is(al)) {
            uint32_t imm4 = imm / dt.GetSize();
            EmitA32(0xf2b00040U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                    rm.Encode(5, 0) | (imm4 << 8));
            return;
          }
        }
      }
    }
  }
  Delegate(kVext, &Assembler::vext, cond, dt, rd, rn, rm, operand);
}

void Assembler::vfma(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFMA{<c>}{<q>}.F32 <Dd>, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000c10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VFMA{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; T2
    if (dt.Is(F64)) {
      EmitT32_32(0xeea00b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VFMA{<c>}{<q>}.F32 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000c10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VFMA{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; A2
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0ea00b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVfma, &Assembler::vfma, cond, dt, rd, rn, rm);
}

void Assembler::vfma(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFMA{<c>}{<q>}.F32 <Qd>, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000c50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VFMA{<c>}{<q>}.F32 <Qd>, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000c50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVfma, &Assembler::vfma, cond, dt, rd, rn, rm);
}

void Assembler::vfma(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFMA{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; T2
    if (dt.Is(F32)) {
      EmitT32_32(0xeea00a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VFMA{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; A2
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0ea00a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVfma, &Assembler::vfma, cond, dt, rd, rn, rm);
}

void Assembler::vfms(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFMS{<c>}{<q>}.F32 <Dd>, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200c10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VFMS{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; T2
    if (dt.Is(F64)) {
      EmitT32_32(0xeea00b40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VFMS{<c>}{<q>}.F32 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200c10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VFMS{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; A2
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0ea00b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVfms, &Assembler::vfms, cond, dt, rd, rn, rm);
}

void Assembler::vfms(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFMS{<c>}{<q>}.F32 <Qd>, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200c50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VFMS{<c>}{<q>}.F32 <Qd>, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200c50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVfms, &Assembler::vfms, cond, dt, rd, rn, rm);
}

void Assembler::vfms(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFMS{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; T2
    if (dt.Is(F32)) {
      EmitT32_32(0xeea00a40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VFMS{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; A2
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0ea00a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVfms, &Assembler::vfms, cond, dt, rd, rn, rm);
}

void Assembler::vfnma(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFNMA{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; T1
    if (dt.Is(F32)) {
      EmitT32_32(0xee900a40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VFNMA{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; A1
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e900a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVfnma, &Assembler::vfnma, cond, dt, rd, rn, rm);
}

void Assembler::vfnma(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFNMA{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; T1
    if (dt.Is(F64)) {
      EmitT32_32(0xee900b40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VFNMA{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e900b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVfnma, &Assembler::vfnma, cond, dt, rd, rn, rm);
}

void Assembler::vfnms(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFNMS{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; T1
    if (dt.Is(F32)) {
      EmitT32_32(0xee900a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VFNMS{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; A1
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e900a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVfnms, &Assembler::vfnms, cond, dt, rd, rn, rm);
}

void Assembler::vfnms(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VFNMS{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; T1
    if (dt.Is(F64)) {
      EmitT32_32(0xee900b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VFNMS{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e900b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVfnms, &Assembler::vfnms, cond, dt, rd, rn, rm);
}

void Assembler::vhadd(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VHADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000000U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VHADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000000U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVhadd, &Assembler::vhadd, cond, dt, rd, rn, rm);
}

void Assembler::vhadd(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VHADD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000040U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VHADD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000040U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVhadd, &Assembler::vhadd, cond, dt, rd, rn, rm);
}

void Assembler::vhsub(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VHSUB{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000200U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VHSUB{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000200U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVhsub, &Assembler::vhsub, cond, dt, rd, rn, rm);
}

void Assembler::vhsub(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VHSUB{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000240U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VHSUB{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000240U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVhsub, &Assembler::vhsub, cond, dt, rd, rn, rm);
}

void Assembler::vld1(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const AlignedMemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Dt_size_6 encoded_dt(dt);
    Dt_size_7 encoded_dt_2(dt);
    Align_align_1 encoded_align_1(align, nreglist);
    Align_a_1 encoded_align_2(align, dt);
    Align_index_align_1 encoded_align_3(align, nreglist, dt);
    if (IsUsingT32()) {
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitT32_32(0xf920000fU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitT32_32(0xf920000dU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 2) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.GetLength() - 1;
          EmitT32_32(0xf9a00c0fU | (encoded_dt_2.GetEncodingValue() << 6) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 2) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.GetLength() - 1;
          EmitT32_32(0xf9a00c0dU | (encoded_dt_2.GetEncodingValue() << 6) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && operand.IsOffset() &&
          encoded_align_3.IsValid() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a0000fU | (encoded_dt_2.GetEncodingValue() << 10) |
                     (encoded_align_3.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && operand.IsPostIndex() &&
          encoded_align_3.IsValid() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a0000dU | (encoded_dt_2.GetEncodingValue() << 10) |
                     (encoded_align_3.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitA32(0xf420000fU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitA32(0xf420000dU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 2) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.GetLength() - 1;
          EmitA32(0xf4a00c0fU | (encoded_dt_2.GetEncodingValue() << 6) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 2) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.GetLength() - 1;
          EmitA32(0xf4a00c0dU | (encoded_dt_2.GetEncodingValue() << 6) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && operand.IsOffset() &&
          encoded_align_3.IsValid() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a0000fU | (encoded_dt_2.GetEncodingValue() << 10) |
                  (encoded_align_3.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && operand.IsPostIndex() &&
          encoded_align_3.IsValid() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a0000dU | (encoded_dt_2.GetEncodingValue() << 10) |
                  (encoded_align_3.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Register rm = operand.GetOffsetRegister();
    Dt_size_6 encoded_dt(dt);
    Dt_size_7 encoded_dt_2(dt);
    Align_align_1 encoded_align_1(align, nreglist);
    Align_a_1 encoded_align_2(align, dt);
    Align_index_align_1 encoded_align_3(align, nreglist, dt);
    if (IsUsingT32()) {
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitT32_32(0xf9200000U | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 2) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.GetLength() - 1;
          EmitT32_32(0xf9a00c00U | (encoded_dt_2.GetEncodingValue() << 6) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && !rm.IsPC() && !rm.IsSP() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a00000U | (encoded_dt_2.GetEncodingValue() << 10) |
                     (encoded_align_3.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16) |
                     rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitA32(0xf4200000U | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 2) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.GetLength() - 1;
          EmitA32(0xf4a00c00U | (encoded_dt_2.GetEncodingValue() << 6) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VLD1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && !rm.IsPC() && !rm.IsSP() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a00000U | (encoded_dt_2.GetEncodingValue() << 10) |
                  (encoded_align_3.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVld1, &Assembler::vld1, cond, dt, nreglist, operand);
}

void Assembler::vld2(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const AlignedMemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Dt_size_7 encoded_dt(dt);
    Align_align_2 encoded_align_1(align, nreglist);
    Align_a_2 encoded_align_2(align, dt);
    Align_index_align_2 encoded_align_3(align, nreglist, dt);
    if (IsUsingT32()) {
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitT32_32(0xf920000fU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitT32_32(0xf920000dU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9a00d0fU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9a00d0dU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsOffset() && encoded_align_3.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a0010fU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_3.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsPostIndex() && encoded_align_3.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a0010dU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_3.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitA32(0xf420000fU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitA32(0xf420000dU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4a00d0fU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4a00d0dU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsOffset() && encoded_align_3.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a0010fU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_3.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsPostIndex() && encoded_align_3.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a0010dU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_3.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Register rm = operand.GetOffsetRegister();
    Dt_size_7 encoded_dt(dt);
    Align_align_2 encoded_align_1(align, nreglist);
    Align_a_2 encoded_align_2(align, dt);
    Align_index_align_2 encoded_align_3(align, nreglist, dt);
    if (IsUsingT32()) {
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitT32_32(0xf9200000U | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9a00d00U | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a00100U | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_3.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16) |
                     rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitA32(0xf4200000U | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4a00d00U | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VLD2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a00100U | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_3.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVld2, &Assembler::vld2, cond, dt, nreglist, operand);
}

void Assembler::vld3(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const AlignedMemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Dt_size_7 encoded_dt(dt);
    Align_align_3 encoded_align_1(align);
    if (IsUsingT32()) {
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitT32_32(0xf920000fU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitT32_32(0xf920000dU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitA32(0xf420000fU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitA32(0xf420000dU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Register rm = operand.GetOffsetRegister();
    Dt_size_7 encoded_dt(dt);
    Align_align_3 encoded_align_1(align);
    if (IsUsingT32()) {
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitT32_32(0xf9200000U | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitA32(0xf4200000U | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVld3, &Assembler::vld3, cond, dt, nreglist, operand);
}

void Assembler::vld3(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Dt_size_7 encoded_dt(dt);
    Index_1 encoded_align_1(nreglist, dt);
    if (IsUsingT32()) {
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9a00e0fU | (encoded_dt.GetEncodingValue() << 6) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPreIndex() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9a00e0dU | (encoded_dt.GetEncodingValue() << 6) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a0020fU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPreIndex() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a0020dU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4a00e0fU | (encoded_dt.GetEncodingValue() << 6) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPreIndex() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4a00e0dU | (encoded_dt.GetEncodingValue() << 6) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a0020fU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPreIndex() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a0020dU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Dt_size_7 encoded_dt(dt);
    Index_1 encoded_align_1(nreglist, dt);
    if (IsUsingT32()) {
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>], #<Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          sign.IsPlus() && operand.IsPostIndex() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9a00e00U | (encoded_dt.GetEncodingValue() << 6) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>], #<Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          sign.IsPlus() && operand.IsPostIndex() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a00200U | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16) |
                     rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>], #<Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          sign.IsPlus() && operand.IsPostIndex() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4a00e00U | (encoded_dt.GetEncodingValue() << 6) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VLD3{<c>}{<q>}.<dt> <list>, [<Rn>], #<Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          sign.IsPlus() && operand.IsPostIndex() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a00200U | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVld3, &Assembler::vld3, cond, dt, nreglist, operand);
}

void Assembler::vld4(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const AlignedMemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Dt_size_7 encoded_dt(dt);
    Dt_size_8 encoded_dt_2(dt, align);
    Align_align_4 encoded_align_1(align);
    Align_a_3 encoded_align_2(align, dt);
    Align_index_align_3 encoded_align_3(align, nreglist, dt);
    if (IsUsingT32()) {
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf920000fU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf920000dU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9a00f0fU | (encoded_dt_2.GetEncodingValue() << 6) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9a00f0dU | (encoded_dt_2.GetEncodingValue() << 6) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_3.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a0030fU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_3.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_3.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a0030dU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_3.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf420000fU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf420000dU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4a00f0fU | (encoded_dt_2.GetEncodingValue() << 6) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4a00f0dU | (encoded_dt_2.GetEncodingValue() << 6) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_3.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a0030fU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_3.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_3.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a0030dU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_3.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Register rm = operand.GetOffsetRegister();
    Dt_size_7 encoded_dt(dt);
    Dt_size_8 encoded_dt_2(dt, align);
    Align_align_4 encoded_align_1(align);
    Align_a_3 encoded_align_2(align, dt);
    Align_index_align_3 encoded_align_3(align, nreglist, dt);
    if (IsUsingT32()) {
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9200000U | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9a00f00U | (encoded_dt_2.GetEncodingValue() << 6) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 5) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9a00300U | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_3.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16) |
                     rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4200000U | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferAllLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4a00f00U | (encoded_dt_2.GetEncodingValue() << 6) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 5) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VLD4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4a00300U | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_3.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVld4, &Assembler::vld4, cond, dt, nreglist, operand);
}

void Assembler::vldm(Condition cond,
                     DataType dt,
                     Register rn,
                     WriteBack write_back,
                     DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VLDM{<c>}{<q>}{.<size>} <Rn>{!}, <dreglist> ; T1
    if (((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xec900b00U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VLDM{<c>}{<q>}{.<size>} <Rn>{!}, <dreglist> ; A1
    if (cond.IsNotNever() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0c900b00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVldm, &Assembler::vldm, cond, dt, rn, write_back, dreglist);
}

void Assembler::vldm(Condition cond,
                     DataType dt,
                     Register rn,
                     WriteBack write_back,
                     SRegisterList sreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VLDM{<c>}{<q>}{.<size>} <Rn>{!}, <sreglist> ; T2
    const SRegister& sreg = sreglist.GetFirstSRegister();
    unsigned len = sreglist.GetLength();
    EmitT32_32(0xec900a00U | (rn.GetCode() << 16) |
               (write_back.GetWriteBackUint32() << 21) | sreg.Encode(22, 12) |
               (len & 0xff));
    AdvanceIT();
    return;
  } else {
    // VLDM{<c>}{<q>}{.<size>} <Rn>{!}, <sreglist> ; A2
    if (cond.IsNotNever()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitA32(0x0c900a00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | sreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVldm, &Assembler::vldm, cond, dt, rn, write_back, sreglist);
}

void Assembler::vldmdb(Condition cond,
                       DataType dt,
                       Register rn,
                       WriteBack write_back,
                       DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VLDMDB{<c>}{<q>}{.<size>} <Rn>!, <dreglist> ; T1
    if (write_back.DoesWriteBack() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xed300b00U | (rn.GetCode() << 16) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VLDMDB{<c>}{<q>}{.<size>} <Rn>!, <dreglist> ; A1
    if (write_back.DoesWriteBack() && cond.IsNotNever() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0d300b00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              dreg.Encode(22, 12) | (len & 0xff));
      return;
    }
  }
  Delegate(kVldmdb, &Assembler::vldmdb, cond, dt, rn, write_back, dreglist);
}

void Assembler::vldmdb(Condition cond,
                       DataType dt,
                       Register rn,
                       WriteBack write_back,
                       SRegisterList sreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VLDMDB{<c>}{<q>}{.<size>} <Rn>!, <sreglist> ; T2
    if (write_back.DoesWriteBack()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitT32_32(0xed300a00U | (rn.GetCode() << 16) | sreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VLDMDB{<c>}{<q>}{.<size>} <Rn>!, <sreglist> ; A2
    if (write_back.DoesWriteBack() && cond.IsNotNever()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitA32(0x0d300a00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              sreg.Encode(22, 12) | (len & 0xff));
      return;
    }
  }
  Delegate(kVldmdb, &Assembler::vldmdb, cond, dt, rn, write_back, sreglist);
}

void Assembler::vldmia(Condition cond,
                       DataType dt,
                       Register rn,
                       WriteBack write_back,
                       DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VLDMIA{<c>}{<q>}{.<size>} <Rn>{!}, <dreglist> ; T1
    if (((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xec900b00U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VLDMIA{<c>}{<q>}{.<size>} <Rn>{!}, <dreglist> ; A1
    if (cond.IsNotNever() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0c900b00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVldmia, &Assembler::vldmia, cond, dt, rn, write_back, dreglist);
}

void Assembler::vldmia(Condition cond,
                       DataType dt,
                       Register rn,
                       WriteBack write_back,
                       SRegisterList sreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VLDMIA{<c>}{<q>}{.<size>} <Rn>{!}, <sreglist> ; T2
    const SRegister& sreg = sreglist.GetFirstSRegister();
    unsigned len = sreglist.GetLength();
    EmitT32_32(0xec900a00U | (rn.GetCode() << 16) |
               (write_back.GetWriteBackUint32() << 21) | sreg.Encode(22, 12) |
               (len & 0xff));
    AdvanceIT();
    return;
  } else {
    // VLDMIA{<c>}{<q>}{.<size>} <Rn>{!}, <sreglist> ; A2
    if (cond.IsNotNever()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitA32(0x0c900a00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | sreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVldmia, &Assembler::vldmia, cond, dt, rn, write_back, sreglist);
}

void Assembler::vldr(Condition cond, DataType dt, DRegister rd, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // VLDR{<c>}{<q>}{.64} <Dd>, <label> ; T1
    if (dt.IsNoneOr(Untyped64) &&
        ((label->IsBound() && (offset >= -1020) && (offset <= 1020) &&
          ((offset & 0x3) == 0)) ||
         !label->IsBound())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-1020, 1020) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -1020) && (offset <= 1020) &&
                      ((offset & 0x3) == 0));
          int32_t target = offset >> 2;
          uint32_t U = (target >= 0);
          target = abs(target) | (U << 8);
          return instr | (target & 0xff) | ((target & 0x100) << 15);
        }
      } immop;
      EmitT32_32(Link(0xed1f0b00U | rd.Encode(22, 12), label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // VLDR{<c>}{<q>}{.64} <Dd>, <label> ; A1
    if (dt.IsNoneOr(Untyped64) &&
        ((label->IsBound() && (offset >= -1020) && (offset <= 1020) &&
          ((offset & 0x3) == 0)) ||
         !label->IsBound()) &&
        cond.IsNotNever()) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-1020, 1020) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -1020) && (offset <= 1020) &&
                      ((offset & 0x3) == 0));
          int32_t target = offset >> 2;
          uint32_t U = (target >= 0);
          target = abs(target) | (U << 8);
          return instr | (target & 0xff) | ((target & 0x100) << 15);
        }
      } immop;
      EmitA32(
          Link(0x0d1f0b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12),
               label,
               immop));
      return;
    }
  }
  Delegate(kVldr, &Assembler::vldr, cond, dt, rd, label);
}

void Assembler::vldr(Condition cond,
                     DataType dt,
                     DRegister rd,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // VLDR{<c>}{<q>}{.64} <Dd>, [PC, #<_plusminus_><imm>] ; T1
      if (dt.IsNoneOr(Untyped64) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && rn.Is(pc) && operand.IsOffset()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xed1f0b00U | rd.Encode(22, 12) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
      // VLDR{<c>}{<q>}{.64} <Dd>, [<Rn>{, #{+/-}<imm>}] ; T1
      if (dt.IsNoneOr(Untyped64) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xed100b00U | rd.Encode(22, 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // VLDR{<c>}{<q>}{.64} <Dd>, [PC, #<_plusminus_><imm>] ; A1
      if (dt.IsNoneOr(Untyped64) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && rn.Is(pc) && operand.IsOffset() &&
          cond.IsNotNever()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitA32(0x0d1f0b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                offset_ | (sign << 23));
        return;
      }
      // VLDR{<c>}{<q>}{.64} <Dd>, [<Rn>{, #{+/-}<imm>}] ; A1
      if (dt.IsNoneOr(Untyped64) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && operand.IsOffset() && cond.IsNotNever() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitA32(0x0d100b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                (rn.GetCode() << 16) | offset_ | (sign << 23));
        return;
      }
    }
  }
  Delegate(kVldr, &Assembler::vldr, cond, dt, rd, operand);
}

void Assembler::vldr(Condition cond, DataType dt, SRegister rd, Label* label) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Label::Offset offset =
      label->IsBound()
          ? label->GetLocation() -
                AlignDown(GetCursorOffset() + GetArchitectureStatePCOffset(), 4)
          : 0;
  if (IsUsingT32()) {
    // VLDR{<c>}{<q>}{.32} <Sd>, <label> ; T2
    if (dt.IsNoneOr(Untyped32) &&
        ((label->IsBound() && (offset >= -1020) && (offset <= 1020) &&
          ((offset & 0x3) == 0)) ||
         !label->IsBound())) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-1020, 1020) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -1020) && (offset <= 1020) &&
                      ((offset & 0x3) == 0));
          int32_t target = offset >> 2;
          uint32_t U = (target >= 0);
          target = abs(target) | (U << 8);
          return instr | (target & 0xff) | ((target & 0x100) << 15);
        }
      } immop;
      EmitT32_32(Link(0xed1f0a00U | rd.Encode(22, 12), label, immop));
      AdvanceIT();
      return;
    }
  } else {
    // VLDR{<c>}{<q>}{.32} <Sd>, <label> ; A2
    if (dt.IsNoneOr(Untyped32) &&
        ((label->IsBound() && (offset >= -1020) && (offset <= 1020) &&
          ((offset & 0x3) == 0)) ||
         !label->IsBound()) &&
        cond.IsNotNever()) {
      static class EmitOp : public Label::LabelEmitOperator {
       public:
        EmitOp() : Label::LabelEmitOperator(-1020, 1020) {}
        virtual uint32_t Encode(uint32_t instr,
                                Label::Offset pc,
                                const Label* label) const VIXL_OVERRIDE {
          Label::Offset offset = label->GetLocation() - AlignDown(pc, 4);
          VIXL_ASSERT((offset >= -1020) && (offset <= 1020) &&
                      ((offset & 0x3) == 0));
          int32_t target = offset >> 2;
          uint32_t U = (target >= 0);
          target = abs(target) | (U << 8);
          return instr | (target & 0xff) | ((target & 0x100) << 15);
        }
      } immop;
      EmitA32(
          Link(0x0d1f0a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12),
               label,
               immop));
      return;
    }
  }
  Delegate(kVldr, &Assembler::vldr, cond, dt, rd, label);
}

void Assembler::vldr(Condition cond,
                     DataType dt,
                     SRegister rd,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // VLDR{<c>}{<q>}{.32} <Sd>, [PC, #<_plusminus_><imm>] ; T2
      if (dt.IsNoneOr(Untyped32) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && rn.Is(pc) && operand.IsOffset()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xed1f0a00U | rd.Encode(22, 12) | offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
      // VLDR{<c>}{<q>}{.32} <Sd>, [<Rn>{, #{+/-}<imm>}] ; T2
      if (dt.IsNoneOr(Untyped32) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && operand.IsOffset() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xed100a00U | rd.Encode(22, 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // VLDR{<c>}{<q>}{.32} <Sd>, [PC, #<_plusminus_><imm>] ; A2
      if (dt.IsNoneOr(Untyped32) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && rn.Is(pc) && operand.IsOffset() &&
          cond.IsNotNever()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitA32(0x0d1f0a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                offset_ | (sign << 23));
        return;
      }
      // VLDR{<c>}{<q>}{.32} <Sd>, [<Rn>{, #{+/-}<imm>}] ; A2
      if (dt.IsNoneOr(Untyped32) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && operand.IsOffset() && cond.IsNotNever() &&
          ((rn.GetCode() & 0xf) != 0xf)) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitA32(0x0d100a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                (rn.GetCode() << 16) | offset_ | (sign << 23));
        return;
      }
    }
  }
  Delegate(kVldr, &Assembler::vldr, cond, dt, rd, operand);
}

void Assembler::vmax(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMAX{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000f00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMAX{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000600U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMAX{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000f00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMAX{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000600U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmax, &Assembler::vmax, cond, dt, rd, rn, rm);
}

void Assembler::vmax(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMAX{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000f40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMAX{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000640U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMAX{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000f40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMAX{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000640U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmax, &Assembler::vmax, cond, dt, rd, rn, rm);
}

void Assembler::vmaxnm(DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VMAXNM{<q>}.F32 <Dd>, <Dn>, <Dm> ; T1
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xff000f10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VMAXNM{<q>}.F64 <Dd>, <Dn>, <Dm> ; T2
    if (OutsideITBlock() && dt.Is(F64)) {
      EmitT32_32(0xfe800b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMAXNM{<q>}.F32 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      EmitA32(0xf3000f10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
    // VMAXNM{<q>}.F64 <Dd>, <Dn>, <Dm> ; A2
    if (dt.Is(F64)) {
      EmitA32(0xfe800b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVmaxnm, &Assembler::vmaxnm, dt, rd, rn, rm);
}

void Assembler::vmaxnm(DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VMAXNM{<q>}.F32 <Qd>, <Qn>, <Qm> ; T1
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xff000f50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMAXNM{<q>}.F32 <Qd>, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      EmitA32(0xf3000f50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVmaxnm, &Assembler::vmaxnm, dt, rd, rn, rm);
}

void Assembler::vmaxnm(DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VMAXNM{<q>}.F32 <Sd>, <Sn>, <Sm> ; T2
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xfe800a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMAXNM{<q>}.F32 <Sd>, <Sn>, <Sm> ; A2
    if (dt.Is(F32)) {
      EmitA32(0xfe800a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVmaxnm, &Assembler::vmaxnm, dt, rd, rn, rm);
}

void Assembler::vmin(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMIN{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200f00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMIN{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000610U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMIN{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200f00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMIN{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000610U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmin, &Assembler::vmin, cond, dt, rd, rn, rm);
}

void Assembler::vmin(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMIN{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200f40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMIN{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000650U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMIN{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200f40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMIN{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000650U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmin, &Assembler::vmin, cond, dt, rd, rn, rm);
}

void Assembler::vminnm(DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VMINNM{<q>}.F32 <Dd>, <Dn>, <Dm> ; T1
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xff200f10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VMINNM{<q>}.F64 <Dd>, <Dn>, <Dm> ; T2
    if (OutsideITBlock() && dt.Is(F64)) {
      EmitT32_32(0xfe800b40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMINNM{<q>}.F32 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      EmitA32(0xf3200f10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
    // VMINNM{<q>}.F64 <Dd>, <Dn>, <Dm> ; A2
    if (dt.Is(F64)) {
      EmitA32(0xfe800b40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVminnm, &Assembler::vminnm, dt, rd, rn, rm);
}

void Assembler::vminnm(DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VMINNM{<q>}.F32 <Qd>, <Qn>, <Qm> ; T1
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xff200f50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMINNM{<q>}.F32 <Qd>, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      EmitA32(0xf3200f50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVminnm, &Assembler::vminnm, dt, rd, rn, rm);
}

void Assembler::vminnm(DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VMINNM{<q>}.F32 <Sd>, <Sn>, <Sm> ; T2
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xfe800a40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMINNM{<q>}.F32 <Sd>, <Sn>, <Sm> ; A2
    if (dt.Is(F32)) {
      EmitA32(0xfe800a40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVminnm, &Assembler::vminnm, dt, rd, rn, rm);
}

void Assembler::vmla(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_9 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLA{<c>}{<q>}.<type><size> <Dd>, <Dn>, <Dm[x]> ; T1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800040U | (encoded_dt.GetTypeEncodingValue() << 8) |
                   (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                   rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLA{<c>}{<q>}.<type><size> <Dd>, <Dn>, <Dm[x]> ; A1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800040U | (encoded_dt.GetTypeEncodingValue() << 8) |
                (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVmla, &Assembler::vmla, cond, dt, rd, rn, rm);
}

void Assembler::vmla(
    Condition cond, DataType dt, QRegister rd, QRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_9 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLA{<c>}{<q>}.<type><size> <Qd>, <Qn>, <Dm[x]> ; T1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff800040U | (encoded_dt.GetTypeEncodingValue() << 8) |
                   (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                   rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLA{<c>}{<q>}.<type><size> <Qd>, <Qn>, <Dm[x]> ; A1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al)) {
        EmitA32(0xf3800040U | (encoded_dt.GetTypeEncodingValue() << 8) |
                (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVmla, &Assembler::vmla, cond, dt, rd, rn, rm);
}

void Assembler::vmla(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_10 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLA{<c>}{<q>}.F32 <Dd>, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000d10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMLA{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; T2
    if (dt.Is(F64)) {
      EmitT32_32(0xee000b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VMLA{<c>}{<q>}.<type><size> <Dd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000900U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLA{<c>}{<q>}.F32 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000d10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMLA{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; A2
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e000b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
    // VMLA{<c>}{<q>}.<type><size> <Dd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000900U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmla, &Assembler::vmla, cond, dt, rd, rn, rm);
}

void Assembler::vmla(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_10 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLA{<c>}{<q>}.F32 <Qd>, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000d50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMLA{<c>}{<q>}.<type><size> <Qd>, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000940U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLA{<c>}{<q>}.F32 <Qd>, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000d50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMLA{<c>}{<q>}.<type><size> <Qd>, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000940U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmla, &Assembler::vmla, cond, dt, rd, rn, rm);
}

void Assembler::vmla(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMLA{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; T2
    if (dt.Is(F32)) {
      EmitT32_32(0xee000a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMLA{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; A2
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e000a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVmla, &Assembler::vmla, cond, dt, rd, rn, rm);
}

void Assembler::vmlal(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_11 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLAL{<c>}{<q>}.<type><size> <Qd>, <Dn>, <Dm[x]> ; T1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800240U | (encoded_dt.GetTypeEncodingValue() << 28) |
                   (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                   rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLAL{<c>}{<q>}.<type><size> <Qd>, <Dn>, <Dm[x]> ; A1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800240U | (encoded_dt.GetTypeEncodingValue() << 24) |
                (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVmlal, &Assembler::vmlal, cond, dt, rd, rn, rm);
}

void Assembler::vmlal(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_12 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLAL{<c>}{<q>}.<type><size> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800800U | (encoded_dt.GetTypeEncodingValue() << 28) |
                   (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                   rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLAL{<c>}{<q>}.<type><size> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800800U | (encoded_dt.GetTypeEncodingValue() << 24) |
                (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmlal, &Assembler::vmlal, cond, dt, rd, rn, rm);
}

void Assembler::vmls(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_9 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLS{<c>}{<q>}.<type><size> <Dd>, <Dn>, <Dm[x]> ; T1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800440U | (encoded_dt.GetTypeEncodingValue() << 8) |
                   (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                   rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLS{<c>}{<q>}.<type><size> <Dd>, <Dn>, <Dm[x]> ; A1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800440U | (encoded_dt.GetTypeEncodingValue() << 8) |
                (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVmls, &Assembler::vmls, cond, dt, rd, rn, rm);
}

void Assembler::vmls(
    Condition cond, DataType dt, QRegister rd, QRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_9 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLS{<c>}{<q>}.<type><size> <Qd>, <Qn>, <Dm[x]> ; T1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff800440U | (encoded_dt.GetTypeEncodingValue() << 8) |
                   (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                   rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLS{<c>}{<q>}.<type><size> <Qd>, <Qn>, <Dm[x]> ; A1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al)) {
        EmitA32(0xf3800440U | (encoded_dt.GetTypeEncodingValue() << 8) |
                (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVmls, &Assembler::vmls, cond, dt, rd, rn, rm);
}

void Assembler::vmls(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_10 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLS{<c>}{<q>}.F32 <Dd>, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200d10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMLS{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; T2
    if (dt.Is(F64)) {
      EmitT32_32(0xee000b40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VMLS{<c>}{<q>}.<type><size> <Dd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000900U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLS{<c>}{<q>}.F32 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200d10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMLS{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; A2
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e000b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
    // VMLS{<c>}{<q>}.<type><size> <Dd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3000900U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmls, &Assembler::vmls, cond, dt, rd, rn, rm);
}

void Assembler::vmls(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_10 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLS{<c>}{<q>}.F32 <Qd>, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200d50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMLS{<c>}{<q>}.<type><size> <Qd>, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000940U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLS{<c>}{<q>}.F32 <Qd>, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200d50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMLS{<c>}{<q>}.<type><size> <Qd>, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3000940U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmls, &Assembler::vmls, cond, dt, rd, rn, rm);
}

void Assembler::vmls(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMLS{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; T2
    if (dt.Is(F32)) {
      EmitT32_32(0xee000a40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMLS{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; A2
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e000a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVmls, &Assembler::vmls, cond, dt, rd, rn, rm);
}

void Assembler::vmlsl(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_11 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLSL{<c>}{<q>}.<type><size> <Qd>, <Dn>, <Dm[x]> ; T1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800640U | (encoded_dt.GetTypeEncodingValue() << 28) |
                   (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                   rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLSL{<c>}{<q>}.<type><size> <Qd>, <Dn>, <Dm[x]> ; A1
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1)))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800640U | (encoded_dt.GetTypeEncodingValue() << 24) |
                (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVmlsl, &Assembler::vmlsl, cond, dt, rd, rn, rm);
}

void Assembler::vmlsl(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_12 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMLSL{<c>}{<q>}.<type><size> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800a00U | (encoded_dt.GetTypeEncodingValue() << 28) |
                   (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                   rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMLSL{<c>}{<q>}.<type><size> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800a00U | (encoded_dt.GetTypeEncodingValue() << 24) |
                (encoded_dt.GetEncodingValue() << 20) | rd.Encode(22, 12) |
                rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmlsl, &Assembler::vmlsl, cond, dt, rd, rn, rm);
}

void Assembler::vmov(Condition cond, Register rt, SRegister rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMOV{<c>}{<q>} <Rt>, <Sn> ; T1
    if ((!rt.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xee100a10U | (rt.GetCode() << 12) | rn.Encode(7, 16));
      AdvanceIT();
      return;
    }
  } else {
    // VMOV{<c>}{<q>} <Rt>, <Sn> ; A1
    if (cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
      EmitA32(0x0e100a10U | (cond.GetCondition() << 28) | (rt.GetCode() << 12) |
              rn.Encode(7, 16));
      return;
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, rt, rn);
}

void Assembler::vmov(Condition cond, SRegister rn, Register rt) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMOV{<c>}{<q>} <Sn>, <Rt> ; T1
    if ((!rt.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xee000a10U | rn.Encode(7, 16) | (rt.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // VMOV{<c>}{<q>} <Sn>, <Rt> ; A1
    if (cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
      EmitA32(0x0e000a10U | (cond.GetCondition() << 28) | rn.Encode(7, 16) |
              (rt.GetCode() << 12));
      return;
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, rn, rt);
}

void Assembler::vmov(Condition cond, Register rt, Register rt2, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMOV{<c>}{<q>} <Rt>, <Rt2>, <Dm> ; T1
    if (((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xec500b10U | (rt.GetCode() << 12) | (rt2.GetCode() << 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMOV{<c>}{<q>} <Rt>, <Rt2>, <Dm> ; A1
    if (cond.IsNotNever() &&
        ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0c500b10U | (cond.GetCondition() << 28) | (rt.GetCode() << 12) |
              (rt2.GetCode() << 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, rt, rt2, rm);
}

void Assembler::vmov(Condition cond, DRegister rm, Register rt, Register rt2) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMOV{<c>}{<q>} <Dm>, <Rt>, <Rt2> ; T1
    if (((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xec400b10U | rm.Encode(5, 0) | (rt.GetCode() << 12) |
                 (rt2.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // VMOV{<c>}{<q>} <Dm>, <Rt>, <Rt2> ; A1
    if (cond.IsNotNever() &&
        ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0c400b10U | (cond.GetCondition() << 28) | rm.Encode(5, 0) |
              (rt.GetCode() << 12) | (rt2.GetCode() << 16));
      return;
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, rm, rt, rt2);
}

void Assembler::vmov(
    Condition cond, Register rt, Register rt2, SRegister rm, SRegister rm1) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMOV{<c>}{<q>} <Rt>, <Rt2>, <Sm>, <Sm1> ; T1
    if ((((rm.GetCode() + 1) % kNumberOfSRegisters) == rm1.GetCode()) &&
        ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xec500a10U | (rt.GetCode() << 12) | (rt2.GetCode() << 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMOV{<c>}{<q>} <Rt>, <Rt2>, <Sm>, <Sm1> ; A1
    if ((((rm.GetCode() + 1) % kNumberOfSRegisters) == rm1.GetCode()) &&
        cond.IsNotNever() &&
        ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0c500a10U | (cond.GetCondition() << 28) | (rt.GetCode() << 12) |
              (rt2.GetCode() << 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, rt, rt2, rm, rm1);
}

void Assembler::vmov(
    Condition cond, SRegister rm, SRegister rm1, Register rt, Register rt2) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMOV{<c>}{<q>} <Sm>, <Sm1>, <Rt>, <Rt2> ; T1
    if ((((rm.GetCode() + 1) % kNumberOfSRegisters) == rm1.GetCode()) &&
        ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
      EmitT32_32(0xec400a10U | rm.Encode(5, 0) | (rt.GetCode() << 12) |
                 (rt2.GetCode() << 16));
      AdvanceIT();
      return;
    }
  } else {
    // VMOV{<c>}{<q>} <Sm>, <Sm1>, <Rt>, <Rt2> ; A1
    if ((((rm.GetCode() + 1) % kNumberOfSRegisters) == rm1.GetCode()) &&
        cond.IsNotNever() &&
        ((!rt.IsPC() && !rt2.IsPC()) || AllowUnpredictable())) {
      EmitA32(0x0c400a10U | (cond.GetCondition() << 28) | rm.Encode(5, 0) |
              (rt.GetCode() << 12) | (rt2.GetCode() << 16));
      return;
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, rm, rm1, rt, rt2);
}

void Assembler::vmov(Condition cond,
                     DataType dt,
                     DRegisterLane rd,
                     Register rt) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_opc1_opc2_1 encoded_dt(dt, rd);
  if (IsUsingT32()) {
    // VMOV{<c>}{<q>}{.<size>} <Dd[x]>, <Rt> ; T1
    if (encoded_dt.IsValid() && (!rt.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xee000b10U | ((encoded_dt.GetEncodingValue() & 0x3) << 5) |
                 ((encoded_dt.GetEncodingValue() & 0xc) << 19) |
                 rd.Encode(7, 16) | (rt.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // VMOV{<c>}{<q>}{.<size>} <Dd[x]>, <Rt> ; A1
    if (encoded_dt.IsValid() && cond.IsNotNever() &&
        (!rt.IsPC() || AllowUnpredictable())) {
      EmitA32(0x0e000b10U | (cond.GetCondition() << 28) |
              ((encoded_dt.GetEncodingValue() & 0x3) << 5) |
              ((encoded_dt.GetEncodingValue() & 0xc) << 19) | rd.Encode(7, 16) |
              (rt.GetCode() << 12));
      return;
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, dt, rd, rt);
}

void Assembler::vmov(Condition cond,
                     DataType dt,
                     DRegister rd,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVmov encoded_dt(dt, operand.GetNeonImmediate());
    ImmediateVFP vfp(operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VMOV{<c>}{<q>}.<dt> <Dd>, #<imm> ; T1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(
              0xef800010U | ((encoded_dt.GetEncodingValue() & 0xf) << 8) |
              ((encoded_dt.GetEncodingValue() & 0x10) << 1) |
              rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
              ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
              ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
      // VMOV{<c>}{<q>}.F64 <Dd>, #<imm> ; T2
      if (dt.Is(F64) && vfp.IsValid()) {
        EmitT32_32(0xeeb00b00U | rd.Encode(22, 12) |
                   (vfp.GetEncodingValue() & 0xf) |
                   ((vfp.GetEncodingValue() & 0xf0) << 12));
        AdvanceIT();
        return;
      }
    } else {
      // VMOV{<c>}{<q>}.<dt> <Dd>, #<imm> ; A1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al)) {
          EmitA32(0xf2800010U | ((encoded_dt.GetEncodingValue() & 0xf) << 8) |
                  ((encoded_dt.GetEncodingValue() & 0x10) << 1) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
      // VMOV{<c>}{<q>}.F64 <Dd>, #<imm> ; A2
      if (dt.Is(F64) && vfp.IsValid() && cond.IsNotNever()) {
        EmitA32(0x0eb00b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                (vfp.GetEncodingValue() & 0xf) |
                ((vfp.GetEncodingValue() & 0xf0) << 12));
        return;
      }
    }
  }
  if (operand.IsRegister()) {
    DRegister rm = operand.GetRegister();
    if (IsUsingT32()) {
      // VMOV{<c>}{<q>}.F64 <Dd>, <Dm> ; T2
      if (dt.Is(F64)) {
        EmitT32_32(0xeeb00b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
      // VMOV{<c>}{<q>}{.<dt>} <Dd>, <Dm> ; T1
      if (!dt.Is(F64)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef200110U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                     rm.Encode(5, 0));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VMOV{<c>}{<q>}.F64 <Dd>, <Dm> ; A2
      if (dt.Is(F64) && cond.IsNotNever()) {
        EmitA32(0x0eb00b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                rm.Encode(5, 0));
        return;
      }
      // VMOV{<c>}{<q>}{.<dt>} <Dd>, <Dm> ; A1
      if (!dt.Is(F64)) {
        if (cond.Is(al)) {
          EmitA32(0xf2200110U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                  rm.Encode(5, 0));
          return;
        }
      }
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, dt, rd, operand);
}

void Assembler::vmov(Condition cond,
                     DataType dt,
                     QRegister rd,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVmov encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VMOV{<c>}{<q>}.<dt> <Qd>, #<imm> ; T1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(
              0xef800050U | ((encoded_dt.GetEncodingValue() & 0xf) << 8) |
              ((encoded_dt.GetEncodingValue() & 0x10) << 1) |
              rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
              ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
              ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VMOV{<c>}{<q>}.<dt> <Qd>, #<imm> ; A1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al)) {
          EmitA32(0xf2800050U | ((encoded_dt.GetEncodingValue() & 0xf) << 8) |
                  ((encoded_dt.GetEncodingValue() & 0x10) << 1) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  if (operand.IsRegister()) {
    QRegister rm = operand.GetRegister();
    if (IsUsingT32()) {
      // VMOV{<c>}{<q>}{.<dt>} <Qd>, <Qm> ; T1
      if (!dt.Is(F64)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef200150U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                     rm.Encode(5, 0));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VMOV{<c>}{<q>}{.<dt>} <Qd>, <Qm> ; A1
      if (!dt.Is(F64)) {
        if (cond.Is(al)) {
          EmitA32(0xf2200150U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                  rm.Encode(5, 0));
          return;
        }
      }
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, dt, rd, operand);
}

void Assembler::vmov(Condition cond,
                     DataType dt,
                     SRegister rd,
                     const SOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVFP vfp(operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VMOV{<c>}{<q>}.F32 <Sd>, #<imm> ; T2
      if (dt.Is(F32) && vfp.IsValid()) {
        EmitT32_32(0xeeb00a00U | rd.Encode(22, 12) |
                   (vfp.GetEncodingValue() & 0xf) |
                   ((vfp.GetEncodingValue() & 0xf0) << 12));
        AdvanceIT();
        return;
      }
    } else {
      // VMOV{<c>}{<q>}.F32 <Sd>, #<imm> ; A2
      if (dt.Is(F32) && vfp.IsValid() && cond.IsNotNever()) {
        EmitA32(0x0eb00a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                (vfp.GetEncodingValue() & 0xf) |
                ((vfp.GetEncodingValue() & 0xf0) << 12));
        return;
      }
    }
  }
  if (operand.IsRegister()) {
    SRegister rm = operand.GetRegister();
    if (IsUsingT32()) {
      // VMOV{<c>}{<q>}.F32 <Sd>, <Sm> ; T2
      if (dt.Is(F32)) {
        EmitT32_32(0xeeb00a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VMOV{<c>}{<q>}.F32 <Sd>, <Sm> ; A2
      if (dt.Is(F32) && cond.IsNotNever()) {
        EmitA32(0x0eb00a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, dt, rd, operand);
}

void Assembler::vmov(Condition cond,
                     DataType dt,
                     Register rt,
                     DRegisterLane rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_opc1_opc2_1 encoded_dt(dt, rn);
  if (IsUsingT32()) {
    // VMOV{<c>}{<q>}{.<dt>} <Rt>, <Dn[x]> ; T1
    if (encoded_dt.IsValid() && (!rt.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xee100b10U | ((encoded_dt.GetEncodingValue() & 0x3) << 5) |
                 ((encoded_dt.GetEncodingValue() & 0xc) << 19) |
                 ((encoded_dt.GetEncodingValue() & 0x10) << 19) |
                 (rt.GetCode() << 12) | rn.Encode(7, 16));
      AdvanceIT();
      return;
    }
  } else {
    // VMOV{<c>}{<q>}{.<dt>} <Rt>, <Dn[x]> ; A1
    if (encoded_dt.IsValid() && cond.IsNotNever() &&
        (!rt.IsPC() || AllowUnpredictable())) {
      EmitA32(0x0e100b10U | (cond.GetCondition() << 28) |
              ((encoded_dt.GetEncodingValue() & 0x3) << 5) |
              ((encoded_dt.GetEncodingValue() & 0xc) << 19) |
              ((encoded_dt.GetEncodingValue() & 0x10) << 19) |
              (rt.GetCode() << 12) | rn.Encode(7, 16));
      return;
    }
  }
  Delegate(kVmov, &Assembler::vmov, cond, dt, rt, rn);
}

void Assembler::vmovl(Condition cond, DataType dt, QRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_imm3H_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMOVL{<c>}{<q>}.<dt> <Qd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800a10U | ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                   ((encoded_dt.GetEncodingValue() & 0x8) << 25) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMOVL{<c>}{<q>}.<dt> <Qd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800a10U | ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                ((encoded_dt.GetEncodingValue() & 0x8) << 21) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmovl, &Assembler::vmovl, cond, dt, rd, rm);
}

void Assembler::vmovn(Condition cond, DataType dt, DRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMOVN{<c>}{<q>}.<dt> <Dd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb20200U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMOVN{<c>}{<q>}.<dt> <Dd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b20200U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmovn, &Assembler::vmovn, cond, dt, rd, rm);
}

void Assembler::vmrs(Condition cond,
                     RegisterOrAPSR_nzcv rt,
                     SpecialFPRegister spec_reg) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMRS{<c>}{<q>} <Rt>, <spec_reg> ; T1
    EmitT32_32(0xeef00a10U | (rt.GetCode() << 12) | (spec_reg.GetReg() << 16));
    AdvanceIT();
    return;
  } else {
    // VMRS{<c>}{<q>} <Rt>, <spec_reg> ; A1
    if (cond.IsNotNever()) {
      EmitA32(0x0ef00a10U | (cond.GetCondition() << 28) | (rt.GetCode() << 12) |
              (spec_reg.GetReg() << 16));
      return;
    }
  }
  Delegate(kVmrs, &Assembler::vmrs, cond, rt, spec_reg);
}

void Assembler::vmsr(Condition cond, SpecialFPRegister spec_reg, Register rt) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMSR{<c>}{<q>} <spec_reg>, <Rt> ; T1
    if ((!rt.IsPC() || AllowUnpredictable())) {
      EmitT32_32(0xeee00a10U | (spec_reg.GetReg() << 16) |
                 (rt.GetCode() << 12));
      AdvanceIT();
      return;
    }
  } else {
    // VMSR{<c>}{<q>} <spec_reg>, <Rt> ; A1
    if (cond.IsNotNever() && (!rt.IsPC() || AllowUnpredictable())) {
      EmitA32(0x0ee00a10U | (cond.GetCondition() << 28) |
              (spec_reg.GetReg() << 16) | (rt.GetCode() << 12));
      return;
    }
  }
  Delegate(kVmsr, &Assembler::vmsr, cond, spec_reg, rt);
}

void Assembler::vmul(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rn,
                     DRegister dm,
                     unsigned index) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMUL{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm>[<index>] ; T1
    if (encoded_dt.IsValid() &&
        ((dt.Is(I16) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(I16) && (index <= 1)))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        uint32_t shift = 4;
        if (dt.Is(I16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitT32_32(0xef800840U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                   ((mvm & 0x10) << 1));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMUL{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm>[<index>] ; A1
    if (encoded_dt.IsValid() &&
        ((dt.Is(I16) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(I16) && (index <= 1)))) {
      if (cond.Is(al)) {
        uint32_t shift = 4;
        if (dt.Is(I16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitA32(0xf2800840U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                ((mvm & 0x10) << 1));
        return;
      }
    }
  }
  Delegate(kVmul, &Assembler::vmul, cond, dt, rd, rn, dm, index);
}

void Assembler::vmul(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rn,
                     DRegister dm,
                     unsigned index) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMUL{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm>[<index>] ; T1
    if (encoded_dt.IsValid() &&
        ((dt.Is(I16) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(I16) && (index <= 1)))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        uint32_t shift = 4;
        if (dt.Is(I16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitT32_32(0xff800840U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                   ((mvm & 0x10) << 1));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMUL{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm>[<index>] ; A1
    if (encoded_dt.IsValid() &&
        ((dt.Is(I16) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(I16) && (index <= 1)))) {
      if (cond.Is(al)) {
        uint32_t shift = 4;
        if (dt.Is(I16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitA32(0xf3800840U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                ((mvm & 0x10) << 1));
        return;
      }
    }
  }
  Delegate(kVmul, &Assembler::vmul, cond, dt, rd, rn, dm, index);
}

void Assembler::vmul(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMUL{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000d10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMUL{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; T2
    if (dt.Is(F64)) {
      EmitT32_32(0xee200b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VMUL{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000910U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMUL{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000d10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMUL{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; A2
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e200b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
    // VMUL{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000910U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmul, &Assembler::vmul, cond, dt, rd, rn, rm);
}

void Assembler::vmul(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMUL{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000d50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VMUL{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000950U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMUL{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000d50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VMUL{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000950U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmul, &Assembler::vmul, cond, dt, rd, rn, rm);
}

void Assembler::vmul(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VMUL{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; T2
    if (dt.Is(F32)) {
      EmitT32_32(0xee200a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VMUL{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; A2
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e200a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVmul, &Assembler::vmul, cond, dt, rd, rn, rm);
}

void Assembler::vmull(Condition cond,
                      DataType dt,
                      QRegister rd,
                      DRegister rn,
                      DRegister dm,
                      unsigned index) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_2 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMULL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm>[<index>] ; T1
    if (encoded_dt.IsValid() &&
        (((dt.Is(S16) || dt.Is(U16)) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(S16) && !dt.Is(U16) && (index <= 1)))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        uint32_t shift = 4;
        if (dt.Is(S16) || dt.Is(U16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitT32_32(0xef800a40U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                   ((mvm & 0x10) << 1));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMULL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm>[<index>] ; A1
    if (encoded_dt.IsValid() &&
        (((dt.Is(S16) || dt.Is(U16)) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(S16) && !dt.Is(U16) && (index <= 1)))) {
      if (cond.Is(al)) {
        uint32_t shift = 4;
        if (dt.Is(S16) || dt.Is(U16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitA32(0xf2800a40U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                ((mvm & 0x10) << 1));
        return;
      }
    }
  }
  Delegate(kVmull, &Assembler::vmull, cond, dt, rd, rn, dm, index);
}

void Assembler::vmull(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VMULL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800c00U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   ((encoded_dt.GetEncodingValue() & 0x8) << 6) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VMULL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800c00U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                ((encoded_dt.GetEncodingValue() & 0x8) << 6) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmull, &Assembler::vmull, cond, dt, rd, rn, rm);
}

void Assembler::vmvn(Condition cond,
                     DataType dt,
                     DRegister rd,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVmvn encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VMVN{<c>}{<q>}.<dt> <Dd>, #<imm> ; T1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800030U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VMVN{<c>}{<q>}.<dt> <Dd>, #<imm> ; A1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al)) {
          EmitA32(0xf2800030U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  if (operand.IsRegister()) {
    DRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VMVN{<c>}{<q>}{.<dt>} <Dd>, <Dm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00580U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VMVN{<c>}{<q>}{.<dt>} <Dd>, <Dm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf3b00580U | rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmvn, &Assembler::vmvn, cond, dt, rd, operand);
}

void Assembler::vmvn(Condition cond,
                     DataType dt,
                     QRegister rd,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVmvn encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VMVN{<c>}{<q>}.<dt> <Qd>, #<imm> ; T1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800070U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VMVN{<c>}{<q>}.<dt> <Qd>, #<imm> ; A1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al)) {
          EmitA32(0xf2800070U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  if (operand.IsRegister()) {
    QRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VMVN{<c>}{<q>}{.<dt>} <Qd>, <Qm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb005c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VMVN{<c>}{<q>}{.<dt>} <Qd>, <Qm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf3b005c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVmvn, &Assembler::vmvn, cond, dt, rd, operand);
}

void Assembler::vneg(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VNEG{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb10380U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VNEG{<c>}{<q>}.F64 <Dd>, <Dm> ; T2
    if (dt.Is(F64)) {
      EmitT32_32(0xeeb10b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VNEG{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b10380U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
    // VNEG{<c>}{<q>}.F64 <Dd>, <Dm> ; A2
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb10b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVneg, &Assembler::vneg, cond, dt, rd, rm);
}

void Assembler::vneg(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VNEG{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb103c0U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VNEG{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b103c0U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 8) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVneg, &Assembler::vneg, cond, dt, rd, rm);
}

void Assembler::vneg(Condition cond, DataType dt, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VNEG{<c>}{<q>}.F32 <Sd>, <Sm> ; T2
    if (dt.Is(F32)) {
      EmitT32_32(0xeeb10a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VNEG{<c>}{<q>}.F32 <Sd>, <Sm> ; A2
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb10a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVneg, &Assembler::vneg, cond, dt, rd, rm);
}

void Assembler::vnmla(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VNMLA{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; T1
    if (dt.Is(F32)) {
      EmitT32_32(0xee100a40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VNMLA{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; A1
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e100a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVnmla, &Assembler::vnmla, cond, dt, rd, rn, rm);
}

void Assembler::vnmla(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VNMLA{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; T1
    if (dt.Is(F64)) {
      EmitT32_32(0xee100b40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VNMLA{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e100b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVnmla, &Assembler::vnmla, cond, dt, rd, rn, rm);
}

void Assembler::vnmls(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VNMLS{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; T1
    if (dt.Is(F32)) {
      EmitT32_32(0xee100a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VNMLS{<c>}{<q>}.F32 <Sd>, <Sn>, <Sm> ; A1
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e100a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVnmls, &Assembler::vnmls, cond, dt, rd, rn, rm);
}

void Assembler::vnmls(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VNMLS{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; T1
    if (dt.Is(F64)) {
      EmitT32_32(0xee100b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VNMLS{<c>}{<q>}.F64 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e100b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVnmls, &Assembler::vnmls, cond, dt, rd, rn, rm);
}

void Assembler::vnmul(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VNMUL{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; T1
    if (dt.Is(F32)) {
      EmitT32_32(0xee200a40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VNMUL{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; A1
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e200a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVnmul, &Assembler::vnmul, cond, dt, rd, rn, rm);
}

void Assembler::vnmul(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VNMUL{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F64)) {
      EmitT32_32(0xee200b40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VNMUL{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e200b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVnmul, &Assembler::vnmul, cond, dt, rd, rn, rm);
}

void Assembler::vorn(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rn,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVorn encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VORN{<c>}{<q>}.<dt> {<Ddn>}, <Ddn>, #<imm> ; T1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800010U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VORN{<c>}{<q>}.<dt> {<Ddn>}, <Ddn>, #<imm> ; A1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al)) {
          EmitA32(0xf2800010U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  if (operand.IsRegister()) {
    DRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VORN{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef300110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VORN{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf2300110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVorn, &Assembler::vorn, cond, dt, rd, rn, operand);
}

void Assembler::vorn(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rn,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    ImmediateVorn encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VORN{<c>}{<q>}.<dt> {<Qdn>}, <Qdn>, #<imm> ; T1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800050U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VORN{<c>}{<q>}.<dt> {<Qdn>}, <Qdn>, #<imm> ; A1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al)) {
          EmitA32(0xf2800050U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  if (operand.IsRegister()) {
    QRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VORN{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef300150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VORN{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf2300150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVorn, &Assembler::vorn, cond, dt, rd, rn, operand);
}

void Assembler::vorr(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rn,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsRegister()) {
    DRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VORR{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VORR{<c>}{<q>}{.<dt>} {<Dd>}, <Dn>, <Dm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf2200110U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  if (operand.IsImmediate()) {
    ImmediateVorr encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VORR{<c>}{<q>}.<dt> {<Ddn>}, <Ddn>, #<imm> ; T1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800010U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VORR{<c>}{<q>}.<dt> {<Ddn>}, <Ddn>, #<imm> ; A1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al)) {
          EmitA32(0xf2800010U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  Delegate(kVorr, &Assembler::vorr, cond, dt, rd, rn, operand);
}

void Assembler::vorr(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rn,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsRegister()) {
    QRegister rm = operand.GetRegister();
    USE(dt);
    if (IsUsingT32()) {
      // VORR{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; T1
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    } else {
      // VORR{<c>}{<q>}{.<dt>} {<Qd>}, <Qn>, <Qm> ; A1
      if (cond.Is(al)) {
        EmitA32(0xf2200150U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  if (operand.IsImmediate()) {
    ImmediateVorr encoded_dt(dt, operand.GetNeonImmediate());
    if (IsUsingT32()) {
      // VORR{<c>}{<q>}.<dt> {<Qdn>}, <Qdn>, #<imm> ; T1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef800050U | (encoded_dt.GetEncodingValue() << 8) |
                     rd.Encode(22, 12) |
                     (encoded_dt.GetEncodedImmediate() & 0xf) |
                     ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                     ((encoded_dt.GetEncodedImmediate() & 0x80) << 21));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VORR{<c>}{<q>}.<dt> {<Qdn>}, <Qdn>, #<imm> ; A1
      if (encoded_dt.IsValid() && rd.Is(rn)) {
        if (cond.Is(al)) {
          EmitA32(0xf2800050U | (encoded_dt.GetEncodingValue() << 8) |
                  rd.Encode(22, 12) | (encoded_dt.GetEncodedImmediate() & 0xf) |
                  ((encoded_dt.GetEncodedImmediate() & 0x70) << 12) |
                  ((encoded_dt.GetEncodedImmediate() & 0x80) << 17));
          return;
        }
      }
    }
  }
  Delegate(kVorr, &Assembler::vorr, cond, dt, rd, rn, operand);
}

void Assembler::vpadal(Condition cond,
                       DataType dt,
                       DRegister rd,
                       DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_size_2 encoded_dt(dt);
  if (IsUsingT32()) {
    // VPADAL{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00600U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 5) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VPADAL{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00600U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 5) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVpadal, &Assembler::vpadal, cond, dt, rd, rm);
}

void Assembler::vpadal(Condition cond,
                       DataType dt,
                       QRegister rd,
                       QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_size_2 encoded_dt(dt);
  if (IsUsingT32()) {
    // VPADAL{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00640U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 5) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VPADAL{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00640U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 5) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVpadal, &Assembler::vpadal, cond, dt, rd, rm);
}

void Assembler::vpadd(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_4 encoded_dt(dt);
  if (IsUsingT32()) {
    // VPADD{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000d00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VPADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000b10U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VPADD{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000d00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VPADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000b10U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVpadd, &Assembler::vpadd, cond, dt, rd, rn, rm);
}

void Assembler::vpaddl(Condition cond,
                       DataType dt,
                       DRegister rd,
                       DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_size_2 encoded_dt(dt);
  if (IsUsingT32()) {
    // VPADDL{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00200U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 5) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VPADDL{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00200U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 5) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVpaddl, &Assembler::vpaddl, cond, dt, rd, rm);
}

void Assembler::vpaddl(Condition cond,
                       DataType dt,
                       QRegister rd,
                       QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_size_2 encoded_dt(dt);
  if (IsUsingT32()) {
    // VPADDL{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00240U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 5) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VPADDL{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00240U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 5) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVpaddl, &Assembler::vpaddl, cond, dt, rd, rm);
}

void Assembler::vpmax(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VPMAX{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000f00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VPMAX{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000a00U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VPMAX{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3000f00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VPMAX{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000a00U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVpmax, &Assembler::vpmax, cond, dt, rd, rn, rm);
}

void Assembler::vpmin(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VPMIN{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff200f00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VPMIN{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000a10U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VPMIN{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3200f00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VPMIN{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000a10U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVpmin, &Assembler::vpmin, cond, dt, rd, rn, rm);
}

void Assembler::vpop(Condition cond, DataType dt, DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VPOP{<c>}{<q>}{.<size>} <dreglist> ; T1
    if (((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xecbd0b00U | dreg.Encode(22, 12) | (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VPOP{<c>}{<q>}{.<size>} <dreglist> ; A1
    if (cond.IsNotNever() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0cbd0b00U | (cond.GetCondition() << 28) | dreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVpop, &Assembler::vpop, cond, dt, dreglist);
}

void Assembler::vpop(Condition cond, DataType dt, SRegisterList sreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VPOP{<c>}{<q>}{.<size>} <sreglist> ; T2
    const SRegister& sreg = sreglist.GetFirstSRegister();
    unsigned len = sreglist.GetLength();
    EmitT32_32(0xecbd0a00U | sreg.Encode(22, 12) | (len & 0xff));
    AdvanceIT();
    return;
  } else {
    // VPOP{<c>}{<q>}{.<size>} <sreglist> ; A2
    if (cond.IsNotNever()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitA32(0x0cbd0a00U | (cond.GetCondition() << 28) | sreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVpop, &Assembler::vpop, cond, dt, sreglist);
}

void Assembler::vpush(Condition cond, DataType dt, DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VPUSH{<c>}{<q>}{.<size>} <dreglist> ; T1
    if (((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xed2d0b00U | dreg.Encode(22, 12) | (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VPUSH{<c>}{<q>}{.<size>} <dreglist> ; A1
    if (cond.IsNotNever() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0d2d0b00U | (cond.GetCondition() << 28) | dreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVpush, &Assembler::vpush, cond, dt, dreglist);
}

void Assembler::vpush(Condition cond, DataType dt, SRegisterList sreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VPUSH{<c>}{<q>}{.<size>} <sreglist> ; T2
    const SRegister& sreg = sreglist.GetFirstSRegister();
    unsigned len = sreglist.GetLength();
    EmitT32_32(0xed2d0a00U | sreg.Encode(22, 12) | (len & 0xff));
    AdvanceIT();
    return;
  } else {
    // VPUSH{<c>}{<q>}{.<size>} <sreglist> ; A2
    if (cond.IsNotNever()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitA32(0x0d2d0a00U | (cond.GetCondition() << 28) | sreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVpush, &Assembler::vpush, cond, dt, sreglist);
}

void Assembler::vqabs(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_5 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQABS{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00700U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQABS{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00700U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqabs, &Assembler::vqabs, cond, dt, rd, rm);
}

void Assembler::vqabs(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_5 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQABS{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00740U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQABS{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00740U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqabs, &Assembler::vqabs, cond, dt, rd, rm);
}

void Assembler::vqadd(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000010U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000010U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqadd, &Assembler::vqadd, cond, dt, rd, rn, rm);
}

void Assembler::vqadd(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQADD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000050U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQADD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000050U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqadd, &Assembler::vqadd, cond, dt, rd, rn, rm);
}

void Assembler::vqdmlal(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMLAL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid() && (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800900U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMLAL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid() && (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800900U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqdmlal, &Assembler::vqdmlal, cond, dt, rd, rn, rm);
}

void Assembler::vqdmlal(Condition cond,
                        DataType dt,
                        QRegister rd,
                        DRegister rn,
                        DRegister dm,
                        unsigned index) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMLAL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm>[<index>] ; T2
    if (encoded_dt.IsValid() &&
        ((dt.Is(S16) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(S16) && (index <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        uint32_t shift = 4;
        if (dt.Is(S16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitT32_32(0xef800340U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                   ((mvm & 0x10) << 1));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMLAL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm>[<index>] ; A2
    if (encoded_dt.IsValid() &&
        ((dt.Is(S16) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(S16) && (index <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        uint32_t shift = 4;
        if (dt.Is(S16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitA32(0xf2800340U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                ((mvm & 0x10) << 1));
        return;
      }
    }
  }
  Delegate(kVqdmlal, &Assembler::vqdmlal, cond, dt, rd, rn, dm, index);
}

void Assembler::vqdmlsl(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMLSL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid() && (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800b00U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMLSL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid() && (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800b00U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqdmlsl, &Assembler::vqdmlsl, cond, dt, rd, rn, rm);
}

void Assembler::vqdmlsl(Condition cond,
                        DataType dt,
                        QRegister rd,
                        DRegister rn,
                        DRegister dm,
                        unsigned index) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMLSL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm>[<index>] ; T2
    if (encoded_dt.IsValid() &&
        ((dt.Is(S16) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(S16) && (index <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        uint32_t shift = 4;
        if (dt.Is(S16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitT32_32(0xef800740U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                   ((mvm & 0x10) << 1));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMLSL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm>[<index>] ; A2
    if (encoded_dt.IsValid() &&
        ((dt.Is(S16) && (index <= 3) && (dm.GetCode() <= 7)) ||
         (!dt.Is(S16) && (index <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        uint32_t shift = 4;
        if (dt.Is(S16)) {
          shift = 3;
        }
        uint32_t mvm = dm.GetCode() | index << shift;
        EmitA32(0xf2800740U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | (mvm & 0xf) |
                ((mvm & 0x10) << 1));
        return;
      }
    }
  }
  Delegate(kVqdmlsl, &Assembler::vqdmlsl, cond, dt, rd, rn, dm, index);
}

void Assembler::vqdmulh(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMULH{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000b00U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMULH{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000b00U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqdmulh, &Assembler::vqdmulh, cond, dt, rd, rn, rm);
}

void Assembler::vqdmulh(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMULH{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000b40U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMULH{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000b40U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqdmulh, &Assembler::vqdmulh, cond, dt, rd, rn, rm);
}

void Assembler::vqdmulh(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMULH{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm[x]> ; T2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800c40U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMULH{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm[x]> ; A2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800c40U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVqdmulh, &Assembler::vqdmulh, cond, dt, rd, rn, rm);
}

void Assembler::vqdmulh(
    Condition cond, DataType dt, QRegister rd, QRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMULH{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm[x]> ; T2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff800c40U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMULH{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm[x]> ; A2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        EmitA32(0xf3800c40U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVqdmulh, &Assembler::vqdmulh, cond, dt, rd, rn, rm);
}

void Assembler::vqdmull(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMULL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid() && (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800d00U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMULL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid() && (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800d00U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqdmull, &Assembler::vqdmull, cond, dt, rd, rn, rm);
}

void Assembler::vqdmull(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQDMULL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm[x]> ; T2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800b40U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQDMULL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm[x]> ; A2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800b40U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVqdmull, &Assembler::vqdmull, cond, dt, rd, rn, rm);
}

void Assembler::vqmovn(Condition cond,
                       DataType dt,
                       DRegister rd,
                       QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_op_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQMOVN{<c>}{<q>}.<dt> <Dd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb20280U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0xc) << 4) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQMOVN{<c>}{<q>}.<dt> <Dd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b20280U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0xc) << 4) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqmovn, &Assembler::vqmovn, cond, dt, rd, rm);
}

void Assembler::vqmovun(Condition cond,
                        DataType dt,
                        DRegister rd,
                        QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_14 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQMOVUN{<c>}{<q>}.<dt> <Dd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb20240U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQMOVUN{<c>}{<q>}.<dt> <Dd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b20240U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqmovun, &Assembler::vqmovun, cond, dt, rd, rm);
}

void Assembler::vqneg(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_5 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQNEG{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00780U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQNEG{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00780U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqneg, &Assembler::vqneg, cond, dt, rd, rm);
}

void Assembler::vqneg(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_5 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQNEG{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb007c0U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQNEG{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b007c0U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqneg, &Assembler::vqneg, cond, dt, rd, rm);
}

void Assembler::vqrdmulh(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQRDMULH{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000b00U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQRDMULH{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3000b00U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqrdmulh, &Assembler::vqrdmulh, cond, dt, rd, rn, rm);
}

void Assembler::vqrdmulh(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQRDMULH{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000b40U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQRDMULH{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3000b40U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqrdmulh, &Assembler::vqrdmulh, cond, dt, rd, rn, rm);
}

void Assembler::vqrdmulh(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQRDMULH{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm[x]> ; T2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800d40U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQRDMULH{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm[x]> ; A2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800d40U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVqrdmulh, &Assembler::vqrdmulh, cond, dt, rd, rn, rm);
}

void Assembler::vqrdmulh(
    Condition cond, DataType dt, QRegister rd, QRegister rn, DRegisterLane rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_13 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQRDMULH{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm[x]> ; T2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff800d40U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQRDMULH{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm[x]> ; A2
    if (encoded_dt.IsValid() &&
        (((dt.GetSize() == 16) && (rm.GetCode() <= 7) && (rm.GetLane() <= 3)) ||
         ((dt.GetSize() == 32) && (rm.GetCode() <= 15) &&
          (rm.GetLane() <= 1))) &&
        (dt.Is(S16) || dt.Is(S32))) {
      if (cond.Is(al)) {
        EmitA32(0xf3800d40U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.EncodeX(dt, 5, 0));
        return;
      }
    }
  }
  Delegate(kVqrdmulh, &Assembler::vqrdmulh, cond, dt, rd, rn, rm);
}

void Assembler::vqrshl(
    Condition cond, DataType dt, DRegister rd, DRegister rm, DRegister rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQRSHL{<c>}{<q>}.<dt> {<Dd>}, <Dm>, <Dn> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000510U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQRSHL{<c>}{<q>}.<dt> {<Dd>}, <Dm>, <Dn> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000510U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
        return;
      }
    }
  }
  Delegate(kVqrshl, &Assembler::vqrshl, cond, dt, rd, rm, rn);
}

void Assembler::vqrshl(
    Condition cond, DataType dt, QRegister rd, QRegister rm, QRegister rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQRSHL{<c>}{<q>}.<dt> {<Qd>}, <Qm>, <Qn> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000550U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQRSHL{<c>}{<q>}.<dt> {<Qd>}, <Qm>, <Qn> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000550U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
        return;
      }
    }
  }
  Delegate(kVqrshl, &Assembler::vqrshl, cond, dt, rd, rm, rn);
}

void Assembler::vqrshrn(Condition cond,
                        DataType dt,
                        DRegister rd,
                        QRegister rm,
                        const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_op_size_3 encoded_dt(dt);
      Dt_imm6_1 encoded_dt_2(dt);
      if (IsUsingT32()) {
        // VQRSHRN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb20280U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0xc) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
        // VQRSHRN{<c>}{<q>}.<type><size> <Dd>, <Qm>, #<imm> ; T1
        if (encoded_dt_2.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitT32_32(0xef800950U |
                       (encoded_dt_2.GetTypeEncodingValue() << 28) |
                       ((encoded_dt_2.GetEncodingValue() & 0x7) << 19) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VQRSHRN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b20280U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0xc) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
        // VQRSHRN{<c>}{<q>}.<type><size> <Dd>, <Qm>, #<imm> ; A1
        if (encoded_dt_2.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitA32(0xf2800950U | (encoded_dt_2.GetTypeEncodingValue() << 24) |
                    ((encoded_dt_2.GetEncodingValue() & 0x7) << 19) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVqrshrn, &Assembler::vqrshrn, cond, dt, rd, rm, operand);
}

void Assembler::vqrshrun(Condition cond,
                         DataType dt,
                         DRegister rd,
                         QRegister rm,
                         const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_imm6_2 encoded_dt(dt);
      Dt_size_14 encoded_dt_2(dt);
      if (IsUsingT32()) {
        // VQRSHRUN{<c>}{<q>}.<type><size> <Dd>, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitT32_32(0xff800850U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
        // VQRSHRUN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; T1
        if (encoded_dt_2.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb20240U | (encoded_dt_2.GetEncodingValue() << 18) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VQRSHRUN{<c>}{<q>}.<type><size> <Dd>, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitA32(0xf3800850U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
        // VQRSHRUN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; A1
        if (encoded_dt_2.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b20240U | (encoded_dt_2.GetEncodingValue() << 18) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVqrshrun, &Assembler::vqrshrun, cond, dt, rd, rm, operand);
}

void Assembler::vqshl(Condition cond,
                      DataType dt,
                      DRegister rd,
                      DRegister rm,
                      const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsRegister()) {
    DRegister rn = operand.GetRegister();
    Dt_U_size_3 encoded_dt(dt);
    if (IsUsingT32()) {
      // VQSHL{<c>}{<q>}.<dt> {<Dd>}, <Dm>, <Dn> ; T1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef000410U |
                     ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                     ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                     rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VQSHL{<c>}{<q>}.<dt> {<Dd>}, <Dm>, <Dn> ; A1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al)) {
          EmitA32(0xf2000410U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                  ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                  rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
          return;
        }
      }
    }
  }
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VQSHL{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = imm;
            EmitT32_32(0xef800710U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VQSHL{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al)) {
            uint32_t imm6 = imm;
            EmitA32(0xf2800710U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVqshl, &Assembler::vqshl, cond, dt, rd, rm, operand);
}

void Assembler::vqshl(Condition cond,
                      DataType dt,
                      QRegister rd,
                      QRegister rm,
                      const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsRegister()) {
    QRegister rn = operand.GetRegister();
    Dt_U_size_3 encoded_dt(dt);
    if (IsUsingT32()) {
      // VQSHL{<c>}{<q>}.<dt> {<Qd>}, <Qm>, <Qn> ; T1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef000450U |
                     ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                     ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                     rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VQSHL{<c>}{<q>}.<dt> {<Qd>}, <Qm>, <Qn> ; A1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al)) {
          EmitA32(0xf2000450U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                  ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                  rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
          return;
        }
      }
    }
  }
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VQSHL{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = imm;
            EmitT32_32(0xef800750U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VQSHL{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al)) {
            uint32_t imm6 = imm;
            EmitA32(0xf2800750U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVqshl, &Assembler::vqshl, cond, dt, rd, rm, operand);
}

void Assembler::vqshlu(Condition cond,
                       DataType dt,
                       DRegister rd,
                       DRegister rm,
                       const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_2 encoded_dt(dt);
      if (IsUsingT32()) {
        // VQSHLU{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = imm;
            EmitT32_32(0xef800610U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VQSHLU{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al)) {
            uint32_t imm6 = imm;
            EmitA32(0xf2800610U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVqshlu, &Assembler::vqshlu, cond, dt, rd, rm, operand);
}

void Assembler::vqshlu(Condition cond,
                       DataType dt,
                       QRegister rd,
                       QRegister rm,
                       const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_2 encoded_dt(dt);
      if (IsUsingT32()) {
        // VQSHLU{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = imm;
            EmitT32_32(0xef800650U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VQSHLU{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al)) {
            uint32_t imm6 = imm;
            EmitA32(0xf2800650U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVqshlu, &Assembler::vqshlu, cond, dt, rd, rm, operand);
}

void Assembler::vqshrn(Condition cond,
                       DataType dt,
                       DRegister rd,
                       QRegister rm,
                       const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_op_size_3 encoded_dt(dt);
      Dt_imm6_1 encoded_dt_2(dt);
      if (IsUsingT32()) {
        // VQSHRN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; T1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb20280U |
                       ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                       ((encoded_dt.GetEncodingValue() & 0xc) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
        // VQSHRN{<c>}{<q>}.<type><size> <Dd>, <Qm>, #<imm> ; T1
        if (encoded_dt_2.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitT32_32(0xef800910U |
                       (encoded_dt_2.GetTypeEncodingValue() << 28) |
                       ((encoded_dt_2.GetEncodingValue() & 0x7) << 19) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VQSHRN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; A1
        if (encoded_dt.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b20280U |
                    ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                    ((encoded_dt.GetEncodingValue() & 0xc) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
        // VQSHRN{<c>}{<q>}.<type><size> <Dd>, <Qm>, #<imm> ; A1
        if (encoded_dt_2.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitA32(0xf2800910U | (encoded_dt_2.GetTypeEncodingValue() << 24) |
                    ((encoded_dt_2.GetEncodingValue() & 0x7) << 19) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVqshrn, &Assembler::vqshrn, cond, dt, rd, rm, operand);
}

void Assembler::vqshrun(Condition cond,
                        DataType dt,
                        DRegister rd,
                        QRegister rm,
                        const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_imm6_2 encoded_dt(dt);
      Dt_size_14 encoded_dt_2(dt);
      if (IsUsingT32()) {
        // VQSHRUN{<c>}{<q>}.<type><size> <Dd>, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitT32_32(0xff800810U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
        // VQSHRUN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; T1
        if (encoded_dt_2.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb20240U | (encoded_dt_2.GetEncodingValue() << 18) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VQSHRUN{<c>}{<q>}.<type><size> <Dd>, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitA32(0xf3800810U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
        // VQSHRUN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; A1
        if (encoded_dt_2.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b20240U | (encoded_dt_2.GetEncodingValue() << 18) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVqshrun, &Assembler::vqshrun, cond, dt, rd, rm, operand);
}

void Assembler::vqsub(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQSUB{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000210U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQSUB{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000210U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqsub, &Assembler::vqsub, cond, dt, rd, rn, rm);
}

void Assembler::vqsub(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VQSUB{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000250U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VQSUB{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000250U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVqsub, &Assembler::vqsub, cond, dt, rd, rn, rm);
}

void Assembler::vraddhn(
    Condition cond, DataType dt, DRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRADDHN{<c>}{<q>}.<dt> <Dd>, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid() && (dt.Is(I16) || dt.Is(I32) || dt.Is(I64))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff800400U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRADDHN{<c>}{<q>}.<dt> <Dd>, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid() && (dt.Is(I16) || dt.Is(I32) || dt.Is(I64))) {
      if (cond.Is(al)) {
        EmitA32(0xf3800400U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVraddhn, &Assembler::vraddhn, cond, dt, rd, rn, rm);
}

void Assembler::vrecpe(Condition cond,
                       DataType dt,
                       DRegister rd,
                       DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_4 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRECPE{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb30400U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRECPE{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b30400U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrecpe, &Assembler::vrecpe, cond, dt, rd, rm);
}

void Assembler::vrecpe(Condition cond,
                       DataType dt,
                       QRegister rd,
                       QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_4 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRECPE{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb30440U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRECPE{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b30440U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrecpe, &Assembler::vrecpe, cond, dt, rd, rm);
}

void Assembler::vrecps(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRECPS{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000f10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRECPS{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000f10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrecps, &Assembler::vrecps, cond, dt, rd, rn, rm);
}

void Assembler::vrecps(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRECPS{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000f50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRECPS{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2000f50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrecps, &Assembler::vrecps, cond, dt, rd, rn, rm);
}

void Assembler::vrev16(Condition cond,
                       DataType dt,
                       DRegister rd,
                       DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VREV16{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00100U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VREV16{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00100U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrev16, &Assembler::vrev16, cond, dt, rd, rm);
}

void Assembler::vrev16(Condition cond,
                       DataType dt,
                       QRegister rd,
                       QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VREV16{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00140U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VREV16{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00140U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrev16, &Assembler::vrev16, cond, dt, rd, rm);
}

void Assembler::vrev32(Condition cond,
                       DataType dt,
                       DRegister rd,
                       DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_15 encoded_dt(dt);
  if (IsUsingT32()) {
    // VREV32{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00080U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VREV32{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00080U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrev32, &Assembler::vrev32, cond, dt, rd, rm);
}

void Assembler::vrev32(Condition cond,
                       DataType dt,
                       QRegister rd,
                       QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_15 encoded_dt(dt);
  if (IsUsingT32()) {
    // VREV32{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb000c0U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VREV32{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b000c0U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrev32, &Assembler::vrev32, cond, dt, rd, rm);
}

void Assembler::vrev64(Condition cond,
                       DataType dt,
                       DRegister rd,
                       DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_7 encoded_dt(dt);
  if (IsUsingT32()) {
    // VREV64{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00000U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VREV64{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00000U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrev64, &Assembler::vrev64, cond, dt, rd, rm);
}

void Assembler::vrev64(Condition cond,
                       DataType dt,
                       QRegister rd,
                       QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_7 encoded_dt(dt);
  if (IsUsingT32()) {
    // VREV64{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb00040U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VREV64{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b00040U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrev64, &Assembler::vrev64, cond, dt, rd, rm);
}

void Assembler::vrhadd(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRHADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000100U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRHADD{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000100U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrhadd, &Assembler::vrhadd, cond, dt, rd, rn, rm);
}

void Assembler::vrhadd(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRHADD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000140U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRHADD{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000140U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrhadd, &Assembler::vrhadd, cond, dt, rd, rn, rm);
}

void Assembler::vrinta(DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTA{<q>}.F32.F32 <Dd>, <Dm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba0500U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VRINTA{<q>}.F64.F64 <Dd>, <Dm> ; T1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitT32_32(0xfeb80b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTA{<q>}.F32.F32 <Dd>, <Dm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba0500U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
    // VRINTA{<q>}.F64.F64 <Dd>, <Dm> ; A1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitA32(0xfeb80b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrinta, &Assembler::vrinta, dt1, dt2, rd, rm);
}

void Assembler::vrinta(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTA{<q>}.F32.F32 <Qd>, <Qm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba0540U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTA{<q>}.F32.F32 <Qd>, <Qm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba0540U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrinta, &Assembler::vrinta, dt1, dt2, rd, rm);
}

void Assembler::vrinta(DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTA{<q>}.F32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xfeb80a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTA{<q>}.F32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xfeb80a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrinta, &Assembler::vrinta, dt1, dt2, rd, rm);
}

void Assembler::vrintm(DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTM{<q>}.F32.F32 <Dd>, <Dm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba0680U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VRINTM{<q>}.F64.F64 <Dd>, <Dm> ; T1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitT32_32(0xfebb0b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTM{<q>}.F32.F32 <Dd>, <Dm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba0680U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
    // VRINTM{<q>}.F64.F64 <Dd>, <Dm> ; A1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitA32(0xfebb0b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintm, &Assembler::vrintm, dt1, dt2, rd, rm);
}

void Assembler::vrintm(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTM{<q>}.F32.F32 <Qd>, <Qm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba06c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTM{<q>}.F32.F32 <Qd>, <Qm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba06c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintm, &Assembler::vrintm, dt1, dt2, rd, rm);
}

void Assembler::vrintm(DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTM{<q>}.F32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xfebb0a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTM{<q>}.F32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xfebb0a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintm, &Assembler::vrintm, dt1, dt2, rd, rm);
}

void Assembler::vrintn(DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTN{<q>}.F32.F32 <Dd>, <Dm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba0400U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VRINTN{<q>}.F64.F64 <Dd>, <Dm> ; T1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitT32_32(0xfeb90b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTN{<q>}.F32.F32 <Dd>, <Dm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba0400U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
    // VRINTN{<q>}.F64.F64 <Dd>, <Dm> ; A1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitA32(0xfeb90b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintn, &Assembler::vrintn, dt1, dt2, rd, rm);
}

void Assembler::vrintn(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTN{<q>}.F32.F32 <Qd>, <Qm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba0440U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTN{<q>}.F32.F32 <Qd>, <Qm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba0440U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintn, &Assembler::vrintn, dt1, dt2, rd, rm);
}

void Assembler::vrintn(DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTN{<q>}.F32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xfeb90a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTN{<q>}.F32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xfeb90a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintn, &Assembler::vrintn, dt1, dt2, rd, rm);
}

void Assembler::vrintp(DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTP{<q>}.F32.F32 <Dd>, <Dm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba0780U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VRINTP{<q>}.F64.F64 <Dd>, <Dm> ; T1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitT32_32(0xfeba0b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTP{<q>}.F32.F32 <Dd>, <Dm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba0780U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
    // VRINTP{<q>}.F64.F64 <Dd>, <Dm> ; A1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitA32(0xfeba0b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintp, &Assembler::vrintp, dt1, dt2, rd, rm);
}

void Assembler::vrintp(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTP{<q>}.F32.F32 <Qd>, <Qm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba07c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTP{<q>}.F32.F32 <Qd>, <Qm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba07c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintp, &Assembler::vrintp, dt1, dt2, rd, rm);
}

void Assembler::vrintp(DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTP{<q>}.F32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xfeba0a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTP{<q>}.F32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xfeba0a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintp, &Assembler::vrintp, dt1, dt2, rd, rm);
}

void Assembler::vrintr(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRINTR{<c>}{<q>}.F32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xeeb60a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTR{<c>}{<q>}.F32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb60a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintr, &Assembler::vrintr, cond, dt1, dt2, rd, rm);
}

void Assembler::vrintr(
    Condition cond, DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRINTR{<c>}{<q>}.F64.F64 <Dd>, <Dm> ; T1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitT32_32(0xeeb60b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTR{<c>}{<q>}.F64.F64 <Dd>, <Dm> ; A1
    if (dt1.Is(F64) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb60b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintr, &Assembler::vrintr, cond, dt1, dt2, rd, rm);
}

void Assembler::vrintx(
    Condition cond, DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRINTX{<q>}.F32.F32 <Dd>, <Dm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba0480U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VRINTX{<c>}{<q>}.F64.F64 <Dd>, <Dm> ; T1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitT32_32(0xeeb70b40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTX{<q>}.F32.F32 <Dd>, <Dm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba0480U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
    // VRINTX{<c>}{<q>}.F64.F64 <Dd>, <Dm> ; A1
    if (dt1.Is(F64) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb70b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintx, &Assembler::vrintx, cond, dt1, dt2, rd, rm);
}

void Assembler::vrintx(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTX{<q>}.F32.F32 <Qd>, <Qm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba04c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTX{<q>}.F32.F32 <Qd>, <Qm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba04c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintx, &Assembler::vrintx, dt1, dt2, rd, rm);
}

void Assembler::vrintx(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRINTX{<c>}{<q>}.F32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xeeb70a40U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTX{<c>}{<q>}.F32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb70a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintx, &Assembler::vrintx, cond, dt1, dt2, rd, rm);
}

void Assembler::vrintz(
    Condition cond, DataType dt1, DataType dt2, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRINTZ{<q>}.F32.F32 <Dd>, <Dm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba0580U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VRINTZ{<c>}{<q>}.F64.F64 <Dd>, <Dm> ; T1
    if (dt1.Is(F64) && dt2.Is(F64)) {
      EmitT32_32(0xeeb60bc0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTZ{<q>}.F32.F32 <Dd>, <Dm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba0580U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
    // VRINTZ{<c>}{<q>}.F64.F64 <Dd>, <Dm> ; A1
    if (dt1.Is(F64) && dt2.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb60bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintz, &Assembler::vrintz, cond, dt1, dt2, rd, rm);
}

void Assembler::vrintz(DataType dt1, DataType dt2, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VRINTZ{<q>}.F32.F32 <Qd>, <Qm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xffba05c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTZ{<q>}.F32.F32 <Qd>, <Qm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitA32(0xf3ba05c0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintz, &Assembler::vrintz, dt1, dt2, rd, rm);
}

void Assembler::vrintz(
    Condition cond, DataType dt1, DataType dt2, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRINTZ{<c>}{<q>}.F32.F32 <Sd>, <Sm> ; T1
    if (dt1.Is(F32) && dt2.Is(F32)) {
      EmitT32_32(0xeeb60ac0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VRINTZ{<c>}{<q>}.F32.F32 <Sd>, <Sm> ; A1
    if (dt1.Is(F32) && dt2.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb60ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVrintz, &Assembler::vrintz, cond, dt1, dt2, rd, rm);
}

void Assembler::vrshl(
    Condition cond, DataType dt, DRegister rd, DRegister rm, DRegister rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRSHL{<c>}{<q>}.<dt> {<Dd>}, <Dm>, <Dn> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000500U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRSHL{<c>}{<q>}.<dt> {<Dd>}, <Dm>, <Dn> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000500U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
        return;
      }
    }
  }
  Delegate(kVrshl, &Assembler::vrshl, cond, dt, rd, rm, rn);
}

void Assembler::vrshl(
    Condition cond, DataType dt, QRegister rd, QRegister rm, QRegister rn) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRSHL{<c>}{<q>}.<dt> {<Qd>}, <Qm>, <Qn> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000540U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRSHL{<c>}{<q>}.<dt> {<Qd>}, <Qm>, <Qn> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000540U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
        return;
      }
    }
  }
  Delegate(kVrshl, &Assembler::vrshl, cond, dt, rd, rm, rn);
}

void Assembler::vrshr(Condition cond,
                      DataType dt,
                      DRegister rd,
                      DRegister rm,
                      const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VRSHR{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xef800210U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
        // VRSHR{<c>}{<q>}.<dt> <Dd>, <Dm>, #0 ; T1
        if ((dt.Is(kDataTypeS) || dt.Is(kDataTypeU)) && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xef200110U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                       rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VRSHR{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf2800210U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
        // VRSHR{<c>}{<q>}.<dt> <Dd>, <Dm>, #0 ; A1
        if ((dt.Is(kDataTypeS) || dt.Is(kDataTypeU)) && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf2200110U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                    rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVrshr, &Assembler::vrshr, cond, dt, rd, rm, operand);
}

void Assembler::vrshr(Condition cond,
                      DataType dt,
                      QRegister rd,
                      QRegister rm,
                      const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VRSHR{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xef800250U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
        // VRSHR{<c>}{<q>}.<dt> <Qd>, <Qm>, #0 ; T1
        if ((dt.Is(kDataTypeS) || dt.Is(kDataTypeU)) && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xef200150U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                       rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VRSHR{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf2800250U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
        // VRSHR{<c>}{<q>}.<dt> <Qd>, <Qm>, #0 ; A1
        if ((dt.Is(kDataTypeS) || dt.Is(kDataTypeU)) && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf2200150U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                    rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVrshr, &Assembler::vrshr, cond, dt, rd, rm, operand);
}

void Assembler::vrshrn(Condition cond,
                       DataType dt,
                       DRegister rd,
                       QRegister rm,
                       const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_imm6_3 encoded_dt(dt);
      Dt_size_3 encoded_dt_2(dt);
      if (IsUsingT32()) {
        // VRSHRN{<c>}{<q>}.I<size> <Dd>, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitT32_32(0xef800850U |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
        // VRSHRN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; T1
        if (encoded_dt_2.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb20200U | (encoded_dt_2.GetEncodingValue() << 18) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VRSHRN{<c>}{<q>}.I<size> <Dd>, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitA32(0xf2800850U |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
        // VRSHRN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; A1
        if (encoded_dt_2.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b20200U | (encoded_dt_2.GetEncodingValue() << 18) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVrshrn, &Assembler::vrshrn, cond, dt, rd, rm, operand);
}

void Assembler::vrsqrte(Condition cond,
                        DataType dt,
                        DRegister rd,
                        DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_4 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRSQRTE{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb30480U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRSQRTE{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b30480U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrsqrte, &Assembler::vrsqrte, cond, dt, rd, rm);
}

void Assembler::vrsqrte(Condition cond,
                        DataType dt,
                        QRegister rd,
                        QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_F_size_4 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRSQRTE{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb304c0U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRSQRTE{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b304c0U | ((encoded_dt.GetEncodingValue() & 0x3) << 18) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 6) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrsqrte, &Assembler::vrsqrte, cond, dt, rd, rm);
}

void Assembler::vrsqrts(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRSQRTS{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200f10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRSQRTS{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200f10U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrsqrts, &Assembler::vrsqrts, cond, dt, rd, rn, rm);
}

void Assembler::vrsqrts(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VRSQRTS{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200f50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRSQRTS{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200f50U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrsqrts, &Assembler::vrsqrts, cond, dt, rd, rn, rm);
}

void Assembler::vrsra(Condition cond,
                      DataType dt,
                      DRegister rd,
                      DRegister rm,
                      const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VRSRA{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xef800310U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VRSRA{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf2800310U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVrsra, &Assembler::vrsra, cond, dt, rd, rm, operand);
}

void Assembler::vrsra(Condition cond,
                      DataType dt,
                      QRegister rd,
                      QRegister rm,
                      const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VRSRA{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xef800350U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VRSRA{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf2800350U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVrsra, &Assembler::vrsra, cond, dt, rd, rm, operand);
}

void Assembler::vrsubhn(
    Condition cond, DataType dt, DRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VRSUBHN{<c>}{<q>}.<dt> <Dd>, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid() && (dt.Is(I16) || dt.Is(I32) || dt.Is(I64))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff800600U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VRSUBHN{<c>}{<q>}.<dt> <Dd>, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid() && (dt.Is(I16) || dt.Is(I32) || dt.Is(I64))) {
      if (cond.Is(al)) {
        EmitA32(0xf3800600U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVrsubhn, &Assembler::vrsubhn, cond, dt, rd, rn, rm);
}

void Assembler::vseleq(DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VSELEQ.F64 <Dd>, <Dn>, <Dm> ; T1
    if (OutsideITBlock() && dt.Is(F64)) {
      EmitT32_32(0xfe000b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSELEQ.F64 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F64)) {
      EmitA32(0xfe000b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVseleq, &Assembler::vseleq, dt, rd, rn, rm);
}

void Assembler::vseleq(DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VSELEQ.F32 <Sd>, <Sn>, <Sm> ; T1
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xfe000a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSELEQ.F32 <Sd>, <Sn>, <Sm> ; A1
    if (dt.Is(F32)) {
      EmitA32(0xfe000a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVseleq, &Assembler::vseleq, dt, rd, rn, rm);
}

void Assembler::vselge(DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VSELGE.F64 <Dd>, <Dn>, <Dm> ; T1
    if (OutsideITBlock() && dt.Is(F64)) {
      EmitT32_32(0xfe200b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSELGE.F64 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F64)) {
      EmitA32(0xfe200b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVselge, &Assembler::vselge, dt, rd, rn, rm);
}

void Assembler::vselge(DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VSELGE.F32 <Sd>, <Sn>, <Sm> ; T1
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xfe200a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSELGE.F32 <Sd>, <Sn>, <Sm> ; A1
    if (dt.Is(F32)) {
      EmitA32(0xfe200a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVselge, &Assembler::vselge, dt, rd, rn, rm);
}

void Assembler::vselgt(DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VSELGT.F64 <Dd>, <Dn>, <Dm> ; T1
    if (OutsideITBlock() && dt.Is(F64)) {
      EmitT32_32(0xfe300b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSELGT.F64 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F64)) {
      EmitA32(0xfe300b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVselgt, &Assembler::vselgt, dt, rd, rn, rm);
}

void Assembler::vselgt(DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VSELGT.F32 <Sd>, <Sn>, <Sm> ; T1
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xfe300a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSELGT.F32 <Sd>, <Sn>, <Sm> ; A1
    if (dt.Is(F32)) {
      EmitA32(0xfe300a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVselgt, &Assembler::vselgt, dt, rd, rn, rm);
}

void Assembler::vselvs(DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VSELVS.F64 <Dd>, <Dn>, <Dm> ; T1
    if (OutsideITBlock() && dt.Is(F64)) {
      EmitT32_32(0xfe100b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSELVS.F64 <Dd>, <Dn>, <Dm> ; A1
    if (dt.Is(F64)) {
      EmitA32(0xfe100b00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVselvs, &Assembler::vselvs, dt, rd, rn, rm);
}

void Assembler::vselvs(DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(al);
  if (IsUsingT32()) {
    // VSELVS.F32 <Sd>, <Sn>, <Sm> ; T1
    if (OutsideITBlock() && dt.Is(F32)) {
      EmitT32_32(0xfe100a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSELVS.F32 <Sd>, <Sn>, <Sm> ; A1
    if (dt.Is(F32)) {
      EmitA32(0xfe100a00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVselvs, &Assembler::vselvs, dt, rd, rn, rm);
}

void Assembler::vshl(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_3 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSHL{<c>}{<q>}.I<size> {<Dd>}, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = imm;
            EmitT32_32(0xef800510U |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSHL{<c>}{<q>}.I<size> {<Dd>}, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al)) {
            uint32_t imm6 = imm;
            EmitA32(0xf2800510U |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  if (operand.IsRegister()) {
    DRegister rn = operand.GetRegister();
    Dt_U_size_3 encoded_dt(dt);
    if (IsUsingT32()) {
      // VSHL{<c>}{<q>}.<dt> {<Dd>}, <Dm>, <Dn> ; T1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef000400U |
                     ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                     ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                     rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VSHL{<c>}{<q>}.<dt> {<Dd>}, <Dm>, <Dn> ; A1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al)) {
          EmitA32(0xf2000400U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                  ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                  rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
          return;
        }
      }
    }
  }
  Delegate(kVshl, &Assembler::vshl, cond, dt, rd, rm, operand);
}

void Assembler::vshl(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_3 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSHL{<c>}{<q>}.I<size> {<Qd>}, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = imm;
            EmitT32_32(0xef800550U |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSHL{<c>}{<q>}.I<size> {<Qd>}, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al)) {
            uint32_t imm6 = imm;
            EmitA32(0xf2800550U |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  if (operand.IsRegister()) {
    QRegister rn = operand.GetRegister();
    Dt_U_size_3 encoded_dt(dt);
    if (IsUsingT32()) {
      // VSHL{<c>}{<q>}.<dt> {<Qd>}, <Qm>, <Qn> ; T1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          EmitT32_32(0xef000440U |
                     ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                     ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                     rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VSHL{<c>}{<q>}.<dt> {<Qd>}, <Qm>, <Qn> ; A1
      if (encoded_dt.IsValid()) {
        if (cond.Is(al)) {
          EmitA32(0xf2000440U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                  ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                  rd.Encode(22, 12) | rm.Encode(5, 0) | rn.Encode(7, 16));
          return;
        }
      }
    }
  }
  Delegate(kVshl, &Assembler::vshl, cond, dt, rd, rm, operand);
}

void Assembler::vshll(Condition cond,
                      DataType dt,
                      QRegister rd,
                      DRegister rm,
                      const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_imm6_4 encoded_dt(dt);
      Dt_size_16 encoded_dt_2(dt);
      if (IsUsingT32()) {
        // VSHLL{<c>}{<q>}.<type><size> <Qd>, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() + imm;
            EmitT32_32(0xef800a10U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
        // VSHLL{<c>}{<q>}.<type><size> <Qd>, <Dm>, #<imm> ; T2
        if (encoded_dt_2.IsValid() && (imm == dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb20300U | (encoded_dt_2.GetEncodingValue() << 18) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSHLL{<c>}{<q>}.<type><size> <Qd>, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() + imm;
            EmitA32(0xf2800a10U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
        // VSHLL{<c>}{<q>}.<type><size> <Qd>, <Dm>, #<imm> ; A2
        if (encoded_dt_2.IsValid() && (imm == dt.GetSize())) {
          if (cond.Is(al)) {
            EmitA32(0xf3b20300U | (encoded_dt_2.GetEncodingValue() << 18) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVshll, &Assembler::vshll, cond, dt, rd, rm, operand);
}

void Assembler::vshr(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSHR{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xef800010U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
        // VSHR{<c>}{<q>}.<dt> <Dd>, <Dm>, #0 ; T1
        if ((dt.Is(kDataTypeS) || dt.Is(kDataTypeU)) && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xef200110U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                       rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSHR{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf2800010U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
        // VSHR{<c>}{<q>}.<dt> <Dd>, <Dm>, #0 ; A1
        if ((dt.Is(kDataTypeS) || dt.Is(kDataTypeU)) && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf2200110U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                    rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVshr, &Assembler::vshr, cond, dt, rd, rm, operand);
}

void Assembler::vshr(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSHR{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xef800050U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
        // VSHR{<c>}{<q>}.<dt> <Qd>, <Qm>, #0 ; T1
        if ((dt.Is(kDataTypeS) || dt.Is(kDataTypeU)) && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xef200150U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                       rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSHR{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf2800050U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
        // VSHR{<c>}{<q>}.<dt> <Qd>, <Qm>, #0 ; A1
        if ((dt.Is(kDataTypeS) || dt.Is(kDataTypeU)) && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf2200150U | rd.Encode(22, 12) | rm.Encode(7, 16) |
                    rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVshr, &Assembler::vshr, cond, dt, rd, rm, operand);
}

void Assembler::vshrn(Condition cond,
                      DataType dt,
                      DRegister rd,
                      QRegister rm,
                      const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_imm6_3 encoded_dt(dt);
      Dt_size_3 encoded_dt_2(dt);
      if (IsUsingT32()) {
        // VSHRN{<c>}{<q>}.I<size> <Dd>, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitT32_32(0xef800810U |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
        // VSHRN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; T1
        if (encoded_dt_2.IsValid() && (imm == 0)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            EmitT32_32(0xffb20200U | (encoded_dt_2.GetEncodingValue() << 18) |
                       rd.Encode(22, 12) | rm.Encode(5, 0));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSHRN{<c>}{<q>}.I<size> <Dd>, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize() / 2)) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() / 2 - imm;
            EmitA32(0xf2800810U |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
        // VSHRN{<c>}{<q>}.<dt> <Dd>, <Qm>, #0 ; A1
        if (encoded_dt_2.IsValid() && (imm == 0)) {
          if (cond.Is(al)) {
            EmitA32(0xf3b20200U | (encoded_dt_2.GetEncodingValue() << 18) |
                    rd.Encode(22, 12) | rm.Encode(5, 0));
            return;
          }
        }
      }
    }
  }
  Delegate(kVshrn, &Assembler::vshrn, cond, dt, rd, rm, operand);
}

void Assembler::vsli(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_4 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSLI{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = imm;
            EmitT32_32(0xff800510U |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSLI{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al)) {
            uint32_t imm6 = imm;
            EmitA32(0xf3800510U |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVsli, &Assembler::vsli, cond, dt, rd, rm, operand);
}

void Assembler::vsli(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_4 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSLI{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = imm;
            EmitT32_32(0xff800550U |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSLI{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm <= dt.GetSize() - 1)) {
          if (cond.Is(al)) {
            uint32_t imm6 = imm;
            EmitA32(0xf3800550U |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVsli, &Assembler::vsli, cond, dt, rd, rm, operand);
}

void Assembler::vsqrt(Condition cond, DataType dt, SRegister rd, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VSQRT{<c>}{<q>}.F32 <Sd>, <Sm> ; T1
    if (dt.Is(F32)) {
      EmitT32_32(0xeeb10ac0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSQRT{<c>}{<q>}.F32 <Sd>, <Sm> ; A1
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0eb10ac0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVsqrt, &Assembler::vsqrt, cond, dt, rd, rm);
}

void Assembler::vsqrt(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VSQRT{<c>}{<q>}.F64 <Dd>, <Dm> ; T1
    if (dt.Is(F64)) {
      EmitT32_32(0xeeb10bc0U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSQRT{<c>}{<q>}.F64 <Dd>, <Dm> ; A1
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0eb10bc0U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVsqrt, &Assembler::vsqrt, cond, dt, rd, rm);
}

void Assembler::vsra(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSRA{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xef800110U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSRA{<c>}{<q>}.<type><size> {<Dd>}, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf2800110U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVsra, &Assembler::vsra, cond, dt, rd, rm, operand);
}

void Assembler::vsra(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_1 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSRA{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xef800150U | (encoded_dt.GetTypeEncodingValue() << 28) |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSRA{<c>}{<q>}.<type><size> {<Qd>}, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf2800150U | (encoded_dt.GetTypeEncodingValue() << 24) |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVsra, &Assembler::vsra, cond, dt, rd, rm, operand);
}

void Assembler::vsri(Condition cond,
                     DataType dt,
                     DRegister rd,
                     DRegister rm,
                     const DOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_4 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSRI{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xff800410U |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSRI{<c>}{<q>}.<dt> {<Dd>}, <Dm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf3800410U |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVsri, &Assembler::vsri, cond, dt, rd, rm, operand);
}

void Assembler::vsri(Condition cond,
                     DataType dt,
                     QRegister rd,
                     QRegister rm,
                     const QOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    if (operand.GetNeonImmediate().CanConvert<uint32_t>()) {
      uint32_t imm = operand.GetNeonImmediate().GetImmediate<uint32_t>();
      Dt_L_imm6_4 encoded_dt(dt);
      if (IsUsingT32()) {
        // VSRI{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #<imm> ; T1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al) || AllowStronglyDiscouraged()) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitT32_32(0xff800450U |
                       ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                       ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                       rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            AdvanceIT();
            return;
          }
        }
      } else {
        // VSRI{<c>}{<q>}.<dt> {<Qd>}, <Qm>, #<imm> ; A1
        if (encoded_dt.IsValid() && (imm >= 1) && (imm <= dt.GetSize())) {
          if (cond.Is(al)) {
            uint32_t imm6 = dt.GetSize() - imm;
            EmitA32(0xf3800450U |
                    ((encoded_dt.GetEncodingValue() & 0x7) << 19) |
                    ((encoded_dt.GetEncodingValue() & 0x8) << 4) |
                    rd.Encode(22, 12) | rm.Encode(5, 0) | (imm6 << 16));
            return;
          }
        }
      }
    }
  }
  Delegate(kVsri, &Assembler::vsri, cond, dt, rd, rm, operand);
}

void Assembler::vst1(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const AlignedMemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Dt_size_6 encoded_dt(dt);
    Dt_size_7 encoded_dt_2(dt);
    Align_align_5 encoded_align_1(align, nreglist);
    Align_index_align_1 encoded_align_2(align, nreglist, dt);
    if (IsUsingT32()) {
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitT32_32(0xf900000fU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitT32_32(0xf900000dU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && operand.IsOffset() &&
          encoded_align_2.IsValid() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf980000fU | (encoded_dt_2.GetEncodingValue() << 10) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && operand.IsPostIndex() &&
          encoded_align_2.IsValid() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf980000dU | (encoded_dt_2.GetEncodingValue() << 10) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitA32(0xf400000fU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitA32(0xf400000dU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && operand.IsOffset() &&
          encoded_align_2.IsValid() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf480000fU | (encoded_dt_2.GetEncodingValue() << 10) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && operand.IsPostIndex() &&
          encoded_align_2.IsValid() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf480000dU | (encoded_dt_2.GetEncodingValue() << 10) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Register rm = operand.GetOffsetRegister();
    Dt_size_6 encoded_dt(dt);
    Dt_size_7 encoded_dt_2(dt);
    Align_align_5 encoded_align_1(align, nreglist);
    Align_index_align_1 encoded_align_2(align, nreglist, dt);
    if (IsUsingT32()) {
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitT32_32(0xf9000000U | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && !rm.IsPC() && !rm.IsSP() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9800000U | (encoded_dt_2.GetEncodingValue() << 10) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16) |
                     rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          switch (nreglist.GetLength()) {
            default:
              VIXL_UNREACHABLE_OR_FALLTHROUGH();
            case 1:
              len_encoding = 0x7;
              break;
            case 2:
              len_encoding = 0xa;
              break;
            case 3:
              len_encoding = 0x6;
              break;
            case 4:
              len_encoding = 0x2;
              break;
          }
          EmitA32(0xf4000000U | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VST1{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt_2.IsValid() && nreglist.IsTransferOneLane() &&
          (nreglist.GetLength() == 1) && !rm.IsPC() && !rm.IsSP() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4800000U | (encoded_dt_2.GetEncodingValue() << 10) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVst1, &Assembler::vst1, cond, dt, nreglist, operand);
}

void Assembler::vst2(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const AlignedMemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Dt_size_7 encoded_dt(dt);
    Align_align_2 encoded_align_1(align, nreglist);
    Align_index_align_2 encoded_align_2(align, nreglist, dt);
    if (IsUsingT32()) {
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitT32_32(0xf900000fU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitT32_32(0xf900000dU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf980010fU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf980010dU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitA32(0xf400000fU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitA32(0xf400000dU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf480010fU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf480010dU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Register rm = operand.GetOffsetRegister();
    Dt_size_7 encoded_dt(dt);
    Align_align_2 encoded_align_1(align, nreglist);
    Align_index_align_2 encoded_align_2(align, nreglist, dt);
    if (IsUsingT32()) {
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitT32_32(0xf9000000U | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9800100U | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16) |
                     rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding;
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x8;
          }
          if (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2)) {
            len_encoding = 0x9;
          }
          if (nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) {
            len_encoding = 0x3;
          }
          EmitA32(0xf4000000U | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VST2{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 2)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 2))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4800100U | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVst2, &Assembler::vst2, cond, dt, nreglist, operand);
}

void Assembler::vst3(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const AlignedMemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Dt_size_7 encoded_dt(dt);
    Align_align_3 encoded_align_1(align);
    if (IsUsingT32()) {
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitT32_32(0xf900000fU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitT32_32(0xf900000dU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitA32(0xf400000fU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitA32(0xf400000dU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Register rm = operand.GetOffsetRegister();
    Dt_size_7 encoded_dt(dt);
    Align_align_3 encoded_align_1(align);
    if (IsUsingT32()) {
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitT32_32(0xf9000000U | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x4 : 0x5;
          EmitA32(0xf4000000U | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVst3, &Assembler::vst3, cond, dt, nreglist, operand);
}

void Assembler::vst3(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Dt_size_7 encoded_dt(dt);
    Index_1 encoded_align_1(nreglist, dt);
    if (IsUsingT32()) {
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf980020fU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPreIndex() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf980020dU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsOffset() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf480020fU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          operand.IsPreIndex() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf480020dU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Sign sign = operand.GetSign();
    Register rm = operand.GetOffsetRegister();
    Dt_size_7 encoded_dt(dt);
    Index_1 encoded_align_1(nreglist, dt);
    if (IsUsingT32()) {
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>], #<Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          sign.IsPlus() && operand.IsPostIndex() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9800200U | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16) |
                     rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST3{<c>}{<q>}.<dt> <list>, [<Rn>], #<Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 3)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 3))) &&
          sign.IsPlus() && operand.IsPostIndex() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4800200U | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVst3, &Assembler::vst3, cond, dt, nreglist, operand);
}

void Assembler::vst4(Condition cond,
                     DataType dt,
                     const NeonRegisterList& nreglist,
                     const AlignedMemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediateZero()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Dt_size_7 encoded_dt(dt);
    Align_align_4 encoded_align_1(align);
    Align_index_align_3 encoded_align_2(align, nreglist, dt);
    if (IsUsingT32()) {
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf900000fU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf900000dU | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf980030fU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf980030dU | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16));
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf400000fU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_1.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf400000dU | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16));
          return;
        }
      }
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}] ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsOffset() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf480030fU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}]! ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          operand.IsPostIndex() && encoded_align_2.IsValid() &&
          (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf480030dU | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16));
          return;
        }
      }
    }
  }
  if (operand.IsPlainRegister()) {
    Register rn = operand.GetBaseRegister();
    Alignment align = operand.GetAlignment();
    Register rm = operand.GetOffsetRegister();
    Dt_size_7 encoded_dt(dt);
    Align_align_4 encoded_align_1(align);
    Align_index_align_3 encoded_align_2(align, nreglist, dt);
    if (IsUsingT32()) {
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitT32_32(0xf9000000U | (encoded_dt.GetEncodingValue() << 6) |
                     (encoded_align_1.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (len_encoding << 8) |
                     (rn.GetCode() << 16) | rm.GetCode());
          AdvanceIT();
          return;
        }
      }
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; T1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al) || AllowStronglyDiscouraged()) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitT32_32(0xf9800300U | (encoded_dt.GetEncodingValue() << 10) |
                     (encoded_align_2.GetEncodingValue() << 4) |
                     first.Encode(22, 12) | (rn.GetCode() << 16) |
                     rm.GetCode());
          AdvanceIT();
          return;
        }
      }
    } else {
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferMultipleLanes() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          uint32_t len_encoding = nreglist.IsSingleSpaced() ? 0x0 : 0x1;
          EmitA32(0xf4000000U | (encoded_dt.GetEncodingValue() << 6) |
                  (encoded_align_1.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (len_encoding << 8) |
                  (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
      // VST4{<c>}{<q>}.<dt> <list>, [<Rn>{:<align>}], <Rm> ; A1
      if (encoded_dt.IsValid() && nreglist.IsTransferOneLane() &&
          ((nreglist.IsSingleSpaced() && (nreglist.GetLength() == 4)) ||
           (nreglist.IsDoubleSpaced() && (nreglist.GetLength() == 4))) &&
          !rm.IsPC() && !rm.IsSP() && (!rn.IsPC() || AllowUnpredictable())) {
        if (cond.Is(al)) {
          const DRegister& first = nreglist.GetFirstDRegister();
          EmitA32(0xf4800300U | (encoded_dt.GetEncodingValue() << 10) |
                  (encoded_align_2.GetEncodingValue() << 4) |
                  first.Encode(22, 12) | (rn.GetCode() << 16) | rm.GetCode());
          return;
        }
      }
    }
  }
  Delegate(kVst4, &Assembler::vst4, cond, dt, nreglist, operand);
}

void Assembler::vstm(Condition cond,
                     DataType dt,
                     Register rn,
                     WriteBack write_back,
                     DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VSTM{<c>}{<q>}{.<size>} <Rn>{!}, <dreglist> ; T1
    if (((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xec800b00U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VSTM{<c>}{<q>}{.<size>} <Rn>{!}, <dreglist> ; A1
    if (cond.IsNotNever() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0c800b00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVstm, &Assembler::vstm, cond, dt, rn, write_back, dreglist);
}

void Assembler::vstm(Condition cond,
                     DataType dt,
                     Register rn,
                     WriteBack write_back,
                     SRegisterList sreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VSTM{<c>}{<q>}{.<size>} <Rn>{!}, <sreglist> ; T2
    const SRegister& sreg = sreglist.GetFirstSRegister();
    unsigned len = sreglist.GetLength();
    EmitT32_32(0xec800a00U | (rn.GetCode() << 16) |
               (write_back.GetWriteBackUint32() << 21) | sreg.Encode(22, 12) |
               (len & 0xff));
    AdvanceIT();
    return;
  } else {
    // VSTM{<c>}{<q>}{.<size>} <Rn>{!}, <sreglist> ; A2
    if (cond.IsNotNever()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitA32(0x0c800a00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | sreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVstm, &Assembler::vstm, cond, dt, rn, write_back, sreglist);
}

void Assembler::vstmdb(Condition cond,
                       DataType dt,
                       Register rn,
                       WriteBack write_back,
                       DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VSTMDB{<c>}{<q>}{.<size>} <Rn>!, <dreglist> ; T1
    if (write_back.DoesWriteBack() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xed200b00U | (rn.GetCode() << 16) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VSTMDB{<c>}{<q>}{.<size>} <Rn>!, <dreglist> ; A1
    if (write_back.DoesWriteBack() && cond.IsNotNever() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0d200b00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              dreg.Encode(22, 12) | (len & 0xff));
      return;
    }
  }
  Delegate(kVstmdb, &Assembler::vstmdb, cond, dt, rn, write_back, dreglist);
}

void Assembler::vstmdb(Condition cond,
                       DataType dt,
                       Register rn,
                       WriteBack write_back,
                       SRegisterList sreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VSTMDB{<c>}{<q>}{.<size>} <Rn>!, <sreglist> ; T2
    if (write_back.DoesWriteBack()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitT32_32(0xed200a00U | (rn.GetCode() << 16) | sreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VSTMDB{<c>}{<q>}{.<size>} <Rn>!, <sreglist> ; A2
    if (write_back.DoesWriteBack() && cond.IsNotNever()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitA32(0x0d200a00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              sreg.Encode(22, 12) | (len & 0xff));
      return;
    }
  }
  Delegate(kVstmdb, &Assembler::vstmdb, cond, dt, rn, write_back, sreglist);
}

void Assembler::vstmia(Condition cond,
                       DataType dt,
                       Register rn,
                       WriteBack write_back,
                       DRegisterList dreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VSTMIA{<c>}{<q>}{.<size>} <Rn>{!}, <dreglist> ; T1
    if (((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitT32_32(0xec800b00U | (rn.GetCode() << 16) |
                 (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
                 (len & 0xff));
      AdvanceIT();
      return;
    }
  } else {
    // VSTMIA{<c>}{<q>}{.<size>} <Rn>{!}, <dreglist> ; A1
    if (cond.IsNotNever() &&
        ((dreglist.GetLength() <= 16) || AllowUnpredictable())) {
      const DRegister& dreg = dreglist.GetFirstDRegister();
      unsigned len = dreglist.GetLength() * 2;
      EmitA32(0x0c800b00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | dreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVstmia, &Assembler::vstmia, cond, dt, rn, write_back, dreglist);
}

void Assembler::vstmia(Condition cond,
                       DataType dt,
                       Register rn,
                       WriteBack write_back,
                       SRegisterList sreglist) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VSTMIA{<c>}{<q>}{.<size>} <Rn>{!}, <sreglist> ; T2
    const SRegister& sreg = sreglist.GetFirstSRegister();
    unsigned len = sreglist.GetLength();
    EmitT32_32(0xec800a00U | (rn.GetCode() << 16) |
               (write_back.GetWriteBackUint32() << 21) | sreg.Encode(22, 12) |
               (len & 0xff));
    AdvanceIT();
    return;
  } else {
    // VSTMIA{<c>}{<q>}{.<size>} <Rn>{!}, <sreglist> ; A2
    if (cond.IsNotNever()) {
      const SRegister& sreg = sreglist.GetFirstSRegister();
      unsigned len = sreglist.GetLength();
      EmitA32(0x0c800a00U | (cond.GetCondition() << 28) | (rn.GetCode() << 16) |
              (write_back.GetWriteBackUint32() << 21) | sreg.Encode(22, 12) |
              (len & 0xff));
      return;
    }
  }
  Delegate(kVstmia, &Assembler::vstmia, cond, dt, rn, write_back, sreglist);
}

void Assembler::vstr(Condition cond,
                     DataType dt,
                     DRegister rd,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // VSTR{<c>}{<q>}{.64} <Dd>, [<Rn>{, #{+/-}<imm>}] ; T1
      if (dt.IsNoneOr(Untyped64) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && operand.IsOffset()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xed000b00U | rd.Encode(22, 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // VSTR{<c>}{<q>}{.64} <Dd>, [<Rn>{, #{+/-}<imm>}] ; A1
      if (dt.IsNoneOr(Untyped64) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && operand.IsOffset() && cond.IsNotNever()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitA32(0x0d000b00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                (rn.GetCode() << 16) | offset_ | (sign << 23));
        return;
      }
    }
  }
  Delegate(kVstr, &Assembler::vstr, cond, dt, rd, operand);
}

void Assembler::vstr(Condition cond,
                     DataType dt,
                     SRegister rd,
                     const MemOperand& operand) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (operand.IsImmediate()) {
    Register rn = operand.GetBaseRegister();
    int32_t offset = operand.GetOffsetImmediate();
    if (IsUsingT32()) {
      // VSTR{<c>}{<q>}{.32} <Sd>, [<Rn>{, #{+/-}<imm>}] ; T2
      if (dt.IsNoneOr(Untyped32) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && operand.IsOffset()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitT32_32(0xed000a00U | rd.Encode(22, 12) | (rn.GetCode() << 16) |
                   offset_ | (sign << 23));
        AdvanceIT();
        return;
      }
    } else {
      // VSTR{<c>}{<q>}{.32} <Sd>, [<Rn>{, #{+/-}<imm>}] ; A2
      if (dt.IsNoneOr(Untyped32) && (offset >= -1020) && (offset <= 1020) &&
          ((offset % 4) == 0) && operand.IsOffset() && cond.IsNotNever()) {
        uint32_t sign = operand.GetSign().IsPlus() ? 1 : 0;
        uint32_t offset_ = abs(offset) >> 2;
        EmitA32(0x0d000a00U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
                (rn.GetCode() << 16) | offset_ | (sign << 23));
        return;
      }
    }
  }
  Delegate(kVstr, &Assembler::vstr, cond, dt, rd, operand);
}

void Assembler::vsub(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_2 encoded_dt(dt);
  if (IsUsingT32()) {
    // VSUB{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200d00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VSUB{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; T2
    if (dt.Is(F64)) {
      EmitT32_32(0xee300b40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
    // VSUB{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000800U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VSUB{<c>}{<q>}.F32 {<Dd>}, <Dn>, <Dm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200d00U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VSUB{<c>}{<q>}.F64 {<Dd>}, <Dn>, <Dm> ; A2
    if (dt.Is(F64) && cond.IsNotNever()) {
      EmitA32(0x0e300b40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
    // VSUB{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3000800U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVsub, &Assembler::vsub, cond, dt, rd, rn, rm);
}

void Assembler::vsub(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_2 encoded_dt(dt);
  if (IsUsingT32()) {
    // VSUB{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; T1
    if (dt.Is(F32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef200d40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                   rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VSUB{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xff000840U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VSUB{<c>}{<q>}.F32 {<Qd>}, <Qn>, <Qm> ; A1
    if (dt.Is(F32)) {
      if (cond.Is(al)) {
        EmitA32(0xf2200d40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                rm.Encode(5, 0));
        return;
      }
    }
    // VSUB{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3000840U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVsub, &Assembler::vsub, cond, dt, rd, rn, rm);
}

void Assembler::vsub(
    Condition cond, DataType dt, SRegister rd, SRegister rn, SRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VSUB{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; T2
    if (dt.Is(F32)) {
      EmitT32_32(0xee300a40U | rd.Encode(22, 12) | rn.Encode(7, 16) |
                 rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSUB{<c>}{<q>}.F32 {<Sd>}, <Sn>, <Sm> ; A2
    if (dt.Is(F32) && cond.IsNotNever()) {
      EmitA32(0x0e300a40U | (cond.GetCondition() << 28) | rd.Encode(22, 12) |
              rn.Encode(7, 16) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVsub, &Assembler::vsub, cond, dt, rd, rn, rm);
}

void Assembler::vsubhn(
    Condition cond, DataType dt, DRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_3 encoded_dt(dt);
  if (IsUsingT32()) {
    // VSUBHN{<c>}{<q>}.<dt> <Dd>, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid() && (dt.Is(I16) || dt.Is(I32) || dt.Is(I64))) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800600U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VSUBHN{<c>}{<q>}.<dt> <Dd>, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid() && (dt.Is(I16) || dt.Is(I32) || dt.Is(I64))) {
      if (cond.Is(al)) {
        EmitA32(0xf2800600U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVsubhn, &Assembler::vsubhn, cond, dt, rd, rn, rm);
}

void Assembler::vsubl(
    Condition cond, DataType dt, QRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VSUBL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800200U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VSUBL{<c>}{<q>}.<dt> <Qd>, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800200U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVsubl, &Assembler::vsubl, cond, dt, rd, rn, rm);
}

void Assembler::vsubw(
    Condition cond, DataType dt, QRegister rd, QRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_U_size_1 encoded_dt(dt);
  if (IsUsingT32()) {
    // VSUBW{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef800300U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                   ((encoded_dt.GetEncodingValue() & 0x4) << 26) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VSUBW{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2800300U | ((encoded_dt.GetEncodingValue() & 0x3) << 20) |
                ((encoded_dt.GetEncodingValue() & 0x4) << 22) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVsubw, &Assembler::vsubw, cond, dt, rd, rn, rm);
}

void Assembler::vswp(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VSWP{<c>}{<q>}{.<dt>} <Dd>, <Dm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xffb20000U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSWP{<c>}{<q>}{.<dt>} <Dd>, <Dm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3b20000U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVswp, &Assembler::vswp, cond, dt, rd, rm);
}

void Assembler::vswp(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  USE(dt);
  if (IsUsingT32()) {
    // VSWP{<c>}{<q>}{.<dt>} <Qd>, <Qm> ; T1
    if (cond.Is(al) || AllowStronglyDiscouraged()) {
      EmitT32_32(0xffb20040U | rd.Encode(22, 12) | rm.Encode(5, 0));
      AdvanceIT();
      return;
    }
  } else {
    // VSWP{<c>}{<q>}{.<dt>} <Qd>, <Qm> ; A1
    if (cond.Is(al)) {
      EmitA32(0xf3b20040U | rd.Encode(22, 12) | rm.Encode(5, 0));
      return;
    }
  }
  Delegate(kVswp, &Assembler::vswp, cond, dt, rd, rm);
}

void Assembler::vtbl(Condition cond,
                     DataType dt,
                     DRegister rd,
                     const NeonRegisterList& nreglist,
                     DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VTBL{<c>}{<q>}.8 <Dd>, <list>, <Dm> ; T1
    if (dt.Is(Untyped8) && nreglist.IsTransferMultipleLanes() &&
        (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        const DRegister& first = nreglist.GetFirstDRegister();
        uint32_t len_encoding = nreglist.GetLength() - 1;
        EmitT32_32(0xffb00800U | rd.Encode(22, 12) | first.Encode(7, 16) |
                   (len_encoding << 8) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VTBL{<c>}{<q>}.8 <Dd>, <list>, <Dm> ; A1
    if (dt.Is(Untyped8) && nreglist.IsTransferMultipleLanes() &&
        (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4)) {
      if (cond.Is(al)) {
        const DRegister& first = nreglist.GetFirstDRegister();
        uint32_t len_encoding = nreglist.GetLength() - 1;
        EmitA32(0xf3b00800U | rd.Encode(22, 12) | first.Encode(7, 16) |
                (len_encoding << 8) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVtbl, &Assembler::vtbl, cond, dt, rd, nreglist, rm);
}

void Assembler::vtbx(Condition cond,
                     DataType dt,
                     DRegister rd,
                     const NeonRegisterList& nreglist,
                     DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // VTBX{<c>}{<q>}.8 <Dd>, <list>, <Dm> ; T1
    if (dt.Is(Untyped8) && nreglist.IsTransferMultipleLanes() &&
        (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        const DRegister& first = nreglist.GetFirstDRegister();
        uint32_t len_encoding = nreglist.GetLength() - 1;
        EmitT32_32(0xffb00840U | rd.Encode(22, 12) | first.Encode(7, 16) |
                   (len_encoding << 8) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VTBX{<c>}{<q>}.8 <Dd>, <list>, <Dm> ; A1
    if (dt.Is(Untyped8) && nreglist.IsTransferMultipleLanes() &&
        (nreglist.IsSingleSpaced()) && (nreglist.GetLength() <= 4)) {
      if (cond.Is(al)) {
        const DRegister& first = nreglist.GetFirstDRegister();
        uint32_t len_encoding = nreglist.GetLength() - 1;
        EmitA32(0xf3b00840U | rd.Encode(22, 12) | first.Encode(7, 16) |
                (len_encoding << 8) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVtbx, &Assembler::vtbx, cond, dt, rd, nreglist, rm);
}

void Assembler::vtrn(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_7 encoded_dt(dt);
  if (IsUsingT32()) {
    // VTRN{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb20080U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VTRN{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b20080U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVtrn, &Assembler::vtrn, cond, dt, rd, rm);
}

void Assembler::vtrn(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_7 encoded_dt(dt);
  if (IsUsingT32()) {
    // VTRN{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb200c0U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VTRN{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b200c0U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVtrn, &Assembler::vtrn, cond, dt, rd, rm);
}

void Assembler::vtst(
    Condition cond, DataType dt, DRegister rd, DRegister rn, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_7 encoded_dt(dt);
  if (IsUsingT32()) {
    // VTST{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000810U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VTST{<c>}{<q>}.<dt> {<Dd>}, <Dn>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000810U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVtst, &Assembler::vtst, cond, dt, rd, rn, rm);
}

void Assembler::vtst(
    Condition cond, DataType dt, QRegister rd, QRegister rn, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_7 encoded_dt(dt);
  if (IsUsingT32()) {
    // VTST{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xef000850U | (encoded_dt.GetEncodingValue() << 20) |
                   rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VTST{<c>}{<q>}.<dt> {<Qd>}, <Qn>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf2000850U | (encoded_dt.GetEncodingValue() << 20) |
                rd.Encode(22, 12) | rn.Encode(7, 16) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVtst, &Assembler::vtst, cond, dt, rd, rn, rm);
}

void Assembler::vuzp(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_15 encoded_dt(dt);
  if (IsUsingT32()) {
    // VUZP{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb20100U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VUZP{<c>}{<q>}.32 <Dd>, <Dm> ; T1
    if (dt.Is(Untyped32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffba0080U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VUZP{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b20100U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
    // VUZP{<c>}{<q>}.32 <Dd>, <Dm> ; A1
    if (dt.Is(Untyped32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3ba0080U | rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVuzp, &Assembler::vuzp, cond, dt, rd, rm);
}

void Assembler::vuzp(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_7 encoded_dt(dt);
  if (IsUsingT32()) {
    // VUZP{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb20140U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VUZP{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b20140U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVuzp, &Assembler::vuzp, cond, dt, rd, rm);
}

void Assembler::vzip(Condition cond, DataType dt, DRegister rd, DRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_15 encoded_dt(dt);
  if (IsUsingT32()) {
    // VZIP{<c>}{<q>}.<dt> <Dd>, <Dm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb20180U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
    // VZIP{<c>}{<q>}.32 <Dd>, <Dm> ; T1
    if (dt.Is(Untyped32)) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffba0080U | rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VZIP{<c>}{<q>}.<dt> <Dd>, <Dm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b20180U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
    // VZIP{<c>}{<q>}.32 <Dd>, <Dm> ; A1
    if (dt.Is(Untyped32)) {
      if (cond.Is(al)) {
        EmitA32(0xf3ba0080U | rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVzip, &Assembler::vzip, cond, dt, rd, rm);
}

void Assembler::vzip(Condition cond, DataType dt, QRegister rd, QRegister rm) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  Dt_size_7 encoded_dt(dt);
  if (IsUsingT32()) {
    // VZIP{<c>}{<q>}.<dt> <Qd>, <Qm> ; T1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al) || AllowStronglyDiscouraged()) {
        EmitT32_32(0xffb201c0U | (encoded_dt.GetEncodingValue() << 18) |
                   rd.Encode(22, 12) | rm.Encode(5, 0));
        AdvanceIT();
        return;
      }
    }
  } else {
    // VZIP{<c>}{<q>}.<dt> <Qd>, <Qm> ; A1
    if (encoded_dt.IsValid()) {
      if (cond.Is(al)) {
        EmitA32(0xf3b201c0U | (encoded_dt.GetEncodingValue() << 18) |
                rd.Encode(22, 12) | rm.Encode(5, 0));
        return;
      }
    }
  }
  Delegate(kVzip, &Assembler::vzip, cond, dt, rd, rm);
}

void Assembler::yield(Condition cond, EncodingSize size) {
  VIXL_ASSERT(AllowAssembler());
  CheckIT(cond);
  if (IsUsingT32()) {
    // YIELD{<c>}{<q>} ; T1
    if (!size.IsWide()) {
      EmitT32_16(0xbf10);
      AdvanceIT();
      return;
    }
    // YIELD{<c>}.W ; T2
    if (!size.IsNarrow()) {
      EmitT32_32(0xf3af8001U);
      AdvanceIT();
      return;
    }
  } else {
    // YIELD{<c>}{<q>} ; A1
    if (cond.IsNotNever()) {
      EmitA32(0x0320f001U | (cond.GetCondition() << 28));
      return;
    }
  }
  Delegate(kYield, &Assembler::yield, cond, size);
}
// End of generated code.

}  // namespace aarch32
}  // namespace vixl
