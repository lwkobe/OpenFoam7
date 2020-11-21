#include <dynamicCode.hxx>

#include <dynamicCodeContext.hxx>
#include <stringOps.hxx>
#include <IFstream.hxx>
#include <OFstream.hxx>
#include <OSspecific.hxx>
#include <etcFiles.hxx>
#include <dictionary.hxx>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

int tnbLib::dynamicCode::allowSystemOperations
(
	tnbLib::debug::infoSwitch("allowSystemOperations", 0)
);


const tnbLib::word tnbLib::dynamicCode::codeTemplateEnvName
= "FOAM_CODE_TEMPLATES";

const tnbLib::fileName tnbLib::dynamicCode::codeTemplateDirName
= "codeTemplates/dynamicCode";

const char* const tnbLib::dynamicCode::libTargetRoot =
"LIB = $(PWD)/../platforms/$(WM_OPTIONS)/lib/lib";

const char* const tnbLib::dynamicCode::topDirName = "dynamicCode";


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

void tnbLib::dynamicCode::checkSecurity
(
	const char* title,
	const dictionary& dict
)
{
	/*if (isAdministrator())
	{
		FatalIOErrorInFunction(dict)
			<< "This code should not be executed by someone with administrator"
			<< " rights due to security reasons." << nl
			<< "(it writes a shared library which then gets loaded "
			<< "using dlopen)"
			<< exit(FatalIOError);
	}*/

	if (!allowSystemOperations)
	{
		FatalIOErrorInFunction(dict)
			<< "Loading a shared library using case-supplied code is not"
			<< " enabled by default" << nl
			<< "because of security issues. If you trust the code you can"
			<< " enable this" << nl
			<< "facility be adding to the InfoSwitches setting in the system"
			<< " controlDict:" << nl << nl
			<< "    allowSystemOperations 1" << nl << nl
			<< "The system controlDict is either" << nl << nl
			<< "    ~/.OpenFOAM/$WM_PROJECT_VERSION/controlDict" << nl << nl
			<< "or" << nl << nl
			<< "    $WM_PROJECT_DIR/etc/controlDict" << nl
			<< endl
			<< exit(FatalIOError);
	}
}


tnbLib::word tnbLib::dynamicCode::libraryBaseName(const fileName& libPath)
{
	word libName(libPath.name(true));
	libName.erase(0, 3);    // Remove leading 'lib' from name
	return libName;
}



// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void tnbLib::dynamicCode::copyAndFilter
(
	ISstream& is,
	OSstream& os,
	const HashTable<string>& mapping
)
{
	if (!is.good())
	{
		FatalErrorInFunction
			<< "Failed opening for reading " << is.name()
			<< exit(FatalError);
	}

	if (!os.good())
	{
		FatalErrorInFunction
			<< "Failed writing " << os.name()
			<< exit(FatalError);
	}

	// Copy file while rewriting $VARS and ${VARS}
	string line;
	do
	{
		is.getLine(line);

		// Expand according to mapping.
		// Expanding according to env variables might cause too many
		// surprises
		stringOps::inplaceExpand(line, mapping);
		os.writeQuoted(line, false) << nl;
	} while (is.good());
}


bool tnbLib::dynamicCode::resolveTemplates
(
	const UList<fileName>& templateNames,
	DynamicList<fileName>& resolvedFiles,
	DynamicList<fileName>& badFiles
)
{
	// Try to get template from FOAM_CODESTREAM_TEMPLATES
	const fileName templateDir(tnbLib::getEnv(codeTemplateEnvName));

	bool allOkay = true;
	forAll(templateNames, fileI)
	{
		const fileName& templateName = templateNames[fileI];

		fileName file;
		if (!templateDir.empty() && isDir(templateDir))
		{
			file = templateDir / templateName;
			if (!isFile(file, false, false))
			{
				file.clear();
			}
		}

		// Not found - fallback to ~OpenFOAM expansion
		if (file.empty())
		{
			file = findEtcFile(codeTemplateDirName / templateName);
		}

		if (file.empty())
		{
			badFiles.append(templateName);
			allOkay = false;
		}
		else
		{
			resolvedFiles.append(file);
		}
	}

	return allOkay;
}


bool tnbLib::dynamicCode::writeCommentSHA1(Ostream& os) const
{
	const bool hasSHA1 = filterVars_.found("SHA1sum");

	if (hasSHA1)
	{
		os << "/* dynamicCode:\n * SHA1 = ";
		os.writeQuoted(filterVars_["SHA1sum"], false) << "\n */\n";
	}

	return hasSHA1;
}


bool tnbLib::dynamicCode::createMakeFiles() const
{
	// Create Make/files
	if (compileFiles_.empty())
	{
		return false;
	}

	const fileName dstFile(this->codePath() / "Make/files");

	// Create dir
	mkDir(dstFile.path());

	OFstream os(dstFile);
	// Info<< "Writing to " << dstFile << endl;
	if (!os.good())
	{
		FatalErrorInFunction
			<< "Failed writing " << dstFile
			<< exit(FatalError);
	}

	writeCommentSHA1(os);

	// Write compile files
	forAll(compileFiles_, fileI)
	{
		os.writeQuoted(compileFiles_[fileI], false) << nl;
	}

	os << nl
		<< libTargetRoot << codeName_.c_str() << nl;

	return true;
}


bool tnbLib::dynamicCode::createMakeOptions() const
{
	// Create Make/options
	if (compileFiles_.empty() || makeOptions_.empty())
	{
		return false;
	}

	const fileName dstFile(this->codePath() / "Make/options");

	// Create dir
	mkDir(dstFile.path());

	OFstream os(dstFile);
	// Info<< "Writing to " << dstFile << endl;
	if (!os.good())
	{
		FatalErrorInFunction
			<< "Failed writing " << dstFile
			<< exit(FatalError);
	}

	writeCommentSHA1(os);
	os.writeQuoted(makeOptions_, false) << nl;

	return true;
}


bool tnbLib::dynamicCode::writeDigest(const SHA1Digest& sha1) const
{
	const fileName file = digestFile();
	mkDir(file.path());

	OFstream os(file);
	sha1.write(os, true) << nl;

	return os.good();
}


bool tnbLib::dynamicCode::writeDigest(const std::string& sha1) const
{
	const fileName file = digestFile();
	mkDir(file.path());

	OFstream os(file);
	os << '_';
	os.writeQuoted(sha1, false) << nl;

	return os.good();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tnbLib::dynamicCode::dynamicCode(const word& codeName, const word& codeDirName)
	:
	codeRoot_(stringOps::expand("$FOAM_CASE") / topDirName),
	libSubDir_(stringOps::expand("platforms/$WM_OPTIONS/lib")),
	codeName_(codeName),
	codeDirName_(codeDirName)
{
	if (codeDirName_.empty())
	{
		codeDirName_ = codeName_;
	}

	clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tnbLib::fileName tnbLib::dynamicCode::codeRelPath() const
{
	return topDirName / codeDirName_;
}


tnbLib::fileName tnbLib::dynamicCode::libRelPath() const
{
	return codeRelPath() / libSubDir_ / "lib" + codeName_ + ".so";
}


void tnbLib::dynamicCode::clear()
{
	compileFiles_.clear();
	copyFiles_.clear();
	createFiles_.clear();
	filterVars_.clear();
	filterVars_.set("typeName", codeName_);
	filterVars_.set("SHA1sum", SHA1Digest().str());

	// Provide default Make/options
	makeOptions_ =
		"EXE_INC = -g\n"
		"\n\nLIB_LIBS = ";
}


void tnbLib::dynamicCode::reset
(
	const dynamicCodeContext& context
)
{
	clear();

	forAllConstIter(HashTable<string>, context.code(), iter)
	{
		setFilterVariable(iter.key(), iter());
	}

	setFilterVariable("SHA1sum", context.sha1().str());
}


void tnbLib::dynamicCode::addCompileFile(const fileName& name)
{
	compileFiles_.append(name);
}


void tnbLib::dynamicCode::addCopyFile(const fileName& name)
{
	copyFiles_.append(name);
}


void tnbLib::dynamicCode::addCreateFile
(
	const fileName& name,
	const string& contents
)
{
	createFiles_.append(fileAndContent(name, contents));
}


void tnbLib::dynamicCode::setFilterVariable
(
	const word& key,
	const std::string& value
)
{
	filterVars_.set(key, value);
}


void tnbLib::dynamicCode::setMakeOptions(const std::string& content)
{
	makeOptions_ = content;
}


bool tnbLib::dynamicCode::copyOrCreateFiles(const bool verbose) const
{
	if (verbose)
	{
		Info << "Creating new library in " << this->libRelPath() << endl;
	}

	const label nFiles = compileFiles_.size() + copyFiles_.size();

	DynamicList<fileName> resolvedFiles(nFiles);
	DynamicList<fileName> badFiles(nFiles);

	// Resolve template, or add to bad-files
	resolveTemplates(compileFiles_, resolvedFiles, badFiles);
	resolveTemplates(copyFiles_, resolvedFiles, badFiles);

	if (!badFiles.empty())
	{
		FatalErrorInFunction
			<< "Could not find the code template(s): "
			<< badFiles << nl
			<< "Under the $" << codeTemplateEnvName
			<< " directory or via via the ~OpenFOAM/"
			<< codeTemplateDirName << " expansion"
			<< exit(FatalError);
	}



	// Create dir
	const fileName outputDir = this->codePath();

	// Create dir
	mkDir(outputDir);

	// Copy/filter files
	forAll(resolvedFiles, fileI)
	{
		const fileName& srcFile = resolvedFiles[fileI];
		const fileName  dstFile(outputDir / srcFile.name());

		IFstream is(srcFile);
		// Info<< "Reading from " << is.name() << endl;
		if (!is.good())
		{
			FatalErrorInFunction
				<< "Failed opening " << srcFile
				<< exit(FatalError);
		}

		OFstream os(dstFile);
		// Info<< "Writing to " << dstFile.name() << endl;
		if (!os.good())
		{
			FatalErrorInFunction
				<< "Failed writing " << dstFile
				<< exit(FatalError);
		}

		// Copy lines while expanding variables
		copyAndFilter(is, os, filterVars_);
	}


	// Create files:
	forAll(createFiles_, fileI)
	{
		const fileName dstFile
		(
			outputDir / stringOps::expand(createFiles_[fileI].first())
		);

		mkDir(dstFile.path());
		OFstream os(dstFile);
		// Info<< "Writing to " << createFiles_[fileI].first() << endl;
		if (!os.good())
		{
			FatalErrorInFunction
				<< "Failed writing " << dstFile
				<< exit(FatalError);
		}
		os.writeQuoted(createFiles_[fileI].second(), false) << nl;
	}


	// Create Make/files + Make/options
	createMakeFiles();
	createMakeOptions();

	writeDigest(filterVars_["SHA1sum"]);

	return true;
}


bool tnbLib::dynamicCode::wmakeLibso() const
{
	const tnbLib::string wmakeCmd("wmake -s libso " + this->codePath());
	Info << "Invoking " << wmakeCmd << endl;

	if (tnbLib::system(wmakeCmd))
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool tnbLib::dynamicCode::upToDate(const SHA1Digest& sha1) const
{
	const fileName file = digestFile();

	if (!exists(file, false, false) || SHA1Digest(IFstream(file)()) != sha1)
	{
		return false;
	}

	return true;
}


bool tnbLib::dynamicCode::upToDate(const dynamicCodeContext& context) const
{
	return upToDate(context.sha1());
}


// ************************************************************************* //