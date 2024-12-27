import re


def sanitize_nestest_log(log):
    pattern = r"([A-F0-9]{4}) +([A-F0-9 ]{5,8}).*A:([0-9A-F]{2}) X:([0-9A-F]{2}) Y:([0-9A-F]{2}) P:([0-9A-F]{2}) SP:([0-9A-F]{2})"

    output = []

    for line in log:
        match = re.match(pattern, line)
        if match:
            address, codes, a, x, y, p, sp = match.groups()
            output.append(f"{address} {codes.strip()} A:{a} X:{x} Y:{y} P:{p} SP:{sp}")
        else:
            output.append("No match found for line: " + line)
    return output


def is_valid_line(line):
    if len(line) == 0:
        return False
    if "INITIAL" in line:
        return False
    if "PC REGISTER" in line:
        return False
    if "[info]" in line:
        return False
    if "SETTING" in line:
        return False
    if "[critical]" in line:
        return False
    return True


def compare_lines(line1, line2):
    nestest_tokens = line1.split()
    nesemu_tokens = line2.split()[4:]  # Remove the first 4 tokens
    emu_token = 0
    nestest_token = 0
    while emu_token < len(nesemu_tokens):
        token1 = nestest_tokens[nestest_token].replace("$", "")
        if len(nestest_tokens) == 9 and nestest_token == 2:
            token1 = nestest_tokens[nestest_token + 1].replace("$", "") + token1
            nestest_token += 1
        token2 = nesemu_tokens[emu_token]
        if "A:" in token1:
            if "A:" not in token2:
                emu_token += 1
                token2 = nesemu_tokens[emu_token]

        # Strips extra characters and converts into hex to compare
        val1 = int(token1.replace("A:", "").replace("X:", "").replace("Y:", "").replace("SP:", "").replace("P:", ""),
                   16)
        val2 = int(token2.replace("A:", "").replace("X:", "").replace("Y:", "").replace("SP:", "").replace("SR:", ""),
                   16)
        if val1 != val2:
            if "P:" in token1:
                bits = val1 ^ val2
                if bits == 0x20 or bits == 0x10 or bits == 0x30:
                    emu_token += 1
                    nestest_token += 1
                    continue
            print("\nnestest:")
            print(line1)
            print(nestest_tokens)
            print(token1)
            print(val1)
            print("\nnesemu:")
            print(nesemu_tokens)
            print(token2)
            print(val2)
            return False
        emu_token += 1
        nestest_token += 1
    return True


def compare_logs(nestest_log, nesemu_log):
    with open(nestest_log, 'r') as f1, open(nesemu_log, 'r') as f2:
        nestest_lines = f1.readlines()
        nestest_lines = sanitize_nestest_log(nestest_lines)
        nesemu_lines = f2.readlines()
        emu_line = 0
        nestest_line = 0
        while nestest_line < len(nestest_lines):
            if emu_line >= len(nesemu_lines):
                return nestest_line + 1, emu_line + 1
            if is_valid_line(nesemu_lines[emu_line]):
                nestest_curr_line = nestest_lines[nestest_line]
                nesemu_curr_line = nesemu_lines[emu_line]
                if not compare_lines(nestest_curr_line, nesemu_curr_line):
                    return nestest_line + 1, emu_line + 1
                nestest_line += 1
                emu_line += 1
            else:
                emu_line += 1
        return 0, 0
