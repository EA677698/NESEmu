import pandas as pd
import re

# Load the opcode and cycles table
table_path = 'opcodes_cycles.csv'  # Update with the correct table file path
opcodes_df = pd.read_csv(table_path)

# Function to parse log file
def parse_log(log_path):
    more_cycles = []  # Discrepancies where logged cycles > expected
    less_cycles = []  # Discrepancies where logged cycles < expected
    total_instructions = 0

    with open(log_path, 'r') as file:
        for _ in range(2): # Skip the first 2 lines
            next(file)
        for line in file:
            match = re.search(r'0x([0-9A-Fa-f]{2}):\s*(\d+)', line)
            if match:
                opcode = f"0x{match.group(1)}"
                cycles = int(match.group(2))
                total_instructions += 1

                # Check if opcode exists in the table
                expected = opcodes_df.loc[opcodes_df['opcode'] == opcode, 'cycles']

                if not expected.empty:
                    expected_cycles = int(expected.iloc[0])
                    # Categorize discrepancy
                    if cycles > expected_cycles:
                        more_cycles.append({
                            'opcode': opcode,
                            'expected_cycles': expected_cycles,
                            'logged_cycles': cycles
                        })
                    elif cycles < expected_cycles:
                        less_cycles.append({
                            'opcode': opcode,
                            'expected_cycles': expected_cycles,
                            'logged_cycles': cycles
                        })
                else:
                    # Skip unofficial opcodes
                    pass
                    #print(f"Skipping unofficial opcode: {opcode}")

    return more_cycles, less_cycles, total_instructions


def run_comparison(log_path, print_results=True):
    more_cycles, less_cycles, total_instructions = parse_log(log_path)

    # Print discrepancies
    if print_results:
        if more_cycles or less_cycles:
            print("\nDiscrepancies found:")

            if more_cycles:
                print("\nMore Cycles:")
                for item in more_cycles:
                    print(f"Opcode {item['opcode']}: Expected {item['expected_cycles']} cycles, Logged {item['logged_cycles']} cycles")

            if less_cycles:
                print("\nLess Cycles:")
                for item in less_cycles:
                    print(f"Opcode {item['opcode']}: Expected {item['expected_cycles']} cycles, Logged {item['logged_cycles']} cycles")
        else:
            print("\nNo discrepancies found.")

        # Print summary
        print(f"\nSummary:")
        print(f"Cycle correctness: {total_instructions - (len(more_cycles) + len(less_cycles))} / {total_instructions}")
    return total_instructions - (len(more_cycles) + len(less_cycles)), total_instructions
