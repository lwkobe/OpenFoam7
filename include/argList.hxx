#pragma once
#ifndef _argList_Header
#define _argList_Header

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
	\\  /    A nd           | Copyright (C) 2011-2019 OpenFOAM Foundation
	 \\/     M anipulation  |
-------------------------------------------------------------------------------
License
	This file is part of OpenFOAM.

	OpenFOAM is free software: you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
	for more details.

	You should have received a copy of the GNU General Public License
	along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
	tnbLib::argList

Description
	Extract command arguments and options from the supplied
	\a argc and \a argv parameters.

	Sequences with "(" ... ")" are transformed into a stringList.
	For example,
	\verbatim
		program -listFiles \( *.txt \)
	\endverbatim
	would create a stringList:
	\verbatim
		( "file1.txt" "file2.txt" ... "fileN.txt" )
	\endverbatim
	The backslash-escaping is required to avoid interpretation by the shell.

	Default command-line options:
	  - \par -case \<dir\>
		Select a case directory instead of the current working directory
	  - \par -parallel
		Specify case as a parallel job
	  - \par -doc
		Display the documentation in browser
	  - \par -srcDoc
		Display the source documentation in browser
	  - \par -help
		Print the usage

	The environment variable \b FOAM_CASE is set to the path of the
	global case (same for serial and parallel jobs).
	The environment variable \b FOAM_CASENAME is set to the name of the
	global case.

Note
	- The document browser used is defined by the \b FOAM_DOC_BROWSER
	  environment variable or the <tt>Documentation/docBrowser</tt> entry
	  in the <tt>~OpenFOAM/controlDict</tt> file.
	  The \%f token is used as a placeholder for the file name.
	- The valid (mandatory) arguments can be adjusted
	  by directly manipulating the argList::validArgs static member.
	- The valid options can be adjusted
	  via the addOption/removeOption static methods instead of directly
	  manipulating the argList::validOptions static member.

SourceFiles
	argList.C
	argListI.H

\*---------------------------------------------------------------------------*/

#include <stringList.hxx>
#include <SubList.hxx>
#include <SLList.hxx>
#include <HashTable.hxx>
#include <word.hxx>
#include <fileName.hxx>
#include <parRun.hxx>
#include <IStringStream.hxx>
#include <OSspecific.hxx>

#include <sigFpe.hxx>
#include <sigInt.hxx>
#include <sigQuit.hxx>
#include <sigSegv.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{

	/*---------------------------------------------------------------------------*\
							   Class argList Declaration
	\*---------------------------------------------------------------------------*/

	class argList
	{
		// Private Data

			//- Switch on/off parallel mode. Has to be first to be constructed
			//  so destructor is done last.
		ParRunControl parRunControl_;

		stringList args_;
		HashTable<string> options_;

		word executable_;
		fileName rootPath_;
		fileName globalCase_;
		fileName case_;
		string argListStr_;

		// Signal handlers
		sigFpe sigFpe_;
		sigInt sigInt_;
		sigQuit sigQuit_;
		sigSegv sigSegv_;


		// Private Member Functions

			//- Helper function for printUsage
		static FoamBase_EXPORT void printOptionUsage
		(
			const label location,
			const string& str
		);

		//- Get rootPath_ / globalCase_ from one of the following forms
		//   * [-case dir]
		//   * cwd
		//
		// Also export FOAM_CASE and FOAM_CASENAME environment variables
		// so they can be used immediately (eg, in decomposeParDict)
		FoamBase_EXPORT void getRootCase();

		//- Transcribe argv into internal args_
		//  return true if any "(" ... ")" sequences were captured
		FoamBase_EXPORT bool regroupArgv(int& argc, char**& argv);


	public:

		// Static Data Members

			//- A list of valid (mandatory) arguments
		static FoamBase_EXPORT SLList<string> validArgs;

		//- A list of valid options
		static FoamBase_EXPORT HashTable<string> validOptions;

		//- A list of valid parallel options
		static FoamBase_EXPORT HashTable<string> validParOptions;

		//- Short usage information for validOptions
		static FoamBase_EXPORT HashTable<string> optionUsage;

		//- Additional notes for usage
		static FoamBase_EXPORT SLList<string> notes;

		//- Min offset for displaying usage (default: 20)
		static FoamBase_EXPORT string::size_type usageMin;

		//- Max screen width for displaying usage (default: 80)
		static FoamBase_EXPORT string::size_type usageMax;

		//- Standard name for the post-processing option
		static FoamBase_EXPORT word postProcessOptionName;

		// Class to initialize options table
		// with the standard case related options
		class initValidTables
		{
		public:

			FoamBase_EXPORT initValidTables();

			//- Clear the options table
			//  for utilities which do not operate on files rather than cases
			static FoamBase_EXPORT void clear();
		};


		// Constructors

			//- Construct from argc and argv
			//  checking the arguments and options as requested
		FoamBase_EXPORT argList
		(
			int& argc,
			char**& argv,
			bool checkArgs = true,
			bool checkOpts = true,
			bool initialise = true
		);

		//- Construct copy with new options
		FoamBase_EXPORT argList
		(
			const argList& args,
			const HashTable<string>& options,
			bool checkArgs = true,
			bool checkOpts = true,
			bool initialise = true
		);


		//- Destructor
		FoamBase_EXPORT virtual ~argList();


		// Member Functions

			//- Parse
		FoamBase_EXPORT void parse
		(
			bool checkArgs,
			bool checkOpts,
			bool initialise
		);


		// Access

			//- Name of executable without the path
		inline const word& executable() const;

		//- Return root path
		inline const fileName& rootPath() const;

		//- Return case name (parallel run) or global case (serial run)
		inline const fileName& caseName() const;

		//- Return case name
		inline const fileName& globalCaseName() const;

		//- Return parRunControl
		inline const ParRunControl& parRunControl() const;

		//- Return the path to the caseName
		inline fileName path() const;

		//- Return arguments
		inline const stringList& args() const;

		//- Return non-const access to arguments
		inline stringList& args();

		//- Return the argument corresponding to index.
		inline const string& arg(const label index) const;

		//- Return the number of arguments
		inline label size() const;

		//- Read a value from the argument at index.
		//  Index 0 corresponds to the name of the executable.
		//  Index 1 corresponds to the first argument.
		template<class T>
		inline T argRead(const label index) const;

		//- Return options
		inline const tnbLib::HashTable<string>& options() const;

		//- Return non-const access to options
		inline tnbLib::HashTable<string>& options();

		//- Return the argument string associated with the named option
		inline const string& option(const word& opt) const;

		//- Return true if the named option is found
		inline bool optionFound(const word& opt) const;

		//- Return an IStringStream from the named option
		inline IStringStream optionLookup(const word& opt) const;

		//- Read a value from the named option
		template<class T>
		inline T optionRead(const word& opt) const;

		//- Read a value from the named option if present.
		//  Return true if the named option was found.
		template<class T>
		inline bool optionReadIfPresent(const word& opt, T&) const;

		//- Read a value from the named option if present.
		//  Return true if the named option was found, otherwise
		//  use the supplied default and return false.
		template<class T>
		inline bool optionReadIfPresent
		(
			const word& opt,
			T&,
			const T& deflt
		) const;

		//- Read a value from the named option if present.
		//  Return supplied default otherwise.
		template<class T>
		inline T optionLookupOrDefault
		(
			const word& opt,
			const T& deflt
		) const;

		//- Read a List of values from the named option
		template<class T>
		List<T> optionReadList(const word& opt) const
		{
			return readList<T>(optionLookup(opt)());
		}


		//- Return the argument corresponding to index.
		//  Index 0 corresponds to the name of the executable.
		//  Index 1 corresponds to the first argument.
		inline const string& operator[](const label index) const;

		//- Return the argument string associated with the named option
		//  \sa option()
		inline const string& operator[](const word& opt) const;


		// Edit

			//- Add to a bool option to validOptions with usage information
		static FoamBase_EXPORT void addBoolOption
		(
			const word& opt,
			const string& usage = ""
		);

		//- Add to an option to validOptions with usage information
		//  An option with an empty param is a bool option
		static FoamBase_EXPORT void addOption
		(
			const word& opt,
			const string& param = "",
			const string& usage = ""
		);

		//- Add option usage information to optionUsage
		static FoamBase_EXPORT void addUsage
		(
			const word& opt,
			const string& usage
		);

		//- Add extra notes for the usage information
		//  This string is used "as-is" without additional formatting
		static FoamBase_EXPORT void addNote(const string&);

		//- Remove option from validOptions and from optionUsage
		static FoamBase_EXPORT void removeOption(const word& opt);

		//- Remove the parallel options
		static FoamBase_EXPORT void noParallel();

		//- Return true if the post-processing option is specified
		static FoamBase_EXPORT bool postProcess(int argc, char *argv[]);

		//- Set option directly (use with caution)
		//  An option with an empty param is a bool option.
		//  Not all valid options can also be set: eg, -case, -roots, ...
		//  Return true if the existing option value needed changing,
		//  or if the option did not previously exist.
		FoamBase_EXPORT bool setOption(const word& opt, const string& param = "");

		//- Unset option directly (use with caution)
		//  Not all valid options can also be unset: eg, -case, -roots ...
		//  Return true if the option existed before being unset.
		FoamBase_EXPORT bool unsetOption(const word& opt);


		// Print

			//- Print notes (if any)
		FoamBase_EXPORT void printNotes() const;

		//- Print usage
		FoamBase_EXPORT void printUsage() const;

		//- Display documentation in browser
		//  Optionally display the application source code
		FoamBase_EXPORT void displayDoc(bool source = false) const;


		// Check

			//- Check argument list
		FoamBase_EXPORT bool check(bool checkArgs = true, bool checkOpts = true) const;

		//- Check root path and case path
		FoamBase_EXPORT bool checkRootCase() const;
	};


	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace tnbLib

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include <argListI.hxx>

#endif // !_argList_Header
