#include <string.h>
#include <iostream>
#include "appSettings.h"
#include "stringSplit.h"
#include "stringTrim.h"

const char* AppSettings::appName = "WARFACE-FASTZOOM";
const int AppSettings::appVersion[] = { 1, 0, 0, 0 };

void AppSettings::append(std::ostream& stream, char ch) {
	stream.write(&ch, std::streamsize(1));
}

void AppSettings::append(std::ostream& stream, std::string str) {
	stream.write(str.c_str(), str.length());
}

void AppSettings::append(std::ostream& stream, const char* str) {
	stream.write(str, strlen(str));
}

void AppSettings::append(std::ostream& stream, int num) {
	append(stream, std::to_string(num));
}

void AppSettings::appendLine(std::ostream& stream) {
	append(stream, '\n');
}

struct AppSettings::Settings AppSettings::Settings::ProvideDefault() {
	struct AppSettings::Settings s;
	s.binds[0].name = "Disabled";
	s.binds[0].delay = 0;

	s.binds[1].name = "Steyr Scout";
	s.binds[1].delay = 80;

	s.binds[2].name = "SAP6";
	s.binds[2].delay = 50;

	s.binds[3].name = "CheyTac M200";
	s.binds[3].delay = 200;

	return s;
}

void AppSettings::Serialize(std::ostream& toStream, struct Settings settings) {
	toStream.clear();
	append(toStream, '$');
	append(toStream, appName);
	append(toStream, '$');
	append(toStream, appVersion[0]);
	append(toStream, '.');
    append(toStream, appVersion[1]);
	append(toStream, '.');
    append(toStream, appVersion[2]);
	append(toStream, '.');
    append(toStream, appVersion[3]);
	
	appendLine(toStream);
	appendLine(toStream);

	append(toStream, "[BINDS]");
	appendLine(toStream);

	for (int i = 0; i < bindsCount; i++) {
		append(toStream, "NUMPAD_");
		append(toStream, i);
		append(toStream, "_NAME");
		append(toStream, '=');
		append(toStream, settings.binds[i].name);
		appendLine(toStream);

		append(toStream, "NUMPAD_");
		append(toStream, i);
		append(toStream, "_DELAY");
		append(toStream, '=');
		append(toStream, (int)settings.binds[i].delay);
		appendLine(toStream);
		appendLine(toStream);
	}
}

bool AppSettings::tryReadHeader(std::string in, std::string& out) {
	int size = in.size();
	if (in.rfind('[', 0) != 0)
		return false;

	int closeBracket = in.find_last_of(']');
	if (closeBracket == -1)
		return false;
	
	out = in.substr(1, closeBracket - 1);
	return true;
}

bool AppSettings::tryReadKeyValuePair(std::string in, std::string& outKey, std::string& outValue) {
	std::vector<std::string> pair = stringSplit(in, '=');
	if (pair.size() != 2)
		return false;

	outKey = trim_copy(pair[0]);
	outValue = trim_copy(pair[1]);
	return true;
}

struct AppSettings::Settings AppSettings::Deserialize(std::istream& fromStream) {
	std::string s;

	{ // Parsing First Line
		std::getline(fromStream, s);
		std::vector<std::string> values = stringSplit(s, '$');
		if (values.size() == 0 || values[0].compare(appName) != 0)
			throw Exceptions::UnsupportedAppName();

		std::vector<std::string> versionValues = stringSplit(values[1], '.');
		if (versionValues.size() != 4)
			throw Exceptions::InvalidAppVersion();

		int version[4];
		for (int i = 0; i < 4; i++)
			version[i] = std::stoi(versionValues[i]);

		for (int i = 0; i < 4; i++)
			if (version[i] > appVersion[i])
				throw Exceptions::UnsupportedAppVersion();
	}
	
	struct Settings data;
	std::string c_header; // c_ = Current
	std::string c_key;
	std::string c_value;
	while (fromStream) {
		std::getline(fromStream, s);
		trim(s);

		if (tryReadHeader(s, c_header))
			continue;

		if (!tryReadKeyValuePair(s, c_key, c_value))
			continue;

		if (c_header == "BINDS") {
			const std::string prefix = "NUMPAD_";
			const std::string suffix1 = "_NAME";
			const std::string suffix2 = "_DELAY";
			if (c_key.compare(0, prefix.size(), prefix) == 0) {
				if (c_key.compare(c_key.size() - suffix1.size(), suffix1.size(), suffix1) == 0) // If ends with `suffix1`
					data.binds[stoi(c_key.substr(prefix.size(), 1))].name = c_value;
				else if (c_key.compare(c_key.size() - suffix2.size(), suffix2.size(), suffix2) == 0) // If ends with `suffix2`
					data.binds[stoi(c_key.substr(prefix.size(), 1))].delay = std::stoi(c_value);
			}
		}
	}

	return data;
}