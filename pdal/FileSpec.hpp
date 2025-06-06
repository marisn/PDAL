/******************************************************************************
* Copyright (c) 2025, Hobu Inc.
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. nor the names of its contributors
*       may be used to endorse or promote products derived from this
*       software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#pragma once

#include <filesystem>

#include <pdal/PDALUtils.hpp>
#include <pdal/pdal_types.hpp>

#include <pdal/JsonFwd.hpp>

using StringMap = std::map<std::string, std::string>;

namespace pdal
{

class FileSpec
{
public:
    FileSpec()
    {}

    bool valid() const
    { return !m_path.empty(); }
    bool onlyFilename() const
    { return m_headers.empty() && m_query.empty(); }
    Utils::StatusWithReason parse(NL::json& json);
    // parse a user input string that could be a json spec or filename
    Utils::StatusWithReason ingest(const std::string& pathOrJson);

    friend std::ostream& operator << (std::ostream& out, const FileSpec& spec);

private:
    Utils::StatusWithReason extractPath(NL::json& node);
    Utils::StatusWithReason extractQuery(NL::json& node);
    Utils::StatusWithReason extractHeaders(NL::json& node);

public:
    std::filesystem::path m_path;
    StringMap m_headers;
    StringMap m_query;
};

namespace Utils
{
    template<>
    inline StatusWithReason fromString(const std::string& s, FileSpec& spec)
    {
        return spec.ingest(s);
    }
}

} // namespace pdal

