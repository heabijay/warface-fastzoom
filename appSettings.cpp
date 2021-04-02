#include <string.h>
#include "appSettings.h"

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
	s.binds[0].name = "Steyr Scout";
	s.binds[0].delay = 80;

	s.binds[1].name = "SAP6";
	s.binds[1].delay = 50;

	s.binds[2].name = "CheyTac M200";
	s.binds[2].delay = 200;

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
		append(toStream, i+1);
		append(toStream, "_NAME");
		append(toStream, '=');
		append(toStream, settings.binds[i].name);
		appendLine(toStream);

		append(toStream, "NUMPAD_");
		append(toStream, i+1);
		append(toStream, "_DELAY");
		append(toStream, '=');
		append(toStream, (int)settings.binds[i].delay);
		appendLine(toStream);
		appendLine(toStream);
	}
}