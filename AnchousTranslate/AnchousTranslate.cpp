#include <iostream>
#include <tgbot/tgbot.h>
#include <map>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include <cpr/cpr.h>
#include <boost/json.hpp>

// API key and bot token 
#include "Keys.h"

using namespace TgBot;
using namespace std::chrono;

std::map<std::string, unsigned int> limits;

system_clock::time_point prevTime;

inline std::vector<std::string> Split(const std::string& str, const char delim)
{
	std::vector<std::string> out;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		out.push_back(item);
	}
	return out;
}

int main()
{
	Bot bot(Keys::TOKEN);
	prevTime = system_clock::now();

	bot.getEvents().onAnyMessage([&bot](Message::Ptr msg)
	{
		auto now = system_clock::now();
		if (duration_cast<hours>(prevTime - now).count() < 1)
			return;

		prevTime = now;
		limits.clear();
	});

	bot.getEvents().onCommand("start", [&bot](Message::Ptr msg)
	{
		bot.getApi().sendMessage(msg->chat->id, "Hi!");
	});

	bot.getEvents().onCommand("limits", [&bot](Message::Ptr msg)
	{
		std::string sender = msg->from->username;
		if (limits.find(sender) == limits.end())
			limits[sender] = 1000;

		const auto reply = boost::format("You have %1% characters left for this hour") % limits[sender];
		bot.getApi().sendMessage(msg->chat->id, reply.str());
	});

	bot.getEvents().onCommand("translate", [&bot](const Message::Ptr& msg)
	{
		std::string sender = msg->from->username;
		
		if (limits.find(sender) == limits.end())
			limits[sender] = 1000;

		const std::vector<std::string> args = Split(msg->text, ' ');

		if (args.size() < 3)
		{
			bot.getApi().sendMessage(msg->chat->id, "Wrong command format. Use this: \"/translate LANGUAGE TEXT\"");
			return;
		}
		std::string lang = args[1];
		const std::string text = msg->text.erase(0, 14);
		std::cout << text;

		const int text_size = text.length();
		if (text_size > limits[sender])
		{
			const auto reply = boost::format("You've reached text limit for this hour."
					"\nYour text has %1% chars, your limit is %2% chars.")
				% text_size
				% limits[sender];
			bot.getApi().sendMessage(msg->chat->id, reply.str());
			return;
		}

		cpr::Response request = Post(cpr::Url("https://translation.googleapis.com/language/translate/v2/"),
			cpr::Parameters{{"key", Keys::API_KEY}, {"q", text}, {"target", lang}});
		
		if(request.status_code == 400)
		{
			bot.getApi().sendMessage(msg->chat->id, "Wrong command format. Use this: \"/translate LANGUAGE TEXT\"");
			return;
		}

		auto result = boost::json::parse(request.text);

		std::stringstream reply;
		reply << result.at("data").at("translations").at(0).at("detectedSourceLanguage").as_string().c_str();
		reply <<  ": " + text;
		reply << "\n-------------\n";
		reply << lang + ": ";
		reply << result.at("data").at("translations").at(0).at("translatedText").as_string().c_str();
		
		bot.getApi().sendMessage(msg->chat->id, reply.str());

		limits[sender] = limits[sender] - text_size;
	});

	std::cout << "Anchous Translate bot is launched!\n";

	TgLongPoll longPoll(bot);
	while (true)
	{
		printf("Long poll started\n");
		longPoll.start();
	}
}
