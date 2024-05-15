#include <argp.h>

#include <plx/ArgParser.hpp>
#include <plx/data/HashTable.hpp>
#include <plx/literal/Symbol.hpp>

const char *argp_program_version = "plx 0.1";
const char *argp_program_bug_address = "https://github.com/jeffrey-a-meunier";

namespace PLX {

    static char doc[] = "plx -- A small functional programming language.";

    struct Arguments {
        char *args[2]; // arg1 & arg2
        int silent;
        int verbose;
        char *output_file;
    };

    ArgParser::ArgParser() {}

    HashTable* argMap;

    static error_t parse_opt(int key, char *arg, struct argp_state *state) {
        Arguments* args = (Arguments*)state->input;
        switch (key) {
            case 'b':
                std::cerr << "b: " << arg << "\n";
                break;
            case 'q':
            case 's':
                args->silent = 1;
                break;
            case 'v':
                args->verbose = 1;
                break;
            case 'o':
                args->output_file = arg;
                break;
            // case ARGP_KEY_ARG:
            //     if (state->arg_num >= 2)
            //         /* Too many arguments. */
            //         argp_usage(state);
            //     args->args[state->arg_num] = arg;
            //     break;
            // case ARGP_KEY_END:
            //     if (state->arg_num < 2)
            //         /* Not enough arguments. */
            //         argp_usage(state);
            //     break;
            default:
                return ARGP_ERR_UNKNOWN;
        }
        return 0;
    }

    static struct argp_option options[] = {
        {"bind",    'b', "BINDING", 0, "Bind name to value, where BINDING is id=value", 0},
        {"verbose", 'v', 0, 0, "Produce verbose output", 0},
        {"quiet",   'q', 0, 0, "Don't produce any output", 0},
        {"silent",  's', 0, OPTION_ALIAS, 0, 0},
        {"output",  'o', "FILE", 0, "Output to FILE instead of standard output", 0},
        {0, 0, 0, 0, 0, 0}
    };


    HashTable *ArgParser::parse(int argc, char **argv) {
        argMap = new HashTable();

        Arguments args;
        args.silent = 0;
        args.verbose = 0;
        args.output_file = const_cast<char*>("-");

        /* Parse our arguments; every option seen by parse_opt will
            be reflected in arguments. */
        static struct argp argp = {options, parse_opt, 0, doc, 0, 0, 0};
        argp_parse(&argp, argc, argv, 0, 0, &args);

        // argMap->put(Symbol::create("Bindings"), bindings);
        // argMap->put(Symbol::create("Extra"), extra->asList());
        // argMap->put(Symbol::create("Positional"), positional->asList());
        // argMap->put(Symbol::create("Switches"), switches->asList());
        return argMap;
    }

}
