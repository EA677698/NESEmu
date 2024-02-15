import pytest

import subprocess

executable_path = "../build/bin/Release/emulator.exe"


def test_check():
    assert 1 + 1 == 2
    print("Basic check succeeded, 1 + 1 == 2")


def test_basics():
    rom_path = "instr_test-v5/rom_singles/01-basics.nes"
    result = subprocess.run([executable_path, rom_path, '-1', '0x6000', '0x6004', '~'],
                            capture_output=True, text=True)
    print(f"Return code: {result.returncode}")
    print(f"Stderr: {result.stderr}")
    assert result.returncode == 0, f"Expected return code 0, got {result.returncode}"
