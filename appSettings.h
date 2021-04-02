#include <string>
#include <istream>
#include <ostream>

#ifndef AppSettings_H
#define AppSettings_H

class AppSettings
{
private:
    AppSettings() { }
protected:
	static const char* appName;
	static const int appVersion[4];
	static const int bindsCount = 9;

	static void append(std::ostream& stream, char ch);
	static void append(std::ostream& stream, std::string str);
	static void append(std::ostream& stream, const char* str);
	static void append(std::ostream& stream, int num);
	static void appendLine(std::ostream& stream);
public:
	struct Settings
	{
		struct WeaponBinds { 
			std::string name;
			unsigned int delay;

			WeaponBinds() {
				name = "";
				delay = 0;
			}
		} binds[bindsCount];

        static struct Settings ProvideDefault();
	};

	static void Serialize(std::ostream& toStream, struct Settings settings);
	static struct Settings Deserialize(std::ostream& fromStream);
};

#endif