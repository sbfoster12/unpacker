/**
 * Class for helping to parse the odb xml string
 *
 * Taken from here: https://cdcvs.fnal.gov/redmine/projects/gm2util/repository/revisions/develop/entry/parser/ODBParser.hh
 */

#ifndef ODBPARSER_HH
#define ODBPARSER_HH

#include <vector>
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>

namespace dataProducts {
class ODBParser {
 public:
  typedef std::pair<int, int> intpair;
  typedef std::vector<std::string> stringlist;

  class dirlist {
   public:
    dirlist() {}

    typedef std::vector<std::string> stringlist;
    dirlist(const stringlist& inList) : heldList(inList) {}

    // variadic constructor, tries to interpret every arg as a string
    template <typename... Args>
    dirlist(Args... args)
        : heldList(std::initializer_list<std::string>{args...}) {}
    stringlist heldList;
  };

  // default constructor doesn't do much
  ODBParser() : odbTree_() {}

  explicit ODBParser(const std::string& odbString) { ParseODB(odbString); }

  // parses xml odb string, stores it internally as a ptree
  void ParseODB(const std::string& odbString);

  // get an end value
  template <typename dtype>
  dtype GetValue(const dirlist& list) const {
    return GetValue<dtype>(odbTree_, list);
  }

  template <typename dtype>
  static dtype GetValue(const boost::property_tree::ptree& pt,
                        const dirlist& list) {
    auto dirptr = GetDirectory(pt, list);
    return dirptr->get_value<dtype>();
  }

  // get pointer to subtree correspoding to dirname, exception if not found
  const boost::property_tree::ptree* GetDirectory(
      const dirlist& dirnames) const {
    return GetDirectory(odbTree_, dirnames);
  }

  // get pointer to subtree corresponding to dirname[0]/dirname[1]/...
  // exception if not found
  static const boost::property_tree::ptree* GetDirectory(
      const boost::property_tree::ptree& pt, const dirlist& dirnames);

  // returns contents of base directory in odb
  stringlist GetODBContents() const { return GetDirectoryContents(odbTree_); }

  // returns contents of the directory represented by pt
  static stringlist GetDirectoryContents(const boost::property_tree::ptree& pt);

  // get vector of subdirs matching name specified
  // if exactMatch == true (default), subdir name must exactly match expression,
  // otherwise substring match is fine
  static std::vector<const boost::property_tree::ptree*> GetSubDirectories(
      const boost::property_tree::ptree& pt, const std::string& dirNameExpr,
      bool exactMatch = true);

  // get the name for the provided directory
  static std::string GetDirectoryName(const boost::property_tree::ptree& pt);

  // get arrays from odb
  template <typename dtype>
  std::vector<dtype> GetVector(const dirlist& list) const {
    return GetVector<dtype>(odbTree_, list);
  }

  template <typename dtype>
  static std::vector<dtype> GetVector(const boost::property_tree::ptree& pt,
                                      const dirlist& list) {
    auto dirptr = GetDirectory(pt, list);

    auto vecSize = dirptr->get<unsigned int>("<xmlattr>.num_values");
    std::vector<dtype> outvec(vecSize);

    for (const auto& child : *dirptr) {
      if (auto indexOpt =
              child.second.get_optional<unsigned int>("<xmlattr>.index")) {
        outvec[*indexOpt] = child.second.get_value<dtype>();
      }
    }

    return outvec;
  }

  // generic tokeniser
  static std::vector<std::string> tokenize(std::string s, std::string delim);

 private:
  boost::property_tree::ptree odbTree_;

  static const boost::property_tree::ptree* GetSingleDirectory(
      const boost::property_tree::ptree& pt, const std::string& dirname);
};

}  // namespace

#endif
