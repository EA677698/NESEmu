import pytest
import find_diff as fd
import subprocess
from subprocess import TimeoutExpired
import os

executable_paths = [
    "../build/bin/Release/emulator.exe",
    "../cmake-build-debug/bin/emulator.exe"
]

# Function to find the valid executable path
def find_executable_path(paths):
    for path in paths:
        if os.path.isfile(path):  # Check if the path exists and is a file
            try:
                # Attempt to run the executable with a simple command or just check if it's accessible
                subprocess.run([path], capture_output=True, timeout=1)
                return path
            except Exception as e:
                # Log or print the exception if needed
                print(f"Attempted path {path} but encountered an error: {e}")
                continue
    raise FileNotFoundError("No valid executable path found among provided options.")

# Attempt to find the valid executable path
try:
    executable_path = find_executable_path(executable_paths)
except FileNotFoundError as e:
    print(e)
    pytest.exit("Terminating tests due to lack of valid emulator executable path.")

exec_time_out = 600  # 10 minutes


def test_check():
    assert 1 + 1 == 2
    print("Basic check succeeded, 1 + 1 == 2")


def test_nestest():
    rom_path = "nestest/nestest.nes"
    rom_log = "nestest/nestest_log.txt"
    NESEmu_log = "latestLog.txt"
    try:
        subprocess.run([executable_path, rom_path, 'nestest', '1', '1'],
                       capture_output=True, text=False, timeout=exec_time_out)
    except TimeoutExpired:
        print("Program timed out")
    result = fd.compare_logs(rom_log, NESEmu_log)
    assert result == (0, 0), f"Mismatch at line {result[0]} in nestest log and {result[1]} in NESEmu log"

def test_01_basics():
    rom_path = "instr_test-v5/rom_singles/01-basics.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                       capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_02_implied():
    rom_path = "instr_test-v5/rom_singles/02-implied.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_03_immediate():
    rom_path = "instr_test-v5/rom_singles/03-immediate.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_04_zero_page():
    rom_path = "instr_test-v5/rom_singles/04-zero_page.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_05_zp_xy():
    rom_path = "instr_test-v5/rom_singles/05-zp_xy.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_06_absolute():
    rom_path = "instr_test-v5/rom_singles/06-absolute.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_07_abs_xy():
    rom_path = "instr_test-v5/rom_singles/07-abs_xy.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_08_indirect_x():
    rom_path = "instr_test-v5/rom_singles/08-ind_x.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_09_indirect_y():
    rom_path = "instr_test-v5/rom_singles/09-ind_y.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_10_branches():
    rom_path = "instr_test-v5/rom_singles/10-branches.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_11_stack():
    rom_path = "instr_test-v5/rom_singles/11-stack.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_12_jmp_jsr():
    rom_path = "instr_test-v5/rom_singles/12-jmp_jsr.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_13_rti_rts():
    rom_path = "instr_test-v5/rom_singles/13-rts.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_14_rti():
    rom_path = "instr_test-v5/rom_singles/14-rti.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_15_brk():
    rom_path = "instr_test-v5/rom_singles/15-brk.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message

def test_16_special():
    rom_path = "instr_test-v5/rom_singles/16-special.nes"
    NESEmu_log = "latestLog.txt"
    status = 1
    message = ""
    test_message = ""
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '0', '1'],
                                capture_output=True, text=False, timeout=exec_time_out)
        test_message = status.stdout[status.stdout.find(b"Test result: "):].decode("utf-8")
        message = f"Test failed with status {status.returncode} and stdout {test_message}"
    except TimeoutExpired:
        message = "Program timed out"
    res = test_message.find("Passed") != -1
    assert res == True, message
