namespace PLX {

    class HashTable;

    class ArgParser {
    public:
        ArgParser();
        HashTable* parse(int argc, char** argv);
    };

}
