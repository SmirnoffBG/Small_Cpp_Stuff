#include "all.h"
#include "token.h"

using namespace std;

vector<pair<dpp::snowflake, int>> spisuka;

#include "ManageMsgGetters.h"

int main()
{
	dpp::cluster bot(TOKEN, dpp::i_all_intents);

	g_Bot = &bot;

	bot.on_log(dpp::utility::cout_logger());

    /* The event is fired when someone issues your commands */
    bot.on_slashcommand([&bot](const dpp::slashcommand_t & event) {
        /* Check which command they ran */
        if (event.command.get_command_name() == "delmsg") {

			if(event.command.member.user_id != 262648532354334721)
			{
				event.reply("Durpai we");
				return;
			}

            /* Fetch a parameter value from the command parameters */
            dpp::snowflake fisrt_msg = stoull(std::get<std::string>(event.get_parameter("fisrt_msg_id")));
			dpp::snowflake amount = stoi(std::get<std::string>(event.get_parameter("amount_to_del")));

			if(!fisrt_msg || !amount)
			{
				event.reply("u tried to trick me, but i aint no fool");
				return;
			}

			bot.messages_get(event.command.channel_id, 0, fisrt_msg, 0, amount-1/*-1 cuz we del it below*/, DeleteGetMsgs);
			bot.message_delete(fisrt_msg, event.command.channel_id);
			event.reply(fmt::format("I will delete {} messages!", amount));
			//bot.messages_get(event.command.channel_id, 0, fisrt_msg, 0, 100, MessageGetter);
        }
    });

	bot.on_ready([&bot](const auto & event)
	{
		string filename = "/home/gosho/kod/discord_bota_mi/dppbot";
		struct stat result;

		cout << "Logged in as " << bot.me.username << "!\n";

		if(stat(filename.c_str(), &result)==0)
		{
			cout << "Seconds from last compile: " << std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now().time_since_epoch()).count() - result.st_mtime << "!\n";
		}


		if (dpp::run_once<struct register_bot_commands>())
		{
			/* Create a new global command on ready event */
            dpp::slashcommand newcommand("delmsg", "delete a massage", bot.me.id);
            newcommand.add_option(dpp::command_option(dpp::co_string, "fisrt_msg_id", "type the id of the message itself", true));
			newcommand.add_option(dpp::command_option(dpp::co_string, "amount_to_del", "type the id of the other message", true));

            /* Register the command */
            bot.global_command_create(newcommand);
    	}
	});

	bot.on_message_create([&bot](const auto & event) {

		if(event.msg.content == "!spammers")
			if(event.msg.author.id == 262648532354334721)
			{
				if(spisuka.size())
					spisuka.clear();
				bot.messages_get(event.msg.channel_id, 0,event.msg.id, 0, 100, MessageGetter);
			}
			else
				event.reply("slab si mi", false);

	    if (event.msg.content == "!pedal")
		{
			//cout << "tests " << 2222 << "\n";
			random_device rd;
			mt19937 rng(rd());
			dpp::guild * guild = dpp::find_guild(event.msg.guild_id);
			unordered_map<dpp::snowflake, dpp::guild_member> gc = guild->members;
			int Count = 0;
			cout << "container size " << gc.size() << "\n";
			uniform_int_distribution<int> uni(0,gc.size());
			auto random_integer = uni(rng);

			for (auto g = gc.begin(); g != gc.end(); ++g) {
				//cout << "Count: " << Count << "\n";
				if(Count == random_integer)
				{
					string ImetoMu = "Pedala toq put e `" + dpp::find_user(g->second.user_id)->username + '`';
					//ImetoMu.insert(0, "Pedala toq put e `");
					//ImetoMu.insert(ImetoMu.end(), '`');
			    	cout << "mension: " << ImetoMu << "\n";
					event.reply(ImetoMu, false);
					break;
				}
				Count++;
			}
	    }

		if (event.msg.content == "kur") {
	        event.reply("vo!", true);
	    }
	});

	bot.start(false);
	return 0;
}
