# NESEMU - NES Emulator

NESEMU is a software emulator for the Nintendo Entertainment System (NES), a popular 8-bit home video game console from the 1980s.
This emulator is the NTSC version of the NES. There are no current plans to add PAL compatibility.

  ---------------## Dependencies

- [SDL2](https://www.libsdl.org/download-2.0.php)
- [SPDLOG](https://github.com/gabime/spdlog)
- [cxxopts](https://github.com/jarro2783/cxxopts)

## Building

### Windows

Currently, NESEMU is only compiled using Visual Studio 2022's compiler on Windows, and it is developed using CLion as the IDE.

#### Build Steps

1. Clone the repository: `git clone https://github.com/EA677698/NESEmu`.
2. Open the project in your perferred IDE.
3. Make sure SDL2, SPDLOG, and cxxopts are properly linked.
4. Build and run the project.

### Other Operating Systems

Support for other operating systems is planned and will be added in the future.

## CLI Usage

The emulator can be used through the CLI.

The following options are available:
```shell
Usage: NESEmu [options]
Options:
  -h, --help
    Print help
  -r, --rom <path>
    Path to the NES ROM file
  -t  --test
    Test type
  -d, --debug
    Enable debug mode logging
  -m, --dump_memory
    Dump end memory to a file
  -i, --instruction_cycles
    Print instruction cycles
  -p --disable_ppu
    Disable PPU rendering
```

## Resources

A lot of information, research, and resources that have been used in this project can be found at [nesdev.org](https://nesdev.org).

Information about unofficial opcodes used can be found at [www.masswerk.at](https://www.masswerk.at/nowgobang/2021/6502-illegal-opcodes).

## Contributing

If you're interested in contributing, please open an issue or a pull request. Contributions are welcome!

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

- Special thanks to [nesdev.org](https://nesdev.org) for the comprehensive information and guidance.

## Contact

Feel free to reach out to me at eafc677698@gmail.com for any questions or feedback.
