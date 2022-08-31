
bool Sortation(pair<dpp::snowflake, int> purwiq, pair<dpp::snowflake, int> vtoriq)
{
	return (purwiq.second > vtoriq.second);
}

void DeleteGetMsgs(dpp::confirmation_callback_t Callback)
{
	cout << "v nachaloto na del\n";
	if(Callback.http_info.ratelimit_remaining == 0)
	{
		cout << "sleepnahme za " << Callback.http_info.ratelimit_reset_after<< " sekundi \n";
		this_thread::sleep_for (std::chrono::seconds(Callback.http_info.ratelimit_reset_after));
	}
	if(Callback.is_error())
	{
		cout << "Error:" <<  Callback.get_error().message << "\n";

		return;
	}

	dpp::message_map msg_mapa = get<dpp::message_map>(Callback.value);
	std::vector< dpp::snowflake > vec_messages;
	std::vector< dpp::snowflake >::iterator it_msg;

	it_msg = vec_messages.begin();

	for ( auto it = msg_mapa.begin(); it != msg_mapa.end(); ++it )
	{
		it_msg = vec_messages.insert(it_msg, it->second.id);
		cout << "maikati " << it->second.id << "\n";
	}
	g_Bot->message_delete_bulk(vec_messages, msg_mapa.begin()->second.channel_id);
	cout << "sled delete\n";
}
//****************************************************************************************//
void  MessageGetter(dpp::confirmation_callback_t Callback)
{
	if(Callback.http_info.ratelimit_remaining == 0)
	{
		cout << "sleepnahme za " << Callback.http_info.ratelimit_reset_after<< " sekundi \n";
		this_thread::sleep_for (std::chrono::seconds(Callback.http_info.ratelimit_reset_after));
	}
	if(Callback.is_error())
	{
		cout << "Error:" <<  Callback.get_error().message << "\n";

		return;
	}
	dpp::message_map msg_mapa = get<dpp::message_map>(Callback.value);

	//cout << "mymap.size() is " << msg_mapa.size() << std::endl;

	if(msg_mapa.size() == 0)
		return;

	pair<double, dpp::snowflake> smallest_id = make_pair(
		msg_mapa.begin()->second.get_creation_time(),
		msg_mapa.begin()->second.id
	);

	bool vector_accessed = false;
	int i;

	for ( auto it = msg_mapa.begin(); it != msg_mapa.end(); ++it )
	{
		if(it->second.get_creation_time() < smallest_id.first || smallest_id.first == 0.0)
		{
			smallest_id.first = it->second.get_creation_time();
			smallest_id.second = it->second.id;
		}

		for(i=0; i < spisuka.size(); i++)
		{
			if(spisuka[i].first == it->second.author.id)
			{
				vector_accessed = true;
				spisuka[i].second++;
			}
		}

		if(!vector_accessed)
		{
			spisuka.push_back(make_pair(it->second.author.id, 1));
		}
		else
			vector_accessed = false;
	}
	//cout << "spisuka.size() is " << spisuka.size() << std::endl;

	for (std::vector<pair<dpp::snowflake, int>>::iterator it = spisuka.begin() ; it != spisuka.end(); ++it)
	{
    	std::cout << ' ' << dpp::find_user(it->first)->username << "(" << it->second << ")\n";
	}
  	std::cout << '\n';
	//cout << "msg_mapa.size() " << msg_mapa.size() << std::endl;
	if(msg_mapa.size() == 100)
	{
		cout << "msg id " << smallest_id.second << std::endl;
		g_Bot->messages_get(
			msg_mapa.begin()->second.channel_id,
			0,
			smallest_id.second,
			0,
			100,
			MessageGetter
		);
	}
	else
	{
		sort(spisuka.begin(), spisuka.end(), Sortation);

		string listinga = "```";

		//for(i=0; i < spisuka.size(); i++)
		for (auto it = spisuka.begin() ; it != spisuka.end(); ++it)
		{
			cout << "v loopa " << it->first << " | " << it->second << "\n";
			listinga += dpp::find_user(it->first)->username + "(" + to_string(it->second)  + ")" + '\n';
		}

		listinga += "```";
		g_Bot -> message_create(dpp::message(msg_mapa.begin()->second.channel_id, listinga));
	}
}
