#ifndef CMDLINE_PARSER_H_
#define CMDLINE_PARSER_H_

#include <iostream>
#include <list>
#include <string>

#include <boost/program_options.hpp>

//---------------------------------------------------------------------------//
class cmdline_parser
{
public:
    //-----------------------------------------------------------------------//
	// Constructor
	cmdline_parser(void); 

	// Member function that parses command line options
	bool process_cmdline(int argc, char * argv[]);
	// Return the input filename
	std::string get_input_filename(void) const;
	// Return the output filename
	std::string get_output_filename(void) const;
	// Get the number of threads
	int get_number_threads(void);
	// Get the vignere key
	std::string get_vignere_key(void) const;
	// Get the xor encrypt key
	int get_xor_key(void) const;
	// Get the xor encrypt Initialization key for CBC mode
	int get_IV(void) const;
	// Get the xor encrypt mode
	std::string get_mode(void) const;

	void print_errors(std::ostream & out) const;

	bool should_print_help(void) const;
	// Output help to the specified output stream
	void print_help(std::ostream & out) const;
	// Various boolean options that could be chosen
	bool should_group(void) const;
	bool should_pack(void) const;
	bool should_encode(void) const;
	bool should_decode(void) const;
	// Returns 'true' for vignere and 'false' for xor
	bool vignere_or_xor(void) const;


private:
    //-----------------------------------------------------------------------//
	// Member variables
	boost::program_options::variables_map vm;
	boost::program_options::options_description od;
	std::list<std::string> errors;

    //-----------------------------------------------------------------------//
	// Static string variables
	static const std::string INPUTFILE;
	static const std::string OUTPUTFILE;
	static const std::string NUMBERTHREADS;
};

#endif /* CMDLINE_PARSER_H_ */
