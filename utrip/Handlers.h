#ifndef HANDLERS_H
#define HANDLERS_H

#include "Utrip.h"
#include "../server/server.hpp"
#include <map>
#include <string>

#define ARGUMENT_DELIM '?'
#define EMPTY_LINE "\n"

const std::string GET_CMD = "GET";
const std::string POST_CMD = "POST";
const std::string DELETE_CMD = "DELETE";
const std::string SIGNUP_CMD = "signup";
const std::string LOGIN_CMD = "login";
const std::string LOGOUT_CMD = "logout";
const std::string WALLET_CMD = "wallet";
const std::string FILTER_CMD = "filters";
const std::string RESERVE_CMD = "reserves";
const std::string HOTELS_CMD = "hotels";
const std::string COMMENT_CMD = "comments";
const std::string RATING_CMD = "ratings";
const std::string AMOUNT_ARG = "amount";
const std::string ID_ARG = "id";
const std::string COUNT_ARG = "count";
const std::string ACTIVATION_ARG = "active";
const std::string ORDER_ARG = "order";
const std::string PROPERTY_ARG = "property";
const std::string ASCENDING_TYPE = "ascending";
const std::string DESCENDING_TYPE = "descending";
const std::string CITY_FILTER = "city";
const std::string MIN_STAR_FILTER = "min_star";
const std::string MAX_STAR_FILTER = "max_star";
const std::string MIN_PRICE_FILTER = "min_price";
const std::string MAX_PRICE_FILTER = "max_price";
const std::string DEFAULT_PRICE_FILTER_CMD = "default_price_filter";
const std::string SORT_CMD = "sort";
const std::string MANUAL_WEIGHTS_CMD = "manual_weights";
const std::string ESTIMATED_WEIGHTS_CMD = "estimated_weights";
const std::string TRUE = "true";
const std::string FALSE = "false";

class MainHandler
{
public:
	MainHandler(Utrip* _utrip, Server* _server) : utrip(_utrip), server(_server) {}
	void execute();
protected:
	Utrip *utrip;
	Server *server;
};

class SignUp : public RequestHandler
{
public:
	SignUp(Utrip* _utrip) : utrip(_utrip) {}
	Response *callback(Request* req);

private:
	Utrip* utrip;
};

class LoginHandler : public RequestHandler
{
public:
	LoginHandler(Utrip* _utrip) : utrip(_utrip) {}
	Response *callback(Request *req);

private:
	Utrip *utrip;
};

class HomePage : public RequestHandler
{
public:
	HomePage(Utrip* _utrip) : utrip(_utrip) {}
	Response* callback(Request* req);
private:
	Utrip* utrip;
};

class HotelHandler : public RequestHandler
{
public:
	HotelHandler(Utrip* _utrip) : utrip(_utrip) {}
	Response* callback(Request* req);
private:
	Utrip* utrip;
};

class LogoutHandler : public RequestHandler
{
public:
	LogoutHandler(Utrip* _utrip) : utrip(_utrip) {}
	Response* callback(Request* req);
private:
	Utrip* utrip;
};

class WalletHandler : public RequestHandler
{
public:
	WalletHandler(Utrip* _utrip) : utrip(_utrip) {}
	Response* callback(Request* req);
private:
	Utrip* utrip;
};

class WalletAdder : public RequestHandler
{
public:
	WalletAdder(Utrip* _utrip) : utrip(_utrip) {}
	Response* callback(Request* req);
private:
	Utrip* utrip;
};

class StarFilterHandler : public RequestHandler
{
public:
	StarFilterHandler(Utrip* _utrip) : utrip(_utrip) {}
	Response* callback(Request* req);
private:
	Utrip* utrip;
};

#endif