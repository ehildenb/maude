#include <cstddef>
#include "macros.hh"
#include "vector.hh"
#include "directoryManager.hh"

//// XXX": Fix
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
#include "term.hh"
#include "lineNumber.hh"
#include "IO_Manager.hh"
#include "token.hh"
#include "userLevelRewritingContext.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
#include "mixfixModule.hh"
#include "interpreter.hh"
#include "global.hh"
#include "interpreter.hh"
#include "global.hh"

#include <cassert>

class FakeDirectoryManager
    : public AbstractDirectoryManager
{
public:
    void setCwd(string const& dir) { cwd = dir; }
    void setExecutableDir(string const& dir) { executableDirectory = dir; }
    void setMaudeLib(string const& path) {
        // TODO: Change realPath and searchPath to not depend on cwd / MAUDE_LIB
        setenv("MAUDE_LIB", path.c_str(), 1);
    }
    void addFile(string const& file) { files.append(file); }

    bool checkAccess(const string& directory,
  	       string& fileName,
           int mode,
           char const* const ext[] = 0)
    {
        string path = directory;
        if (directory.empty() || *directory.rbegin() != '/') path += "/";
        if (path.at(0) == '.' && path.at(1) == '/')
            path.replace(0, 1, getCwd());
        path += fileName;
        path += ".maude";
        // Sanity checks: Unit tests don't support non-canonical paths.
        assert(path.find("//")   == string::npos);
        assert(path.find("/../") == string::npos);
        assert(path.find("/./")  == string::npos);
        for (auto const& f : files) {
            if (f == path) {
                fileName = fileName + ".maude";
                return true;
            }
        }
        return false;
    }

    const char* getCwd() {
        return cwd.c_str();
    }

private:
    string cwd;
    string executableDirectory;
    Vector<string> files;
};


bool checkTrue(bool b)
{
    if (!b) std::cerr << "FAILED" << std::endl;
    return b;
}

bool checkEqual(string const& actual, string const& expected)
{
    if (actual == expected) return true;
    std::cerr << "FAILED: '" << actual << "' != '" << expected << "'" << std::endl;
    return false;
}

bool checkFindFile(AbstractDirectoryManager& dm, string const& file, string const& expectedDir, string const& expectedFile)
{
    string resolvedDir, resolvedFile;
    if (!checkTrue(dm.findFile(file, resolvedDir, resolvedFile, 0))) return false;
    bool result = true;
    result &= checkEqual(expectedDir,  resolvedDir);
    result &= checkEqual(expectedFile, resolvedFile);
    return result;
}

int main() {
    FakeDirectoryManager dm;
    dm.setCwd("/path/to/cwd");
    dm.setExecutableDir("/path/to/maude/exe/");
    dm.setMaudeLib("/share/maude:/lib/maude");

    dm.addFile("/foo.maude");
    dm.addFile("/other/foo.maude");
    dm.addFile("/path/to/cwd/foo.maude");
    dm.addFile("/path/to/cwd/other/foo.maude");

    bool result = true;
    result &= checkFindFile(dm, "/foo",       "/", "foo.maude");      // TODO: BUG
    result &= checkFindFile(dm, "/other/foo", "/other", "foo.maude");

    dm.addFile("/share/buzz.maude");
// We shouldn't search above components in MAUDE_LIB
    string directory, fileName;
    result &= checkTrue(!dm.findFile("../buzz.maude", directory, fileName, 0));
// but we should above cwd.
    dm.addFile("/path/to/buzz.maude");
    result &= checkFindFile(dm, "../buzz", "/path/to", "buzz.maude");

// Check that search happens in the correct order
    dm.addFile("/path/to/maude/exe/bar.maude");                      // TODO: Test suite doesn't support searching in exe dir
    result &= checkFindFile(dm, "bar", "/path/to/maude/exe", "bar.maude");
    dm.addFile("/lib/maude/bar.maude");
    result &= checkFindFile(dm, "bar", "/lib/maude", "bar.maude");
    dm.addFile("/share/maude/bar.maude");
    result &= checkFindFile(dm, "bar", "/share/maude", "bar.maude");
    dm.addFile("/path/to/cwd/bar.maude");
    result &= checkFindFile(dm, "bar", ".", "bar.maude"); // path/to/cwd is OK too

    dm.addFile("/share/maude/widget/bar.maude");
    dm.addFile("/share/maude/gadget/bar.maude");
    dm.addFile("/share/maude/bar.maude");
    result &= checkFindFile(dm, "gadget/bar", "/share/maude/gadget", "bar.maude"); // path/to/cwd is OK too

    return result ? 0 : 1;
}

// TODO: This function must be implemented by all executables
// See comment in: src/Core/lineNumber.hh
ostream& operator<<(ostream& s, const LineNumber& lineNumber)
{
  fileTable.printLineNumber(s, lineNumber.getLineNumber());
  return s;
}
