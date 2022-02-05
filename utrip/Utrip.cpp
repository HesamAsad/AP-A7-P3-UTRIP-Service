#include "Utrip.h"
#include "Exceptions.h"
#include "Reservation.h"
#include "Handlers.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <iostream>
#include <numeric>
using namespace std;

vector<string> Utrip::split(const string &str, char delim)
{
	size_t start_pos = 0, delim_pos;
	string sub;

	vector<string> splitted;

	while ((delim_pos = str.find(delim, start_pos)) != string::npos)
	{
		sub = str.substr(start_pos, delim_pos - start_pos);
		splitted.push_back(sub);
		start_pos = delim_pos + 1;
	}

	if (str[start_pos] != '\0')
	{
		sub = str.substr(start_pos);
		splitted.push_back(sub);
	}

	return splitted;
}

void Utrip::load_hotels(string assets_path)
{
	ifstream infile(assets_path);
	string buffer;
	getline(infile, buffer);

	while (getline(infile, buffer))
	{
		vector<string> data = split(buffer, CSV_DELIM);
		Rooms_info rooms_info = Room::read_rooms_info(data);
		vector<Room*> loaded_rooms = read_rooms(rooms_info);

		hotels.push_back(Hotel(data[ID], data[NAME],
			stoi(data[STARS]), data[OVERVIEW],
			data[AMENITIES], data[CITY],
			stod(data[LATITUDE]), stod(data[LONGTITUDE]),
			data[IMAGE_URL], rooms_info, loaded_rooms));
	}
}

std::vector<Room*> Utrip::read_rooms(Rooms_info& info)
{
	int i;
	vector<Room*> rooms;

	for (i = 0; i < info.std_count; i++)
		rooms.push_back(new Std_room(info.std_price));

	for (i = 0; i < info.deluxe_count; i++)
		rooms.push_back(new Deluxe_room(info.deluxe_price));

	for (i = 0; i < info.lux_count; i++)
		rooms.push_back(new Lux_room(info.lux_price));

	for (i = 0; i < info.prem_count; i++)
		rooms.push_back(new Prem_room(info.prem_price));

	Room::reset_uid_num();
	return rooms;
}

void Utrip::signup_user(map<string, string> arguments)
{	
	for(auto argument : arguments)
		if(argument.second.empty())
			throw Bad_request();
	
	string username, email, pass;
	for (auto argument = arguments.begin(); argument != arguments.end(); argument++)
	{
		string key = argument->first, value = argument->second;
		if (key == PASS_ARG)
			pass = value;
		else if (key == USERNAME_ARG)
			username = value;
		else if (key == EMAIL_ARG)
			email = value;
		else
			throw Bad_request();
	}
	if (user_exists(email, username))
		throw Bad_request();
	users[to_string(session_id)] = (new User(username, email, pass));
	all_users.push_back(users[to_string(session_id)]);
	delete_filter(to_string(session_id));
	COMMAND_EXECUTED;
}

void Utrip::login_user(string sessionId, map<string, string> arguments)
{
	string username, pass;
	for (auto argument = arguments.begin(); argument != arguments.end(); argument++)
	{
		string key = argument->first, value = argument->second;
		if (key == PASS_ARG)
			pass = value;
		else if (key == USERNAME_ARG)
			username = value;
		else
			throw Bad_request();
	}
	User* user = find_user(username);
	if (!user->is_password_true(pass))
		throw Bad_request();
	users[sessionId] = user;
	delete_filter(sessionId);
	COMMAND_EXECUTED;
}

User* Utrip::find_user(string username)
{
	for (auto user : all_users)
		if (user->get_username() == username)
			return user;
	throw Bad_request();
}

void Utrip::add_to_wallet(string sessionId, double amount)
{
	if (amount <= 0)
		throw Bad_request();

	users[sessionId]->add_to_wallet(amount);
	COMMAND_EXECUTED;
}

bool Utrip::user_exists(string email, string username)
{
	for (auto user : all_users)
		if (user->get_email() == email || user->get_username() == username)
			return true;
	return false;
}

void Utrip::logout_user(string sessionId)
{
	users.erase(sessionId);
	filtered_hotels.erase(sessionId);
	COMMAND_EXECUTED;
}

void Utrip::stars_filter(string sessionId, int min, int max)
{
	if (min > max || min < 1 || max > 5)
		throw Bad_request();
	Stars_filter* filter = new Stars_filter(min, max);
	filtered_hotels[sessionId] = filter->apply(hotels);
	delete filter;
}

void Utrip::delete_filter(string sessionId)
{
	vector<Hotel*> filtered_hotel;
	for(Hotel& hotel : hotels)
		filtered_hotel.push_back(&hotel);
	filtered_hotels[sessionId] = filtered_hotel;
	COMMAND_EXECUTED;
}

Utrip::~Utrip()
{
	for (Hotel& hotel : hotels)
		hotel.clear();
	for (auto user : users)
		delete user.second;
}

void Utrip::check_room_type(string type)
{
	if(type != STDROOMTYPE && type != DELUXEROOMTYPE && type != LUXROOMTYPE && type != PREMROOMTYPE)
		throw Bad_request();
}

Hotel* Utrip::find_hotel(string uid)
{
	for (Hotel& hotel : hotels)
		if (hotel.get_uid() == uid)
			return &hotel;
	throw Not_found();
}

string Utrip::get_wallet(std::string sessionId, string count)
{
	return count.empty() ? users[sessionId]->get_wallet(50) : users[sessionId]->get_wallet(stoi(count));
}

vector<Hotel*> Utrip::get_printable_hotels(string sessionId)
{
	return filtered_hotels[sessionId];
}

void Utrip::load_ratings(string assets_path)
{
	ifstream infile(assets_path);
	string buffer;
	getline(infile, buffer);

	while (getline(infile, buffer))
	{
		vector<string> data = split(buffer, CSV_DELIM);
		Hotel* hotel = find_hotel(data[HOTEL_ID]);
		hotel->add_rating(new Rating(stod(data[RATING_LOCATION]), stod(data[RATING_CLEANLINESS]),
			stod(data[RATING_STAFF]), stod(data[RATING_FACILITIES]), stod(data[RATING_V_FOR_MONEY]),
			stod(data[RATING_OVERALL])));
	}
}

string Utrip::truncate(string num)
{
	string truncated = "";
	unsigned int i;
	for (i = 0; i < num.size() && i < 4; i++)
		truncated += num[i];
	if (i == 1)
		truncated += ".00";
	else if (i == 3)
		truncated += "0";
	return truncated;
}

vector<Hotel*> Utrip::get_all_hotels()
{
	vector<Hotel*> all_hotels;
	for (Hotel& hotel : hotels)
		all_hotels.push_back(&hotel);
	return all_hotels;
}

int Utrip::session_id = 1;

string Utrip::get_session()
{
	return to_string(session_id++);
}