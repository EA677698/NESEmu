import pytest

import subprocess

executable_path = "../build/bin/Release/emulator.exe"


def test_check():
    assert 1 + 1 == 2


def test_basics():
    result = subprocess.run([executable_path, 'instr_test-v5/rom_singles/01_basics.nes', '-1', '0x6000', '0x6004', '~'],
                            capture_output=True, text=True)
    assert result.returncode != 1
