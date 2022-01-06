#ifndef _READCSVDATA_HPP_
#define _READCSVDATA_HPP_

class Read{
public:
	std::vector<std::string> readVidIds(std::string filePath);

	std::vector<std::string> readVidNames(std::string filePath);

	std::unordered_map<std::string, std::vector<int>> readProfileRatings(std::string filePath);
};

#endif //_READCSVDATA_HPP_