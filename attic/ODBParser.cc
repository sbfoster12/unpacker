#include "ODBParser.hh"
#include <boost/property_tree/xml_parser.hpp>
#include <sstream>
#include <iostream>

namespace dataProducts {

void ODBParser::ParseODB(const std::string& odbString) {
  std::stringstream xmlstream(odbString);
  read_xml(xmlstream, odbTree_);

  odbTree_ = odbTree_.get_child("odb");
}

const boost::property_tree::ptree* ODBParser::GetSingleDirectory(
    const boost::property_tree::ptree& pt, const std::string& dirname) {
  const boost::property_tree::ptree* returnPtr = nullptr;

  for (const auto& child : pt) {
    const auto& dirOptional =
        child.second.get_optional<std::string>("<xmlattr>.name");

    if (dirOptional && (dirOptional.get() == dirname)) {
      returnPtr = &child.second;
      break;
    }
  }

  if (!returnPtr) {
     std::cout << "sub directory " << dirname << " not found!\n" << std::endl;
     exit(1);
  }

  return returnPtr;
}

const boost::property_tree::ptree* ODBParser::GetDirectory(
    const boost::property_tree::ptree& pt, const dirlist& dirnames) {
  const boost::property_tree::ptree* returnPtr = &pt;
  for (const auto& dirname : dirnames.heldList) {
    if (returnPtr) {
      returnPtr = GetSingleDirectory(*returnPtr, dirname);
    }
  }
  return returnPtr;
}

ODBParser::stringlist ODBParser::GetDirectoryContents(
    const boost::property_tree::ptree& pt) {
  stringlist outDirs;

  for (const auto& child : pt) {
    const auto& dirOptional =
        child.second.get_optional<std::string>("<xmlattr>.name");
    if (dirOptional) {
      outDirs.push_back(dirOptional.get());
    }
  }

  return outDirs;
}

std::vector<const boost::property_tree::ptree*> ODBParser::GetSubDirectories(
    const boost::property_tree::ptree& pt, const std::string& dirNameExpr,
    bool exactMatch) {
  std::vector<const boost::property_tree::ptree*> returnVect;

  // Loop over subdirs
  for (const auto& child : pt) {
    // Get dir name, handling case where no name exists
    const auto& dirOptional =
        child.second.get_optional<std::string>("<xmlattr>.name");

    if (dirOptional) {
      const auto& dirName = dirOptional.get();

      // Check if name matches expression
      bool nameMatches = exactMatch
                             ? (dirName == dirNameExpr)
                             : (dirName.find(dirNameExpr) != std::string::npos);

      // If matches, add to output vector
      if (nameMatches) {
        returnVect.push_back(&child.second);
      }
    }
  }

  // Check found something
  if (returnVect.size() == 0) {
    std::cout
        << "No sub directories "
        << std::string(exactMatch ? "called "
                                  : "with names matching expression ")
        << dirNameExpr << " found!\n" << std::endl;
  }

  return returnVect;
}

std::string ODBParser::GetDirectoryName(const boost::property_tree::ptree& pt) {
  const auto& dirOptional = pt.get_optional<std::string>("<xmlattr>.name");

  if (dirOptional) {
    return dirOptional.get();
  } else {
    std::cout
        << "No name found for directory!\n";
  }

  // Won't get here
  return "";
}

// Shamelessly stolen:
// http://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<std::string> ODBParser::tokenize(std::string s, std::string delim) {
  std::vector<std::string> tokens;

  auto start = 0U;
  auto end = s.find(delim);
  while (end != std::string::npos) {
    tokens.push_back(s.substr(start, end - start));
    start = end + delim.length();
    end = s.find(delim, start);
  }

  tokens.push_back(s.substr(start, end));

  return tokens;
}

}  // namespace
