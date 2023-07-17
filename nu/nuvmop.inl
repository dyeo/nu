#ifndef __NU_VM_ILOP_INL__
#define __NU_VM_ILOP_INL__

#define NU_VM_OP_0(OP, NAME)                \
    void nu_vm_##NAME(nu_vm *);             \
    const static uint8_t nu_op_##NAME = OP; \
    const static uint8_t nu_op_##NAME##_len = 0

#define NU_VM_OP_1(OP, NAME, ARG0)               \
    void nu_vm_##NAME(nu_vm *, ARG0);            \
    const static uint8_t nu_op_##NAME = OP;      \
    const static uint8_t nu_op_##NAME##_len = 1; \
    const static uint8_t nu_op_##NAME##_sze[] = {sizeof(ARG0)}

#define NU_VM_OP_2(OP, NAME, ARG0, ARG1)         \
    void nu_vm_##NAME(nu_vm *, ARG0, ARG1);      \
    const static uint8_t nu_op_##NAME = OP;      \
    const static uint8_t nu_op_##NAME##_len = 2; \
    const static uint8_t nu_op_##NAME##_sze[] = {sizeof(ARG0), sizeof(ARG1)}

#define NU_VM_OP_3(OP, NAME, ARG0, ARG1, ARG2)    \
    void nu_vm_##NAME(nu_vm *, ARG0, ARG1, ARG2); \
    const static uint8_t nu_op_##NAME = OP;       \
    const static uint8_t nu_op_##NAME##_len = 3;  \
    const static uint8_t nu_op_##NAME##_sze[] = {sizeof(ARG0), sizeof(ARG1), sizeof(ARG2)}

#define NU_VM_OP_4(OP, NAME, ARG0, ARG1, ARG2, ARG3)    \
    void nu_vm_##NAME(nu_vm *, ARG0, ARG1, ARG2, ARG3); \
    const static uint8_t nu_op_##NAME = OP;             \
    const static uint8_t nu_op_##NAME##_len = 4;        \
    const static uint8_t nu_op_##NAME##_sze[] = {sizeof(ARG0), sizeof(ARG1), sizeof(ARG2), sizeof(ARG3)}

#define NU_VM_OP_5(OP, NAME, ARG0, ARG1, ARG2, ARG3, ARG4)    \
    void nu_vm_##NAME(nu_vm *, ARG0, ARG1, ARG2, ARG3, ARG4); \
    const static uint8_t nu_op_##NAME = OP;                   \
    const static uint8_t nu_op_##NAME##_len = 5;              \
    const static uint8_t nu_op_##NAME##_sze[] = {sizeof(ARG0), sizeof(ARG1), sizeof(ARG2), sizeof(ARG3), sizeof(ARG4)}

#define NU_VM_OP_6(OP, NAME, ARG0, ARG1, ARG2, ARG3, ARG4, ARG5)    \
    void nu_vm_##NAME(nu_vm *, ARG0, ARG1, ARG2, ARG3, ARG4, ARG5); \
    const static uint8_t nu_op_##NAME = OP;                         \
    const static uint8_t nu_op_##NAME##_len = 6;                    \
    const static uint8_t nu_op_##NAME##_sze[] = {sizeof(ARG0), sizeof(ARG1), sizeof(ARG2), sizeof(ARG3), sizeof(ARG4), sizeof(ARG5)}

#endif
