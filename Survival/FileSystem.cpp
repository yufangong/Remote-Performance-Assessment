/////////////////////////////////////////////////////////////////////////////
// FileSystem.cpp - Support file and directory operations                  //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Jim Fawcett, 2012                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Dell XPS 8300, Core i7, Windows 7 SP1                      //
// Application: Spring Projects, 2014                                      //
// Author:      Jim Fawcett, CST 4-187, Syracuse University                //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include "FileSystem.h"

using namespace FileSystem;

/////////////////////////////////////////////////////////
// helper FileSystemSearch

class FileSystemSearch
{
public:
	FileSystemSearch();
	~FileSystemSearch();
	std::string firstFile(const std::string& path = ".", const std::string& pattern = "*.*");
	std::string nextFile();
	std::string firstDirectory(const std::string& path = ".", const std::string& pattern = "*.*");
	std::string nextDirectory();
	void close();
private:
	HANDLE hFindFile;
	WIN32_FIND_DATAA FindFileData;
	WIN32_FIND_DATAA* pFindFileData;
};

FileSystemSearch::FileSystemSearch() : pFindFileData(&FindFileData) {}
FileSystemSearch::~FileSystemSearch() { ::FindClose(hFindFile); }
void FileSystemSearch::close() { ::FindClose(hFindFile); }

//----< block constructor taking array iterators >-------------------------

Block::Block(Byte* beg, Byte* end) : bytes_(beg, end) {}

//----< push back block byte >---------------------------------------------

void Block::push_back(Byte b)
{
	bytes_.push_back(b);
}
//----< non-const indexer >------------------------------------------------

Byte& Block::operator[](size_t i)
{
	if (i<0 || bytes_.size() <= i)
		throw std::runtime_error("index out of range in Block");
	return bytes_[i];
}
//----< const indexer >----------------------------------------------------

Byte Block::operator[](size_t i) const
{
	if (i<0 || bytes_.size() <= i)
		throw std::runtime_error("index out of range in Block");
	return bytes_[i];
}
//----< equality comparison >----------------------------------------------

bool Block::operator==(const Block& block) const
{
	return bytes_ == block.bytes_;
}
//----< inequality comparison >--------------------------------------------

bool Block::operator!=(const Block& block) const
{
	return bytes_ != block.bytes_;
}
//----< return number of bytes in block >----------------------------------

size_t Block::size() const
{
	return bytes_.size();
}

//----< File constructor opens file stream >-------------------------------

File::File(const std::string& filespec)
: name_(filespec), pIStream(0), pOStream(0), dirn_(in), typ_(text), good_(true)
{
}
//----< File destructor closes file stream >-------------------------------

File::~File()
{
	if (pIStream)
	{
		pIStream->close();
		delete pIStream;
	}
	if (pOStream)
	{
		pOStream->close();
		delete pOStream;
	}
}
//----< open for reading or writing >--------------------------------------

bool File::open(direction dirn, type typ)
{
	dirn_ = dirn;
	typ_ = typ;
	if (dirn == in)
	{
		pIStream = new std::ifstream;
		if (typ == binary)
			pIStream->open(name_.c_str(), std::ios::in | std::ios::binary);
		else
			pIStream->open(name_.c_str(), std::ios::in);
		if (!(*pIStream))
			good_ = false;
		//throw std::runtime_error("\n  open for input failed in File constructor");
	}
	else
	{
		pOStream = new std::ofstream;
		if (typ == binary)
			pOStream->open(name_.c_str(), std::ios::out | std::ios::binary);
		else
			pOStream->open(name_.c_str(), std::ios::out);
		if (!(*pOStream))
			good_ = false;
		//throw std::runtime_error("\n  open for output failed in File constructor");
	}
	return good_;
}
//----< reads one line of a text file >------------------------------------

std::string File::getLine()
{
	if (pIStream == 0 || !pIStream->good())
		throw std::runtime_error("input stream not open");
	if (typ_ == binary)
		throw std::runtime_error("getting text line from binary file");
	if (dirn_ == out)
		throw std::runtime_error("reading output file");
	const int BufSize = 255;
	char buffer[BufSize];
	pIStream->getline(buffer, BufSize);
	return std::string(buffer);
}
//----< writes one line of a text to a file >------------------------------

void File::putLine(const std::string& s, bool wantReturn)
{
	if (pOStream == 0 || !pOStream->good())
		throw std::runtime_error("output stream not open");
	if (typ_ == binary)
		throw std::runtime_error("writing text line to binary file");
	if (dirn_ == in)
		throw std::runtime_error("writing input file");
	for (size_t i = 0; i<s.size(); ++i)
		pOStream->put(s[i]);
	if (wantReturn)
		pOStream->put('\n');
	pOStream->flush();
}
//----< reads a block of bytes from binary file >--------------------------

Block File::getBlock(size_t size)
{
	if (pIStream == 0 || !pIStream->good())
		throw std::runtime_error("input stream not open");
	if (typ_ != binary)
		throw std::runtime_error("reading binary from text file");
	if (dirn_ == out)
		throw std::runtime_error("reading output file");
	Block blk;
	if (pIStream)
	{
		for (size_t i = 0; i<size; ++i)
		{
			Byte b;
			pIStream->get(b);
			if (pIStream->good())
				blk.push_back(b);
			else
				break;
		}
	}
	return blk;
}
//----< writes a block of bytes to binary file >---------------------------

void File::putBlock(const Block& blk)
{
	if (pOStream == 0 || !pOStream->good())
		throw std::runtime_error("output stream not open");
	if (typ_ != binary)
		throw std::runtime_error("writing binary to text file");
	if (dirn_ == in)
		throw std::runtime_error("writing input file");
	if (!pOStream->good())
		return;
	for (size_t i = 0; i<blk.size(); ++i)
	{
		pOStream->put(blk[i]);
	}
}
//----< tests for error free stream state >--------------------------------

bool File::isGood()
{
	if (!good_)
		return false;
	if (pIStream)
		return (good_ = pIStream->good());
	if (pOStream)
		return (good_ = pOStream->good());
	return (good_ = false);
}
//----< flushes output stream to its file >--------------------------------

void File::flush()
{
	if (pOStream)
		pOStream->flush();
	if (pOStream)
		return;
}
//----< clears error state enabling operations again >---------------------

void File::clear()
{
	if (pIStream)
		pIStream->clear();
	if (pOStream)
		pOStream->clear();
}
//----< close file handle >------------------------------------------------

void File::close()
{
	File::flush();
	if (pIStream)
		pIStream->close();
	if (pOStream)
		pOStream->close();
}
//----< file exists >--------------------------------------------------

bool File::exists(const std::string& file)
{
	return ::GetFileAttributesA(file.c_str()) != INVALID_FILE_ATTRIBUTES;
}
//----< copy file >----------------------------------------------------

bool File::copy(const std::string& src, const std::string& dst, bool failIfExists)
{
	return ::CopyFileA(src.c_str(), dst.c_str(), failIfExists) != 0;
}
//----< remove file >--------------------------------------------------

bool File::remove(const std::string& file)
{
	return ::DeleteFileA(file.c_str()) != 0;
}
//----< constructor >--------------------------------------------------

FileInfo::FileInfo(const std::string& fileSpec)
{
	HANDLE hFile = ::FindFirstFileA(fileSpec.c_str(), &data);
	if (hFile == INVALID_HANDLE_VALUE)
		good_ = false;
	else
		good_ = true;
}
//----< is passed filespec valid? >------------------------------------

bool FileInfo::good()
{
	return good_;
}
//----< return file name >---------------------------------------------

std::string FileInfo::name() const
{
	return Path::getName(data.cFileName);
}
//----< conversion helper >--------------------------------------------

std::string FileInfo::intToString(long i)
{
	std::ostringstream out;
	out.fill('0');
	out << std::setw(2) << i;
	return out.str();
}
//----< return file date >---------------------------------------------

std::string FileInfo::date(dateFormat df) const
{
	std::string dateStr, timeStr;
	FILETIME ft;
	SYSTEMTIME st;
	::FileTimeToLocalFileTime(&data.ftLastWriteTime, &ft);
	::FileTimeToSystemTime(&ft, &st);
	dateStr = intToString(st.wMonth) + '/' + intToString(st.wDay) + '/' + intToString(st.wYear);
	timeStr = intToString(st.wHour) + ':' + intToString(st.wMinute) + ':' + intToString(st.wSecond);
	if (df == dateformat)
		return dateStr;
	if (df == timeformat)
		return timeStr;
	return dateStr + " " + timeStr;
}
//----< return file size >---------------------------------------------

size_t FileInfo::size() const
{
	return (size_t)(data.nFileSizeLow + (data.nFileSizeHigh << 8));
}
//----< is type archive? >---------------------------------------------

bool FileInfo::isArchive() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0;
}
//----< is type compressed? >------------------------------------------

bool FileInfo::isCompressed() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0;
}
//----< is type directory? >-------------------------------------------

bool FileInfo::isDirectory() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
//----< is type encrypted? >---------------------------------------------

bool FileInfo::isEncrypted() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) != 0;
}
//----< is type hiddent? >---------------------------------------------

bool FileInfo::isHidden() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;
}
//----< is type normal? >---------------------------------------------

bool FileInfo::isNormal() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0;
}
//----< is type offline? >---------------------------------------------

bool FileInfo::isOffLine() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE) != 0;
}
//----< is type readonly? >--------------------------------------------

bool FileInfo::isReadOnly() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
}
//----< is type system? >----------------------------------------------

bool FileInfo::isSystem() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0;
}
//----< is type temporary? >-------------------------------------------

bool FileInfo::isTemporary() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0;
}
//----< compare names alphabetically >---------------------------------

bool FileInfo::operator<(const FileInfo& fi) const
{
	return strcmp(data.cFileName, fi.data.cFileName) == -1;
}
//----< compare names alphabetically >---------------------------------

bool FileInfo::operator==(const FileInfo& fi) const
{
	return strcmp(data.cFileName, fi.data.cFileName) == 0;
}
//----< compare names alphabetically >---------------------------------

bool FileInfo::operator>(const FileInfo& fi) const
{
	return strcmp(data.cFileName, fi.data.cFileName) == 1;
}
//----< compare file times >-------------------------------------------

bool FileInfo::earlier(const FileInfo& fi) const
{
	FILETIME ft1 = data.ftLastWriteTime;
	FILETIME ft2 = fi.data.ftLastWriteTime;
	return ::CompareFileTime(&ft1, &ft2) == -1;
}
//----< compare file times >-------------------------------------------

bool FileInfo::later(const FileInfo& fi) const
{
	FILETIME ft1 = data.ftLastWriteTime;
	FILETIME ft2 = fi.data.ftLastWriteTime;
	return ::CompareFileTime(&ft1, &ft2) == 1;
}
//----< smaller >------------------------------------------------------

bool FileInfo::smaller(const FileInfo &fi) const
{
	return size() < fi.size();
}
//----< larger >-------------------------------------------------------

bool FileInfo::larger(const FileInfo &fi) const
{
	return size() > fi.size();
}
//----< convert string to lower case chars >---------------------------

std::string Path::toLower(const std::string& src)
{
	std::string temp;
	for (size_t i = 0; i<src.length(); ++i)
		temp += tolower(src[i]);
	return temp;
}
//----< convert string to upper case chars >---------------------------


std::string Path::toUpper(const std::string& src)
{
	std::string temp;
	for (size_t i = 0; i<src.length(); ++i)
		temp += toupper(src[i]);
	return temp;
}
//----< get path from fileSpec >---------------------------------------

std::string Path::getName(const std::string &fileSpec, bool withExt)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
	{
		pos = fileSpec.find_last_of("\\");
		if (pos >= fileSpec.length())
		{
			// no path prepended
			if (withExt)
				return fileSpec;
			else
			{
				// remove ext
				size_t pos = fileSpec.find(".");
				if (pos > fileSpec.size())
					return fileSpec;
				return fileSpec.substr(0, pos - 1);
			}
		}
	}
	if (withExt)
		return fileSpec.substr(pos + 1, fileSpec.length() - pos);
	else
	{
		// remove ext
		size_t pos2 = fileSpec.find(".", pos);
		if (pos2 > fileSpec.size())
			// no ext
			return fileSpec.substr(pos + 1);
		return fileSpec.substr(pos + 1, pos2 - pos - 1);
	}
}
//----< get extension from fileSpec >----------------------------------

std::string Path::getExt(const std::string& fileSpec)
{
	size_t pos1 = fileSpec.find_last_of('/');
	size_t pos2 = fileSpec.find_last_of('\\');
	size_t pos = fileSpec.find_last_of('.');
	// handle ../ or ..\\ with no extension
	if (pos1 < fileSpec.length() || pos2 < fileSpec.length())
	{
		if (pos < min(pos1, pos2))
			return std::string("");
	}
	// only . is extension delimiter
	if (0 <= pos && pos < fileSpec.length())
		return toLower(fileSpec.substr(pos + 1, fileSpec.length() - pos));
	return std::string("");
}
//----< get path from fileSpec >---------------------------------------

std::string Path::getPath(const std::string &fileSpec)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
		pos = fileSpec.find_last_of("\\");
	if (pos >= fileSpec.length())
		return ".";
	if (fileSpec.find(".", pos + 1))
		return fileSpec.substr(0, pos + 1);
	return fileSpec;
}
//----< get absoluth path from fileSpec >------------------------------

std::string Path::getFullFileSpec(const std::string &fileSpec)
{
	const size_t BufSize = 256;
	char buffer[BufSize];
	char filebuffer[BufSize];  // don't use but GetFullPathName will
	char* name = filebuffer;
	::GetFullPathNameA(fileSpec.c_str(), BufSize, buffer, &name);
	return std::string(buffer);
}
//----< create file spec from path and name >--------------------------

std::string Path::fileSpec(const std::string &path, const std::string &name)
{
	std::string fs;
	size_t len = path.size();
	if (path[len - 1] == '/' || path[len - 1] == '\\')
		fs = path + name;
	else
	{
		if (path.find("/") < path.size())
			fs = path + "/" + name;
		else if (path.find("\\") < path.size())
			fs = path + "\\" + name;
		else
			fs = path + "/" + name;
	}
	return fs;
}
//----< return name of the current directory >-----------------------------

std::string Directory::getCurrentDirectory()
{
	char buffer[MAX_PATH];
	::GetCurrentDirectoryA(MAX_PATH, buffer);
	return std::string(buffer);
}
//----< change the current directory to path >-----------------------------

bool Directory::setCurrentDirectory(const std::string& path)
{
	return ::SetCurrentDirectoryA(path.c_str()) != 0;
}
//----< get names of all the files matching pattern (path:name) >----------

std::vector<std::string> Directory::getFiles(const std::string& path, const std::string& pattern)
{
	std::vector<std::string> files;
	FileSystemSearch fss;
	std::string file = fss.firstFile(path, pattern);
	if (file.size() == 0)
		return files;
	files.push_back(file);
	while (true)
	{
		file = fss.nextFile();
		if (file.size() == 0)
			return files;
		files.push_back(file);
	}
	return files;
}
//----< get names of all directories matching pattern (path:name) >--------

std::vector<std::string> Directory::getDirectories(const std::string& path, const std::string& pattern)
{
	std::vector<std::string> dirs;
	FileSystemSearch fss;
	std::string dir = fss.firstDirectory(path, pattern);
	if (dir.size() == 0)
		return dirs;
	dirs.push_back(dir);
	while (true)
	{
		dir = fss.nextDirectory();
		if (dir.size() == 0)
			return dirs;
		dirs.push_back(dir);
	}
	return dirs;
}
//----< create directory >-------------------------------------------------

bool Directory::create(const std::string& path)
{
	return ::CreateDirectoryA(path.c_str(), NULL) == 0;
}
//----< does directory exist? >--------------------------------------------

bool Directory::exists(const std::string& path)
{
	DWORD dwAttrib = GetFileAttributesA(path.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
//----< remove directory >-------------------------------------------------

bool Directory::remove(const std::string& path)
{
	return ::RemoveDirectoryA(path.c_str()) == 0;
}
//----< find first file >--------------------------------------------------

std::string FileSystemSearch::firstFile(const std::string& path, const std::string& pattern)
{
	hFindFile = ::FindFirstFileA(Path::fileSpec(path, pattern).c_str(), pFindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return pFindFileData->cFileName;
		else
		while (::FindNextFileA(hFindFile, pFindFileData))
		if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return pFindFileData->cFileName;
	}
	return "";
}
//----< find next file >---------------------------------------------------

std::string FileSystemSearch::nextFile()
{
	while (::FindNextFileA(hFindFile, pFindFileData))
	if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		return pFindFileData->cFileName;
	return "";
}
//----< find first file >--------------------------------------------------

std::string FileSystemSearch::firstDirectory(const std::string& path, const std::string& pattern)
{
	hFindFile = ::FindFirstFileA(Path::fileSpec(path, pattern).c_str(), pFindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			return pFindFileData->cFileName;
		else
		while (::FindNextFileA(hFindFile, pFindFileData))
		if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			return pFindFileData->cFileName;
	}
	return "";
}
//----< find next file >---------------------------------------------------

std::string FileSystemSearch::nextDirectory()
{
	while (::FindNextFileA(hFindFile, pFindFileData))
	if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return pFindFileData->cFileName;
	return "";
}
//----< test stub >--------------------------------------------------------

#ifdef TEST_FILESYSTEM

void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}
int main(int argc, char* argv[])
{
	title("Demonstrate Path Class");

	std::string fs = Path::fileSpec("../../", "temp.txt");
	std::cout << "\n  Path::fileSpec(\".\",\"temp.txt\") = " << fs;

	std::string path = Path::getPath(fs);
	std::cout << "\n  Path::getPath(\"" + fs + "\") = " << path;

	std::string ffs = Path::getFullFileSpec(fs);
	std::cout << "\n  Path::getFullFileSpec(\"" + fs + "\") = " << ffs;

	std::string name = Path::getName(fs);
	std::cout << "\n  Path::getName(\"" + fs + "\") = " << name;

	std::string ext = Path::getExt(fs);
	std::cout << "\n  Path::getExt(\"" + fs + "\") = " << ext;

	std::string upper = Path::toUpper("temp.txt");
	std::cout << "\n  Path::toUpper(\"temp.txt\") = " << upper;

	std::string lower = Path::toLower("Temp.Txt");
	std::cout << "\n  Path::toLower(\"Temp.Txt\") = " << lower;
	std::cout << std::endl;
	/*
	title("Demonstrate FileSystemSearch class");

	const size_t PathSetSize = 2;
	std::string home = ::getenv("HOMEDRIVE");
	std::string pathSet[] = { home+"\\\\", "." };
	FileSystemSearch fss;
	for(size_t i = 0; i<PathSetSize; ++i)
	{
	std::cout << "\n  searching for files on \"" << pathSet[i] << "\"";
	std::cout << "\n " << std::string(27 + pathSet[i].size(), '-');
	std::string searchPath = pathSet[i];
	std::string fileName = fss.firstFile(searchPath);
	if(fileName.size() > 0)
	std::cout << "\n  " << fileName;
	else
	std::cout << "\n  no files match search";
	while(true) {
	fileName = fss.nextFile();
	if(fileName.size() > 0)
	std::cout << "\n  " << fileName;
	else
	break;
	}
	std::cout << std::endl;

	std::cout << "\n  searching for directories on \"" << pathSet[i] << "\"";
	std::cout << "\n " << std::string(33 + pathSet[i].size(), '-');
	std::string dirName = fss.firstDirectory(searchPath);
	if(dirName.size() > 0)
	std::cout << "\n  " << dirName;
	else
	std::cout << "\n  no directories match search";
	while(true)
	{
	dirName = fss.nextDirectory();
	if(dirName.size() > 0)
	std::cout << "\n  " << dirName;
	else
	break;
	}
	std::cout << std::endl;
	}
	*/
	title("Demonstrate Directory class");

	// Display contents of current directory

	std::cout << "\n  current directory is:\n    " << Directory::getCurrentDirectory();
	std::cout << "\n  It contains files:";
	std::vector<std::string> currfiles = Directory::getFiles();
	///////////////////////////////////////////////////////
	// This works too
	// std::vector<std::string> currfiles = d.getFiles();  
	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << currfiles[i].c_str();
	std::cout << "\n  and contains directories:";
	std::vector<std::string> currdirs = Directory::getDirectories();
	for (size_t i = 0; i<currdirs.size(); ++i)
		std::cout << "\n    " << currdirs[i].c_str();
	std::cout << "\n";

	// Display contents of non-current directory

	std::cout << "\n  .txt files residing in C:/temp are:";
	currfiles = Directory::getFiles("c:/temp/", "*.txt");
	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << currfiles[i].c_str();
	std::cout << "\n";
	std::cout << "\n  directories residing in C:/temp are:";
	currdirs = Directory::getDirectories("c:/temp/");
	for (size_t i = 0; i<currdirs.size(); ++i)
		std::cout << "\n    " << currdirs[i].c_str();
	std::cout << "\n";

	// Create directory

	title("Demonstrate FileInfo Class Operations", '=');
	std::cout << "\n";

	Directory::setCurrentDirectory(".");
	std::cout << "\n  current path is \"" << Directory::getCurrentDirectory();

	std::string fn1;
	if (argc > 1)
		fn1 = argv[1];
	else
		fn1 = "c:\\temp\\test.txt";
	FileInfo fi(fn1);

	if (fi.good())
	{
		std::cout << "\n  name: " << "\t" << fi.name();
		std::cout << "\n  date: " << "\t" << fi.date();
		std::cout << "\n  date: " << "\t" << fi.date(FileInfo::dateformat);
		std::cout << "\n  date: " << "\t" << fi.date(FileInfo::timeformat);
		std::cout << "\n  size: " << "\t" << fi.size() << " bytes";
		if (fi.isArchive())
			std::cout << "\n  is archive";
		else
			std::cout << "\n  is not archive";
		if (fi.isCompressed())
			std::cout << "\n  is compressed";
		else
			std::cout << "\n  is not compressed";
		if (fi.isDirectory())
			std::cout << "\n  is directory";
		else
			std::cout << "\n  is not directory";
		if (fi.isEncrypted())
			std::cout << "\n  is encrypted";
		else
			std::cout << "\n  is not encrypted";
		if (fi.isHidden())
			std::cout << "\n  is hidden";
		else
			std::cout << "\n  is not hidden";
		if (fi.isNormal())
			std::cout << "\n  is normal";
		else
			std::cout << "\n  is not normal";
		if (fi.isOffLine())
			std::cout << "\n  is offline";
		else
			std::cout << "\n  is not offline";
		if (fi.isReadOnly())
			std::cout << "\n  is readonly";
		else
			std::cout << "\n  is not readonly";
		if (fi.isSystem())
			std::cout << "\n  is system";
		else
			std::cout << "\n  is not system";
		if (fi.isTemporary())
			std::cout << "\n  is temporary";
		else
			std::cout << "\n  is not temporary";
	}
	else
		std::cout << "\n  filename " << fn1 << " is not valid in this context\n";

	std::string fn2;
	if (argc > 2)
	{
		fn1 = argv[1];
		fn2 = argv[2];
	}
	else
	{
		fn1 = "FileSystem.h";
		fn2 = "FileSystem.cpp";
	}
	FileInfo fi1(fn1);
	FileInfo fi2(fn2);
	if (fi1.good() && fi2.good())
	{
		if (fi1 == fi1)
			std::cout << "\n  " << fi1.name() << " == " << fi1.name();
		else
			std::cout << "\n  " << fi1.name() << " != " << fi1.name();
		if (fi1 < fi1)
			std::cout << "\n  " << fi1.name() << " < " << fi1.name();
		else
			std::cout << "\n  " << fi1.name() << " >= " << fi1.name();
		if (fi1 == fi2)
			std::cout << "\n  " << fi1.name() << " == " << fi2.name();
		else
			std::cout << "\n  " << fi1.name() << " != " << fi2.name();
		if (fi1 < fi2)
			std::cout << "\n  " << fi1.name() << " < " << fi2.name();
		else
			std::cout << "\n  " << fi1.name() << " >= " << fi2.name();
		if (fi1.smaller(fi2))
			std::cout << "\n  " << fi1.name() << " is smaller than " << fi2.name();
		else
			std::cout << "\n  " << fi1.name() << " is not smaller than " << fi2.name();
		if (fi1.earlier(fi2))
			std::cout << "\n  " << fi1.name() << " is earlier than " << fi2.name();
		else
			std::cout << "\n  " << fi1.name() << " is not earlier than " << fi2.name();
		std::cout << std::endl;
	}
	else
		std::cout << "\n  files " << fn1 << " and " << fn2 << " are not valid in this context\n";

	title("Demonstrate File class operations", '=');
	std::cout << "\n";

	// copy binary file from one directory to another

	File me("../debug/filesystemdemo.exe");
	me.open(File::in, File::binary);
	std::cout << "\n  copying " << me.name().c_str() << " to c:/temp";
	if (!me.isGood())
	{
		std::cout << "\n  can't open executable\n";
		std::cout << "\n  looking for:\n  ";
		std::cout << Path::getFullFileSpec(me.name()) << "\n";
	}
	else
	{
		File you("c:/temp/fileSystemdemo.exe");
		you.open(File::out, File::binary);
		if (you.isGood())
		{
			while (me.isGood())
			{
				Block b = me.getBlock(1024);
				std::cout << "\n    reading block of " << b.size() << " bytes";
				you.putBlock(b);
				std::cout << "\n    writing block of " << b.size() << " bytes";
			}
			std::cout << "\n";
		}
	}

	// save some filespecs of text files in a vector for File demonstrations

	std::vector<std::string> files;
	if (argc == 1)
	{
		std::cout << "\n\n  Please enter, on the command line, a filename to process.\n";
	}

	for (int i = 1; i<argc; ++i)
	{
		files.push_back(argv[i]);
	}
	files.push_back("FileSystem.cpp");
	files.push_back("../FileSystemDemo/FileSystem.cpp"); // file from project directory
	files.push_back("../test.txt");  // file in executable's debug directory
	files.push_back("foobar");             // doesn't exist

	// open each file and display a few lines of text

	for (size_t i = 0; i<files.size(); ++i)
	{
		File file(files[i]);
		file.open(File::in);
		if (!file.isGood())
		{
			std::cout << "\n  Can't open file " << file.name();
			std::cout << "\n  Here's what the program can't find:\n  " << Path::getFullFileSpec(file.name());
			continue;
		}
		std::string temp = std::string("Processing file ") + files[i];
		title(temp, '-');
		for (int j = 0; j<10; ++j)
		{
			if (!file.isGood())
				break;
			std::cout << "\n  " << file.getLine().c_str();
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	// read text file and write to another text file

	title("writing to c:/temp/test.txt", '-');
	File in("../test.txt");
	in.open(File::in);
	File out("c:/temp/test.txt");
	out.open(File::out);
	while (in.isGood())
	{
		std::string temp = in.getLine();
		std::cout << "\n  " << temp.c_str();
		out.putLine(temp);
		out.putLine("\n");
	}
	std::cout << "\n\n";
}
#endif
