import re
import os


OPCODE_PATTERN = r"void \s*nu_op_(\w+)\s*\(nu_vm\s*\*\w+\);"


def main() -> None:
    ops: list[str] = []

    path = os.path.dirname(__file__)
    with open(f'{path}/../nu/nu.h', 'r', encoding='utf8') as file:
        content = file.read()
        matches = re.findall(OPCODE_PATTERN, content)

        for match in matches:
            ops += [match]

    print("#ifndef __NU_VM_OPS_INL__")
    print("#define __NU_VM_OPS_INL__")
    print()
    print("#include \"nu.h\"")
    print()
    i = 0
    for o in ops:
        print(f'#define NU_OP_{o.upper()} {i}')
        i += 1
    print(f'#define NU_OPS_COUNT {i}')
    print()
    nu_vm_op_name = '\n{\n\t' + \
        ',\n\t'.join([f'[NU_OP_{o.upper()}] = "{o}"' for o in ops]) + \
        '\n}'
    print(f"const static str_t nu_op_name[] = {nu_vm_op_name};")
    print()
    nu_vm_op_ptr = '\n{\n\t' + \
        ',\n\t'.join([f'[NU_OP_{o.upper()}] = &nu_op_{o}' for o in ops]) + \
        '\n}'
    print(f"const static nu_op_fptr nu_op_ptr[] = {nu_vm_op_ptr};")
    print()
    print("#endif")


if __name__ == "__main__":
    main()
