import pandas as pd
import re

# Load the opcode and cycle ranges table
table_path = 'opcodes_with_cycle_ranges.csv'
opcodes_df = pd.read_csv(table_path)

# Function to parse log file and check against cycle ranges
def parse_log_with_tolerance(log_path):
    more_cycles = []  # Discrepancies where logged cycles > max_cycles
    less_cycles = []  # Discrepancies where logged cycles < min_cycles
    total_instructions = 0

    with open(log_path, 'r') as file:
        for line in file:
            match = re.search(r'0x([0-9A-Fa-f]{1,2}):\s*(\d+)', line)
            if match:
                opcode = f"0x{match.group(1).zfill(2)}"  # Ensure opcode is always two digits (e.g., 0x09)
                logged_cycles = int(match.group(2))
                total_instructions += 1

                # Find the expected cycles for this opcode
                expected_row = opcodes_df.loc[opcodes_df['opcode'] == opcode]

                if not expected_row.empty:
                    min_cycles = expected_row['min_cycles'].iloc[0]
                    max_cycles = expected_row['max_cycles'].iloc[0]

                    # Compare logged cycles to the range
                    if logged_cycles > max_cycles: # Modified to make branches be equal to the minimum cycles
                        more_cycles.append({
                            'opcode': opcode,
                            'min_cycles': min_cycles,
                            'max_cycles': max_cycles,
                            'logged_cycles': logged_cycles
                        })
                    elif logged_cycles < min_cycles:
                        less_cycles.append({
                            'opcode': opcode,
                            'min_cycles': min_cycles,
                            'max_cycles': max_cycles,
                            'logged_cycles': logged_cycles})
            else:
                pass

    return total_instructions - (len(more_cycles) + len(less_cycles)), total_instructions