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

exec_time_out = 5


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
    try:
        status = subprocess.run([executable_path, rom_path, 'blargg', '1', '1'],
                       capture_output=True, text=False, timeout=exec_time_out)
    except TimeoutExpired:
        print("Program timed out")
    assert status == 0, f"Test failed with status {status}"
