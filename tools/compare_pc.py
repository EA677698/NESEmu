import re
import santize_log as sl



def parse_mesen_line(line):
    match = re.match(r"([0-9A-F]{4})\s+.*?\s+BC:([0-9A-F]{2})", line)
    if match:
        return {
            'PC': int(match.group(1), 16),
            'Opcode': int(match.group(2), 16),
        }
    return None

def parse_emulator_line(line):
    # Skip irrelevant lines
    if any(skip in line for skip in ["PC REGISTER", "INITIAL OPCODE", "Status:"]):
        return None

    match = re.match(r".*0x([0-9A-F]{4})\s+0x([0-9A-F]{2})", line)
    if match:
        return {
            'PC': int(match.group(1), 16),
            'Opcode': int(match.group(2), 16),
        }
    return None

def compare_logs(mesen_log, emulator_log):
    with open(mesen_log, 'r') as mesen_file, open(emulator_log, 'r') as emulator_file:
        mesen_lines = mesen_file.readlines()
        emulator_lines = [
            line for line in emulator_file
            if "[debug]" in line and not any(skip in line for skip in ["PC REGISTER", "INITIAL OPCODE", "Status:"])
        ]

        for line_number, (mesen_line, emulator_line) in enumerate(zip(mesen_lines, emulator_lines), start=1):
            mesen_data = parse_mesen_line(mesen_line)
            emulator_data = parse_emulator_line(emulator_line)

            if mesen_data and emulator_data:
                if mesen_data != emulator_data:
                    print(f"Discrepancy found at line {line_number}:")
                    print(f"Mesen: PC=0x{mesen_data['PC']:04X}, Opcode=0x{mesen_data['Opcode']:02X}")
                    print(f"Emulator: PC=0x{emulator_data['PC']:04X}, Opcode=0x{emulator_data['Opcode']:02X}")
                    return
            elif mesen_data or emulator_data:
                print(f"Discrepancy found at line {line_number} (one log has data, the other does not):")
                print(f"Mesen: {mesen_data if mesen_data else 'No data'}")
                print(f"Emulator: {emulator_data if emulator_data else 'No data'}")
                return

        print("No discrepancies found.")

sl.sanitize_emulator_log("latestLog.txt", "processed.txt")
compare_logs('dk.txt', 'processed.txt')
