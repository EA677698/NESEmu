import pytest
import find_diff as fd
import subprocess

executable_path = "../build/bin/Release/emulator.exe"
#executable_path = "../cmake-build-debug/bin/emulator.exe"
exec_time_out = 10


def test_check():
    assert 1 + 1 == 2
    print("Basic check succeeded, 1 + 1 == 2")


def test_nestest():
    rom_path = "nestest/nestest.nes"
    rom_log = "nestest/nestest_log.txt"
    NESEmu_log = "latestLog.txt"
    #NESEmu_log = "../cmake-build-debug/bin/latestLog.txt"
    subprocess.run([executable_path, rom_path, 'nestest', '1', '1'],
                   capture_output=True, text=False, timeout=exec_time_out)
    result = fd.compare_logs(rom_log, NESEmu_log)
    assert result == (0, 0), f"Mismatch at line {result[0]} in nestest log and {result[1]} in NESEmu log"
