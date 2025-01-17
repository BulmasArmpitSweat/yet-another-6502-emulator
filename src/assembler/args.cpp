#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <getopt.h>
#include "include.cpp"

constexpr char* version = (char *)"UWUASM embedded assembler v0.2";

static void Version() {
	std::cout << version << std::endl;
}

static void Help(std::string prog_name) {
	std::cout << prog_name + ": [options] file...\n" << std::endl;
	
	std::cout << "Yet another assembler for the 6502. Supports a modified version of old-style 6502 syntax\n" << std::endl;

	std::cout << "Options:\n" << std::endl;
	std::cout << " -h, --help         shows help and exits" << std::endl;
	std::cout << " -opt, --options    shows available assembler options and exits" << std::endl;
	std::cout << " --version          shows version information and exits\n" << std::endl;

	std::cout << " -c <file>          specify the input file. Defaults to stdin if not specified" << std::endl;
	std::cout << " -o <file>          specify the output file. Defaults to 'a.out' if not specified" << std::endl;
	std::cout << " -v, --verbose      make the assembler print out information about what it's doing"<< std::endl;
	std::cout << " -q, --quiet        silence all warnings and output" << std::endl;
	std::cout << " -A<option>	    enable a select assembler option. Can also be passed into the assembler in the file with this syntax: '#opt = <option>'" << std::endl;
}

static void Assembler_options() {
	std::cout << "Available assembler options:" << std::endl;
	std::cout << '\t' << "KEEP-TEMPS                           ||  Keep temporary files, such as the token file, which is saved alongside the output file" << std::endl;
	std::cout << '\t' << "NO-UNDOCUMENTED-INSTRUCTION-WARNING  ||  Suppresses warnings when undocumented instructions are used" << std::endl;
	std::cout << '\t' << "KEEP-OUTPUT-ON-FAIL                  ||  Don't delete output files when compilation is prematurely terminated" << std::endl;
}

struct args {
	bool verbose = false;
	bool quiet = false;
	std::string input = "/dev/stdin";
	std::string output = "a.out";
	std::vector<std::string> assembler_args = {};
};

constexpr int option_only_long = 1000;

static constexpr struct option options[] = {
	{"help",        no_argument, 	   0,  'h'},
	{"version",     no_argument, 	   0,   0 },
	{"verbose",     no_argument, 	   0,  'v'},
	{"quiet",       no_argument, 	   0,  'q'},
	{"options",     no_argument, 	   0, option_only_long},
	{0, 0, 0, 0} // Terminating entry
};


static constexpr char* args_short = (char *)"hvqo:A:c:";

struct args parse(int argc, char** argv) {
	args arg;

	int idx;
	int c;

	while ((c = getopt_long(argc, argv, args_short, options, &idx)) != -1) {
		switch (c) {
			case 'v':
				arg.verbose = true;
				break;
			case 'q':
				arg.quiet = true;
				break;
			case 'A':
				arg.assembler_args.push_back(optarg);
				break;
			case 'o':
				arg.output = optarg;
				break;
			case 'c':
				arg.input = optarg;
				break;
			case 'h':
				Help(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case option_only_long:
				Assembler_options();
				exit(EXIT_SUCCESS);
				break;
			case 0:
				Version();
				exit(EXIT_SUCCESS);
				break;
			case '?':
				error("Unrecognized option: " + std::string(argv[optind - 1]));
				exit(EXIT_FAILURE);
		}
	}
	
	// Relative file paths
	if (arg.input[0] != '/') {
		arg.input = "./" + arg.input;
	}
	
	return arg;
}
