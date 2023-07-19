#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

template <typename T>
T toNumber(const std::string& str) {
    T result;
    std::istringstream iss(str);
    iss >> result;
    return result;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string Response::_parseHeader( const std::string& line )  {
	std::string _key, _value;
	size_t _end = 0;

	std::string _line = trim(line);
	// key
	while (_end < _line.length() && _line[_end] != ':')	++_end;
	_key =  trim(_line.substr(0, _end));

	// value
	if (_end < _line.length())
		_value = trim(_line.substr(++_end));
	if (_key == "X_POWRED_PY:" || key == "Status:") {
		if (_key == "Status:")
			setStatusCode(toNumber<int>(_value));
		return ("");
	}

	return std::string(_key + " " + _value);
}

void Response::parseCgiOutput( const std::string& cgioutput) {
	std::string _line;
	size_t _seek = 0;

	std::ifstream ifile(cgioutput.c_str(), std::ifstream::in | std::ifstream::binary);

	if (!ifile.is_open())
		return std::make_pair(false, r);

	while (std::getline(ifile, _line)) {
		_seek += _line.length() + 1;
		_line = trim(_line);
		if (_line.empty()) break;
		std::string _header = _parseHeader(_line);
		if (!_header.empty())
			setHeader(_header);
	}


	// std::string _status = r.getHeader(H_STATUS);
	// if (!_status.empty()) {
	// 	r.setStatusCode(toNumber<int>(trim(_status)));
	// 	r.setStatus(getReason(r.getStatusCode()));
	// }
	// r.removeHeader(H_STATUS);
	// r.removeHeader(H_X_POWERED_BY);

	if (ifile.eof()) {
		ifile.close();
		// remove(filePath.c_str());
		setHeader("Content-Lenth: 0");
		throw(getStatusCode());
	}

	ifile.close();
	r.setFilePath(filePath);
	size_t _contentLength = !r.getFilePath().empty() ? FileHandler::getFileSize(r.getFilePath()) : r.getBody().size();
	r.addHeader(H_CONTENT_LENGTH, toString<size_t>(_contentLength > 0 ? (_contentLength - _seek) : 0));
	if (!r.setupFile())
		return std::make_pair(false, r);
	r.getFile()->seekg(_seek, std::fstream::beg);
	r.setBuffered(true);
	r.setFromCGI(true);
	return std::make_pair(true, r);
}
