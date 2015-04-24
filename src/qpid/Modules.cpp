/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

#include "config.h"
#include "qpid/Modules.h"
#include "qpid/Exception.h"
#include "qpid/log/Statement.h"
#include "qpid/sys/Shlib.h"
<<<<<<< HEAD

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace fs=boost::filesystem;
=======
#include "qpid/sys/FileSysDir.h"
>>>>>>> 3bbfc42... Imported Upstream version 0.32

namespace {

// CMake sets QPID_MODULE_SUFFIX; Autoconf doesn't, so assume Linux .so
#ifndef QPID_MODULE_SUFFIX
#define QPID_MODULE_SUFFIX ".so"
#endif

inline std::string& suffix() {
    static std::string s(QPID_MODULE_SUFFIX);
    return s;
}

bool isShlibName(const std::string& name) {
<<<<<<< HEAD
    return name.find (suffix()) == name.length() - suffix().length();
=======
    return name.substr(name.size()-suffix().size()) == suffix();
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

}

namespace qpid {

ModuleOptions::ModuleOptions(const std::string& defaultModuleDir)
    : qpid::Options("Module options"), loadDir(defaultModuleDir), noLoad(false)
{
    addOptions()
        ("module-dir",    optValue(loadDir, "DIR"),  "Load all shareable modules in this directory")
        ("load-module",   optValue(load,    "FILE"), "Specifies additional module(s) to be loaded")
        ("no-module-dir", optValue(noLoad),          "Don't load modules from module directory");
}

<<<<<<< HEAD
void tryShlib(const char* libname_, bool noThrow) {
    std::string libname(libname_);
    if (!isShlibName(libname)) libname += suffix();
    try {
        sys::Shlib shlib(libname);
    }
    catch (const std::exception& /*e*/) {
        if (!noThrow)
            throw;
    }
}

void loadModuleDir (std::string dirname, bool isDefault)
{
    fs::path dirPath (dirname, fs::native);

    if (!fs::exists (dirPath))
    {
        if (isDefault)
            return;
        throw Exception ("Directory not found: " + dirname);
    }
    if (!fs::is_directory(dirPath))
    {
        throw Exception ("Invalid value for module-dir: " + dirname + " is not a directory");
    }

    fs::directory_iterator endItr;
    for (fs::directory_iterator itr (dirPath); itr != endItr; ++itr)
    {
        if (!fs::is_directory(*itr) && isShlibName(itr->string()))
            tryShlib (itr->string().data(), true);
    }
=======
void tryShlib(const std::string& libname) {
    sys::Shlib shlib( isShlibName(libname) ? libname : (libname + suffix()));
}

namespace {

void tryOnlyShlib(const std::string& libname) throw() {
    try {
        if (isShlibName(libname)) sys::Shlib shlib( libname );
    }
    catch (const std::exception& /*e*/) {
    }
}

}

void loadModuleDir (std::string dirname, bool isDefault)
{

    sys::FileSysDir dirPath (dirname);

    bool exists;
    try
    {
        exists = dirPath.exists();
    } catch (Exception& e) {
        throw Exception ("Invalid value for module-dir: " + e.getMessage());
    }
    if (!exists) {
        if (isDefault) return;
        throw Exception ("Directory not found: " + dirname);
    }

    dirPath.forEachFile(&tryOnlyShlib);
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

} // namespace qpid
