import pytest
import find_diff as fd
import subprocess
from subprocess import TimeoutExpired
import os

# Paths to emulator executables
executable_paths = [
    "../build/bin/Release/emulator.exe",
    "../cmake-build-debug/bin/emulator.exe"
]


# Function to find the valid executable path
def find_executable_path(paths):
    for path in paths:
        if os.path.isfile(path):
            try:
                subprocess.run([path], capture_output=True, timeout=1)
                return path
            except Exception as e:
                print(f"Error checking executable at {path}: {e}")
    raise FileNotFoundError("No valid executable path found.")


# Get the executable path or exit tests
try:
    executable_path = find_executable_path(executable_paths)
except FileNotFoundError as e:
    pytest.exit(f"Error: {e}")

exec_time_out = 60  # 60 seconds timeout for subprocesses


def run_test(rom_path, test_name, test_type="blargg", debug_mode="1", dump_ram="0", compare_logs=False,
             reference_log=None):
    log_file = f"logs/{test_name}.log"
    os.makedirs("logs", exist_ok=True)

    try:
        # Run the emulator
        result = subprocess.run(
            [executable_path, rom_path, test_type, debug_mode, dump_ram],
            capture_output=True,
            timeout=exec_time_out
        )
        stdout = result.stdout.decode("utf-8", errors="ignore")
    except TimeoutExpired as e:
        stdout = e.stdout.decode("utf-8", errors="ignore") if e.stdout else ""
        stderr = e.stderr.decode("utf-8", errors="ignore") if e.stderr else ""
        stdout += f"\nTimeoutExpired: {e}\n" + stderr

    # Write log regardless of success or timeout
    with open(log_file, "w") as log:
        log.write(stdout)

    # Check for test-specific results
    if "TimeoutExpired" in stdout:
        return False, f"Test {test_name} timed out. See log at {log_file}."
    if compare_logs and reference_log:
        comparison_result = fd.compare_logs(reference_log, log_file)
        if comparison_result != (0, 0):
            return False, f"Log mismatch at nestest line {comparison_result[0]} and emulator line {comparison_result[1]}. See log at {log_file}."
    elif "Test result: Passed" not in stdout:
        return False, f"Test failed: {test_name}. See log at {log_file}."

    return True, None


# Example of a basic test
def test_check():
    assert 1 + 1 == 2


# Nestest-specific test
def test_nestest():
    rom_path = "nestest/nestest.nes"
    reference_log = "nestest/nestest_log.txt"
    passed, message = run_test(rom_path, "nestest", test_type="nestest", compare_logs=True, reference_log=reference_log)
    assert passed, message


# Parameterized tests for other ROMs
@pytest.mark.parametrize("rom_file, test_name", [
    ("instr_test-v5/rom_singles/01-basics.nes", "01_basics"),
    ("instr_test-v5/rom_singles/02-implied.nes", "02_implied"),
    ("instr_test-v5/rom_singles/03-immediate.nes", "03_immediate"),
    ("instr_test-v5/rom_singles/04-zero_page.nes", "04_zero_page"),
    ("instr_test-v5/rom_singles/05-zp_xy.nes", "05_zp_xy"),
    ("instr_test-v5/rom_singles/06-absolute.nes", "06_absolute"),
    ("instr_test-v5/rom_singles/07-abs_xy.nes", "07_abs_xy"),
    ("instr_test-v5/rom_singles/08-ind_x.nes", "08_indirect_x"),
    ("instr_test-v5/rom_singles/09-ind_y.nes", "09_indirect_y"),
    ("instr_test-v5/rom_singles/10-branches.nes", "10_branches"),
    ("instr_test-v5/rom_singles/11-stack.nes", "11_stack"),
    ("instr_test-v5/rom_singles/12-jmp_jsr.nes", "12_jmp_jsr"),
    ("instr_test-v5/rom_singles/13-rts.nes", "13_rti_rts"),
    ("instr_test-v5/rom_singles/14-rti.nes", "14_rti"),
    ("instr_test-v5/rom_singles/15-brk.nes", "15_brk"),
    ("instr_test-v5/rom_singles/16-special.nes", "16_special")
])
def test_rom(rom_file, test_name):
    passed, message = run_test(rom_file, test_name)
    assert passed, message
