#include "utils/logger.hpp"
#include "utils/skutilflagparser.hpp"
#include "gameboy/classic.hpp"

#include <string>

using namespace GameBoy;

Logger logger;

std::string romPath = "";
void ChangeRomPath(OPT int inputCount, OPT char** inputVal)
{
    logger.Log(TRACE, "Changing rom path to %s", inputVal[0]);
    romPath = inputVal[0];
}

int main(int argc, char** argv) 
{
    // FIXME: For some reason the logger does not work during some executions
    // this isnt a major issue since the program could be re-run until output
    // is shown but that should not be the main fix
    logger = Logger("logs/", "info");
    
    // TODO: Check for debug flag passed to pre proc 
    logger.EnableTracing();

    SKUTIL::SK_VEC<SKUTIL::Flag> flags {
        {
            'f',
            "rom-file",
            "Location of rom file to be loaded into Gameboy",
            1,
            ChangeRomPath,
        },
    };

    SKUTIL::FlagParser parser = SKUTIL::FlagParser(&flags);

    if (argc == 1) {
        logger.Log(ERROR, "Usage: ./gameboy-emu -f <rom path>");
        return -1;
    } else {
        parser.ParseFlags(argc, argv);
    }

    if (romPath == "") {
        logger.Log(INFO, "No Rom provided");
        logger.Log(ERROR, "Usage: ./gameboy-emu -f <rom path>");
        return -1;
    }

    // Initialize cartridge and load data from file [path] into memory
    Cartridge::Cartridge cartridge = Cartridge::Cartridge(romPath);
    // cartridge.DumpContents();

    // Initialize gameboy classic
    Classic gb = Classic();
    gb.LoadCartridge(&cartridge);
    gb.Start();
}
