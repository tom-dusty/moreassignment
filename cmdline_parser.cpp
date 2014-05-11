#include "cmdline_parser.h"

//-------------------------------------------------------------------------//
// Constructor, initialise the variables_map object with default
// constructor, options_descriptor with the name "Options"
//-------------------------------------------------------------------------//
cmdline_parser::cmdline_parser(void) : vm(), od("Options")
{
	// Shorter alias for the boost::program_options namespace
	namespace po = boost::program_options;

	// Add cmdline options
	// --help or -?
	// --input or -i
	// --output or -o
	// --threads or -t
	od.add_options()
		("help,?", "produce help message")
		((INPUTFILE+",i").c_str(), po::value<std::string>()->default_value("numbers.lst"),
			"input file name")
		((OUTPUTFILE+",o").c_str(), po::value<std::string>()->default_value("primes.lst"),
			"output file name")
		((NUMBERTHREADS+",t").c_str(), po::value<int>()->default_value(4),
			"number of threads"); 
};

//-------------------------------------------------------------------------//
// Process the cmdline
//-------------------------------------------------------------------------//
bool cmdline_parser::process_cmdline(int argc, char * argv[])
{
	// Shorter alias for the boost::program_options namespace
	namespace po = boost::program_options;

	// Clear the variable map
	vm.clear();

	// Parse the cmdline arguments defined by argc and argv,
	// looking for the options defined in the od variable,
	// and store them in the vm variable.
	po::store(po::parse_command_line(argc, argv, od), vm);
	po::notify(vm);

	// Assume processing always succeeds
	return true;
}

//-----------------------------------------------------------------------//
// Return the input filename
//-------------------------------------------------------------------------//
std::string cmdline_parser::get_input_filename(void) const
{
	// Return whatever value is stored as a string
	return vm[INPUTFILE].as<std::string>();
}

//-----------------------------------------------------------------------//
// Return the output filename
//-------------------------------------------------------------------------//
std::string cmdline_parser::get_output_filename(void) const
{
	// Return whatever value is stored  as a string
	return vm[OUTPUTFILE].as<std::string>();
}

//-------------------------------------------------------------------------//
// Get the number of threads to use
//-------------------------------------------------------------------------//
int cmdline_parser::get_number_threads(void)
	{ return vm[NUMBERTHREADS].as<int>(); }

//-----------------------------------------------------------------------//
// Should we print cmdline help?
//-------------------------------------------------------------------------//
bool cmdline_parser::should_print_help(void) const
{
	// Are there instances of the help option stored in the variable map
	return vm.count("help") > 0;
}

//-------------------------------------------------------------------------//
// Print out the options_descriptor to the supplied output stream
//-------------------------------------------------------------------------//
void cmdline_parser::print_help(std::ostream & out) const
{
	out << od;
}

//-------------------------------------------------------------------------//
// Print out the options_descriptor to the supplied output stream
//-------------------------------------------------------------------------//
void cmdline_parser::print_errors(std::ostream & out) const
{
	std::cerr << "Error processing command line arguments:" << std::endl;
	std::copy(errors.begin(), errors.end(),
		std::ostream_iterator<std::string>(out, "\n"));
}

// Definition of static strings in the class
const std::string cmdline_parser::INPUTFILE     = "input";
const std::string cmdline_parser::OUTPUTFILE    = "output";
const std::string cmdline_parser::NUMBERTHREADS = "threads";

