import pytest
import opcodeCompare as oc
import find_diff as fd
import subprocess
from subprocess import TimeoutExpired
import os

# Paths to emulator executables
executable_paths = [
    "../build/bin/Release/RemiNESent.exe",
    "../build/bin/Debug/RemiNESent.exe",
    "../cmake-build-debug/bin/RemiNESent.exe"
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

exec_time_out = 120  # 2 minutes timeout for subprocesses


def run_test(test_name, args=None, compare_logs=False,
             reference_log=None):
    if args is None:
        args = []
    log_file = f"logs/{test_name}.log"
    os.makedirs("logs", exist_ok=True)

    try:
        # Run the emulator
        cmd = [executable_path] + args
        result = subprocess.run(
            cmd,
            capture_output=True,
            timeout=exec_time_out
        )
        stdout = result.stdout
    except TimeoutExpired as e:
        stdout = e.stdout if e.stdout else b""
        stderr = e.stderr if e.stderr else b""
        stdout += f"\nTimeoutExpired: {e}\n".encode() + stderr

    # Write log regardless of success or timeout
    with open(log_file, "wb") as log:
        log.write(stdout)

    # Check for test-specific results
    stdout_text = stdout.decode("utf-8", errors="ignore")
    if "TimeoutExpired" in stdout_text:
        return False, f"Test {test_name} timed out. See log at {log_file}."
    if compare_logs and reference_log:
        comparison_result = fd.compare_logs(reference_log, log_file)
        if comparison_result != (0, 0):
            return False, f"Log mismatch at nestest line {comparison_result[0]} and emulator line {comparison_result[1]}. See log at {log_file}."
    elif "Passed" not in stdout_text:
        return False, f"Test failed: {test_name}. See log at {log_file}."

    return True, None


def test_cpu_cycle_accuracy():
    run_test("cpu_cycle_accuracy", ["--instruction_cycles","--disable_ppu"])
    correct, total = oc.parse_log_with_tolerance("logs/cpu_cycle_accuracy.log")
    message = f"\nCycle correctness: {correct}/{total}"
    print(message)
    assert correct == total, message

# Nestest-specific test
def test_nestest():
    rom_path = "test_roms/other/nestest.nes"
    reference_log = "test_roms/other/nestest.log"
    passed, message = run_test("nestest", ["--rom", f"{rom_path}", "--disable_ppu", "--debug", "--test", "nestest"], compare_logs=True, reference_log=reference_log)
    assert passed, message

# Parameterized tests for other ROMs
@pytest.mark.parametrize("rom_file, test_name", [
    ("test_roms/instr_test-v5/rom_singles/01-basics.nes", "01_basics"),
    ("test_roms/instr_test-v5/rom_singles/02-implied.nes", "02_implied"),
    ("test_roms/instr_test-v5/rom_singles/03-immediate.nes", "03_immediate"),
    ("test_roms/instr_test-v5/rom_singles/04-zero_page.nes", "04_zero_page"),
    ("test_roms/instr_test-v5/rom_singles/05-zp_xy.nes", "05_zp_xy"),
    ("test_roms/instr_test-v5/rom_singles/06-absolute.nes", "06_absolute"),
    ("test_roms/instr_test-v5/rom_singles/07-abs_xy.nes", "07_abs_xy"),
    ("test_roms/instr_test-v5/rom_singles/08-ind_x.nes", "08_indirect_x"),
    ("test_roms/instr_test-v5/rom_singles/09-ind_y.nes", "09_indirect_y"),
    ("test_roms/instr_test-v5/rom_singles/10-branches.nes", "10_branches"),
    ("test_roms/instr_test-v5/rom_singles/11-stack.nes", "11_stack"),
    ("test_roms/instr_test-v5/rom_singles/12-jmp_jsr.nes", "12_jmp_jsr"),
    ("test_roms/instr_test-v5/rom_singles/13-rts.nes", "13_rti_rts"),
    ("test_roms/instr_test-v5/rom_singles/14-rti.nes", "14_rti"),
    ("test_roms/instr_test-v5/rom_singles/15-brk.nes", "15_brk"),
    ("test_roms/instr_test-v5/rom_singles/16-special.nes", "16_special")
])
def test_rom(rom_file, test_name):
    passed, message = run_test(test_name, ["--rom", f"{rom_file}", "--test", "blargg", "--disable_ppu"])
    assert passed, message
